#include "GameRandom.h"

using namespace Lynx;

UInt16 GameRandom::mSeed = 0;
UInt16* GameRandom::mRandomArray = NULL;

bool 
GameRandom::initial(const String& filePath)
{
    char* buf;
    UInt32 bufLen = RESOURCE_GROUP_MANAGER().loadFileToBuffer(filePath, &buf, "Data");
    if (bufLen != (32768 * sizeof(UInt16)))
    {
        LOG_WARN("Failed to initial GameRandom.");
        return false;
    }

    mRandomArray = (UInt16*)XALLOC(32768 * sizeof(UInt16));
    ASSERT(mRandomArray);
    memcpy(mRandomArray, buf, 32768 * sizeof(UInt16));
    RESOURCE_GROUP_MANAGER().freeBuffer(buf);

    LOG_INFO("GameRandom initial success.");
    return true;
}

void 
GameRandom::release()
{
    if (mRandomArray)
    {
        XFREE(mRandomArray);
        mRandomArray = NULL;
    }
    mSeed = 0;
}

void GameRandom::genRandomArray(const String& filePath)
{
    UInt16* randomArray = (UInt16*)XALLOC(32768 * sizeof(UInt16));
    ASSERT(randomArray);

    Random::getSingleton().setSeed(TimeUtil::getTimeSec());

    for (UInt32 i = 0; i <= 32767; ++i)
    {
        randomArray[i] = Random::getSingleton().getUInt32Random(0, 32767);
        //LOG_INFO("%d", randomArray[i]);
    }

    if (!FileUtil::saveFile(filePath, (char*)(randomArray), 32768 * sizeof(UInt16)))
    {
        LOG_WARN("Failed to gen random array.");
        return;
    }
}

void 
GameRandom::setSeed(UInt16 seed)
{
    if (seed <= 32767)
    {
        mSeed = seed;
    }
    else
    {
        LOG_WARN("Failed to set logic random seed.");
        mSeed = 0;
    }
}

UInt32
GameRandom::getUInt32Random(UInt16 min, UInt16 max)
{
    if (min > max)
    {
        LOG_WARN("Failed to get uint random for min > max.");
        return 0;
    }

    if (!mRandomArray)
    {
        LOG_WARN("Failed to get uint random for randomarray is null.");
        return 0;
    }

    UInt32 random = mRandomArray[mSeed];
    mSeed++;
    if (mSeed == 32768)
    {
        mSeed = 0;
    }

    return (UInt32)(((float)random / 32767.0f) * ((float)max - (float)min) + (float)min);
}

float 
GameRandom::getFloatRandom(float min, float max)
{
    if (min > max)
    {
        LOG_WARN("Failed to get float random for min > max.");
        return 0.0f;
    }

    if (!mRandomArray)
    {
        LOG_WARN("Failed to get float random for randomarray is null.");
        return 0.0f;
    }

    UInt32 random = mRandomArray[mSeed];
    mSeed++;
    if (mSeed == 32768)
    {
        mSeed = 0;
    }

    return (random / 32767.0f) * (max - min) + min;
}
