#ifndef __LYNX_PLATFORM_LIB_THREAD_H__
#define __LYNX_PLATFORM_LIB_THREAD_H__

#include "Compat.h"

#ifndef _WIN32
# include <pthread.h>
#endif

#define LYNX_THREAD_PRIORITY_LOWEST 0
#define LYNX_THREAD_PRIORITY_BELOW_NORMAL 1
#define LYNX_THREAD_PRIORITY_NORMAL 2
#define LYNX_THREAD_PRIORITY_HIGHEST 3
#define LYNX_THREAD_PRIORITY_ABOVE_NORMAL 4
#define LYNX_THREAD_PRIORITY_ERROR_RETURN 5
#define LYNX_THREAD_PRIORITY_TIME_CRITICAL 6
#define LYNX_THREAD_PRIORITY_IDLE 7

namespace Lynx
{
    class _PlatformExport Thread
    {
#if defined (_WIN32)
        typedef unsigned int Id;
#else
        typedef pthread_t Id;
#endif

    public:
        Thread();
        virtual ~Thread();

        bool startup(unsigned int stackSize = 1024 * 1024 * 2);
        virtual void shutdown();
        void join();
        bool isShutdown();
        bool setPriority(UInt32 priority);

        static unsigned int getThreadId();

        virtual void threadFunc() = 0;

        static UInt32 getThreadCount();
    protected:
        Id mId;
#if !defined (_WIN32)
        volatile unsigned long mShutdown;   
#else
        volatile long mShutdown;
#endif

#if defined (_WIN32)
        void* mThread;
#endif
    private:
        static UInt32 msThreadCount;
    };   
} // namespace Lynx    

#endif // __LYNX_PLATFORM_LIB_THREAD_H__


