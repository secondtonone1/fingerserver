#ifndef __LYNX_PLATFORM_LIB_TIMEOUT_SERVICE_H__
#define __LYNX_PLATFORM_LIB_TIMEOUT_SERVICE_H__

#include "Delegate.h"
#include "DataStruct.h"
#include "Logger.h"
#include "TimeUtil.h"

#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
#include <sys/time.h>
#endif

namespace Lynx
{
    typedef Delegate<bool (void* /*args */)> TimeoutCallback; /* return true indicate permanence */
    template <UInt64 Capacity/*millisecond*/, UInt64 Precision/*millisecond*/>
    class TimeoutService
    {
    public:
        TimeoutService(UInt32 cachesize = 100) 
            : mScale(0), mMaxScale((UInt32)(Capacity / (Precision + 1))) // begin with 0  
            , mCacheSize(cachesize), mCacheLen(0)
        {
            for (UInt32 i = 0; i <= mMaxScale; ++i)
            {
                LYNX_LIST_INIT(&mHandlerList[i]);
            }
            LYNX_LIST_INIT(&mCache);
  
            {
#if defined (_WIN32)
                mInitTime = TimeUtil::getTickCount();
#elif defined (__linux__) || defined (__FreeBSD__)
                struct timeval tv;
                gettimeofday(&tv, NULL);
                mInitTime = ((UInt64)tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
#endif
            }
        }

        ~TimeoutService() 
        {
            clean();
        }

        void clean()
        {
            struct Handler* handler;
            for (UInt32 i = 0; i < mMaxScale; ++i)
            {
                while (handler = LYNX_LIST_FIRST(&mHandlerList[i]))
                {
                    LYNX_LIST_REMOVE_HEAD(&mHandlerList[i], mEntries);
                    XDELETE(handler);
                }
            }
                
            while (handler = LYNX_LIST_FIRST(&mCache))
            {
                LYNX_LIST_REMOVE_HEAD(&mCache, mEntries);
                XDELETE(handler);
            }

            mScale = 0;
            mCacheLen = 0;
        }

        void reset()
        {
            clean();
#if defined (_WIN32)
            mInitTime = TimeUtil::getTickCount();
#elif defined (__linux__) || defined (__FreeBSD__)
            struct timeval tv;
            gettimeofday(&tv, NULL);
            mInitTime = ((UInt64)tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
#endif
        }

        void* registerCallback(UInt32 duration/*millisecond*/, const TimeoutCallback& cb, void* args)
        {
            return registerCallback(duration, cb, args, NULL);
        }

        void cancel(void* hdl)
        {
            if (hdl == NULL) return;
            struct Handler* handler = (struct Handler *)hdl;
            handler->mIndex = -2;
        }

        void tick()
        {
#if defined (_WIN32)
                UInt64 currTime = TimeUtil::getTickCount();
#elif defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
                struct timeval tv;
                gettimeofday(&tv, NULL);
                UInt64 currTime = ((UInt64)tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
#endif
            ASSERT(currTime >= mInitTime);
            UInt32 scale = (UInt32)((currTime - mInitTime) / Precision);
            if (scale == 0) return;

            UInt32 oldScale = mScale;
            mScale = scale % mMaxScale;
                
            for (UInt32 i = oldScale; i < scale && i <= mMaxScale; ++i)
            {
                doCallback(i);
            }

            if (scale >= mMaxScale)
            {
                mInitTime = currTime;
                for (UInt32 i = 0; i < mScale; ++i)
                {
                    doCallback(i);
                }
                LOG_INFO("TimeoutService loopback, currTime: %llu", mInitTime);
            }
        }

    private:
        struct Handler
        {
            Handler() : mIndex(-1), mArgs(NULL), mDuration(0) {} 
            Int32 mIndex;
            UInt32 mDuration;
            TimeoutCallback mCallback;
            void* mArgs;
            Handler* mNext;
            //LYNX_LIST_ENTRY(Handler) mEntries;
        };

        struct Handler* allocHandler()
        {
            struct Handler* handler;
            if (mCacheLen <= 0) 
            {
                handler = XNEW(struct Handler)();
                ASSERT(handler);
                return handler;
            }
				
            handler = LYNX_LIST_FIRST(&mCache);
            ASSERT(handler); 
            LYNX_LIST_REMOVE_HEAD(&mCache, mEntries);
            mCacheLen--;
            return new (handler) struct Handler();
        }

        void deallocHandler(struct Handler* handler)
        {
            ASSERT(handler);
            if (mCacheLen >= mCacheSize)
            {
                XDELETE(handler);
                return;
            }
            handler->~Handler();
            LYNX_LIST_INSERT_HEAD(&mCache, handler, mEntries);
            mCacheLen++;
        }

        void* registerCallback(UInt32 duration/*millisecond*/, 
            const TimeoutCallback& cb, void* args, Handler* handler)
        {
            if (duration <= 0 || duration > Capacity || cb.empty())
                return NULL;

            Int32 index = (duration % Precision) ? 
                ((duration / Precision) + mScale) % (mMaxScale + 1) :
                ((duration / Precision) + mScale) % (mMaxScale + 1) - 1;

            if (index < 0) index = 0;

            if (handler == NULL)
            {
                handler = allocHandler();
                if (!handler->mCallback.exist(cb)) 
                    handler->mCallback = cb; 
                handler->mArgs = args;
                handler->mDuration = duration;
            }

            handler->mIndex = index;

            if (mHandlerList[index].mUsrPtr == NULL)
                LYNX_LIST_INSERT_HEAD(&mHandlerList[index], handler, mEntries);
            else
                LYNX_LIST_INSERT_AFTER((struct Handler *)mHandlerList[index].mUsrPtr, handler, mEntries);
            mHandlerList[index].mUsrPtr = handler;
            return handler;
        }

        void doCallback(UInt32 index)
        {
            struct Handler* handler;
            struct HandlerList handlerList;
            handlerList.mFirst = mHandlerList[index].mFirst;
            //handlerList.mUsrPtr = mHandlerList[index].mUsrPtr;
            LYNX_LIST_INIT(&mHandlerList[index]);

            while (handler = LYNX_LIST_FIRST(&handlerList))
            {
                LYNX_LIST_REMOVE_HEAD(&handlerList, mEntries);
                if (handler->mIndex < 0)
                {
                    deallocHandler(handler);
                }
                else
                {
                    if (handler->mCallback(handler->mArgs))
                    {
                        if (!registerCallback(handler->mDuration, handler->mCallback, handler->mArgs, handler))
                        {
                            LOG_WARN("Failed to register timeout, handler: %p", handler);
                            deallocHandler(handler);
                        }
                    }
                    else
                    {
                        deallocHandler(handler);
                    }
                }
            }
        }

        LYNX_LIST_HEAD(HandlerList, Handler) mHandlerList[Capacity/(Precision+1)+1];
        LYNX_LIST_HEAD(HandlerCacheList, Handler) mCache;

        UInt32 mMaxScale;
        UInt64 mInitTime;     
        UInt32 mScale;
        UInt32 mCacheSize; 
        UInt32 mCacheLen;
    };
} // namespace Lynx

#endif // __LYNX_PLATFORM_LIB_TIMEOUT_SERVICE_H__


