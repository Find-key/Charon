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
 * @brief 通过字节匹配解析系统调用函数中的系统调用号（SSN）
 *
 * @param[in]  funcaddr  系统调用函数函数地址
 * @param[out]  ssn  储存解析结果的指针
 *
 * @return bool  成功返回 1，失败返回 0
 */
bool GetSSN(uint8_t* funcaddr, uint16_t* ssn) {
	
	if (!MatchSSN(funcaddr, ssn)) {
		if (!CalculateSSN(funcaddr, ssn)) {
			return false;
		}
	}

	return true;
}


/**
 * @brief 通过字节匹配解析系统调用函数中的系统调用号（SSN）
 *
 * @param[in]  funcaddr  系统调用函数函数地址
 * @param[out]  ssn  储存解析结果的指针
 *
 * @return bool  成功返回 1，失败返回 0
 */
bool MatchSSN(uint8_t* funcaddr, uint16_t* ssn) {
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
			*ssn = (high << 8) | low;
			return true;
		}
	}

	return false;
}



/**
 * @brief 通过寻找未被 hook 的系统调用函数间接获得系统调用号（SSN）
 *
 * @param[in]  funcaddr  系统调用函数函数地址
 * @param[out]  ssn  储存解析结果的指针
 *
 * @return bool  成功返回 1，失败返回 0
 */
bool CalculateSSN(uint8_t* funcaddr, uint16_t* ssn) {
	uint16_t Result = 0;
	for (uint8_t i = 0; i < 200; i++) {
		uint8_t* Address = funcaddr - 32 * i;
		if (MatchSSN(Address, &Result)) {
			*ssn = Result + i;
			return true;
		}

		Address = funcaddr + 32 * i;
		if (MatchSSN(Address, &Result)) {
			*ssn = Result - i;
			return true;
		}
	}

	return false;
}