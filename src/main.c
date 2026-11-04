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


void RunMockTests() {
    // 用例 1: 标准 NT Syscall (SSN = 0x0018)
    // 4C 8B D1 | B8 18 00 00 00 | 0F 05 | C3
    uint8_t stub_valid[] = {
        0x4c, 0x8b, 0xd1,             // mov r10, rcx
        0xb8, 0x18, 0x00, 0x00, 0x00, // mov eax, 0x0018
        0x0f, 0x05,                   // syscall
        0xc3                          // ret
    };
    assert(GetSSN(stub_valid) == 0x0018);
    printf("[PASS] Test Valid SSN (0x0018)\n");

    // 用例 2: 高位非零的 SSN (SSN = 0x0123)
    // 4C 8B D1 | B8 23 01 00 00 | C3
    uint8_t stub_high_byte[] = {
        0x4c, 0x8b, 0xd1,
        0xb8, 0x23, 0x01, 0x00, 0x00,
        0xc3
    };
    assert(GetSSN(stub_high_byte) == 0x0123);
    printf("[PASS] Test Multi-byte SSN (0x0123)\n");

    // 用例 3: 匹配指令前存在前置指令 (例如 nop 填充)
    uint8_t stub_offset[] = {
        0x90, 0x90,                   // nop; nop
        0x4c, 0x8b, 0xd1,
        0xb8, 0x3f, 0x00, 0x00, 0x00,
        0xc3
    };
    assert(GetSSN(stub_offset) == 0x003f);
    printf("[PASS] Test Offset Matching (0x003f)\n");

    // 用例 4: 不匹配特征码（普通非 syscall 函数）
    uint8_t stub_no_match[] = {
        0x48, 0x83, 0xec, 0x28,       // sub rsp, 0x28
        0x48, 0x83, 0xc4, 0x28,       // add rsp, 0x28
        0xc3                          // ret
    };
    assert(GetSSN(stub_no_match) == 0);
    printf("[PASS] Test Non-Syscall Function (returns 0)\n");
}


int main() {
	PLIInit();

    RunMockTests();

	system("pause");
	return 0;
}