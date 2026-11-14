#include <stdio.h>

#include "PEBWalk.h"
#include "Syscalls.h"
#include "SleepObf.h"

void Init() {
	PLIInit();
	
	API.CreateTimerQueue = (pfnCreateTimerQueue)GetFuncAddrByName(pKernel32, "CreateTimerQueue");
	API.NtCreateEvent = (pfnNtCreateEvent)GetFuncAddrByName(pNtdll, "NtCreateEvent");
	API.CreateTimerQueueTimer = (pfnCreateTimerQueueTimer)GetFuncAddrByName(pKernel32, "CreateTimerQueueTimer");
	API.RtlCaptureContext = (pfnRtlCaptureContext)GetFuncAddrByName(pNtdll, "RtlCaptureContext");
	API.WaitForSingleObjectEx = (pfnWaitForSingleObjectEx)GetFuncAddrByName(pKernel32, "WaitForSingleObjectEx");
	API.NtAllocateVirtualMemory = (pfnNtAllocateVirtualMemory)GetFuncAddrByName(pNtdll, "NtAllocateVirtualMemory");
	API.NtProtectVirtualMemory = (pfnNtProtectVirtualMemory)GetFuncAddrByName(pNtdll, "pfnNtProtectVirtualMemory");
	API.NtSetEvent = (pfnNtSetEvent)GetFuncAddrByName(pNtdll, "NtSetEvent");
	API.VirtualProtect = (pfnVirtualProtect)GetFuncAddrByName(pKernel32, "VirtualProtect");
	API.Sleep = (pfnSleep)GetFuncAddrByName(pKernel32, "Sleep");
	API.RtlCreateTimerQueue = (pfnRtlCreateTimerQueue)GetFuncAddrByName(pNtdll, "RtlCreateTimerQueue");
	API.RtlCreateTimer = (pfnRtlCreateTimer)GetFuncAddrByName(pNtdll, "RtlCreateTimer");
	API.RtlDeleteTimerQueueEx = (pfnRtlDeleteTimerQueueEx)GetFuncAddrByName(pNtdll, "RtlDeleteTimerQueueEx");
	API.NtContinue = (pfnNtContinue)GetFuncAddrByName(pNtdll, "NtContinue");
	API.RtlDeleteTimerQueue = (pfnRtlDeleteTimerQueue)GetFuncAddrByName(pNtdll, "RtlDeleteTimerQueue");
	API.NtClose = (pfnNtClose)GetFuncAddrByName(pNtdll, "NtClose");
	API.NtFreeVirtualMemory = (pfnNtFreeVirtualMemory)GetFuncAddrByName(pNtdll, "NtFreeVirtualMemory");
	API.NtSignalAndWaitForSingleObject = (pfnNtSignalAndWaitForSingleObject)GetFuncAddrByName(pNtdll, "NtSignalAndWaitForSingleObject");
	API.RtlExitUserThread = (pfnRtlExitUserThread)GetFuncAddrByName(pNtdll, "RtlExitUserThread");
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
		SleepObf(10000);
	}

	return 0;
}