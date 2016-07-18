#ifdef __linux__

#include "IOService.h"
#include "Logger.h"
#include "Container.h"
#include "ErrorCode.h"

#include <sys/epoll.h>
#include <unistd.h>

using namespace Lynx;

#ifdef HAVE_SETFD
#define FD_CLOSEONEXEC(x) do {                                         \
        if (fcntl(x, F_SETFD, 1) == -1)                                \
                event_warn("fcntl(%d, F_SETFD)", x);                   \
} while (0)
#else
#define FD_CLOSEONEXEC(x)
#endif

/* On Linux kernels at least up to 2.6.24.4, epoll can't handle timeout
 * values bigger than (LONG_MAX - 999ULL)/HZ.  HZ in the wild can be
 * as big as 1000, and LONG_MAX can be as small as (1<<31)-1, so the
 * largest number of msec we can support here is 2147482.  Let's
 * round that down by 47 seconds.
 */
#define MAX_EPOLL_TIMEOUT_MSEC (35 * 60 * 1000)

#define INITIAL_NFILES         20480 
#define INITIAL_NEVENTS        4096
#define MAX_NEVENTS            4096

// Due to limitations in the epool interface, we need to keep track of all file descriptiors outself.

struct EventWrapper
{
    IOEvent*    mReadEvent;
    IOEvent*    mWriteEvent;
};

struct EpollOperation
{
    EpollOperation(Int32 epollFd) : mEpollFd(epollFd) 
    {
        mEpollEventArray = (struct epoll_event*)malloc(INITIAL_NEVENTS * sizeof(struct epoll_event));
        mEpollEventArraySize = INITIAL_NEVENTS;
        mEventWrapperArray = (EventWrapper*)calloc(INITIAL_NFILES, sizeof(EventWrapper));
        mEventWrapperArraySize = INITIAL_NFILES;
    }

    ~EpollOperation()
    {
        if (mEventWrapperArray) free(mEventWrapperArray);
        if (mEpollEventArray) free(mEpollEventArray);
    }

    //Int32 resizeEventWrapperArray(Int32 newSize)
    //{
    //    if (newSize >= mEventWrapperArraySize)
    //    {
    //        LOG_WARN("resizeEventWrapperArray was call with new size %d", newSize);
    //        Int32 size = mEventWrapperArraySize;
    //        while (size <= newSize) size <<= 1;
    //        EventWrapper* wrapperArray = (EventWrapper*)realloc(mEventWrapperArray, 
    //            size * sizeof(EventWrapper));
    //        if (wrapperArray == NULL)
    //        {
    //            LOG_WARN("Failed to recalloc.");
    //            return -1;
    //        }
    //
    //        mEventWrapperArray = wrapperArray;
    //        memset(wrapperArray + mEventWrapperArraySize, 0, 
    //            (size - mEventWrapperArraySize) * sizeof(EventWrapper));
    //        mEventWrapperArraySize = size;
    //    }
    //    return 0;
    //}

    //Int32 resizeEpollEventArray(Int32 newSize)
    //{
    //    LOG_WARN("ResizeEventEventArray was call with new size %d", newSize);
    //    if (newSize >= mEpollEventArraySize)
    //    {
    //        Int32 size = mEpollEventArraySize;
    //        while (size <= newSize) size <<= 1;
    //        struct epoll_event* eventArray = (struct epoll_event*)realloc(mEpollEventArray, 
    //            size * sizeof(struct epoll_event));
    //        if (eventArray == NULL)
    //        {
    //            LOG_WARN("Failed to realloc.");
    //            return -1;
    //        }
    //        mEpollEventArray = eventArray;
    //        mEpollEventArraySize = size;
    //    }
    //    return 0;
    //}

    EventWrapper*            mEventWrapperArray;
    Int32                    mEventWrapperArraySize;
    struct epoll_event*      mEpollEventArray;
    Int32                    mEpollEventArraySize;
    Int32                    mEpollFd;
};

static void*  epollInitial(IOService* ioService);
static void   epollRelease(void* op);
static Int32  epollAddEvent(IOEvent* ev, void* op);
static Int32  epollDelEvent(IOEvent* ev, void* op);
static Int32  epollDispatch(IOService* ioService, void* op, Timeval* tv);

IOEventOperator epollOperator = 
{
    IOSERVICE_EPOLL_DRIVER,
    "EPOLL",
    epollInitial,
    epollRelease,
    epollAddEvent,
    epollDelEvent,
    epollDispatch
};

void*  
epollInitial(IOService* ioService)
{
    // initialize zhe kernel queue
    Int32 epollFd = epoll_create(32000);
    if (epollFd == -1)
    {
        LOG_WARN("Failed to call epoll_crate.");
        return NULL;
    }
    
    FD_CLOSEONEXEC(epollFd);

    EpollOperation* operation = XNEW(EpollOperation)(epollFd);
    return operation;
}

void epollRelease(void* op)
{
    EpollOperation* operation = (EpollOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid EpollOperation pointer");
        return;
    }

    if (operation->mEpollFd)
    {
        close(operation->mEpollFd);
    }

    XDELETE(operation);
}

Int32  
epollAddEvent(IOEvent* ev, void* op)
{
    if (ev == NULL)
    {
        LOG_WARN("Invalid IOEvent pointer");
        return -1;
    }

    EpollOperation* operation = (EpollOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid EpollOperation pointer");
        return -1;
    }

    Int32 fd = ev->mFd;

    if (fd >= operation->mEventWrapperArraySize)
    {
        LOG_WARN("fd %d > EventWraperArraySize %d", fd, 
            operation->mEventWrapperArraySize);
        return -1;
        // Extent the file descriptor array necessary
        //if (operation->resizeEventWrapperArray(fd) == -1)
        //{
        //    LOG_WARN("Failed to resizeEventWrapperArray fd %d.", fd);
        //    return -1;
        //}
    }

    EventWrapper* wrapper = &operation->mEventWrapperArray[fd];
    Int32 opType = EPOLL_CTL_ADD;
    Int32 events = 0;

    if (wrapper->mReadEvent != NULL)
    {
        events |= EPOLLIN;
        opType = EPOLL_CTL_MOD;
    }
    if (wrapper->mWriteEvent != NULL)
    {
        events |= EPOLLOUT;
        opType = EPOLL_CTL_MOD;
    }

    if (ev->mEventType & IOSERVICE_READ_EVENT)
    {
        events |= EPOLLIN;
    }
    if (ev->mEventType & IOSERVICE_WRITE_EVENT)
    {
        events |= EPOLLOUT;
    }

    struct epoll_event epollEvent = {0, {0}};
    epollEvent.data.fd = fd;
    epollEvent.events = events;
    if (epoll_ctl(operation->mEpollFd, opType, fd, &epollEvent) == -1)
    {
        LOG_WARN("Failed to call epoll_ctl opType %d fd %d errno %d", 
            opType, fd, errno);
        return -1;
    }

    // Update events responsible
    if (ev->mEventType & IOSERVICE_READ_EVENT)
    {
        wrapper->mReadEvent = ev;
    }

    if (ev->mEventType & IOSERVICE_WRITE_EVENT)
    {
        wrapper->mWriteEvent = ev;
    }

    return 0;
}

Int32  
epollDelEvent(IOEvent* ev, void* op)
{
    if (ev == NULL)
    {
        LOG_WARN("Invalid IOEvent pointer");
        return -1;
    }

    EpollOperation* operation = (EpollOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid SelectOperation pointer");
        return -1;
    }

    Int32 fd = ev->mFd;
    if (fd >= operation->mEventWrapperArraySize)
    {
        return 0;
    }

    EventWrapper* wrapper = &operation->mEventWrapperArray[fd];
    Int32 opType = EPOLL_CTL_DEL;
    Int32 events = 0;

    if (ev->mEventType & IOSERVICE_READ_EVENT)
    {
        events |= EPOLLIN;
    }
    if (ev->mEventType & IOSERVICE_WRITE_EVENT)
    {
        events |= EPOLLOUT;
    }

    bool writeDelete = true;
    bool readDelete  = true;

    if ((events & (EPOLLIN | EPOLLOUT)) != (EPOLLIN | EPOLLOUT))
    {
        if ((events & EPOLLIN) && wrapper->mWriteEvent != NULL)
        {
            writeDelete = false;
            events = EPOLLOUT;
            opType = EPOLL_CTL_MOD;
        }
        else if ((events & EPOLLOUT) && wrapper->mReadEvent != NULL)
        {
            readDelete = false;
            events = EPOLLIN;
            opType = EPOLL_CTL_MOD;
        }
    }

    struct epoll_event epollEvent = {0, {0}};
    epollEvent.data.fd = fd;
    epollEvent.events = events;

    if (epoll_ctl(operation->mEpollFd, opType, fd, &epollEvent) == -1)
    {
        LOG_WARN("Failed to call epoll_ctl opType %d fd %d errno %d", 
            opType, fd, errno);
        return -1;
    }

    if (readDelete)
    {
        wrapper->mReadEvent = NULL;
    }
    if (writeDelete)
    {
        wrapper->mWriteEvent = NULL;
    }
    return 0;
}

Int32  
epollDispatch(IOService* ioService, void* op, Timeval* tv)
{
    if (ioService == NULL)
    {
        LOG_WARN("Invalid IOService pointer");
        return -1;
    }

    EpollOperation* operation = (EpollOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid EpollOperation pointer");
        return -1;
    }

    Int32 timeout = -1;

    if (tv != NULL)
    {
        timeout = tv->tv_sec * 1000 + (tv->tv_usec + 999) / 1000;
    }

    if (timeout < 0 || timeout > MAX_EPOLL_TIMEOUT_MSEC)
    {
        // Linux kernels can wait forever if the timeout is too big;
        // See comment on MAX_EPOLL_TIMEOUT_MSEC
        timeout = MAX_EPOLL_TIMEOUT_MSEC;
    }

    Int32 result = epoll_wait(operation->mEpollFd, operation->mEpollEventArray, 
        operation->mEpollEventArraySize, timeout);
    if (result == -1)
    {
        if (ERRNUM() != EINTR)
        {
            LOG_WARN("Failed to epoll_wait epfd %d, errno %d",
                operation->mEpollFd, errno);
            return -1;
        }
        return 0;
    }

    if (result >= operation->mEpollEventArraySize)
    {
        LOG_WARN("result %d > EpollEventArraySize %d", result,
            operation->mEpollEventArraySize);
        return -1;
    }
        
    EventWrapper* wrapper = NULL;

    for (Int32 i = 0; i < result; i++)
    {
        Int32 what = operation->mEpollEventArray[i].events;
        IOEvent* readEvent = NULL, *writeEvent = NULL;
        Int32 fd = operation->mEpollEventArray[i].data.fd;

        if (fd < 0 || fd >= operation->mEventWrapperArraySize)
        {
            continue;
        }

        wrapper = &operation->mEventWrapperArray[fd];

        if (what & (EPOLLHUP | EPOLLERR))
        {
            readEvent = wrapper->mReadEvent;
            writeEvent = wrapper->mWriteEvent;
        }
        else
        {
            if (what & EPOLLIN)
            {
                readEvent = wrapper->mReadEvent;
            }
            if (what & EPOLLOUT)
            {
                writeEvent = wrapper->mWriteEvent;
            }
        }

        if (!(readEvent || writeEvent))
        {
            continue;
        }

        if (readEvent != NULL)
        {
            readEvent->mCallback(readEvent, IOSERVICE_READ_EVENT);
        }

        if (writeEvent != NULL)
        {
            writeEvent->mCallback(writeEvent, IOSERVICE_WRITE_EVENT);
        }
    }

    //if (result == operation->mEpollEventArraySize && 
    //    operation->mEpollEventArraySize < MAX_NEVENTS)
    //{
    //    // We used all of the event space this time. 
    //    // We should be ready for more events next time.
    //    operation->resizeEpollEventArray(operation->mEpollEventArraySize * 2);
    //}

    return 0;
}

#endif


