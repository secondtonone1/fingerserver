#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_ACHIEVEMENT_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_ACHIEVEMENT_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class AchievementManager : public MainThread
    {
    public:
        AchievementManager();
        virtual ~AchievementManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);

        void checkAchievement(UInt8 type, UInt32 value = 0);

        ErrorId getAchievementAward(UInt32 templateId, bool& maxLevel, UInt32& curTemplateId);

    private:
        Player* mPlayer;
      //  PlayerAchievementData* mPlayerAchievementData;
        Map<UInt8, UInt32> mMaxLevelTemplateIdMap;
        bool mDrity;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_ACHIEVEMENT_MAMAGER_H__
