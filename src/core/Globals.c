#include "Globals.h"

WIN_API API;
uint8_t* ImageAddress = 0;
uint64_t ImageSize = 0;
uint8_t* TextAddress = 0;
uint64_t TextSize = 0;

uint8_t* pNtdll = 0;
uint8_t* pKernel32 = 0;
uint8_t* pAdvapi32 = 0;
uint8_t* NtAddress = 0;
uint8_t* SyscallAddress = 0;