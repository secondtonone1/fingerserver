#ifndef __LYNX_CORE_LIB_TIMER_H__
#define __LYNX_CORE_LIB_TIMER_H__

#include "Delegate.h"
#include "IOService.h"

namespace Lynx
{
    typedef Delegate<bool (void*)> IOTimerCallback;

    class IOTimer
    {
    public:
        IOTimer(IOService* ioService);
        virtual ~IOTimer();

        friend class IOService;

        IOService* getIOService() { return mIOService; }

        bool set(UInt32 duration/*ms*/, const IOTimerCallback& cb, void* args);
        void cancel();

    private:
        static void onTimer(IOEvent* ev, UInt32 eventType);

    private:
        IOService*          mIOService;
        IOTimerCallback     mOnTimer;      
        void*               mArgs;
        IOEvent             mTimerEvent;
    };
} // namespace Lynx

#endif // __LYNX_CORE_LIB_TIMER_H__


