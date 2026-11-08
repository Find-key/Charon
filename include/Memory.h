#pragma once

#include "Common.h"

uint8_t* MemAlloc(uint64_t size);
bool MemChg(uint8_t* addr, uint64_t size, uint8_t perm);