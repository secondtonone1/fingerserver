#include "ReconnectStrategy.h"

using namespace Lynx;


ReconnectStrategy::ReconnectStrategy(GameRobot* robot) : StrategyBase(robot)
{

}

ReconnectStrategy::~ReconnectStrategy()
{

}

void 
ReconnectStrategy::onExec()
{
    LOG_DEBUG("Reconnect ......");
    mRobot->closeConnection();
}


