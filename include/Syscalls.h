#pragma once

#include "Common.h"

uint8_t* SyscallAddress;
uint8_t* NtAddress;

void SysInit();
uint16_t GetSSN(uint8_t* funcaddr);