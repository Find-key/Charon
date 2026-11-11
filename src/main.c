#include <stdio.h>

#include "PEBWalk.h"
#include "Syscalls.h"

void Init() {
	PLIInit();
	
	API.CreateTimerQueue = (pfnCreateTimerQueue)GetFuncAddrByName(pNtdll, "CreateTimerQueue");
	API.NtCreateEvent = (pfnNtCreateEvent)GetFuncAddrByName(pNtdll, "NtCreateEvent");
	API.CreateTimerQueueTimer = (pfnCreateTimerQueueTimer)GetFuncAddrByName(pNtdll, "CreateTimerQueueTimer");
	API.RtlCaptureContext = (pfnRtlCaptureContext)GetFuncAddrByName(pNtdll, "RtlCaptureContext");
	API.WaitForSingleObjectEx = (pfnWaitForSingleObjectEx)GetFuncAddrByName(pNtdll, "WaitForSingleObjectEx");
	API.NtAllocateVirtualMemory = (pfnNtAllocateVirtualMemory)GetFuncAddrByName(pNtdll, "NtAllocateVirtualMemory");
	API.NtProtectVirtualMemory = (pfnNtProtectVirtualMemory)GetFuncAddrByName(pNtdll, "pfnNtProtectVirtualMemory");
	API.NtSetEvent = (pfnNtSetEvent)GetFuncAddrByName(pNtdll, "NtSetEvent");
	API.VirtualProtect = (pfnVirtualProtect)GetFuncAddrByName(pNtdll, "VirtualProtect");
	API.Sleep = (pfnSleep)GetFuncAddrByName(pNtdll, "Sleep");
	API.RtlCreateTimerQueue = (pfnRtlCreateTimerQueue)GetFuncAddrByName(pNtdll, "RtlCreateTimerQueue");
	API.RtlCreateTimer = (pfnRtlCreateTimer)GetFuncAddrByName(pNtdll, "RtlCreateTimer");
	API.NtContinue = (pfnNtContinue)GetFuncAddrByName(pNtdll, "NtContinue");
	API.RtlDeleteTimerQueue = (pfnRtlDeleteTimerQueue)GetFuncAddrByName(pNtdll, "RtlDeleteTimerQueue");
	API.NtClose = (pfnNtClose)GetFuncAddrByName(pNtdll, "NtClose");
	API.NtFreeVirtualMemory = (pfnNtFreeVirtualMemory)GetFuncAddrByName(pNtdll, "NtFreeVirtualMemory");
	API.NtSignalAndWaitForSingleObject = (pfnNtSignalAndWaitForSingleObject)GetFuncAddrByName(pNtdll, "NtSignalAndWaitForSingleObject");
}



int main() {
	Init();

	uint8_t* pNt = GetDLLBaseAddrByName("ntdll.dll");
	uint8_t* pNtReadFile = GetFuncAddrByName(pNt, "NtReadFile");
	uint16_t result = -1;
	if (GetSSN(pNtReadFile, &result)) {
		printf("NtReadFile SSN: %hd\n", result);
	}

	system("pause");
	return 0;
}