#ifndef __LYNX_PLATFORM_LIB_LOCK_H__
#define __LYNX_PLATFORM_LIB_LOCK_H__

namespace Lynx
{
    template <typename MutexTraits, typename LockTraits>
    class BaseLock
    {
    public:    
        BaseLock(MutexTraits& mtx): mMutex(mtx)
        {
            LockTraits::lock(mMutex);
        }

        ~BaseLock()
        {
            LockTraits::unlock(mMutex);
        }

        inline void lock()
        {
            LockTraits::lock(mMutex);
        }
    
        inline void unlock()
        {
            LockTraits::unlock(mMutex);
        }
    protected:
        MutexTraits& mMutex;
    };
} // namespace Lynx

#endif // __LYNX_PLATFORM_LIB_LOCK_H__


