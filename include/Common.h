#pragma once

/*
 * 定义全局常量与全局宏
 */

#include "Types.h"

#define ResolveByName(funcname, pmodule)                                    \
    API.funcname = (pfn##funcname)GetFuncAddrByName((pmodule), #funcname)