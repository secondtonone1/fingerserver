#ifndef __LYNX_PLATFROM_LIB_LRU_CACHEEX_H__
#define __LYNX_PLATFROM_LIB_LRU_CACHEEX_H__

#include "Container.h"
#include "TimeUtil.h"

namespace Lynx
{

    //-----------------------------------------------------------------------------------------------------------------------------
    //    add by liu jiang, 增加一个在回调用可以区分是否是clear标志的lrucache
    //-----------------------------------------------------------------------------------------------------------------------------
    // 回调函数返回为假， 表示此对象不能删除， 可能是因为资源正在使用
    // LruCache应该更新该对象存活时间， 不从Map中把对象移除
    typedef Delegate<bool (void* dataPointer, bool isClear)> LruCacheRemoveCallbackEx;

    struct LruElementEx
    {
        UInt32 mOperateTime;
        void* mData;
    };

    template <typename PointerType>
    class LruCacheEx
    {
    public:
        LruCacheEx(LruCacheRemoveCallbackEx removeCallback, UInt32 cacheTime/*second*/) 
            : mCacheTime(cacheTime), mLastUpdateTime(0)
        {
            mRemoveCallback = removeCallback;
            mUpdateInterval = cacheTime / 2;
        }
        ~LruCacheEx() { clear(); }

        void clear()
        {
            for (StringMap<LruElementEx>::Iter* iter = mElementMap.begin();
                iter != mElementMap.end(); iter = mElementMap.next(iter))
            {
                // 此处不做判断回调函数返回值， 直接删除。
                mRemoveCallback(iter->mValue.mData, true);
            }
            mElementMap.clear();
        }

        UInt32 size() const
        {
            return mElementMap.size();
        }

        void add(const String& name, PointerType pointer)
        {
            LruElementEx elem;
            elem.mData = pointer;
            elem.mOperateTime = TimeUtil::getLocalTimeSec();
            mElementMap.insert(name, elem);
        }

        //void remove(const String& name)
        //{
        //    StringMap<LruElementEx>::Iter* iter = mElementMap.find(name);
        //    if (iter != mElementMap.end())
        //    {
        //        // 此处不做判断回调函数返回值， 直接删除。
        //        mRemoveCallback(iter->mValue.mData);
        //        mElementMap.erase(iter);
        //    }
        //}

        PointerType get(const String& name)
        {
            StringMap<LruElementEx>::Iter* iter = mElementMap.find(name);
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

            LruElementEx* elem = NULL;
            for (StringMap<LruElementEx>::Iter* iter = mElementMap.begin();
                iter != mElementMap.end();)
            {
                if (iter->mValue.mOperateTime + mCacheTime <= currTime)
                {
                    // 回调函数返回为假， 表示此对象不能删除， 可能是因为资源正在使用
                    // LruCache应该更新该对象存活时间， 不从Map中把对象移除
                    if (!mRemoveCallback(iter->mValue.mData, false))
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
        StringMap<LruElementEx> mElementMap;
        LruCacheRemoveCallbackEx mRemoveCallback;
        UInt32 mCacheTime;
        UInt32 mUpdateInterval;
        UInt32 mLastUpdateTime;
    };
} // namespace Lynx


#endif // __LYNX_PLATFROM_LIB_LRU_CACHEEX_H__

