#include "HeroManager.h"
#include "GameServer.h"

using namespace Lynx;

HeroManager::HeroManager() : mPlayer(NULL)
{
    mPlayerHeroDataMap.clear();
    mPlayerPresentHeroDataMap.clear();
}


HeroManager::~HeroManager()
{
    mPlayer = NULL;
    mPlayerHeroData = NULL;
}

bool 
HeroManager::initial(Player* player)
{
    if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }

    mPlayer = player;
	//mPlayerData.mHeroData 字段注释 sec
  /*  mPlayerHeroData = &player->mPlayerData.mHeroData;

    for (List<HeroData>::Iter* iter = mPlayerHeroData->mHeroDataList.begin();
        iter != NULL; iter = mPlayerHeroData->mHeroDataList.next(iter))
    {
        mPlayerHeroDataMap.insert(iter->mValue.mBaseData.mHeroId, &iter->mValue);
        if (iter->mValue.mBaseData.mPresentPos != 0)
        {
            mPlayerPresentHeroDataMap.insert(iter->mValue.mBaseData.mPresentPos, &iter->mValue);
        }
    }*/

    return true;
}

void 
HeroManager::update(const UInt64& accTime)
{
   /* for (List<HeroData>::Iter* iter = mPlayerHeroData->mHeroDataList.begin();
        iter != NULL; iter = mPlayerHeroData->mHeroDataList.next(iter))
    {
        if (iter->mValue.mDirtyFlag)
        {
            PersistUpdateHeroNotify notify;
            notify.mPlayerGuid = mPlayer->getPlayerGuid();
            notify.mHeroData = iter->mValue;
            PersistSystem::getSingleton().postThreadMsg(notify, mPlayer->getPlayerGuid());
            iter->mValue.mDirtyFlag = false;
        }
    }*/
}

void 
HeroManager::calculateHeroData(const Guid& heroId)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return;
    }
   
    HeroData* heroData = iter->mValue;
    if (!heroData)
    {
        LOG_WARN("HeroData pointer is NULL.");
        return;
    }

    const PlayerData& playerData = mPlayer->getPlayerData();
  //  const PlayerPresentData& playerPresentData = mPlayer->getPlayerPresentData();

    HeroTemplate* heroTemplate = HERO_TABLE().get(heroData->mBaseData.mTemplateId);
    if (!heroTemplate)
    {
        LOG_WARN("Failed to get hero template by id %u", heroData->mBaseData.mTemplateId);
        return;
    }

    //CalculateUtil::calculateHeroData(playerData, *heroData, playerPresentData);
    
    if (heroData->mBaseData.mPresentPos != 0)
    {
        mPlayer->calculatePlayerData();
        mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_BATTLE_VALUE);
    }
}

void 
HeroManager::release()
{

}

const HeroData* 
HeroManager::createHero(const TemplateId& templateId, bool promp/*promp = true*/)
{
    HeroTemplate* heroTemplate = HERO_TABLE().get(templateId);
    if (!heroTemplate)
    {
        LOG_WARN("Failed to get hero template by template id %u", templateId);
        return NULL;
    }

    HeroData heroData;
    heroData.mBaseData.mTemplateId = templateId;
    heroData.mBaseData.mSrcTemplateId = templateId;
    heroData.mBaseData.mHeroId = LogicSystem::getSingleton().generateHeroGuid();
    heroData.mBaseData.mAdvanced = 0;
    heroData.mBaseData.mAnger = 0;
    heroData.mBaseData.mHeroName = heroTemplate->mHeroName.c_str();
    heroData.mBaseData.mHp = heroTemplate->mHp;
    heroData.mBaseData.mLevel = 1;
    heroData.mBaseData.mLevelExp = 0;
    heroData.mBaseData.mPresentPos = 0;

    if (mPlayerHeroDataMap.empty())
    {
        heroData.mBaseData.mPresentPos = 1;
    }
    
    heroData.mExtendData.mAngerUpper = heroTemplate->mAngerUpper;
    heroData.mExtendData.mAttack = heroTemplate->mAttack;
    heroData.mExtendData.mAvoidDamage = heroTemplate->mAvoidDamage;
    heroData.mExtendData.mCritical = heroTemplate->mCritical;
    heroData.mExtendData.mCriticalDamage = heroTemplate->mCriticalDamage;
    heroData.mExtendData.mCure = heroTemplate->mCure;
    heroData.mExtendData.mDodge = heroTemplate->mDodge;
    heroData.mExtendData.mHit = heroTemplate->mHit;
    heroData.mExtendData.mHpUpper = heroTemplate->mHp;
    heroData.mExtendData.mMagicDefense = heroTemplate->mMagicDefense;
    heroData.mExtendData.mPhysicsDefense = heroTemplate->mPhysicsDefense;
    heroData.mExtendData.mRescrit = heroTemplate->mRescrit;
    heroData.mExtendData.mSuck = heroTemplate->mSuck;

    //heroData.mSkillData.mCommonSkill.mTemplateId = heroTemplate->mSkill.mAttackSkillId;
    //if (heroTemplate->mSkill.mAttackSkillId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(heroTemplate->mSkill.mAttackSkillId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skill template failed id[%u]", heroTemplate->mSkill.mAttackSkillId);
    //        return false;
    //    }
    //    //heroData.mSkillData.mCommonSkill.mSkillAttackType = skillTemplate->mAttackType;
    //}

    //heroData.mSkillData.mSuperSkill.mTemplateId = heroTemplate->mSkill.mGreatSkillId;
    //if (heroTemplate->mSkill.mGreatSkillId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(heroTemplate->mSkill.mGreatSkillId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skill template failed id[%u]", heroTemplate->mSkill.mGreatSkillId);
    //        return false;
    //    }
    //    //heroData.mSkillData.mSuperSkill.mSkillAttackType = skillTemplate->mAttackType;
    //}

    //heroData.mSkillData.mAutoSkill.mTemplateId = heroTemplate->mSkill.mAutoSkillId;
    //if (heroTemplate->mSkill.mAutoSkillId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(heroTemplate->mSkill.mAutoSkillId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skill template failed id[%u]", heroTemplate->mSkill.mAutoSkillId);
    //        return false;
    //    }
    //    //heroData.mSkillData.mAutoSkill.mSkillAttackType = skillTemplate->mAttackType;
    //}

    //heroData.mSkillData.mEnterSkill.mTemplateId = heroTemplate->mSkill.mComeSkillId;
    //if (heroTemplate->mSkill.mComeSkillId != 0)
    //{
    //    SkillTemplate* skillTemplate = SKILL_TABLE().get(heroTemplate->mSkill.mComeSkillId);
    //    if (skillTemplate == NULL)
    //    {
    //        LOG_WARN("Get skill template failed id[%u]", heroTemplate->mSkill.mComeSkillId);
    //        return false;
    //    }
    //    //heroData.mSkillData.mEnterSkill.mSkillAttackType = skillTemplate->mAttackType;
    //}

    List<HeroData>::Iter* iter = mPlayerHeroData->mHeroDataList.insertTail(heroData);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to insert hero data into hero data list.");
    //    return NULL;
    //}

    mPlayerHeroDataMap.insert(iter->mValue.mBaseData.mHeroId, &iter->mValue);
    if (iter->mValue.mBaseData.mPresentPos != 0)
    {
        mPlayerPresentHeroDataMap.insert(iter->mValue.mBaseData.mPresentPos, &iter->mValue);
    }

    PersistInsertHeroNotify persistInsertHeroNotify;
    persistInsertHeroNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    persistInsertHeroNotify.mHeroData = iter->mValue;
    PersistSystem::getSingleton().postThreadMsg(persistInsertHeroNotify, mPlayer->getPlayerGuid());

    AddHeroNotify addHeroNotify;
    addHeroNotify.mHeroData = iter->mValue;
    addHeroNotify.mPromp = promp;
    mPlayer->sendMsgWithAck(addHeroNotify);

    mPlayer->updatePlayerPresentHeros();
    calculateHeroData(heroData.mBaseData.mHeroId);

    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_OWN_STAR_FOUR_HERO_CNT);
    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_OWN_STAR_FIVE_HERO_CNT);
    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_OWN_STAR_SIX_HERO_CNT);
    
    return &iter->mValue;
}

bool 
HeroManager::destroyHero(Guid guid)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(guid);
    if (iter == NULL)
    {
        LOG_WARN("Failed to destroy hero [%llu] for not exist.", guid);
        return false;
    }

    UInt8 presentPos = iter->mValue->mBaseData.mPresentPos;
    mPlayerHeroDataMap.erase(iter);

    for (List<HeroData>::Iter* jter = mPlayerHeroData->mHeroDataList.begin();
        jter != NULL; jter = mPlayerHeroData->mHeroDataList.next(jter))
    {
        if (jter->mValue.mBaseData.mHeroId == guid)
        {
            mPlayerHeroData->mHeroDataList.erase(jter);
            for (Map<UInt8, HeroData*>::Iter* kter = mPlayerPresentHeroDataMap.begin();
                kter != NULL; kter = mPlayerPresentHeroDataMap.next(kter))
            {
                if (kter->mValue->mBaseData.mHeroId == guid)
                {
                    mPlayerPresentHeroDataMap.erase(kter);
                    break;
                }
            }
            break;
        }
    }

    PersistDelHeroNotify persistDelHeroNotify;
    persistDelHeroNotify.mHeroGuid = guid;
    persistDelHeroNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(persistDelHeroNotify, mPlayer->getPlayerGuid());

    DelHeroNotify delHeroNotify;
    delHeroNotify.mHeroGuid = guid;
    mPlayer->sendMsgWithAck(delHeroNotify);

    if (presentPos > 0)
    {
        mPlayer->updatePlayerPresentHeros();
        mPlayer->calculatePlayerData();
        mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_BATTLE_VALUE);
    }
    return true;
}

const HeroData* 
HeroManager::getHeroDataByGuid(const Guid& guid) const
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(guid);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", guid);
        return NULL;
    }
    
    return iter->mValue;
}

const HeroData* 
HeroManager::getHeroDataByPresentPos(const UInt8& presentPos) const
{
    Map<UInt8, HeroData*>::Iter* iter = mPlayerPresentHeroDataMap.find(presentPos);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero by slot [%u].", presentPos);
        return NULL;
    }
    return iter->mValue;
}

void 
HeroManager::refinePresentHeros(List<Guid>& heroLevelUpList, UInt32 incExp)
{
    //bool heroLevelUp = false;
    //for (Map<UInt8, HeroData*>::Iter* heroIter = mPlayer->getPlayerPresentData().mPresentHeroMap.begin();
    //    heroIter != NULL; heroIter = mPlayer->getPlayerPresentData().mPresentHeroMap.next(heroIter))
    //{
    //    HeroData* heroData = heroIter->mValue;
    //    if (heroData)
    //    {
    //        // 计算增加的经验可以升几级
    //        UInt32 remainExp = heroData->mBaseData.mLevelExp + incExp;
    //        UInt32 upLevel = 0;
    //        do
    //        {
    //            HeroGrowTemplate* growTemplate = HERO_GROW_TABLE().get(heroData->mBaseData.mLevel + upLevel);
    //            if (!growTemplate)
    //            {
    //                LOG_WARN("Failed to get hero grow template by current level %u", 
    //                    heroData->mBaseData.mLevel + upLevel);
    //                break;
    //            }

    //            if (remainExp >= growTemplate->mExperience)
    //            {
    //                remainExp -= growTemplate->mExperience;
    //                upLevel++;
    //            }
    //            else if (remainExp < growTemplate->mExperience)
    //            {
    //                break;
    //            }
    //        } while (1);

    //        if (upLevel > 0)
    //        {
    //            increaseHeroLevel(heroData->mBaseData.mHeroId, upLevel);
    //            heroLevelUpList.insertTail(heroData->mBaseData.mHeroId);
    //        }

    //        if (remainExp > heroData->mBaseData.mLevelExp)
    //        {
    //            increaseHeroLevelExp(heroData->mBaseData.mHeroId, remainExp - heroData->mBaseData.mLevelExp);
    //        }
    //        else if (remainExp < heroData->mBaseData.mLevelExp)
    //        {
    //            reduceHeroLevelExp(heroData->mBaseData.mHeroId, heroData->mBaseData.mLevelExp - remainExp);
    //        }
    //    }
    //}
}

ErrorId 
HeroManager::refineHero(const Guid& heroId, UInt32& upLevelCount, UInt32 incExp)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return LynxErrno::HeroNotExist;
    }

    HeroData* heroData = iter->mValue;
    if (heroData)
    {
        // 计算增加的经验可以升几级
        UInt32 remainExp = heroData->mBaseData.mLevelExp + incExp;
        UInt32 upLevel = 0;
        do
        {
            HeroGrowTemplate* growTemplate = HERO_GROW_TABLE().get(heroData->mBaseData.mLevel + upLevel);
            if (!growTemplate)
            {
                LOG_WARN("Failed to get hero grow template by current level %u", 
                    heroData->mBaseData.mLevel + upLevel);
                break;
            }

            if (remainExp >= growTemplate->mExperience)
            {
                remainExp -= growTemplate->mExperience;
                upLevel++;
            }
            else if (remainExp < growTemplate->mExperience)
            {
                break;
            }
        } while (1);

        if (upLevel > 0)
        {
            increaseHeroLevel(heroData->mBaseData.mHeroId, upLevel);
        }

        if (remainExp > heroData->mBaseData.mLevelExp)
        {
            increaseHeroLevelExp(heroData->mBaseData.mHeroId, remainExp - heroData->mBaseData.mLevelExp);
        }
        else if (remainExp < heroData->mBaseData.mLevelExp)
        {
            reduceHeroLevelExp(heroData->mBaseData.mHeroId, heroData->mBaseData.mLevelExp - remainExp);
        }

        upLevelCount = upLevel;
    }

    return LynxErrno::None;
}

ErrorId 
HeroManager::refineHero(const Guid& heroId, UInt32 levelCount)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return LynxErrno::HeroNotExist;
    }

    HeroData* heroData = iter->mValue;

    if (!heroData)
    {
        LOG_WARN("Hero data is null");
        return LynxErrno::HeroNotExist;
    }
    // 英雄等级不能超过玩家等级
    if (mPlayer->mPlayerData.mBaseData.m_nLevel < heroData->mBaseData.mLevel + levelCount)
    {
        LOG_WARN("Hero level cannot be more than team level.");
        return LynxErrno::InvalidOperation;
    }

    UInt32 requireExp = 0;
    for (UInt32 i = 0; i < levelCount; i++)
    {
        HeroGrowTemplate* heroGrowTemplate = HERO_GROW_TABLE().get(heroData->mBaseData.mLevel + i);
        if (heroGrowTemplate)
        {
            requireExp += heroGrowTemplate->mExperience;
        }
        else
        {
            LOG_WARN("Hero level is the highest.");
            return LynxErrno::InvalidOperation;
        }
    }

    if (requireExp < heroData->mBaseData.mLevelExp || requireExp == 0)
    {
        LOG_WARN("Hero level is the highest.");
        return LynxErrno::InvalidOperation;
    }
    requireExp -= heroData->mBaseData.mLevelExp;
    UInt32 requireGold = requireExp;

    if (mPlayer->getPlayerData().mBaseData.m_nGold < requireExp)
    {
        LOG_WARN("Gold isn't full.");
        return LynxErrno::GoldNotEnough;
    }

	//sec  Hero 字段注释了 
  /*  if (mPlayer->getPlayerData().mBaseData.mHeroExpPool < requireExp)
    {
        LOG_WARN("Exp isn't full.");
        return LynxErrno::LevelExpNotEnough;
    }*/

    mPlayer->reducePlayerGold(requireGold);
    mPlayer->reducePlayerHeroExpPool(requireExp);

    increaseHeroConsumeGold(heroId, requireGold);
    increaseHeroLevel(heroId, levelCount);
    reduceHeroLevelExp(heroId, heroData->mBaseData.mLevelExp);

    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_HERO_REFINE_LEVEL);

    return LynxErrno::None;
}

ErrorId 
HeroManager::advancedHero(const Guid& heroId, const List<Guid>& heroList, 
                          const List<Guid>& itemList)
{
    //强化英雄原游戏注释掉，sec
	//Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
    //    return LynxErrno::HeroNotExist;
    //}

    //HeroData* heroData = iter->mValue;

    //if (!heroData)
    //{
    //    LOG_WARN("Hero data is null");
    //    return LynxErrno::HeroNotExist;
    //}

    //HeroTemplate* heroTemplate = HERO_TABLE().get(heroData->mBaseData.mTemplateId);
    //if (!heroTemplate)
    //{
    //    LOG_WARN("Failed to get hero template %u", heroData->mBaseData.mTemplateId);
    //    return LynxErrno::TemplateNotExist;
    //}

    //UInt32 advancedConditionId = 0;
    //if (heroData->mBaseData.mAdvanced == 0)
    //{
    //    advancedConditionId = heroTemplate->mEvolution.mAdvancedConditions1;
    //}
    //else if (heroData->mBaseData.mAdvanced == 1)
    //{
    //    advancedConditionId = heroTemplate->mEvolution.mAdvancedConditions2;
    //}
    //else if (heroData->mBaseData.mAdvanced == 2)
    //{
    //    advancedConditionId = heroTemplate->mEvolution.mAdvancedConditions3;
    //}
    //else if (heroData->mBaseData.mAdvanced == 3)
    //{
    //    advancedConditionId = heroTemplate->mEvolution.mAdvancedConditions4;
    //}
    //else if (heroData->mBaseData.mAdvanced == 4)
    //{
    //    advancedConditionId = heroTemplate->mEvolution.mAdvancedConditions5;
    //}
    //else
    //{
    //    advancedConditionId = 0;
    //}

    //if (advancedConditionId == 0)
    //{
    //    LOG_WARN("Failed to advanced hero for condtion invalid");
    //    return LynxErrno::InvalidOperation;
    //}

    //UInt32 requireHeroLevel = 0;
    //Map<TemplateId, UInt32> requireItems;
    //Map<TemplateId, UInt32> requireHeros;
    //UInt32 requireGold = 0;

    //UInt32 n = 0;
    //for (HeroConditionTemplateMap::Iter* iter = HERO_CONDITION_TABLE().mMap.begin();
    //    iter != NULL; iter = HERO_CONDITION_TABLE().mMap.next(iter))
    //{
    //    if (iter->mValue.mConditionID == advancedConditionId)
    //    {
    //        if (requireHeroLevel < iter->mValue.mHeroLevel)
    //        {
    //            requireHeroLevel = iter->mValue.mHeroLevel;
    //        }

    //        if (iter->mValue.mHeroRequireType == 1)
    //        {
    //            // 消耗和自己一样的英雄
    //            requireHeros.insert(heroData->mBaseData.mTemplateId, iter->mValue.mHeroCount);
    //        }
    //        else if (iter->mValue.mHeroRequireType == 2 && iter->mValue.mHeroID != 0)
    //        {
    //            // 消耗制定英雄
    //            requireHeros.insert(iter->mValue.mHeroID, iter->mValue.mHeroCount);
    //        }

    //        if (iter->mValue.mItemID != 0)
    //        {
    //            requireItems.insert(iter->mValue.mItemID, iter->mValue.mItemCount);
    //        }

    //        requireGold += iter->mValue.mGold;
    //        
    //        if (n++ >= 5)
    //        {
    //            break;
    //        }
    //    }
    //}

    //// 检查英雄等级
    //if (heroData->mBaseData.mLevel < requireHeroLevel) 
    //{
    //    LOG_WARN("Failed to advanced for level < require level.");
    //    return LynxErrno::LevelNotEnough;
    //}

    //// 检查金币
    //if (mPlayer->getPlayerData().mBaseData.m_nGold< requireGold)
    //{
    //    LOG_WARN("Failed to advanced for gold < require gold.");
    //    return LynxErrno::GoldNotEnough;
    //}

    //// 检查需要英雄数量
    //Map<TemplateId, UInt32> tmpHeroMap;
    //for (List<Guid>::Iter* iter = heroList.begin();
    //    iter != NULL; iter = heroList.next(iter))
    //{
    //    Map<Guid, HeroData*>::Iter* heroDataIter = mPlayerHeroDataMap.find(iter->mValue);
    //    if (!heroDataIter)
    //    {
    //        LOG_WARN("Failed to advanced for hero isn't exist.");
    //        return LynxErrno::HeroNotExist;
    //    }
    //    if (heroDataIter->mValue->mBaseData.mLevel >= 5)
    //    {
    //        LOG_WARN("Failed to awaken for hero isn't exist.");
    //        return LynxErrno::LevelNotEnough;
    //    }

    //    if (heroDataIter->mValue)
    //    {
    //        Map<TemplateId, UInt32>::Iter* kter = tmpHeroMap.find(heroDataIter->mValue->mBaseData.mTemplateId);
    //        if (!kter)
    //        {
    //            kter = tmpHeroMap.insert(heroDataIter->mValue->mBaseData.mTemplateId, 0);
    //        }

    //        kter->mValue += 1;
    //    }
    //}

    //if (requireHeros.size() != tmpHeroMap.size())
    //{
    //    LOG_WARN("Failed to advanced for hero isn't exist.");
    //    return LynxErrno::InvalidOperation;
    //}
    //for (Map<TemplateId, UInt32>::Iter* iter = requireHeros.begin();
    //    iter != NULL; iter = requireHeros.next(iter))
    //{
    //    Map<TemplateId, UInt32>::Iter* kter = tmpHeroMap.find(iter->mKey);
    //    if (!kter)
    //    {
    //        LOG_WARN("Failed to advanced for hero isn't exist.");
    //        return LynxErrno::HeroNotExist;
    //    }
    //    if (kter->mValue != iter->mValue)
    //    {
    //        LOG_WARN("Failed to advanced for hero isn't exist.");
    //        return LynxErrno::HeroNotExist;
    //    }
    //}

    //// 检查需要道具数量
    //Map<TemplateId, UInt32> tmpItemMap;
    //Map<Guid, UInt32> tmpItemGuidMap;
    //for (List<Guid>::Iter* iter = itemList.begin();
    //    iter != NULL; iter = itemList.next(iter))
    //{
    //    const ItemData* itemData = mPlayer->getItemManager().getItemByGuid(iter->mValue);
    //    if (!itemData)
    //    {
    //        LOG_WARN("Failed to advanced for item isn't exist.");
    //        return LynxErrno::ItemNotExist;
    //    }

    //    tmpItemMap.insert(itemData->mTemplateId, itemData->mCount);
    //}

    //if (requireItems.size() != tmpItemMap.size())
    //{
    //    LOG_WARN("Failed to advanced for hero isn't exist.");
    //    return LynxErrno::InvalidOperation;
    //}
    //for (Map<TemplateId, UInt32>::Iter* iter = requireItems.begin();
    //    iter != NULL; iter = requireItems.next(iter))
    //{
    //    Map<TemplateId, UInt32>::Iter* kter = tmpItemMap.find(iter->mKey);
    //    if (!kter)
    //    {
    //        LOG_WARN("Failed to advanced for item isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }
    //    // 客户端没有发送数量，所以不用检查数量，这里检查需求表的数量是否大于背包数量
    //    if (kter->mValue < iter->mValue)
    //    {
    //        LOG_WARN("Failed to advanced for item isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }
    //    const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(iter->mKey);
    //    if (!itemData)
    //    {
    //        LOG_WARN("Failed to advanced for item isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }
    //    //直接用需求表中的数量
    //    tmpItemGuidMap.insert(itemData->mItemId, iter->mValue);
    //}

    //// 执行进阶操作
    //mPlayer->reducePlayerGold(requireGold);

    //for (List<Guid>::Iter* iter = heroList.begin();
    //    iter != NULL; iter = heroList.next(iter))
    //{
    //    destroyHero(iter->mValue);
    //}

    //for (Map<Guid, UInt32>::Iter* iter = tmpItemGuidMap.begin();
    //    iter != NULL; iter = tmpItemGuidMap.next(iter))
    //{
    //    mPlayer->getItemManager().delItem(iter->mKey, iter->mValue);
    //}
    //increaseHeroAdvanced(heroId, 1);

    return LynxErrno::None;
}

ErrorId 
HeroManager::awakenHero(const Guid& heroId, const List<Guid>& heroList, 
                        const List<Guid>& itemList)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return LynxErrno::HeroNotExist;
    }

    HeroData* heroData = iter->mValue;

    if (!heroData)
    {
        LOG_WARN("Hero data is null");
        return LynxErrno::HeroNotExist;
    }

    HeroTemplate* heroTemplate = HERO_TABLE().get(heroData->mBaseData.mTemplateId);
    if (!heroTemplate)
    {
        LOG_WARN("Failed to get hero template %u", heroData->mBaseData.mTemplateId);
        return LynxErrno::TemplateNotExist;
    }

    UInt32 awakenConditionId = heroTemplate->mEvolution.mAwakenConditions;
    if (awakenConditionId == 0)
    {
        LOG_WARN("Failed to awaken hero for condtion invalid");
        return LynxErrno::InvalidOperation;
    }

    UInt32 requireHeroLevel = 0;
    Map<TemplateId, UInt32> requireItems;
    Map<TemplateId, UInt32> requireHeros;
    UInt32 requireGold = 0;
    UInt32 requireAdvancedLevel = 0;
    TemplateId heroTemplateId = heroTemplate->mEvolution.mAwaken;

    UInt32 n = 0;

	//英雄觉醒暂时关闭 sec
    //for (HeroConditionTemplateMap::Iter* iter = HERO_CONDITION_TABLE().mMap.begin();
    //    iter != NULL; iter = HERO_CONDITION_TABLE().mMap.next(iter))
    //{
    //    if (iter->mValue.mConditionID == awakenConditionId)
    //    {
    //        if (requireAdvancedLevel < iter->mValue.mAdvanced)
    //        {
    //            requireAdvancedLevel = iter->mValue.mHeroLevel;
    //        }

    //        if (requireHeroLevel < iter->mValue.mHeroLevel)
    //        {
    //            requireHeroLevel = iter->mValue.mHeroLevel;
    //        }

    //        if (iter->mValue.mHeroRequireType == 1)
    //        {
    //            // 消耗和自己一样的英雄
    //            requireHeros.insert(heroData->mBaseData.mTemplateId, iter->mValue.mHeroCount);
    //        }
    //        else if (iter->mValue.mHeroRequireType == 2 && iter->mValue.mHeroID != 0)
    //        {
    //            // 消耗制定英雄
    //            requireHeros.insert(iter->mValue.mHeroID, iter->mValue.mHeroCount);
    //        }

    //        if (iter->mValue.mItemID != 0)
    //        {
    //            requireItems.insert(iter->mValue.mItemID, iter->mValue.mItemCount);
    //        }

    //        requireGold += iter->mValue.mGold;
    //        break;
    //    }
    //}

    //// 检查英雄等级
    //if (heroData->mBaseData.mLevel < requireHeroLevel) 
    //{
    //    LOG_WARN("Failed to awaken for level < require level.");
    //    return LynxErrno::LevelNotEnough;
    //}

    //// 检查金币
    //if (mPlayer->getPlayerData().mBaseData.mGold < requireGold)
    //{
    //    LOG_WARN("Failed to awaken for gold < require gold.");
    //    return LynxErrno::GoldNotEnough;
    //}

    //// 检查进阶等级
    //if (heroData->mBaseData.mAdvanced < requireAdvancedLevel)
    //{
    //    LOG_WARN("Failed to awaken for advanced level.");
    //    return LynxErrno::AdvancedLevelNotEnough;
    //}

    //// 检查需要英雄数量
    //Map<TemplateId, UInt32> tmpHeroMap;
    //for (List<Guid>::Iter* iter = heroList.begin();
    //    iter != NULL; iter = heroList.next(iter))
    //{
    //    Map<Guid, HeroData*>::Iter* heroDataIter = mPlayerHeroDataMap.find(iter->mValue);
    //    if (!heroDataIter)
    //    {
    //        LOG_WARN("Failed to awaken for hero isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }
    //    if (heroDataIter->mValue->mBaseData.mLevel >= 5)
    //    {
    //        LOG_WARN("Failed to awaken for hero isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }

    //    if (heroDataIter->mValue)
    //    {
    //        Map<TemplateId, UInt32>::Iter* kter = tmpHeroMap.find(heroDataIter->mValue->mBaseData.mTemplateId);
    //        if (!kter)
    //        {
    //            kter = tmpHeroMap.insert(heroDataIter->mValue->mBaseData.mTemplateId, 0);
    //        }

    //        kter->mValue += 1;
    //    }
    //}

    //if (requireHeros.size() != tmpHeroMap.size())
    //{
    //    LOG_WARN("Failed to advanced for hero isn't exist.");
    //    return LynxErrno::InvalidOperation;
    //}
    //for (Map<TemplateId, UInt32>::Iter* iter = requireHeros.begin();
    //    iter != NULL; iter = requireHeros.next(iter))
    //{
    //    Map<TemplateId, UInt32>::Iter* kter = tmpHeroMap.find(iter->mKey);
    //    if (!kter)
    //    {
    //        LOG_WARN("Failed to awaken for hero isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }
    //    if (kter->mValue != iter->mValue)
    //    {
    //        LOG_WARN("Failed to awaken for hero isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }
    //}

    //// 检查需要道具数量
    //Map<TemplateId, UInt32> tmpItemMap;
    //Map<Guid, UInt32> tmpItemGuidMap;
    //for (List<Guid>::Iter* iter = itemList.begin();
    //    iter != NULL; iter = itemList.next(iter))
    //{
    //    const ItemData* itemData = mPlayer->getItemManager().getItemByGuid(iter->mValue);
    //    if (!itemData)
    //    {
    //        LOG_WARN("Failed to awaken for item isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }

    //    tmpItemMap.insert(itemData->mTemplateId, itemData->mCount);
    //}

    //if (requireItems.size() != tmpItemMap.size())
    //{
    //    LOG_WARN("Failed to awaken for hero isn't exist.");
    //    return LynxErrno::InvalidOperation;
    //}
    //for (Map<TemplateId, UInt32>::Iter* iter = requireItems.begin();
    //    iter != NULL; iter = requireItems.next(iter))
    //{
    //    Map<TemplateId, UInt32>::Iter* kter = tmpItemMap.find(iter->mKey);
    //    if (!kter)
    //    {
    //        LOG_WARN("Failed to awaken for item isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }
    //    if (kter->mValue < iter->mValue)
    //    {
    //        LOG_WARN("Failed to awaken for item isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }
    //    const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(iter->mKey);
    //    if (!itemData)
    //    {
    //        LOG_WARN("Failed to awaken for item isn't exist.");
    //        return LynxErrno::InvalidOperation;
    //    }
    //    tmpItemGuidMap.insert(itemData->mItemId, iter->mValue);
    //}

    //// 执行进阶操作
    //HeroTemplate* newHeroTemplate = HERO_TABLE().get(heroTemplateId);
    //if (!newHeroTemplate)
    //{
    //    LOG_WARN("Failed to awaken for hero for create new hero %u failed.", heroTemplateId);
    //    return LynxErrno::InvalidOperation;
    //}

    //if (newHeroTemplate->mHeroClassId != heroTemplate->mHeroClassId)
    //{
    //    LOG_WARN("Awaken class id don't match, %u %u", newHeroTemplate->mHeroClassId, heroTemplate->mHeroClassId);
    //    return LynxErrno::InvalidOperation;
    //}

    //heroData->mBaseData.mTemplateId = heroTemplateId;
    //heroData->mBaseData.mAwakeCnt ++;
    //
    //calculateHeroData(heroId);

    //reduceHeroAdvanced(heroId, heroData->mBaseData.mAdvanced);

    //SyncHeroTemplateIdNotify notify;
    //notify.mHeroId = heroId;
    //notify.mTemplateId = heroTemplateId;
    //mPlayer->sendMsgWithAck(notify);

    //mPlayer->reducePlayerGold(requireGold);

    //for (List<Guid>::Iter* iter = heroList.begin();
    //    iter != NULL; iter = heroList.next(iter))
    //{
    //    destroyHero(iter->mValue);
    //}

    //for (Map<Guid, UInt32>::Iter* iter = tmpItemGuidMap.begin();
    //    iter != NULL; iter = tmpItemGuidMap.next(iter))
    //{
    //    mPlayer->getItemManager().delItem(iter->mKey, iter->mValue);
    //}

    //heroData->mDirtyFlag = true;

    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_OWN_STAR_FIVE_HERO_CNT);
    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_TWO_HERO_AWAKEN_LEVEL);
    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_THREE_HERO_AWAKEN_LEVEL);
    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_FOUR_HERO_AWAKEN_LEVEL);
    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_FIVE_HERO_AWAKEN_LEVEL);

    return LynxErrno::None;
}

ErrorId 
HeroManager::smeltHero(const Guid& heroId, UInt32& gold, UInt32& levelExp, UInt32& soul)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return LynxErrno::HeroNotExist;
    }

    HeroData* heroData = iter->mValue;

    if (!heroData)
    {
        LOG_WARN("Hero data is null");
        return LynxErrno::HeroNotExist;
    }

    HeroTemplate* heroTemplate = HERO_TABLE().get(heroData->mBaseData.mTemplateId);
    if (!heroTemplate)
    {
        LOG_WARN("Failed to get hero template %u", heroData->mBaseData.mTemplateId);
        return LynxErrno::TemplateNotExist;
    }

    gold += heroTemplate->mDestroyGold;
    levelExp += heroTemplate->mDestroyExp;
    soul += heroTemplate->mDestroySoul;

    // 销毁英雄
    destroyHero(heroId);
    return LynxErrno::None;
}

ErrorId 
HeroManager::rebornHero(const Guid& heroId, UInt32& gold, UInt32& levelExp,
                        List<Guid>& heroList, List<Guid>& itemList)
{
    // sec注释原游戏重生英雄
	//Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
    //    return LynxErrno::HeroNotExist;
    //}

    //HeroData* heroData = iter->mValue;

    //if (!heroData)
    //{
    //    LOG_WARN("Hero data is null");
    //    return LynxErrno::HeroNotExist;
    //}

    //// 判断英雄不能出场， 否则不允许重生
    //if (heroData->mBaseData.mPresentPos != 0)
    //{
    //    LOG_WARN("Failed to reborn hero for hero present %u", heroData->mBaseData.mPresentPos);
    //    return LynxErrno::InvalidPermission;
    //}

    //HeroTemplate* heroTemplate = HERO_TABLE().get(heroData->mBaseData.mTemplateId);
    //if (!heroTemplate)
    //{
    //    LOG_WARN("Failed to get hero template %u", heroData->mBaseData.mTemplateId);
    //    return LynxErrno::TemplateNotExist;
    //}

    //HeroConsumeReport heroConsumeReport;
    //if (!CalculateUtil::calculateHeroConsumeReport(*heroData, heroConsumeReport))
    //{
    //    LOG_WARN("Failed to calculate hero consume report %llu", heroId);
    //    return LynxErrno::InvalidOperation;
    //}

    //for (Map<TemplateId, UInt32>::Iter* jter = heroConsumeReport.mHeroMap.begin();
    //    jter != NULL; jter = heroConsumeReport.mHeroMap.next(jter))
    //{
    //    for (UInt32 i = 0; i < jter->mValue; ++i)
    //    {
    //        const HeroData* newHeroData = createHero(jter->mKey);
    //        if (newHeroData)
    //        {
    //            heroList.insertTail(newHeroData->mBaseData.mHeroId);
    //        }
    //    }
    //}

    //for (Map<TemplateId, UInt32>::Iter* jter = heroConsumeReport.mItemMap.begin();
    //    jter != NULL; jter = heroConsumeReport.mItemMap.next(jter))
    //{
    //    const ItemData* newItemData = mPlayer->getItemManager().createItem(jter->mKey, jter->mValue, 0);
    //    if (newItemData)
    //    {
    //        itemList.insertTail(newItemData->mItemId);
    //    }
    //}

    //gold += heroConsumeReport.mGold;
    //levelExp += heroConsumeReport.mLevelExp;

    //heroData->mBaseData.mTemplateId = heroData->mBaseData.mSrcTemplateId;
    //heroData->mBaseData.mAdvanced = 0;
    //heroData->mBaseData.mAnger = 0;
    //heroData->mBaseData.mHeroName = heroTemplate->mHeroName.c_str();
    //heroData->mBaseData.mHp = 0;
    //heroData->mBaseData.mLevel = 1;
    //heroData->mBaseData.mLevelExp = 0;
    //heroData->mBaseData.mPresentPos = 0;

    //RebornHeroNotify notify;
    //notify.mHeroId = heroData->mBaseData.mHeroId;
    //mPlayer->sendMsgWithAck(notify);

    //heroData->mDirtyFlag = true;

    //PersistUpdateHeroNotify dbNotify;
    //dbNotify.mHeroData = *heroData;
    //dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    //PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
    return LynxErrno::None;
}

ErrorId 
HeroManager::setHeroPresentPos(const Guid& heroId, UInt8 presentPos)
{
    // 判断出场目标出场位置不越界
    if (presentPos > HERO_PRESENT_POS_ALTERNATE_2)
    {
        LOG_WARN("Present pos %u overflow.", presentPos);
        return LynxErrno::InvalidOperation;
    }

    // 判断英雄有效
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return LynxErrno::HeroNotExist;
    }

    HeroData* heroData = iter->mValue;
    if (!heroData)
    {
        LOG_WARN("Hero pointer is null.");
        return LynxErrno::HeroNotExist;
    }

    // 判断英雄准备上阵的英雄之前不能是已经出场的英雄
    if (heroData->mBaseData.mPresentPos != 0)
    {
        LOG_WARN("Failed to set hero present pos for hero has present %u", heroData->mBaseData.mPresentPos);
        return LynxErrno::InvalidOperation;
    }

    // 获得目标出场位置对应的格子是否有英雄存在
    Map<UInt8, HeroData*>::Iter* targetHeroIter = mPlayerPresentHeroDataMap.find(presentPos);
    if (!targetHeroIter) // 目标位置没有上阵英雄，上阵情况
    {
        // 判断不能上阵相同类型的英雄
        for (Map<UInt8, HeroData*>::Iter* i = mPlayerPresentHeroDataMap.begin();
            i != NULL; i = mPlayerPresentHeroDataMap.next(i))
        {
            if (i->mValue)
            {
                if (i->mValue->mBaseData.mSrcTemplateId == iter->mValue->mBaseData.mSrcTemplateId)
                {
                    LOG_WARN("Hero present repeat. %u", iter->mValue->mBaseData.mSrcTemplateId);
                    return LynxErrno::HeroRepeat;
                }
            }
        }
    }
    else // 目标位置有英雄， 更换情况
    {
        // 判断目标英雄有效
        HeroData* targetHeroData = targetHeroIter->mValue;
        if (!targetHeroData)
        {
            LOG_WARN("Hero pointer is null.");
            return LynxErrno::HeroNotExist;
        }

        // 判断更换英雄跟目标英雄不能是同一GUID
        if (heroData->mBaseData.mHeroId == targetHeroData->mBaseData.mHeroId)
        {
            LOG_WARN("Failed to set hero pos for myself repeat set.");
            return LynxErrno::InvalidOperation;
        }

        // 判断英雄不能跟除了目标英雄之外的其他出场英雄有相同的原模板ID
        for (Map<UInt8, HeroData*>::Iter* i = mPlayerPresentHeroDataMap.begin();
            i != NULL; i = mPlayerPresentHeroDataMap.next(i))
        {
            if (i->mValue)
            {
                if (i->mValue->mBaseData.mHeroId != targetHeroData->mBaseData.mHeroId)
                {
                    if (i->mValue->mBaseData.mSrcTemplateId == iter->mValue->mBaseData.mSrcTemplateId)
                    {
                        LOG_WARN("Hero present repeat. %u", iter->mValue->mBaseData.mSrcTemplateId);
                        return LynxErrno::HeroRepeat;
                    }
                }
            }
        }
    }
    
    // 获得常量表
    ConstItemIdTemplate* constItemIdTemplate = CONST_ITEM_ID_TABLE().get(1);
    if (constItemIdTemplate == NULL)
    {
        LOG_WARN("Failed to get const item id template");
        return LynxErrno::TemplateNotExist;
    }

    // 获得出场英雄的最大位置
    Map<UInt8, HeroData*>::Iter* maxPresentHeroIter = mPlayerPresentHeroDataMap.getMax();
    if (maxPresentHeroIter == NULL)
    {
        LOG_WARN("Failed to get max present hero iter");
        return LynxErrno::InvalidOperation;
    }

    // 如果最大位置>5, 异常情况
    if (maxPresentHeroIter->mKey > 5)
    {
        LOG_WARN("Failed to get max present hero count > 5");
        return LynxErrno::InvalidOperation;
    }

    // 如果最大位置小于5, 说明现在英雄还没出场到5位，需要根据出场位置逻辑判断目标位置是否有权限使用
    if (maxPresentHeroIter->mKey < 5)
    {
        UInt8 pos = 0;
        if (maxPresentHeroIter->mKey == 4)
        {
            if (mPlayer->getPlayerData().mBaseData.m_nLevel < constItemIdTemplate->mRequireLevelForHeroPresentPos5)
            {
                pos = 4;
            }
            else
            {
                pos = 5;
            }
        }
        else if (maxPresentHeroIter->mKey == 3)
        {
            if (mPlayer->getPlayerData().mBaseData.m_nLevel < constItemIdTemplate->mRequireLevelForHeroPresentPos4)
            {
                pos = 3;
            }
            else
            {
                pos = 4;
            }
        }
        else if (maxPresentHeroIter->mKey == 2)
        {
            if (mPlayer->getPlayerData().mBaseData.m_nLevel < constItemIdTemplate->mRequireLevelForHeroPresentPos3)
            {
                pos = 2;
            }
            else
            {
                pos = 3;
            }
        }
        else if (maxPresentHeroIter->mKey == 1)
        {
            if (mPlayer->getPlayerData().mBaseData.m_nLevel < constItemIdTemplate->mRequireLevelForHeroPresentPos2)
            {
                pos = 1;
            }
            else
            {
                pos = 2;
            }
        }

        if (presentPos > pos)
        {
            LOG_WARN("Failed to set hero present pos for permission indeny. %u %u", presentPos, pos);
            return LynxErrno::InvalidPermission;
        }
    }

    // 上面代码判断结束， 下面代码为操作更换代码

    // 将英雄出场位置设置为目标位置
    heroData->mBaseData.mPresentPos = presentPos;
    heroData->mDirtyFlag = true;
    
    // 如果目标位置有英雄， 需要将目标位置英雄修改为不出场
    Guid heroGuid2 = 0;
    Map<UInt8, HeroData*>::Iter* jter = mPlayerPresentHeroDataMap.find(presentPos);
    if (jter != NULL)
    {
        heroGuid2 = jter->mValue->mBaseData.mHeroId;
        jter->mValue->mBaseData.mPresentPos = 0;
        //notify.mOldHeroId = jter->mValue->mBaseData.mHeroId;
        //notify.mOldHeroPresentPos = 0;
        jter->mValue->mDirtyFlag = true;
        mPlayerPresentHeroDataMap.erase(jter);
    }
    mPlayerPresentHeroDataMap.insert(presentPos, heroData);

    // 更新玩家的出场英雄表
    mPlayer->updatePlayerPresentHeros();
    // 计算因为英雄出场修改导致的二级属性变化
    if (heroGuid2)
    {
        calculateHeroData(heroGuid2);
    }
    calculateHeroData(heroData->mBaseData.mHeroId);
    
    return LynxErrno::None;
}

ErrorId 
HeroManager::exchangeHeroPresentPos(const Guid& heroGuid1, const Guid& heroGuid2)
{
    // 判断两个英雄GUID是否相同
    if (heroGuid1 == heroGuid2)
    {
        LOG_WARN("Failed to exchangeHeroPresentPos for hero repeated.");
        return LynxErrno::HeroRepeat;
    }

    // 判断英雄1是否存在
    Map<Guid, HeroData*>::Iter* hero1Iter = mPlayerHeroDataMap.find(heroGuid1);
    if (!hero1Iter)
    {
        LOG_WARN("Failed to get hero %llu", heroGuid1);
        return LynxErrno::HeroNotExist;
    }

    HeroData* hero1 = hero1Iter->mValue;
    if (!hero1)
    {
        LOG_WARN("Failed to get hero %llu", heroGuid1);
        return LynxErrno::HeroNotExist;
    }

    // 判断英雄2是否存在
    Map<Guid, HeroData*>::Iter* hero2Iter = mPlayerHeroDataMap.find(heroGuid2);
    if (!hero2Iter)
    {
        LOG_WARN("Failed to get hero %llu", heroGuid2);
        return LynxErrno::HeroNotExist;
    }

    HeroData* hero2 = hero2Iter->mValue;
    if (!hero2)
    {
        LOG_WARN("Failed to get hero %llu", heroGuid2);
        return LynxErrno::HeroNotExist;
    }

    // 获得英雄1跟英雄2当前的出场位置
    UInt8 hero1Pos = hero1->mBaseData.mPresentPos;
    UInt8 hero2Pos = hero2->mBaseData.mPresentPos;

    // 校验英雄1的出场位置有效
    if (hero1Pos < 1 || hero1Pos > 5)
    {
        LOG_WARN("Failed to exchange hero pos for hero pos invalid.");
        return LynxErrno::InvalidParameter;
    }

    // 校验英雄2的出场位置有效
    if (hero2Pos < 1 || hero2Pos > 5)
    {
        LOG_WARN("Failed to exchange hero pos for hero pos invalid.");
        return LynxErrno::InvalidParameter;
    }

    // 如果英雄1==英雄2，操作无效
    if (hero1Pos == hero2Pos)
    {
        LOG_WARN("Failed to exchange hero pos for hero pos repeated.");
        return LynxErrno::InvalidOperation;
    }

    // 判断两个英雄的原模板ID不能相同
    if (hero1->mBaseData.mSrcTemplateId == hero2->mBaseData.mSrcTemplateId)
    {
        LOG_WARN("Failed to exchange hero for hero src template id %u repeated.", 
            hero1->mBaseData.mSrcTemplateId);
        return LynxErrno::HeroRepeat;
    }

    // 判断hero1不能上阵相同类型的英雄
    for (Map<UInt8, HeroData*>::Iter* i = mPlayerPresentHeroDataMap.begin();
        i != NULL; i = mPlayerPresentHeroDataMap.next(i))
    {
        if (i->mValue)
        {
            if (i->mValue->mBaseData.mHeroId != hero1->mBaseData.mHeroId)
            {
                if (i->mValue->mBaseData.mSrcTemplateId == hero1->mBaseData.mSrcTemplateId)
                {
                    LOG_WARN("Hero present repeat. %u", hero1->mBaseData.mSrcTemplateId);
                    return LynxErrno::HeroRepeat;
                }
            }
        }
    }

    // 判断hero2不能上阵相同类型的英雄
    for (Map<UInt8, HeroData*>::Iter* i = mPlayerPresentHeroDataMap.begin();
        i != NULL; i = mPlayerPresentHeroDataMap.next(i))
    {
        if (i->mValue)
        {
            if (i->mValue->mBaseData.mHeroId != hero2->mBaseData.mHeroId)
            {
                if (i->mValue->mBaseData.mSrcTemplateId == hero2->mBaseData.mSrcTemplateId)
                {
                    LOG_WARN("Hero present repeat. %u", hero2->mBaseData.mSrcTemplateId);
                    return LynxErrno::HeroRepeat;
                }
            }
        }
    }

    // 判断逻辑结束，执行更换操作
    mPlayerPresentHeroDataMap.erase(hero1Pos);
    mPlayerPresentHeroDataMap.erase(hero2Pos);

    hero1->mBaseData.mPresentPos = hero2Pos;
    hero1->mDirtyFlag = true;
    
    hero2->mBaseData.mPresentPos = hero1Pos;
    hero2->mDirtyFlag = true;

    mPlayerPresentHeroDataMap.insert(hero2Pos, hero1);
    mPlayerPresentHeroDataMap.insert(hero1Pos, hero2);
    
    // 更新玩家的出场英雄表
    mPlayer->updatePlayerPresentHeros();
    // 计算因为英雄出场修改导致的二级属性变化
    calculateHeroData(heroGuid1);
    calculateHeroData(heroGuid2);

    return LynxErrno::None;
}


void 
HeroManager::increaseHeroLevel(const Guid& heroId, UInt16 offset)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return;
    }

    HeroData* heroData = iter->mValue;
    if (!heroData)
    {
        LOG_WARN("Hero pointer is null.");
        return;
    }

    if (heroData->mBaseData.mLevel + offset >= HERO_LEVEL_MAX_VALUE)
    {
        LOG_WARN("Hero level overflow %u %u", heroData->mBaseData.mLevel, offset);
        offset = HERO_LEVEL_MAX_VALUE - heroData->mBaseData.mLevel;
        heroData->mBaseData.mLevel = HERO_LEVEL_MAX_VALUE;
    }
    else
    {
        heroData->mBaseData.mLevel += offset;
    }

    heroData->mDirtyFlag = true;
    calculateHeroData(heroId);

    SyncHeroLevelNotify notify;
    notify.mHeroId = heroId;
    notify.mLevel = heroData->mBaseData.mLevel;
    mPlayer->sendMsgWithAck(notify);
}

void 
HeroManager::reduceHeroLevel(const Guid& heroId, UInt16 offset)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return;
    }

    HeroData* heroData = iter->mValue;
    if (!heroData)
    {
        LOG_WARN("Hero pointer is null.");
        return;
    }

    if (heroData->mBaseData.mLevel < offset )
    {
        LOG_WARN("Hero level overflow %u %u", heroData->mBaseData.mLevel, offset);
        offset = heroData->mBaseData.mLevel;
        heroData->mBaseData.mLevel = 1;
    }
    else
    {
        heroData->mBaseData.mLevel -= offset;
    }

    heroData->mDirtyFlag = true;
    calculateHeroData(heroId);

    SyncHeroLevelNotify notify;
    notify.mHeroId = heroId;
    notify.mLevel = heroData->mBaseData.mLevel;
    mPlayer->sendMsgWithAck(notify);
}

void 
HeroManager::increaseHeroLevelExp(const Guid& heroId, UInt32 offset)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return;
    }

    HeroData* heroData = iter->mValue;
    if (!heroData)
    {
        LOG_WARN("Hero pointer is null.");
        return;
    }

    if (heroData->mBaseData.mLevelExp + offset >= HERO_EXP_MAX_VALUE)
    {
        LOG_WARN("Hero exp overflow %u %u", heroData->mBaseData.mLevelExp, offset);
        offset = HERO_EXP_MAX_VALUE - heroData->mBaseData.mLevelExp;
        heroData->mBaseData.mLevelExp = HERO_EXP_MAX_VALUE;
    }
    else
    {
        heroData->mBaseData.mLevelExp += offset;
    }

    heroData->mDirtyFlag = true;
    //calculateHeroData(heroId);

    SyncHeroLevelExpNotify notify;
    notify.mHeroId = heroId;
    notify.mLevelExp = heroData->mBaseData.mLevelExp;
    mPlayer->sendMsgWithAck(notify);
}

void 
HeroManager::reduceHeroLevelExp(const Guid& heroId, UInt32 offset)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return;
    }

    HeroData* heroData = iter->mValue;
    if (!heroData)
    {
        LOG_WARN("Hero pointer is null.");
        return;
    }

    if (heroData->mBaseData.mLevelExp < offset)
    {
        LOG_WARN("Hero exp overflow %u %u", heroData->mBaseData.mLevelExp, offset);
        offset = heroData->mBaseData.mLevelExp;
        heroData->mBaseData.mLevelExp = 0;
    }
    else
    {
        heroData->mBaseData.mLevelExp -= offset;
    }

    heroData->mDirtyFlag = true;
    //calculateHeroData(heroId);

    SyncHeroLevelExpNotify notify;
    notify.mHeroId = heroId;
    notify.mLevelExp = heroData->mBaseData.mLevelExp;
    mPlayer->sendMsgWithAck(notify);
}

void 
HeroManager::increaseHeroAdvanced(const Guid& heroId, UInt32 offset)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return;
    }

    HeroData* heroData = iter->mValue;
    if (!heroData)
    {
        LOG_WARN("Hero pointer is null.");
        return;
    }

    if (heroData->mBaseData.mAdvanced + offset >= HERO_ADVANCED_MAX_VALUE)
    {
        LOG_WARN("Hero advanced overflow %u %u", heroData->mBaseData.mAdvanced, offset);
        offset = HERO_ADVANCED_MAX_VALUE - heroData->mBaseData.mAdvanced;
        heroData->mBaseData.mAdvanced = HERO_ADVANCED_MAX_VALUE;
    }
    else
    {
        heroData->mBaseData.mAdvanced += offset;
    }

    heroData->mDirtyFlag = true;
    calculateHeroData(heroId);

    SyncHeroAdvancedNotify notify;
    notify.mHeroId = heroId;
    notify.mAdvanced = heroData->mBaseData.mAdvanced;
    mPlayer->sendMsgWithAck(notify);
}

void 
HeroManager::reduceHeroAdvanced(const Guid& heroId, UInt32 offset)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return;
    }

    HeroData* heroData = iter->mValue;
    if (!heroData)
    {
        LOG_WARN("Hero pointer is null.");
        return;
    }

    if (heroData->mBaseData.mAdvanced < offset)
    {
        LOG_WARN("Hero exp overflow %u %u", heroData->mBaseData.mAdvanced, offset);
        offset = heroData->mBaseData.mAdvanced;
        heroData->mBaseData.mAdvanced = 0;
    }
    else
    {
        heroData->mBaseData.mAdvanced -= offset;
    }

    heroData->mDirtyFlag = true;
    calculateHeroData(heroId);

    SyncHeroAdvancedNotify notify;
    notify.mHeroId = heroId;
    notify.mAdvanced = heroData->mBaseData.mAdvanced;
    mPlayer->sendMsgWithAck(notify);
}

void 
HeroManager::increaseHeroConsumeGold(const Guid& heroId, UInt32 offset)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return;
    }

    HeroData* heroData = iter->mValue;
    if (!heroData)
    {
        LOG_WARN("Hero pointer is null.");
        return;
    }

    if (heroData->mBaseData.mConsumeGold + offset >= PLAYER_GOLD_MAX_VALUE)
    {
        LOG_WARN("Hero consume gold overflow %u %u", heroData->mBaseData.mConsumeGold, offset);
        offset = PLAYER_GOLD_MAX_VALUE - heroData->mBaseData.mConsumeGold;
        heroData->mBaseData.mConsumeGold = PLAYER_GOLD_MAX_VALUE;
    }
    else
    {
        heroData->mBaseData.mConsumeGold += offset;
    }

    heroData->mDirtyFlag = true;
    //calculateHeroData(heroId);

    SyncHeroConsumeGoldNotify notify;
    notify.mHeroId = heroId;
    notify.mConsumeGold = heroData->mBaseData.mConsumeGold;
    mPlayer->sendMsgWithAck(notify);
}

void 
HeroManager::reduceHeroConsumeGold(const Guid& heroId, UInt32 offset)
{
    Map<Guid, HeroData*>::Iter* iter = mPlayerHeroDataMap.find(heroId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", heroId);
        return;
    }

    HeroData* heroData = iter->mValue;
    if (!heroData)
    {
        LOG_WARN("Hero pointer is null.");
        return;
    }

    if (heroData->mBaseData.mConsumeGold < offset)
    {
        LOG_WARN("Hero consume gold overflow %u %u", heroData->mBaseData.mConsumeGold, offset);
        offset = heroData->mBaseData.mConsumeGold;
        heroData->mBaseData.mConsumeGold = 0;
    }
    else
    {
        heroData->mBaseData.mConsumeGold -= offset;
    }

    heroData->mDirtyFlag = true;
    //calculateHeroData(heroId);

    SyncHeroConsumeGoldNotify notify;
    notify.mHeroId = heroId;
    notify.mConsumeGold = heroData->mBaseData.mConsumeGold;
    mPlayer->sendMsgWithAck(notify);
}

ErrorId
HeroManager::sellHero(List<Guid>& sellList, UInt32& getGold)
{
    for (List<Guid>::Iter* it = sellList.begin();
        it != NULL; it = sellList.next(it))
    {
        Map<Guid, HeroData*>::Iter* existIter = mPlayerHeroDataMap.find(it->mValue);
        if (!existIter)
        {
            LOG_WARN("Failed to get hero by id: %llu", it->mValue);
            return LynxErrno::InvalidParameter;
        }
        if (existIter->mValue->mBaseData.mPresentPos)
        {
            LOG_WARN("can not sell hero, hero is on present");
            return LynxErrno::InvalidParameter;
        }

    }
    for (List<Guid>::Iter* it = sellList.begin();
        it != NULL; it = sellList.next(it))
    {
        Map<Guid, HeroData*>::Iter* existIter = mPlayerHeroDataMap.find(it->mValue);
        HeroTemplate* heroTemplate = HERO_TABLE().get(existIter->mValue->mBaseData.mTemplateId);
        if (!heroTemplate)
        {
            LOG_WARN("Failed to get heroTemplate by id:%lu", existIter->mValue->mBaseData.mTemplateId);
            continue;
        }
        destroyHero(existIter->mValue->mBaseData.mHeroId);
        mPlayer->increasePlayerGold(heroTemplate->mDestroyGold);
        getGold += heroTemplate->mDestroyGold;
    }
    return LynxErrno::None;
}

bool 
HeroManager::isHeroFull(List<TemplateId>& heroList)
{
    return (mPlayerHeroDataMap.size() + heroList.size()) > BAG_MAX_SIZE;
}
