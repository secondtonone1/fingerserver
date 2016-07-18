#ifndef __LYNX_PLATFORM_LIB_FIFO_H__
#define __LYNX_PLATFORM_LIB_FIFO_H__

#include "Mutex.h"
#include "Condition.h"
#include "TimeUtil.h"
#include "Container.h"

namespace Lynx
{
    template <typename Type >
    class Fifo
    {
    public:
        Fifo(const Type& nullValue = 0) : mNullValue(nullValue) {}
        ~Fifo() {}

        inline UInt32 size() const
        {
            lock lock(mMutex);
            return mList.size();
        }

        inline bool empty() const
        {
            lock lock(mMutex);
            return mList.empty();
        }
         
        inline void clear()
        {
            lock lock(mMutex);
            mList.clear();
            mCondition.signal();
        }
         
        inline void push(Type value)
        {
            lock lock(mMutex);
            mList.insertTail(value);
            mCondition.signal();
        }

        inline void pushHead(Type value)
        {
            lock lock(mMutex);
            mList.insertHead(value);
            mCondition.signal();
        }

        Type pop()
        {
            lock lock(mMutex);

            if (mList.empty())
            {
                return mNullValue;
            }            
            // Return the first message on the fifo.
            typename List<Type >::Iter* iter = mList.begin();
            if (iter == NULL) return mNullValue;
            Type value = iter->mValue;
            mList.erase(iter);
            return value;
        }

        Vector<Type > pops(UInt32 maxCount = 10)
        {
            Vector<Type > retVector;
            lock lock(mMutex);

            if (mList.empty())
            {
                return retVector;
            }

            while (1)
            {
                for (typename List<Type >::node* node = mList.begin(); node != mList.end();)
                {
                    if (maxCount <= 0) 
                    {
                        break;
                    }
                    retVector.push_back(node->mValue);
                    node = mList.erase(node);
                    maxCount--;
                }
                return retVector;
            }
        }

        Type getNext()
        {
            lock lock(mMutex);
            
            // wait util there are messages available.
            while (mList.empty())
            {
                mCondition.wait(mMutex);
            }
            
            // Return the first message on the fifo.
            typename List<Type >::node* node = mList.begin();
            if (node == NULL) return mNullValue;
            Type value = node->mValue;
            mList.erase(node);
            return value;
        }
         
        Type getNext(UInt32 ms)
        {
            ASSERT(ms > 0);
            const UInt64 begin(TimeUtil::getLocalTimeMilliSec());
            const UInt64 end(begin + (unsigned int)(ms)); 
            lock lock(mMutex); 

            // wait until there are messages available
            while (mList.empty())
            {
               const UInt64 now(TimeUtil::getLocalTimeMilliSec());
               if(now >= end)
               {
                   return mNullValue;
               }

               unsigned int timeout((unsigned int)(end - now));
              
               // bail if total wait time exceeds limit
               bool signaled = mCondition.wait(mMutex, timeout);
               if (!signaled)
               {
                   return mNullValue;
               }
            }

            // Return the first message on the fifo.
            typename List<Type >::node* node = mList.begin();
            if (node == NULL) return mNullValue;
            Type value = node->mValue;
            mList.erase(node);
            return value;
        } 

        Vector<Type > getNexts(UInt32 ms, Int32 maxCount = 10)
        {
            ASSERT(ms > 0);
            const UInt64 begin(TimeUtil::getLocalTimeMilliSec());
            const UInt64 end(begin + (unsigned int)(ms)); 
            Vector<Type > retVector;
            lock lock(mMutex); 

            // wait until there are messages available
            while (mList.empty())
            {
               const UInt64 now(TimeUtil::getLocalTimeMilliSec());
               if(now >= end)
               {
                   return retVector;
               }

               unsigned int timeout((unsigned int)(end - now));
              
               // bail if total wait time exceeds limit
               bool signaled = mCondition.wait(mMutex, timeout);
               if (!signaled)
               {
                   return retVector;
               }
            }

            while (1)
            {
                for (typename List<Type >::node* node = mList.begin(); node != mList.end();)
                {
                    if (maxCount <= 0) 
                    {
                        break;
                    }
                    retVector.push_back(node->mValue);
                    node = mList.erase(node);
                    maxCount--;
                }
                return retVector;
            }
        }
            
    protected:
        Type mNullValue;
        List<Type > mList;
        mutable Mutex mMutex;
        Condition mCondition;
    };
} // namespace Lynx

#endif // __LYNX_PLATFORM_LIB_FIFO_H__



