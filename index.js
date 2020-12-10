"use strict";

const fastlist = require("bindings")("fastlist");

module.exports = {
  getProcessList: fastlist.getProcessList
};

