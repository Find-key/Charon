#pragma once

/*
 * 定义 windows 相关类型
 */

#include "Common.h"

// 1. 128 位浮点/向量寄存器结构 (用于保存 XMM 寄存器)
#pragma pack(push, 16)
struct M128A {
    unsigned __int64 Low;
    __int64          High;
};
#pragma pack(pop)

// 2. 扩展保存区域结构 (XSAVE / FXSAVE 兼容格式)
struct XMM_SAVE_AREA32 {
    uint16_t  ControlWord;
    uint16_t  StatusWord;
    uint8_t  TagWord;
    uint8_t  Reserved1;
    uint16_t  ErrorOpcode;
    uint32_t ErrorOffset;
    uint16_t  ErrorSelector;
    uint16_t  Reserved2;
    uint32_t DataOffset;
    uint16_t  DataSelector;
    uint16_t  Reserved3;
    uint32_t MxCsr;
    uint32_t MxCsr_Mask;
    struct M128A FloatRegisters[8];
    struct M128A XmmRegisters[16];
    uint8_t  Reserved4[96];
};

// 3. x64 完整的 CONTEXT 结构体
// 必须严格 16 字节对齐，且总大小固定为 0x4D0 字节
__declspec(align(16))
struct CONTEXT {
    // 寄存器参数区 (Home Addresses)
    uint64_t P1Home;
    uint64_t P2Home;
    uint64_t P3Home;
    uint64_t P4Home;
    uint64_t P5Home;
    uint64_t P6Home;

    // 控制标志 (指定哪些寄存器组有效)
    uint32_t ContextFlags;
    uint32_t MxCsr;

    // 段寄存器
    uint16_t SegCs;
    uint16_t SegDs;
    uint16_t SegEs;
    uint16_t SegFs;
    uint16_t SegGs;
    uint16_t SegSs;
    uint32_t EFlags;

    // 调试寄存器
    uint64_t Dr0;
    uint64_t Dr1;
    uint64_t Dr2;
    uint64_t Dr3;
    uint64_t Dr6;
    uint64_t Dr7;

    // 通用整型寄存器
    uint64_t Rax;
    uint64_t Rcx;
    uint64_t Rdx;
    uint64_t Rbx;
    uint64_t Rsp; // 偏移 0x98
    uint64_t Rbp;
    uint64_t Rsi;
    uint64_t Rdi;
    uint64_t R8;
    uint64_t R9;
    uint64_t R10;
    uint64_t R11;
    uint64_t R12;
    uint64_t R13;
    uint64_t R14;
    uint64_t R15;

    // 程序计数器
    uint64_t Rip; // 偏移 0xF8

    // 浮点与 SIMD 寄存器状态
    union {
        struct XMM_SAVE_AREA32 FltSave;
        struct {
            struct M128A Header[2];
            struct M128A Legacy[8];
            struct M128A Xmm0;
            struct M128A Xmm1;
            struct M128A Xmm2;
            struct M128A Xmm3;
            struct M128A Xmm4;
            struct M128A Xmm5;
            struct M128A Xmm6;
            struct M128A Xmm7;
            struct M128A Xmm8;
            struct M128A Xmm9;
            struct M128A Xmm10;
            struct M128A Xmm11;
            struct M128A Xmm12;
            struct M128A Xmm13;
            struct M128A Xmm14;
            struct M128A Xmm15;
        } s;
    } DUMMYUNIONNAME;

    // 向量寄存器
    struct M128A VectorRegister[26];
    uint64_t VectorControl;

    // 特殊控制寄存器
    uint64_t DebugControl;
    uint64_t LastBranchToRip;
    uint64_t LastBranchFromRip;
    uint64_t LastExceptionToRip;
    uint64_t LastExceptionFromRip;
};

