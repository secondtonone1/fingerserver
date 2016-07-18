#include "IOService.h"
#include "Logger.h"

using namespace Lynx;

extern struct IOEventOperator iocpOperator;
extern struct IOEventOperator selectOperator;
extern struct IOEventOperator epollOperator;

IOService::IOService(Int32 type) : mShutdown(false), mPaused(false)
{
#ifdef _WIN32
    switch (type)
    {
    case IOSERVICE_SELECT_DRIVER:
        mIOEventOperator = &selectOperator;
        break;
    //case IOSERVICE_IOCP_DRIVER:
    //    mIOEventOperator = &iocpOperator;
    //    break;
    default:
        ASSERT(0);
    }
#elif defined (__linux__) 
    switch (type)
    {
    case IOSERVICE_SELECT_DRIVER:
        mIOEventOperator = &selectOperator;
        break;
    case IOSERVICE_EPOLL_DRIVER:
        mIOEventOperator = &epollOperator;
        break;
    default:
        ASSERT(0);
    }
#else
    mIOEventOperator = &selectOperator;
#endif

    mOperation = mIOEventOperator->mInitial(this);
    TimeUtil::getTimeOfDay(&mTimeFlag, NULL);
    LOG_INFO("IOEventOperator was implement by %s", mIOEventOperator->mName);
}

IOService::~IOService()
{
    mIOEventOperator->mRelease(mOperation);
}

Int32 
IOService::addEvent(IOEvent* ev)
{
    Int32 result  = 0;
    if (ev->mEventType & IOSERVICE_TIMEOUT_EVENT)
    {
        struct Timeval now;
        TimeUtil::getTimeOfDay(&now, NULL);
        TIMERADD(&now, &ev->mDuration, &ev->mTimeout);
        mTimeoutHeap.push(ev);
        ev->mValid = true;
    }
    else
    {
        result = mIOEventOperator->mAddEvent(ev, mOperation);
        if (result == 0) ev->mValid = true;
    }
    return result;
}

void 
IOService::delEvent(IOEvent* ev)
{
    if (!ev->mValid) return;
    if (ev->mEventType & IOSERVICE_TIMEOUT_EVENT)
    {
        mTimeoutHeap.erase(ev);
    }
    else
    {
        mIOEventOperator->mDelEvent(ev, mOperation);
    }
    ev->mValid = false;
}

Int32 
IOService::poll(UInt32 times)
{
    if (mShutdown == true)
    {
        LOG_WARN("IOService has stoped...");
        return 0;
    }

    if (mPaused == true)
    {
        LOG_DEBUG("IOService has paused...");
        TimeUtil::sleep(1);
        return 0;
    }

    Timeval tv, now;
    IOEvent* ev;

    TimeUtil::getTimeOfDay(&now, NULL);

    // Check if time is running backwords
    if (TIMERCMP(&now, &mTimeFlag, <))
    {
        Timeval off;
        TIMERSUB(&mTimeFlag, &now, &off);
        for (Int32 i = 0; i < mTimeoutHeap.size(); ++i)
        {
            TIMERSUB(&mTimeoutHeap.mHeap[i]->mTimeout, &off, &mTimeoutHeap.mHeap[i]->mTimeout);
        }
    }

    mTimeFlag = now;
    tv.tv_sec = tv.tv_usec = 0;

    while (times--)
    {
        if (mIOEventOperator->mDispatch(this, mOperation, &tv) != 0)
        {
            LOG_WARN("Exception for io disptach.");
            return -1;
        }
    }

    // Process timeout
    if (mTimeoutHeap.empty())
    {
        return 0;
    }

    TimeUtil::getTimeOfDay(&now, NULL);

    while ((ev = mTimeoutHeap.top()))
    {
        if (TIMERCMP(&ev->mTimeout, &now, >))
        {
            break;
        }

        mTimeoutHeap.pop();
        ev->mValid = false;
        ev->mCallback(ev, IOSERVICE_TIMEOUT_EVENT);
    }
    return 0;
}

Int32 
IOService::run()
{
    Timeval tv, now;
    IOEvent* ev;

    while (!mShutdown)
    {
        if (mPaused == true)
        {
            LOG_DEBUG("IOService has paused...");
            TimeUtil::sleep(1);
            continue;
        }

        tv.tv_sec = tv.tv_usec = 0;
        TimeUtil::getTimeOfDay(&now, NULL);

        // Check if time is running backwords
        if (TIMERCMP(&now, &mTimeFlag, <))
        {
            Timeval off;
            TIMERSUB(&mTimeFlag, &now, &off);
            for (Int32 i = 0; i < mTimeoutHeap.size(); ++i)
            {
                TIMERSUB(&mTimeoutHeap.mHeap[i]->mTimeout, &off, &mTimeoutHeap.mHeap[i]->mTimeout);
            }
        }

        mTimeFlag = now;
        ev = mTimeoutHeap.top();
        if (ev != NULL)
        {
            if (TIMERCMP(&ev->mTimeout, &now, >))
            {
                TIMERSUB(&ev->mTimeout, &now, &tv); 
            }
            else
            {
                tv.tv_usec = 10;
            }
        }
        else
        {
            tv.tv_usec = 10;
        }

        if (mIOEventOperator->mDispatch(this, mOperation, &tv) != 0)
        {
            LOG_WARN("Exception for io disptach.");
            return -1;
        }

        // Process timeout
        if (mTimeoutHeap.empty())
        {
            continue;
        }

        TimeUtil::getTimeOfDay(&now, NULL);

        while ((ev = mTimeoutHeap.top()))
        {
            if (TIMERCMP(&ev->mTimeout, &now, >))
            {
                break;
            }

            mTimeoutHeap.pop();
            ev->mValid = false;
            ev->mCallback(ev, IOSERVICE_TIMEOUT_EVENT);
        }
    }
    LOG_INFO("IOService has stoped...");
    return 0;
}
        
void 
IOService::stop()
{
    mShutdown = true;
}

void 
IOService::pause()
{
    mPaused = true;
}

void 
IOService::resume()
{
    mPaused = false;
}
