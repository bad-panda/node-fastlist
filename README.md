# gyg-fastlist

## Installation

gyg-fastlist is currently only supported on Windows.

````bash
npm install git+https://github.com/bad-panda/node-fastlist.git
yarn add https://github.com/bad-panda/node-fastlist.git
````

## Usage

````js
const fastlist = require("@gifyourgame/fastlist");

for (const process of fastlist.getProcessList()) {
    console.log(`${monitor}`);
}
````

## Docs

* ### `getProcessList()`
  Gets a list of the current connected monitors.
  * #### Return value
    An array of json dumped `String` containing the process name, PID, and parent PID.
    Ex.
    `{"name":"winlogon.exe","parentPid":772,"pid":944},
     {"name":"wlanext.exe","parentPid":684,"pid":5424}`
