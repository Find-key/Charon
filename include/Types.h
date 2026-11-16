#pragma once

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef enum {
    false = 0,
    true = 1
} bool;

typedef enum {
    NotificationEvent,      /* 对应 Win32 的 Manual-Reset Event (手动重置) */
    SynchronizationEvent    /* 对应 Win32 的 Auto-Reset Event (自动重置) */
} EVENT_TYPE;

typedef struct {
    uint16_t Length;
    uint16_t MaximumLength;
    char* Buffer;
} USTRING;

typedef struct {
    uint16_t Length;
    uint16_t MaximumLength;
    uint16_t* Buffer;
} UNICODE_STRING;

// 128 位浮点/向量寄存器结构 (用于保存 XMM 寄存器)
#pragma pack(push, 16)
struct M128A {
    unsigned __int64 Low;
    __int64          High;
};
#pragma pack(pop)

// 扩展保存区域结构 (XSAVE / FXSAVE 兼容格式)
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
    uint64_t Rsp;
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
    uint64_t Rip;

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

void __movsb(unsigned char* Destination, const unsigned char* Source, unsigned __int64 Count);
#pragma intrinsic(__movsb)

typedef void(__stdcall* WAITORTIMERCALLBACK)(
    void* Parameter,
    unsigned char TimerOrWaitFired
    );

typedef int(__stdcall* pfnRtlCreateTimerQueue)(
    void** TimerQueueHandle
    );

typedef int(__stdcall* pfnRtlCreateTimer)(
    void* TimerQueue,
    void** NewTimer,
    WAITORTIMERCALLBACK Callback,
    void* Parameter,
    uint32_t DueTime,
    uint32_t Period,
    uint32_t Flags
    );

typedef int(__stdcall* pfnRtlDeleteTimerQueue)(
    void* TimerQueueHandle
    );

typedef void* (__stdcall* pfnCreateTimerQueue)(void);

typedef int(__stdcall* pfnCreateTimerQueueTimer)(
    void** phNewTimer,     // 输出参数：接收创建出的定时器句柄
    void* TimerQueue,     // 定时器队列句柄（NULL 表示使用默认进程队列）
    WAITORTIMERCALLBACK   Callback,       // 到期触发的回调函数指针
    void* Parameter,      // 传递给回调函数的上下文参数
    unsigned long         DueTime,        // 首次触发前的延迟时间（毫秒）
    unsigned long         Period,         // 后续周期触发间隔（毫秒；若为 0 则仅单次触发）
    unsigned long         Flags           // 控制执行策略的标志位（WT_*）
    );

typedef bool(__stdcall* pfnDeleteTimerQueueTimer)(
    void* TimerQueue,
    void* Timer,
    void* CompletionEvent
    );

typedef bool(__stdcall* pfnDeleteTimerQueue)(
    void* TimerQueue
    );

typedef void(__stdcall* pfnRtlCaptureContext)(
    struct CONTEXT* ContextRecord
    );

typedef uint32_t(__stdcall* pfnNtCreateEvent)(
    void** EventHandle,
    uint32_t            DesiredAccess,
    uint8_t* ObjectAttributes,
    EVENT_TYPE          EventType,
    bool                InitialState
    );

typedef int(__stdcall* pfnNtClose)(
    void* Handle
    );

typedef uint32_t(__stdcall* pfnWaitForSingleObjectEx)(
    void* hHandle,
    uint32_t    dwMilliseconds,
    bool        bAlertable
    );

typedef int(__stdcall* pfnNtSignalAndWaitForSingleObject)(
    void* SignalHandle,
    void* WaitHandle,
    bool          Alertable,
    uint64_t* Timeout
    );

typedef uint32_t(__stdcall* pfnNtSetEvent)(
    void* EventHandle,
    uint32_t* PreviousState
    );

typedef int(__stdcall* pfnNtAllocateVirtualMemory)(
    uint8_t* ProcessHandle,         // 目标进程句柄（自身进程传 (HANDLE)-1 或 NtCurrentProcess()）
    uint8_t** BaseAddress,           // 指向分配基址的指针（输入期望地址；若填 0 则由系统自动选取并写回实际地址）
    uint64_t     ZeroBits,              // 基地址的高位零比特数（通常传 0）
    uint64_t* RegionSize,            // 指向申请大小的指针（系统会自动向上对齐到页面大小，并写回实际分配大小）
    uint32_t     AllocationType,        // 分配类型：MEM_COMMIT (0x1000) | MEM_RESERVE (0x2000)
    uint32_t     Protect                // 内存保护属性：PAGE_READWRITE (0x04)、PAGE_EXECUTE_READWRITE (0x40) 等
    );

typedef int(__stdcall* pfnNtProtectVirtualMemory)(
    uint8_t* ProcessHandle,      // 目标进程句柄（当前进程传 (HANDLE)-1）
    uint8_t** BaseAddress,        // 指向需要修改属性的内存基地址的指针 (输入期望地址，输出按页对齐后的基址)
    uint64_t* RegionSize,         // 指向修改范围大小的指针 (输入期望大小，输出按页对齐后的大小)
    uint32_t    NewProtect,         // 新的保护属性（如 PAGE_EXECUTE_READ = 0x20）
    uint32_t* OldProtect          // 接收旧保护属性的变量指针（必须提供有效指针，否则触发崩溃）
    );

typedef int(__stdcall* pfnVirtualProtect)(
    void* lpAddress,
    uint64_t dwSize,
    uint32_t flNewProtect,
    uint32_t* lpflOldProtect
    );

typedef int(__stdcall* pfnNtFreeVirtualMemory)(
    void* ProcessHandle,
    void** BaseAddress,
    uint32_t* RegionSize,
    uint32_t       FreeType
    );

typedef void(__stdcall* pfnSleep)(
    uint32_t dwMilliseconds
    );

typedef int(__stdcall* pfnNtContinue)(
    struct CONTEXT* ContextRecord,
    bool TestAlert
    );

typedef void(__stdcall* pfnRtlExitUserThread)(
    int ExitStatus
    );

typedef int(__stdcall* pfnRtlDeleteTimerQueueEx)(
    void* TimerQueue,
    void* CompletionEvent
    );

typedef int(__stdcall* pfnLdrGetProcedureAddress)(
    void* DllHandle,
    USTRING* ProcedureName,
    uint64_t        ProcedureNumber,
    void** ProcedureAddress
    );

typedef int(__stdcall* pfnLdrLoadDll)(
    uint16_t* PathToFile,
    uint64_t* DllCharacteristics,
    UNICODE_STRING* DllName,
    void** DllHandle
    );




typedef struct {
    pfnNtCreateEvent NtCreateEvent;
    pfnNtSetEvent NtSetEvent;
    pfnNtClose NtClose;
    pfnNtAllocateVirtualMemory NtAllocateVirtualMemory;
    pfnNtProtectVirtualMemory NtProtectVirtualMemory;
    pfnNtFreeVirtualMemory NtFreeVirtualMemory;
    pfnNtContinue NtContinue;
    pfnNtSignalAndWaitForSingleObject NtSignalAndWaitForSingleObject;

    pfnVirtualProtect VirtualProtect;

    pfnSleep Sleep;

    pfnLdrLoadDll LdrLoadDll;
    pfnLdrGetProcedureAddress LdrGetProcedureAddress;


    pfnCreateTimerQueue CreateTimerQueue;
    pfnCreateTimerQueueTimer CreateTimerQueueTimer;

    pfnWaitForSingleObjectEx WaitForSingleObjectEx;
    pfnRtlCreateTimer RtlCreateTimer;
    pfnRtlCreateTimerQueue RtlCreateTimerQueue;
    pfnRtlDeleteTimerQueue RtlDeleteTimerQueue;
    pfnRtlDeleteTimerQueueEx RtlDeleteTimerQueueEx;
    pfnRtlCaptureContext RtlCaptureContext;
    pfnRtlExitUserThread RtlExitUserThread;
} WIN_API;