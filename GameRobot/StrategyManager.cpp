#include "StrategyManager.h"
#include "ChatStrategy.h"
#include "ReconnectStrategy.h"
#include "BattleStrategy.h"
#include "LotteryStrategy.h"

using namespace Lynx;

StrategyManager::StrategyManager(GameRobot* robot) : mRobot(robot)
{

}

StrategyManager::~StrategyManager()
{
    mRobot = NULL;
}

void 
StrategyManager::addStrategy(const String& strategyType, UInt32 duration)
{
    StrategyBase* strategy = NULL;

    String type = strategyType;
    lynxToLowerCase(type);

    if (type == "reconnect")
    {
        strategy = XNEW(ReconnectStrategy)(mRobot);
    }
    else if (type == "chat")
    {
        strategy = XNEW(ChatStrategy)(mRobot);
    }
    else if (type == "lottery")
    {
        strategy = XNEW(LotteryStrategy)(mRobot);
    }
    else if (type == "battle")
    {
        strategy = XNEW(BattleStrategy)(mRobot);
    }
    else
    {
        LOG_WARN("Failed to add strategy %s", strategyType.c_str());
        return;
    }

    mTimeoutService.registerCallback(duration, TimeoutCallback(this, &StrategyManager::execStrategy), strategy);
}

bool 
StrategyManager::execStrategy(void* args)
{
    StrategyBase* strategy = (StrategyBase*)(args);
    if (strategy)
    {
        strategy->onExec();
    }

    return true;
}

void 
StrategyManager::update(const UInt64& accTime)
{
    mTimeoutService.tick();
    if (mRobot)
    {
        mRobot->update(accTime);
    }
}

