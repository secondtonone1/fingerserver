#include "BattleManager.h"
#include "GameServer.h"

using namespace Lynx;

BattleManager::BattleManager() : mPlayer(NULL), mLastEndTag(""), mAttackNum(0),
mRoundNum(0), mPlayerPetSkillUsed(false), mEnemyPetSkillUsed(false), mDeathCount(0),
mMySuperSkillUsed(false), mOppSuperSkillUsed(false), mPlayerUserIndex(0),
mEnemyUserIndex(0), mIsOppRobot(false), mIsSuperSkill(false), mIsCommonSkill(false),
mIsAutoSkill(false), mIsEnterSkill(false), mAttackAction(""), mAttackCount(0), mAttackMultiIndex(0),
mSimulationData(NULL), mIsVerifyBattle(false), mIsVerifyPass(false), mMyDps(0), 
mIsTower(false), mIsTowerPass(false), mTowerId(0)
{

}


BattleManager::~BattleManager()
{
    mPlayer = NULL;
}

bool 
BattleManager::initial(Player* player)
{
    if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }

    mPlayer = player;

    mEnemyBench.clear();
    mPlayerBench.clear();

    return true;
}

void 
BattleManager::update(const UInt64& accTime)
{

}

void 
BattleManager::release()
{

}

bool 
BattleManager::battleSimulation(const PlayerData& challengerData, const PlayerData& oppData, 
                                bool isOppRobot, BattleSimulationData& battleSimulationData)
{
    mIsOppRobot = isOppRobot;
    mIsVerifyBattle = false;
    mIsTower = false;
    mSimulationData = &battleSimulationData;
    mMyDps = 0;
    mIsVerifyPass = false;
    mIsTowerPass = false;

    if (!initPlayerLineup(challengerData))
    {
        LOG_WARN("Battle simulation initial player lineup failed.");
        return false;
    }
    if (!initEnemyLineup(oppData))
    {
        LOG_WARN("Battle simulation initial enemy lineup failed.");
        return false;
    }
    UInt16 seedRandom = (UInt16)RANDOM().getUInt32Random();
    GameRandom::setSeed(seedRandom);
    mSimulationData->mRandomStartSeed = seedRandom;

    BattleData* user = getUser("E3", BATTLE_FOE);
    if (user == NULL)
    {
        LOG_WARN("Battle simulation start failed.");
        return false;
    }
    mLastEndTag = user->mBaseData.mTag;
    useCommonSkill(user, user->mSkillData.mCommonSkill);
    return true;
}

bool 
BattleManager::verifyBattleResult(const PlayerData& challengerData, const PlayerData& oppData, 
                                  bool isOppRobot, const BattleVerifyData& battleVerifyData, bool isTower)
{
    //mIsVerifyPass = true;
    //mIsTowerPass = true;
    //return true;
    mIsOppRobot = isOppRobot;
    mIsVerifyBattle = true;
    mIsTower = isTower;
    mBattleVerifyData = battleVerifyData;
    mMyDps = 0;
    mIsVerifyPass = false;
    mIsTowerPass = false;

    if (!initPlayerLineup(challengerData))
    {
        LOG_WARN("Battle simulation initial player lineup failed.");
        return false;
    }
    if (!initEnemyLineup(oppData))
    {
        LOG_WARN("Battle simulation initial enemy lineup failed.");
        return false;
    }

    GameRandom::setSeed(mBattleVerifyData.mRandomStartSeed);

    BattleData* user = getUser("E3", BATTLE_FOE);
    if (user == NULL)
    {
        LOG_WARN("Battle simulation start failed.");
        return false;
    }
    mLastEndTag = user->mBaseData.mTag;
    useCommonSkill(user, user->mSkillData.mCommonSkill);

    if (!mIsVerifyPass)
    {
        return false;
    }
    return true;
}

bool 
BattleManager::initPlayerLineup(const PlayerData& playerData)
{
    //playerData.mTowerData 数据字段目前被删除 sec
	/*mTowerBufferList.clear();

    if (playerData.mBaseData.mPlayerId == 0)
    {
        return false;
    }

    if (mIsTower)
    {
        mTowerBufferList = playerData.mTowerData.mTowerBufferList;
        mTowerId = playerData.mTowerData.mCurrentTowerId;
    }

    BattleData heroBenchArray[2];
    const List<HeroData>& playerHeroList = playerData.mHeroData.mHeroDataList;
    for (List<HeroData>::Iter* iter = playerHeroList.begin();
        iter != playerHeroList.end(); iter = playerHeroList.next(iter))
    {
        if (iter->mValue.mBaseData.mHeroId == 0 ||
            iter->mValue.mBaseData.mPresentPos == 0)
        {
            continue;
        }

        BattleData heroBattleData;
        if (!genBattleDataFromHeroData(iter->mValue, heroBattleData, true))
        {
            LOG_WARN("Gen battle data from hero[%llu] data failed.", iter->mValue.mBaseData.mHeroId);
            continue;
        }

        if (iter->mValue.mBaseData.mPresentPos == 1)
        {
            mPlayerArray[0] = heroBattleData;
            mPlayerArray[0].mBaseData.mTag = "P1";
            mPlayerArray[0].mBaseData.mRelationship = BATTLE_FRIEND;
        }
        else if (iter->mValue.mBaseData.mPresentPos == 2)
        {
            mPlayerArray[1] = heroBattleData;
            mPlayerArray[1].mBaseData.mTag = "P2";
            mPlayerArray[1].mBaseData.mRelationship = BATTLE_FRIEND;
        }
        else if (iter->mValue.mBaseData.mPresentPos == 3)
        {
            mPlayerArray[2] = heroBattleData;
            mPlayerArray[2].mBaseData.mTag = "P3";
            mPlayerArray[2].mBaseData.mRelationship = BATTLE_FRIEND;
        }
        else if (iter->mValue.mBaseData.mPresentPos == 4)
        {
            heroBenchArray[0] = heroBattleData;
        }
        else if (iter->mValue.mBaseData.mPresentPos == 5)
        {
            heroBenchArray[1] = heroBattleData;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (heroBenchArray[i].mBaseData.mGuidId != 0)
        {
            mPlayerBench.insertTail(heroBenchArray[i]);
        }
    }

    const List<PetData>& playerPetList = playerData.mPetData.mPetDataList;
    for (List<PetData>::Iter* iter = playerPetList.begin();
        iter != playerPetList.end(); iter = playerPetList.next(iter))
    {
        if (iter->mValue.mBaseData.mPetId == 0 ||
            iter->mValue.mBaseData.mPresentPos == 0)
        {
            continue;
        }

        BattleData petBattleData;
        if (!genBattleDataFromPetData(iter->mValue, petBattleData))
        {
            LOG_WARN("Gen battle data from pet[%llu] data failed.", iter->mValue.mBaseData.mPetId);
            continue;
        }

        if (iter->mValue.mBaseData.mPresentPos == 1)
        {
            mPlayerPet = petBattleData;
            mPlayerPet.mBaseData.mTag = "PS";
            mPlayerPet.mBaseData.mRelationship = BATTLE_FRIEND;
            break;
        }
    }*/

    return true;
}

bool 
BattleManager::initEnemyLineup(const PlayerData& enemyData)
{
    //enemyData.mHeroData sec注释
	/*if (enemyData.mBaseData.mPlayerId == 0)
    {
        return false;
    }

    BattleData heroBenchArray[2];
    const List<HeroData>& playerHeroList = enemyData.mHeroData.mHeroDataList;
    for (List<HeroData>::Iter* iter = playerHeroList.begin();
        iter != playerHeroList.end(); iter = playerHeroList.next(iter))
    {
        if (iter->mValue.mBaseData.mHeroId == 0 ||
            iter->mValue.mBaseData.mPresentPos == 0)
        {
            continue;
        }

        BattleData heroBattleData;
        if (!genBattleDataFromHeroData(iter->mValue, heroBattleData, false))
        {
            LOG_WARN("Gen battle data from hero[%llu] data failed.", iter->mValue.mBaseData.mHeroId);
            continue;
        }

        if (iter->mValue.mBaseData.mPresentPos == 1)
        {
            mEnemyArray[0] = heroBattleData;
            mEnemyArray[0].mBaseData.mTag = "E1";
            mEnemyArray[0].mBaseData.mRelationship = BATTLE_FOE;
        }
        else if (iter->mValue.mBaseData.mPresentPos == 2)
        {
            mEnemyArray[1] = heroBattleData;
            mEnemyArray[1].mBaseData.mTag = "E2";
            mEnemyArray[1].mBaseData.mRelationship = BATTLE_FOE;
        }
        else if (iter->mValue.mBaseData.mPresentPos == 3)
        {
            mEnemyArray[2] = heroBattleData;
            mEnemyArray[2].mBaseData.mTag = "E3";
            mEnemyArray[2].mBaseData.mRelationship = BATTLE_FOE;
        }
        else if (iter->mValue.mBaseData.mPresentPos == 4)
        {
            heroBenchArray[0] = heroBattleData;
        }
        else if (iter->mValue.mBaseData.mPresentPos == 5)
        {
            heroBenchArray[1] = heroBattleData;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (heroBenchArray[i].mBaseData.mGuidId != 0)
        {
            mEnemyBench.insertTail(heroBenchArray[i]);
        }
    }

    const List<PetData>& playerPetList = enemyData.mPetData.mPetDataList;
    for (List<PetData>::Iter* iter = playerPetList.begin();
        iter != playerPetList.end(); iter = playerPetList.next(iter))
    {
        if (iter->mValue.mBaseData.mPetId == 0 ||
            iter->mValue.mBaseData.mPresentPos == 0)
        {
            continue;
        }

        BattleData petBattleData;
        if (!genBattleDataFromPetData(iter->mValue, petBattleData))
        {
            LOG_WARN("Gen battle data from pet[%llu] data failed.", iter->mValue.mBaseData.mPetId);
            continue;
        }

        if (iter->mValue.mBaseData.mPresentPos == 1)
        {
            mEnemyPet = petBattleData;
            mEnemyPet.mBaseData.mTag = "ES";
            mEnemyPet.mBaseData.mRelationship = BATTLE_FOE;
            break;
        }
    }*/
    return true;
}

bool 
BattleManager::genBattleDataFromHeroData(const HeroData& heroData, BattleData& battleData, bool isPlayer)
{
    //战斗数据生成 sec注释
	//if (heroData.mBaseData.mHeroId == 0
    //    || heroData.mBaseData.mTemplateId == 0)
    //{
    //    return false;
    //}

    //battleData.mBaseData.mGuidId = heroData.mBaseData.mHeroId;
    //battleData.mBaseData.mTemplateId = heroData.mBaseData.mTemplateId;
    //battleData.mBaseData.mTag = "";
    //battleData.mBaseData.mCharacterType = BATTLE_HERO;
    //battleData.mBaseData.mRelationship = BATTLE_FRIEND;
    //battleData.mBaseData.mHp = heroData.mExtendData.mHpUpper;
    //battleData.mBaseData.mAnger = 0;
    //battleData.mBaseData.mParalysisId = 0;
    //battleData.mBaseData.mBlindId = 0;
    //battleData.mBaseData.mConvergedId = 0;
    //battleData.mBaseData.mSilentId = 0;
    //battleData.mBaseData.mSealId = 0;
    //battleData.mBaseData.mPhyInvincibleId = 0;
    //battleData.mBaseData.mMagInvincibleId = 0;

    //battleData.mExtendData.mHpUpper = heroData.mExtendData.mHpUpper;
    //battleData.mExtendData.mAngerUpper = heroData.mExtendData.mAngerUpper;
    //battleData.mExtendData.mAttack = heroData.mExtendData.mAttack;
    //battleData.mExtendData.mPhysicsDefense = heroData.mExtendData.mPhysicsDefense;
    //battleData.mExtendData.mMagicDefense = heroData.mExtendData.mMagicDefense;
    //battleData.mExtendData.mCritical = heroData.mExtendData.mCritical;
    //battleData.mExtendData.mRescrit = heroData.mExtendData.mRescrit;
    //battleData.mExtendData.mHit = heroData.mExtendData.mHit;
    //battleData.mExtendData.mDodge = heroData.mExtendData.mDodge;
    //battleData.mExtendData.mSuck = heroData.mExtendData.mSuck;
    //battleData.mExtendData.mAvoidDamage = heroData.mExtendData.mAvoidDamage;
    //battleData.mExtendData.mCure = heroData.mExtendData.mCure;
    //battleData.mExtendData.mCriticalDamage = heroData.mExtendData.mCriticalDamage;

    //String actorAssets = "";
    //if (!isPlayer && mIsOppRobot)
    //{
    //    MonsterTemplate* monsterTemplate = MONSTER_TABLE().get(heroData.mBaseData.mTemplateId);
    //    if (monsterTemplate == NULL)
    //    {
    //        LOG_WARN("Get robot monster template [%u] failed.", heroData.mBaseData.mTemplateId);
    //        return false;
    //    }
    //    battleData.mSkillData.mPassiveSkill1 = monsterTemplate->mBuffer1;
    //    battleData.mSkillData.mPassiveSkill2 = monsterTemplate->mBuffer2;
    //    battleData.mSkillData.mPassiveSkill3 = monsterTemplate->mBuffer3;
    //    actorAssets = monsterTemplate->mModelAsset;
    //}
    //else
    //{
    //    HeroTemplate* heroTemplate = HERO_TABLE().get(heroData.mBaseData.mTemplateId);
    //    if (heroTemplate == NULL)
    //    {
    //        LOG_WARN("Get hero template [%u] failed.", heroData.mBaseData.mTemplateId);
    //        return false;
    //    }
    //    battleData.mSkillData.mPassiveSkill1 = heroTemplate->mSkill.mBuff1;
    //    battleData.mSkillData.mPassiveSkill2 = heroTemplate->mSkill.mBuff2;
    //    battleData.mSkillData.mPassiveSkill3 = heroTemplate->mSkill.mBuff3;
    //    actorAssets = heroTemplate->mResourceName;
    //}

    //lynxToLowerCase(actorAssets);
    //ActorSkillInfoMap::Iter* actorIter = LogicSystem::getSingleton().getActorSkillInfoFile().mActorSkillInfoMap.find(actorAssets);
    //if (actorIter == NULL)
    //{
    //    LOG_WARN("Get actor [%s] skill info failed.", actorAssets.c_str());
    //    return false;
    //}

    //if (heroData.mSkillData.mCommonSkill.mTemplateId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(heroData.mSkillData.mCommonSkill.mTemplateId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skill template [%u] failed.", heroData.mSkillData.mCommonSkill.mTemplateId);
    //        return false;
    //    }
    //    battleData.mSkillData.mCommonSkill.mTemplateId = heroData.mSkillData.mCommonSkill.mTemplateId;
    //    battleData.mSkillData.mCommonSkill.mSkillAttackType = actorIter->mValue.mAttack.mAttackType;
    //    battleData.mSkillData.mCommonSkill.mCombatType = actorIter->mValue.mAttack.mCombatType;
    //    battleData.mSkillData.mCommonSkill.mSkillRelationship = actorIter->mValue.mAttack.mRelationship;
    //    battleData.mSkillData.mCommonSkill.mMultiAttack = actorIter->mValue.mAttack.mMultiAttack;
    //    battleData.mSkillData.mCommonSkill.mMultiNum = actorIter->mValue.mAttack.mMultiNum;
    //    battleData.mSkillData.mCommonSkill.mSkillAttackType = skillTemplate->mAttackType;
    //    battleData.mSkillData.mCommonSkill.mCombatType = skillTemplate->mCombatType;
    //    battleData.mSkillData.mCommonSkill.mSkillRelationship = skillTemplate->mRelationship;
    //    battleData.mSkillData.mCommonSkill.mMultiAttack = skillTemplate->mMultiAttack;
    //    battleData.mSkillData.mCommonSkill.mMultiNum = skillTemplate->mMultiNum;
    //    battleData.mSkillData.mCommonSkill.mTriggerAnger = 0;
    //    battleData.mSkillData.mCommonSkill.mAngerInc = skillTemplate->mIncAnger;
    //    battleData.mSkillData.mCommonSkill.mCoolDownRound = 0;
    //    battleData.mSkillData.mCommonSkill.mHitPointCount = 0;
    //}

    //if (heroData.mSkillData.mAutoSkill.mTemplateId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(heroData.mSkillData.mAutoSkill.mTemplateId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skill template [%u] failed.", heroData.mSkillData.mAutoSkill.mTemplateId);
    //        return false;
    //    }
    //    battleData.mSkillData.mAutoSkill.mTemplateId = heroData.mSkillData.mAutoSkill.mTemplateId;
    //    battleData.mSkillData.mAutoSkill.mSkillAttackType = actorIter->mValue.mAuto.mAttackType;
    //    battleData.mSkillData.mAutoSkill.mCombatType = actorIter->mValue.mAuto.mCombatType;
    //    battleData.mSkillData.mAutoSkill.mSkillRelationship = actorIter->mValue.mAuto.mRelationship;
    //    battleData.mSkillData.mAutoSkill.mMultiAttack = actorIter->mValue.mAuto.mMultiAttack;
    //    battleData.mSkillData.mAutoSkill.mMultiNum = actorIter->mValue.mAuto.mMultiNum;
    //    //battleData.mSkillData.mAutoSkill.mSkillAttackType = skillTemplate->mAttackType;
    //    //battleData.mSkillData.mAutoSkill.mCombatType = skillTemplate->mCombatType;
    //    //battleData.mSkillData.mAutoSkill.mSkillRelationship = skillTemplate->mRelationship;
    //    //battleData.mSkillData.mAutoSkill.mMultiAttack = skillTemplate->mMultiAttack;
    //    //battleData.mSkillData.mAutoSkill.mMultiNum = skillTemplate->mMultiNum;
    //    battleData.mSkillData.mAutoSkill.mTriggerAnger = skillTemplate->mTriggerAnger;
    //    battleData.mSkillData.mAutoSkill.mAngerInc = 0;
    //    battleData.mSkillData.mAutoSkill.mCoolDownRound = 0;
    //    battleData.mSkillData.mAutoSkill.mHitPointCount = 0;
    //}

    //if (heroData.mSkillData.mEnterSkill.mTemplateId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(heroData.mSkillData.mEnterSkill.mTemplateId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skill template [%u] failed.", heroData.mSkillData.mEnterSkill.mTemplateId);
    //        return false;
    //    }
    //    battleData.mSkillData.mEnterSkill.mTemplateId = heroData.mSkillData.mEnterSkill.mTemplateId;
    //    battleData.mSkillData.mEnterSkill.mSkillAttackType = actorIter->mValue.mAppearance.mAttackType;
    //    battleData.mSkillData.mEnterSkill.mCombatType = actorIter->mValue.mAppearance.mCombatType;
    //    battleData.mSkillData.mEnterSkill.mSkillRelationship = actorIter->mValue.mAppearance.mRelationship;
    //    battleData.mSkillData.mEnterSkill.mMultiAttack = actorIter->mValue.mAppearance.mMultiAttack;
    //    battleData.mSkillData.mEnterSkill.mMultiNum = actorIter->mValue.mAppearance.mMultiNum;
    //    //battleData.mSkillData.mEnterSkill.mSkillAttackType = skillTemplate->mAttackType;
    //    //battleData.mSkillData.mEnterSkill.mCombatType = skillTemplate->mCombatType;
    //    //battleData.mSkillData.mEnterSkill.mSkillRelationship = skillTemplate->mRelationship;
    //    //battleData.mSkillData.mEnterSkill.mMultiAttack = skillTemplate->mMultiAttack;
    //    //battleData.mSkillData.mEnterSkill.mMultiNum = skillTemplate->mMultiNum;
    //    battleData.mSkillData.mEnterSkill.mTriggerAnger = 0;
    //    battleData.mSkillData.mEnterSkill.mAngerInc = 0;
    //    battleData.mSkillData.mEnterSkill.mCoolDownRound = 0;
    //    battleData.mSkillData.mEnterSkill.mHitPointCount = 0;
    //}

    //if (heroData.mSkillData.mSuperSkill.mTemplateId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(heroData.mSkillData.mSuperSkill.mTemplateId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skill template [%u] failed.", heroData.mSkillData.mSuperSkill.mTemplateId);
    //        return false;
    //    }
    //    battleData.mSkillData.mSuperSkill.mTemplateId = heroData.mSkillData.mSuperSkill.mTemplateId;
    //    battleData.mSkillData.mSuperSkill.mSkillAttackType = actorIter->mValue.mSuper.mAttackType;
    //    battleData.mSkillData.mSuperSkill.mCombatType = actorIter->mValue.mSuper.mCombatType;
    //    battleData.mSkillData.mSuperSkill.mSkillRelationship = actorIter->mValue.mSuper.mRelationship;
    //    battleData.mSkillData.mSuperSkill.mMultiAttack = actorIter->mValue.mSuper.mMultiAttack;
    //    battleData.mSkillData.mSuperSkill.mMultiNum = actorIter->mValue.mSuper.mMultiNum;
    //    //battleData.mSkillData.mSuperSkill.mSkillAttackType = skillTemplate->mAttackType;
    //    //battleData.mSkillData.mSuperSkill.mCombatType = skillTemplate->mCombatType;
    //    //battleData.mSkillData.mSuperSkill.mSkillRelationship = skillTemplate->mRelationship;
    //    //battleData.mSkillData.mSuperSkill.mMultiAttack = skillTemplate->mMultiAttack;
    //    //battleData.mSkillData.mSuperSkill.mMultiNum = skillTemplate->mMultiNum;
    //    battleData.mSkillData.mSuperSkill.mTriggerAnger = 0;
    //    battleData.mSkillData.mSuperSkill.mAngerInc = 0;
    //    battleData.mSkillData.mSuperSkill.mCoolDownRound = 0;
    //    battleData.mSkillData.mSuperSkill.mHitPointCount = 0;
    //}

    //battleData.mSkillData.mActionHitMap = actorIter->mValue.mActionHitMap;

    //initBattleBufferData(battleData);

    return true;
}

//bool 
//BattleManager::genBattleDataFromPetData(const PetData& petData, BattleData& battleData)
//{
//    if (petData.mBaseData.mPetId == 0
//        || petData.mBaseData.mTemplateId == 0)
//    {
//        return false;
//    }
//
//    battleData.mBaseData.mGuidId = petData.mBaseData.mPetId;
//    battleData.mBaseData.mTemplateId = petData.mBaseData.mTemplateId;
//    battleData.mBaseData.mTag = "";
//    battleData.mBaseData.mCharacterType = BATTLE_PET;
//    battleData.mBaseData.mRelationship = BATTLE_FRIEND;
//    battleData.mBaseData.mHp = 0;
//    battleData.mBaseData.mAnger = 0;
//    battleData.mBaseData.mParalysisId = 0;
//    battleData.mBaseData.mBlindId = 0;
//    battleData.mBaseData.mConvergedId = 0;
//    battleData.mBaseData.mSilentId = 0;
//    battleData.mBaseData.mSealId = 0;
//    battleData.mBaseData.mPhyInvincibleId = 0;
//    battleData.mBaseData.mMagInvincibleId = 0;
//
//    battleData.mExtendData.mHpUpper = 0;
//    battleData.mExtendData.mAngerUpper = 0;
//    battleData.mExtendData.mAttack = petData.mTalentData.mAttack;
//    battleData.mExtendData.mPhysicsDefense = petData.mTalentData.mPhysicsDefense;
//    battleData.mExtendData.mMagicDefense = petData.mTalentData.mMagicDefense;
//    battleData.mExtendData.mCritical = 0;
//    battleData.mExtendData.mRescrit = 0;
//    battleData.mExtendData.mHit = 0;
//    battleData.mExtendData.mDodge = 0;
//    battleData.mExtendData.mSuck = 0;
//    battleData.mExtendData.mAvoidDamage = 0;
//    battleData.mExtendData.mCure = 0;
//    battleData.mExtendData.mCriticalDamage = 0;
//
//    PetTemplate* petTemplate = PET_TABLE().get(petData.mBaseData.mTemplateId);
//    if (petTemplate == NULL)
//    {
//        LOG_WARN("Get pet template [%u] failed.", petData.mBaseData.mTemplateId);
//        return false;
//    }
//    battleData.mSkillData.mPassiveSkill1 = petTemplate->mBuffId1;
//    battleData.mSkillData.mPassiveSkill2 = petTemplate->mBuffId2;
//    battleData.mSkillData.mPassiveSkill3 = petTemplate->mBuffId3;
//
//    String actorAssets = petTemplate->mResourceName;
//    lynxToLowerCase(actorAssets);
//    ActorSkillInfoMap::Iter* actorIter = LogicSystem::getSingleton().getActorSkillInfoFile().mActorSkillInfoMap.find(actorAssets);
//    if (actorIter == NULL)
//    {
//        LOG_WARN("Get actor [%s] skill info failed.", actorAssets.c_str());
//        return false;
//    }
//
//    if (petData.mSkillData.mCommonSkill.mTemplateId != 0)
//    {
//        SkillTemplate* skillTemplate = SKILL_TABLE().get(petData.mSkillData.mCommonSkill.mTemplateId);
//        if (skillTemplate == NULL)
//        {
//            LOG_WARN("Get skill template [%u] failed.", petData.mSkillData.mCommonSkill.mTemplateId);
//            return false;
//        }
//        battleData.mSkillData.mCommonSkill.mTemplateId = petData.mSkillData.mCommonSkill.mTemplateId;
//        battleData.mSkillData.mCommonSkill.mSkillAttackType = actorIter->mValue.mAttack.mAttackType;
//        battleData.mSkillData.mCommonSkill.mCombatType = actorIter->mValue.mAttack.mCombatType;
//        battleData.mSkillData.mCommonSkill.mSkillRelationship = actorIter->mValue.mAttack.mRelationship;
//        battleData.mSkillData.mCommonSkill.mMultiAttack = actorIter->mValue.mAttack.mMultiAttack;
//        battleData.mSkillData.mCommonSkill.mMultiNum = actorIter->mValue.mAttack.mMultiNum;
//        //battleData.mSkillData.mCommonSkill.mSkillAttackType = skillTemplate->mAttackType;
//        //battleData.mSkillData.mCommonSkill.mCombatType = skillTemplate->mCombatType;
//        //battleData.mSkillData.mCommonSkill.mSkillRelationship = skillTemplate->mRelationship;
//        //battleData.mSkillData.mCommonSkill.mMultiAttack = skillTemplate->mMultiAttack;
//        //battleData.mSkillData.mCommonSkill.mMultiNum = skillTemplate->mMultiNum;
//        battleData.mSkillData.mCommonSkill.mTriggerAnger = 0;
//        battleData.mSkillData.mCommonSkill.mAngerInc = 0;
//        battleData.mSkillData.mCommonSkill.mCoolDownRound = skillTemplate->mCoolDownRound;
//        battleData.mSkillData.mCommonSkill.mHitPointCount = 0;
//    }
//
//    battleData.mSkillData.mActionHitMap = actorIter->mValue.mActionHitMap;
//
//    initBattleBufferData(battleData);
//
//    return true;
//}

void 
BattleManager::initBattleBufferData(BattleData& battleData)
{
    battleData.mBufferData.mHpUpper = battleData.mExtendData.mHpUpper;
    battleData.mBufferData.mAttack = battleData.mExtendData.mAttack;
    battleData.mBufferData.mPhysicsDefense = battleData.mExtendData.mPhysicsDefense;
    battleData.mBufferData.mMagicDefense = battleData.mExtendData.mMagicDefense;
    battleData.mBufferData.mCritical = battleData.mExtendData.mCritical;
    battleData.mBufferData.mRescrit = battleData.mExtendData.mRescrit;
    battleData.mBufferData.mHit = battleData.mExtendData.mHit;
    battleData.mBufferData.mDodge = battleData.mExtendData.mDodge;
    battleData.mBufferData.mSuck = battleData.mExtendData.mSuck;
    battleData.mBufferData.mAvoidDamage = battleData.mExtendData.mAvoidDamage;
    battleData.mBufferData.mCure = battleData.mExtendData.mCure;
    battleData.mBufferData.mCriticalDamage = battleData.mExtendData.mCriticalDamage;

    memset(battleData.mBufferData.mPassivityId, 0, sizeof(UInt32) * 3);

    int index = 0;
    if (battleData.mSkillData.mPassiveSkill1 != 0)
    {
        BufferTemplate* bufferTemplate = BUFFER_TABLE().get(battleData.mSkillData.mPassiveSkill1);
        if (bufferTemplate == NULL)
        {
            LOG_WARN("Get buffer template[%u] failed.", battleData.mSkillData.mPassiveSkill1);
        }
        else
        {
            battleData.mBufferData.mPassivityId[index] = battleData.mSkillData.mPassiveSkill1;
            index++;
        }
    }

    if (battleData.mSkillData.mPassiveSkill2 != 0)
    {
        BufferTemplate* bufferTemplate = BUFFER_TABLE().get(battleData.mSkillData.mPassiveSkill2);
        if (bufferTemplate == NULL)
        {
            LOG_WARN("Get buffer template[%u] failed.", battleData.mSkillData.mPassiveSkill2);
        }
        else
        {
            battleData.mBufferData.mPassivityId[index] = battleData.mSkillData.mPassiveSkill2;
            index++;
        }
    }

    if (battleData.mSkillData.mPassiveSkill3 != 0)
    {
        BufferTemplate* bufferTemplate = BUFFER_TABLE().get(battleData.mSkillData.mPassiveSkill3);
        if (bufferTemplate == NULL)
        {
            LOG_WARN("Get buffer template[%u] failed.", battleData.mSkillData.mPassiveSkill3);
        }
        else
        {
            battleData.mBufferData.mPassivityId[index] = battleData.mSkillData.mPassiveSkill3;
            index++;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (battleData.mBufferData.mPassivityId[i] != 0)
        {
            updateProperty(battleData.mBufferData.mPassivityId[i], battleData);
        }
    }

    initBattleTowerBuffer(battleData);

    battleData.mExtendData.mHpUpper = battleData.mBufferData.mHpUpper;
}

void 
BattleManager::initBattleTowerBuffer(BattleData& battleData)
{
    if (!mIsTower)
    {
        return;
    }

    if (battleData.mBaseData.mTag != "P1" 
        && battleData.mBaseData.mTag != "P2" 
        && battleData.mBaseData.mTag != "P3")
    {
        return;
    }

    if (mTowerBufferList.size() == 0)
    {
        return;
    }

    for (List<TowerBufferData>::Iter* iter = mTowerBufferList.begin();
        iter != mTowerBufferList.end(); iter = mTowerBufferList.next(iter))
    {
        switch (iter->mValue.mBufferType)
        {
        case TOWER_BUFFER_HP:
            {
                UInt32 bufferValue = (UInt32)((float)battleData.mBufferData.mHpUpper * (iter->mValue.mBufferValue / 100.0f));
                battleData.mBufferData.mHpUpper += bufferValue;
            }
        	break;
        case TOWER_BUFFER_ATTACK:
            {
                UInt32 bufferValue = (UInt32)((float)battleData.mBufferData.mAttack * (iter->mValue.mBufferValue / 100.0f));
                battleData.mBufferData.mAttack += bufferValue;
            }
            break;
        case TOWER_BUFFER_CRITICAL:
            {
                UInt32 bufferValue = (UInt32)((float)battleData.mBufferData.mCritical * (iter->mValue.mBufferValue / 100.0f));
                battleData.mBufferData.mCritical += bufferValue;
            }
            break;
        case TOWER_BUFFER_RESCRIT:
            {
                UInt32 bufferValue = (UInt32)((float)battleData.mBufferData.mRescrit * (iter->mValue.mBufferValue / 100.0f));
                battleData.mBufferData.mRescrit += bufferValue;
            }
            break;
        case TOWER_BUFFER_HIT:
            {
                UInt32 bufferValue = (UInt32)((float)battleData.mBufferData.mHit * (iter->mValue.mBufferValue / 100.0f));
                battleData.mBufferData.mHit += bufferValue;
            }
            break;
        case TOWER_BUFFER_DODGE:
            {
                UInt32 bufferValue = (UInt32)((float)battleData.mBufferData.mDodge * (iter->mValue.mBufferValue / 100.0f));
                battleData.mBufferData.mDodge += bufferValue;
            }
            break;
        case TOWER_BUFFER_DAMAGE_RED:
            {
                UInt32 bufferValue = (UInt32)((float)battleData.mBufferData.mAvoidDamage * (iter->mValue.mBufferValue / 100.0f));
                battleData.mBufferData.mAvoidDamage += bufferValue;
            }
            break;
        default:
            LOG_WARN("Unknown tower buffer type %u", iter->mValue.mBufferType);
            break;
        }
    }
}

bool 
BattleManager::checkExistEffectType(UInt8 effectType, const Map<UInt32, BufferData>& bufferMap)
{
   //sec  原来buff注释
	/* for (Map<UInt32, BufferData>::Iter* iter = bufferMap.begin();
        iter != bufferMap.end(); iter = bufferMap.next(iter))
    {
        if (iter->mKey != 0 && iter->mValue.mActionCount != 0
            && iter->mValue.mEffectType == effectType)
        {
            return true;
        }
    }*/
    return false;
}

void 
BattleManager::updateBufferActionCount(BattleData& battleData)
{
	//sec 原来buff注释   
	//Map<UInt32, BufferData>& bufferMap = battleData.mBufferData.mBufferMap;
    //List<UInt32> delList;
    //for (Map<UInt32, BufferData>::Iter* iter = bufferMap.begin();
    //    iter != bufferMap.end(); iter = bufferMap.next(iter))
    //{
    //    if (iter->mKey != 0)
    //    {
    //        if (iter->mValue.mActionCount != 0)
    //        {
    //            if (iter->mValue.mNextBegin)
    //            {
    //                iter->mValue.mNextBegin = false;
    //                updateProperty(iter->mValue.mBufferId, battleData);
    //            }
    //            else
    //            {
    //                iter->mValue.mActionCount--;
    //                if (iter->mValue.mActionCount == 0)
    //                {
    //                    delList.insertTail(iter->mKey);
    //                }
    //            }
    //        }
    //        else
    //        {
    //            // 删除立即生效的buff
    //            delList.insertTail(iter->mKey);
    //        }
    //    }
    //}

    //for (List<UInt32>::Iter* i = delList.begin();
    //    i != delList.end(); i = delList.next(i))
    //{
    //    delBuffer(i->mValue, battleData);
    //}
}

// 主动清除立即生效的buffer
void 
BattleManager::delImmediatelyBuffer(BattleData& battleData)
{
    //sec 原来buff注释
	/*Map<UInt32, BufferData>& bufferMap = battleData.mBufferData.mBufferMap;
    List<UInt32> delList;
    for (Map<UInt32, BufferData>::Iter* iter = bufferMap.begin();
        iter != bufferMap.end(); iter = bufferMap.next(iter))
    {
        if (iter->mKey != 0 && iter->mValue.mActionCount == 0)
        {
            delList.insertTail(iter->mKey);
        }
    }
    for (List<UInt32>::Iter* i = delList.begin();
        i != delList.end(); i = delList.next(i))
    {
        delBuffer(i->mValue, battleData);
    }*/
}

// 主动更新下回合生效的buffer，在施法者回合结束时
void 
BattleManager::updateNextBeginBuffer(BattleData& battleData)
{
	//下一个开始的buff，sec注释   
	/*Map<UInt32, BufferData>& bufferMap = battleData.mBufferData.mBufferMap;
    for (Map<UInt32, BufferData>::Iter* iter = bufferMap.begin();
        iter != bufferMap.end(); iter = bufferMap.next(iter))
    {
        if (iter->mKey != 0 && iter->mValue.mActionCount != 0)
        {
            if (iter->mValue.mNextBegin)
            {
                iter->mValue.mNextBegin = false;
                updateProperty(iter->mValue.mBufferId, battleData);
            }
        }
    }*/
}

void 
BattleManager::getBufferListByPriority(List<BufferData*>& bufferList, BattleData& battleData)
{
    //获取buffer列表通过优先级，原来的，sec注释
	//Map<UInt32, BufferData>& bufferMap = battleData.mBufferData.mBufferMap;
    //// 通过优先级排序
    //for (int i = 10; i >= 1; i--)
    //{
    //    for (Map<UInt32, BufferData>::Iter* iter = bufferMap.begin();
    //        iter != bufferMap.end(); iter = bufferMap.next(iter))
    //    {
    //        if (iter->mKey != 0 && iter->mValue.mPriority == i)
    //        {
    //            bufferList.insertTail(&iter->mValue);
    //        }
    //    }
    //}
}

BufferData* 
BattleManager::getBufferById(UInt32 bufferId, const BattleData& battleData)
{
    Map<UInt32, BufferData>::Iter* iter = battleData.mBufferData.mBufferMap.find(bufferId);
    if (iter != NULL)
    {
        return &iter->mValue;
    }
    return NULL;
}

void 
BattleManager::addBuffer(BattleData& selfData, UInt32 bufferId, BattleData& skillUser, bool nextBegin)
{
	//原来buffer注释
    //if (bufferId == 0)
    //{
    //    return;
    //}

    //// 如果是宠物不会被施加buffer
    //if (selfData.mBaseData.mCharacterType == BATTLE_PET)
    //{
    //    return;
    //}

    //BufferTemplate* bufferTemplate = BUFFER_TABLE().get(bufferId);
    //if (bufferTemplate == NULL)
    //{
    //    LOG_WARN("Add buffer get buffer template[%u] failed.", bufferId);
    //    return;
    //}

    //Map<UInt32, BufferData>::Iter* iter = selfData.mBufferData.mBufferMap.find(bufferId);
    //if (iter != NULL)
    //{
    //    if (bufferTemplate->mOverlayCount != 0)
    //    {
    //        // 可叠加
    //        if (updateProperty(bufferId, selfData))
    //        {
    //            iter->mValue.mOverlayCount++;
    //        }
    //    }
    //    // 重置回合数
    //    iter->mValue.mActionCount = bufferTemplate->mLastActionCount;
    //}
    //else
    //{
    //    switch (bufferTemplate->mEffectType)
    //    {
    //    case BUFFER_EFFECT_BLEED:
    //        {
    //            if (checkExistEffectType(BUFFER_EFFECT_PHY_INVINCIBLE, selfData.mBufferData.mBufferMap))
    //            {
    //                //LOG_DEBUG("Have phy invincible buffer can't add.");
    //                return;
    //            }
    //        }
    //        break;
    //    case BUFFER_EFFECT_BURN:
    //    case BUFFER_EFFECT_POISONING:
    //        {
    //            if (checkExistEffectType(BUFFER_EFFECT_MAG_INVINCIBLE, selfData.mBufferData.mBufferMap))
    //            {
    //                //LOG_DEBUG("Have mag invincible buffer can't add.");
    //                return;
    //            }
    //        }
    //        break;
    //    case BUFFER_EFFECT_PARALYSIS:
    //        {
    //            if (checkExistEffectType(BUFFER_EFFECT_MAG_INVINCIBLE, selfData.mBufferData.mBufferMap))
    //            {
    //                //LOG_DEBUG("Have mag invincible buffer can't add.");
    //                return;
    //            }
    //            selfData.mBaseData.mParalysisId = bufferId;
    //        }
    //        break;
    //    case BUFFER_EFFECT_BLIND:
    //        {
    //            if (checkExistEffectType(BUFFER_EFFECT_MAG_INVINCIBLE, selfData.mBufferData.mBufferMap))
    //            {
    //                //LOG_DEBUG("Have mag invincible buffer can't add.");
    //                return;
    //            }
    //            selfData.mBaseData.mBlindId = bufferId;
    //        }
    //        break;
    //    case BUFFER_EFFECT_CONVERGED:
    //        {
    //            if (checkExistEffectType(BUFFER_EFFECT_MAG_INVINCIBLE, selfData.mBufferData.mBufferMap))
    //            {
    //                //LOG_DEBUG("Have mag invincible buffer can't add.");
    //                return;
    //            }
    //            selfData.mBaseData.mConvergedId = bufferId;
    //        }
    //        break;
    //    case BUFFER_EFFECT_SILENT:
    //        {
    //            if (checkExistEffectType(BUFFER_EFFECT_MAG_INVINCIBLE, selfData.mBufferData.mBufferMap))
    //            {
    //                //LOG_DEBUG("Have mag invincible buffer can't add.");
    //                return;
    //            }
    //            selfData.mBaseData.mSilentId = bufferId;
    //        }
    //        break;
    //    case BUFFER_EFFECT_SEAL:
    //        {
    //            if (checkExistEffectType(BUFFER_EFFECT_MAG_INVINCIBLE, selfData.mBufferData.mBufferMap))
    //            {
    //                //LOG_DEBUG("Have mag invincible buffer can't add.");
    //                return;
    //            }
    //            selfData.mBaseData.mSealId = bufferId;
    //        }
    //        break;
    //    case BUFFER_EFFECT_PHY_INVINCIBLE:
    //        {
    //            selfData.mBaseData.mPhyInvincibleId = bufferId;
    //        }
    //        break;
    //    case BUFFER_EFFECT_MAG_INVINCIBLE:
    //        {
    //            selfData.mBaseData.mMagInvincibleId = bufferId;
    //        }
    //        break;
    //    case BUFFER_EFFECT_CURE:
    //    case BUFFER_EFFECT_ANGER_RED:
    //    case BUFFER_EFFECT_ANGER_INC:
    //    case BUFFER_EFFECT_CLEAN:
    //    case BUFFER_EFFECT_REBOUND:
    //    case BUFFER_EFFECT_SELF_CURE:
    //        break;
    //    default:
    //        LOG_WARN("Unknown buffer effect type[%u].", bufferTemplate->mEffectType);
    //        return;
    //    }

    //    BufferData bufferData;
    //    bufferData.mBufferId = bufferId;
    //    bufferData.mActionCount = bufferTemplate->mLastActionCount;
    //    bufferData.mOverlayCount = 0;
    //    bufferData.mEffectType = bufferTemplate->mEffectType;
    //    bufferData.mEffectValue = getEffectValue(bufferData.mBufferId, skillUser);
    //    bufferData.mEffectUseName = "";
    //    //bufferData.mEffectUseName = mCharacter.AddBufferEffect(bufferData.mBufferId);
    //    bufferData.mPriority = bufferTemplate->mPriority;
    //    bufferData.mNextBegin = nextBegin;
    //    selfData.mBufferData.mBufferMap.insert(bufferData.mBufferId, bufferData);
    //    // 如果不是立即生效的buff则添加后不更新相应属性
    //    if (!bufferData.mNextBegin)
    //    {
    //        updateProperty(bufferData.mBufferId, selfData);
    //    }
    //}
}

void 
BattleManager::delBuffer(UInt32 bufferId, BattleData& battleData)
{
    //原游戏删除buffer的函数，sec注释
	//Map<UInt32, BufferData>::Iter* iter = battleData.mBufferData.mBufferMap.find(bufferId);
    //if (iter == NULL)
    //{
    //    return;
    //}

    //if (!revertProperty(battleData, iter->mValue))
    //{
    //    LOG_WARN("Del buffer revert property failed, bufferId[%u].", bufferId);
    //}

    //switch (iter->mValue.mEffectType)
    //{
    //case BUFFER_EFFECT_PARALYSIS:
    //    battleData.mBaseData.mParalysisId = 0;
    //    break;
    //case BUFFER_EFFECT_BLIND:
    //    battleData.mBaseData.mBlindId = 0;
    //    break;
    //case BUFFER_EFFECT_CONVERGED:
    //    battleData.mBaseData.mConvergedId = 0;
    //    break;
    //case BUFFER_EFFECT_SILENT:
    //    battleData.mBaseData.mSilentId = 0;
    //    break;
    //case BUFFER_EFFECT_SEAL:
    //    battleData.mBaseData.mSealId = 0;
    //    break;
    //case BUFFER_EFFECT_PHY_INVINCIBLE:
    //    battleData.mBaseData.mPhyInvincibleId = 0;
    //    break;
    //case BUFFER_EFFECT_MAG_INVINCIBLE:
    //    battleData.mBaseData.mMagInvincibleId = 0;
    //    break;
    //case BUFFER_EFFECT_BLEED:
    //case BUFFER_EFFECT_BURN:
    //case BUFFER_EFFECT_POISONING:
    //case BUFFER_EFFECT_CURE:
    //case BUFFER_EFFECT_ANGER_RED:
    //case BUFFER_EFFECT_ANGER_INC:
    //case BUFFER_EFFECT_CLEAN:
    //case BUFFER_EFFECT_REBOUND:
    //case BUFFER_EFFECT_SELF_CURE:
    //    break;
    //default:
    //    LOG_WARN("Unknown buffer effect type[%u].", iter->mValue.mEffectType);
    //    break;
    //}

    ////mCharacter.DelBufferEffect(bufferData.mEffectUseName);
    //battleData.mBufferData.mBufferMap.erase(bufferId);
}

void 
BattleManager::delAllBuffer(BattleData& battleData)
{
    List<UInt32> delList;
    Map<UInt32, BufferData>& bufferMap = battleData.mBufferData.mBufferMap;
    for (Map<UInt32, BufferData>::Iter* iter = bufferMap.begin();
        iter != bufferMap.end(); iter = bufferMap.next(iter))
    {
        if (iter->mKey != 0)
        {
            delList.insertTail(iter->mKey);
        }
    }

    for (List<UInt32>::Iter* i = delList.begin();
        i != delList.end(); i = delList.next(i))
    {
        delBuffer(i->mValue, battleData);
    }

    battleData.mBaseData.mParalysisId = 0;
    battleData.mBaseData.mBlindId = 0;
    battleData.mBaseData.mConvergedId = 0;
    battleData.mBaseData.mSilentId = 0;
    battleData.mBaseData.mSealId = 0;
    battleData.mBaseData.mPhyInvincibleId = 0;
    battleData.mBaseData.mMagInvincibleId = 0;
}

bool 
BattleManager::updateBufferBeforeAttack(BattleData& battleData)
{
    //原游戏更新攻击前的buffer sec
	//List<BufferData*> bufferList;
    //getBufferListByPriority(bufferList, battleData);
    //if (bufferList.size() == 0)
    //{
    //    return true;
    //}
    ////List<BufferData> cleanList;

    //for (List<BufferData*>::Iter* iter = bufferList.begin();
    //    iter != bufferList.end(); iter = bufferList.next(iter))
    //{
    //    if (iter->mValue == NULL && iter->mValue->mActionCount == 0)
    //    {
    //        continue;
    //    }

    //    //LynxHUDText hudText = mCharacter.gameObject.GetComponent<LynxHUDText>();
    //    //if (hudText == null)
    //    //{
    //    //    Debug.LogWarning("UpdateBufferBeforeAttack get self LynxHUDText failed.");
    //    //    return false;
    //    //}

    //    //BufferTemplate bufferTemplate = BufferTable.GetSingleton().Get(bufferId);
    //    //if (bufferTemplate == null)
    //    //{
    //    //    Debug.LogWarning("Get effect value get buffer template failed, templateId:" + bufferId.ToString());
    //    //    return effectValue;
    //    //}

    //    //LynxBuffer characterBuffer = skillUser.GetComponent<LynxBuffer>();
    //    //if (characterBuffer == null)
    //    //{
    //    //    Debug.LogWarning("Get effect value failed when get user LynxBuffer.");
    //    //    return effectValue;
    //    //}

    //    switch (iter->mValue->mEffectType)
    //    {
    //    case BUFFER_EFFECT_BURN:
    //    case BUFFER_EFFECT_POISONING:
    //    case BUFFER_EFFECT_BLEED:
    //        {
    //            //hudText.DelHp((float)buffer.mEffectValue);
    //            if (battleData.mBaseData.mHp <= iter->mValue->mEffectValue)
    //            {
    //                battleData.mBaseData.mHp = 0;
    //            }
    //            else
    //            {
    //                battleData.mBaseData.mHp = battleData.mBaseData.mHp - iter->mValue->mEffectValue;
    //            }
    //        }
    //        break;
    //    case BUFFER_EFFECT_CURE:
    //        {
    //            //hudText.AddHp((float)buffer.mEffectValue);
    //            if (battleData.mBaseData.mHp + iter->mValue->mEffectValue >= battleData.mBufferData.mHpUpper)
    //            {
    //                battleData.mBaseData.mHp = battleData.mBufferData.mHpUpper;
    //            }
    //            else
    //            {
    //                battleData.mBaseData.mHp = battleData.mBaseData.mHp + iter->mValue->mEffectValue;
    //            }
    //        }
    //        break;
    //    case BUFFER_EFFECT_SELF_CURE:
    //    case BUFFER_EFFECT_PARALYSIS:
    //    case BUFFER_EFFECT_BLIND:
    //        break;
    //    case BUFFER_EFFECT_ANGER_RED:
    //    case BUFFER_EFFECT_ANGER_INC:
    //    case BUFFER_EFFECT_CONVERGED:
    //    case BUFFER_EFFECT_SILENT:
    //    case BUFFER_EFFECT_PHY_INVINCIBLE:
    //    case BUFFER_EFFECT_MAG_INVINCIBLE:
    //    case BUFFER_EFFECT_SEAL:
    //    case BUFFER_EFFECT_REBOUND:
    //        break;
    //    case BUFFER_EFFECT_CLEAN:
    //        //GetCleanList(ref cleanList, (BufferType)buffer.mEffectValue);
    //        break;
    //    default:
    //        LOG_WARN("Unknown buffer effect type[%u].", iter->mValue->mEffectType);
    //        return false;
    //    }
    //}

    //// 净化相关buffer的删除
    ////if (cleanList.Count != 0)
    ////{
    ////    foreach (BufferData cleanBuffer in cleanList)
    ////    {
    ////        DelBuffer(cleanBuffer.mBufferId);
    ////    }
    ////}

    return true;
}

bool 
BattleManager::updateBufferOnHit(BattleData& battleData)
{
    // 更新被攻击的buffer，sec注释
	//List<BufferData*> bufferList;
    //getBufferListByPriority(bufferList, battleData);
    //if (bufferList.size() == 0)
    //{
    //    return true;
    //}
    //List<BufferData*> cleanList;

    //for (List<BufferData*>::Iter* iter = bufferList.begin();
    //    iter != bufferList.end(); iter = bufferList.next(iter))
    //{
    //    if (iter->mValue == NULL)
    //    {
    //        continue;
    //    }

    //    //LynxHUDText hudText = mCharacter.gameObject.GetComponent<LynxHUDText>();
    //    //if (hudText == null)
    //    //{
    //    //    Debug.LogWarning("UpdateBufferBeforeAttack get self LynxHUDText failed.");
    //    //    return false;
    //    //}

    //    switch (iter->mValue->mEffectType)
    //    {
    //    case BUFFER_EFFECT_BURN:
    //    case BUFFER_EFFECT_POISONING:
    //    case BUFFER_EFFECT_BLEED:
    //    case BUFFER_EFFECT_CURE:
    //        break;
    //    case BUFFER_EFFECT_SELF_CURE:
    //        {
    //            //hudText.AddHp((float)buffer.mEffectValue);
    //            if (battleData.mBaseData.mHp + iter->mValue->mEffectValue >= battleData.mBufferData.mHpUpper)
    //            {
    //                battleData.mBaseData.mHp = battleData.mBufferData.mHpUpper;
    //            }
    //            else
    //            {
    //                battleData.mBaseData.mHp = battleData.mBaseData.mHp + iter->mValue->mEffectValue;
    //            }
    //        }
    //        break;
    //    case BUFFER_EFFECT_PARALYSIS:
    //    case BUFFER_EFFECT_BLIND:
    //        break;
    //    case BUFFER_EFFECT_ANGER_RED:
    //        //if (mCharacter.Anger <= buffer.mEffectValue)
    //        //{
    //        //    mCharacter.Anger = 0;
    //        //}
    //        //else
    //        //{
    //        //    mCharacter.Anger = mCharacter.Anger - buffer.mEffectValue;
    //        //}
    //        break;
    //    case BUFFER_EFFECT_ANGER_INC:
    //        //if (mCharacter.Anger + buffer.mEffectValue >= mCharacter.AngerUpperBase)
    //        //{
    //        //    mCharacter.Anger = mCharacter.AngerUpperBase;
    //        //}
    //        //else
    //        //{
    //        //    mCharacter.Anger = mCharacter.Anger + buffer.mEffectValue;
    //        //}
    //        break;
    //    case BUFFER_EFFECT_CONVERGED:
    //    case BUFFER_EFFECT_SILENT:
    //    case BUFFER_EFFECT_PHY_INVINCIBLE:
    //    case BUFFER_EFFECT_MAG_INVINCIBLE:
    //    case BUFFER_EFFECT_SEAL:
    //    case BUFFER_EFFECT_REBOUND:
    //        break;
    //    case BUFFER_EFFECT_CLEAN:
    //        {
    //            getCleanList(cleanList, iter->mValue->mEffectValue, battleData);
    //        }
    //        break;
    //    default:
    //        LOG_WARN("Unknown buffer effect type[%u].", iter->mValue->mEffectType);
    //        return false;
    //    }
    //}

    //// 净化相关buffer的删除
    //if (cleanList.size() != 0)
    //{
    //    for (List<BufferData*>::Iter* iter = bufferList.begin();
    //        iter != bufferList.end(); iter = bufferList.next(iter))
    //    {
    //        if (iter->mValue != NULL)
    //        {
    //            delBuffer(iter->mValue->mBufferId, battleData);
    //        }
    //    }
    //}
    return true;
}

// 目标被击时的buffer更新，包括立即生效的buffer
bool 
BattleManager::updateBufferBeHit(BattleData& skillUser, float hurt, BattleData& selfData)
{
    //原游戏注释sec
	//List<BufferData*> bufferList;
    //getBufferListByPriority(bufferList, selfData);
    //if (bufferList.size() == 0)
    //{
    //    return true;
    //}
    //List<BufferData*> cleanList;

    //for (List<BufferData*>::Iter* iter = bufferList.begin();
    //    iter != bufferList.end(); iter = bufferList.next(iter))
    //{
    //    if (iter->mValue != NULL)
    //    {
    //        continue;
    //    }
    //    //LynxHUDText hudText = mCharacter.gameObject.GetComponent<LynxHUDText>();
    //    //if (hudText == null)
    //    //{
    //    //    Debug.LogWarning("UpdateBufferBeHit get self LynxHUDText failed.");
    //    //    return false;
    //    //}

    //    switch (iter->mValue->mEffectType)
    //    {
    //    case BUFFER_EFFECT_BURN:
    //    case BUFFER_EFFECT_POISONING:
    //    case BUFFER_EFFECT_BLEED:
    //    case BUFFER_EFFECT_CURE:
    //        break;
    //    case BUFFER_EFFECT_SELF_CURE:
    //        //hudText.AddHp((float)buffer.mEffectValue);
    //        //if (mCharacter.Hp + buffer.mEffectValue >= mHpUpper)
    //        //{
    //        //    mCharacter.Hp = mHpUpper;
    //        //}
    //        //else
    //        //{
    //        //    mCharacter.Hp = mCharacter.Hp + buffer.mEffectValue;
    //        //}
    //        break;
    //    case BUFFER_EFFECT_PARALYSIS:
    //    case BUFFER_EFFECT_BLIND:
    //        break;
    //    case BUFFER_EFFECT_ANGER_RED:
    //        {
    //            if (selfData.mBaseData.mAnger <= iter->mValue->mEffectValue)
    //            {
    //                selfData.mBaseData.mAnger = 0;
    //            }
    //            else
    //            {
    //                selfData.mBaseData.mAnger = selfData.mBaseData.mAnger - iter->mValue->mEffectValue;
    //            }
    //        }
    //        break;
    //    case BUFFER_EFFECT_ANGER_INC:
    //        {
    //            if (selfData.mBaseData.mAnger + iter->mValue->mEffectValue >= selfData.mExtendData.mAngerUpper)
    //            {
    //                selfData.mBaseData.mAnger = selfData.mExtendData.mAngerUpper;
    //            }
    //            else
    //            {
    //                selfData.mBaseData.mAnger = selfData.mBaseData.mAnger + iter->mValue->mEffectValue;
    //            }
    //        }
    //        break;
    //    case BUFFER_EFFECT_CONVERGED:
    //    case BUFFER_EFFECT_SILENT:
    //    case BUFFER_EFFECT_PHY_INVINCIBLE:
    //    case BUFFER_EFFECT_MAG_INVINCIBLE:
    //    case BUFFER_EFFECT_SEAL:
    //        break;
    //    case BUFFER_EFFECT_REBOUND:
    //        {
    //            if (hurt < 1.0f)
    //            {
    //                break;
    //            }
    //            //LynxCharacter user = skillUser.GetComponent<LynxCharacter>();
    //            //if (user == null)
    //            //{
    //            //    Debug.LogWarning("UpdateBufferBeHit failed when get user LynxCharacter.");
    //            //    return false;
    //            //}

    //            if (skillUser.mBaseData.mCharacterType == BATTLE_PET)
    //            {
    //                return true;
    //            }

    //            //LynxHUDText userHudText = skillUser.GetComponent<LynxHUDText>();
    //            //if (userHudText == null)
    //            //{
    //            //    Debug.Log("UpdateBufferBeHit get user LynxHUDText failed.");
    //            //    return false;
    //            //}

    //            UInt32 redHp = (UInt32)((float)hurt * ((float)iter->mValue->mEffectValue / 1000.0f));
    //            //userHudText.DelHp((float)redHp);
    //            if (skillUser.mBaseData.mHp <= redHp)
    //            {
    //                skillUser.mBaseData.mHp = 0;
    //                //如果是施法者自己死亡则需要处理战斗逻辑，主动结束战斗
    //                if (selfData.mBaseData.mTag == skillUser.mBaseData.mTag)
    //                {
    //                    deathForBufferInAttack(skillUser);
    //                }
    //            }
    //            else
    //            {
    //                skillUser.mBaseData.mHp = skillUser.mBaseData.mHp - redHp;
    //            }
    //        }
    //        break;
    //    case BUFFER_EFFECT_CLEAN:
    //        {
    //            getCleanList(cleanList, iter->mValue->mEffectValue, selfData);
    //        }
    //        break;
    //    default:
    //        LOG_WARN("Unknown buffer effect type[%u].", iter->mValue->mEffectType);
    //        return false;
    //    }
    //}

    //// 净化相关buffer的删除
    //if (cleanList.size() != 0)
    //{
    //    for (List<BufferData*>::Iter* iter = bufferList.begin();
    //        iter != bufferList.end(); iter = bufferList.next(iter))
    //    {
    //        if (iter->mValue != NULL)
    //        {
    //            delBuffer(iter->mValue->mBufferId, selfData);
    //        }
    //    }
    //}
    return true;
}

void 
BattleManager::getCleanList(List<BufferData*>& cleanList, UInt8 bufferType, BattleData& battleData)
{
    //原游戏获取清除buffer的列表，sec注释
	/*Map<UInt32, BufferData>& bufferMap = battleData.mBufferData.mBufferMap;
    for (Map<UInt32, BufferData>::Iter* iter = bufferMap.begin();
        iter != bufferMap.end(); iter = bufferMap.next(iter))
    {
        if (iter->mKey != 0 && iter->mValue.mActionCount != 0)
        {
            BufferTemplate* bufferTemplate = BUFFER_TABLE().get(iter->mKey);
            if (bufferTemplate == NULL)
            {
                continue;
            }
            if (bufferTemplate->mType == bufferType 
                && bufferTemplate->mCleanUp == 0)
            {
                cleanList.insertTail(&iter->mValue);
            }
        }
    }*/
}

void 
BattleManager::addPassivityBufferByTriggerType(BattleData& selfData, BattleData& skillUser, UInt8 triggerType)
{
    for (int i = 0; i < 3; i++)
    {
        if (selfData.mBufferData.mPassivityId[i] != 0)
        {
            BufferTemplate* bufferTemplate = BUFFER_TABLE().get(selfData.mBufferData.mPassivityId[i]);
            if (bufferTemplate == NULL)
            {
                LOG_WARN("Get passivity template[%u] failed.", selfData.mBufferData.mPassivityId[i]);
                continue;
            }

            if (bufferTemplate->mTriggerType != triggerType)
            {
                continue;
            }

            UInt32 randomValue = GameRandom::getUInt32Random(1, 100);
            if (randomValue > bufferTemplate->mOdds)
            {
                continue;
            }

            bool nextBegin = bufferTemplate->mLastActionCount != 0 ? true : false;
            if (bufferTemplate->mTargetType == BUFFER_TARGET_SELF)
            {
                addBuffer(selfData, selfData.mBufferData.mPassivityId[i], skillUser, nextBegin);
            }
            else
            {
                //LynxBuffer lynxBuffer = skillUser.GetComponent<LynxBuffer>();
                //if (lynxBuffer == null)
                //{
                //    Debug.LogWarning("TriggerPassivityOnBeHit get self LynxBuffer failed.");
                //    continue;
                //}

                //lynxBuffer.AddBuffer(mPassivityId[i], skillUser, nextBegin);
                addBuffer(skillUser, selfData.mBufferData.mPassivityId[i], skillUser, nextBegin);
            }
        }
    }
}

void 
BattleManager::triggerPassivityOnBeHit(BattleData& selfData, BattleData& skillUser)
{
    addPassivityBufferByTriggerType(selfData, skillUser, BUFFER_TRIGGER_BEHIT);
}

void 
BattleManager::triggerPassivityOnDodge(BattleData& selfData, BattleData& skillUser)
{
    addPassivityBufferByTriggerType(selfData, skillUser, BUFFER_TRIGGER_DODGE);
}

void 
BattleManager::triggerPassivityOnCritical(BattleData& selfData, BattleData& skillUser)
{
    addPassivityBufferByTriggerType(selfData, skillUser, BUFFER_TRIGGER_CRITICAL);
}

void 
BattleManager::triggerPassivityOnKillInOnce(BattleData& selfData, BattleData& skillUser)
{
    addPassivityBufferByTriggerType(selfData, skillUser, BUFFER_TRIGGER_KILL_IN_ONCE);
}

bool 
BattleManager::updateProperty(UInt32 bufferId, BattleData& battleData)
{
    BufferTemplate* bufferTemplate = BUFFER_TABLE().get(bufferId);
    if (bufferTemplate == NULL)
    {
        LOG_WARN("Update buffer property get buffer template[%u] failed.", bufferId);
        return false;
    }

    for (int i = 0; i < 2; i++)
    {
        UInt8 propertyType = bufferTemplate->mPropertyType1;
        float propertyValue = bufferTemplate->mPropertyValue1;
        if (i == 1)
        {
            propertyType = bufferTemplate->mPropertyType2;
            propertyValue = bufferTemplate->mPropertyValue2;
        }

        if (propertyType == 0 || propertyValue == 0)
        {
            continue;
        }

        switch (propertyType)
        {
        case BUFFER_PROPERTY_HP:
            {
                UInt32 hp = (UInt32)((float)battleData.mExtendData.mHpUpper * (abs(propertyValue) / 100.0f));
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mHpUpper += hp;
                    battleData.mBaseData.mHp += hp;
                }
                else
                {
                    if (battleData.mBufferData.mHpUpper > hp)
                    {
                        battleData.mBufferData.mHpUpper -= hp;
                        if (battleData.mBufferData.mHpUpper < battleData.mBaseData.mHp)
                        {
                            battleData.mBaseData.mHp = battleData.mBufferData.mHpUpper;
                        }
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_ATTACK:
            {
                UInt32 attack = (UInt32)((float)battleData.mExtendData.mAttack * (abs(propertyValue) / 100.0f));
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mAttack += attack;
                }
                else
                {
                    if (battleData.mBufferData.mAttack > attack)
                    {
                        battleData.mBufferData.mAttack -= attack;
                    }
                    else
                    {
                        battleData.mBufferData.mAttack = 1;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_PHYDEF:
            {
                UInt32 phyDef = (UInt32)((float)battleData.mExtendData.mPhysicsDefense * (abs(propertyValue) / 100.0f));
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mPhysicsDefense += phyDef;
                }
                else
                {
                    if (battleData.mBufferData.mPhysicsDefense > phyDef)
                    {
                        battleData.mBufferData.mPhysicsDefense -= phyDef;
                    }
                    else
                    {
                        battleData.mBufferData.mPhysicsDefense = 0;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_MAGDEF:
            {
                UInt32 magDef = (UInt32)((float)battleData.mBufferData.mMagicDefense * (abs(propertyValue) / 100.0f));
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mMagicDefense += magDef;
                }
                else
                {
                    if (battleData.mBufferData.mMagicDefense > magDef)
                    {
                        battleData.mBufferData.mMagicDefense -= magDef;
                    }
                    else
                    {
                        battleData.mBufferData.mMagicDefense = 0;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_CRITICAL:
            {
                UInt32 critical = (UInt32)abs(propertyValue);
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mCritical += critical;
                }
                else
                {
                    if (battleData.mBufferData.mCritical > critical)
                    {
                        battleData.mBufferData.mCritical -= critical;
                    }
                    else
                    {
                        battleData.mBufferData.mCritical = 0;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_RESCRIT:
            {
                UInt32 rescrit = (UInt32)abs(propertyValue);
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mRescrit += rescrit;
                }
                else
                {
                    if (battleData.mBufferData.mRescrit > rescrit)
                    {
                        battleData.mBufferData.mRescrit -= rescrit;
                    }
                    else
                    {
                        battleData.mBufferData.mRescrit = 0;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_HIT:
            {
                UInt32 hit = (UInt32)abs(propertyValue);
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mHit += hit;
                }
                else
                {
                    if (battleData.mBufferData.mHit > hit)
                    {
                        battleData.mBufferData.mHit -= hit;
                    }
                    else
                    {
                        battleData.mBufferData.mHit = 0;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_DODGE:
            {
                UInt32 dodge = (UInt32)abs(propertyValue);
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mDodge += dodge;
                }
                else
                {
                    if (battleData.mBufferData.mDodge > dodge)
                    {
                        battleData.mBufferData.mDodge -= dodge;
                    }
                    else
                    {
                        battleData.mBufferData.mDodge = 0;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_DAMAGE_RED:
            {
                UInt32 damageRed = (UInt32)abs(propertyValue);
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mAvoidDamage += damageRed;
                }
                else
                {
                    if (battleData.mBufferData.mAvoidDamage > damageRed)
                    {
                        battleData.mBufferData.mAvoidDamage -= damageRed;
                    }
                    else
                    {
                        battleData.mBufferData.mAvoidDamage = 0;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_SUCK_BLOOD:
            {
                UInt32 suckBlood = (UInt32)abs(propertyValue);
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mSuck += suckBlood;
                }
                else
                {
                    if (battleData.mBufferData.mSuck > suckBlood)
                    {
                        battleData.mBufferData.mSuck -= suckBlood;
                    }
                    else
                    {
                        battleData.mBufferData.mSuck = 0;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_CURE:
            {
                UInt32 cure = (UInt32)abs(propertyValue);
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mCure += cure;
                }
                else
                {
                    if (battleData.mBufferData.mCure > cure)
                    {
                        battleData.mBufferData.mCure -= cure;
                    }
                    else
                    {
                        battleData.mBufferData.mCure = 0;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_CRITICAL_DAMAGE:
            {
                UInt32 criticalDamage = (UInt32)abs(propertyValue);
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mCriticalDamage += criticalDamage;
                }
                else
                {
                    if (battleData.mBufferData.mCriticalDamage > criticalDamage)
                    {
                        battleData.mBufferData.mCriticalDamage -= criticalDamage;
                    }
                    else
                    {
                        battleData.mBufferData.mCriticalDamage = 0;
                    }
                }
            }
            break;
        default:
            LOG_WARN("Unknown property type[%u].", propertyType);
            return false;
        }
    }
    return true;
}

bool 
BattleManager::revertProperty(BattleData& battleData, BufferData bufferData)
{
    //sec 注释，原来的游戏回复属性的函数
	
	/*BufferTemplate* bufferTemplate = BUFFER_TABLE().get(bufferData.mBufferId);
    if (bufferTemplate == NULL)
    {
        LOG_WARN("Revert buffer property get buffer template[%u] failed.", bufferData.mBufferId);
        return false;
    }

    for (int i = 0; i < 2; i++)
    {
        UInt8 propertyType = bufferTemplate->mPropertyType1;
        float propertyValue = bufferTemplate->mPropertyValue1;
        if (i == 1)
        {
            propertyType = bufferTemplate->mPropertyType2;
            propertyValue = bufferTemplate->mPropertyValue2;
        }

        if (propertyType == 0 || propertyValue == 0)
        {
            continue;
        }

        switch (propertyType)
        {
        case BUFFER_PROPERTY_HP:
            {
                UInt32 hp = (UInt32)((float)battleData.mExtendData.mHpUpper * (abs(propertyValue) / 100.0f)) 
                    * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mHpUpper > hp)
                    {
                        battleData.mBufferData.mHpUpper -= hp;
                        if (battleData.mBufferData.mHpUpper < battleData.mBaseData.mHp)
                        {
                            battleData.mBaseData.mHp = battleData.mBufferData.mHpUpper;
                        }
                    }
                }
                else
                {
                    battleData.mBufferData.mHpUpper += hp;
                    battleData.mBaseData.mHp += hp;
                }
            }
            break;
        case BUFFER_PROPERTY_ATTACK:
            {
                UInt32 attack = (UInt32)((float)battleData.mExtendData.mAttack * (abs(propertyValue) / 100.0f))
                    * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mAttack > attack)
                    {
                        battleData.mBufferData.mAttack -= attack;
                    }
                    else
                    {
                        battleData.mBufferData.mAttack = 1;
                    }
                }
                else
                {
                    battleData.mBufferData.mAttack += attack;
                }
            }
            break;
        case BUFFER_PROPERTY_PHYDEF:
            {
                UInt32 phyDef = (UInt32)((float)battleData.mExtendData.mPhysicsDefense * (abs(propertyValue) / 100.0f))
                    * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mPhysicsDefense > phyDef)
                    {
                        battleData.mBufferData.mPhysicsDefense -= phyDef;
                    }
                    else
                    {
                        battleData.mBufferData.mPhysicsDefense = 0;
                    }
                }
                else
                {
                    battleData.mBufferData.mPhysicsDefense += phyDef;
                }
            }
            break;
        case BUFFER_PROPERTY_MAGDEF:
            {
                UInt32 magDef = (UInt32)((float)battleData.mExtendData.mMagicDefense * (abs(propertyValue) / 100.0f))
                    * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    battleData.mBufferData.mMagicDefense += magDef;
                }
                else
                {
                    if (battleData.mBufferData.mMagicDefense > magDef)
                    {
                        battleData.mBufferData.mMagicDefense -= magDef;
                    }
                    else
                    {
                        battleData.mBufferData.mMagicDefense = 0;
                    }
                }
            }
            break;
        case BUFFER_PROPERTY_CRITICAL:
            {
                UInt32 critical = (UInt32)abs(propertyValue) * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mCritical > critical)
                    {
                        battleData.mBufferData.mCritical -= critical;
                    }
                    else
                    {
                        battleData.mBufferData.mCritical = 0;
                    }
                }
                else
                {
                    battleData.mBufferData.mCritical += critical;
                }
            }
            break;
        case BUFFER_PROPERTY_RESCRIT:
            {
                UInt32 rescrit = (UInt32)abs(propertyValue) * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mRescrit > rescrit)
                    {
                        battleData.mBufferData.mRescrit -= rescrit;
                    }
                    else
                    {
                        battleData.mBufferData.mRescrit = 0;
                    }
                }
                else
                {
                    battleData.mBufferData.mRescrit += rescrit;
                }
            }
            break;
        case BUFFER_PROPERTY_HIT:
            {
                UInt32 hit = (UInt32)abs(propertyValue) * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mHit > hit)
                    {
                        battleData.mBufferData.mHit -= hit;
                    }
                    else
                    {
                        battleData.mBufferData.mHit = 0;
                    }
                }
                else
                {
                    battleData.mBufferData.mHit += hit;
                }
            }
            break;
        case BUFFER_PROPERTY_DODGE:
            {
                UInt32 dodge = (UInt32)abs(propertyValue) * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mDodge > dodge)
                    {
                        battleData.mBufferData.mDodge -= dodge;
                    }
                    else
                    {
                        battleData.mBufferData.mDodge = 0;
                    }
                }
                else
                {
                    battleData.mBufferData.mDodge += dodge;
                }
            }
            break;
        case BUFFER_PROPERTY_DAMAGE_RED:
            {
                UInt32 damageRed = (UInt32)abs(propertyValue) * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mAvoidDamage > damageRed)
                    {
                        battleData.mBufferData.mAvoidDamage -= damageRed;
                    }
                    else
                    {
                        battleData.mBufferData.mAvoidDamage = 0;
                    }
                }
                else
                {
                    battleData.mBufferData.mAvoidDamage += damageRed;
                }
            }
            break;
        case BUFFER_PROPERTY_SUCK_BLOOD:
            {
                UInt32 suckBlood = (UInt32)abs(propertyValue) * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mSuck > suckBlood)
                    {
                        battleData.mBufferData.mSuck -= suckBlood;
                    }
                    else
                    {
                        battleData.mBufferData.mSuck = 0;
                    }
                }
                else
                {
                    battleData.mBufferData.mSuck += suckBlood;
                }
            }
            break;
        case BUFFER_PROPERTY_CURE:
            {
                UInt32 cure = (UInt32)abs(propertyValue) * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mCure > cure)
                    {
                        battleData.mBufferData.mCure -= cure;
                    }
                    else
                    {
                        battleData.mBufferData.mCure = 0;
                    }
                }
                else
                {
                    battleData.mBufferData.mCure += cure;
                }
            }
            break;
        case BUFFER_PROPERTY_CRITICAL_DAMAGE:
            {
                UInt32 criticalDamage = (UInt32)abs(propertyValue) * (UInt32)(bufferData.mOverlayCount + 1);
                if (propertyValue > 0.0f)
                {
                    if (battleData.mBufferData.mCriticalDamage > criticalDamage)
                    {
                        battleData.mBufferData.mCriticalDamage -= criticalDamage;
                    }
                    else
                    {
                        battleData.mBufferData.mCriticalDamage = 0;
                    }
                }
                else
                {
                    battleData.mBufferData.mCriticalDamage += criticalDamage;
                }
            }
            break;
        default:
            LOG_WARN("Unknown property type[%u].", propertyType);
            return false;
        }
    }*/
    return true;
}

UInt32 
BattleManager::getEffectValue(UInt32 bufferId, BattleData& skillUser)
{
    UInt32 effectValue = 0;
    BufferTemplate* bufferTemplate = BUFFER_TABLE().get(bufferId);
    if (bufferTemplate == NULL)
    {
        LOG_WARN("Get effect value get buffer template[%u] failed.", bufferId);
        return effectValue;
    }

    //LynxBuffer characterBuffer = skillUser.GetComponent<LynxBuffer>();
    //if (characterBuffer == null)
    //{
    //    Debug.LogWarning("Get effect value failed when get user LynxBuffer.");
    //    return effectValue;
    //}

    switch (bufferTemplate->mEffectType)
    {
    case BUFFER_EFFECT_BURN:
    case BUFFER_EFFECT_POISONING:
    case BUFFER_EFFECT_BLEED:
    case BUFFER_EFFECT_CURE:
        {
            effectValue = (UInt32)((float)skillUser.mBufferData.mAttack * (bufferTemplate->mEffectValue / 1000.0f));
        }
        break;
    case BUFFER_EFFECT_PARALYSIS:
    case BUFFER_EFFECT_BLIND:
    case BUFFER_EFFECT_ANGER_RED:
    case BUFFER_EFFECT_ANGER_INC:
        {
            effectValue = (UInt32)bufferTemplate->mEffectValue;
        }
        break;
    case BUFFER_EFFECT_CONVERGED:
    case BUFFER_EFFECT_SILENT:
    case BUFFER_EFFECT_PHY_INVINCIBLE:
    case BUFFER_EFFECT_MAG_INVINCIBLE:
    case BUFFER_EFFECT_SEAL:
    case BUFFER_EFFECT_CLEAN:
        {
            effectValue = 0;
        }
        break;
    case BUFFER_EFFECT_REBOUND:
        {
            //effectValue = (UInt32)((float)hurt * (bufferTemplate.mEffectValue / 1000.0f));
            effectValue = (UInt32)bufferTemplate->mEffectValue;
        }
        break;
    case BUFFER_EFFECT_SELF_CURE:
        {
            effectValue = (UInt32)((float)skillUser.mBufferData.mHpUpper * (bufferTemplate->mEffectValue / 1000.0f));
        }
        break;
    default:
        LOG_WARN("Unkonwn buffer effect type[%u].", bufferTemplate->mEffectType);
        return effectValue;
    }
    return effectValue;
}

void 
BattleManager::addMyselfDeathCount()
{
    mDeathCount++;
}

void 
BattleManager::oneAttackEnd(bool isSuperSkill, bool isEnterSkill, UInt8 relationship, 
                            UInt8 characterType, String curEndTag)
{
    String endTag = curEndTag;

    if (characterType != BATTLE_PET && !isEnterSkill && !isSuperSkill)
    {
        mAttackNum++;
        //mSkillButton.SetToggleNum((int)mAttackNum);
        //Debug.Log("Attack num: " + mAttackNum.ToString());
    }

    // 是否在回合结束时检查时候替补可以出场
    if (checkBenchEnter())
    {
        //Debug.Log("Bench enter and use enter skill.");
        return;
    }

    if (checkResult(relationship))
    {
        //Debug.Log("Battle end.");
        return;
    }

    //SetCharacterIdleDir();

    if (characterType == BATTLE_PET)
    {
        endTag = mLastEndTag;
    }

    if (isEnterSkill)
    {
        endTag = mLastEndTag;
    }

    if (mMySuperSkillUsed)
    {
        mMySuperSkillUsed = false;
        //Debug.Log("Enemy use superskill.");
        // todo yangyang 目前随机选取技能，以后需要按照AI规则来选取技能
        if (superSkillUse(BATTLE_FOE, GameRandom::getUInt32Random(0, 2)))
        {
            return;
        }
        else
        {
            LOG_WARN("Enemy use superskill failed.");
            // 失败后接着进行下一个user的获取
            endTag = mLastEndTag;
        }
        //if (relationship == LynxCharacter.Relationship.FRIEND)
        //{
        //    Debug.LogWarning("Enemy use superskill.");
        //    if (SuperSkillUse(LynxCharacter.Relationship.FOE, UnityEngine.Random.Range(0, 2)))
        //    {
        //        return;
        //    }
        //    else
        //    {
        //        Debug.LogWarning("Enemy use superskill failed.");
        //        // 失败后接着进行下一个user的获取
        //        endTag = mLastEndTag;
        //    }
        //}
        //else
        //{
        //    endTag = mLastEndTag;
        //    mSkillButton.SetToggleNum(0);
        //}
    }
    else
    {
        if (mOppSuperSkillUsed)
        {
            mOppSuperSkillUsed = false;
            endTag = mLastEndTag;
            //mSkillButton.SetToggleNum(0);
            //mSkillButton.SetRoundText((int)mRoundNum + 1);
        }

        if (characterType != BATTLE_PET)
        {
            //// 检查当前结束的tag是否可以释放自动技能
            //// 如果当前角色是死亡后的替补上来了，由于anger不足所以不成问题
            //LynxCharacter curEnd = GetCharacterByTag(endTag);
            //if (curEnd != null)
            //{
            //    if (curEnd.GetAnger() >= curEnd.GetTriggerAnger())
            //    {
            //        curEnd.UseAutoSkill();
            //        return;
            //    }
            //}

            // 检查大招
            if (mAttackNum >= 6)
            {
                // 6次行动为1回合
                mRoundNum++;
                mPlayerPetSkillUsed = false;
                mEnemyPetSkillUsed = false;
                //Debug.Log("round num : " + mRoundNum);
                //mSkillButton.SetRoundText((int)mRoundNum);

                mAttackNum = 0;
                //SkillButtonInit();
                mLastEndTag = endTag;
                //MCameraCtrl.Reset();
                //Debug.Log("use super skill friend waiting. mlastendTag " + mLastEndTag);

                if (!mIsVerifyBattle)
                {
                    // todo yangyang 目前随机选取技能，以后需要按照AI规则来选取技能
                    if (superSkillUse(BATTLE_FRIEND, GameRandom::getUInt32Random(0, 2)))
                    {
                        return;
                    }
                    else
                    {
                        LOG_WARN("Player use superskill failed.");
                        // 失败后接着进行下一个user的获取
                        endTag = mLastEndTag;
                    }
                }
                else
                {
                    // todo yangyang 目前随机选取技能，以后需要按照AI规则来选取技能
                    // 此处为了服务器播放验证随机数的一致性使用系统随机，不使用GameRandom
                    if (superSkillUse(BATTLE_FRIEND, RANDOM().getUInt32Random(0, 2)))
                    {
                        return;
                    }
                    else
                    {
                        LOG_WARN("Player use superskill failed.");
                        // 失败后接着进行下一个user的获取
                        endTag = mLastEndTag;
                    }
                }
                //return;
            }
        }

    }

    // 检查PetSkill
    if (!mPlayerPetSkillUsed)
    {
        //if (relationship == LynxCharacter.Relationship.FRIEND
        //    && mPlayerCharacterPet.CoolDownRound != 0 && mRoundNum != 0)
        if (mPlayerPet.mSkillData.mCommonSkill.mCoolDownRound != 0 && mRoundNum != 0)
        {
            UInt32 round = mRoundNum % mPlayerPet.mSkillData.mCommonSkill.mCoolDownRound;
            if (round == 0)
            {
                mPlayerPetSkillUsed = true;
                mLastEndTag = endTag;
                useCommonSkill(&mPlayerPet, mPlayerPet.mSkillData.mCommonSkill);
                return;
            }
        }
    }
    if (!mEnemyPetSkillUsed)
    {
        //if (relationship == LynxCharacter.Relationship.FOE
        //    && mEnemyCharacterPet.CoolDownRound != 0 && mRoundNum != 0)
        if (mEnemyPet.mSkillData.mCommonSkill.mCoolDownRound != 0 && mRoundNum != 0)
        {
            UInt32 round = mRoundNum % mEnemyPet.mSkillData.mCommonSkill.mCoolDownRound;
            if (round == 0)
            {
                mEnemyPetSkillUsed = true;
                mLastEndTag = endTag;
                useCommonSkill(&mEnemyPet, mEnemyPet.mSkillData.mCommonSkill);
                return;
            }
        }
    }

    // 检查是否到20回合，20回合若对方有存活则判定为失败
    if (checkOverRound())
    {
        //Debug.Log("Lose!");
        showResult(0);
        return;
    }

    BattleData* user = getUser(endTag, relationship);
    if (user == NULL)
    {
        LOG_WARN("Attack end but get user failed tag[%s].", endTag.c_str());
        return;
    }

    mLastEndTag = user->mBaseData.mTag;
    // 看是否被沉默，沉默不能使用自动技能
    if (checkAutoSkill(user) && user->mBaseData.mSilentId == 0)
    {
        //Debug.Log("get user use autoSkill tag : " + user.Tag);
        useAutoSkill(user, user->mSkillData.mAutoSkill);
    }
    else
    {
        //Debug.Log("get user use commonSkill tag : " + user.Tag);
        useCommonSkill(user, user->mSkillData.mCommonSkill);
    }

}

List<BattleData>::Iter* 
BattleManager::getEnemyBench()
{
    for (List<BattleData>::Iter* iter = mEnemyBench.begin();
        iter != mEnemyBench.end(); iter = mEnemyBench.next(iter))
    {
        if (iter->mValue.mBaseData.mGuidId != 0
            && iter->mValue.mBaseData.mHp != 0)
        {
            return iter;
        }
    }
    return NULL;
}

List<BattleData>::Iter* 
BattleManager::getPlayerBench()
{
    for (List<BattleData>::Iter* iter = mPlayerBench.begin();
        iter != mPlayerBench.end(); iter = mPlayerBench.next(iter))
    {
        if (iter->mValue.mBaseData.mGuidId != 0
            && iter->mValue.mBaseData.mHp != 0)
        {
            return iter;
        }
    }
    return NULL;
}

bool 
BattleManager::checkBenchEnter()
{
    for (int i = 0; i < 3; i++)
    {
        if (mEnemyArray[i].mBaseData.mHp == 0)
        {
            List<BattleData>::Iter* heroETmp = getEnemyBench();
            if (heroETmp == NULL)
            {
                //mEnemyBenchIndex = 0;
                break;
            }
            BattleData heroE = heroETmp->mValue;
            mEnemyBench.erase(heroETmp);
            //mEnemyBenchIndex++;
            //switch (mEnemyBenchIndex)
            //{
            //case 1:
            //    mSkillButton.SetOtherBenchAEnable(false);
            //    break;
            //case 2:
            //    mSkillButton.SetOtherBenchBEnable(false);
            //    break;
            //}
            ////bool roundEnd = mEnemy[i].RoundEnd;
            //Destroy(mEnemy[i].gameObject);
            switch (i)
            {
            case 0:
                {
                    mEnemyArray[0] = heroE;
                    mEnemyArray[0].mBaseData.mTag = "E1";
                    mEnemyArray[0].mBaseData.mRelationship = BATTLE_FOE;
                    useEnterSkill(&mEnemyArray[0], mEnemyArray[0].mSkillData.mEnterSkill);
                }
                break;
            case 1:
                {
                    mEnemyArray[1] = heroE;
                    mEnemyArray[1].mBaseData.mTag = "E2";
                    mEnemyArray[1].mBaseData.mRelationship = BATTLE_FOE;
                    useEnterSkill(&mEnemyArray[1], mEnemyArray[1].mSkillData.mEnterSkill);
                }
                break;
            case 2:
                {
                    mEnemyArray[2] = heroE;
                    mEnemyArray[2].mBaseData.mTag = "E3";
                    mEnemyArray[2].mBaseData.mRelationship = BATTLE_FOE;
                    useEnterSkill(&mEnemyArray[2], mEnemyArray[2].mSkillData.mEnterSkill);
                }
                break;
            }
            return true;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (mPlayerArray[i].mBaseData.mHp == 0)
        {
            List<BattleData>::Iter* heroTmp = getPlayerBench();
            if (heroTmp == NULL)
            {
                //mPlayerBenchIndex = 0;
                break;
            }
            BattleData hero = heroTmp->mValue;
            mPlayerBench.erase(heroTmp);
            //mPlayerBenchIndex++;
            //switch (mPlayerBenchIndex)
            //{
            //case 1:
            //    mSkillButton.SetBenchAEnable(false);
            //    break;
            //case 2:
            //    mSkillButton.SetBenchBEnable(false);
            //    break;
            //}
            ////bool roundEnd = mPlayer[i].RoundEnd;
            //Destroy(mPlayer[i].gameObject);
            switch (i)
            {
            case 0:
                {
                    mPlayerArray[0] = hero;
                    mPlayerArray[0].mBaseData.mTag = "P1";
                    mPlayerArray[0].mBaseData.mRelationship = BATTLE_FRIEND;
                    useEnterSkill(&mPlayerArray[0], mPlayerArray[0].mSkillData.mEnterSkill);
                }
                break;
            case 1:
                {
                    mPlayerArray[1] = hero;
                    mPlayerArray[1].mBaseData.mTag = "P2";
                    mPlayerArray[1].mBaseData.mRelationship = BATTLE_FRIEND;
                    useEnterSkill(&mPlayerArray[1], mPlayerArray[1].mSkillData.mEnterSkill);
                }
                break;
            case 2:
                {
                    mPlayerArray[2] = hero;
                    mPlayerArray[2].mBaseData.mTag = "P3";
                    mPlayerArray[2].mBaseData.mRelationship = BATTLE_FRIEND;
                    useEnterSkill(&mPlayerArray[2], mPlayerArray[2].mSkillData.mEnterSkill);
                }
                break;
            }
            return true;
        }
    }

    return false;
}

bool 
BattleManager::superSkillUse(UInt8 relationShip, UInt32 index)
{
    if (index > 2)
    {
        LOG_WARN("Super skill use index[%u] overflow.", index);
        return false;
    }

    UInt32 userIndex = index;
    if (mIsVerifyBattle)
    {
        if (mBattleVerifyData.mSuperSkillRecord.size() != 0)
        {
            List<UInt32>::Iter* iter = mBattleVerifyData.mSuperSkillRecord.begin();
            if (iter == NULL)
            {
                LOG_WARN("Super skill use in verify battle get record failed.");
                return false;
            }
            userIndex = iter->mValue - 1;
            mBattleVerifyData.mSuperSkillRecord.erase(iter);
            LOG_INFO("Battle test Super skill use index : %u", userIndex + 1);
        }
    }
    else
    {
        if (mSimulationData != NULL)
        {
            mSimulationData->mSuperSkillList.insertTail(userIndex + 1);
        }
    }

    BattleData* user;
    if (relationShip == BATTLE_FOE)
    {
        user = &mEnemyArray[userIndex];
        if (user == NULL || user->mBaseData.mHp == 0)
        {
            user = getAnyEnemy();
        }
    }
    else
    {
        user = &mPlayerArray[userIndex];
        if (user == NULL || user->mBaseData.mHp == 0)
        {
            user = getAnyPlayer();
        }
    }

    if (user != NULL)
    {
        if (relationShip == BATTLE_FRIEND)
        {
            mMySuperSkillUsed = true;
            //mSkillButton.SkillClose();
        }
        else
        {
            mOppSuperSkillUsed = true;
        }
        useSuperSkill(user, user->mSkillData.mSuperSkill);
    }
    else
    {
        LOG_WARN("Enemy use skill failed, no one use skill.");
        return false;
    }
    return true;
}

BattleData* 
BattleManager::getUser(String curEndTag, UInt8 relationship)
{
    BattleData* user = NULL;
    if (curEndTag == "P1" || curEndTag == "P2" || curEndTag == "P3")
    {
        user = getEnemyUser();
    }
    else if (curEndTag == "E1" || curEndTag == "E2" || curEndTag == "E3")
    {
        user = getPlayerUser();
    }

    if (user == NULL)
    {
        checkResult(relationship);
    }
    //if (user == null)
    //{
    //    ResetAllRoundEnd();
    //    user = GetPlayerUser();
    //    mRoundNum++;
    //    mPlayerPetSkillUsed = false;
    //    mEnemyPetSkillUsed = false;
    //    Debug.Log("round num : " + mRoundNum);
    //}
    return user;
}

BattleData* 
BattleManager::getPlayerUser()
{
    int round = 0;
    for (int i = mPlayerUserIndex; i < 3; i++)
    {
        if (mPlayerArray[i].mBaseData.mHp != 0)
        {
            mPlayerUserIndex = (i == 2) ? 0 : (i + 1);
            return &mPlayerArray[i];
        }

        if (i == 2)
        {
            round++;
            i = -1;
        }

        if (round == 2)
        {
            //Debug.LogWarning("Get player user failed round 2.");
            break;
        }
    }
    //for (int i = 0; i < mPlayer.Length; i++)
    //{
    //    if (mPlayer[i] != null && mPlayer[i].Hp != 0
    //        && !mPlayer[i].RoundEnd)
    //    {
    //        return mPlayer[i];
    //    }
    //}
    return NULL;
}

BattleData* 
BattleManager::getEnemyUser()
{
    int round = 0;
    for (int i = mEnemyUserIndex; i < 3; i++)
    {
        if (mEnemyArray[i].mBaseData.mHp != 0)
        {
            mEnemyUserIndex = (i == 2) ? 0 : (i + 1);
            LOG_INFO("Battle test mEnemyUserIndex %u", mEnemyUserIndex);
            return &mEnemyArray[i];
        }

        if (i == 2)
        {
            round++;
            i = -1;
        }

        if (round == 2)
        {
            //Debug.LogWarning("Get enemy user failed round 2.");
            break;
        }
    }
    //for (int i = 0; i < mEnemy.Length; i++)
    //{
    //    if (mEnemy[i] != null && mEnemy[i].Hp != 0
    //        && !mEnemy[i].RoundEnd)
    //    {
    //        return mEnemy[i];
    //    }
    //}
    return NULL;
}

bool 
BattleManager::checkResult(UInt8 relationship)
{
    BattleData* character = NULL;
    if (relationship == BATTLE_FRIEND)
    {
        character = getAnyEnemy();
        if (character == NULL)
        {
            //Debug.Log("Win!");
            showResult(1);
            return true;
        }
    }
    else
    {
        character = getAnyPlayer();
        if (character == NULL)
        {
            //Debug.Log("Lose!");
            showResult(0);
            return true;
        }
    }
    return false;
}

bool 
BattleManager::checkOverRound()
{
    // 由于是从0开始算起的，20回合结束
    if (mRoundNum >= 19)
    {
        BattleData* character = getAnyEnemy();
        if (character != NULL)
        {
            return true;
        }
    }
    return false;
}

bool 
BattleManager::getTarget(List<BattleData*>& targetList, BattleSkillItem skillItem, String currentTag, UInt8 relationship)
{
    if (currentTag.empty())
    {
        LOG_WARN("Get target args error.");
        return false;
    }
    targetList.clear();
    BattleData* target = NULL;
    // 获取技能敌我类型
    //LynxCharacter.SkillRelationship skillRelationship = LynxCharacter.SkillRelationship.FOE;
    //SkillTemplate* skillTemplate = SKILL_TABLE().get(skillItem.mTemplateId);
    //if (skillTemplate == NULL)
    //{
    //    LOG_WARN("Get skillTemplate failed templateId[%u].", skillItem.mTemplateId);
    //    return false;
    //}
    UInt8 skillRelationship = skillItem.mSkillRelationship;

    switch (skillItem.mSkillAttackType)
    {
    case SKILL_ONLY_POSITION:
        {
            target = getPositionTarget(currentTag, relationship, skillRelationship);
            if (target == NULL)
            {
                LOG_WARN("Get position target failed tag[%s].", currentTag.c_str());
                return false;
            }
            targetList.insertTail(target);
        }
        break;
    case SKILL_RANDOM_ONE:
        {
            target = getRandomOneTarget(relationship, skillRelationship);
            if (target == NULL)
            {
                LOG_WARN("Get random one target failed tag[%s]", currentTag.c_str());
                return false;
            }
            targetList.insertTail(target);
        }
        break;
    case SKILL_RANDOM_TWO:
        {
            getRandomTwoTarget(targetList, relationship, skillRelationship);
        }
        break;
    case SKILL_ALL:
        {
            getAllTarget(targetList, relationship, skillRelationship);
        }
        break;
    case SKILL_MOST_BLOOD_ONE:
        {
            target = getMostBloodOneTarget(relationship, skillRelationship);
            if (target == NULL)
            {
                LOG_WARN("Get most blood one target failed tag[%s].", currentTag.c_str());
                return false;
            }
            targetList.insertTail(target);
        }
        break;
    case SKILL_LEAST_BLOOD_ONE:
        {
            target = getLeastBloodOneTarget(relationship, skillRelationship);
            if (target == NULL)
            {
                LOG_WARN("Get least blood one target failed tag[%s].", currentTag.c_str());
                return false;
            }
            targetList.insertTail(target);
        }
        break;
    default:
        LOG_WARN("Unknown skill type[%d] define.", skillItem.mSkillAttackType);
        return false;
    }

    return true;
}

BattleData* 
BattleManager::getPositionTarget(String currentTag, UInt8 relationship, UInt8 skillRelationship)
{
    BattleData* character = NULL;
    Int32 index = 0;
    if (currentTag == "E1" || currentTag == "P1")
    {
        index = 0;
    }
    else if (currentTag == "E2" || currentTag == "P2")
    {
        index = 1;
    }
    else if (currentTag == "E3" || currentTag == "P3")
    {
        index = 2;
    }
    else if (currentTag == "ES" || currentTag == "PS")
    {
        index = 1;
    }
    else
    {
        LOG_WARN("Unknown current tag[%s]", currentTag.c_str());
        return NULL;
    }

    // 对位的友方技能即为释放技能英雄自己
    if (skillRelationship == SKILL_FRIEND)
    {
        if (relationship == BATTLE_FOE)
        {
            character = getEnemyByIndex(index);
        }
        else
        {
            character = getPlayerByIndex(index);
        }

        if (character == NULL)
        {
            LOG_WARN("Get position target skill relationship friend failed.");
        }
        return character;
    }

    if (relationship == BATTLE_FOE)
    {
        //获取集火目标
        character = getRandomOneConvergedPlayer();
        if (character == NULL)
        {
            character = getPlayerByIndex(index);
            if (character == NULL)
            {
                if (index == 0)
                {
                    character = getPlayerByIndex(index + 1);
                }
                else if (index == 1 || index == 2)
                {
                    character = getPlayerByIndex(index - 1);
                }

                if (character == NULL)
                {
                    character = getAnyPlayer();
                }
            }
        }
    }
    else
    {
        //获取集火目标
        character = getRandomOneConvergedEnemy();
        if (character == NULL)
        {
            character = getEnemyByIndex(index);
            if (character == NULL)
            {
                if (index == 0)
                {
                    character = getEnemyByIndex(index + 1);
                }
                else if (index == 1 || index == 2)
                {
                    character = getEnemyByIndex(index - 1);
                }

                if (character == NULL)
                {
                    character = getAnyEnemy();
                }
            }
        }
    }
    return character;
}

BattleData* 
BattleManager::getRandomOneTarget(UInt8 relationship, UInt8 skillRelationship)
{
    List<BattleData*> characterList;
    Int32 index = 0;
    if (skillRelationship == SKILL_FRIEND)
    {
        if (relationship == BATTLE_FOE)
        {
            getAliveEnemy(characterList);
        }
        else
        {
            getAlivePlayer(characterList);
        }
    }
    else
    {
        if (relationship == BATTLE_FOE)
        {
            // 获取集火目标
            getConvergedPlayer(characterList);
            if (characterList.size() == 0)
            {
                getAlivePlayer(characterList);
            }
        }
        else
        {
            // 获取集火目标
            getConvergedEnemy(characterList);
            if (characterList.size() == 0)
            {
                getAliveEnemy(characterList);
            }
        }
    }

    if (characterList.size() == 0)
    {
        return NULL;
    }

    index = GameRandom::getUInt32Random(0, characterList.size() - 1);
    List<BattleData*>::Iter* characterIter = characterList.getn(index);
    if (characterIter == NULL)
    {
        return NULL;
    }

    return characterIter->mValue;
}

void 
BattleManager::getRandomTwoTarget(List<BattleData*>& targetList, UInt8 relationship, UInt8 skillRelationship)
{
    List<BattleData*> characterList;
    Int32 index = 0;
    if (skillRelationship == SKILL_FRIEND)
    {
        if (relationship == BATTLE_FOE)
        {
            getAliveEnemy(characterList);
        }
        else
        {
            getAlivePlayer(characterList);
        }
    }
    else
    {
        if (relationship == BATTLE_FOE)
        {
            // 获取集火目标
            getConvergedPlayer(characterList);
            if (characterList.size() == 0)
            {
                getAlivePlayer(characterList);
            }
            else if (characterList.size() == 1)
            {
                List<BattleData*> aliveList;
                getAlivePlayer(aliveList);
                if (aliveList.size() >= 2)
                {
                    List<BattleData*> selectList;
                    List<BattleData*>::Iter* characterFirst = characterList.begin();

                    for (List<BattleData*>::Iter* iter = aliveList.begin();
                        iter != aliveList.end(); iter = aliveList.next(iter))
                    {
                        if (characterFirst == NULL)
                        {
                            break;
                        }

                        if (characterFirst->mValue->mBaseData.mTag != iter->mValue->mBaseData.mTag)
                        {
                            selectList.insertTail(iter->mValue);
                        }
                    }

                    if (selectList.size() != 0)
                    {
                        int random = GameRandom::getUInt32Random(0, selectList.size() - 1);
                        List<BattleData*>::Iter* characterIter = selectList.getn(random);
                        if (characterIter != NULL)
                        {
                            characterList.insertTail(characterIter->mValue);
                        }
                    }
                }
            }
        }
        else
        {
            // 获取集火目标
            getConvergedEnemy(characterList);
            if (characterList.size() == 0)
            {
                getAliveEnemy(characterList);
            }
            else if (characterList.size() == 1)
            {
                List<BattleData*> aliveList;
                getAliveEnemy(aliveList);
                if (aliveList.size() >= 2)
                {
                    List<BattleData*> selectList;
                    List<BattleData*>::Iter* characterFirst = characterList.begin();

                    for (List<BattleData*>::Iter* iter = aliveList.begin();
                        iter != aliveList.end(); iter = aliveList.next(iter))
                    {
                        if (characterFirst == NULL)
                        {
                            break;
                        }

                        if (characterFirst->mValue->mBaseData.mTag != iter->mValue->mBaseData.mTag)
                        {
                            selectList.insertTail(iter->mValue);
                        }
                    }

                    if (selectList.size() != 0)
                    {
                        int random = GameRandom::getUInt32Random(0, selectList.size() - 1);
                        List<BattleData*>::Iter* characterIter = selectList.getn(random);
                        if (characterIter != NULL)
                        {
                            characterList.insertTail(characterIter->mValue);
                        }
                    }
                }
            }
        }
    }

    if (characterList.size() == 0)
    {
        return;
    }

    if (characterList.size() == 3)
    {
        index = GameRandom::getUInt32Random(0, 2);
        int characterListIndex = 0;
        for (List<BattleData*>::Iter* iter = characterList.begin();
            iter != characterList.end(); iter = characterList.next(iter))
        {
            if (characterListIndex != index)
            {
                targetList.insertTail(iter->mValue);
            }
            characterListIndex++;
        }
    }
    else
    {
        for (List<BattleData*>::Iter* iter = characterList.begin();
            iter != characterList.end(); iter = characterList.next(iter))
        {
            targetList.insertTail(iter->mValue);
        }
    }
}

void 
BattleManager::getAllTarget(List<BattleData*>& targetList, UInt8 relationship, UInt8 skillRelationship)
{
    List<BattleData*> characterList;
    if (skillRelationship == SKILL_FRIEND)
    {
        if (relationship == BATTLE_FOE)
        {
            getAliveEnemy(characterList);
        }
        else
        {
            getAlivePlayer(characterList);
        }
    }
    else
    {
        if (relationship == BATTLE_FOE)
        {
            getAlivePlayer(characterList);
        }
        else
        {
            getAliveEnemy(characterList);
        }
    }

    if (characterList.size() == 0)
    {
        return;
    }

    for (List<BattleData*>::Iter* iter = characterList.begin();
        iter != characterList.end(); iter = characterList.next(iter))
    {
        targetList.insertTail(iter->mValue);
    }
}

BattleData* 
BattleManager::getMostBloodOneTarget(UInt8 relationship, UInt8 skillRelationship)
{
    BattleData* character = NULL;
    if (skillRelationship == SKILL_FRIEND)
    {
        if (relationship == BATTLE_FOE)
        {
            character = getMostBloodEnemy();
        }
        else
        {
            character = getMostBloodPlayer();
        }
    }
    else
    {
        List<BattleData*> characterList;
        if (relationship == BATTLE_FOE)
        {
            // 获取集火目标
            getConvergedPlayer(characterList);
            if (characterList.size() == 0)
            {
                character = getMostBloodPlayer();
            }
            else
            {
                Int32 index = GameRandom::getUInt32Random(0, characterList.size() - 1);
                List<BattleData*>::Iter* characterIter = characterList.getn(index);
                if (characterIter != NULL)
                {
                    character = characterIter->mValue;
                }
            }
        }
        else
        {
            // 获取集火目标
            getConvergedEnemy(characterList);
            if (characterList.size() == 0)
            {
                character = getMostBloodEnemy();
            }
            else
            {
                Int32 index = GameRandom::getUInt32Random(0, characterList.size() - 1);
                List<BattleData*>::Iter* characterIter = characterList.getn(index);
                if (characterIter != NULL)
                {
                    character = characterIter->mValue;
                }
            }
        }
    }

    return character;
}

BattleData* 
BattleManager::getLeastBloodOneTarget(UInt8 relationship, UInt8 skillRelationship)
{
    BattleData* character = NULL;
    if (skillRelationship == SKILL_FRIEND)
    {
        if (relationship == BATTLE_FOE)
        {
            character = getLeastBloodEnemy();
        }
        else
        {
            character = getLeastBloodPlayer();
        }
    }
    else
    {
        List<BattleData*> characterList;
        if (relationship == BATTLE_FOE)
        {
            // 获取集火目标
            getConvergedPlayer(characterList);
            if (characterList.size() == 0)
            {
                character = getLeastBloodPlayer();
            }
            else
            {
                Int32 index = GameRandom::getUInt32Random(0, characterList.size() - 1);
                List<BattleData*>::Iter* characterIter = characterList.getn(index);
                if (characterIter != NULL)
                {
                    character = characterIter->mValue;
                }
            }
        }
        else
        {
            // 获取集火目标
            getConvergedEnemy(characterList);
            if (characterList.size() == 0)
            {
                character = getLeastBloodEnemy();
            }
            else
            {
                Int32 index = GameRandom::getUInt32Random(0, characterList.size() - 1);
                List<BattleData*>::Iter* characterIter = characterList.getn(index);
                if (characterIter != NULL)
                {
                    character = characterIter->mValue;
                }
            }
        }
    }

    return character;
}

BattleData* 
BattleManager::getMostBloodEnemy()
{
    BattleData* character = NULL;
    UInt32 mostBlood = 0;
    for (int i = 0; i < 3; i++)
    {
        if (mEnemyArray[i].mBaseData.mHp != 0
            && mEnemyArray[i].mBaseData.mHp > mostBlood)
        {
            mostBlood = mEnemyArray[i].mBaseData.mHp;
            character = &mEnemyArray[i];
        }
    }
    return character;
}

BattleData* 
BattleManager::getMostBloodPlayer()
{
    BattleData* character = NULL;
    UInt32 mostBlood = 0;
    for (int i = 0; i < 3; i++)
    {
        if (mPlayerArray[i].mBaseData.mHp != 0
            && mPlayerArray[i].mBaseData.mHp > mostBlood)
        {
            mostBlood = mPlayerArray[i].mBaseData.mHp;
            character = &mPlayerArray[i];
        }
    }
    return character;
}

BattleData* 
BattleManager::getLeastBloodEnemy()
{
    BattleData* character = NULL;
    UInt32 leastBlood = 0;
    for (int i = 0; i < 3; i++)
    {
        if (mEnemyArray[i].mBaseData.mHp != 0)
        {
            if (leastBlood == 0)
            {
                leastBlood = mEnemyArray[i].mBaseData.mHp;
                character = &mEnemyArray[i];
            }
            else if (mEnemyArray[i].mBaseData.mHp < leastBlood)
            {
                leastBlood = mEnemyArray[i].mBaseData.mHp;
                character = &mEnemyArray[i];
            }
        }
    }
    return character;
}

BattleData* 
BattleManager::getLeastBloodPlayer()
{
    BattleData* character = NULL;
    UInt32 leastBlood = 0;
    for (int i = 0; i < 3; i++)
    {
        if (mPlayerArray[i].mBaseData.mHp != 0)
        {
            if (leastBlood == 0)
            {
                leastBlood = mPlayerArray[i].mBaseData.mHp;
                character = &mPlayerArray[i];
            }
            else if (mPlayerArray[i].mBaseData.mHp < leastBlood)
            {
                leastBlood = mPlayerArray[i].mBaseData.mHp;
                character = &mPlayerArray[i];
            }
        }
    }
    return character;
}

void 
BattleManager::getAliveEnemy(List<BattleData*>& characters)
{
    for (int i = 0; i < 3; i++)
    {
        if (mEnemyArray[i].mBaseData.mHp != 0)
        {
            characters.insertTail(&mEnemyArray[i]);
        }
    }
}

void 
BattleManager::getAlivePlayer(List<BattleData*>& characters)
{
    for (int i = 0; i < 3; i++)
    {
        if (mPlayerArray[i].mBaseData.mHp != 0)
        {
            characters.insertTail(&mPlayerArray[i]);
        }
    }
}

BattleData* 
BattleManager::getAnyPlayer()
{
    for (int i = 0; i < 3; i++)
    {
        if (mPlayerArray[i].mBaseData.mHp != 0)
        {
            return &mPlayerArray[i];
        }
    }
    return NULL;
}

BattleData* 
BattleManager::getPlayerByIndex(UInt32 index)
{
    if (index >= 0 && index < 3
        && mPlayerArray[index].mBaseData.mHp != 0)
    {
        return &mPlayerArray[index];
    }
    return NULL;
}

BattleData* 
BattleManager::getAnyEnemy()
{
    for (int i = 0; i < 3; i++)
    {
        if (mEnemyArray[i].mBaseData.mHp != 0)
        {
            return &mEnemyArray[i];
        }
    }
    return NULL;
}

BattleData* 
BattleManager::getEnemyByIndex(UInt32 index)
{
    if (index >= 0 && index < 3
        && mEnemyArray[index].mBaseData.mHp != 0)
    {
        return &mEnemyArray[index];
    }
    return NULL;
}

void 
BattleManager::getConvergedEnemy(List<BattleData*>& characters)
{
    for (int i = 0; i < 3; i++)
    {
        if (mEnemyArray[i].mBaseData.mHp != 0
            && mEnemyArray[i].mBaseData.mConvergedId != 0)
        {
            characters.insertTail(&mEnemyArray[i]);
        }
    }
}

void 
BattleManager::getConvergedPlayer(List<BattleData*>& characters)
{
    for (int i = 0; i < 3; i++)
    {
        if (mPlayerArray[i].mBaseData.mHp != 0
            && mPlayerArray[i].mBaseData.mConvergedId != 0)
        {
            characters.insertTail(&mPlayerArray[i]);
        }
    }
}

BattleData* 
BattleManager::getRandomOneConvergedEnemy()
{
    List<BattleData*> characterList;
    getConvergedEnemy(characterList);
    if (characterList.size() != 0)
    {
        Int32 index = GameRandom::getUInt32Random(0, characterList.size() - 1);
        List<BattleData*>::Iter* characterIter = characterList.getn(index);
        if (characterIter != NULL)
        {
            return characterIter->mValue;
        }
    }
    return NULL;
}

BattleData* 
BattleManager::getRandomOneConvergedPlayer()
{
    List<BattleData*> characterList;
    getConvergedPlayer(characterList);
    if (characterList.size() != 0)
    {
        Int32 index = GameRandom::getUInt32Random(0, characterList.size() - 1);
        List<BattleData*>::Iter* characterIter = characterList.getn(index);
        if (characterIter != NULL)
        {
            return characterIter->mValue;
        }
    }
    return NULL;
}

void 
BattleManager::checkTowerResultAfterWin()
{
    TollgateTemplate* tollgateTemplate = TOLLGATE_TABLE().get(mTowerId);
    if (tollgateTemplate == NULL)
    {
        return;
    }

    switch (tollgateTemplate->mPassConditionType)
    {
    case TOWER_PASS_CONDITION_WIN:
        {
            mIsTowerPass = true;
        }
    	break;
    case TOWER_PASS_CONDITION_WIN_IN_ROUND:
        {
            if (mRoundNum <= tollgateTemplate->mPassConditionValue)
            {
                mIsTowerPass = true;
            }
        }
        break;
    case TOWER_PASS_CONDITION_DEATH_COUNT:
        {
            if (mDeathCount <= tollgateTemplate->mPassConditionValue)
            {
                mIsTowerPass = true;
            }
        }
        break;
    case TOWER_PASS_CONDITION_REMAIN_HP:
        {
            UInt32 remainHp = 0;
            UInt32 totalHp = 0;

            for (int i = 0; i < 3; i++)
            {
                if (mPlayerArray[i].mBaseData.mGuidId != 0
                    && mPlayerArray[i].mBaseData.mHp != 0)
                {
                    remainHp += mPlayerArray[i].mBaseData.mHp;
                }
            }
            for (List<BattleData>::Iter* iter = mPlayerBench.begin();
                iter != mPlayerBench.end(); iter = mPlayerBench.next(iter))
            {
                if (iter->mValue.mBaseData.mGuidId != 0
                    && iter->mValue.mBaseData.mHp != 0)
                {
                    remainHp += iter->mValue.mBaseData.mHp;
                }
            }


            for (int i = 0; i < 3; i++)
            {
                if (mPlayerArray[i].mBaseData.mGuidId != 0
                    && mPlayerArray[i].mBufferData.mHpUpper != 0)
                {
                    totalHp += mPlayerArray[i].mBufferData.mHpUpper;
                }
            }
            for (List<BattleData>::Iter* iter = mPlayerBench.begin();
                iter != mPlayerBench.end(); iter = mPlayerBench.next(iter))
            {
                if (iter->mValue.mBaseData.mGuidId != 0
                    && iter->mValue.mBufferData.mHpUpper != 0)
                {
                    totalHp += iter->mValue.mBufferData.mHpUpper;
                }
            }

            float remainPercent = (float)remainHp / (float)totalHp;
            if (remainPercent >= ((float)tollgateTemplate->mPassConditionValue / 100.0f))
            {
                mIsTowerPass = true;
            }
        }
        break;
    default:
        break;
    }
}

void 
BattleManager::showResult(UInt32 result)
{
    //SkillPanel.enabled = false;

    //ResultPanel.enabled = true;
    // for local test +
    //mRestartButton.SetEnable(true);
    //Transform panel = ResultPanel.transform.Find("ResultText");
    //if (panel != null)
    //{
    //    UILabel label = panel.GetComponent<UILabel>();
    //    if (result == 0)
    //    {
    //        label.text = "Lose!";
    //    }
    //    else
    //    {
    //        label.text = "Win!";
    //    }
    //}

    UInt32 starCount = 0;
    if (result == 0)
    {
        //mResultShow.SetResultText("失败");
        //mResultShow.SetStarNum(0);
        starCount = 0;
    }
    else
    {
        switch (mDeathCount)
        {
            case 0:
                {
                    //mResultShow.SetResultText("完胜");
                    //mResultShow.SetStarNum(3);
                    starCount = 3;
                }
                break;
            case 1:
                {
                    //mResultShow.SetResultText("胜利");
                    //mResultShow.SetStarNum(2);
                    starCount = 2;
                }
                break;
            default:
                {
                    //mResultShow.SetResultText("险胜");
                    //mResultShow.SetStarNum(1);
                    starCount = 1;
                }
                break;
        }
    }

    if (mIsVerifyBattle)
    {
        bool victory = (result == 0) ? false : true;
        if (victory == mBattleVerifyData.mIsVictory
            && mDeathCount == mBattleVerifyData.mDeathCount)
        {
            mIsVerifyPass = true;
        }
        else
        {
            mIsVerifyPass = false;
        }

        if (mIsTower && result != 0)
        {
            checkTowerResultAfterWin();
        }
    }
    else
    {
        if (mSimulationData != NULL)
        {
            mSimulationData->mStarCount = starCount;
        }
    }

    //MCameraCtrl.PlayAnimation();

    //if (BattleEnd != null)
    //{
    //    BattleEnd();
    //}

    //// for local test -
    //if (mIsArenaBattle)
    //{
    //    ShowArenaBattleResult();
    //}
    //else
    //{
    //    TollgateBattleResultReq req = new TollgateBattleResultReq();
    //    req.mTollgateId = mBattleTollgateId;
    //    req.mIsVictory = (result == 0) ? false : true;
    //    req.mDeathCount = (Byte)mDeathCount;
    //    GlobalObjects.GetSingle().mNetWorkManager.SendMessage(req);
    //}

    mEnemyBench.clear();
    mPlayerBench.clear();
    mTargetList.clear();

    mIsOppRobot = false;
    mSimulationData = NULL;
    mLastEndTag = "";
    mAttackNum = 0;
    mRoundNum = 0;
    mPlayerPetSkillUsed = false;
    mEnemyPetSkillUsed = false;
    mDeathCount = 0;
    mMySuperSkillUsed = false;
    mOppSuperSkillUsed = false;
    mPlayerUserIndex = 0;
    mEnemyUserIndex = 0;
    mIsSuperSkill = false;
    mIsCommonSkill = false;
    mIsAutoSkill = false;
    mIsEnterSkill = false;
    mAttackCount = 0;
    mAttackMultiIndex = 0;
}

void 
BattleManager::dead(BattleData* selfData)
{
    if (selfData == NULL)
    {
        LOG_WARN("Dead selfData is null.");
        return;
    }
    if (selfData->mBaseData.mCharacterType != BATTLE_PET)
    {
        if (selfData->mBaseData.mHp == 0)
        {
            delAllBuffer(*selfData);
            // 播放死亡特效
            //if (mDeadEffect.Length != 0)
            //{
            //    GameObject effectObject;
            //    Vector3 effectPosition = new Vector3(transform.position.x,
            //        transform.position.y, transform.position.z);
            //    //effectPosition.y += 2.0f;

            //    String deadEffect = "Effects/" + mDeadEffect;

            //    try
            //    {
            //        effectObject = GameObject.Instantiate(Resources.Load(deadEffect), effectPosition, Quaternion.identity) as GameObject;
            //    }
            //    catch (System.Exception ex)
            //    {
            //        Debug.LogError("Failed to instantiate effect , effect name: " + deadEffect + "\n" + ex.Message);
            //        return;
            //    }

            //    if (effectObject == null)
            //    {
            //        Debug.LogWarning("OnHit get effect[" + deadEffect + "] object failed.");
            //        return;
            //    }

            //    EffectDestroySelf effect = effectObject.GetComponent<EffectDestroySelf>();
            //    if (effect == null)
            //    {
            //        Debug.LogWarning("OnHit get effect[" + deadEffect + "] EffectDestroySelf failed.");
            //        return;
            //    }
            //    effect.Play(false);
            //}
            // 增加本方死亡人数
            if (selfData->mBaseData.mRelationship == BATTLE_FRIEND)
            {
                addMyselfDeathCount();
            }
            //gameObject.SetActive(false);
            // 在替补上阵前将当前的roundend状态赋给新上阵英雄

            // 设置技能图标为不可用
            //mLordMain.SetSkillIconForDeath(mTag);
        }
    }

}

void 
BattleManager::useSuperSkill(BattleData* userData, BattleSkillItem skillItem)
{
    mIsSuperSkill = true;
    attack(userData, skillItem);
}

void 
BattleManager::useCommonSkill(BattleData* userData, BattleSkillItem skillItem)
{
    mIsCommonSkill = true;
    attack(userData, skillItem);
}

void 
BattleManager::useAutoSkill(BattleData* userData, BattleSkillItem skillItem)
{
    mIsAutoSkill = true;
    attack(userData, skillItem);
}

void 
BattleManager::useEnterSkill(BattleData* userData, BattleSkillItem skillItem)
{
    mIsEnterSkill = true;
    attack(userData, skillItem);
}

void 
BattleManager::addAnger(BattleData* userData)
{
    if (mIsCommonSkill)
    {
        if (userData == NULL)
        {
            return;
        }
        // 封印
        if (userData->mBaseData.mSealId != 0)
        {
            //Debug.Log("Seal can't get anger.");
            return;
        }
        userData->mBaseData.mAnger += userData->mSkillData.mCommonSkill.mAngerInc;
        if (userData->mBaseData.mAnger > userData->mExtendData.mAngerUpper)
        {
            userData->mBaseData.mAnger = userData->mExtendData.mAngerUpper;
        }
    }
}

bool 
BattleManager::checkAutoSkill(BattleData* userData)
{
    if (userData != NULL && userData->mSkillData.mAutoSkill.mTemplateId != 0)
    {
        if (userData->mBaseData.mAnger >= userData->mSkillData.mAutoSkill.mTriggerAnger 
            && userData->mSkillData.mAutoSkill.mTriggerAnger != 0)
        {
            userData->mBaseData.mAnger -= userData->mSkillData.mAutoSkill.mTriggerAnger;
            return true;
        }
    }
    //if (mIsCommonSkill)
    //{
    //    if (mAutoSkill != null && mAutoSkill.mTemplateId != 0)
    //    {
    //        mAnger += mAngerInc;
    //        if (mAnger > mAngerUpper)
    //        {
    //            mAnger = mAngerUpper;
    //        }
    //        if (mAnger >= mTriggerAnger && mTriggerAnger != 0)
    //        {
    //            mAnger -= mTriggerAnger;
    //            mIsAutoSkill = true;
    //            mIsCommonSkill = false;
    //            mIsEnterSkill = false;
    //            mIsSuperSkill = false;
    //            //Debug.Log(mTag + " auto skill used.");
    //            Attack(mAutoSkill);
    //            return true;
    //        }
    //    }
    //}
    return false;
}

void 
BattleManager::attack(BattleData* userData, BattleSkillItem skill)
{
    if (userData == NULL)
    {
        LOG_WARN("Attack userData is null.");
        attackEnd(userData);
        return;
    }
    //mIdleAction = "Idle";
    ////mBeHitAction = "Behit";
    if (mIsSuperSkill)
    {
        //mMoveFrontAction = "Move_Forward_Super";
        //mMoveBackAction = "Move_Backward_Super";
        mAttackAction = "Super_Attack";
    }
    else
    {
        //mMoveFrontAction = "Move_Forward";
        //mMoveBackAction = "Move_Backward";
        mAttackAction = "Attack";
        if (mIsAutoSkill)
        {
            //mMoveFrontAction = "Move_Forward_Auto";
            //mMoveBackAction = "Move_Backward_Auto";
            mAttackAction = "Auto_Attack";
        }
        else if (mIsEnterSkill)
        {
            mAttackAction = "Appearance";
        }
    }

    //mMoveFrontTime = mActor.GetActionLength(mMoveFrontAction);
    //mMoveBackTime = mActor.GetActionLength(mMoveBackAction);
    //Debug.LogWarning("Move Frone time: " + mMoveFrontTime + ", move back time: " + mMoveBackTime);

    if (skill.mTemplateId == 0)
    {
        LOG_WARN("Attack use skillitem is null. user tag[%s].", userData->mBaseData.mTag.c_str());
        attackEnd(userData);
        return;
    }

    mCurrentSkill = skill;
    // 初始化技能相关部分
    // 通过模板读取相应数据填充
    SkillTemplate* skillTemplate = SKILL_TABLE().get(skill.mTemplateId);
    if (skillTemplate == NULL)
    {
        LOG_WARN("Get skillTemplate failed templateId[%u]", skill.mTemplateId);
        attackEnd(userData);
        return;
    }
    //mCombatType = (CombatType)skillTemplate.mCombatType;
    //mSkillRelationship = (SkillRelationship)skillTemplate.mRelationship;
    //mAngerInc = skillTemplate.mIncAnger;

    //mCurePercent = skillTemplate.mCurePercent;
    //mMultiAttack = skillTemplate.mMultiAttack;
    //mEffectType = (EffectType)skillTemplate.mAttackEffectType;
    //mCameraPos = skillTemplate.mCameraPos;
    //mMultiNum = skillTemplate.mMultiNum;
    //mDeadEffect = skillTemplate.mDeadEffect;

    if (!mIsSuperSkill && !mIsEnterSkill && userData->mBaseData.mCharacterType != BATTLE_PET)
    {
        if (mAttackCount < 1 && !updateBufferBeforeAttack(*userData) && userData->mBaseData.mHp == 0)
        {
            deathForBufferBeforeAttack(userData);
            return;
        }

        // 原判断麻痹状态 sec注释
        //if (userData->mBaseData.mParalysisId != 0)
        //{
        //    BufferData* bufferData = getBufferById(userData->mBaseData.mParalysisId, *userData);
        //    if (bufferData != NULL)
        //    {
        //        UInt32 paralysisRandom = GameRandom::getUInt32Random(1, 100);
        //        if (paralysisRandom <= bufferData->mEffectValue)
        //        {
        //            //Debug.Log("Paralysis buffer effect.");
        //            attackEnd(userData);
        //            return;
        //        }
        //    }
        //}
    }

    getTarget(mTargetList, skill, userData->mBaseData.mTag, userData->mBaseData.mRelationship);
    if (mTargetList.size() == 0)
    {
        // 如果没有找到目标则结束当前行动，目标可能在多次攻击中被打死，结束行动后看是否有替补可以出场
        LOG_WARN("Attack get target list empty. user tag[%s].", userData->mBaseData.mTag.c_str());
        if (mCurrentSkill.mCombatType == BATTLE_CLOSE)
        {
            //if (mMoveBackAction.Length != 0 && mMoveBackTime > 0.0f)
            //{
            //    mMoving = true;
            //    mMovePos = mInitPos;
            //    mMoveSpeed = Vector3.Distance(transform.position, mMovePos) / mMoveBackTime;
            //    Vector3 lookAtPos = (transform.position - mMovePos) * 2 + mMovePos;
            //    transform.LookAt(lookAtPos);
            //    PlayAction(mMoveBackAction);
            //    mActor.Move(transform.position, mMovePos, mMoveBackAction);
            //}
            //else
            //{
            //    mMoveBackActionEnd = true;
            //    mMovePosEnd = true;
            //    transform.position = mInitPos;
            //    transform.rotation = mInitDir;
            //}
            attackEnd(userData);
        }
        else
        {
            attackEnd(userData);
        }
        return;
    }

    LOG_INFO("Battle test Attack user %s ", userData->mBaseData.mTag.c_str());
    for (List<BattleData*>::Iter* iter = mTargetList.begin();
        iter != mTargetList.end(); iter = mTargetList.next(iter))
    {
        if (iter->mValue != NULL)
        {
            LOG_INFO("Battle test Attack target %s", iter->mValue->mBaseData.mTag.c_str());
        }
    }
    // 设置普通攻击相机位置
    //if (mIsCommonSkill)
    //{
    //    if (userData->mBaseData.mCharacterType == BATTLE_PET && mCameraPos != 0)
    //    {
    //        mLordMain.MCameraCtrl.PlayAnimation((int)mCameraPos, mRelationship);
    //        mLordMain.HideCharacterByRelationship(mRelationship);
    //    }
    //    else
    //    {
    //        mLordMain.MCameraCtrl.PlayAnimation();
    //    }
    //}

    // 设置自动攻击相机
    //if (mIsAutoSkill)
    //{
    //    mLordMain.MCameraCtrl.PlayAnimation();
    //}

    // 设置大招相机位置
    //if (mIsSuperSkill)
    //{
    //    bool isAll = false;
    //    if ((SkillAttackType)mCurrentSkill.mSkillAttackType == SkillAttackType.ALL
    //        || (SkillAttackType)mCurrentSkill.mSkillAttackType == SkillAttackType.RANDOM_TWO)
    //    {
    //        isAll = true;
    //    }
    //    //else if ((SkillAttackType)mCurrentSkill.mSkillAttackType == SkillAttackType.SPECIFY_TYPE)
    //    //{
    //    //    if (mTargetList.Count > 1)
    //    //    {
    //    //        isAll = true;
    //    //    }
    //    //}

    //    if (!isAll)
    //    {
    //        LynxCharacter character = mTargetList[0].GetComponent<LynxCharacter>();
    //        if (character == null)
    //        {
    //            Debug.Log("Attack get target LynxCharacter failed when set camera.");
    //            return;
    //        }
    //        mLordMain.MCameraCtrl.PlayAnimation(mTag, character.Tag, mSkillRelationship, false);
    //    }
    //    else
    //    {
    //        mLordMain.MCameraCtrl.PlayAnimation(mTag, "", mSkillRelationship, true);
    //    }
    //    //mLordMain.SkillPanel.enabled = false;
    //}

    // 设置出场技相机
    //if (mIsEnterSkill)
    //{
    //    mLordMain.MCameraCtrl.PlayAnimation(mTag, "", mSkillRelationship, true);
    //}

    // 设置相机位置
    //if (mIsSuperSkill && mRelationship == Relationship.FRIEND)
    //{
    //    LynxCharacter character = mTargetList[0].GetComponent<LynxCharacter>();
    //    if (character == null)
    //    {
    //        Debug.Log("Attack get target LynxCharacter failed when set camera.");
    //        return;
    //    }
    //    //mLordMain.MCameraCtrl.LookAt(Tag, character.Tag);
    //    //Debug.Log ("attack : " + user.Tag + ", defence : " + target.Tag);
    //    mLordMain.SkillPanel.enabled = false;
    //}

    if (mCurrentSkill.mMultiNum > 0)
    {
        mAttackMultiIndex++;
        if (mAttackMultiIndex > 1)
        {
            char buf[128];
            snprintf(buf, sizeof(buf), "%u", mAttackMultiIndex);
            mAttackAction = mAttackAction + buf;
        }

        if (mAttackMultiIndex == mCurrentSkill.mMultiNum)
        {
            mAttackMultiIndex = 0;
        }
    }
    else
    {
        mAttackMultiIndex = 0;
    }

    // 获得动作打击点数量
    mCurrentSkill.mHitPointCount = getAttackHitPoint(userData);

    // 远程类型则直接在原地释放，进程技能如果是非单体则移动到对方中间角色前方释放
    switch (mCurrentSkill.mCombatType)
    {
    case BATTLE_CLOSE:
        {
            // 查看是友方技能还是敌方技能，如果是友方近战则在原地释放
            if (mCurrentSkill.mSkillRelationship == SKILL_FRIEND)
            {
                ////mMovePos = transform.position;
                //mMoveFrontActionEnd = true;
                //mMovePosEnd = true;

                for (int i = 1; i <= mCurrentSkill.mHitPointCount; i++)
                {
                    onHit(userData, i, mCurrentSkill.mHitPointCount);
                }
                onAttackActionEnd(userData);
                return;
            }

            //if ((SkillAttackType)mCurrentSkill.mSkillAttackType == SkillAttackType.ALL
            //    || (SkillAttackType)mCurrentSkill.mSkillAttackType == SkillAttackType.RANDOM_TWO)
            //{
            //    if (mRelationship == Relationship.FOE)
            //    {
            //        GameObject playerPosM = GameObject.Find("PlayerActor_M");
            //        if (playerPosM == null)
            //        {
            //            Debug.LogWarning("Can't find player pos middle!");
            //            return;
            //        }
            //        mMovePos = playerPosM.transform.position;
            //        mMovePos.z -= 2f;
            //    }
            //    else
            //    {
            //        GameObject enemyPosM = GameObject.Find("EnemyActor_M");
            //        if (enemyPosM == null)
            //        {
            //            Debug.LogWarning("Can't find enemy pos middle!");
            //            return;
            //        }
            //        mMovePos = enemyPosM.transform.position;
            //        mMovePos.z += 2f;
            //    }
            //}
            ////else if ((SkillAttackType)mCurrentSkill.mSkillAttackType == SkillAttackType.SPECIFY_TYPE)
            ////{
            ////    if (mTargetList.Count > 1)
            ////    {
            ////        if (mRelationship == Relationship.FOE)
            ////        {
            ////            GameObject playerPosM = GameObject.Find("PlayerActor_M");
            ////            if (playerPosM == null)
            ////            {
            ////                Debug.LogWarning("Can't find player pos middle!");
            ////                return;
            ////            }
            ////            mMovePos = playerPosM.transform.position;
            ////            mMovePos.z -= 2f;
            ////        }
            ////        else
            ////        {
            ////            GameObject enemyPosM = GameObject.Find("EnemyActor_M");
            ////            if (enemyPosM == null)
            ////            {
            ////                Debug.LogWarning("Can't find enemy pos middle!");
            ////                return;
            ////            }
            ////            mMovePos = enemyPosM.transform.position;
            ////            mMovePos.z += 2f;
            ////        }
            ////    }
            ////    else
            ////    {
            ////        Vector3 tp = mTargetList[0].transform.position - transform.position;
            ////        tp.Normalize();
            ////        mMovePos = mTargetList[0].transform.position - tp * 2f;
            ////        //if (mIsAutoSkill || (mMultiAttack > 1 && mAttackCount >= 1))
            ////        if (mMultiAttack > 1 && mAttackCount >= 1)
            ////        {
            ////            Vector3 atp = mTargetList[0].transform.position - mInitPos;
            ////            atp.Normalize();
            ////            mMovePos = mTargetList[0].transform.position - atp * 2f;
            ////            //mMovePos = mTargetList[0].transform.position;
            ////            //if (mRelationship == Relationship.FOE)
            ////            //{
            ////            //    mMovePos.z -= 2f;
            ////            //}
            ////            //else
            ////            //{
            ////            //    mMovePos.z += 2f;
            ////            //}
            ////        }
            ////    }
            ////}
            //else
            //{
            //    Vector3 tp = mTargetList[0].transform.position - transform.position;
            //    tp.Normalize();
            //    mMovePos = mTargetList[0].transform.position - tp * 2f;
            //    //if (mIsAutoSkill || (mMultiAttack > 1 && mAttackCount >= 1))
            //    if (mMultiAttack > 1 && mAttackCount >= 1)
            //    {
            //        Vector3 atp = mTargetList[0].transform.position - mInitPos;
            //        atp.Normalize();
            //        mMovePos = mTargetList[0].transform.position - atp * 2f;
            //        //mMovePos = mTargetList[0].transform.position;
            //        //if (mRelationship == Relationship.FOE)
            //        //{
            //        //    mMovePos.z -= 2f;
            //        //}
            //        //else
            //        //{
            //        //    mMovePos.z += 2f;
            //        //}
            //    }
            //}

            // 自动技能，多段动作，多次攻击则直接瞬移过去
            //if (mIsAutoSkill || (mMultiAttack > 1 && mAttackCount >= 1))
            if (mCurrentSkill.mMultiAttack > 1 && mAttackCount >= 1)
            {
                //mMoveFrontActionEnd = true;
                //mMovePosEnd = true;
                //transform.position = mMovePos;
                //transform.LookAt(mMovePos);
                for (int i = 1; i <= mCurrentSkill.mHitPointCount; i++)
                {
                    onHit(userData, i, mCurrentSkill.mHitPointCount);
                }
                onAttackActionEnd(userData);
                return;
            }

            // 播放前冲动作
            //if (mMoveFrontAction.Length != 0 && mMoveFrontTime > 0.0f)
            //{
            //    mMoving = true;
            //    mMoveSpeed = Vector3.Distance(transform.position, mMovePos) / mMoveFrontTime;
            //    transform.LookAt(mMovePos);
            //    PlayAction(mMoveFrontAction);
            //    mActor.Move(transform.position, mMovePos, mMoveFrontAction);
            //}
            //else
            //{
            //    mMoveFrontActionEnd = true;
            //    mMovePosEnd = true;
            //    transform.position = mMovePos;
            //    transform.LookAt(mMovePos);
            //    //transform.rotation = mInitDir;
            //}
            for (int i = 1; i <= mCurrentSkill.mHitPointCount; i++)
            {
                onHit(userData, i, mCurrentSkill.mHitPointCount);
            }
            onAttackActionEnd(userData);
        }
        break;
    case BATTLE_FAR:
        {
            //if ((SkillAttackType)mCurrentSkill.mSkillAttackType == SkillAttackType.ALL 
            //    || (SkillAttackType)mCurrentSkill.mSkillAttackType == SkillAttackType.RANDOM_TWO)
            //{
            //    if (mRelationship == Relationship.FOE)
            //    {
            //        GameObject playerPosM = GameObject.Find("PlayerActor_M");
            //        if (playerPosM == null)
            //        {
            //            Debug.LogWarning("Can't find player pos middle!");
            //            return;
            //        }
            //        mLookAtPos = playerPosM.transform.position;
            //    }
            //    else
            //    {
            //        GameObject enemyPosM = GameObject.Find("EnemyActor_M");
            //        if (enemyPosM == null)
            //        {
            //            Debug.LogWarning("Can't find enemy pos middle!");
            //            return;
            //        }
            //        mLookAtPos = enemyPosM.transform.position;
            //    }
            //}
            ////else if ((SkillAttackType)mCurrentSkill.mSkillAttackType == SkillAttackType.SPECIFY_TYPE)
            ////{
            ////    if (mTargetList.Count > 1)
            ////    {
            ////        if (mRelationship == Relationship.FOE)
            ////        {
            ////            GameObject playerPosM = GameObject.Find("PlayerActor_M");
            ////            if (playerPosM == null)
            ////            {
            ////                Debug.LogWarning("Can't find player pos middle!");
            ////                return;
            ////            }
            ////            mLookAtPos = playerPosM.transform.position;
            ////        }
            ////        else
            ////        {
            ////            GameObject enemyPosM = GameObject.Find("EnemyActor_M");
            ////            if (enemyPosM == null)
            ////            {
            ////                Debug.LogWarning("Can't find enemy pos middle!");
            ////                return;
            ////            }
            ////            mLookAtPos = enemyPosM.transform.position;
            ////        }
            ////    }
            ////    else
            ////    {
            ////        mLookAtPos = mTargetList[0].transform.position;
            ////    }
            ////}
            //else
            //{
            //    mLookAtPos = mTargetList[0].transform.position;
            //}

            //if (mAttackAction.Length != 0)
            //{
            //    transform.LookAt(mLookAtPos);
            //    PlayAction(mAttackAction);
            //}
            for (int i = 1; i <= mCurrentSkill.mHitPointCount; i++)
            {
                onHit(userData, i, mCurrentSkill.mHitPointCount);
            }
            onAttackActionEnd(userData);
        }
        break;
    default:
        LOG_WARN("Unknown combat type[%u].", mCurrentSkill.mCombatType);
        break;
    }
}

void 
BattleManager::onHit(BattleData* selfData, UInt32 index, UInt32 count)
{
    if (mTargetList.size() == 0)
    {
        LOG_WARN("Target object is null.");
        return;
    }

    bool hitEnd = false;
    if (index == count)
    {
        hitEnd = true;
    }

    switch (mCurrentSkill.mCombatType)
    {
    case BATTLE_CLOSE:
        {
            for (List<BattleData*>::Iter* iter = mTargetList.begin();
                iter != mTargetList.end(); iter = mTargetList.next(iter))
            {
                if (iter->mValue != NULL)
                {
                    hit(selfData, iter->mValue, hitEnd);
                }
            }
        }
        break;
    case BATTLE_FAR:
        {
            onFlyerHit(selfData, index, count, hitEnd);
        }
        break;
    default:
        LOG_WARN("Unknown combat type[%u].", mCurrentSkill.mCombatType);
        break;
    }

    addSelfSkillBuffer(selfData);
    updateBufferOnHit(*selfData);
}

void 
BattleManager::onFlyerHit(BattleData* selfData, UInt32 index, UInt32 count, bool isEnd)
{
    if (selfData == NULL)
    {
        LOG_WARN("OnFlyerHit selfData is null.");
        return;
    }

    if (index == count)
    {
        //GameObject.Destroy(self);
    }

    bool hitEnd = false;
    if (index == count && isEnd)
    {
        hitEnd = true;
    }

    for (List<BattleData*>::Iter* iter = mTargetList.begin();
        iter != mTargetList.end(); iter = mTargetList.next(iter))
    {
        if (iter->mValue != NULL)
        {
            hit(selfData, iter->mValue, hitEnd);
        }
    }

}

void 
BattleManager::hit(BattleData* selfData, BattleData* target, bool isEnd)
{
    if (selfData == NULL)
    {
        LOG_WARN("Hit selfData is null.");
        return;
    }

    if (target == NULL)
    {
        LOG_WARN("Hit target is null.");
        return;
    }

    //LynxHUDText hudText = target.GetComponent<LynxHUDText>();
    //if (hudText == null)
    //{
    //    Debug.LogWarning("Hit get target LynxHUDText failed.");
    //    return;
    //}
    // 计算攻击的数值
    //if (mCurrentSkill == null)
    //{
    //    Debug.LogWarning("Get attack value current skill is null.");
    //    return;
    //}

    //LynxCharacter character = target.GetComponent<LynxCharacter>();
    //if (character == null)
    //{
    //    Debug.LogWarning("Hit get target LynxCharacter failed.");
    //    return;
    //}

    if (target->mBaseData.mCharacterType == BATTLE_PET)
    {
        return;
    }

    //LynxBuffer characterBuffer = target.GetComponent<LynxBuffer>();
    //if (characterBuffer == null)
    //{
    //    Debug.LogWarning("Hit get target LynxBuffer failed.");
    //    return;
    //}

    //Actor actor = target.GetComponentInChildren<Actor>();
    //if (actor == null)
    //{
    //    Debug.LogWarning("Hit get target Actor failed.");
    //    return;
    //}

    SkillTemplate* skillTemplate = SKILL_TABLE().get(mCurrentSkill.mTemplateId);
    if (skillTemplate == NULL)
    {
        LOG_WARN("Get skillTemplate failed templateId[%u]", mCurrentSkill.mTemplateId);
        return;
    }

    if (!mIsSuperSkill && !mIsEnterSkill && selfData->mBaseData.mCharacterType != BATTLE_PET)
    {
        // 致盲
        if (selfData->mBaseData.mBlindId != 0)
        {
            BufferData* bufferData = getBufferById(selfData->mBaseData.mBlindId, *selfData);
            if (bufferData != NULL)
            {
                UInt32 blindRandom = GameRandom::getUInt32Random(1, 100);
				//buffer字段更改，sec注释
                //if (blindRandom <= bufferData->mEffectValue)
                //{
                //    //Debug.Log("Blind buffer effect.");
                //    //hudText.Miss();
                //    // 在最后一个打击点判断是否死亡
                //    if (isEnd)
                //    {
                //        if (target->mBaseData.mHp == 0)
                //        {
                //            dead(target);
                //        }
                //    }
                //    return;
                //}
            }
        }
    }

    float criticalFactor = 1.0f;
    bool isPhy = false;
    bool isMag = false;
    float attack = 0;
    if (skillTemplate->mPhysicsAttack > 0)
    {
        attack = ((float)selfData->mBufferData.mAttack * (float)skillTemplate->mPhysicsAttack / 100.0f - (float)target->mBufferData.mPhysicsDefense) *
            criticalFactor * (1.0f - (float)target->mBufferData.mAvoidDamage / 1000.0f);
        isPhy = true;
    }
    else
    {
        attack = ((float)selfData->mBufferData.mAttack * (float)skillTemplate->mMagicAttack / 100.0f - (float)target->mBufferData.mMagicDefense) *
            criticalFactor * (1.0f - (float)target->mBufferData.mAvoidDamage / 1000.0f);
        isMag = true;
    }
    if (attack < 1.0f)
    {
        attack = 0.0f;
    }

    // 计算暴击或者闪避
    // 目前暂时按照随机数进行计算
    float crit = (float)selfData->mBufferData.mCritical - (float)target->mBufferData.mRescrit + 50;
    if (crit < 1.0f)
    {
        crit = 0.0f;
    }
    float dodge = (float)target->mBufferData.mDodge - (float)selfData->mBufferData.mHit + 50;
    if (dodge < 1.0f)
    {
        dodge = 0.0f;
    }
    float randomValue = GameRandom::getFloatRandom(1.0f, 1000.0f);

    //UInt32 crit = 25;
    //UInt32 dodge = 10;
    //UInt32 randomValue = (UInt32)UnityEngine.Random.Range(1, 100);
    if (mCurrentSkill.mSkillRelationship == SKILL_FOE)
    {
        bool isCrit = false;
        if (randomValue <= dodge)
        {
            //闪避
            //hudText.Dodge();
            triggerPassivityOnDodge(*target, *selfData);
            //updateBufferBeHit(*selfData, attack, *target);
            //characterBuffer.TriggerPassivityOnDodge(transform.gameObject);
            //characterBuffer.UpdateBufferBeHit(transform.gameObject, attack);
            // 在最后一个打击点判断是否死亡
            if (isEnd)
            {
                if (target->mBaseData.mHp == 0)
                {
                    dead(target);
                }
            }
            return;
        }
        else if (randomValue > dodge && randomValue <= (dodge + crit))
        {
            isCrit = true;
            triggerPassivityOnCritical(*selfData, *selfData);
            updateBufferOnHit(*selfData);
            //mBuffer.TriggerPassivityOnCritical(transform.gameObject);
            //mBuffer.UpdateBufferOnHit();
            criticalFactor = 2.0f + (float)selfData->mBufferData.mCriticalDamage / 1000.0f;
            if (isPhy)
            {
                attack = ((float)selfData->mBufferData.mAttack * (float)skillTemplate->mPhysicsAttack / 100.0f - (float)target->mBufferData.mPhysicsDefense) *
                    criticalFactor * (1.0f - (float)target->mBufferData.mAvoidDamage / 1000.0f);
            }
            if (isMag)
            {
                attack = ((float)selfData->mBufferData.mAttack * (float)skillTemplate->mMagicAttack / 100.0f - (float)target->mBufferData.mMagicDefense) *
                    criticalFactor * (1.0f - (float)target->mBufferData.mAvoidDamage / 1000.0f);
            }
            if (attack < 1.0f)
            {
                attack = 0.0f;
            }
        }

        // 为目标添加buffer后由于有立即生效的buffer，所以需要对数值进行重新计算
        addTargetSkillBuffer(selfData, target);
        if (isPhy)
        {
            attack = ((float)selfData->mBufferData.mAttack * (float)skillTemplate->mPhysicsAttack / 100.0f - (float)target->mBufferData.mPhysicsDefense) *
                criticalFactor * (1.0f - (float)target->mBufferData.mAvoidDamage / 1000.0f);
            //attack = skillTemplate.mPhysicsAttack;
        }
        if (isMag)
        {
            attack = ((float)selfData->mBufferData.mAttack * (float)skillTemplate->mMagicAttack / 100.0f - (float)target->mBufferData.mMagicDefense) *
                criticalFactor * (1.0f - (float)target->mBufferData.mAvoidDamage / 1000.0f);
            //attack = skillTemplate.mMagicAttack;
        }
        if (attack < 1.0f)
        {
            attack = 0.0f;
        }

        // 看目标是否无敌，物理无敌还是魔法无敌
        if ((isPhy && target->mBaseData.mPhyInvincibleId != 0) ||
            (isMag && target->mBaseData.mMagInvincibleId != 0))
        {
            //hudText.DelHp(0);
        }
        else
        {
            //if (isCrit)
            //{
            //    hudText.Critical(false, attack);
            //}
            //else
            //{
            //    hudText.DelHp(attack);
            //}

            mMyDps += attack;

            if (target->mBaseData.mHp <= attack)
            {
                target->mBaseData.mHp = 0;
                triggerPassivityOnKillInOnce(*selfData, *selfData);
                updateBufferOnHit(*selfData);
                //mBuffer.TriggerPassivityOnKillInOnce(transform.gameObject);
                //mBuffer.UpdateBufferOnHit();
            }
            else
            {
                target->mBaseData.mHp = target->mBaseData.mHp - (UInt32)attack;
            }

            // 吸血
            UInt32 suckBlood = (UInt32)((float)attack * (skillTemplate->mSuckPercent / 100.0f + (float)selfData->mBufferData.mSuck / 1000.0f));
            if (suckBlood != 0)
            {
                //mHudText.AddHp(suckBlood);
                if (selfData->mBaseData.mHp + suckBlood >= selfData->mBufferData.mHpUpper)
                {
                    selfData->mBaseData.mHp = selfData->mBufferData.mHpUpper;
                }
                else
                {
                    selfData->mBaseData.mHp = selfData->mBaseData.mHp + suckBlood;
                }
            }
        }

        triggerPassivityOnBeHit(*target, *selfData);
        updateBufferBeHit(*selfData, attack, *target);
        //characterBuffer.TriggerPassivityOnBeHit(transform.gameObject);
        //characterBuffer.UpdateBufferBeHit(transform.gameObject, attack);
    }
    else
    {
        //友方技能没有闪避暴击
        // 读取模板治疗量百分比计算
        UInt32 cureValue = (UInt32)((float)selfData->mBufferData.mAttack * (skillTemplate->mCurePercent / 100.0f) * (1.0f + selfData->mBufferData.mCure / 1000.0f));
        // 为目标添加buffer后由于有立即生效的buffer，所以需要对数值进行重新计算
        addTargetSkillBuffer(selfData, target);
        cureValue = (UInt32)((float)selfData->mBufferData.mAttack * (skillTemplate->mCurePercent / 100.0f) * (1.0f + selfData->mBufferData.mCure / 1000.0f));
        //cureValue = skillTemplate.mPhysicsAttack;

        if (cureValue != 0)
        {
            //hudText.AddHp(cureValue);
            if (target->mBaseData.mHp + cureValue >= target->mBufferData.mHpUpper)
            {
                target->mBaseData.mHp = target->mBufferData.mHpUpper;
            }
            else
            {
                target->mBaseData.mHp = target->mBaseData.mHp + cureValue;
            }
        }

        // 如果是给友方增加的buffer更新时没有伤害hurt传递0
        updateBufferBeHit(*selfData, 0.0f, *target);
    }

    // 在最后一个打击点判断是否死亡
    if (isEnd)
    {
        if (target->mBaseData.mHp == 0)
        {
            dead(target);
        }
    }
}

void 
BattleManager::onAttackActionEnd(BattleData* userData)
{
    if (mCurrentSkill.mCombatType == BATTLE_CLOSE)
    {
        if (mCurrentSkill.mMultiAttack > 1)
        {
            mAttackCount++;
            if (mAttackCount >= mCurrentSkill.mMultiAttack)
            {
                mAttackCount = 0;

                //if (CheckAutoSkill())
                //{
                //    return;
                //}

                //if (mMoveBackAction.Length != 0 && mMoveBackTime > 0.0f)
                //{
                //    mMoving = true;
                //    mMovePos = mInitPos;
                //    mMoveSpeed = Vector3.Distance(transform.position, mMovePos) / mMoveBackTime;
                //    Vector3 lookAtPos = (transform.position - mMovePos) * 2 + mMovePos;
                //    transform.LookAt(lookAtPos);
                //    PlayAction(mMoveBackAction);
                //    mActor.Move(transform.position, mMovePos, mMoveBackAction);
                //}
                //else
                //{
                //    mMoveBackActionEnd = true;
                //    mMovePosEnd = true;
                //    transform.position = mInitPos;
                //    transform.rotation = mInitDir;
                //}

                attackEnd(userData);
            }
            else
            {
                //if (mCurrentSkill.mMultiNum > 1)
                //{
                //    if (mAttackCount == mCurrentSkill.mMultiNum)
                //    {
                //        PlayAction(mIdleAction);
                //    }
                //}
                //else
                //{
                //    PlayAction(mIdleAction);
                //}
                attack(userData, mCurrentSkill);
            }
        }
        else
        {
            mAttackCount = 0;

            //if (CheckAutoSkill())
            //{
            //    return;
            //}

            //if (mMoveBackAction.Length != 0 && mMoveBackTime > 0.0f)
            //{
            //    mMoving = true;
            //    mMovePos = mInitPos;
            //    mMoveSpeed = Vector3.Distance(transform.position, mMovePos) / mMoveBackTime;
            //    Vector3 lookAtPos = (transform.position - mMovePos) * 2 + mMovePos;
            //    transform.LookAt(lookAtPos);
            //    PlayAction(mMoveBackAction);
            //    mActor.Move(transform.position, mMovePos, mMoveBackAction);
            //}
            //else
            //{
            //    mMoveBackActionEnd = true;
            //    mMovePosEnd = true;
            //    transform.position = mInitPos;
            //    transform.rotation = mInitDir;
            //}
            attackEnd(userData);
        }
    }
    else
    {
        if (mCurrentSkill.mMultiAttack > 1)
        {
            mAttackCount++;
            if (mAttackCount >= mCurrentSkill.mMultiAttack)
            {
                mAttackCount = 0;
                //if (CheckAutoSkill())
                //{
                //    return;
                //}
                attackEnd(userData);
            }
            else
            {
                attack(userData, mCurrentSkill);
            }
        }
        else
        {
            mAttackCount = 0;
            //if (CheckAutoSkill())
            //{
            //    return;
            //}
            attackEnd(userData);
        }
    }

}

void 
BattleManager::attackEnd(BattleData* userData)
{
    if (userData == NULL)
    {
        LOG_WARN("Attack end userData is null.");
        return;
    }
    addAnger(userData);
    //if (mIsCommonSkill)
    //{
    //    if (userData->mBaseData.mCharacterType == BATTLE_PET && mCameraPos != 0)
    //    {
    //        mLordMain.ShowCharacterByRelationship(mRelationship);
    //    }
    //}
    //transform.rotation = mInitDir;
    //transform.position = mInitPos;
    //PlayAction(mIdleAction);
    if (!mIsSuperSkill && !mIsEnterSkill)
    {
        //mRoundEnd = true;
        updateBufferActionCount(*userData);
    }

    if (mTargetList.size() != 0)
    {
        for (List<BattleData*>::Iter* iter = mTargetList.begin();
            iter != mTargetList.end(); iter = mTargetList.next(iter))
        {
            if (iter->mValue == NULL)
            {
                continue;
            }

            delImmediatelyBuffer(*iter->mValue);
            updateNextBeginBuffer(*iter->mValue);
            //LynxCharacter character = target.GetComponent<LynxCharacter>();
            //if (character == null)
            //{
            //    continue;
            //}
            //if (character.Hp == 0)
            //{
            //    character.Dead();
            //}
        }
    }

    //if (mIsSuperSkill && mRelationship == Relationship.FRIEND)
    //{
    //    //mLordMain.MCameraCtrl.reset();
    //    mLordMain.SkillPanel.enabled = true;
    //}

    mAttackCount = 0;
    mAttackMultiIndex = 0;
    bool isSuper = mIsSuperSkill;
    bool isEnter = mIsEnterSkill;
    mIsSuperSkill = false;
    mIsCommonSkill = false;
    mIsAutoSkill = false;
    mIsEnterSkill = false;
    mAttackAction = "";
    oneAttackEnd(isSuper, isEnter, userData->mBaseData.mRelationship, 
        userData->mBaseData.mCharacterType, userData->mBaseData.mTag);
}

// 由于buffer在行动前造成的死亡
void 
BattleManager::deathForBufferBeforeAttack(BattleData* userData)
{
    if (userData == NULL)
    {
        LOG_WARN("Attack end userData is null.");
        return;
    }
    //transform.rotation = mInitDir;
    //transform.position = mInitPos;
    //if (!mIsSuperSkill && !mIsEnterSkill)
    //{
    //    //mRoundEnd = true;
    //}

    // 死亡
    dead(userData);

    mAttackCount = 0;
    mAttackMultiIndex = 0;
    bool isSuper = mIsSuperSkill;
    bool isEnter = mIsEnterSkill;
    mIsSuperSkill = false;
    mIsCommonSkill = false;
    mIsAutoSkill = false;
    mIsEnterSkill = false;
    mAttackAction = "";
    oneAttackEnd(isSuper, isEnter, userData->mBaseData.mRelationship, 
        userData->mBaseData.mCharacterType, userData->mBaseData.mTag);
}

// 由于buffer在行动中造成的死亡
void 
BattleManager::deathForBufferInAttack(BattleData& battleData)
{
    if (mTargetList.size() != 0)
    {
        for (List<BattleData*>::Iter* iter = mTargetList.begin();
            iter != mTargetList.end(); iter = mTargetList.next(iter))
        {
            if (iter->mValue == NULL)
            {
                continue;
            }

            delImmediatelyBuffer(*iter->mValue);
            updateNextBeginBuffer(*iter->mValue);
            //LynxCharacter character = target.GetComponent<LynxCharacter>();
            //if (character == null)
            //{
            //    continue;
            //}
            //if (character.Hp == 0)
            //{
            //    character.Dead();
            //}
        }
    }
    //transform.rotation = mInitDir;
    //transform.position = mInitPos;
    //if (!mIsSuperSkill && !mIsEnterSkill)
    //{
    //    //mRoundEnd = true;
    //}

    // 自身死亡
    dead(&battleData);

    mAttackCount = 0;
    mAttackMultiIndex = 0;
    bool isSuper = mIsSuperSkill;
    bool isEnter = mIsEnterSkill;
    mIsSuperSkill = false;
    mIsCommonSkill = false;
    mIsAutoSkill = false;
    mIsEnterSkill = false;
    mAttackAction = "";
    oneAttackEnd(isSuper, isEnter, battleData.mBaseData.mRelationship, 
        battleData.mBaseData.mCharacterType, battleData.mBaseData.mTag);
}

void 
BattleManager::addSelfSkillBuffer(BattleData* selfData)
{
    if (selfData == NULL)
    {
        LOG_WARN("AddTargetSkillBuffer get self failed.");
        return;
    }

    SkillTemplate* skillTemplate = SKILL_TABLE().get(mCurrentSkill.mTemplateId);
    if (skillTemplate == NULL)
    {
        LOG_WARN("AddSelfSkillBuffer get skill template failed, templateId[%u].", mCurrentSkill.mTemplateId);
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        UInt32 skillBufferId = 0;
        switch (i)
        {
        case 0:
            skillBufferId = skillTemplate->mSkillBuffer1;
            break;
        case 1:
            skillBufferId = skillTemplate->mSkillBuffer2;
            break;
        case 2:
            skillBufferId = skillTemplate->mSkillBuffer3;
            break;
        }

        if (skillBufferId == 0)
        {
            continue;
        }

        BufferTemplate* bufferTemplate = BUFFER_TABLE().get(skillBufferId);
        if (bufferTemplate == NULL)
        {
            LOG_WARN("Get self skill buffer template failed, templateId[%u]", skillBufferId);
            continue;
        }

        UInt32 randomValue = GameRandom::getUInt32Random(1, 100);
        if (randomValue > bufferTemplate->mOdds)
        {
            continue;
        }

        bool nextBegin = bufferTemplate->mLastActionCount != 0 ? true : false;
        if (bufferTemplate->mTargetType == BUFFER_TARGET_SELF)
        {
            addBuffer(*selfData, skillBufferId, *selfData, nextBegin);
        }

    }

}

void 
BattleManager::addTargetSkillBuffer(BattleData* selfData, BattleData* target)
{
    if (selfData == NULL)
    {
        LOG_WARN("AddTargetSkillBuffer get self failed.");
        return;
    }

    if (target == NULL)
    {
        LOG_WARN("AddTargetSkillBuffer get target failed.");
        return;
    }

    //LynxBuffer targetBuffer = target.GetComponent<LynxBuffer>();
    //if (targetBuffer == null)
    //{
    //    Debug.LogWarning("AddTargetSkillBuffer get LynxBuffer failed.");
    //    return;
    //}

    SkillTemplate* skillTemplate = SKILL_TABLE().get(mCurrentSkill.mTemplateId);
    if (skillTemplate == NULL)
    {
        LOG_WARN("AddTargetSkillBuffer get skill template failed, templateId[%u].", mCurrentSkill.mTemplateId);
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        UInt32 skillBufferId = 0;
        switch (i)
        {
        case 0:
            skillBufferId = skillTemplate->mSkillBuffer1;
            break;
        case 1:
            skillBufferId = skillTemplate->mSkillBuffer2;
            break;
        case 2:
            skillBufferId = skillTemplate->mSkillBuffer3;
            break;
        }

        if (skillBufferId == 0)
        {
            continue;
        }

        BufferTemplate* bufferTemplate = BUFFER_TABLE().get(skillBufferId);
        if (bufferTemplate == NULL)
        {
            LOG_WARN("Get target skill buffer template failed, templateId[%u]", skillBufferId);
            continue;
        }

        UInt32 randomValue = GameRandom::getUInt32Random(1, 100);
        if (randomValue > bufferTemplate->mOdds)
        {
            continue;
        }

        bool nextBegin = bufferTemplate->mLastActionCount != 0 ? true : false;
        if (bufferTemplate->mTargetType == BUFFER_TARGET_TARGET)
        {
            addBuffer(*target, skillBufferId, *selfData, nextBegin);
        }

    }

}

UInt32 
BattleManager::getAttackHitPoint(BattleData* userData)
{
    /*if (mAttackAction.empty())
    {
        return 0;
    }

    ActionHitMap::Iter* iter = userData->mSkillData.mActionHitMap.find(mAttackAction);
    if (iter == NULL)
    {
        LOG_WARN("Get action [%s] hit point count failed.", mAttackAction.c_str());
        return 0;
    }

    return iter->mValue;*/
	return 0;
}
