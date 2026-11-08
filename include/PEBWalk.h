#pragma once
#include "Common.h"

uint8_t* pNtdll;
uint8_t* pKernel32;

void PLIInit();
uint8_t* GetDLLBaseAddrByName(const char* dllname);
uint8_t* GetDLLBaseAddrByHash(uint64_t hash);
uint8_t* GetFuncAddrByName(uint8_t* dll, const char* funcname);
uint8_t* GetFuncAddrByHash(uint8_t* dll, uint64_t hash);
uint8_t* GetFuncAddrByOrdinal(uint8_t* dll, uint32_t ordinal);