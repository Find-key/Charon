#pragma once
#include "Types.h"

bool CmpStrsIgnCase(const char* str1, const char* str2);
bool CmpStrAndUniIgnCase(const char* str, const short* uni);
uint64_t Hash(const char* str);
uint64_t HashUni(const short* str);