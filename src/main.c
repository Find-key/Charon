#include <windows.h>
#include <stdio.h>

#include "PEBWalk.h"

int main() {
	InitInfo();

	uint8_t* pKernel32 = GetDLLBaseAddrByName("kernel32.dll");
	if (pKernel32 == 0) {
		printf("Can not find!");
		return 0;
	}

	void* fnSetConsoleTitleA = GetFuncAddrByName(pKernel32, "SetConsoleTitleA");
	
	((void (*)(char*))fnSetConsoleTitleA)("Kernel32 Demo - Console Title Changed!");

    system("pause");
    return 0;
}