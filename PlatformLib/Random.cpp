#include "Random.h"

using namespace Lynx;


Random::Random() : mSeed(0)
{

}

Random::~Random()
{

}

void 
Random::setSeed(UInt32 seed)
{
    if (!seed)
    {
        mSeed = TimeUtil::getTimeSec();
    }
    else
    {
        mSeed = seed;
    }
}

UInt32 
Random::getUInt32Random(UInt32 min, UInt32 max)
{
    if (min > max)
    {
        UInt32 tmp = max;
        max = min;
        min = tmp;
    }

    if (max - min == 0xffffffff)
    {
        return min;
    }

    SpinLock lock(mSpinMutex);
    mSeed = 214013 * mSeed + 2531011;
    UInt32 result = min + (mSeed ^ mSeed >> 15) % (max - min + 1);
    return result;
}

float 
Random::getFloatRandom(float min, float max)
{
    if (min > max)
    {
        float tmp = max;
        max = min;
        min = tmp;
    }

    if (FLOAT_EQUAL(max, min))
    {
        return min;
    }

    SpinLock lock(mSpinMutex);
    mSeed = 214013 * mSeed + 2531011;
    float result = min + (mSeed >> 16) * (1.0f / 65535.0f) * (max - min);
    return result;
}

