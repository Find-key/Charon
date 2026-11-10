#pragma once

#include "Common.h"

uint8_t* MemAlloc(uint64_t size);
bool MemChg(uint8_t* addr, uint64_t size, uint32_t perm);
void MemCopy(uint8_t* taddr, uint8_t* oaddr, uint64_t size);