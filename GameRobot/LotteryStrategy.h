#ifndef __LYNX_GAME_ROBOT_LOTTERY_STRATEGY_H__
#define __LYNX_GAME_ROBOT_LOTTERY_STRATEGY_H__

#include "StrategyBase.h"

namespace Lynx
{
    class LotteryStrategy : public StrategyBase
    {
    public:
        LotteryStrategy(GameRobot* robot);
        virtual ~LotteryStrategy();

        virtual void onExec();
    };
} // namespace Lynx

#endif // __LYNX_GAME_ROBOT_LOTTERY_STRATEGY_H__