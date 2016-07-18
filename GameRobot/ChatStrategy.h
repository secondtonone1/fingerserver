#ifndef __LYNX_GAME_ROBOT_CHAT_STRATEGY_H__
#define __LYNX_GAME_ROBOT_CHAT_STRATEGY_H__

#include "StrategyBase.h"

namespace Lynx
{
    class ChatStrategy : public StrategyBase
    {
    public:
        ChatStrategy(GameRobot* robot);
        virtual ~ChatStrategy();

        virtual void onExec();
    };
} // namespace Lynx

#endif // __LYNX_GAME_ROBOT_CHAT_STRATEGY_H__