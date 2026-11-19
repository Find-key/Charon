#include <stdio.h>

#include "Test.h"
#include "Globals.h"
#include "PEBWalk.h"
#include "SleepObf.h"
#include "StackSpoof.h"

int Test() {
    int failed = 0;

    printf("[1] ntdll base: 0x%p\n", (void*)pNtdll);
    if (!pNtdll) {
        printf("    FAIL: ntdll not found\n");
        failed = 1;
    }

    printf("[2] kernel32 base: 0x%p\n", (void*)pKernel32);
    if (!pKernel32) {
        printf("    FAIL: kernel32 not found\n");
        failed = 1;
    }

    uint8_t* ntReadFile = GetFuncAddrByName(pNtdll, "NtReadFile");
    printf("[3] NtReadFile address: 0x%p\n", (void*)ntReadFile);
    if (!ntReadFile) {
        printf("    FAIL: export not found\n");
        failed = 1;
    }

    for (uint32_t i = 0; i < 3; i++) {
        printf("Sleep loop %d\n", i + 1);
        Parameter p = { 1, 30000 };
        ShadowCall(SleepObf, &p);
    }
    

    printf("SELFTEST: %s\n", failed ? "FAILED" : "PASSED");
    return failed ? 1 : 0;
}