#ifndef __LYNX_GAME_ROBOT_STRATEGY_BASE_H__
#define __LYNX_GAME_ROBOT_STRATEGY_BASE_H__

#include "GameRobot.h"

namespace Lynx
{
    class StrategyBase
    {
    public:
        StrategyBase(GameRobot* robot) : mRobot(robot) {}
        virtual ~StrategyBase() {}

        virtual void onExec() = 0;

    protected:
        GameRobot* mRobot;
    };

} // namespace Lynx

#endif // __LYNX_GAME_ROBOT_STRATEGY_BASE_H__


