#include "TowerManager.h"
#include "GameServer.h"

using namespace Lynx;

#define TOWER_FIRST_FLOOR 300011

TowerManager::TowerManager() : mPlayer(NULL), mTowerIdInBattle(0),
mCheckResetInterval(0), mTowerResetTime(0)
{
    //mTollgateMap.clear();
    //mPlayerPresentHeroDataMap.clear();
}


TowerManager::~TowerManager()
{
    mPlayer = NULL;
    mPlayerTowerData = NULL;
}

bool 
TowerManager::initial(Player* player)
{
    if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }

    mPlayer = player;
	//mPlayerData.mTowerData  sec
    //mPlayerTowerData = &player->mPlayerData.mTowerData;

    //if (mPlayerTowerData->mTowerDataList.size() == 0)
    //{
    //    TemplateId towerId = TOWER_FIRST_FLOOR;
    //    mPlayerTowerData->mCurrentTowerId = towerId;
    //    mPlayerTowerData->mCurrentTowerFloor = 1;
    //    for (int i = 0; i < TOWER_FLOOR_MAX_COUNT; i++)
    //    {
    //        TowerData towerData;
    //        towerData.mTowerId = towerId;
    //        towerData.mStarCount = 0;
    //        mPlayerTowerData->mTowerDataList.insertTail(towerData);
    //        towerId += 10;
    //    }
    //}

    //mTowerIdInBattle = 0;
    //mTowerMonsterList.clear();
    //mSelectBufferList.clear();

    //mCheckResetInterval = TimeUtil::getTimeSec();
    //mTowerResetTime = TimeUtil::getTodayBaseSec() + TOWER_RESET_TIME;

    //// 重置爬塔数据
    //if (mPlayer->getPlayerData().mAccountData.mLogoutTime <= mTowerResetTime
    //    && TimeUtil::getTimeSec() >= mTowerResetTime)
    //{
    //    refreshTowerInfo();
    //    mTowerResetTime = 24 * 3600 + mTowerResetTime;
    //}

    return true;
}

void 
TowerManager::update(const UInt64& accTime)
{
    // 每60秒更新一次
    if (mCheckResetInterval + 60 < TimeUtil::getTimeSec())
    {
        if (TimeUtil::getTimeSec() > mTowerResetTime)
        {
            refreshTowerInfo();
            mTowerResetTime = 24 * 3600 + mTowerResetTime;
        }
        mCheckResetInterval = TimeUtil::getTimeSec();
    }
}

void 
TowerManager::release()
{

}

void 
TowerManager::syncTowerDataToClient()
{
    SyncTowerDataToClientNotify notify;
    notify.mPlayerTowerData = *mPlayerTowerData;
    mPlayer->sendMsg(notify);
}

void 
TowerManager::saveTowerData()
{
    //mPlayerData.mTowerData 字段注释 sec

	/*PersistUpdatePlayerTowerDataNotify updateTowerNotify;
    updateTowerNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    updateTowerNotify.mTowerData = mPlayer->mPlayerData.mTowerData;
    PersistSystem::getSingleton().postThreadMsg(updateTowerNotify, mPlayer->getPlayerGuid());*/
}

void 
TowerManager::openTowerBuffer()
{
    OpenTowerBufferResp resp;
    resp.mErrorId = LynxErrno::None;

    if (mPlayer->getPlayerLeval() < TOWER_OPEN_LEVEL_LIMIT)
    {
        resp.mErrorId = LynxErrno::TowerOpenLevelLimit;
        mPlayer->sendMsg(resp);
        return;
    }

    if (mPlayerTowerData->mIsResetEnable)
    {
        resp.mErrorId = LynxErrno::TowerPleaseReset;
        mPlayer->sendMsg(resp);
        return;
    }

    mSelectBufferList.clear();
    for (int i = 0; i < 3; i++)
    {
        TowerBufferData towerBufferData;
        if (genTowerBufferData(i + 1, towerBufferData))
        {
            resp.mBufferList.insertTail(towerBufferData);
            mSelectBufferList.insertTail(towerBufferData);
        }
    }
    mPlayer->sendMsg(resp);
}

bool 
TowerManager::genTowerBufferData(UInt32 index, TowerBufferData& towerBufferData)
{
    TowerBufferTemplate* towerBufferTemplate = NULL;
    switch (index)
    {
    case 1:
        {
            towerBufferTemplate = TOWER_BUFFER_TABLE().get(1);
        }
    	break;
    case 2:
        {
            towerBufferTemplate = TOWER_BUFFER_TABLE().get(2);
        }
        break;
    case 3:
        {
            towerBufferTemplate = TOWER_BUFFER_TABLE().get(3);
        }
        break;
    }

    if (towerBufferTemplate == NULL)
    {
        return false;
    }

    if (towerBufferTemplate->mTowerBufferList.size() == 0)
    {
        return false;
    }

    UInt32 randomValue = RANDOM().getUInt32Random(0, towerBufferTemplate->mTowerBufferList.size() - 1);
    List<TowerBufferLine>::Iter* iter = towerBufferTemplate->mTowerBufferList.getn(randomValue);
    if (iter == NULL)
    {
        return false;
    }

    towerBufferData.mBufferType = iter->mValue.mBufferType;
    towerBufferData.mBufferValue = iter->mValue.mBufferValue;
    towerBufferData.mBufferIcon = iter->mValue.mBufferIcon;
    return true;
}

void 
TowerManager::selectTowerBuffer(UInt32 index)
{
    SelectBufferResp resp;
    resp.mErrorId = LynxErrno::None;

    if (mPlayer->getPlayerLeval() < TOWER_OPEN_LEVEL_LIMIT)
    {
        resp.mErrorId = LynxErrno::TowerOpenLevelLimit;
        mPlayer->sendMsg(resp);
        return;
    }

    if (mPlayerTowerData->mIsResetEnable)
    {
        resp.mErrorId = LynxErrno::TowerPleaseReset;
        mPlayer->sendMsg(resp);
        return;
    }

    UInt32 costStar = 0;
    switch (index)
    {
    case 1:
        costStar = 2;
        break;
    case 2:
        costStar = 4;
        break;
    case 3:
        costStar = 6;
        break;
    default:
        resp.mErrorId = LynxErrno::TowerBufferSelectNotExist;
        mPlayer->sendMsg(resp);
        return;
    }

    if (costStar > mPlayerTowerData->mUsableStars)
    {
        resp.mErrorId = LynxErrno::TowerBufferStarNotEnough;
        mPlayer->sendMsg(resp);
        return;
    }

    if (!addTowerBuffer(index))
    {
        resp.mErrorId = LynxErrno::TowerBufferAddFailed;
        mPlayer->sendMsg(resp);
        return;
    }

    mPlayerTowerData->mUsableStars -= costStar;
    syncTowerDataToClient();
    saveTowerData();

    mPlayer->sendMsg(resp);
}

bool 
TowerManager::addTowerBuffer(UInt32 index)
{
    List<TowerBufferData>::Iter* iter = mSelectBufferList.getn(index - 1);
    if (iter == NULL)
    {
        LOG_WARN("Get select buffer index %u failed.", index);
        return false;
    }

    if (iter->mValue.mBufferType == 0 || iter->mValue.mBufferType > 7)
    {
        return false;
    }

    bool isExist = false;
    for (List<TowerBufferData>::Iter* myIter = mPlayerTowerData->mTowerBufferList.begin();
        myIter != mPlayerTowerData->mTowerBufferList.end();
        myIter = mPlayerTowerData->mTowerBufferList.next(myIter))
    {
        if (myIter->mValue.mBufferType == iter->mValue.mBufferType)
        {
            isExist = true;
            myIter->mValue.mBufferValue += iter->mValue.mBufferValue;
            break;
        }
    }

    if (!isExist)
    {
        mPlayerTowerData->mTowerBufferList.insertTail(iter->mValue);
    }
    return true;
}

void 
TowerManager::startTowerBattle(UInt8 tollgateDifficulty)
{
    /*mTowerMonsterList.clear();
    UInt32 tollgateId = mPlayerTowerData->mCurrentTowerId;
    StartTowerBattleResp resp;
    resp.mTollgateId = tollgateId;
    resp.mTollgateDifficulty = tollgateDifficulty;

    if (mPlayer->getPlayerLeval() < TOWER_OPEN_LEVEL_LIMIT)
    {
        resp.mErrorId = LynxErrno::TowerOpenLevelLimit;
        mPlayer->sendMsg(resp);
        return;
    }

    if (LogicSystem::getSingleton().checkBagIsFull(mPlayer))
    {
        resp.mErrorId = LynxErrno::BagIsFull;
        mPlayer->sendMsg(resp);
        return;
    }

    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    if (tollgateTemplate == NULL)
    {
        LOG_WARN("Get tollgate template failed tollgate id:%u", tollgateId);
        resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
        mPlayer->sendMsg(resp);
        return;
    }

    if (mPlayerTowerData->mIsResetEnable)
    {
        resp.mErrorId = LynxErrno::TowerPleaseReset;
        mPlayer->sendMsg(resp);
        return;
    }
    
    switch (tollgateDifficulty)
    {
    case TOLLGATE_EASY:
        {
            tollgateId = mPlayerTowerData->mCurrentTowerId;
        }
        break;
    case TOLLGATE_NORMAL:
        {
            tollgateId = tollgateTemplate->mNormalId;
            tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
            if (tollgateTemplate == NULL)
            {
                LOG_WARN("Get tollgate template failed tollgate id:%u", tollgateId);
                resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
                mPlayer->sendMsg(resp);
                return;
            }
        }
        break;
    case TOLLGATE_HARD:
        {
            tollgateId = tollgateTemplate->mHardId;
            tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
            if (tollgateTemplate == NULL)
            {
                LOG_WARN("Get tollgate template failed tollgate id:%u", tollgateId);
                resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
                mPlayer->sendMsg(resp);
                return;
            }
        }
        break;
    default:
        resp.mErrorId = LynxErrno::InvalidTollgateDifficulty;
        mPlayer->sendMsg(resp);
        return;
    }

    resp.mErrorId = checkTowerTollgateId(tollgateId);
    if (resp.mErrorId != LynxErrno::None)
    {
        mPlayer->sendMsg(resp);
        return;
    }

    resp.mTollgateId = tollgateId;
    resp.mBattleSceneName = tollgateTemplate->mBattleMapAsset;
    resp.mTollgateName = tollgateTemplate->mName;

    resp.mErrorId = genTowerBattleData(tollgateId);
    if (resp.mErrorId == LynxErrno::None)
    {
        mTowerIdInBattle = tollgateId;
        resp.mMonsterList = mTowerMonsterList;
        resp.mMonsterPet = mTowerPet;
    }

    mPlayer->sendMsg(resp);*/
}

ErrorId 
TowerManager::checkTowerTollgateId(TemplateId tollgateId)
{
    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    if (tollgateTemplate == NULL)
    {
        return LynxErrno::NoTollgateIdTemplate;
    }

    if (tollgateTemplate->mBattleMapAsset.empty())
    {
        return LynxErrno::TollgateSceneNotExist;
    }

    return LynxErrno::None;
}

ErrorId 
TowerManager::genTowerBattleData(TemplateId tollgateId)
{
   /* TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    if (tollgateTemplate == NULL)
    {
        LOG_WARN("Get tollgate template failed tollgate id:%u", tollgateId);
        return LynxErrno::NoTollgateIdTemplate;
    }

    UInt32 lineUpId = tollgateTemplate->mTeamId;
    if (lineUpId == 0)
    {
        return LynxErrno::TollgateNoLineupId;
    }

    LineupTemplate* lineupTemplate = LINEUP_TABLE().get(lineUpId);
    if (lineupTemplate == NULL)
    {
        LOG_WARN("Get lineupTemplate failed templateId[%u]", lineUpId);
        return LynxErrno::TollgateLineupIdNotExist;
    }

    UInt32 monsterId1 = lineupTemplate->mMonsterId1;
    if (monsterId1 != 0)
    {
        MonsterData monsterLData;
        if (!mPlayer->getTollgateManager().genTollgateMonsterData(monsterLData, monsterId1, 1))
        {
            LOG_WARN("Init monster data failed when add monster, monster templateId[%u]", monsterId1);
            return LynxErrno::TollgateGenDataFailed;
        }
        mTowerMonsterList.insertTail(monsterLData);
    }

    UInt32 monsterId2 = lineupTemplate->mMonsterId2;
    if (monsterId2 != 0)
    {
        MonsterData monsterMData;
        if (!mPlayer->getTollgateManager().genTollgateMonsterData(monsterMData, monsterId2, 2))
        {
            LOG_WARN("Init monster data failed when add monster, monster templateId[%u]", monsterId2);
            return LynxErrno::TollgateGenDataFailed;
        }
        mTowerMonsterList.insertTail(monsterMData);
    }

    UInt32 monsterId3 = lineupTemplate->mMonsterId3;
    if (monsterId3 != 0)
    {
        MonsterData monsterRData;
        if (!mPlayer->getTollgateManager().genTollgateMonsterData(monsterRData, monsterId3, 3))
        {
            LOG_WARN("Init monster data failed when add monster, monster templateId[%u]", monsterId3);
            return LynxErrno::TollgateGenDataFailed;
        }
        mTowerMonsterList.insertTail(monsterRData);
    }

    UInt32 posIndex = 4;
    for (List<TemplateId>::Iter* iter = lineupTemplate->mBenchIdList.begin();
        iter != lineupTemplate->mBenchIdList.end();
        iter = lineupTemplate->mBenchIdList.next(iter))
    {
        if (iter->mValue != 0)
        {
            MonsterData monsterData;
            if (!mPlayer->getTollgateManager().genTollgateMonsterData(monsterData, iter->mValue, posIndex))
            {
                LOG_WARN("Init monster data failed when add monster bench, monster templateId[%u]", iter->mValue);
                return LynxErrno::TollgateGenDataFailed;
            }
            mTowerMonsterList.insertTail(monsterData);
            posIndex++;
        }
    }

    UInt32 petId = lineupTemplate->mPetId;
    if (petId != 0)
    {
        PetData petData;
        if (!mPlayer->getTollgateManager().genTollgatePetData(petData, petId))
        {
            LOG_WARN("Init monster data failed when add monster pet, monster templateId[%u]", petId);
            return LynxErrno::TollgateGenDataFailed;
        }
        mTowerPet = petData;
    }*/
    return LynxErrno::None;
}

void 
TowerManager::towerBattleResult(const TowerBattleResultReq& msg)
{
    TowerBattleResultResp resp;
    resp.mTollgateId = mTowerIdInBattle;
    if (mPlayer->getPlayerLeval() < TOWER_OPEN_LEVEL_LIMIT)
    {
        resp.mErrorId = LynxErrno::TowerOpenLevelLimit;
        mPlayer->sendMsg(resp);
        return;
    }

    resp.mErrorId = checkTowerResult(msg.mTollgateId, msg.mBattleVerifyData, msg.mTollgateDifficulty);
    if (resp.mErrorId != LynxErrno::None)
    {
        mPlayer->sendMsg(resp);
        return;
    }

    if (mPlayerTowerData->mIsResetEnable)
    {
        resp.mErrorId = LynxErrno::TowerPleaseReset;
        mPlayer->sendMsg(resp);
        return;
    }

    if (!msg.mBattleVerifyData.mIsVictory
        || !mPlayer->getBattleManager().getTowerPass())
    {
        mPlayerTowerData->mIsResetEnable = true;
        syncTowerDataToClient();
        saveTowerData();

        resp.mStarCount = 0;
        mPlayer->sendMsg(resp);
        return;
    }

    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(mTowerIdInBattle);
    if (tollgateTemplate == NULL)
    {
        LOG_WARN("Get tollgate template failed tollgate id:%u", mTowerIdInBattle);
        resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
        mPlayer->sendMsg(resp);
        return;
    }

    resp.mAwardList.clear();
    getAwardChests(msg.mTollgateId, resp.mAwardList);
    if (resp.mAwardList.size() != 0)
    {
        if (LogicSystem::getSingleton().checkBagIsFull(mPlayer, resp.mAwardList))
        {
            resp.mErrorId = LynxErrno::BagIsFull;
            mPlayer->sendMsg(resp);
            return;
        }
    }

    UInt32 starCount = 0;
    UInt32 awardStars = 0;
    switch (msg.mTollgateDifficulty)
    {
    case TOLLGATE_EASY:
        {
            awardStars = 1;
            starCount = 1;
        }
        break;
    case TOLLGATE_NORMAL:
        {
            awardStars = 2;
            starCount = 2;
        }
        break;
    case TOLLGATE_HARD:
        {
            awardStars = 3;
            starCount = 3;
        }
        break;
    }

    resp.mStarCount = starCount;

    for (List<TowerData>::Iter* towerIter = mPlayerTowerData->mTowerDataList.begin();
        towerIter != mPlayerTowerData->mTowerDataList.end();
        towerIter = mPlayerTowerData->mTowerDataList.next(towerIter))
    {
        if (towerIter->mValue.mTowerId == mPlayerTowerData->mCurrentTowerId)
        {
            towerIter->mValue.mStarCount = starCount;
            break;
        }
    }

    if (tollgateTemplate->mIndexInScene >= TOWER_FLOOR_MAX_COUNT)
    {
        mPlayerTowerData->mIsResetEnable = true;
    }
    else
    {
        TemplateId nextTowerId = mPlayerTowerData->mCurrentTowerId + 10;
        TollgateTemplate* nextTowerTemplate = TOLLGATE_TABLE().get(nextTowerId);
        if (nextTowerTemplate == NULL)
        {
            LOG_WARN("Get tollgate template failed tollgate id:%u", nextTowerId);
            resp.mTollgateId = nextTowerId;
            resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
            mPlayer->sendMsg(resp);
            return;
        }
        mPlayerTowerData->mCurrentTowerId = nextTowerId;
        mPlayerTowerData->mCurrentTowerFloor = nextTowerTemplate->mIndexInScene;
    }

    mPlayerTowerData->mCrystalCount += tollgateTemplate->mTollgateAwardInfo.mCrystalCount;
    mPlayerTowerData->mCurrentStarCount += awardStars;
    if (mPlayerTowerData->mCurrentStarCount > mPlayerTowerData->mBestStarCount)
    {
        mPlayerTowerData->mBestStarCount = mPlayerTowerData->mCurrentStarCount;
        LogicSystem::getSingleton().syncPlayerTowerBestStar(mPlayer->getPlayerGuid());
    }
    mPlayerTowerData->mUsableStars += awardStars;

    syncTowerDataToClient();
    saveTowerData();

    if (resp.mAwardList.size() != 0)
    {
        for (List<MsgItemBaseData>::Iter* it = resp.mAwardList.begin();
            it != resp.mAwardList.end(); it = resp.mAwardList.next(it))
        {
            switch (it->mValue.mType)
            {
            case ITEM_TYPE:
                {
                    mPlayer->getItemManager().createItem(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
                }
                break;
            case EQUIP_TYPE:
                {
                    mPlayer->getEquipManager().createEquip(it->mValue.mTemplateId, 0, false);
                }
                break;
            case RUNE_TYPE:
                {
                    mPlayer->getRuneManager().createRune(it->mValue.mTemplateId, 0, false);
                }
                break;
            case HERO_TYPE:
                {
                    mPlayer->getHeroManager().createHero(it->mValue.mTemplateId, false);
                }
                break;
            case HERO_DEBRIS_TYPE:
                {
                    mPlayer->getHeroDebrisManager().createHeroDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
                }
                break;
            case PET_DEBRIS_TYPE:
                {
                    mPlayer->getPetDebrisManager().createPetDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
                }
                break;
            default:
                LOG_WARN("Invalid item type %u", it->mValue.mType);
                break;
            }
        }
    }

    //mPlayer->increasePlayerSoul(tollgateTemplate->mTollgateAwardInfo.mSoulValue);
    mPlayer->increasePlayerGold(tollgateTemplate->mTollgateAwardInfo.mGold);

    //resp.mAwardSoul = tollgateTemplate->mTollgateAwardInfo.mSoulValue;
    resp.mAwardGold = tollgateTemplate->mTollgateAwardInfo.mGold;
    resp.mAwardCrystal = tollgateTemplate->mTollgateAwardInfo.mCrystalCount;

    mPlayer->sendMsg(resp);
    mTowerIdInBattle = 0;
}

ErrorId 
TowerManager::checkTowerResult(TemplateId tollgateId, const BattleVerifyData& battleVerifyData, UInt8 tollgateDifficulty)
{
    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    if (tollgateTemplate == NULL)
    {
        return LynxErrno::NoTollgateIdTemplate;
    }
    if (mTowerIdInBattle != tollgateId)
    {
        return LynxErrno::InvalidTollgateId;
    }

    TollgateTemplate* currentTemplate = TOLLGATE_TABLE().get(mPlayerTowerData->mCurrentTowerId);
    if (currentTemplate == NULL)
    {
        return LynxErrno::NoTollgateIdTemplate;
    }

    UInt32 checkId = 0;
    switch (tollgateDifficulty)
    {
    case TOLLGATE_EASY:
        {
            checkId = mPlayerTowerData->mCurrentTowerId;
        }
        break;
    case TOLLGATE_NORMAL:
        {
            checkId = currentTemplate->mNormalId;
        }
        break;
    case TOLLGATE_HARD:
        {
            checkId = currentTemplate->mHardId;
        }
        break;
    default:
        return LynxErrno::InvalidTollgateDifficulty;
    }

    if (checkId != tollgateId)
    {
        return LynxErrno::InvalidTollgateId;
    }

    // 服务器根据客户端传递的操作流程和数值进行模拟，对比客户端数据是否一致，不一致则无效
    PlayerData oppBattleData;
    genTowerOppBattleData(oppBattleData);
    if (!mPlayer->getBattleManager().verifyBattleResult(mPlayer->mPlayerData, oppBattleData, true, battleVerifyData, true))
    {
        return LynxErrno::TollgateVerifyResultFailed;
    }
    return LynxErrno::None;
}

void 
TowerManager::genTowerOppBattleData(PlayerData& oppBattleData)
{
   //mpetdata注释，sec
	/* oppBattleData.mBaseData.mPlayerId = 20150402;

    for (List<MonsterData>::Iter* iter = mTowerMonsterList.begin();
        iter != mTowerMonsterList.end(); iter = mTowerMonsterList.next(iter))
    {
        if (iter->mValue.mBaseData.mMonsterId == 0 ||
            iter->mValue.mBaseData.mPresentPos == 0)
        {
            continue;
        }
        HeroData heroData;
        mPlayer->getTollgateManager().changeMonsterDataToHeroData(heroData, iter->mValue);
        oppBattleData.mHeroData.mHeroDataList.insertTail(heroData);
    }

    oppBattleData.mPetData.mPetDataList.insertTail(mTowerPet);*/
}

void 
TowerManager::refreshTowerInfo()
{
    mPlayerTowerData->mCurrentStarCount = 0;
    mPlayerTowerData->mCurrentTowerFloor = 1;
    mPlayerTowerData->mCurrentTowerId = TOWER_FIRST_FLOOR;
    mPlayerTowerData->mIsResetEnable = false;
    mPlayerTowerData->mResetCount = 0;
    mPlayerTowerData->mTowerBufferList.clear();
    mPlayerTowerData->mUsableStars = 0;

    syncTowerDataToClient();
    saveTowerData();
}

void 
TowerManager::resetTower()
{
    ResetTowerResp resp;
    resp.mErrorId = LynxErrno::None;
    if (mPlayer->getPlayerLeval() < TOWER_OPEN_LEVEL_LIMIT)
    {
        resp.mErrorId = LynxErrno::TowerOpenLevelLimit;
        mPlayer->sendMsg(resp);
        return;
    }

    if (!mPlayerTowerData->mIsResetEnable)
    {
        resp.mErrorId = LynxErrno::TowerCanNotReset;
        mPlayer->sendMsg(resp);
        return;
    }

    if (mPlayerTowerData->mResetCount >= TOWER_RESET_MAX_COUNT)
    {
        resp.mErrorId = LynxErrno::TowerResetUpMaxCount;
        mPlayer->sendMsg(resp);
        return;
    }

    UInt32 costDiamond = getResetCostDiamond(mPlayerTowerData->mResetCount + 1);

	//RMB注释 sec
    /*if (costDiamond > mPlayer->getPlayerData().mAccountData.mRmb)
    {
        resp.mErrorId = LynxErrno::TowerResetDiamondNotEnough;
        mPlayer->sendMsg(resp);
        return;
    }

    mPlayer->mPlayerData.mAccountData.mRmb -= costDiamond;
	*/

    mPlayerTowerData->mCurrentStarCount = 0;
    mPlayerTowerData->mCurrentTowerFloor = 1;
    mPlayerTowerData->mCurrentTowerId = TOWER_FIRST_FLOOR;
    mPlayerTowerData->mIsResetEnable = false;
    mPlayerTowerData->mResetCount++;
    mPlayerTowerData->mTowerBufferList.clear();
    mPlayerTowerData->mUsableStars = 0;

    syncTowerDataToClient();
    saveTowerData();

    mPlayer->sendMsg(resp);
}

UInt32 
TowerManager::getResetCostDiamond(UInt32 resetIndex)
{
    UInt32 diamond = 0;

    switch (resetIndex)
    {
    case 1:
        diamond = 0;
    	break;
    case 2:
        diamond = 50;
        break;
    case 3:
        diamond = 100;
        break;
    case 4:
        diamond = 150;
        break;
    case 5:
        diamond = 200;
        break;
    case 6:
        diamond = 400;
        break;
    case 7:
        diamond = 600;
        break;
    case 8:
        diamond = 800;
        break;
    case 9:
        diamond = 1000;
        break;
    case 10:
        diamond = 1200;
        break;
    default:
        diamond = 0;
        break;
    }

    return diamond;
}

void 
TowerManager::sweepTower()
{
    SweepTowerResp resp;
    resp.mErrorId = LynxErrno::None;

    if (mPlayer->getPlayerLeval() < TOWER_OPEN_LEVEL_LIMIT)
    {
        resp.mErrorId = LynxErrno::TowerOpenLevelLimit;
        mPlayer->sendMsg(resp);
        return;
    }

    if (mPlayerTowerData->mIsResetEnable)
    {
        resp.mErrorId = LynxErrno::TowerPleaseReset;
        mPlayer->sendMsg(resp);
        return;
    }

    List<TowerData> sweepList;
    bool isStart = false;
    for (List<TowerData>::Iter* towerIter = mPlayerTowerData->mTowerDataList.begin();
        towerIter != mPlayerTowerData->mTowerDataList.end();
        towerIter = mPlayerTowerData->mTowerDataList.next(towerIter))
    {
        if (towerIter->mValue.mStarCount == 3)
        {
            if (towerIter->mValue.mTowerId == mPlayerTowerData->mCurrentTowerId)
            {
                isStart = true;
                sweepList.insertTail(towerIter->mValue);
                continue;
            }
            if (isStart)
            {
                sweepList.insertTail(towerIter->mValue);
                continue;
            }
        }
        break;
    }

    if (sweepList.size() == 0)
    {
        resp.mErrorId = LynxErrno::TowerSweepNoFloor;
        mPlayer->sendMsg(resp);
        return;
    }

    UInt32 crystalCount = 0;
    //UInt32 soulCount = 0;
    UInt32 goldCount = 0;

    resp.mFloorCount = sweepList.size();
    resp.mAwardList.clear();

    for (List<TowerData>::Iter* iter = sweepList.begin();
        iter != sweepList.end(); iter = sweepList.next(iter))
    {
        TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(iter->mValue.mTowerId);
        if (tollgateTemplate == NULL)
        {
            LOG_WARN("Get tollgate template failed, templateId: %u", iter->mValue.mTowerId);
            resp.mErrorId = LynxErrno::TowerSweepFailed;
            mPlayer->sendMsg(resp);
            return;
        }

        TollgateTemplate* hardTemplate = TOLLGATE_TABLE().get(tollgateTemplate->mHardId);
        if (hardTemplate == NULL)
        {
            LOG_WARN("Get tollgate template failed tollgate id:%u", tollgateTemplate->mHardId);
            resp.mErrorId = LynxErrno::TowerSweepFailed;
            mPlayer->sendMsg(resp);
            return;
        }

        crystalCount += hardTemplate->mTollgateAwardInfo.mCrystalCount;
        //soulCount += hardTemplate->mTollgateAwardInfo.mSoulValue;
        goldCount += hardTemplate->mTollgateAwardInfo.mGold;

        getAwardChests(tollgateTemplate->mHardId, resp.mAwardList);
    }

    List<TowerData>::Iter* lastIter = sweepList.getn(sweepList.size() - 1);
    if (lastIter == NULL)
    {
        resp.mErrorId = LynxErrno::TowerSweepFailed;
        mPlayer->sendMsg(resp);
        return;
    }

    TemplateId nextTowerId = lastIter->mValue.mTowerId + 10;
    TollgateTemplate* nextTowerTemplate = TOLLGATE_TABLE().get(nextTowerId);
    if (nextTowerTemplate == NULL)
    {
        LOG_WARN("Get tollgate template failed tollgate id:%u", nextTowerId);
        resp.mErrorId = LynxErrno::TowerSweepFailed;
        mPlayer->sendMsg(resp);
        return;
    }

    if (resp.mAwardList.size() != 0)
    {
        if (LogicSystem::getSingleton().checkBagIsFull(mPlayer, resp.mAwardList))
        {
            resp.mErrorId = LynxErrno::BagIsFull;
            mPlayer->sendMsg(resp);
            return;
        }

        for (List<MsgItemBaseData>::Iter* it = resp.mAwardList.begin();
            it != resp.mAwardList.end(); it = resp.mAwardList.next(it))
        {
            switch (it->mValue.mType)
            {
            case ITEM_TYPE:
                {
                    mPlayer->getItemManager().createItem(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
                }
                break;
            case EQUIP_TYPE:
                {
                    mPlayer->getEquipManager().createEquip(it->mValue.mTemplateId, 0, false);
                }
                break;
            case RUNE_TYPE:
                {
                    mPlayer->getRuneManager().createRune(it->mValue.mTemplateId, 0, false);
                }
                break;
            case HERO_TYPE:
                {
                    mPlayer->getHeroManager().createHero(it->mValue.mTemplateId, false);
                }
                break;
            case HERO_DEBRIS_TYPE:
                {
                    mPlayer->getHeroDebrisManager().createHeroDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
                }
                break;
            case PET_DEBRIS_TYPE:
                {
                    mPlayer->getPetDebrisManager().createPetDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
                }
                break;
            default:
                LOG_WARN("Invalid item type %u", it->mValue.mType);
                break;
            }
        }
    }

    mPlayerTowerData->mCurrentTowerId = nextTowerId;
    mPlayerTowerData->mCurrentTowerFloor = nextTowerTemplate->mIndexInScene;
    mPlayerTowerData->mCrystalCount += crystalCount;
    mPlayerTowerData->mCurrentStarCount += sweepList.size() * 3;
    if (mPlayerTowerData->mCurrentStarCount > mPlayerTowerData->mBestStarCount)
    {
        mPlayerTowerData->mBestStarCount = mPlayerTowerData->mCurrentStarCount;
        LogicSystem::getSingleton().syncPlayerTowerBestStar(mPlayer->getPlayerGuid());
    }
    mPlayerTowerData->mUsableStars += sweepList.size() * 3;
    //mPlayer->increasePlayerSoul(soulCount);
    mPlayer->increasePlayerGold(goldCount);

    syncTowerDataToClient();
    saveTowerData();

    resp.mAwardCrystal = crystalCount;
    resp.mAwardGold = goldCount;
    mPlayer->sendMsg(resp);
}

bool 
TowerManager::getAwardChests(TemplateId tollgateId, List<MsgItemBaseData>& itemList)
{
    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    if (tollgateTemplate == NULL)
    {
        LOG_WARN("Get tollgate template failed, templateId: %u", tollgateId);
        return false;
    }

    if (tollgateTemplate->mIndexInScene != 3 && tollgateTemplate->mIndexInScene != 6
        && tollgateTemplate->mIndexInScene != 9 && tollgateTemplate->mIndexInScene != 10)
    {
        return false;
    }

    if (tollgateTemplate->mTollgateAwardInfo.mAwardId != 0)
    {
        AwardTemplate* awardTemplate = AWARD_TABLE().get(tollgateTemplate->mTollgateAwardInfo.mAwardId);
        if (awardTemplate == NULL)
        {
            LOG_WARN("Get award template failed, awardId: %u", tollgateTemplate->mTollgateAwardInfo.mAwardId);
            return false;
        }

        if (awardTemplate->mAwardItemList.size() != 0)
        {
            for (List<AwardLine>::Iter* iter = awardTemplate->mAwardItemList.begin();
                iter != awardTemplate->mAwardItemList.end();
                iter = awardTemplate->mAwardItemList.next(iter))
            {
                if (iter->mValue.mItemId == 0 || iter->mValue.mItemType == 0
                    || iter->mValue.mCount == 0)
                {
                    continue;
                }

                if (!mPlayer->getTollgateManager().checkTemplateInvalid(iter->mValue.mItemId, iter->mValue.mItemType))
                {
                    continue;
                }

                MsgItemBaseData msgItemBaseData;
                msgItemBaseData.mTemplateId = iter->mValue.mItemId;
                msgItemBaseData.mType = iter->mValue.mItemType;
                msgItemBaseData.mCount = iter->mValue.mCount;

                bool findExist = false;
                for (List<MsgItemBaseData>::Iter* it = itemList.begin();
                    it != itemList.end(); it = itemList.next(it))
                {
                    if (it->mValue.mTemplateId == msgItemBaseData.mTemplateId)
                    {
                        it->mValue.mCount += iter->mValue.mCount;
                        findExist = true;
                        break;
                    }
                }

                if (!findExist)
                {
                    itemList.insertTail(msgItemBaseData);
                }
            }

        }
    }

    return true;
}

