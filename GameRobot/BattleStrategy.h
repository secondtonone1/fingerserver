#ifndef __LYNX_GAME_ROBOT_BATTLE_STRATEGY_H__
#define __LYNX_GAME_ROBOT_BATTLE_STRATEGY_H__

#include "StrategyBase.h"

namespace Lynx
{
    class BattleStrategy : public StrategyBase
    {
    public:
        BattleStrategy(GameRobot* robot);
        virtual ~BattleStrategy();

        virtual void onExec();
    };
} // namespace Lynx

#endif // __LYNX_GAME_ROBOT_BATTLE_STRATEGY_H__