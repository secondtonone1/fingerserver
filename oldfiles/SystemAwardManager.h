#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_SYSTEM_AWARD_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_SYSTEM_AWARD_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class SystemAwardManager : public MainThread
    {
    public:
        SystemAwardManager();
        virtual ~SystemAwardManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);

        // 获取连续登陆奖励
        ErrorId getContinousLoginAward(UInt32 loginDay);
        // 获取累积登陆奖励
        ErrorId getAccumulateLoginAward(UInt32 loginDay);
        // 获取等级奖励
        ErrorId getLevelAward(UInt32 level);

    private:
        Player* mPlayer;
        UInt32 mCurTime;
        UInt32 mOnlineDateFlag;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_SYSTEM_AWARD_MAMAGER_H__
