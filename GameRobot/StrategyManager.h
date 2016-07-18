#ifndef __LYNX_GAME_ROBOT_STRATEGY_MANAGER_H__
#define __LYNX_GAME_ROBOT_STRATEGY_MANAGER_H__

#include "StrategyBase.h"

namespace Lynx
{
    class StrategyManager
    {
    public:
        StrategyManager(GameRobot* robot);
        virtual ~StrategyManager();

        void addStrategy(const String& strategyType, UInt32 duration);
        void update(const UInt64& accTime);

    private:
        bool execStrategy(void* args);

    private:
        GameRobot* mRobot;
        TimeoutService<3600*1000, 1000> mTimeoutService;
    };
} // namespace Lynx

#endif // __LYNX_GAME_ROBOT_STRATEGY_MANAGER_H__

