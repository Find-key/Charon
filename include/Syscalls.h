#pragma once

#include "Common.h"

uint8_t* SyscallAddress;
uint8_t* NtAddress;

void SysInit();
bool GetSSN(uint8_t* funcaddr, uint16_t* ssn);