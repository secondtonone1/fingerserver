#include "ActivityManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

#define ONE_DAY_SEC 24 * 3600
ActivityManager::ActivityManager() : mPlayer(NULL), mPreUpdateTime(0), mAlarm(NULL), 
mTollgateIdInBattle(0), mGoldActivityMyDps(0), mRuneTimer(false), mGoldTimer(false),
mEnchantTimer(false), mPetTimer(false), mStoreHouseTimer(false), mDirtyFlag(false)
{

}


ActivityManager::~ActivityManager()
{

}

bool 
ActivityManager::initial(Player* player)
{
    //目前注释活动系统相关的初始化 sec
	//mPlayer = player;

    //// rune
    //UInt32 resetTime = TimeUtil::getTodayBaseSec() 
    //    + getActivitySettingResetTime(Activity_Setting_Rune_TemplateID);

    //if (mPlayer->getPlayerData().mAccountData.mLogoutTime < resetTime)
    //{
    //    mPlayer->mPlayerData.mActivityData.mRuneCount = 0;
    //}

    //// gold
    //resetTime = TimeUtil::getTodayBaseSec() 
    //    + getActivitySettingResetTime(Activity_Setting_Gold_TemplateID);

    //if (mPlayer->getPlayerData().mAccountData.mLogoutTime < resetTime)
    //{
    //    mPlayer->mPlayerData.mActivityData.mRuneCount = 0;
    //}

    //// enchant
    //resetTime = TimeUtil::getTodayBaseSec() 
    //    + getActivitySettingResetTime(Activity_Setting_Enchant_TemplateID);

    //if (mPlayer->getPlayerData().mAccountData.mLogoutTime < resetTime)
    //{
    //    mPlayer->mPlayerData.mActivityData.mRuneCount = 0;
    //}

    //// pet
    //resetTime = TimeUtil::getTodayBaseSec() 
    //    + getActivitySettingResetTime(Activity_Setting_Pet_TemplateID);

    //if (mPlayer->getPlayerData().mAccountData.mLogoutTime < resetTime)
    //{
    //    mPlayer->mPlayerData.mActivityData.mRuneCount = 0;
    //}


    //mPreUpdateTime = TimeUtil::getLocalTimeSec();

    //mAlarm = XNEW(Alarm)();
    //mAlarm->initial();

    return true;
}
ActivitySettingTemplate*
ActivityManager::getActivitySettingTemplate(UInt32 id)
{
    // 初始化活动数据
    ActivitySettingTemplate* at = ACTIVITY_SETTING_TABLE().get(id);
    if (!at)
    {
        LOG_ERROR("Activity  template error.");
        return false;
    }
    return at;
}
UInt32
ActivityManager::getActivitySettingResetTime(UInt32 id)
{
    ActivitySettingTemplate* at = getActivitySettingTemplate(id);
    if (!at)
    {

        return 0;
    }
    return at->mResetTime;
}

void 
ActivityManager::release()
{
    mAlarm->release();
    XDELETE(mAlarm);
}
void
ActivityManager::update(UInt32 accTime)
{
    // 每60秒更新一次
    if (mPreUpdateTime + 60 < TimeUtil::getLocalTimeSec())
    {
        mAlarm->update();
        mPreUpdateTime = TimeUtil::getLocalTimeSec();
    }
    if (!mRuneTimer)
    {
        startRuneTimer();
    }
    if (!mGoldTimer)
    {
        startGoldTimer();
    }
    if (!mEnchantTimer)
    {
        startEnchantTimer();
    }
    if (!mPetTimer)
    {
        startPetTimer();
    }
    if (!mStoreHouseTimer)
    {
        startStoreHouseTimer();
    }
    if (mDirtyFlag)
    {
        saveActivityData();
        syncActivityToClient();
        mDirtyFlag = false;
    }
}

void
ActivityManager::syncActivityToClient()
{
    ActivityChangeNotify ac;
	//活动数据段已经注释 sec
   /* ac.mRuneCount = mPlayer->mPlayerData.mActivityData.mRuneCount;
    ac.mGoldCount = mPlayer->mPlayerData.mActivityData.mGoldCount;
    ac.mEnchantCount = mPlayer->mPlayerData.mActivityData.mEnchantCount;
    ac.mPetCount = mPlayer->mPlayerData.mActivityData.mPetCount;
    ac.mStoreHouseCount = mPlayer->mPlayerData.mActivityData.mStoreHouseCount;*/
    mPlayer->sendMsgWithAck(ac);
}

void
ActivityManager::addStorehouseCount()
{
    //playerdata 的activedata注释了 sec
	/* mPlayer->mPlayerData.mActivityData.mStoreHouseCount++;*/
    setDirty();
}

void
ActivityManager::refreshSetting()
{
    ActivityRefreshSettingResp ars;
    ars.mTodayState = getTodayActivityStatus();
    ars.mRuneCountUpper = getActivityCount(Activity_Setting_Rune_TemplateID);
    ars.mGoldCountUpper = getActivityCount(Activity_Setting_Gold_TemplateID);
    ars.mEnchantCountUpper = getActivityCount(Activity_Setting_Enchant_TemplateID);
    ars.mPetCountUpper = getActivityCount(Activity_Setting_Pet_TemplateID);
    ars.mStoreHouseUpper = getActivityCount(Activity_Setting_StoreHouse_TemplateID);

    mPlayer->sendMsgWithAck(ars);
}
void
ActivityManager::startRuneTimer()
{
    UInt32 resetTime = getActivitySettingResetTime(Activity_Setting_Rune_TemplateID);
    UInt32 alarmTime = TimeUtil::getTodayBaseSec() + resetTime;
    if (TimeUtil::getLocalTimeSec() > alarmTime)
    {
        alarmTime = TimeUtil::getTodayBaseSec() + ONE_DAY_SEC + resetTime;
    }
    mAlarm->start(alarmTime - TimeUtil::getLocalTimeSec(), Alarm::AlarmCallback(this, &ActivityManager::onRuneAlarm));
    mRuneTimer = true;
}
void
ActivityManager::startGoldTimer()
{
    UInt32 resetTime = getActivitySettingResetTime(Activity_Setting_Gold_TemplateID);
    UInt32 alarmTime = TimeUtil::getTodayBaseSec() + resetTime;
    if (TimeUtil::getLocalTimeSec() > alarmTime)
    {
        alarmTime = TimeUtil::getTodayBaseSec() + ONE_DAY_SEC + resetTime;
    }
    mAlarm->start(alarmTime - TimeUtil::getLocalTimeSec(), Alarm::AlarmCallback(this, &ActivityManager::onGoldAlarm));
    mGoldTimer = true;
}
void
ActivityManager::startEnchantTimer()
{
    UInt32 resetTime = getActivitySettingResetTime(Activity_Setting_Enchant_TemplateID);
    UInt32 alarmTime = TimeUtil::getTodayBaseSec() + resetTime;
    if (TimeUtil::getLocalTimeSec() > alarmTime)
    {
        alarmTime = TimeUtil::getTodayBaseSec() + ONE_DAY_SEC + resetTime;
    }
    mAlarm->start(alarmTime - TimeUtil::getLocalTimeSec(), Alarm::AlarmCallback(this, &ActivityManager::onEnchantAlarm));
    mEnchantTimer = true;
}
void
ActivityManager::startPetTimer()
{
    UInt32 resetTime = getActivitySettingResetTime(Activity_Setting_Pet_TemplateID);
    UInt32 alarmTime = TimeUtil::getTodayBaseSec() + resetTime;
    if (TimeUtil::getLocalTimeSec() > alarmTime)
    {
        alarmTime = TimeUtil::getTodayBaseSec() + ONE_DAY_SEC + resetTime;
    }
    mAlarm->start(alarmTime - TimeUtil::getLocalTimeSec(), Alarm::AlarmCallback(this, &ActivityManager::onPetAlarm));
    mPetTimer = true;
}
void
ActivityManager::startStoreHouseTimer()
{
    UInt32 resetTime = getActivitySettingResetTime(Activity_Setting_StoreHouse_TemplateID);
    UInt32 alarmTime = TimeUtil::getTodayBaseSec() + resetTime;
    if (TimeUtil::getLocalTimeSec() > alarmTime)
    {
        alarmTime = TimeUtil::getTodayBaseSec() + ONE_DAY_SEC + resetTime;
    }
    mAlarm->start(alarmTime - TimeUtil::getLocalTimeSec(), Alarm::AlarmCallback(this, &ActivityManager::onStoreHouseAlarm));
    mStoreHouseTimer = true;
}
void
ActivityManager::onRuneAlarm()
{
	//playerdata 的activedata注释了 sec   
	/*mPlayer->mPlayerData.mActivityData.mRuneCount = 0;*/
    mRuneTimer = false;
    setDirty();
}
void
ActivityManager::onGoldAlarm()
{
    //playerdata 的activedata注释了 sec
	/*mPlayer->mPlayerData.mActivityData.mRuneCount = 0;*/
    mGoldTimer = false;
    setDirty();
}
void
ActivityManager::onEnchantAlarm()
{
    //playerdata 的activedata注释了 sec
	/*mPlayer->mPlayerData.mActivityData.mRuneCount = 0;*/
    mEnchantTimer = false;
    setDirty();
}
void
ActivityManager::onPetAlarm()
{
	//playerdata 的activedata注释了 sec
	/* mPlayer->mPlayerData.mActivityData.mRuneCount = 0;*/
    mPetTimer = false;
    setDirty();
}
void
ActivityManager::onStoreHouseAlarm()
{
    //playerdata 的activedata注释了 sec
	/*mPlayer->mPlayerData.mActivityData.mStoreHouseCount = 0;*/
    mStoreHouseTimer = false;
    setDirty();
}
UInt8
ActivityManager::getTodayActivityStatus()
{
    UInt8 state = 0;

    state = (getActivityCount(Activity_Setting_Rune_TemplateID) > 0 ? 1 : 0) |
        (getActivityCount(Activity_Setting_Gold_TemplateID) > 0 ? 1 : 0) << 1 |
        (getActivityCount(Activity_Setting_Enchant_TemplateID) > 0 ? 1 : 0) << 2 |
        (getActivityCount(Activity_Setting_Pet_TemplateID) > 0 ? 1 : 0) << 3 |
    (getActivityCount(Activity_Setting_StoreHouse_TemplateID) > 0 ? 1 : 0) << 4;

    return state;
}
UInt8 
ActivityManager::getActivityCount(UInt32 activityType)
{
    Int32 t = TimeUtil::getTimeSec() - getActivitySettingResetTime(activityType);
    Int32 week = TimeUtil::getDayNum(t);
    UInt8 state = 0;
    ActivitySettingTemplate* ast = getActivitySettingTemplate(activityType);
    // 如果找不到，那么返回0
    if (!ast) return 0;

    switch (week)
    {
    case 0:
        state = ast->mSunday;
        break;
    case 1:
        state = ast->mMonday;
        break;
    case 2:
        state = ast->mTuesday;
        break;
    case 3:
        state = ast->mWednesday;
        break;
    case 4:
        state = ast->mThursday;
        break;
    case 5:
        state = ast->mFriday;
        break;
    case 6:
        state = ast->mSaturday;
        break;
    }
    return state;
}

void 
ActivityManager::saveActivityData()
{
    //playerdata 的activedata注释了 sec
	PersistUpdatePlayerActivityDataNotify updateActivityNotify;
    updateActivityNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    /*updateActivityNotify.mActivityData = mPlayer->mPlayerData.mActivityData;*/
    PersistSystem::getSingleton().postThreadMsg(updateActivityNotify, mPlayer->getPlayerGuid());
}

void 
ActivityManager::startRuneBattle(TemplateId tollgateId)
{
    /*mActivityMonsterList.clear();
    ActivityRuneBattleResp resp;
    resp.mErrorId= checkActivityTollgateId(tollgateId, Activity_Setting_Rune_TemplateID);
    resp.mTollgateId = tollgateId;
    if (resp.mErrorId != LynxErrno::None)
    {
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
    resp.mBattleSceneName = tollgateTemplate->mBattleMapAsset;
    resp.mTollgateName = tollgateTemplate->mName;

    resp.mErrorId = genActivityBattleData(tollgateId);
    if (resp.mErrorId == LynxErrno::None)
    {
        mTollgateIdInBattle = tollgateId;
        resp.mMonsterList = mActivityMonsterList;
        resp.mMonsterPet = mActivityPet;
    }

    mPlayer->sendMsg(resp);*/
}

ErrorId 
ActivityManager::genActivityBattleData(TemplateId tollgateId)
{
    /*TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
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
        mActivityMonsterList.insertTail(monsterLData);
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
        mActivityMonsterList.insertTail(monsterMData);
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
        mActivityMonsterList.insertTail(monsterRData);
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
            mActivityMonsterList.insertTail(monsterData);
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
        mActivityPet = petData;
    }*/
    return LynxErrno::None;
}

ErrorId 
ActivityManager::checkActivityTollgateId(TemplateId tollgateId, UInt32 activityType)
{
    //return LynxErrno::None;
    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    if (tollgateTemplate == NULL)
    {
        return LynxErrno::NoTollgateIdTemplate;
    }

    if (mPlayer->getPlayerLeval() < tollgateTemplate->mNeedLevel)
    {
        return LynxErrno::TollgateLevelLimit;
    }

    if (getActivityCount(activityType) == 0)
    {
        return LynxErrno::ActivityNotOpen;
    }
	//playerdata 的activedata注释了 sec
   /* switch (activityType)
    {
    case Activity_Setting_Rune_TemplateID:
        {
            if (mPlayer->getPlayerData().mActivityData.mRuneCount == getActivityCount(activityType))
            {
                return LynxErrno::ActivityCountNotEnouth;
            }
        }
        break;
    case Activity_Setting_Gold_TemplateID:
        {
            if (mPlayer->getPlayerData().mActivityData.mGoldCount == getActivityCount(activityType))
            {
                return LynxErrno::ActivityCountNotEnouth;
            }
        }
        break;
    case Activity_Setting_Pet_TemplateID:
        {
            if (mPlayer->getPlayerData().mActivityData.mPetCount == getActivityCount(activityType))
            {
                return LynxErrno::ActivityCountNotEnouth;
            }
        }
        break;
    case Activity_Setting_Enchant_TemplateID:
        {
            if (mPlayer->getPlayerData().mActivityData.mEnchantCount == getActivityCount(activityType))
            {
                return LynxErrno::ActivityCountNotEnouth;
            }
        }
        break;
    default:
        return LynxErrno::ActivityTypeNotExist;
    }*/

    if (tollgateTemplate->mBattleMapAsset.empty())
    {
        return LynxErrno::TollgateSceneNotExist;
    }

    return LynxErrno::None;
}

void 
ActivityManager::runeBattleResult(const ActivityRuneResultReq& msg)
{
    //符文战斗结果 sec
	//ActivityRuneResultResp resp;
    //resp.mTollgateId = mTollgateIdInBattle;
    //resp.mErrorId = checkActivityResult(msg.mTollgateId, msg.mBattleVerifyData, Activity_Setting_Rune_TemplateID);
    //if (resp.mErrorId != LynxErrno::None)
    //{
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //if (!msg.mBattleVerifyData.mIsVictory)
    //{
    //    resp.mStarCount = 0;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(mTollgateIdInBattle);
    //if (tollgateTemplate == NULL)
    //{
    //    LOG_WARN("Get tollgate template failed tollgate id:%u", mTollgateIdInBattle);
    //    resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //resp.mAwardList.clear();
    //if (mPlayer->getTollgateManager().getDropItemList(msg.mTollgateId, resp.mAwardList)
    //    && resp.mAwardList.size() != 0)
    //{
    //    if (LogicSystem::getSingleton().checkBagIsFull(mPlayer, resp.mAwardList))
    //    {
    //        resp.mErrorId = LynxErrno::BagIsFull;
    //        mPlayer->sendMsg(resp);
    //        return;
    //    }

    //    for (List<MsgItemBaseData>::Iter* it = resp.mAwardList.begin();
    //        it != resp.mAwardList.end(); it = resp.mAwardList.next(it))
    //    {
    //        switch (it->mValue.mType)
    //        {
    //        case ITEM_TYPE:
    //            {
    //                mPlayer->getItemManager().createItem(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        case EQUIP_TYPE:
    //            {
    //                mPlayer->getEquipManager().createEquip(it->mValue.mTemplateId, 0, false);
    //            }
    //            break;
    //        case RUNE_TYPE:
    //            {
    //                mPlayer->getRuneManager().createRune(it->mValue.mTemplateId, 0, false);
    //            }
    //            break;
    //        case HERO_TYPE:
    //            {
    //                mPlayer->getHeroManager().createHero(it->mValue.mTemplateId, false);
    //            }
    //            break;
    //        case HERO_DEBRIS_TYPE:
    //            {
    //                mPlayer->getHeroDebrisManager().createHeroDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        case PET_DEBRIS_TYPE:
    //            {
    //                mPlayer->getPetDebrisManager().createPetDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        default:
    //            LOG_WARN("Invalid item type %u", it->mValue.mType);
    //            break;
    //        }
    //    }
    //}

    //mPlayer->mPlayerData.mActivityData.mRuneCount++;
    //setDirty();

    //// 增加经验看是否是升级，player和hero
    //UInt32 playerLevelUp = 0;
    //mPlayer->addExpForPlayer(playerLevelUp, tollgateTemplate->mTollgateAwardInfo.mTeamExp);
    //mPlayer->getHeroManager().refinePresentHeros(resp.mHeroLevelUp, tollgateTemplate->mTollgateAwardInfo.mHeroExp);
    //mPlayer->increasePlayerGold(tollgateTemplate->mTollgateAwardInfo.mGold);
    //// 增加符文积分 todo

    //resp.mStarCount = (msg.mBattleVerifyData.mIsVictory == true) ? 1 : 0;
    //resp.mAwardGold = tollgateTemplate->mTollgateAwardInfo.mGold;
    //resp.mAwardTeamExp = tollgateTemplate->mTollgateAwardInfo.mTeamExp;
    //resp.mPlayerLevelUp = playerLevelUp == 0 ? false : true;
    //resp.mAwardHeroExp = tollgateTemplate->mTollgateAwardInfo.mHeroExp;
    //resp.mAwardRunePoint = 0;

    //mPlayer->sendMsg(resp);
}

ErrorId 
ActivityManager::checkActivityResult(TemplateId tollgateId, const BattleVerifyData& battleVerifyData, UInt32 activityType)
{
    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    if (tollgateTemplate == NULL)
    {
        return LynxErrno::NoTollgateIdTemplate;
    }
    if (mTollgateIdInBattle != tollgateId)
    {
        return LynxErrno::InvalidTollgateId;
    }
    if (getActivityCount(activityType) == 0)
    {
        return LynxErrno::ActivityNotOpen;
    }
    // 服务器根据客户端传递的操作流程和数值进行模拟，对比客户端数据是否一致，不一致则无效
    PlayerData oppBattleData;
    genActivityOppBattleData(oppBattleData);
    if (!mPlayer->getBattleManager().verifyBattleResult(mPlayer->mPlayerData, oppBattleData, true, battleVerifyData))
    {
        return LynxErrno::TollgateVerifyResultFailed;
    }
    return LynxErrno::None;
}

void 
ActivityManager::genActivityOppBattleData(PlayerData& oppBattleData)
{
   //关闭活动战斗内容
	/* oppBattleData.mBaseData.mPlayerId = 20150313;

    for (List<MonsterData>::Iter* iter = mActivityMonsterList.begin();
        iter != mActivityMonsterList.end(); iter = mActivityMonsterList.next(iter))
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

    oppBattleData.mPetData.mPetDataList.insertTail(mActivityPet);*/
}

void 
ActivityManager::startGoldBattle(TemplateId tollgateId)
{
   /* mActivityMonsterList.clear();
    ActivityGoldBattleResp resp;
    resp.mErrorId = checkActivityTollgateId(tollgateId, Activity_Setting_Gold_TemplateID);
    resp.mTollgateId = tollgateId;
    if (resp.mErrorId != LynxErrno::None)
    {
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
    resp.mBattleSceneName = tollgateTemplate->mBattleMapAsset;
    resp.mTollgateName = tollgateTemplate->mName;

    resp.mErrorId = genActivityBattleData(tollgateId);
    if (resp.mErrorId == LynxErrno::None)
    {
        mTollgateIdInBattle = tollgateId;
        resp.mMonsterList = mActivityMonsterList;
        resp.mMonsterPet = mActivityPet;
    }

    mPlayer->sendMsg(resp);*/
}

void 
ActivityManager::goldBattleResult(const ActivityGoldResultReq& msg)
{
    //金币战斗结果注释 ，sec PlayerData 活动Data注释
	//ActivityGoldResultResp resp;
    //resp.mErrorId = checkActivityResult(msg.mTollgateId, msg.mBattleVerifyData, Activity_Setting_Gold_TemplateID);
    //if (resp.mErrorId != LynxErrno::None)
    //{
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(mTollgateIdInBattle);
    //if (tollgateTemplate == NULL)
    //{
    //    LOG_WARN("Get tollgate template failed tollgate id:%u", mTollgateIdInBattle);
    //    resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //resp.mAwardList.clear();
    //if (mPlayer->getTollgateManager().getDropItemList(msg.mTollgateId, resp.mAwardList)
    //    && resp.mAwardList.size() != 0)
    //{
    //    if (LogicSystem::getSingleton().checkBagIsFull(mPlayer, resp.mAwardList))
    //    {
    //        resp.mErrorId = LynxErrno::BagIsFull;
    //        mPlayer->sendMsg(resp);
    //        return;
    //    }

    //    for (List<MsgItemBaseData>::Iter* it = resp.mAwardList.begin();
    //        it != resp.mAwardList.end(); it = resp.mAwardList.next(it))
    //    {
    //        switch (it->mValue.mType)
    //        {
    //        case ITEM_TYPE:
    //            {
    //                mPlayer->getItemManager().createItem(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        case EQUIP_TYPE:
    //            {
    //                mPlayer->getEquipManager().createEquip(it->mValue.mTemplateId, 0, false);
    //            }
    //            break;
    //        case RUNE_TYPE:
    //            {
    //                mPlayer->getRuneManager().createRune(it->mValue.mTemplateId, 0, false);
    //            }
    //            break;
    //        case HERO_TYPE:
    //            {
    //                mPlayer->getHeroManager().createHero(it->mValue.mTemplateId, false);
    //            }
    //            break;
    //        case HERO_DEBRIS_TYPE:
    //            {
    //                mPlayer->getHeroDebrisManager().createHeroDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        case PET_DEBRIS_TYPE:
    //            {
    //                mPlayer->getPetDebrisManager().createPetDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        default:
    //            LOG_WARN("Invalid item type %u", it->mValue.mType);
    //            break;
    //        }
    //    }
    //}

    //mPlayer->mPlayerData.mActivityData.mGoldCount++;
    //setDirty();

    //// 通过公式计算奖励金币数目 todo yangyang
    //UInt32 awardGold = 999999;
    //mPlayer->increasePlayerGold(awardGold);

    //resp.mGold = awardGold;
    //// 本次累计输出伤害，通过战斗管理器记录获取
    //resp.mDamage = mPlayer->getBattleManager().getMyDps();
    //mGoldActivityMyDps += resp.mDamage;

    //mPlayer->sendMsg(resp);
}

void 
ActivityManager::startPetBattle(TemplateId tollgateId)
{
   /* mActivityMonsterList.clear();
    ActivityPetBattleResp resp;
    resp.mErrorId = checkActivityTollgateId(tollgateId, Activity_Setting_Pet_TemplateID);
    resp.mTollgateId = tollgateId;
    if (resp.mErrorId != LynxErrno::None)
    {
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
    resp.mBattleSceneName = tollgateTemplate->mBattleMapAsset;
    resp.mTollgateName = tollgateTemplate->mName;

    resp.mErrorId = genActivityBattleData(tollgateId);
    if (resp.mErrorId == LynxErrno::None)
    {
        mTollgateIdInBattle = tollgateId;
        resp.mMonsterList = mActivityMonsterList;
        resp.mMonsterPet = mActivityPet;
    }

    mPlayer->sendMsg(resp);*/
}

void 
ActivityManager::petBattleResult(const ActivityPetResultReq& msg)
{
    //神兽战斗的内容目前注释sec 
	//ActivityPetResultResp resp;
    //resp.mTollgateId = mTollgateIdInBattle;
    //resp.mErrorId = checkActivityResult(msg.mTollgateId, msg.mBattleVerifyData, Activity_Setting_Pet_TemplateID);
    //if (resp.mErrorId != LynxErrno::None)
    //{
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //if (!msg.mBattleVerifyData.mIsVictory)
    //{
    //    resp.mStarCount = 0;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(mTollgateIdInBattle);
    //if (tollgateTemplate == NULL)
    //{
    //    LOG_WARN("Get tollgate template failed tollgate id:%u", mTollgateIdInBattle);
    //    resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //resp.mAwardList.clear();
    //if (mPlayer->getTollgateManager().getDropItemList(msg.mTollgateId, resp.mAwardList)
    //    && resp.mAwardList.size() != 0)
    //{
    //    if (LogicSystem::getSingleton().checkBagIsFull(mPlayer, resp.mAwardList))
    //    {
    //        resp.mErrorId = LynxErrno::BagIsFull;
    //        mPlayer->sendMsg(resp);
    //        return;
    //    }

    //    for (List<MsgItemBaseData>::Iter* it = resp.mAwardList.begin();
    //        it != resp.mAwardList.end(); it = resp.mAwardList.next(it))
    //    {
    //        switch (it->mValue.mType)
    //        {
    //        case ITEM_TYPE:
    //            {
    //                mPlayer->getItemManager().createItem(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        case EQUIP_TYPE:
    //            {
    //                mPlayer->getEquipManager().createEquip(it->mValue.mTemplateId, 0, false);
    //            }
    //            break;
    //        case RUNE_TYPE:
    //            {
    //                mPlayer->getRuneManager().createRune(it->mValue.mTemplateId, 0, false);
    //            }
    //            break;
    //        case HERO_TYPE:
    //            {
    //                mPlayer->getHeroManager().createHero(it->mValue.mTemplateId, false);
    //            }
    //            break;
    //        case HERO_DEBRIS_TYPE:
    //            {
    //                mPlayer->getHeroDebrisManager().createHeroDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        case PET_DEBRIS_TYPE:
    //            {
    //                mPlayer->getPetDebrisManager().createPetDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        default:
    //            LOG_WARN("Invalid item type %u", it->mValue.mType);
    //            break;
    //        }
    //    }
    //}

    //mPlayer->mPlayerData.mActivityData.mPetCount++;
    //setDirty();

    //// 增加经验看是否是升级，player和hero
    //UInt32 playerLevelUp = 0;
    //mPlayer->addExpForPlayer(playerLevelUp, tollgateTemplate->mTollgateAwardInfo.mTeamExp);
    //mPlayer->getHeroManager().refinePresentHeros(resp.mHeroLevelUp, tollgateTemplate->mTollgateAwardInfo.mHeroExp);
    //mPlayer->increasePlayerGold(tollgateTemplate->mTollgateAwardInfo.mGold);

    //resp.mStarCount = (msg.mBattleVerifyData.mIsVictory == true) ? 1 : 0;
    //resp.mAwardGold = tollgateTemplate->mTollgateAwardInfo.mGold;
    //resp.mAwardTeamExp = tollgateTemplate->mTollgateAwardInfo.mTeamExp;
    //resp.mPlayerLevelUp = playerLevelUp == 0 ? false : true;
    //resp.mAwardHeroExp = tollgateTemplate->mTollgateAwardInfo.mHeroExp;

    //mPlayer->sendMsg(resp);
}

void 
ActivityManager::startEnchantBattle(TemplateId tollgateId)
{
   /* mActivityMonsterList.clear();
    ActivityEnchantBattleResp resp;
    resp.mErrorId = checkActivityTollgateId(tollgateId, Activity_Setting_Enchant_TemplateID);
    resp.mTollgateId = tollgateId;
    if (resp.mErrorId != LynxErrno::None)
    {
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
    resp.mBattleSceneName = tollgateTemplate->mBattleMapAsset;
    resp.mTollgateName = tollgateTemplate->mName;

    resp.mErrorId = genActivityBattleData(tollgateId);
    if (resp.mErrorId == LynxErrno::None)
    {
        mTollgateIdInBattle = tollgateId;
        resp.mMonsterList = mActivityMonsterList;
        resp.mMonsterPet = mActivityPet;
    }

    mPlayer->sendMsg(resp);*/
}

void 
ActivityManager::enchantBattleResult(const ActivityEnchantResultReq& msg)
{
    //PlayerData 中的activeData 被注释了，所以sec注释此内容
	//ActivityEnchantResultResp resp;
    //resp.mTollgateId = mTollgateIdInBattle;
    //resp.mErrorId = checkActivityResult(msg.mTollgateId, msg.mBattleVerifyData, Activity_Setting_Enchant_TemplateID);
    //if (resp.mErrorId != LynxErrno::None)
    //{
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //if (!msg.mBattleVerifyData.mIsVictory)
    //{
    //    resp.mStarCount = 0;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(mTollgateIdInBattle);
    //if (tollgateTemplate == NULL)
    //{
    //    LOG_WARN("Get tollgate template failed tollgate id:%u", mTollgateIdInBattle);
    //    resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //resp.mAwardList.clear();
    //if (mPlayer->getTollgateManager().getDropItemList(msg.mTollgateId, resp.mAwardList)
    //    && resp.mAwardList.size() != 0)
    //{
    //    if (LogicSystem::getSingleton().checkBagIsFull(mPlayer, resp.mAwardList))
    //    {
    //        resp.mErrorId = LynxErrno::BagIsFull;
    //        mPlayer->sendMsg(resp);
    //        return;
    //    }

    //    for (List<MsgItemBaseData>::Iter* it = resp.mAwardList.begin();
    //        it != resp.mAwardList.end(); it = resp.mAwardList.next(it))
    //    {
    //        switch (it->mValue.mType)
    //        {
    //        case ITEM_TYPE:
    //            {
    //                mPlayer->getItemManager().createItem(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        case EQUIP_TYPE:
    //            {
    //                mPlayer->getEquipManager().createEquip(it->mValue.mTemplateId, 0, false);
    //            }
    //            break;
    //        case RUNE_TYPE:
    //            {
    //                mPlayer->getRuneManager().createRune(it->mValue.mTemplateId, 0, false);
    //            }
    //            break;
    //        case HERO_TYPE:
    //            {
    //                mPlayer->getHeroManager().createHero(it->mValue.mTemplateId, false);
    //            }
    //            break;
    //        case HERO_DEBRIS_TYPE:
    //            {
    //                mPlayer->getHeroDebrisManager().createHeroDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        case PET_DEBRIS_TYPE:
    //            {
    //                mPlayer->getPetDebrisManager().createPetDebris(it->mValue.mTemplateId, it->mValue.mCount, 0, false);
    //            }
    //            break;
    //        default:
    //            LOG_WARN("Invalid item type %u", it->mValue.mType);
    //            break;
    //        }
    //    }
    //}

    //mPlayer->mPlayerData.mActivityData.mEnchantCount++;
    //setDirty();

    //// 增加经验看是否是升级，player和hero
    //UInt32 playerLevelUp = 0;
    //mPlayer->addExpForPlayer(playerLevelUp, tollgateTemplate->mTollgateAwardInfo.mTeamExp);
    //mPlayer->getHeroManager().refinePresentHeros(resp.mHeroLevelUp, tollgateTemplate->mTollgateAwardInfo.mHeroExp);
    //mPlayer->increasePlayerGold(tollgateTemplate->mTollgateAwardInfo.mGold);

    //resp.mStarCount = (msg.mBattleVerifyData.mIsVictory == true) ? 1 : 0;
    //resp.mAwardGold = tollgateTemplate->mTollgateAwardInfo.mGold;
    //resp.mAwardTeamExp = tollgateTemplate->mTollgateAwardInfo.mTeamExp;
    //resp.mPlayerLevelUp = playerLevelUp == 0 ? false : true;
    //resp.mAwardHeroExp = tollgateTemplate->mTollgateAwardInfo.mHeroExp;

    //mPlayer->sendMsg(resp);
}

void
ActivityManager::runeNumber(ActivityRuneNumberReq& msg)
{
    ActivityRuneNumberResp arn;

    Int32 dayNum = TimeUtil::getDayNum();

    switch (dayNum)
    {
        // 周六日全开
    case 0:case 6:
        arn.mRuneNumber = 0xff;
        break;
    default:
        arn.mRuneNumber = 1 << (dayNum - 1);
    }

    mPlayer->sendMsgWithAck(arn);
}