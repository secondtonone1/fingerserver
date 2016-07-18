#ifndef __LYNX_GAME_SERVER_LOGIC_RANDOM_H__
#define __LYNX_GAME_SERVER_LOGIC_RANDOM_H__

#include "MainThread.h"

namespace Lynx
{
	class GameRandom : public MainThread
	{
    public:
        static bool initial(const String& filePath);
        static void release();

        static void setSeed(UInt16 seed);
        static UInt32 getUInt32Random(UInt16 min = 0, UInt16 max = 32767);
        static float getFloatRandom(float min, float max);

        static void genRandomArray(const String& filePath);

    private:
        static UInt16 mSeed;
        static UInt16* mRandomArray;
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_LOGIC_RANDOM_H__

