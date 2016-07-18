#include "ProduceManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

ProduceManager::ProduceManager(): mPlayer(NULL)
{

}


ProduceManager::~ProduceManager()
{

}
 
bool 
ProduceManager::initial(Player* player)
{
    if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }
    mPlayer = player;
    //mPlayerData.mProduceData 字段不存在 sec
	//mPlayerProduceData = &player->mPlayerData.mProduceData;

    return true;
}

void
ProduceManager::release()
{

}

bool 
ProduceManager::upgreadGoldMineral()
{
	//注释金矿 sec
	/* UInt32 currentLevel = mPlayer->getPlayerData().mProduceData.mGoldMineralLevel;

    ProduceTemplate* produceTemplate = PRODUCE_TABLE().get(currentLevel);
    if (!produceTemplate)
    {
        LOG_WARN("Failed to get produceTemplate %u", currentLevel);
        return false;
    }

    if (mPlayer->getPlayerData().mBaseData.mSoul < produceTemplate->mGoldMineralConsumeSoul)
    {
        LOG_WARN("Failed to upgreadGoldMineral for soul isn't full.");
        return false;
    }
    else
    {
        mPlayer->reducePlayerSoul(produceTemplate->mGoldMineralConsumeSoul);
        CalculateUtil::calculatePlayerGoldMineralData(mPlayer->mPlayerData);
        mPlayerProduceData->mGoldMineralLevel += 1;
        mPlayer->setPlayerDataDirty();
    }

    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_GOLD_MINE_LEVEL);*/

    return true;
}

bool 
ProduceManager::upspeedGoldMineral()
{
    //原游戏金矿加速 sec 注释
	/*ConstItemIdTemplate* constItemIdTemplate = CONST_ITEM_ID_TABLE().get(1);
    if (!constItemIdTemplate)
    {
        LOG_WARN("Failed to get const item id template.");
        return false;
    }

    const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(constItemIdTemplate->mGoldDummyId);
    if (!itemData)
    {
        LOG_WARN("Failed to upspeed gold mineral for item isn't exist.");
        return false;
    }

    if (itemData->mCount < 1)
    {
        LOG_WARN("Failed to upspeed gold mineral for item count invalid.");
        return false;
    }

    if (!mPlayerProduceData->mGoldMineralSpeedUpTime)
    {
        mPlayerProduceData->mGoldMineralStartSpeedTime = TimeUtil::getTimeSec();
    }
    mPlayerProduceData->mGoldMineralSpeedUpTime += 3600;
    mPlayer->setPlayerDataDirty();
    mPlayer->getItemManager().delItem(itemData->mItemId, 1);*/
    return true;
}

bool 
ProduceManager::upgreadDiamondMineral()
{
    //getPlayerData().mProduceData sec
	/*UInt32 currentLevel = mPlayer->getPlayerData().mProduceData.mDiamondMineralLevel;

    ProduceTemplate* produceTemplate = PRODUCE_TABLE().get(currentLevel);
    if (!produceTemplate)
    {
        LOG_WARN("Failed to get produceTemplate %u", currentLevel);
        return false;
    }

    if (mPlayer->getPlayerData().mBaseData.mSoul < produceTemplate->mDiamondMineralConsumeSoul)
    {
        LOG_WARN("Failed to upgreadDiamondMineral for soul isn't full.");
        return false;
    }
    else
    {
        mPlayer->reducePlayerSoul(produceTemplate->mDiamondMineralConsumeSoul);       
        CalculateUtil::calculatePlayerProduceData(mPlayer->mPlayerData);
        mPlayerProduceData->mDiamondMineralLevel += 1;
        mPlayer->setPlayerDataDirty();
    }

    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_DIAMOND_MINE_LEVEL);*/

    return true;
}

bool 
ProduceManager::upspeedDiamondMineral()
{
	//sec注释原游戏代码，加速钻石产出
	/* ConstItemIdTemplate* constItemIdTemplate = CONST_ITEM_ID_TABLE().get(1);
    if (!constItemIdTemplate)
    {
        LOG_WARN("Failed to get const item id template.");
        return false;
    }

    const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(constItemIdTemplate->mDiamondDummyId);
    if (!itemData)
    {
        LOG_WARN("Failed to upspeed diamond mineral for item isn't exist.");
        return false;
    }

    if (itemData->mCount < 1)
    {
        LOG_WARN("Failed to upspeed diamond mineral for item count invalid.");
        return false;
    }

    if (!mPlayerProduceData->mDiamondMineralSpeedUpTime)
    {
        mPlayerProduceData->mDiamondMineralStartSpeedTime = TimeUtil::getTimeSec();
    }
    mPlayerProduceData->mDiamondMineralSpeedUpTime += 3600;
    mPlayer->setPlayerDataDirty();
    mPlayer->getItemManager().delItem(itemData->mItemId, 1);*/
    return true;
}

bool 
ProduceManager::upgreadExpMineral()
{
	//getPlayerData().mProduceData sec

	/*UInt32 currentLevel = mPlayer->getPlayerData().mProduceData.mExpMineralLevel;

    ProduceTemplate* produceTemplate = PRODUCE_TABLE().get(currentLevel);
    if (!produceTemplate)
    {
        LOG_WARN("Failed to get produceTemplate %u", currentLevel);
        return false;
    }

    if (mPlayer->getPlayerData().mBaseData.mSoul < produceTemplate->mExpMineralConsumeSoul)
    {
        LOG_WARN("Failed to upgreadExpMineral for soul isn't full.");
        return false;
    }
    else
    {
        mPlayer->reducePlayerSoul(produceTemplate->mExpMineralConsumeSoul);
        CalculateUtil::calculatePlayerProduceData(mPlayer->mPlayerData);
        mPlayerProduceData->mExpMineralLevel += 1;
        mPlayer->setPlayerDataDirty();
    }

    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_EXP_SPRING_LEVEL);*/

    return true;
}

bool 
ProduceManager::upspeedExpMineral()
{
   //sec注释原游戏代码，加速经验产出
	/* ConstItemIdTemplate* constItemIdTemplate = CONST_ITEM_ID_TABLE().get(1);
    if (!constItemIdTemplate)
    {
        LOG_WARN("Failed to get const item id template.");
        return false;
    }

    const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(constItemIdTemplate->mExpDummyId);
    if (!itemData)
    {
        LOG_WARN("Failed to upspeed diamond mineral for item isn't exist.");
        return false;
    }

    if (itemData->mCount < 1)
    {
        LOG_WARN("Failed to upspeed diamond mineral for item count invalid.");
        return false;
    }

    if (!mPlayerProduceData->mExpMineralSpeedUpTime)
    {
        mPlayerProduceData->mExpMineralStartSpeedTime = TimeUtil::getTimeSec();
    }
    mPlayerProduceData->mDiamondMineralSpeedUpTime += 3600;
    mPlayer->setPlayerDataDirty();
    mPlayer->getItemManager().delItem(itemData->mItemId, 1);*/
    return true;
}

UInt32 
ProduceManager::getGoldMineral()
{
    CalculateUtil::calculatePlayerProduceData(mPlayer->mPlayerData);
    UInt32 goldMineralCount = mPlayerProduceData->mRemainGoldMineralCount;
    mPlayerProduceData->mRemainGoldMineralCount = 0;
    mPlayer->setPlayerDataDirty();
    return goldMineralCount;
}

UInt32 
ProduceManager::getDiamondMineral()
{
    CalculateUtil::calculatePlayerProduceData(mPlayer->mPlayerData);
    UInt32 diamondMineralCount = mPlayerProduceData->mRemainDiamondMineralCount;
    mPlayerProduceData->mRemainDiamondMineralCount = 0;
    mPlayer->setPlayerDataDirty();
    return diamondMineralCount;
}
