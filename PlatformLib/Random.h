#ifndef __LYNX_UTIL_LIB_RANDOM_H__
#define __LYNX_UTIL_LIB_RANDOM_H__

#include "Compat.h"
#include "Mutex.h"
#include "Singleton.h"
#include "TimeUtil.h"

namespace Lynx
{
    class _PlatformExport Random : public Singleton<Random>
    {
    public:
        Random();
        virtual ~Random();

        void setSeed(UInt32 seed);
        UInt32 getUInt32Random(UInt32 min = 0, UInt32 max = 32767);
        float getFloatRandom(float min, float max);

    private:
        UInt32 mSeed;
        SpinMutex mSpinMutex;
    };
} // namespace Lynx

#define RANDOM() Random::getSingleton()

#endif // __LYNX_UTIL_LIB_RANDOM_H__


