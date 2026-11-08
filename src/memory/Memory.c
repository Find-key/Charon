#include "Memory.h"
#include "PEBWalk.h"

typedef bool (*pfnNtAllocateVirtualMemory)(
    uint8_t*     ProcessHandle,         // 目标进程句柄（自身进程传 (HANDLE)-1 或 NtCurrentProcess()）
    uint8_t**    BaseAddress,           // 指向分配基址的指针（输入期望地址；若填 0 则由系统自动选取并写回实际地址）
    uint64_t     ZeroBits,              // 基地址的高位零比特数（通常传 0）
    uint64_t*    RegionSize,            // 指向申请大小的指针（系统会自动向上对齐到页面大小，并写回实际分配大小）
    uint32_t     AllocationType,        // 分配类型：MEM_COMMIT (0x1000) | MEM_RESERVE (0x2000)
    uint32_t     Protect                // 内存保护属性：PAGE_READWRITE (0x04)、PAGE_EXECUTE_READWRITE (0x40) 等
);

/**
 * @brief 申请可读可写内存
 *
 * @param[in]  size  申请内存的大小
 *
 * @return uint8_t*  成功返回地址，失败返回 0
 */
uint8_t* MemAlloc(uint64_t size) {
    uint8_t* Address = 0;
    uint64_t MemSize = size;
    pfnNtAllocateVirtualMemory pNtAllocateVirtualMemory = (pfnNtAllocateVirtualMemory)GetFuncAddrByName(pNtdll, "NtAllocateVirtualMemory");

    if (pNtAllocateVirtualMemory && pNtAllocateVirtualMemory(-1, &Address, 0, &MemSize, 0x3000, 0x04) == 0) {
        return Address;
    }

    return 0;
}




typedef bool (*pfnNtProtectVirtualMemory)(
    uint8_t*    ProcessHandle,      // 目标进程句柄（当前进程传 (HANDLE)-1）
    uint8_t**   BaseAddress,        // 指向需要修改属性的内存基地址的指针 (输入期望地址，输出按页对齐后的基址)
    uint64_t*   RegionSize,         // 指向修改范围大小的指针 (输入期望大小，输出按页对齐后的大小)
    uint32_t    NewProtect,         // 新的保护属性（如 PAGE_EXECUTE_READ = 0x20）
    uint32_t*   OldProtect          // 接收旧保护属性的变量指针（必须提供有效指针，否则触发崩溃）
);

/**
 * @brief 修改指定内存权限
 *
 * @param[in]  addr  内存地址
 * @param[in]  size  指定的大小
 * @param[in]  perm  权限
 *
 * @return uint8_t*  成功返回 true ，失败返回 false
 */
bool MemChg(uint8_t* addr, uint64_t size, uint32_t perm) {
    uint32_t old = 0;
    uint8_t* Address = addr;
    uint64_t MemSize = size;
    pfnNtProtectVirtualMemory pNtProtectVirtualMemory = (pfnNtProtectVirtualMemory)GetFuncAddrByName(pNtdll, "pfnNtProtectVirtualMemory");

    if (pNtProtectVirtualMemory && pNtProtectVirtualMemory(-1, &Address, &MemSize, perm, &old) == 0) {
        return true;
    }

    return false;
}