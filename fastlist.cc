#include <napi.h>

#include "windows.h"
#include "json.hpp"
#include <TlHelp32.h>
#include <atlbase.h>
#include <Psapi.h>
#include <set>
#include <iostream>

using json = nlohmann::json;
std::set<json> processes;

std::string getLastErrorString()
{
    DWORD errorCode = GetLastError();
    if (!errorCode) {
        return std::string();
    }

    LPSTR buf = NULL;
    DWORD size =
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                      | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    errorCode,
                    LANG_SYSTEM_DEFAULT,
                    (LPSTR)&buf,
                    0,
                    NULL);

    std::string message(buf, size);
    return message;
}

Napi::Value refresh(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return env.Undefined();
}


void populateProcessList() 
{

    if (!processes.empty()) {
        processes.clear();
    }

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE)
        return;

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snap, &entry)) {
        CloseHandle(snap);
        std::cout << "no process" << std::endl;
        return;
    }

    do { 
        CW2A exe((LPCWSTR) entry.szExeFile, CP_UTF8);
        json processEntry;
        processEntry["pid"] = entry.th32ProcessID;
        processEntry["parentPid"] = entry.th32ParentProcessID;
        processEntry["name"] = entry.szExeFile;
        processes.insert(processEntry);

    } while (Process32Next(snap, &entry));
    CloseHandle(snap); 
    return;
}


Napi::Array getProcessList(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        populateProcessList();
    } catch (std::runtime_error& e) {
        throw Napi::Error::New(env, e.what());
    }

    Napi::Array ret = Napi::Array::New(env, processes.size());

    int i = 0;
    for (auto const& process : processes) {
        ret.Set(i++, process.dump());
    }

    return ret;
}


Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("getProcessList", 
                   Napi::Function::New(env, getProcessList, "getProcessList"));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
