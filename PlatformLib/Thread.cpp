#include <cassert>
#include <iostream>

#if defined(WIN32)
# include <windows.h>
# include <winbase.h>
# include <process.h> // for _beginthreadex()
typedef unsigned(__stdcall *FG_THREAD_START_ROUTINE)(void*);
#endif

#ifndef _WIN32
#include<stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>

pid_t gettid(void)
{
#if !defined(__APPLE__)
	return syscall(__NR_gettid);
#else
    return pthread_mach_thread_np(pthread_self());
#endif
}
#endif

//#include "Atomic.h"
#include "Thread.h"
#include "Logger.h"

using namespace Lynx;

UInt32 Thread::msThreadCount = 0;

extern "C"
{
    static void*
#ifdef WIN32
    __stdcall
#endif
    threadWrapper(void* threadParm)
    {
        ASSERT(threadParm);
        Thread* t = static_cast <Thread*> (threadParm);

        ASSERT(t);
        t->threadFunc();
#if defined(WIN32)
       _endthreadex(0);
#endif
        return 0;
    }
}

Thread::Thread(): mId(0), mShutdown(-1)
{

}

Thread::~Thread()
{
   shutdown();
   join();
}

bool 
Thread::startup(unsigned int stackSize)
{
    ASSERT(mId == 0);
#if defined (_WIN32)
    mThread = (HANDLE)_beginthreadex(NULL, 0, FG_THREAD_START_ROUTINE(threadWrapper), this, 0, &mId);
    //设置线程为正常优先级
    setPriority(LYNX_THREAD_PRIORITY_NORMAL);
    ASSERT(mThread != 0);
#else
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0)
    {
        LOG_ERROR("Failed init thread attr");
        return false;
    }

    if (pthread_attr_setstacksize(&attr, stackSize) != 0)
    {
        LOG_ERROR("Failed to set stack size.");
        return false;
    }
    
    if (pthread_create(&mId, &attr, threadWrapper, this) != 0)
    {
        LOG_ERROR("Failed to spawn thread");
        return false;
    }

    if (pthread_attr_destroy(&attr) != 0) 
    {
        LOG_ERROR("Failed to destroy thread attr.");
        return false;
    }
#endif
    mShutdown = 0;
    msThreadCount++;
    return true;
}

void 
Thread::shutdown()
{
    if (mShutdown == 0)
    {
        mShutdown = 1;
        msThreadCount--;
    }
    //InterlockedIncrement(&mShutdown);
}

void 
Thread::join()
{
    if (mId == 0)
    {
        return;
    }

#if defined (_WIN32)
   DWORD exitCode;
   while (true)
   {
       if (GetExitCodeThread(mThread, &exitCode) != 0)
       {
            if (exitCode != STILL_ACTIVE)
            {
                break;
            }
            else
            {
                // wait之前， 需要唤起线程， 防止线程处于挂起状态导致死等
                ResumeThread(mThread);
                WaitForSingleObject(mThread, INFINITE);
            }
        }
        else
        {
            // log something here
            break;
        }
    }

    CloseHandle(mThread);
#else
    void* stat;
    if (mId != pthread_self())
    {
        int r = pthread_join(mId, &stat);
        if (r != 0)
        {
            LOG_WARN("Failed to call pthread_join");
            ASSERT(0);
        }
    }
#endif
    mId = 0;
}

bool 
Thread::isShutdown()
{
   return mShutdown == 1;
}

unsigned int 
Thread::getThreadId()
{
#ifdef _WIN32
    return ::GetCurrentThreadId();
#else
    return gettid();
#endif
}

bool 
Thread::setPriority(UInt32 priority)
{
#ifdef _WIN32
    UInt32 pri = THREAD_PRIORITY_NORMAL;
    switch (priority)
    {
    case LYNX_THREAD_PRIORITY_LOWEST:
        pri = THREAD_PRIORITY_LOWEST;
        break;
    case LYNX_THREAD_PRIORITY_BELOW_NORMAL:
        pri = THREAD_PRIORITY_BELOW_NORMAL;
        break;
    case LYNX_THREAD_PRIORITY_NORMAL:
        pri = THREAD_PRIORITY_NORMAL;
        break;
    case LYNX_THREAD_PRIORITY_HIGHEST:
        pri = THREAD_PRIORITY_HIGHEST;
        break;
    case LYNX_THREAD_PRIORITY_ABOVE_NORMAL:
        pri = THREAD_PRIORITY_ABOVE_NORMAL;
        break;
    case LYNX_THREAD_PRIORITY_ERROR_RETURN:
        pri = THREAD_PRIORITY_ERROR_RETURN;
        break;
    case LYNX_THREAD_PRIORITY_TIME_CRITICAL:
        pri = THREAD_PRIORITY_TIME_CRITICAL;
        break;
    case LYNX_THREAD_PRIORITY_IDLE:
        pri = THREAD_PRIORITY_IDLE;
        break;
    default:
        pri = THREAD_PRIORITY_NORMAL;
    };
    return ::SetThreadPriority(::GetCurrentThread(), pri);
#else
    return true;
#endif
}

UInt32 
Thread::getThreadCount()
{
    return msThreadCount;
}
