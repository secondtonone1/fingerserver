#include "TollgateManager.h"
#include "GameServer.h"

using namespace Lynx;

TollgateManager::TollgateManager() : mPlayer(NULL), mTollgateIdInBattle(0), mCheckResetInterval(0),
mResetTime(0), mResetStoreHouseTime(0)/*, mTollgateFighting(false)*/
{
    mTollgateMap.clear();
    //mPlayerPresentHeroDataMap.clear();
}


TollgateManager::~TollgateManager()
{
    mPlayer = NULL;
    mPlayerTollgateData = NULL;
}

bool 
TollgateManager::initial(Player* player)
{
    //mPlayerData.mTollgateData 注释 sec
	//if (player == NULL)
 //   {
 //       LOG_WARN("Player pointer is NULL.");
 //       return false;
 //   }

 //   mPlayer = player;
 //   mPlayerTollgateData = &player->mPlayerData.mTollgateData;
 //   if (mPlayerTollgateData->mSceneList.size() == 0)
 //   {
 //       TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(1011);
 //       if (tollgateTemplate == NULL)
 //       {
 //           LOG_WARN("Get tollgate template failed tollgate id:1011");
 //       }
 //       else
 //       {
 //           TollgateData tollgateData;
 //           tollgateData.mTollgateGuid = LogicSystem::getSingleton().generateTollgateGuid();
 //           tollgateData.mBattleCount = 0;
 //           tollgateData.mEasyData.mTemplateId = tollgateTemplate->mTemplateId;
 //           tollgateData.mEasyData.mStarCount = 0;
 //           tollgateData.mNormalData.mTemplateId = tollgateTemplate->mNormalId;
 //           tollgateData.mNormalData.mStarCount = 0;
 //           tollgateData.mHardData.mTemplateId = tollgateTemplate->mHardId;
 //           tollgateData.mHardData.mStarCount = 0;
 //           addTollgate(tollgateData);
 //       }
 //   }

 //   if (mPlayerTollgateData->mSceneAwardList.size() == 0)
 //   {
 //       SceneGetAwardInfo sceneAwardList;
 //       mPlayerTollgateData->mSceneAwardList.insertTail(sceneAwardList);

 //       PersistUpdatePlayerTollgateAwardDataNotify updateAwardNotify;
 //       updateAwardNotify.mPlayerGuid = mPlayer->getPlayerGuid();
 //       updateAwardNotify.mAwardList = mPlayerTollgateData->mSceneAwardList;
 //       PersistSystem::getSingleton().postThreadMsg(updateAwardNotify, mPlayer->getPlayerGuid());
 //   }

 //   for (WorldSceneList::Iter* iter = mPlayerTollgateData->mSceneList.begin();
 //       iter != mPlayerTollgateData->mSceneList.end(); 
 //       iter = mPlayerTollgateData->mSceneList.next(iter))
 //   {
 //       for (List<TollgateData>::Iter* it = iter->mValue.begin();
 //           it != iter->mValue.end(); it = iter->mValue.next(it))
 //       {
 //           mTollgateMap.insert(it->mValue.mTollgateGuid, &it->mValue);
 //       }
 //   }

 //   mTollgateIdInBattle = 0;
 //   //mTollgateFighting = false;
 //   mTollgateMonsterList.clear();

 //   mCheckResetInterval = TimeUtil::getTimeSec();
 //   mResetTime = TimeUtil::getTodayBaseSec() + TOLLGATE_BATTLE_COUNT_RESET_TIME;
 //   mResetStoreHouseTime = TimeUtil::getTodayBaseSec() + STORE_HOUSE_BATTLE_COUNT_RESET_TIME;

 //   // 重置关卡次数
 //   if (mPlayer->getPlayerData().mAccountData.mLogoutTime <= mResetTime
 //       && TimeUtil::getTimeSec() >= mResetTime)
 //   {
 //       resetTollgateBattleCount();
 //       mResetTime = 24 * 3600 + mResetTime;
 //   }

 //   // 重置宝藏次数
 //   if (mPlayer->getPlayerData().mAccountData.mLogoutTime <= mResetStoreHouseTime
 //       && TimeUtil::getTimeSec() >= mResetStoreHouseTime)
 //   {
 //       resetStoreHouseBattleCount();
 //       mResetStoreHouseTime = 24 * 3600 + mResetStoreHouseTime;
 //   }

    return true;
}

void 
TollgateManager::update(const UInt64& accTime)
{
    // 每60秒更新一次
    if (mCheckResetInterval + 60 < TimeUtil::getTimeSec())
    {
        if (TimeUtil::getTimeSec() > mResetTime)
        {
            resetTollgateBattleCount();
            mResetTime = 24 * 3600 + mResetTime;
        }

        if (TimeUtil::getTimeSec() > mResetStoreHouseTime)
        {
            resetStoreHouseBattleCount();
            mResetStoreHouseTime = 24 * 3600 + mResetStoreHouseTime;
        }

        mCheckResetInterval = TimeUtil::getTimeSec();
    }
}

void 
TollgateManager::release()
{

}

void 
TollgateManager::resetTollgateBattleCount()
{
    for (WorldSceneList::Iter* iter = mPlayerTollgateData->mSceneList.begin();
        iter != mPlayerTollgateData->mSceneList.end(); 
        iter = mPlayerTollgateData->mSceneList.next(iter))
    {
        for (List<TollgateData>::Iter* it = iter->mValue.begin();
            it != iter->mValue.end(); it = iter->mValue.next(it))
        {
            if (it->mValue.mTollgateGuid != 0)
            {
                it->mValue.mBattleCount = 0;
            }
        }
    }
    // DB调用重置关卡次数
    PersistResetPlayerTollgateDataNotify resetNotify;
    resetNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(resetNotify, mPlayer->getPlayerGuid());
}

bool 
TollgateManager::addTollgate(const TollgateData& tollgateData)
{
    if (tollgateData.mTollgateGuid == 0)
    {
        LOG_WARN("Invalid tollgate guid.");
        return false;
    }

    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateData.mEasyData.mTemplateId);
    if (tollgateTemplate == NULL)
    {
        LOG_WARN("Get tollgate template failed tollgate id:%u", tollgateData.mEasyData.mTemplateId);
        return false;
    }

    if (getTollgateDataById(tollgateData.mTollgateGuid) != NULL)
    {
        LOG_WARN("Add tollgate template failed tollgate id:%llu exist", tollgateData.mTollgateGuid);
        return false;
    }

    int sceneId = (int)tollgateData.mEasyData.mTemplateId / 1000;
    if (sceneId > mPlayerTollgateData->mSceneList.size() + 1)
    {
        LOG_WARN("SceneId[%d] is overflow scene list", sceneId);
        return false;
    }

    WorldSceneList::Iter* iterScene = mPlayerTollgateData->mSceneList.getn(sceneId - 1);
    if (iterScene == NULL)
    {
        List<TollgateData> tollgateList;
        iterScene = mPlayerTollgateData->mSceneList.insertTail(tollgateList);
    }

    List<TollgateData>::Iter* iter = iterScene->mValue.insertTail(tollgateData);
    mTollgateMap.insert(iter->mValue.mTollgateGuid, &iter->mValue);

    WorldSceneAwardList::Iter* iterSceneAward = mPlayerTollgateData->mSceneAwardList.getn(sceneId - 1);
    if (iterSceneAward == NULL)
    {
        SceneGetAwardInfo sceneAwardList;
        mPlayerTollgateData->mSceneAwardList.insertTail(sceneAwardList);

        PersistUpdatePlayerTollgateAwardDataNotify updateAwardNotify;
        updateAwardNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        updateAwardNotify.mAwardList = mPlayerTollgateData->mSceneAwardList;
        PersistSystem::getSingleton().postThreadMsg(updateAwardNotify, mPlayer->getPlayerGuid());
    }

    if (tollgateTemplate->mTemplateId != 1011)
    {
        AddTollgateNotify notify;
        notify.mTollgateData = tollgateData;
        mPlayer->sendMsg(notify);
    }

    PersistInsertTollgateDataNotify insertNotify;
    insertNotify.mTollgateData = tollgateData;
    insertNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(insertNotify, mPlayer->getPlayerGuid());
    return true;
}

bool 
TollgateManager::updateTollgate(const TollgateData& tollgateData)
{
    if (tollgateData.mTollgateGuid == 0)
    {
        LOG_WARN("Invalid tollgate guid.");
        return false;
    }

    TollgateData* tollgate = getTollgateDataById(tollgateData.mTollgateGuid);
    if (tollgate == NULL)
    {
        LOG_WARN("Update tollgate[%llu] failed ", tollgateData.mTollgateGuid);
        return false;
    }

    *tollgate = tollgateData;

    UpdateTollgateNotify notify;
    notify.mTollgateData = tollgateData;
    mPlayer->sendMsg(notify);

    PersistUpdateTollgateDataNotify updateNotify;
    updateNotify.mTollgateData = tollgateData;
    updateNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(updateNotify, mPlayer->getPlayerGuid());
    return true;
}

void 
TollgateManager::startTollgateBattle(const Guid& tollgateGuid, UInt8 tollgateDifficulty)
{
    //mTollgateMonsterList.clear();
    //StartTollgateBattleResp resp;
    //resp.mErrorId = checkTollgate(tollgateGuid, tollgateDifficulty);
    //resp.mTollgateGuid = tollgateGuid;
    //resp.mTollgateDifficulty = tollgateDifficulty;
    //if (resp.mErrorId != LynxErrno::None)
    //{
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    ////if (mTollgateFighting)
    ////{
    ////    resp.mErrorId = LynxErrno::TollgateFighting;
    ////    mPlayer->sendMsg(resp);
    ////    return;
    ////}

    //if (LogicSystem::getSingleton().checkBagIsFull(mPlayer))
    //{
    //    resp.mErrorId = LynxErrno::BagIsFull;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //TollgateData* tollgateData = getTollgateDataById(tollgateGuid);
    //if (tollgateData == NULL)
    //{
    //    resp.mErrorId = LynxErrno::InvalidTollgateId;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //UInt32 tollgateId = 0;
    //switch (tollgateDifficulty)
    //{
    //case TOLLGATE_EASY:
    //    {
    //        tollgateId = tollgateData->mEasyData.mTemplateId;
    //    }
    //    break;
    //case TOLLGATE_NORMAL:
    //    {
    //        tollgateId = tollgateData->mNormalData.mTemplateId;
    //    }
    //    break;
    //case TOLLGATE_HARD:
    //    {
    //        tollgateId = tollgateData->mHardData.mTemplateId;
    //    }
    //    break;
    //default:
    //    resp.mErrorId = LynxErrno::InvalidTollgateDifficulty;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    //if (tollgateTemplate == NULL)
    //{
    //    LOG_WARN("Get tollgate template failed tollgate id:%u", tollgateId);
    //    resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    //resp.mTollgateTemplate = tollgateId;
    //resp.mBattleSceneName = tollgateTemplate->mBattleMapAsset;
    //resp.mTollgateName = tollgateTemplate->mName;

    //resp.mErrorId = genTollgateBattleData(tollgateId);
    //if (resp.mErrorId == LynxErrno::None)
    //{
    //    mTollgateIdInBattle = tollgateId;
    //    //mTollgateFighting = true;
    //    // 存储战斗用数据，验证结果时使用
    //    resp.mMonsterList = mTollgateMonsterList;
    //    resp.mMonsterPet = mTollgatePet;
    //}

    //mPlayer->sendMsg(resp);

}

ErrorId 
TollgateManager::genTollgateBattleData(TemplateId tollgateId)
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
        if (!genTollgateMonsterData(monsterLData, monsterId1, 1))
        {
            LOG_WARN("Init monster data failed when add monster, monster templateId[%u]", monsterId1);
            return LynxErrno::TollgateGenDataFailed;
        }
        mTollgateMonsterList.insertTail(monsterLData);
    }

    UInt32 monsterId2 = lineupTemplate->mMonsterId2;
    if (monsterId2 != 0)
    {
        MonsterData monsterMData;
        if (!genTollgateMonsterData(monsterMData, monsterId2, 2))
        {
            LOG_WARN("Init monster data failed when add monster, monster templateId[%u]", monsterId2);
            return LynxErrno::TollgateGenDataFailed;
        }
        mTollgateMonsterList.insertTail(monsterMData);
    }

    UInt32 monsterId3 = lineupTemplate->mMonsterId3;
    if (monsterId3 != 0)
    {
        MonsterData monsterRData;
        if (!genTollgateMonsterData(monsterRData, monsterId3, 3))
        {
            LOG_WARN("Init monster data failed when add monster, monster templateId[%u]", monsterId3);
            return LynxErrno::TollgateGenDataFailed;
        }
        mTollgateMonsterList.insertTail(monsterRData);
    }

    UInt32 posIndex = 4;
    for (List<TemplateId>::Iter* iter = lineupTemplate->mBenchIdList.begin();
        iter != lineupTemplate->mBenchIdList.end();
        iter = lineupTemplate->mBenchIdList.next(iter))
    {
        if (iter->mValue != 0)
        {
            MonsterData monsterData;
            if (!genTollgateMonsterData(monsterData, iter->mValue, posIndex))
            {
                LOG_WARN("Init monster data failed when add monster bench, monster templateId[%u]", iter->mValue);
                return LynxErrno::TollgateGenDataFailed;
            }
            mTollgateMonsterList.insertTail(monsterData);
            posIndex++;
        }
    }

    UInt32 petId = lineupTemplate->mPetId;
    if (petId != 0)
    {
        PetData petData;
        if (!genTollgatePetData(petData, petId))
        {
            LOG_WARN("Init monster data failed when add monster pet, monster templateId[%u]", petId);
            return LynxErrno::TollgateGenDataFailed;
        }
        mTollgatePet = petData;
    }*/
    return LynxErrno::None;
}

bool 
TollgateManager::genTollgateMonsterData(MonsterData& monsterData, TemplateId templateId, UInt32 pos)
{
    //生成关卡怪物数据sec 注释
	//MonsterTemplate* monsterTemplate = MONSTER_TABLE().get(templateId);
    //if (monsterTemplate == NULL)
    //{
    //    LOG_WARN("Get monsterTemplate failed templateId[%u]", templateId);
    //    return false;
    //}

    //monsterData.mBaseData.mMonsterId = LogicSystem::getSingleton().generateHeroGuid();
    //monsterData.mBaseData.mMonsterName = monsterTemplate->mName.c_str();
    //monsterData.mBaseData.mTemplateId = templateId;
    //monsterData.mBaseData.mLevel = monsterTemplate->mLevel;
    //monsterData.mBaseData.mHp = monsterTemplate->mHp;
    //monsterData.mBaseData.mStarLevel = monsterTemplate->mStarLevel;
    //monsterData.mBaseData.mAnger = 0;
    //monsterData.mBaseData.mPresentPos = pos;

    //monsterData.mExtendData.mAttack = monsterTemplate->mAttack;
    //monsterData.mExtendData.mPhysicsDefense = monsterTemplate->mPhysicsDefense;
    //monsterData.mExtendData.mMagicDefense = monsterTemplate->mMagicDefense;
    //monsterData.mExtendData.mCritical = monsterTemplate->mCritical;
    //monsterData.mExtendData.mRescrit = monsterTemplate->mRescrit;
    //monsterData.mExtendData.mHit = monsterTemplate->mHit;
    //monsterData.mExtendData.mDodge = monsterTemplate->mDodge;
    //monsterData.mExtendData.mAvoidDamage = monsterTemplate->mAvoidDamage;
    //monsterData.mExtendData.mSuck = monsterTemplate->mSuck;
    //monsterData.mExtendData.mCure = monsterTemplate->mCure;
    //monsterData.mExtendData.mCriticalDamage = monsterTemplate->mCriticalDamage;
    //monsterData.mExtendData.mHpUpper = monsterTemplate->mHp;
    //monsterData.mExtendData.mAngerUpper = monsterTemplate->mAngerUpper;

    //monsterData.mSkillData.mCommonSkill.mTemplateId = monsterTemplate->mCommonId;
    //if (monsterData.mSkillData.mCommonSkill.mTemplateId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(monsterData.mSkillData.mCommonSkill.mTemplateId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skillTemplate failed templateId[%u]", monsterData.mSkillData.mCommonSkill.mTemplateId);
    //        return false;
    //    }
    //    //monsterData.mSkillData.mCommonSkill.mSkillAttackType = skillTemplate->mAttackType;
    //}

    //monsterData.mSkillData.mAutoSkill.mTemplateId = monsterTemplate->mAutoId;
    //if (monsterData.mSkillData.mAutoSkill.mTemplateId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(monsterData.mSkillData.mAutoSkill.mTemplateId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skillTemplate failed templateId[%u]", monsterData.mSkillData.mAutoSkill.mTemplateId);
    //        return false;
    //    }
    //    //monsterData.mSkillData.mAutoSkill.mSkillAttackType = skillTemplate->mAttackType;
    //}

    //monsterData.mSkillData.mSuperSkill.mTemplateId = monsterTemplate->mSuperId;
    //if (monsterData.mSkillData.mSuperSkill.mTemplateId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(monsterData.mSkillData.mSuperSkill.mTemplateId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skillTemplate failed templateId[%u]", monsterData.mSkillData.mSuperSkill.mTemplateId);
    //        return false;
    //    }
    //    //monsterData.mSkillData.mSuperSkill.mSkillAttackType = skillTemplate->mAttackType;
    //}

    //monsterData.mSkillData.mEnterSkill.mTemplateId = monsterTemplate->mEnterId;
    //if (monsterData.mSkillData.mEnterSkill.mTemplateId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(monsterData.mSkillData.mEnterSkill.mTemplateId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skillTemplate failed templateId[%u]", monsterData.mSkillData.mEnterSkill.mTemplateId);
    //        return false;
    //    }
    //    //monsterData.mSkillData.mEnterSkill.mSkillAttackType = skillTemplate->mAttackType;
    //}

    return true;
}

//bool 
//TollgateManager::genTollgatePetData(PetData& petData, TemplateId templateId)
//{
//    //sec 宠物攻击数据 注释
//	PetTemplate* petTemplate = PET_TABLE().get(templateId);
//    if (petTemplate == NULL)
//    {
//        LOG_WARN("Get petTemplate failed templateId[%u]", templateId);
//        return false;
//    }
//
//    petData.mBaseData.mPetId = LogicSystem::getSingleton().generatePetGuid();
//    petData.mBaseData.mPetName = petTemplate->mName.c_str();
//    petData.mBaseData.mTemplateId = templateId;
//    petData.mBaseData.mLevel = 1;
//    petData.mBaseData.mStarLevel = 0;//petTemplate->mStarLevel;
//    petData.mBaseData.mPresentPos = 1;
//
//    petData.mTalentData.mAttack = petTemplate->mAttack;
//
//    petData.mSkillData.mCommonSkill.mTemplateId = petTemplate->mSkillId;
//    SkillTemplate* skillTemplate = SKILL_TABLE().get(petData.mSkillData.mCommonSkill.mTemplateId);
//    if (skillTemplate == NULL)
//    {
//        LOG_WARN("Get skillTemplate failed templateId[%u]", petData.mSkillData.mCommonSkill.mTemplateId);
//        return false;
//    }
//    //petData.mSkillData.mCommonSkill.mSkillAttackType = skillTemplate->mAttackType;
//
//    return true;
//}

void 
TollgateManager::genOppBattleData(PlayerData& oppBattleData)
{
	// PlayerData   mPetData 字段注释SEC
	/*oppBattleData.mBaseData.mPlayerId = 20150312;

    for (List<MonsterData>::Iter* iter = mTollgateMonsterList.begin();
        iter != mTollgateMonsterList.end(); iter = mTollgateMonsterList.next(iter))
    {
        if (iter->mValue.mBaseData.mMonsterId == 0 ||
            iter->mValue.mBaseData.mPresentPos == 0)
        {
            continue;
        }
        HeroData heroData;
        changeMonsterDataToHeroData(heroData, iter->mValue);
        oppBattleData.mHeroData.mHeroDataList.insertTail(heroData);
    }

    oppBattleData.mPetData.mPetDataList.insertTail(mTollgatePet);*/
}

void 
TollgateManager::changeMonsterDataToHeroData(HeroData& heroData, const MonsterData& monsterData)
{
    //将怪物属性加载到玩家数据 sec注释
	/*heroData.mBaseData.mHeroId = monsterData.mBaseData.mMonsterId;
    heroData.mBaseData.mHeroName = monsterData.mBaseData.mMonsterName.c_str();
    heroData.mBaseData.mHp = monsterData.mBaseData.mHp;
    heroData.mBaseData.mLevel = monsterData.mBaseData.mLevel;
    heroData.mBaseData.mPresentPos = monsterData.mBaseData.mPresentPos;
    heroData.mBaseData.mStarLevel = monsterData.mBaseData.mStarLevel;
    heroData.mBaseData.mTemplateId = monsterData.mBaseData.mTemplateId;

    heroData.mExtendData.mAngerUpper = monsterData.mExtendData.mAngerUpper;
    heroData.mExtendData.mAttack = monsterData.mExtendData.mAttack;
    heroData.mExtendData.mAvoidDamage = monsterData.mExtendData.mAvoidDamage;
    heroData.mExtendData.mCritical = monsterData.mExtendData.mCritical;
    heroData.mExtendData.mCriticalDamage = monsterData.mExtendData.mCriticalDamage;
    heroData.mExtendData.mCure = monsterData.mExtendData.mCure;
    heroData.mExtendData.mDodge = monsterData.mExtendData.mDodge;
    heroData.mExtendData.mHit = monsterData.mExtendData.mHit;
    heroData.mExtendData.mHpUpper = monsterData.mExtendData.mHpUpper;
    heroData.mExtendData.mMagicDefense = monsterData.mExtendData.mMagicDefense;
    heroData.mExtendData.mPhysicsDefense = monsterData.mExtendData.mPhysicsDefense;
    heroData.mExtendData.mRescrit = monsterData.mExtendData.mRescrit;
    heroData.mExtendData.mSuck = monsterData.mExtendData.mSuck;

    heroData.mSkillData.mCommonSkill = monsterData.mSkillData.mCommonSkill;
    heroData.mSkillData.mAutoSkill = monsterData.mSkillData.mAutoSkill;
    heroData.mSkillData.mEnterSkill = monsterData.mSkillData.mEnterSkill;
    heroData.mSkillData.mSuperSkill = monsterData.mSkillData.mSuperSkill;*/
};

void 
TollgateManager::startFiveTollgateSweep(const Guid& tollgateGuid, UInt8 tollgateDifficulty)
{
    StartFiveTollgateSweepResp resp;
    resp.mTollgateGuid = tollgateGuid;
    resp.mTollgateDifficulty = tollgateDifficulty;
    //if (mTollgateFighting)
    //{
    //    resp.mTollgateId = tollgateId;
    //    resp.mErrorId = LynxErrno::TollgateFighting;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    resp.mErrorId = checkTollgate(tollgateGuid, tollgateDifficulty);
    if (resp.mErrorId != LynxErrno::None)
    {
        mPlayer->sendMsg(resp);
        return;
    }

    TollgateData* tollgateData = getTollgateDataById(tollgateGuid);
    if (tollgateData == NULL)
    {
        LOG_WARN("Get tollgate failed tollgate id:%llu", tollgateGuid);
        resp.mErrorId = LynxErrno::TollgateNotExist;
        mPlayer->sendMsg(resp);
        return;
    }

    UInt32 tollgateId = 0;
    switch (tollgateDifficulty)
    {
    case TOLLGATE_EASY:
        {
            if (tollgateData->mEasyData.mStarCount == 0)
            {
                resp.mErrorId = LynxErrno::TollgateSweepMustDoneFirst;
                mPlayer->sendMsg(resp);
                return;
            }
            tollgateId = tollgateData->mEasyData.mTemplateId;
        }
        break;
    case TOLLGATE_NORMAL:
        {
            if (tollgateData->mEasyData.mStarCount == 0 
                || tollgateData->mNormalData.mStarCount == 0)
            {
                resp.mErrorId = LynxErrno::TollgateSweepMustDoneFirst;
                mPlayer->sendMsg(resp);
                return;
            }
            tollgateId = tollgateData->mNormalData.mTemplateId;
        }
        break;
    case TOLLGATE_HARD:
        {
            if (tollgateData->mEasyData.mStarCount == 0
                || tollgateData->mNormalData.mStarCount == 0
                || tollgateData->mHardData.mStarCount == 0)
            {
                resp.mErrorId = LynxErrno::TollgateSweepMustDoneFirst;
                mPlayer->sendMsg(resp);
                return;
            }
            tollgateId = tollgateData->mHardData.mTemplateId;
        }
        break;
    default:
        resp.mErrorId = LynxErrno::InvalidTollgateDifficulty;
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

    UInt32 sweepCount = tollgateTemplate->mMaxBattleCount - tollgateData->mBattleCount;
    if (sweepCount > 5)
    {
        sweepCount = 5;
    }

	//精力值先注释 sec
    UInt32 needEnergy = sweepCount * tollgateTemplate->mNeedEnergy;
	/*   
	if (needEnergy > mPlayer->getPlayerEnergy())
    {
        resp.mErrorId = LynxErrno::TollgateEnergyLimit;
        mPlayer->sendMsg(resp);
        return;
    }*/

    resp.mTollgateGuid = tollgateGuid;
    resp.mTollgateDifficulty = tollgateDifficulty;

    resp.mAwardList.clear();
    for (int i = 0; i < sweepCount; i++)
    {
        getDropItemList(tollgateId, resp.mAwardList);
    }

    if (resp.mAwardList.size() != 0)
    {
        if (LogicSystem::getSingleton().checkBagIsFull(mPlayer, resp.mAwardList))
        {
            resp.mErrorId = LynxErrno::BagIsFull;
            mPlayer->sendMsg(resp);
            return;
        }
    }

    // 更新当前关卡信息
    TollgateData tollgate;
    tollgate = *tollgateData;
    tollgate.mBattleCount = tollgateTemplate->mMaxBattleCount;

    if (!updateTollgate(tollgate))
    {
        LOG_WARN("Update tollgate failed tollgateId[%u]", tollgateId);
        resp.mErrorId = LynxErrno::TollgateUpdateFailed;
        mPlayer->sendMsg(resp);
        return;
    }

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

    mPlayer->reducePlayerEnergy(needEnergy);

    // 增加经验看是否是升级，player和hero
    UInt32 teamExp = sweepCount * tollgateTemplate->mTollgateAwardInfo.mTeamExp;
    UInt32 playerLevelUp = 0;
    mPlayer->addExpForPlayer(playerLevelUp, teamExp);

    UInt32 heroExp = sweepCount * tollgateTemplate->mTollgateAwardInfo.mHeroExp;
    mPlayer->getHeroManager().refinePresentHeros(resp.mHeroLevelUp, heroExp);

    UInt32 gold = sweepCount * tollgateTemplate->mTollgateAwardInfo.mGold;
    mPlayer->increasePlayerGold(gold);

    UInt32 springExp = sweepCount * tollgateTemplate->mTollgateAwardInfo.mSpring;
    mPlayer->increasePlayerHeroExpPool(springExp);

    resp.mAwardGold = gold;
    resp.mAwardTeamExp = teamExp;
    resp.mPlayerLevelUp = playerLevelUp == 0 ? false : true;
    resp.mAwardHeroExp = heroExp;
    resp.mAwardSpring = springExp;
    mPlayer->sendMsg(resp);
}

void 
TollgateManager::againTollgateBattle(const Guid& tollgateGuid, UInt8 tollgateDifficulty)
{
    AgainTollgateBattleResp resp;
    //if (mTollgateFighting)
    //{
    //    resp.mTollgateId = tollgateId;
    //    resp.mErrorId = LynxErrno::TollgateFighting;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}

    resp.mErrorId = checkTollgate(tollgateGuid, tollgateDifficulty);
    if (resp.mErrorId == LynxErrno::None)
    {
        //mTollgateIdInBattle = tollgateId;
        //mTollgateFighting = true;
    }

    if (LogicSystem::getSingleton().checkBagIsFull(mPlayer))
    {
        resp.mErrorId = LynxErrno::BagIsFull;
        mPlayer->sendMsg(resp);
        return;
    }

    resp.mTollgateGuid = tollgateGuid;
    resp.mTollgateDifficulty = tollgateDifficulty;
    mPlayer->sendMsg(resp);
}

void 
TollgateManager::getScenePassAward(TemplateId sceneId, UInt8 chestsIndex)
{
    GetScenePassAwardResp resp;
    resp.mErrorId = LynxErrno::None;
    resp.mSceneId = sceneId;
    resp.mChestsIndex = chestsIndex;
    if (sceneId == 0)
    {
        resp.mErrorId = LynxErrno::GetScenePassAwardFailed;
        mPlayer->sendMsg(resp);
        return;
    }
    
    WorldSceneAwardList::Iter* sceneAwardIter = mPlayerTollgateData->mSceneAwardList.getn(sceneId - 1);
    if (sceneAwardIter == NULL)
    {
        resp.mErrorId = LynxErrno::GetScenePassAwardFailed;
        mPlayer->sendMsg(resp);
        return;
    }

    bool isAlreadyGet = false;
    switch (chestsIndex)
    {
    case 1:
        {
            if (sceneAwardIter->mValue.mIsChests1Get)
            {
                isAlreadyGet = true;
            }
        }
        break;
    case 2:
        {
            if (sceneAwardIter->mValue.mIsChests2Get)
            {
                isAlreadyGet = true;
            }
        }
        break;
    case 3:
        {
            if (sceneAwardIter->mValue.mIsChests3Get)
            {
                isAlreadyGet = true;
            }
        }
        break;
    default:
        resp.mErrorId = LynxErrno::GetScenePassAwardFailed;
        mPlayer->sendMsg(resp);
        return;
    }

    if (isAlreadyGet)
    {
        resp.mErrorId = LynxErrno::ScenePassAwardAlreadyGet;
        mPlayer->sendMsg(resp);
        return;
    }

    WorldSceneList::Iter* iter = mPlayerTollgateData->mSceneList.getn(sceneId - 1);
    if (iter == NULL)
    {
        resp.mErrorId = LynxErrno::GetScenePassAwardFailed;
        mPlayer->sendMsg(resp);
        return;
    }

    UInt32 starCount = 0;
    for (List<TollgateData>::Iter* tollgateItr = iter->mValue.begin();
        tollgateItr != iter->mValue.end(); 
        tollgateItr = iter->mValue.next(tollgateItr))
    {
        starCount += tollgateItr->mValue.getTollgateStar();
    }

    SceneTemplate* sceneTemplate = SCENE_TABLE().get(sceneId);
    if (sceneTemplate == NULL)
    {
        resp.mErrorId = LynxErrno::GetScenePassAwardFailed;
        mPlayer->sendMsg(resp);
        return;
    }

    UInt32 awardId = 0;
    UInt32 needStar = 0;
    switch (chestsIndex)
    {
    case 1:
        {
            awardId = sceneTemplate->mChestsAwardId1;
            needStar = sceneTemplate->mChestsNeedStar1;
        }
        break;
    case 2:
        {
            awardId = sceneTemplate->mChestsAwardId2;
            needStar = sceneTemplate->mChestsNeedStar2;
        }
        break;
    case 3:
        {
            awardId = sceneTemplate->mChestsAwardId3;
            needStar = sceneTemplate->mChestsNeedStar3;
        }
        break;
    }

    if (starCount < needStar)
    {
        resp.mErrorId = LynxErrno::NotEnoughStarForGetAward;
        mPlayer->sendMsg(resp);
        return;
    }

    AwardTemplate* awardTemplate = AWARD_TABLE().get(awardId);
    if (awardTemplate == NULL)
    {
        resp.mErrorId = LynxErrno::GetScenePassAwardFailed;
        mPlayer->sendMsg(resp);
        return;
    }

    if (awardTemplate->mAwardItemList.size() == 0)
    {
        resp.mErrorId = LynxErrno::GetScenePassAwardFailed;
        mPlayer->sendMsg(resp);
        return;
    }

    for (List<AwardLine>::Iter* awardIter = awardTemplate->mAwardItemList.begin();
        awardIter != awardTemplate->mAwardItemList.end(); 
        awardIter = awardTemplate->mAwardItemList.next(awardIter))
    {
        MsgItemBaseData msgItemBaseData;
        msgItemBaseData.mTemplateId = awardIter->mValue.mItemId;
        msgItemBaseData.mType = awardIter->mValue.mItemType;
        msgItemBaseData.mCount = awardIter->mValue.mCount;
        resp.mAwardList.insertTail(msgItemBaseData);
    }

    if (resp.mAwardList.size() != 0)
    {
        if (LogicSystem::getSingleton().checkBagIsFull(mPlayer, resp.mAwardList))
        {
            resp.mErrorId = LynxErrno::BagIsFull;
            mPlayer->sendMsg(resp);
            return;
        }
    }

    for (List<MsgItemBaseData>::Iter* it = resp.mAwardList.begin();
        it != resp.mAwardList.end(); it = resp.mAwardList.next(it))
    {
        switch (it->mValue.mType)
        {
        case ITEM_TYPE:
            {
                mPlayer->getItemManager().createItem(it->mValue.mTemplateId, it->mValue.mCount, 0);
            }
            break;
        case EQUIP_TYPE:
            {
                mPlayer->getEquipManager().createEquip(it->mValue.mTemplateId, 0);
            }
            break;
        case RUNE_TYPE:
            {
                mPlayer->getRuneManager().createRune(it->mValue.mTemplateId, 0);
            }
            break;
        case HERO_TYPE:
            {
                mPlayer->getHeroManager().createHero(it->mValue.mTemplateId);
            }
            break;
        case HERO_DEBRIS_TYPE:
            {
                mPlayer->getHeroDebrisManager().createHeroDebris(it->mValue.mTemplateId, it->mValue.mCount, 0);
            }
            break;
        case PET_DEBRIS_TYPE:
            {
                mPlayer->getPetDebrisManager().createPetDebris(it->mValue.mTemplateId, it->mValue.mCount, 0);
            }
            break;
        default:
            LOG_WARN("Invalid item type %u", it->mValue.mType);
            break;
        }
    }

    // 修改领奖状态，调用写入DB
    switch (chestsIndex)
    {
    case 1:
        {
            sceneAwardIter->mValue.mIsChests1Get = true;
        }
        break;
    case 2:
        {
            sceneAwardIter->mValue.mIsChests2Get = true;
        }
        break;
    case 3:
        {
            sceneAwardIter->mValue.mIsChests3Get = true;
        }
        break;
    }

    PersistUpdatePlayerTollgateAwardDataNotify updateAwardNotify;
    updateAwardNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    updateAwardNotify.mAwardList = mPlayerTollgateData->mSceneAwardList;
    PersistSystem::getSingleton().postThreadMsg(updateAwardNotify, mPlayer->getPlayerGuid());

    mPlayer->sendMsg(resp);

}

ErrorId 
TollgateManager::checkTollgate(const Guid& tollgateGuid, UInt8 tollgateDifficulty)
{
    //return LynxErrno::None;
    TollgateData* tollgateData = getTollgateDataById(tollgateGuid);
    if (tollgateData == NULL)
    {
        return LynxErrno::InvalidTollgateId;
    }

    UInt32 tollgateId = 0;
    switch (tollgateDifficulty)
    {
    case TOLLGATE_EASY:
        {
            tollgateId = tollgateData->mEasyData.mTemplateId;
        }
        break;
    case TOLLGATE_NORMAL:
        {
            if (tollgateData->mEasyData.mStarCount == 0)
            {
                return LynxErrno::InvalidTollgateId;
            }
            tollgateId = tollgateData->mNormalData.mTemplateId;
        }
        break;
    case TOLLGATE_HARD:
        {
            if (tollgateData->mEasyData.mStarCount == 0
                || tollgateData->mNormalData.mStarCount == 0)
            {
                return LynxErrno::InvalidTollgateId;
            }
            tollgateId = tollgateData->mHardData.mTemplateId;
        }
        break;
    default:
        return LynxErrno::InvalidTollgateDifficulty;
    }

    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    if (tollgateTemplate == NULL)
    {
        return LynxErrno::NoTollgateIdTemplate;
    }

    if (mPlayer->getPlayerLeval() < tollgateTemplate->mNeedLevel)
    {
        return LynxErrno::TollgateLevelLimit;
    }
	
	//Energy 被注释了 sec
    /*if (mPlayer->getPlayerEnergy() < tollgateTemplate->mNeedEnergy)
    {
        return LynxErrno::TollgateEnergyLimit;
    }*/

    if (tollgateData->mBattleCount >= tollgateTemplate->mMaxBattleCount)
    {
        return LynxErrno::TollgateBattleCountNull;
    }

    if (tollgateTemplate->mBattleMapAsset.empty())
    {
        return LynxErrno::TollgateSceneNotExist;
    }

    return LynxErrno::None;
}

TollgateData* 
TollgateManager::getTollgateDataById(const Guid& tollgateGuid)
{
    Map<Guid, TollgateData*>::Iter* iter = mTollgateMap.find(tollgateGuid);
    if (iter != mTollgateMap.end())
    {
        return iter->mValue;
    }
    return NULL;
}

TollgateData* 
TollgateManager::getNextTollgateByTemplateId(TemplateId templateId)
{
    for (Map<Guid, TollgateData*>::Iter* iter = mTollgateMap.begin();
        iter != mTollgateMap.end(); iter = mTollgateMap.next(iter))
    {
        if (iter->mValue != NULL && iter->mValue->mEasyData.mTemplateId == templateId)
        {
            return iter->mValue;
        }
    }
    return NULL;
}

void 
TollgateManager::tollgateBattleResult(const TollgateBattleResultReq& msg)
{
    //TollgateBattleResultResp resptest;
    //resptest.mTollgateId = mTollgateIdInBattle;
    //resptest.mErrorId = LynxErrno::None;
    //resptest.mStarCount = 3;
    //resptest.mAwardGold = 100;
    //resptest.mAwardTeamExp = 50;
    //resptest.mPlayerLevelUp = false;
    //resptest.mAwardHeroExp = 100;
    //mPlayer->sendMsg(resptest);
    //return;

    UInt8 starCount = 0;
    bool isFirstPass = false;

    TollgateBattleResultResp resp;
    resp.mTollgateGuid = msg.mTollgateGuid;
    resp.mTollgateDifficulty = msg.mTollgateDifficulty;
    resp.mTollgateTemplate = mTollgateIdInBattle;
    resp.mNextTollgateGuid = 0;

    resp.mErrorId = checkTollgateResult(msg.mTollgateGuid, 
        msg.mTollgateDifficulty, msg.mBattleVerifyData);
    if (resp.mErrorId != LynxErrno::None)
    {
        //resp.mErrorId = errorId;
        mPlayer->sendMsg(resp);
        return;
    }

    if (!msg.mBattleVerifyData.mIsVictory)
    {
        resp.mStarCount = 0;
        mPlayer->sendMsg(resp);
        return;
    }
    //if (!mTollgateFighting)
    //{
    //    resp.mTollgateId = msg.mTollgateId;
    //    resp.mErrorId = LynxErrno::NoTollgateInBattle;
    //    mPlayer->sendMsg(resp);
    //    return;
    //}
    //mTollgateFighting = false;

    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(mTollgateIdInBattle);
    if (tollgateTemplate == NULL)
    {
        LOG_WARN("Get tollgate template failed tollgate id:%u", mTollgateIdInBattle);
        resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
        mPlayer->sendMsg(resp);
        return;
    }

    TollgateData* tollgateData = getTollgateDataById(msg.mTollgateGuid);
    if (tollgateData == NULL)
    {
        LOG_WARN("Get tollgate failed tollgate id:%llu", msg.mTollgateGuid);
        resp.mErrorId = LynxErrno::TollgateNotExist;
        mPlayer->sendMsg(resp);
        return;
    }

    if (msg.mTollgateDifficulty == TOLLGATE_EASY 
        && tollgateData->mEasyData.mStarCount == 0)
    {
        isFirstPass = true;
    }

    if (msg.mBattleVerifyData.mIsVictory)
    {
        starCount = 1;
        //if (msg.mBattleVerifyData.mDeathCount == 0)
        //{
        //    starCount = 3;
        //}
        //else if (msg.mBattleVerifyData.mDeathCount == 1)
        //{
        //    starCount = 2;
        //}
        //else
        //{
        //    starCount = 1;
        //}

		//相应的副本需要扣除体力值
       /* if (mPlayer->getPlayerEnergy() < tollgateTemplate->mNeedEnergy)
        {
            LOG_WARN("Player energy is less than tollgate need energy:%u", mTollgateIdInBattle);
        }
        else
        {
            mPlayer->reducePlayerEnergy(tollgateTemplate->mNeedEnergy);
        }*/

        // 副本过关即可开启下一关卡
        if (isFirstPass)
        {
            if (tollgateTemplate->mNextId != 0)
            {
                TollgateTemplate* nextTollgateTemplate = TOLLGATE_TABLE().get(tollgateTemplate->mNextId);
                if (nextTollgateTemplate == NULL)
                {
                    LOG_WARN("Get next tollgate template failed tollgate id:%u", tollgateTemplate->mNextId);
                    resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
                    mPlayer->sendMsg(resp);
                    return;
                }

                TollgateData nextTollgateData;
                nextTollgateData.mTollgateGuid = LogicSystem::getSingleton().generateTollgateGuid();
                nextTollgateData.mBattleCount = 0;
                nextTollgateData.mEasyData.mTemplateId = nextTollgateTemplate->mTemplateId;
                nextTollgateData.mEasyData.mStarCount = 0;
                nextTollgateData.mNormalData.mTemplateId = nextTollgateTemplate->mNormalId;
                nextTollgateData.mNormalData.mStarCount = 0;
                nextTollgateData.mHardData.mTemplateId = nextTollgateTemplate->mHardId;
                nextTollgateData.mHardData.mStarCount = 0;
                if (!addTollgate(nextTollgateData))
                {
                    LOG_WARN("Add tollgate failed tollgateId[%u]", tollgateTemplate->mNextId);
                    resp.mErrorId = LynxErrno::TollgateAddFailed;
                    mPlayer->sendMsg(resp);
                    return;
                }
            }
        }
    }

    if (tollgateTemplate->mNextId != 0)
    {
        TollgateData* nextTollgate = getNextTollgateByTemplateId(tollgateTemplate->mNextId);
        if (nextTollgate != NULL)
        {
            resp.mNextTollgateGuid = nextTollgate->mTollgateGuid;
        }
    }

    resp.mAwardList.clear();
    if (getDropItemList(mTollgateIdInBattle, resp.mAwardList))
    {
        if (resp.mAwardList.size() != 0)
        {
            if (LogicSystem::getSingleton().checkBagIsFull(mPlayer, resp.mAwardList))
            {
                resp.mErrorId = LynxErrno::BagIsFull;
                mPlayer->sendMsg(resp);
                return;
            }
        }
    }

    // 更新当前关卡信息
    switch (msg.mTollgateDifficulty)
    {
    case TOLLGATE_EASY:
        {
            tollgateData->mEasyData.mStarCount = starCount;
        }
        break;
    case TOLLGATE_NORMAL:
        {
            tollgateData->mNormalData.mStarCount = starCount;
        }
        break;
    case TOLLGATE_HARD:
        {
            tollgateData->mHardData.mStarCount = starCount;
        }
        break;
    }

    TollgateData tollgate;
    tollgate = *tollgateData;
    // 胜利才计次数
    if (msg.mBattleVerifyData.mIsVictory)
    {
        tollgate.mBattleCount++;
    }

    if (!updateTollgate(tollgate))
    {
        LOG_WARN("Update tollgate failed tollgateId[%llu]", msg.mTollgateGuid);
        resp.mErrorId = LynxErrno::TollgateUpdateFailed;
        mPlayer->sendMsg(resp);
        return;
    }

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

    // 增加经验看是否是升级，player和hero
    UInt32 playerLevelUp = 0;
    mPlayer->addExpForPlayer(playerLevelUp, tollgateTemplate->mTollgateAwardInfo.mTeamExp);
    mPlayer->getHeroManager().refinePresentHeros(resp.mHeroLevelUp, tollgateTemplate->mTollgateAwardInfo.mHeroExp);
    mPlayer->increasePlayerGold(tollgateTemplate->mTollgateAwardInfo.mGold);
    mPlayer->increasePlayerHeroExpPool(tollgateTemplate->mTollgateAwardInfo.mSpring);

    resp.mStarCount = starCount;
    resp.mAwardGold = tollgateTemplate->mTollgateAwardInfo.mGold;
    resp.mAwardTeamExp = tollgateTemplate->mTollgateAwardInfo.mTeamExp;
    resp.mPlayerLevelUp = playerLevelUp == 0 ? false : true;
    resp.mAwardHeroExp = tollgateTemplate->mTollgateAwardInfo.mHeroExp;
    resp.mAwardSpring = tollgateTemplate->mTollgateAwardInfo.mSpring;

    mPlayer->sendMsg(resp);
}

bool 
TollgateManager::getDropItemList(TemplateId tollgateId, List<MsgItemBaseData>& dropItemlist)
{
    float indulgePer = 1.0f;

    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    if (tollgateTemplate == NULL)
    {
        LOG_WARN("Get tollgate template failed tollgate id:%u", tollgateId);
        return false;
    }
    // 获取每一个得到的掉落ID
    List<TemplateId> dropIdList;
    // 读取表
    if (tollgateTemplate->mTollgateAwardInfo.mPackage1.mDropId)
    {
        for (Int32 i = 0; i < tollgateTemplate->mTollgateAwardInfo.mPackage1.mLoopTime; ++i)
        {
            // 随机
            Int32 randValue = RANDOM().getUInt32Random(0, 1000);
            //Int32 randValue = rand() % 1000 + 1;
            if (randValue < tollgateTemplate->mTollgateAwardInfo.mPackage1.mDropOdds * indulgePer)
            {
                // 几率中
                dropIdList.insertTail(tollgateTemplate->mTollgateAwardInfo.mPackage1.mDropId);
            }
        }
    }
    if (tollgateTemplate->mTollgateAwardInfo.mPackage2.mDropId)
    {
        for (Int32 i = 0; i < tollgateTemplate->mTollgateAwardInfo.mPackage2.mLoopTime; ++i)
        {
            // 随机
            Int32 randValue = RANDOM().getUInt32Random(0, 1000);
            //Int32 randValue = rand() % 1000 + 1;
            if (randValue < tollgateTemplate->mTollgateAwardInfo.mPackage2.mDropOdds * indulgePer)
            {
                // 几率中
                dropIdList.insertTail(tollgateTemplate->mTollgateAwardInfo.mPackage2.mDropId);
            }
        }
    }
    if (tollgateTemplate->mTollgateAwardInfo.mPackage3.mDropId)
    {
        for (Int32 i = 0; i < tollgateTemplate->mTollgateAwardInfo.mPackage3.mLoopTime; ++i)
        {
            // 随机
            Int32 randValue = RANDOM().getUInt32Random(0, 1000);
            //Int32 randValue = rand() % 1000 + 1;
            if (randValue < tollgateTemplate->mTollgateAwardInfo.mPackage3.mDropOdds * indulgePer)
            {
                // 几率中
                dropIdList.insertTail(tollgateTemplate->mTollgateAwardInfo.mPackage3.mDropId);
            }
        }
    }
    if (tollgateTemplate->mTollgateAwardInfo.mPackage4.mDropId)
    {
        for (Int32 i = 0; i < tollgateTemplate->mTollgateAwardInfo.mPackage4.mLoopTime; ++i)
        {
            // 随机
            Int32 randValue = RANDOM().getUInt32Random(0, 1000);
            //Int32 randValue = rand() % 1000 + 1;
            if (randValue < tollgateTemplate->mTollgateAwardInfo.mPackage4.mDropOdds * indulgePer)
            {
                // 几率中
                dropIdList.insertTail(tollgateTemplate->mTollgateAwardInfo.mPackage4.mDropId);
            }
        }
    }
    if (tollgateTemplate->mTollgateAwardInfo.mPackage5.mDropId)
    {
        for (Int32 i = 0; i < tollgateTemplate->mTollgateAwardInfo.mPackage5.mLoopTime; ++i)
        {
            // 随机
            Int32 randValue = RANDOM().getUInt32Random(0, 1000);
            //Int32 randValue = rand() % 1000 + 1;
            if (randValue < tollgateTemplate->mTollgateAwardInfo.mPackage5.mDropOdds * indulgePer)
            {
                // 几率中
                dropIdList.insertTail(tollgateTemplate->mTollgateAwardInfo.mPackage5.mDropId);
            }
        }
    }

    // 拿到的掉落包id
    for (List<TemplateId>::Iter* iter = dropIdList.begin();
        iter != dropIdList.end(); iter = dropIdList.next(iter))
    {
        // 获取掉落包的模版
        DropItemTemplate* dropItemTemplate = DROP_ITEM_TABLE().get(iter->mValue);
        if (!dropItemTemplate)
        {
            LOG_WARN("DropSetTemplate Id: %u Not Find", iter->mValue);
            continue;
        }
        // 获取掉落具体哪个物品
        // 先查看一共有多少上限
        UInt32 total = 0;
        for (List<DropLine>::Iter* iter = dropItemTemplate->mDropItemList.begin();
            iter != dropItemTemplate->mDropItemList.end();
            iter = dropItemTemplate->mDropItemList.next(iter))
        {
            total += iter->mValue.mDropOdds;
        }
        if (total == 0)
        {
            continue;
        }
        // 随机一个数
        UInt32 randValue = RANDOM().getUInt32Random(1, total);
        //UInt32 randValue = rand() % total + 1;
        // 判断这个数
        UInt32 currentValue = 0;
        MsgItemBaseData msgItemBaseData;
        for (List<DropLine>::Iter* iter = dropItemTemplate->mDropItemList.begin();
            iter != dropItemTemplate->mDropItemList.end();
            iter = dropItemTemplate->mDropItemList.next(iter))
        {
            currentValue += iter->mValue.mDropOdds;
            // 超过不是这个数
            if (randValue > currentValue)
            {
                continue;
            }
            // 没超过 是这个数
            msgItemBaseData.mTemplateId = iter->mValue.mItemId;
            msgItemBaseData.mType = iter->mValue.mItemType;
            msgItemBaseData.mCount = 1;
            break;
        }

        if (!checkTemplateInvalid(msgItemBaseData.mTemplateId, msgItemBaseData.mType))
        {
            continue;
        }

        bool findExist = false;
        for (List<MsgItemBaseData>::Iter* it = dropItemlist.begin();
            it != dropItemlist.end(); it = dropItemlist.next(it))
        {
            if (it->mValue.mTemplateId == msgItemBaseData.mTemplateId)
            {
                it->mValue.mCount++;
                findExist = true;
                break;
            }
        }

        if (!findExist)
        {
            dropItemlist.insertTail(msgItemBaseData);
        }
    }
    return true;
}

//void 
//TollgateManager::tollgateIncreaseHeroExp(List<Guid>& heroLevelUpList, UInt32 heroExp)
//{
//    bool heroLevelUp = false;
//    for (int i = 1; i < 6; i++)
//    {
//        Map<UInt8, HeroData*>::Iter* heroIter = mPlayer->getPlayerPresentData().mPresentHeroMap.find(i);
//        if (heroIter == mPlayer->getPlayerPresentData().mPresentHeroMap.end())
//        {
//            continue;
//        }
//        HeroGrowTemplate* heroGrow = HERO_GROW_TABLE().get(heroIter->mValue->mBaseData.mLevel);
//        if (heroGrow)
//        {
//            if (heroIter->mValue->mBaseData.mLevel >= HERO_LEVEL_MAX_VALUE
//                && heroGrow->mExperience == heroIter->mValue->mBaseData.mLevelExp)
//            {
//                LOG_WARN("Hero is the top level [%u]", heroIter->mValue->mBaseData.mLevel);
//                continue;
//            }
//
//            UInt32 expValue = heroExp;
//            //UInt32 tmpExp = heroIter->mValue->mBaseData.mExp;
//
//            // 可能连升多个级别
//            while (heroGrow && heroGrow->mExperience <= (heroIter->mValue->mBaseData.mLevelExp + expValue))
//            {
//                if (heroIter->mValue->mBaseData.mLevel >= HERO_LEVEL_MAX_VALUE)
//                {
//                    expValue = heroGrow->mExperience - heroIter->mValue->mBaseData.mLevelExp;
//                    break;
//                }
//
//                if (!(HERO_GROW_TABLE().get(heroIter->mValue->mBaseData.mLevel + 1)))
//                {
//                    expValue = heroGrow->mExperience - heroIter->mValue->mBaseData.mLevelExp;
//                    break;
//                }
//
//                expValue -= (heroGrow->mExperience - heroIter->mValue->mBaseData.mLevelExp);
//
//                //OnLevelUp(playerGrow->m_LevelUpExp);
//
//                mPlayer->getHeroManager().increaseHeroLevel(heroIter->mValue->mBaseData.mHeroId, 1);
//                mPlayer->getHeroManager().reduceHeroLevelExp(heroIter->mValue->mBaseData.mHeroId, heroGrow->mExperience);
//
//                //updateExtendDataByLevelChange();
//                heroLevelUp = true;
//                if (heroIter->mValue->mBaseData.mLevel >= HERO_LEVEL_MAX_VALUE)
//                {
//                    break;
//                }
//
//                heroGrow = HERO_GROW_TABLE().get(heroIter->mValue->mBaseData.mLevel);
//            }
//            mPlayer->getHeroManager().increaseHeroLevelExp(heroIter->mValue->mBaseData.mHeroId, expValue);
//            if (heroLevelUp)
//            {
//                heroLevelUpList.insertTail(heroIter->mValue->mBaseData.mHeroId);
//                heroLevelUp = false;
//            }
//        }
//        else
//        {
//            LOG_WARN("level not exist [%u]", heroIter->mValue->mBaseData.mLevel);
//        }
//    }
//}
//
//bool 
//TollgateManager::tollgateIncreasePlayerExp(UInt32 teamExp)
//{
//    bool playerLevelUp = false;
//    mPlayer->increasePlayerLevelExp(teamExp);
//    //HeroGrowTemplate* playerGrow = HERO_GROW_TABLE().get(mPlayer->getPlayerLeval());
//    //if (playerGrow)
//    //{
//    //    if (mPlayer->getPlayerLeval() >= HERO_LEVEL_MAX_VALUE
//    //        && playerGrow->mExperience == mPlayer->getPlayerExp())
//    //    {
//    //        LOG_WARN("Player is the top level [%u]", mPlayer->getPlayerLeval());
//    //        continue;
//    //    }
//
//    //    UInt32 expValue = tollgateTemplate->mTollgateAwardInfo.mTeamExp;
//    //    //UInt32 tmpExp = heroIter->mValue->mBaseData.mExp;
//
//    //    // 可能连升多个级别
//    //    while (playerGrow && playerGrow->mExperience <= (mPlayer->getPlayerExp() + expValue))
//    //    {
//    //        if (mPlayer->getPlayerLeval() >= HERO_LEVEL_MAX_VALUE)
//    //        {
//    //            expValue = playerGrow->mExperience - mPlayer->getPlayerExp();
//    //            break;
//    //        }
//
//    //        if (!(HERO_GROW_TABLE().get(mPlayer->getPlayerLeval() + 1)))
//    //        {
//    //            expValue = playerGrow->mExperience - mPlayer->getPlayerExp();
//    //            break;
//    //        }
//
//    //        expValue -= (playerGrow->mExperience - mPlayer->getPlayerExp());
//
//    //        //OnLevelUp(playerGrow->m_LevelUpExp);
//
//    //        mPlayer->increasePlayerLevel(1);
//    //        mPlayer->reducePlayerExp(playerGrow->mExperience);
//
//    //        //updateExtendDataByLevelChange();
//    //        playerLevelUp = true;
//    //        if (mPlayer->getPlayerLeval() >= HERO_LEVEL_MAX_VALUE)
//    //        {
//    //            break;
//    //        }
//
//    //        playerGrow = HERO_GROW_TABLE().get(mPlayer->getPlayerLeval());
//    //    }
//    //    mPlayer->increasePlayerExp(expValue);
//    //}
//    //else
//    //{
//    //    LOG_WARN("level not exist [%u]", mPlayer->getPlayerLeval());
//    //}
//
//    return playerLevelUp;
//}

bool 
TollgateManager::checkTemplateInvalid(TemplateId templateId, UInt8 itemType)
{
    switch (itemType)
    {
    case EQUIP_TYPE:
        {
            EquipTemplate* equipTemplate = EQUIP_TABLE().get(templateId);
            if (equipTemplate == NULL)
            {
                LOG_WARN("Get equip template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case ITEM_TYPE:
        {
            ItemTemplate* itemTemplate = ITEM_TABLE().get(templateId);
            if (itemTemplate == NULL)
            {
                LOG_WARN("Get item template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case HERO_TYPE:
        {
            HeroTemplate* heroTemplate = HERO_TABLE().get(templateId);
            if (heroTemplate == NULL)
            {
                LOG_WARN("Get hero template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case HERO_DEBRIS_TYPE:
        {
            HeroDebrisTemplate* heroDebrisTemplate = HERO_DEBRIS_TABLE().get(templateId);
            if (heroDebrisTemplate == NULL)
            {
                LOG_WARN("Get hero debris template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case PET_TYPE:
        {
            PetTemplate* petTemplate = PET_TABLE().get(templateId);
            if (petTemplate == NULL)
            {
                LOG_WARN("Get pet template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case PET_DEBRIS_TYPE:
        {
            PetDebrisTemplate* petDebrisTemplate = PET_DEBRIS_TABLE().get(templateId);
            if (petDebrisTemplate == NULL)
            {
                LOG_WARN("Get pet debris template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    case RUNE_TYPE:
        {
            RuneTemplate* runeTemplate = RUNE_TABLE().get(templateId);
            if (runeTemplate == NULL)
            {
                LOG_WARN("Get rune template failed templateId[%u]", templateId);
                return false;
            }
        }
        break;
    default:
        LOG_WARN("Unknown template type[%u] id[%u]", itemType, templateId);
        return false;
    }

    return true;
}

ErrorId 
TollgateManager::checkTollgateResult(const Guid& tollgateGuid, UInt8 tollgateDifficulty, 
                                     const BattleVerifyData& battleVerifyData)
{
    TollgateData* tollgateData = getTollgateDataById(tollgateGuid);
    if (tollgateData == NULL)
    {
        return LynxErrno::InvalidTollgateId;
    }

    UInt32 tollgateId = 0;
    switch (tollgateDifficulty)
    {
    case TOLLGATE_EASY:
        {
            tollgateId = tollgateData->mEasyData.mTemplateId;
        }
        break;
    case TOLLGATE_NORMAL:
        {
            tollgateId = tollgateData->mNormalData.mTemplateId;
        }
        break;
    case TOLLGATE_HARD:
        {
            tollgateId = tollgateData->mHardData.mTemplateId;
        }
        break;
    default:
        return LynxErrno::InvalidTollgateDifficulty;
    }

    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(tollgateId);
    if (tollgateTemplate == NULL)
    {
        return LynxErrno::NoTollgateIdTemplate;
    }

    if (mTollgateIdInBattle != tollgateId)
    {
        return LynxErrno::InvalidTollgateId;
    }
    // 服务器根据客户端传递的操作流程和数值进行模拟，对比客户端数据是否一致，不一致则无效
    PlayerData oppBattleData;
    genOppBattleData(oppBattleData);
    if (!mPlayer->getBattleManager().verifyBattleResult(mPlayer->mPlayerData, oppBattleData, true, battleVerifyData))
    {
        return LynxErrno::TollgateVerifyResultFailed;
    }
    return LynxErrno::None;
}

ErrorId 
TollgateManager::checkStoreHouseTollgateId(TemplateId tollgateId)
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
	
	//sec energy 注释 
    /*if (mPlayer->getPlayerEnergy() < tollgateTemplate->mNeedEnergy)
    {
        return LynxErrno::TollgateEnergyLimit;
    }*/

    if (tollgateTemplate->mBattleMapAsset.empty())
    {
        return LynxErrno::TollgateSceneNotExist;
    }

    // 检查挑战次数是否够  PlayerData.mActivityData 注释 sec
    /*if (mPlayer->mPlayerData.mActivityData.mStoreHouseCount >= mPlayer->getActivityManager().getActivityCount(Activity_Setting_StoreHouse_TemplateID))
    {
        return LynxErrno::TollgateBattleCountNull;
    }*/

    return LynxErrno::None;
}

void 
TollgateManager::startStoreHouseBattle(TemplateId tollgateId)
{
   /* mTollgateMonsterList.clear();
    StartStoreHouseBattleResp resp;
    resp.mErrorId = checkStoreHouseTollgateId(tollgateId);
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

    resp.mErrorId = genTollgateBattleData(tollgateId);
    if (resp.mErrorId == LynxErrno::None)
    {
        mTollgateIdInBattle = tollgateId;
        resp.mMonsterList = mTollgateMonsterList;
        resp.mMonsterPet = mTollgatePet;
    }

    mPlayer->sendMsg(resp);*/
}

void 
TollgateManager::storeHouseBattleResult(const StoreHouseBattleResultReq& msg)
{
    StoreHouseBattleResultResp resp;
    resp.mTollgateId = mTollgateIdInBattle;
    resp.mErrorId = checkStoreHouseResult(msg.mTollgateId, msg.mBattleVerifyData);
    if (resp.mErrorId != LynxErrno::None)
    {
        mPlayer->sendMsg(resp);
        return;
    }

    if (!msg.mBattleVerifyData.mIsVictory)
    {
        resp.mStarCount = 0;
        mPlayer->sendMsg(resp);
        return;
    }

    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(mTollgateIdInBattle);
    if (tollgateTemplate == NULL)
    {
        LOG_WARN("Get tollgate template failed tollgate id:%u", mTollgateIdInBattle);
        resp.mErrorId = LynxErrno::NoTollgateIdTemplate;
        mPlayer->sendMsg(resp);
        return;
    }

    resp.mAwardList.clear();
    if (mPlayer->getTollgateManager().getDropItemList(msg.mTollgateId, resp.mAwardList)
        && resp.mAwardList.size() != 0)
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

	//体力值的字段以后添加 sec
   /* if (mPlayer->getPlayerEnergy() < tollgateTemplate->mNeedEnergy)
    {
        LOG_WARN("Player energy is less than tollgate need energy:%u", mTollgateIdInBattle);
    }
    else
    {
        mPlayer->reducePlayerEnergy(tollgateTemplate->mNeedEnergy);
    }*/

    // 胜利才计次数
    mPlayer->getActivityManager().addStorehouseCount();

    // 增加经验看是否是升级，player和hero
    UInt32 playerLevelUp = 0;
    mPlayer->addExpForPlayer(playerLevelUp, tollgateTemplate->mTollgateAwardInfo.mTeamExp);
    mPlayer->getHeroManager().refinePresentHeros(resp.mHeroLevelUp, tollgateTemplate->mTollgateAwardInfo.mHeroExp);
    mPlayer->increasePlayerGold(tollgateTemplate->mTollgateAwardInfo.mGold);
    mPlayer->increasePlayerHeroExpPool(tollgateTemplate->mTollgateAwardInfo.mSpring);

    resp.mStarCount = (msg.mBattleVerifyData.mIsVictory == true) ? 1 : 0;
    resp.mAwardGold = tollgateTemplate->mTollgateAwardInfo.mGold;
    resp.mAwardTeamExp = tollgateTemplate->mTollgateAwardInfo.mTeamExp;
    resp.mPlayerLevelUp = playerLevelUp == 0 ? false : true;
    resp.mAwardHeroExp = tollgateTemplate->mTollgateAwardInfo.mHeroExp;
    resp.mAwardSpring = tollgateTemplate->mTollgateAwardInfo.mSpring;

    mPlayer->sendMsg(resp);
}

ErrorId 
TollgateManager::checkStoreHouseResult(TemplateId tollgateId, const BattleVerifyData& battleVerifyData)
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
    // 服务器根据客户端传递的操作流程和数值进行模拟，对比客户端数据是否一致，不一致则无效
    PlayerData oppBattleData;
    genOppBattleData(oppBattleData);
    if (!mPlayer->getBattleManager().verifyBattleResult(mPlayer->mPlayerData, oppBattleData, true, battleVerifyData))
    {
        return LynxErrno::TollgateVerifyResultFailed;
    }
    return LynxErrno::None;
}

void 
TollgateManager::resetStoreHouseBattleCount()
{
    // 重置宝藏次数 todo yangyang

    // DB调用重置关卡次数
    //PersistResetPlayerTollgateDataNotify resetNotify;
    //resetNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    //PersistSystem::getSingleton().postThreadMsg(resetNotify, mPlayer->getPlayerGuid());
}
