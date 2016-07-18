#include "IOTimer.h"
#include "Logger.h"

using namespace Lynx;

IOTimer::IOTimer(IOService* ioService) 
: mIOService(ioService), mArgs(0)
{

}

IOTimer::~IOTimer()
{

}

bool 
IOTimer::set(UInt32 duration/*ms*/, const IOTimerCallback& cb, void* args)
{
    mOnTimer = cb; 
    mArgs = args;

    mTimerEvent.mDuration.tv_sec = duration / 1000;
    mTimerEvent.mDuration.tv_usec = (duration % 1000) * 1000;
    mTimerEvent.mOwner = this;
    mTimerEvent.mEventType = IOSERVICE_TIMEOUT_EVENT;
    mTimerEvent.mCallback = &onTimer;

    if (mIOService->addEvent(&mTimerEvent) < 0)
    {
        LOG_WARN("Failed to add timer event.");
        return false;
    }

    return true;
}

void 
IOTimer::cancel()
{
    mIOService->delEvent(&mTimerEvent);
}

void 
IOTimer::onTimer(IOEvent* ev, UInt32 eventType)
{
    if (eventType & IOSERVICE_TIMEOUT_EVENT)
    {
        IOTimer* timer = (IOTimer*)ev->mOwner;
        if (timer == NULL)
        {
            LOG_WARN("Exception for invalid timer pointer");
            return;
        }

        if (timer->mOnTimer(timer->mArgs))
        {
            timer->mIOService->addEvent(&timer->mTimerEvent);
        }
    }
    else
    {
        LOG_WARN("Exception for unknown logic.");
        ASSERT(0);
    }
}


