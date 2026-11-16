#include <stdio.h>

#include "PEBWalk.h"
#include "Syscalls.h"
#include "SleepObf.h"
#include "Globals.h"

void Init() {
	PLIInit();

	pNtdll = GetDLLBaseAddrByName("ntdll.dll");
	pKernel32 = GetDLLBaseAddrByName("kernel32.dll");
	
	ResolveByName(CreateTimerQueue, pKernel32);
	ResolveByName(CreateTimerQueueTimer, pKernel32);
	ResolveByName(WaitForSingleObjectEx, pKernel32);
	ResolveByName(VirtualProtect, pKernel32);
	ResolveByName(Sleep, pKernel32);

	ResolveByName(NtCreateEvent, pNtdll);
	ResolveByName(RtlCaptureContext, pNtdll);
	ResolveByName(NtAllocateVirtualMemory, pNtdll);
	ResolveByName(NtProtectVirtualMemory, pNtdll);
	ResolveByName(NtSetEvent, pNtdll);
	ResolveByName(RtlCreateTimerQueue, pNtdll);
	ResolveByName(RtlCreateTimer, pNtdll);
	ResolveByName(RtlDeleteTimerQueueEx, pNtdll);
	ResolveByName(NtContinue, pNtdll);
	ResolveByName(RtlDeleteTimerQueue, pNtdll);
	ResolveByName(NtClose, pNtdll);
	ResolveByName(NtFreeVirtualMemory, pNtdll);
	ResolveByName(NtSignalAndWaitForSingleObject, pNtdll);
	ResolveByName(RtlExitUserThread, pNtdll);
	ResolveByName(LdrLoadDll, pNtdll);
	ResolveByName(LdrGetProcedureAddress, pNtdll);
}

void show() {
	uint8_t* pNtReadFile = GetFuncAddrByName(pNtdll, "NtReadFile");
	uint16_t result = -1;
	if (GetSSN(pNtReadFile, &result)) {
		printf("NtReadFile SSN: %hd\n", result);
	}
}


int main() {
	Init();

	for (;;) {
		show();
		SleepObf(1000);
	}

	return 0;
}