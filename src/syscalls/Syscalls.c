#include "Syscalls.h"
#include "PEBWalk.h"

/**
 * @brief 初始化 Syscall 的地址
 */
void SysInit() {
	SyscallAddress = 0;
	NtAddress = GetDLLBaseAddrByName("nt.dll");

	for (uint32_t idx = 0; SyscallAddress != 0; idx++) {
		uint8_t* FunctionAddress = GetFuncAddrByOrdinal(NtAddress, idx);
		for (uint32_t i = 0; FunctionAddress[i] != 0xc3; i++) {
			if (*(uint16_t*)&FunctionAddress[i] == 0x50F) {
				SyscallAddress = &FunctionAddress[i];
			}
		}
	}
}


/**
 * @brief 获取系统调用函数中的系统调用号（SSN）
 *
 * @param[in]  funcaddr  系统调用函数函数地址
 *
 * @return uint16_t  成功返回 SNN，失败返回 0
 */
uint16_t GetSSN(uint8_t* funcaddr) {
	for (uint32_t i = 0; i < 32-7 && funcaddr[i] != 0xc3; i++) {

		//    MOV R10, RCX
		//    MOV RCX, <syscall>	
		if (funcaddr[i] == 0x4c
			&& funcaddr[i+1] == 0x8b
			&& funcaddr[i+2] == 0xd1
			&& funcaddr[i+3] == 0xb8
			&& funcaddr[i+6] == 0x00
			&& funcaddr[i+7] == 0x00) {
			uint16_t high = funcaddr[i+5];
			uint16_t low = funcaddr[i+4];
			return (high << 8) | low;
		}
	}

	return 0;
}