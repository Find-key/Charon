#pragma once

/*
 * 定义全局类型与全局变量
 */


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


void __movsb(unsigned char* Destination, const unsigned char* Source, unsigned __int64 Count);
#pragma intrinsic(__movsb)

typedef void(__stdcall* WAITORTIMERCALLBACK)(void* Parameter, unsigned char TimerOrWaitFired);

typedef int (__stdcall* pfnRtlCreateTimerQueue)(
    void** TimerQueueHandle
);

typedef int (__stdcall* pfnRtlCreateTimer)(
    void* TimerQueue,
    void** NewTimer,
    WAITORTIMERCALLBACK Callback,
    void* Parameter,
    uint32_t DueTime,
    uint32_t Period,
    uint32_t Flags
);

typedef int (__stdcall* pfnRtlDeleteTimerQueue)(
    void* TimerQueueHandle
);

typedef void* (__stdcall* pfnCreateTimerQueue)(void);

typedef int (__stdcall* pfnCreateTimerQueueTimer)(
    void** phNewTimer,     // 输出参数：接收创建出的定时器句柄
    void* TimerQueue,     // 定时器队列句柄（NULL 表示使用默认进程队列）
    WAITORTIMERCALLBACK   Callback,       // 到期触发的回调函数指针
    void* Parameter,      // 传递给回调函数的上下文参数
    unsigned long         DueTime,        // 首次触发前的延迟时间（毫秒）
    unsigned long         Period,         // 后续周期触发间隔（毫秒；若为 0 则仅单次触发）
    unsigned long         Flags           // 控制执行策略的标志位（WT_*）
);

typedef bool (__stdcall* pfnDeleteTimerQueueTimer)(
    void* TimerQueue,
    void* Timer,
    void* CompletionEvent
);

typedef bool (__stdcall* pfnDeleteTimerQueue)(
    void* TimerQueue
);

typedef void (__stdcall* pfnRtlCaptureContext)(
    struct CONTEXT* ContextRecord
);

typedef uint32_t (__stdcall* pfnNtCreateEvent)(
    void**              EventHandle,
    uint32_t            DesiredAccess,
    uint8_t*            ObjectAttributes,
    EVENT_TYPE          EventType,
    bool                InitialState
);

typedef int (__stdcall* pfnNtClose)(
    void* Handle
);

typedef uint32_t (__stdcall* pfnWaitForSingleObjectEx)(
    void* hHandle,
    uint32_t    dwMilliseconds,
    bool        bAlertable
);

typedef int (__stdcall* pfnNtSignalAndWaitForSingleObject)(
    void*         SignalHandle,
    void*         WaitHandle,
    bool          Alertable,
    uint64_t*     Timeout
);

typedef uint32_t (__stdcall* pfnNtSetEvent)(
    void* EventHandle,
    uint32_t* PreviousState
);

typedef int (__stdcall* pfnNtAllocateVirtualMemory)(
    uint8_t* ProcessHandle,         // 目标进程句柄（自身进程传 (HANDLE)-1 或 NtCurrentProcess()）
    uint8_t** BaseAddress,           // 指向分配基址的指针（输入期望地址；若填 0 则由系统自动选取并写回实际地址）
    uint64_t     ZeroBits,              // 基地址的高位零比特数（通常传 0）
    uint64_t* RegionSize,            // 指向申请大小的指针（系统会自动向上对齐到页面大小，并写回实际分配大小）
    uint32_t     AllocationType,        // 分配类型：MEM_COMMIT (0x1000) | MEM_RESERVE (0x2000)
    uint32_t     Protect                // 内存保护属性：PAGE_READWRITE (0x04)、PAGE_EXECUTE_READWRITE (0x40) 等
);

typedef int (__stdcall* pfnNtProtectVirtualMemory)(
    uint8_t* ProcessHandle,      // 目标进程句柄（当前进程传 (HANDLE)-1）
    uint8_t** BaseAddress,        // 指向需要修改属性的内存基地址的指针 (输入期望地址，输出按页对齐后的基址)
    uint64_t* RegionSize,         // 指向修改范围大小的指针 (输入期望大小，输出按页对齐后的大小)
    uint32_t    NewProtect,         // 新的保护属性（如 PAGE_EXECUTE_READ = 0x20）
    uint32_t* OldProtect          // 接收旧保护属性的变量指针（必须提供有效指针，否则触发崩溃）
);

typedef int (__stdcall* pfnVirtualProtect)(
    void* lpAddress,
    uint64_t dwSize,
    uint32_t flNewProtect,
    uint32_t* lpflOldProtect
);

typedef int (__stdcall* pfnNtFreeVirtualMemory)(
    void*       ProcessHandle,
    void**      BaseAddress,
    uint32_t*   RegionSize,
    uint32_t       FreeType
);

typedef void (__stdcall* pfnSleep)(
    uint32_t dwMilliseconds
);

typedef int (__stdcall* pfnNtContinue)(
    struct CONTEXT* ContextRecord,
    bool TestAlert
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

    pfnCreateTimerQueue CreateTimerQueue;
    pfnCreateTimerQueueTimer CreateTimerQueueTimer;

    pfnWaitForSingleObjectEx WaitForSingleObjectEx;
    pfnRtlCreateTimer RtlCreateTimer;
    pfnRtlCreateTimerQueue RtlCreateTimerQueue;
    pfnRtlDeleteTimerQueue RtlDeleteTimerQueue;
    pfnRtlCaptureContext RtlCaptureContext;
} WIN_API;
WIN_API API;
uint8_t* ImageAddress;
uint64_t ImageSize;
uint8_t* TextAddress;
uint64_t TextSize;