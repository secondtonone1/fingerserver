#include "LotteryStrategy.h"

using namespace Lynx;


LotteryStrategy::LotteryStrategy(GameRobot* robot) : StrategyBase(robot)
{

}

LotteryStrategy::~LotteryStrategy()
{

}

void 
LotteryStrategy::onExec()
{
    if (Random::getSingleton().getUInt32Random() % 2)
    {
        mRobot->sendGetOneHeroMsg();
    }
    else
    {
        mRobot->sendGetTenHeroMsg();
    }
}


