#ifndef __LYNX_GAME_ROBOT_RECONNECT_STRATEGY_H__
#define __LYNX_GAME_ROBOT_RECONNECT_STRATEGY_H__

#include "StrategyBase.h"

namespace Lynx
{
    class ReconnectStrategy : public StrategyBase
    {
    public:
        ReconnectStrategy(GameRobot* robot);
        virtual ~ReconnectStrategy();

        virtual void onExec();
    };
} // namespace Lynx

#endif // __LYNX_GAME_ROBOT_RECONNECT_STRATEGY_H__