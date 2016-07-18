#include "IOService.h"
#include "Logger.h"
#include "Container.h"
#include "errno.h"

using namespace Lynx;

#ifdef _WIN32

#include <winsock2.h>
#include <windows.h>

#define NEVENT 4096
#define FD_SET_ALLOC_SIZE(n) ((sizeof(FdSet) + ((n) - 1) * sizeof(SOCKET)))

struct FdSet
{
    UInt32                     mFdCount;
    SOCKET                     mFdArray[1];
};

struct EventWrapper
{
    SOCKET                     mFd;
    Int32                      mReadPos;
    Int32                      mWritePos;
    IOEvent*                   mReadEvent;
    IOEvent*                   mWriteEvent;

    EventWrapper() : mFd(-1), mReadPos(-1), mWritePos(-1), mReadEvent(0), mWriteEvent(0) {}
};

struct SelectOperation 
{
    SelectOperation()
    {
        mSize = NEVENT;
        Int32 size = FD_SET_ALLOC_SIZE(NEVENT);
        mReadSetIn = (FdSet*)malloc(size);
        mWriteSetIn = (FdSet*)malloc(size);
        mReadSetOut = (FdSet*)malloc(size);
        mWriteSetOut = (FdSet*)malloc(size);
        mExSetOut = (FdSet*)malloc(size);
        mReadSetIn->mFdCount = mWriteSetIn->mFdCount = 0;
        mReadSetOut->mFdCount = mWriteSetOut->mFdCount = mExSetOut->mFdCount = 0;
    }

    ~SelectOperation()
    {
        if (mReadSetIn) free(mReadSetIn);
        if (mWriteSetIn) free(mWriteSetIn);
        if (mReadSetOut) free(mReadSetOut);
        if (mWriteSetOut) free(mWriteSetOut);
        if (mExSetOut) free(mExSetOut);
    }

    //void Resize(UInt32 size)
    //{
    //    ASSERT(size >= mReadSetIn->mFdCount && size >= mWriteSetIn->mFdCount);
    //    ASSERT(size >= 1);
    //
    //    Int32 tempSize;
    //    tempSize = FD_SET_ALLOC_SIZE(size);
    //    mReadSetIn = (FdSet*)realloc(mReadSetIn, tempSize);
    //    mWriteSetIn = (FdSet*)realloc(mWriteSetIn, tempSize);
    //    mReadSetOut = (FdSet*)realloc(mReadSetOut, tempSize);
    //    mWriteSetOut = (FdSet*)realloc(mWriteSetOut, tempSize);
    //    mSize = size;
    //}

    Int32 doFdSet(EventWrapper* wrapper, bool read)
    {
        SOCKET fd = wrapper->mFd;
        FdSet* fdSet = read ? mReadSetIn : mWriteSetIn;
        if (read)
        {
            if (wrapper->mReadPos >= 0)
            {
                return 0;
            }
        }
        else 
        {
            if (wrapper->mWritePos >= 0)
            {
                return 0;
            }
        }
        if (fdSet->mFdCount >= mSize)
        {
            LOG_WARN("Fd count %d > fd set array size %d", fdSet->mFdCount, mSize);
            return -1;
        }
        fdSet->mFdArray[fdSet->mFdCount] = fd;
        if (read)
        {
            wrapper->mReadPos = fdSet->mFdCount;
        }
        else
        {
            wrapper->mWritePos = fdSet->mFdCount;
        }
        return (fdSet->mFdCount++);
    }

    Int32 doFdClear(EventWrapper* wrapper, bool read)
    {
        Int32  i;
        FdSet* fdSet = read ? mReadSetIn : mWriteSetIn;
        if (read)
        {
            i = wrapper->mReadPos;
            wrapper->mReadPos = -1;
        }
        else
        {
            i = wrapper->mWritePos;
            wrapper->mWritePos = -1;
        }
        if (i < 0)
        {
            return 0;
        }
        if (--fdSet->mFdCount != i)
        {
            SOCKET fd2;
            fd2 = fdSet->mFdArray[i] = fdSet->mFdArray[fdSet->mFdCount];
            Map<SOCKET, EventWrapper*>::Iter* iter = mEventWrapperMap.find(fd2);
            if (iter == NULL || iter->mValue == NULL) // This indicates a bug
            {
                return 0;
            }
            if (read)
            {
                iter->mValue->mReadPos = i;
            }
            else
            {
                iter->mValue->mWritePos = i;
            }
        }
        return 0;
    }

    Int32                               mSize;
    FdSet*                              mReadSetIn;
    FdSet*                              mWriteSetIn;
    FdSet*                              mReadSetOut;
    FdSet*                              mWriteSetOut;
    FdSet*                              mExSetOut;
    Map<SOCKET, EventWrapper*>          mEventWrapperMap;
};

static void*  selectInitial(IOService* ioService);
static void   selectRelease(void* op);
static Int32  selectAddEvent(IOEvent* ev, void* op);
static Int32  selectDelEvent(IOEvent* ev, void* op);
static Int32  selectDispatch(IOService* ioService, void* op, Timeval* tv);

IOEventOperator selectOperator = 
{
    IOSERVICE_SELECT_DRIVER,
    "SELECT",
    selectInitial,
    selectRelease,
    selectAddEvent,
    selectDelEvent,
    selectDispatch
};

#define FD_SET_ALLOC_SIZE(n) ((sizeof(FdSet) + ((n) - 1) * sizeof(SOCKET)))
#define TIMEVAL_TO_MS(tv) (((tv)->tv_sec * 1000) + ((tv)->tv_usec / 1000))

void*  
selectInitial(IOService* ioService)
{
    WSADATA wsaData;
    WORD version = MAKEWORD(2, 2);
    WSAStartup(version, &wsaData);

    SelectOperation* operation = XNEW(SelectOperation)();
    ASSERT(operation);
    return operation;
}

void   
selectRelease(void* op)
{
    SelectOperation* operation = (SelectOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid SelectOperation pointer");
        return;
    }
    XDELETE(operation);
}

Int32  
selectAddEvent(IOEvent* ev, void* op)
{
    if (ev == NULL)
    {
        LOG_WARN("Invalid IOEvent pointer");
        return -1;
    }

    SelectOperation* operation = (SelectOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid SelectOperation pointer");
        return -1;
    }

    if (!(ev->mEventType & (IOSERVICE_READ_EVENT | IOSERVICE_WRITE_EVENT)))
    {
        return 0;
    }
    
    Map<SOCKET, EventWrapper*>::Iter* iter = operation->mEventWrapperMap.find(ev->mFd);
    if (iter == NULL)
    {
        EventWrapper* wrapper = XNEW(EventWrapper)();
        wrapper->mFd = ev->mFd;
        iter = operation->mEventWrapperMap.insert(wrapper->mFd, wrapper);
    }

    if (ev->mEventType & IOSERVICE_READ_EVENT)
    {
        if (operation->doFdSet(iter->mValue, true) < 0)
        {
            return -1;
        }
        iter->mValue->mReadEvent = ev;
    }

    if (ev->mEventType & IOSERVICE_WRITE_EVENT)
    {
        if (operation->doFdSet(iter->mValue, false) < 0)
        {
            return -1;
        }
        iter->mValue->mWriteEvent = ev;
    }
    return 0;
}

Int32  
selectDelEvent(IOEvent* ev, void* op)
{
    if (ev == NULL)
    {
        LOG_WARN("Invalid IOEvent pointer");
        return -1;
    }

    SelectOperation* operation = (SelectOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid SelectOperation pointer");
        return -1;
    }

    Map<SOCKET, EventWrapper*>::Iter* iter = operation->mEventWrapperMap.find(ev->mFd);
    if (iter == NULL || iter->mValue == NULL)
    {
        return -1;
    }

    if (ev == iter->mValue->mReadEvent)
    {
        operation->doFdClear(iter->mValue, true);
        iter->mValue->mReadEvent = NULL;
    }

    if (ev == iter->mValue->mWriteEvent)
    {
        operation->doFdClear(iter->mValue, false);
        iter->mValue->mWriteEvent = NULL;
    }

    if (!iter->mValue->mReadEvent && !iter->mValue->mWriteEvent)
    {
        XDELETE(iter->mValue);
        operation->mEventWrapperMap.erase(iter);
    }
    return 0;
}

void
FdSetCopy(FdSet* out, FdSet* in)
{
    out->mFdCount = in->mFdCount;
    memcpy(out->mFdArray, in->mFdArray, in->mFdCount * sizeof(SOCKET));
}

Int32  
selectDispatch(IOService* ioService, void* op, Timeval* tv)
{
    if (ioService == NULL)
    {
        LOG_WARN("Invalid IOService pointer");
        return -1;
    }

    SelectOperation* operation = (SelectOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid SelectOperation pointer");
        return -1;
    }

    FdSetCopy(operation->mReadSetOut, operation->mReadSetIn);
    FdSetCopy(operation->mExSetOut, operation->mWriteSetIn);
    FdSetCopy(operation->mWriteSetOut, operation->mWriteSetIn);

    Int32 fdCount = (operation->mReadSetOut->mFdCount > operation->mWriteSetOut->mFdCount) ?
        operation->mReadSetOut->mFdCount : operation->mWriteSetOut->mFdCount;

    if (!fdCount)
    {
        // Windows doesn't like you to call select() with no sockets
        if (tv)
        {
            ::Sleep(TIMEVAL_TO_MS((struct timeval*)tv));
        }
        return 0;
    }

    Int32 result = select(fdCount, (fd_set*)operation->mReadSetOut, 
        (fd_set*)operation->mWriteSetOut,(fd_set*)operation->mExSetOut, (struct timeval*)tv);

    if (result <= 0)
    {
        return result;
    }
    
    Int32 i = 0;

    if (operation->mReadSetOut->mFdCount)
    {
        Int32 i = rand() % operation->mReadSetOut->mFdCount;
        SOCKET fd = -1;
        EventWrapper* wrapper = 0;

        for (Int32 j = 0; j < operation->mReadSetOut->mFdCount; ++j)
        {
            if (++i >= operation->mReadSetOut->mFdCount)
            {
                i = 0;
            }
            fd = operation->mReadSetOut->mFdArray[i];
            Map<SOCKET, EventWrapper*>::Iter* iter = operation->mEventWrapperMap.find(fd);
            if (iter && iter->mValue && iter->mValue->mReadEvent)
            {
                iter->mValue->mReadEvent->mCallback(iter->mValue->mReadEvent, 
                    IOSERVICE_READ_EVENT);
            }
        }
    }

    if (operation->mExSetOut->mFdCount)
    {
        Int32 i = rand() % operation->mExSetOut->mFdCount; 
        SOCKET fd = -1;
        EventWrapper* wrapper = 0;
        for (Int32 j = 0; j < operation->mExSetOut->mFdCount; ++j)
        {
            if (++i >= operation->mExSetOut->mFdCount)
            {
                i = 0;
            }
            fd = operation->mExSetOut->mFdArray[i];
            Map<SOCKET, EventWrapper*>::Iter* iter = operation->mEventWrapperMap.find(fd);
            if (iter && iter->mValue && iter->mValue->mWriteEvent)
            {
                iter->mValue->mWriteEvent->mErrno = LYNX_ERROR_TCPSEND;
                iter->mValue->mWriteEvent->mCallback(iter->mValue->mWriteEvent, 
                    IOSERVICE_WRITE_EVENT);
            }
        }
    }

    if (operation->mWriteSetOut->mFdCount)
    {
        Int32 i = rand() % operation->mWriteSetOut->mFdCount;
        SOCKET fd = -1;
        EventWrapper* wrapper = 0;
        for (Int32 j = 0; j < operation->mWriteSetOut->mFdCount; ++j)
        {
            if (++i >= operation->mWriteSetOut->mFdCount)
            {
                i = 0;
            }
            fd = operation->mWriteSetOut->mFdArray[i];
            Map<SOCKET, EventWrapper*>::Iter* iter = operation->mEventWrapperMap.find(fd);
            if (iter && iter->mValue && iter->mValue->mWriteEvent)
            {
                iter->mValue->mWriteEvent->mCallback(iter->mValue->mWriteEvent, 
                    IOSERVICE_WRITE_EVENT);
            }
        }
    }
    return 0;
}

#else /////////////////////////////////////////////////////////////////////////////

#ifndef howmany
#define howmany(x, y) (((x) + ((y) - 1))/(y))
#endif

#define FDBITS (sizeof(long) * 8)
typedef unsigned long FD_MASK;
typedef fd_set FdSet;

struct SelectOperation
{
    SelectOperation() : m_MaxFd(0), mSize(0), mReadSetIn(0), mWriteSetIn(0)
                      , mReadSetOut(0), mWriteSetOut(0)
                      , mReadEventArray(0), mWriteEventArray(0)
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        Resize(32);
#else
        Resize(howmany(32 + 1, FDBITS) * sizeof(FD_MASK));
#endif
    }

    ~SelectOperation()
    {
        if (mReadSetIn) free(mReadSetIn);
        if (mWriteSetIn) free(mWriteSetIn);
        if (mReadSetOut) free(mReadSetOut);
        if (mWriteSetOut) free(mWriteSetOut);
        if (mReadEventArray) free(mReadEventArray);
        if (mWriteEventArray) free(mWriteEventArray);
    }

    void Resize(UInt32 size)
    {
        mReadSetIn = (FdSet*)realloc(mReadSetIn, size);
        mWriteSetIn = (FdSet*)realloc(mWriteSetIn, size);
        mReadSetOut = (FdSet*)realloc(mReadSetOut, size);
        mWriteSetOut = (FdSet*)realloc(mWriteSetOut, size);

        Int32 eventCount = (size / sizeof(FD_MASK)) * FDBITS;
        Int32 oldEventCount = (mSize / sizeof(FD_MASK)) * FDBITS;

        mReadEventArray = (IOEvent**)realloc(mReadEventArray, eventCount * sizeof(IOEvent*));
        mWriteEventArray = (IOEvent**)realloc(mWriteEventArray, eventCount * sizeof(IOEvent*));

        memset((char*)mReadSetIn + mSize, 0, size - mSize);
        memset((char*)mWriteSetIn + mSize, 0, size - mSize);
        memset(mReadEventArray + oldEventCount, 0, (eventCount - oldEventCount) * sizeof(IOEvent*));
        memset(mWriteEventArray + oldEventCount, 0, (eventCount - oldEventCount) * sizeof(IOEvent*));

        mSize = size;
    }

    Int32         m_MaxFd;
    Int32         mSize;
    FdSet*        mReadSetIn;
    FdSet*        mWriteSetIn;
    FdSet*        mReadSetOut;
    FdSet*        mWriteSetOut;   
    IOEvent**     mReadEventArray;
    IOEvent**     mWriteEventArray;
};

static void*  selectInitial(IOService* ioService);
static void   selectRelease(void* op);
static Int32  selectAddEvent(IOEvent* ev, void* op);
static Int32  selectDelEvent(IOEvent* ev, void* op);
static Int32  selectDispatch(IOService* ioService, void* op, Timeval* tv);

IOEventOperator selectOperator = 
{
    IOSERVICE_SELECT_DRIVER,
    "SELECT",
    selectInitial,
    selectRelease,
    selectAddEvent,
    selectDelEvent,
    selectDispatch,
};

static void*
selectInitial(IOService* ioService)
{
    SelectOperation* operation = XNEW(SelectOperation)();
    ASSERT(operation);
    return operation;
}

static void
selectRelease(void* op)
{
    SelectOperation* operation = (SelectOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid SelectOperation pointer");
        return;
    }
    XDELETE(operation);
}

static Int32
selectAddEvent(IOEvent* ev, void* op)
{
    if (ev == NULL)
    {
        LOG_WARN("Invalid IOEvent pointer");
        return -1;
    }

    SelectOperation* operation = (SelectOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid SelectOperation pointer");
        return -1;
    }

    Int32 fd = ev->mFd;

    if (fd >= operation->mSize)
    {
        LOG_WARN("Fd %d overflow, size %d", fd, operation->mSize);
        return -1;
    }

    if (operation->m_MaxFd < fd)
    {
        Int32 size = operation->mSize;
        if (size < sizeof(FD_MASK))
        {
            size = sizeof(FD_MASK);
        }

        while (size < (howmany(fd + 1, FDBITS) * sizeof(FD_MASK)))
        {
            size *= 2;
        }

        if (size != operation->mSize)
        {
            operation->Resize(size);
        }
        operation->m_MaxFd = fd;
    }

    if (ev->mEventType & IOSERVICE_READ_EVENT)
    {
        FD_SET(fd, operation->mReadSetIn);
        operation->mReadEventArray[fd] = ev;
    }

    if (ev->mEventType & IOSERVICE_WRITE_EVENT)
    {
        FD_SET(fd, operation->mWriteSetIn);
        operation->mWriteEventArray[fd] = ev;
    }
    return 0;
}
        
static Int32 
selectDelEvent(IOEvent* ev, void* op)
{
    if (ev == NULL)
    {
        LOG_WARN("Invalid IOEvent pointer");
        return -1;
    }

    SelectOperation* operation = (SelectOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid SelectOperation pointer");
        return -1;
    }

    Int32 fd = ev->mFd;

    if (operation->m_MaxFd < fd)
    {
        return 0;
    }

    if (ev->mEventType & IOSERVICE_READ_EVENT)
    {
        FD_CLR(fd, operation->mReadSetIn);
        operation->mReadEventArray[fd] = NULL;
    }

    if (ev->mEventType & IOSERVICE_WRITE_EVENT)
    {
        FD_CLR(fd, operation->mWriteSetIn);
        operation->mWriteEventArray[fd] = NULL;
    }
    
    return 0;
}

static Int32  
selectDispatch(IOService* ioService, void* op, Timeval* tv)
{
    if (ioService == NULL)
    {
        LOG_WARN("Invalid IOService pointer");
        return -1;
    }

    SelectOperation* operation = (SelectOperation*)op;
    if (operation == NULL)
    {
        LOG_WARN("Invalid SelectOperation pointer");
        return -1;
    }

    memcpy(operation->mReadSetOut, operation->mReadSetIn, operation->mSize);
    memcpy(operation->mWriteSetOut, operation->mWriteSetIn, operation->mSize);

    Int32 result = select(operation->m_MaxFd + 1, operation->mReadSetOut,
        operation->mWriteSetOut, NULL, (struct timeval *)tv);

    if (result == -1)
    {
        if (ERRNUM() != EINTR)
        {
            LOG_WARN("Failed to call system call select");
            return -1;
        }
        return 0;
    }

    Int32 i = random() % (operation->m_MaxFd + 1);
    for (Int32 j = 0; j <= operation->m_MaxFd; ++j)
    {
        IOEvent* readEvent = NULL, *writeEvent = NULL;
        if (++i >= operation->m_MaxFd + 1)
        {
            i = 0;
        }

        result = 0;
        if (FD_ISSET(i, operation->mReadSetOut))
        {
            readEvent = operation->mReadEventArray[i];
            result |= IOSERVICE_READ_EVENT;
        }

        if (FD_ISSET(i, operation->mWriteSetOut))
        {
            writeEvent = operation->mWriteEventArray[i];
            result |= IOSERVICE_WRITE_EVENT;
        }

        if (readEvent && (result & readEvent->mEventType))
        {
            readEvent->mCallback(readEvent, IOSERVICE_READ_EVENT);
        }

        if (writeEvent && writeEvent != readEvent && (result & writeEvent->mEventType))
        {
            writeEvent->mCallback(writeEvent, IOSERVICE_WRITE_EVENT);
        }
    }
    return 0;
}

#endif /////////////////////////////////////////////////////////////////////////////


