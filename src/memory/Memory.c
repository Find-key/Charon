#include "Memory.h"
#include "PEBWalk.h"

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
    

    if (API.NtAllocateVirtualMemory(-1, &Address, 0, &MemSize, 0x3000, 0x04) == 0) {
        return Address;
    }

    return 0;
}


/**
 * @brief 修改指定内存权限
 *
 * @param[in]  addr  内存地址
 * @param[in]  size  指定的大小
 * @param[in]  perm  权限
 *
 * @return bool  成功返回 true ，失败返回 false
 */
bool MemChg(uint8_t* addr, uint64_t size, uint32_t perm) {
    uint32_t old = 0;
    uint8_t* Address = addr;
    uint64_t MemSize = size;

    if (API.NtProtectVirtualMemory(-1, &Address, &MemSize, perm, &old) == 0) {
        return true;
    }

    return false;
}


/**
 * @brief 自实现 memcpy
 *
 * @param[in]  taddr  目标地址
 * @param[in]  oaddr  源地址
 * @param[in]  size   指定的大小
 */
void MemCopy(uint8_t* taddr, uint8_t* oaddr, uint64_t size) {
    __movsb((unsigned char*)taddr, (const unsigned char*)oaddr, size);
}


/**
 * @brief 释放申请的内存
 *
 * @param[in]  addr  内存地址
 *
 */
void MemFree(uint8_t** addr, uint64_t* size) {
    API.NtFreeVirtualMemory(-1, (void**)addr, (uint32_t*)size, 0x8000);
}