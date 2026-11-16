#pragma once

/*
 * 定义全局变量与全局宏
 */

#include "Types.h"

WIN_API API;
uint8_t* ImageAddress;
uint64_t ImageSize;
uint8_t* TextAddress;
uint64_t TextSize;

#define ResolveByName(funcname, pmodule)                                    \
    API.funcname = (pfn##funcname)GetFuncAddrByName((pmodule), #funcname)