#include "ChatStrategy.h"

using namespace Lynx;


ChatStrategy::ChatStrategy(GameRobot* robot) : StrategyBase(robot)
{

}

ChatStrategy::~ChatStrategy()
{

}

void 
ChatStrategy::onExec()
{
    mRobot->sendChatMsg(mRobot->getPlayerData().mBaseData.mPlayerId, "abcd");
}


