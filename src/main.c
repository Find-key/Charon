#include <stdio.h>
#include <assert.h>

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

	system("pause");
	return 0;
}