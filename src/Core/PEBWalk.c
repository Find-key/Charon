#include "PEBWalk.h"

uint8_t* PEBAddress = 0;
uint8_t* LdrAddress = 0;
uint8_t* InLoadOrderModuleListAddress = 0;

/**
 * @brief 初始化 PEB、Ldr、 InLoadOrderModuleList 地址
 */
void PLIInit() {
	PEBAddress = (uint8_t*)__readgsqword(0x60);
	LdrAddress = *(uint8_t**)(PEBAddress + 0x18);
	InLoadOrderModuleListAddress = *(uint8_t**)(LdrAddress + 0x10);

	pNtdll = GetDLLBaseAddrByName("ntdll.dll");
	pKernel32 = GetDLLBaseAddrByName("kernel32.dll");
}


uint8_t* GetNextModule(uint8_t* ModuleAddress) {
	return *(uint8_t**)(ModuleAddress);
}


/**
 * @brief 通过动态链接库（DLL) 的名称来获取在内存中的基地址
 * 
 * @details 不断比对 InLoadOrderModuleList 中每一项的 BaseDllName 的值
 * 
 * @param[in]  dllname  动态链接库的名称
 * 
 * @return uint8_t*  成功返回动态链接库地址，失败返回 0
 */
uint8_t* GetDLLBaseAddrByName(const char* dllname) {
	uint8_t* ModuleAddress = InLoadOrderModuleListAddress;
	do {
		if (CmpStrAndUniIgnCase(dllname, *(char**)(ModuleAddress + 0x60))) {
			return *(uint8_t**)(ModuleAddress + 0x30);
		}
		ModuleAddress = GetNextModule(ModuleAddress);
	} while (ModuleAddress != InLoadOrderModuleListAddress);

	return 0;
}


/**
 * @brief 通过动态链接库（DLL) 名称的哈希值来获取在内存中的基地址
 *
 * @param[in]  hash  动态链接库名称的哈希值
 *
 * @return uint8_t*  成功返回动态链接库地址，失败返回 0
 */
uint8_t* GetDLLBaseAddrByHash(uint64_t hash) {
	uint8_t* ModuleAddress = InLoadOrderModuleListAddress;
	do {
		if (hash == HashUni(*(char**)(ModuleAddress + 0x60))) {
			return *(uint8_t**)(ModuleAddress + 0x30);
		}
		ModuleAddress = GetNextModule(ModuleAddress);
	} while (ModuleAddress != InLoadOrderModuleListAddress);

	return 0;
}


/**
 * @brief 根据函数名称从动态链接库中获取函数地址
 * 
 * @details 解析动态链接库的导出地址表，比对导出的函数名
 *
 * @param[in]  dll  动态链接库地址
 * @param[in]  funcname  函数名称
 *
 * @return uint8_t*  成功返回函数地址，失败返回 0
 */
uint8_t* GetFuncAddrByName(uint8_t* dll, const char* funcname) {
	uint8_t* NTHeaderAddress = *(uint32_t*)(dll + 0x3C) + dll;
	uint8_t* EATAddress = *(uint32_t*)(NTHeaderAddress + 0x88) + dll;
	uint32_t EATSize = *(uint32_t*)(NTHeaderAddress + 0x8C);

	uint32_t NumberOfNames = *(uint32_t*)(EATAddress + 0x18);
	uint32_t* AddressOfFunctions = (uint32_t*)(*(uint32_t*)(EATAddress + 0x1C) + dll);
	uint32_t* AddressOfNames = (uint32_t*)(*(uint32_t*)(EATAddress + 0x20) + dll);
	uint16_t* AddressOfNameOrdinals = (uint16_t*)(*(uint32_t*)(EATAddress + 0x24) + dll);

	for (int i = 0; i < NumberOfNames; i++) {
		if (CmpStrsIgnCase(funcname, (char*)(AddressOfNames[i] + dll))) {
			uint8_t* addr = (uint8_t*)(AddressOfFunctions[AddressOfNameOrdinals[i]] + dll);
			if (EATAddress <= addr && addr <= EATAddress + EATSize) {
				// TODO: 导入转发
				return 0;
			}

			return addr;
		}
	}

	return 0;
}



/**
 * @brief 根据函数名称的哈希值从动态链接库中获取函数地址
 *
 * @param[in]  dll  动态链接库地址
 * @param[in]  hash  函数名称的哈希值
 *
 * @return uint8_t*  成功返回函数地址，失败返回 0
 */
uint8_t* GetFuncAddrByHash(uint8_t* dll, uint64_t hash) {
	uint8_t* NTHeaderAddress = *(uint32_t*)(dll + 0x3C) + dll;
	uint8_t* EATAddress = *(uint32_t*)(NTHeaderAddress + 0x88) + dll;
	uint32_t EATSize = *(uint32_t*)(NTHeaderAddress + 0x8C);

	uint32_t NumberOfNames = *(uint32_t*)(EATAddress + 0x18);
	uint32_t* AddressOfFunctions = (uint32_t*)(*(uint32_t*)(EATAddress + 0x1C) + dll);
	uint32_t* AddressOfNames = (uint32_t*)(*(uint32_t*)(EATAddress + 0x20) + dll);
	uint16_t* AddressOfNameOrdinals = (uint16_t*)(*(uint32_t*)(EATAddress + 0x24) + dll);

	for (int i = 0; i < NumberOfNames; i++) {
		if (hash == HashUni(AddressOfNames[i] + dll)) {
			uint8_t* addr = (uint8_t*)(AddressOfFunctions[AddressOfNameOrdinals[i]] + dll);
			if (EATAddress <= addr && addr <= EATAddress + EATSize) {
				// TODO: 导入转发
				return 0;
			}

			return addr;
		}
	}

	return 0;
}


/**
 * @brief 根据导出函数的序号从动态链接库中获取函数地址
 *
 * @param[in]  dll  动态链接库地址
 * @param[in]  ordinal  导出函数的序号（从 1 开始）
 *
 * @return uint8_t*  成功返回函数地址，失败返回 0
 */
uint8_t* GetFuncAddrByOrdinal(uint8_t* dll, uint32_t ordinal) {
	uint8_t* NTHeaderAddress = *(uint32_t*)(dll + 0x3C) + dll;
	uint8_t* EATAddress = *(uint32_t*)(NTHeaderAddress + 0x88) + dll;

	uint32_t NumberOfFunctions = *(uint32_t*)(EATAddress + 0x14);
	if (ordinal == 0 || ordinal > NumberOfFunctions) {
		return 0;
	}

	uint32_t* AddressOfFunctions = (uint32_t*)(*(uint32_t*)(EATAddress + 0x1C) + dll);

	return (uint8_t*)(AddressOfFunctions[ordinal-1] + dll);
}