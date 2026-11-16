#pragma once
#include "Common.h"

extern WIN_API API;
extern uint8_t* ImageAddress;
extern uint64_t ImageSize;
extern uint8_t* TextAddress;
extern uint64_t TextSize;

extern uint8_t* pNtdll;
extern uint8_t* pKernel32;
extern uint8_t* pAdvapi32;
extern uint8_t* NtAddress;
extern uint8_t* SyscallAddress;