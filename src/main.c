#include <stdio.h>

#include "PEBWalk.h"
#include "Syscalls.h"

void Test_PEBWalk() {
	uint8_t* pKernel32 = GetDLLBaseAddrByName("kernel32.dll");
	if (pKernel32 == 0) {
		printf("Can not find!");
		return 0;
	}

	void* fnSetConsoleTitleA = GetFuncAddrByName(pKernel32, "SetConsoleTitleA");

	((void (*)(char*))fnSetConsoleTitleA)("Kernel32 Demo - Console Title Changed!");
}



int main() {
	PLIInit();

	uint8_t* pNt = GetDLLBaseAddrByName("ntdll.dll");
	uint8_t* pNtReadFile = GetFuncAddrByName(pNt, "NtReadFile");
	uint16_t result = -1;
	if (GetSSN(pNtReadFile, &result)) {
		printf("NtReadFile SSN: %hd\n", result);
	}


	system("pause");
	return 0;
}