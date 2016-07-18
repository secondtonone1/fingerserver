#ifndef __LYNX_PLATFORM_LIB_SINGLETON_H__
#define __LYNX_PLATFORM_LIB_SINGLETON_H__

#include <stdlib.h>
#include "Thread.h"

#define MAX_THREAD_ID_NUMBER 100000

namespace Lynx
{
#ifndef _LYNX_THREAD_SINGLETON
    // Singleton object MUST BE initialized manually.
    template <typename Type>
    class Singleton  // Threadsafe support, Can't be used at static field
    {
    public:
        inline static Type& getSingleton() 
        {
            return sInstance;
        }
        inline static Type* getSingletonPtr()
        {
            return &sInstance;
        }
    protected:
        Singleton() {}
        virtual ~Singleton() {}
    private:
        static Type sInstance;
    };
    template <typename Type>
    Type Singleton<Type >::sInstance;

#else
    template <typename Type>
    class Singleton  // Threadsafe support, Can't be used at static field
    {
    public:
        static Type& getSingleton() 
        {
            UInt32 threadId = Thread::getThreadId();
            //ASSERT(threadId < MAX_THREAD_ID_NUMBER);
            if (sInstanceArray[threadId] == NULL)
            {
                Type* instance = new Type();
                sInstanceArray[threadId] = instance;
            }
            return *sInstanceArray[threadId];
        }

        static Type* getSingletonPtr()
        {
            UInt32 threadId = Thread::getThreadId();
            //ASSERT(threadId < MAX_THREAD_ID_NUMBER);
            if (sInstanceArray[threadId] == NULL)
            {
                Type* instance = new Type();
                sInstanceArray[threadId] = instance;
            }
            return sInstanceArray[threadId];
        }

    protected:
        Singleton() {}
        virtual ~Singleton() {}
    private:
        static Type* sInstanceArray[MAX_THREAD_ID_NUMBER];
    };
    template <typename Type>
    Type* Singleton<Type >::sInstanceArray[MAX_THREAD_ID_NUMBER];
#endif
} // namespace Lynx

#endif // __LYNX_PLATFORM_LIB_SINGLETON_H__


