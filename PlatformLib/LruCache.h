#ifndef __LYNX_PLATFROM_LIB_LRU_CACHE_H__
#define __LYNX_PLATFROM_LIB_LRU_CACHE_H__

#include "PlatformLib.h"

namespace Lynx
{
    // 回调函数返回为假， 表示此对象不能删除， 可能是因为资源正在使用
    // LruCache应该更新该对象存活时间， 不从Map中把对象移除
    typedef Delegate<bool (void* dataPointer)> LruCacheRemoveCallback;

    struct LruElement
    {
        UInt32 mOperateTime;
        void* mData;
    };

    template <typename PointerType>
    class LruCache
    {
    public:
        LruCache(LruCacheRemoveCallback removeCallback, UInt32 cacheTime/*second*/) 
            : mCacheTime(cacheTime), mLastUpdateTime(0)
        {
            mRemoveCallback = removeCallback;
            mUpdateInterval = cacheTime / 2;
        }
        ~LruCache() { clear(); }

        void clear()
        {
            for (StringMap<LruElement>::Iter* iter = mElementMap.begin();
                iter != mElementMap.end(); iter = mElementMap.next(iter))
            {
                // 此处不做判断回调函数返回值， 直接删除。
                mRemoveCallback(iter->mValue.mData);
            }
            mElementMap.clear();
        }

        UInt32 size() const
        {
            return mElementMap.size();
        }

        void add(const String& name, PointerType pointer)
        {
            LruElement elem;
            elem.mData = pointer;
            elem.mOperateTime = TimeUtil::getLocalTimeSec();
            mElementMap.insert(name, elem);
        }

        //void remove(const String& name)
        //{
        //    StringMap<LruElement>::Iter* iter = mElementMap.find(name);
        //    if (iter != mElementMap.end())
        //    {
        //        // 此处不做判断回调函数返回值， 直接删除。
        //        mRemoveCallback(iter->mValue.mData);
        //        mElementMap.erase(iter);
        //    }
        //}

        PointerType get(const String& name)
        {
            StringMap<LruElement>::Iter* iter = mElementMap.find(name);
            if (iter != mElementMap.end())
            {
                iter->mValue.mOperateTime = TimeUtil::getLocalTimeSec();
                return (PointerType)iter->mValue.mData;
            }
            return NULL;
        }

        void update()
        {
            UInt32 currTime = TimeUtil::getLocalTimeSec();
            if (currTime - mLastUpdateTime < mUpdateInterval)
            {
                return;
            }
                
            LruElement* elem = NULL;
            for (StringMap<LruElement>::Iter* iter = mElementMap.begin();
                iter != mElementMap.end();)
            {
                if (iter->mValue.mOperateTime + mCacheTime <= currTime)
                {
                    // 回调函数返回为假， 表示此对象不能删除， 可能是因为资源正在使用
                    // LruCache应该更新该对象存活时间， 不从Map中把对象移除
                    if (!mRemoveCallback(iter->mValue.mData))
                    {
                        iter->mValue.mOperateTime = TimeUtil::getLocalTimeSec();
                        iter = mElementMap.next(iter);
                    }
                    else
                    {
                        iter = mElementMap.erase(iter);
                    }
                }
                else
                {
                    iter = mElementMap.next(iter);
                }
            }
        }
    private:
        StringMap<LruElement> mElementMap;
        LruCacheRemoveCallback mRemoveCallback;
        UInt32 mCacheTime;
        UInt32 mUpdateInterval;
        UInt32 mLastUpdateTime;
    };
} // namespace Lynx


#endif // __LYNX_PLATFROM_LIB_LRU_CACHE_H__

