#include "PetManager.h"
#include "GameServer.h"

using namespace Lynx;

PetManager::PetManager() : mPlayer(NULL)/*, mPresentPetData(NULL)*/
{

}


PetManager::~PetManager()
{

}

bool 
PetManager::initial(Player* player)
{
    //注释宠物字段 sec
	/*if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }

    mPlayer = player;
    mPlayerPetData = &player->mPlayerData.mPetData;

    for (List<PetData>::Iter* iter = mPlayerPetData->mPetDataList.begin();
        iter != NULL; iter = mPlayerPetData->mPetDataList.next(iter))
    {
        mPlayerPetDataMap.insert(iter->mValue.mBaseData.mPetId, &iter->mValue);
        if (iter->mValue.mBaseData.mPresentPos > 0)
        {
            mPresentPetData = &iter->mValue;
        }
    }

    mPetTalentDataMap.clear();*/

    return true;
}

void 
PetManager::update(const UInt64& accTime)
{
    /*for (List<PetData>::Iter* iter = mPlayerPetData->mPetDataList.begin();
        iter != NULL; iter = mPlayerPetData->mPetDataList.next(iter))
    {
        if (iter->mValue.mDirtyFlag)
        {
            PersistUpdatePetNotify notify;
            notify.mPlayerGuid = mPlayer->getPlayerGuid();
            notify.mPetData = iter->mValue;
            PersistSystem::getSingleton().postThreadMsg(notify, mPlayer->getPlayerGuid());
            iter->mValue.mDirtyFlag = false;
        }
    }*/
}

void 
PetManager::calculatePetData(const Guid& petId)
{
   /* Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get hero [%llu] for not exist.", petId);
        return;
    }

    PetData* petData = iter->mValue;
    if (!petData)
    {
        LOG_WARN("PetData pointer is NULL.");
        return;
    }

    const PlayerData& playerData = mPlayer->getPlayerData();

    CalculateUtil::calculatePetData(playerData, *petData);

    if (petData->mBaseData.mPresentPos != 0)
    {
        mPresentPetData = petData;
        mPlayer->calculatePlayerData();
        mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_BATTLE_VALUE);
    }*/
}

void 
PetManager::release()
{

}

//const PetData* 
//PetManager::createPet(const TemplateId& templateId, bool promp/*promp = true*/)
//{
//    PetTemplate* petTemplate = PET_TABLE().get(templateId);
//    if (!petTemplate)
//    {
//        LOG_WARN("Failed to get pet template by template id %u", templateId);
//        return NULL;
//    }
//
//    PetData petData;
//    petData.mBaseData.mTemplateId = templateId;
//    petData.mBaseData.mPetId = LogicSystem::getSingleton().generatePetGuid();
//    petData.mBaseData.mPetName = petTemplate->mName.c_str();
//    petData.mBaseData.mStarLevel = 0;//petTemplate->mStarLevel;
//    petData.mBaseData.mLevel = 1;
//    petData.mBaseData.mAttack = petTemplate->mAttack;
//    petData.mBaseData.mBattleValue = 0;
//    petData.mBaseData.mLevelExp = 0;
//    petData.mBaseData.mPresentPos = 0;
//    petData.mBaseData.mStarLevelExp = 0;
//
//    petData.mTalentData.mAttack = 0;//petTemplate->mAttack;
//    petData.mTalentData.mHp = 0;
//    petData.mTalentData.mMagicDefense = 0;
//    petData.mTalentData.mPhysicsDefense = 0;
//
//
//	//sec 宠物技能注释
//    petData.mSkillData.mCommonSkill.mTemplateId = petTemplate->mSkillId;
//    if (petTemplate->mSkillId != 0)
//    {
//        SkillTemplate* skillTemplate = SKILL_TABLE().get(petTemplate->mSkillId);
//        if (skillTemplate == NULL)
//        {
//            LOG_WARN("Get skill template failed id[%u]", petTemplate->mSkillId);
//            return false;
//        }
//        //petData.mSkillData.mCommonSkill.mSkillAttackType = skillTemplate->mAttackType;
//    }
//
//    List<PetData>::Iter* iter = mPlayerPetData->mPetDataList.insertTail(petData);
//    if (iter == NULL)
//    {
//        LOG_WARN("Failed to insert pet data into pet data list.");
//        return NULL;
//    }
//
//    mPlayerPetDataMap.insert(iter->mValue.mBaseData.mPetId, &iter->mValue);
//
//    PersistInsertPetNotify persistInsertPetNotify;
//    persistInsertPetNotify.mPlayerGuid = mPlayer->getPlayerGuid();
//    persistInsertPetNotify.mPetData = iter->mValue;
//    PersistSystem::getSingleton().postThreadMsg(persistInsertPetNotify, mPlayer->getPlayerGuid());
//
//    AddPetNotify notify;
//    notify.mPetData = iter->mValue;
//    notify.mPromp = true;
//    mPlayer->sendMsgWithAck(notify);
//
//    mPlayer->updatePlayerPresentPet();
//    calculatePetData(petData.mBaseData.mPetId);
//
//    return &iter->mValue;
//}

bool 
PetManager::destroyPet(Guid guid)
{
   /* Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(guid);
    if (iter == NULL)
    {
        LOG_WARN("Failed to destroy pet [%llu] for not exist.", guid);
        return false;
    }

    if (!iter->mValue)
    {
        LOG_WARN("Pet pointer is null.");
        return false;
    }

    UInt8 presentPos = iter->mValue->mBaseData.mPresentPos;
    mPlayerPetDataMap.erase(iter);

    for (List<PetData>::Iter* jter = mPlayerPetData->mPetDataList.begin();
        jter != NULL; jter = mPlayerPetData->mPetDataList.next(jter))
    {
        if (jter->mValue.mBaseData.mPetId == guid)
        {
            mPlayerPetData->mPetDataList.erase(jter);
            break;
        }
    }

    PersistDelPetNotify persistDelPetNotify;
    persistDelPetNotify.mPetGuid = guid;
    persistDelPetNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(persistDelPetNotify, mPlayer->getPlayerGuid());

    DelPetNotify notify;
    notify.mPetGuid = guid;
    mPlayer->sendMsgWithAck(notify);

    if (presentPos > 0)
    {
        mPresentPetData = NULL;
        mPlayer->updatePlayerPresentPet();
        mPlayer->calculatePlayerData();
        mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_BATTLE_VALUE);
    }*/
    return true;
}

//const PetData* 
//PetManager::getPetDataByGuid(const Guid& guid) const
//{
//    Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(guid);
//    if (iter == NULL)
//    {
//        LOG_WARN("Failed to get pet [%llu] for not exist.", guid);
//        return NULL;
//    }
//    
//    return iter->mValue;
//}

//const PetData* 
//PetManager::getPresentPet() const
//{
//    return mPresentPetData;
//}

void 
PetManager::increasePetLevel(const Guid& petId, UInt16 offset)
{
  
	/* Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
        return;
    }

    PetData* petData = iter->mValue;
    if (!petData)
    {
        LOG_WARN("Pet pointer is null.");
        return;
    }

    if (petData->mBaseData.mLevel + offset >= PET_LEVEL_MAX_VALUE)
    {
        LOG_WARN("Pet level overflow %u %u", petData->mBaseData.mLevel, offset);
        offset = PET_LEVEL_MAX_VALUE - petData->mBaseData.mLevel;
        petData->mBaseData.mLevel = PET_LEVEL_MAX_VALUE;
    }
    else
    {
        petData->mBaseData.mLevel += offset;
    }

    petData->mDirtyFlag = true;
    calculatePetData(petId);

    SyncPetLevelNotify notify;
    notify.mPetId = petId;
    notify.mLevel = petData->mBaseData.mLevel;
    mPlayer->sendMsgWithAck(notify);

    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_PET_LEVEL);*/
}

void 
PetManager::reducePetLevel(const Guid& petId, UInt16 offset)
{
   /* Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
        return;
    }

    PetData* petData = iter->mValue;
    if (!petData)
    {
        LOG_WARN("Pet pointer is null.");
        return;
    }

    if (petData->mBaseData.mLevel < offset)
    {
        LOG_WARN("Pet level overflow %u %u", petData->mBaseData.mLevel, offset);
        offset = petData->mBaseData.mLevel;
        petData->mBaseData.mLevel = 1;
    }
    else
    {
        petData->mBaseData.mLevel -= offset;
    }

    petData->mDirtyFlag = true;
    calculatePetData(petId);

    SyncPetLevelNotify notify;
    notify.mPetId = petId;
    notify.mLevel = petData->mBaseData.mLevel;
    mPlayer->sendMsgWithAck(notify);*/
}

void 
PetManager::increasePetLevelExp(const Guid& petId, UInt32 offset)
{

//    Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
//    if (iter == NULL)
//    {
//        LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
//        return;
//    }
//
//    PetData* petData = iter->mValue;
//    if (!petData)
//    {
//        LOG_WARN("Pet pointer is null.");
//        return;
//    }
//
//    if (petData->mBaseData.mLevelExp + offset >= PET_EXP_MAX_VALUE)
//    {
//        LOG_WARN("Pet exp overflow %u %u", petData->mBaseData.mLevelExp, offset);
//        offset = PET_EXP_MAX_VALUE - petData->mBaseData.mLevelExp;
//        petData->mBaseData.mLevelExp = PET_EXP_MAX_VALUE;
//    }
//    else
//    {
//        petData->mBaseData.mLevelExp += offset;
//    }
//
//    petData->mDirtyFlag = true;
//    //calculateRuneData(petId);
//
//    SyncPetLevelExpNotify notify;
//    notify.mPetId = petId;
//    notify.mLevelExp = petData->mBaseData.mLevelExp;
//    mPlayer->sendMsgWithAck(notify);

}

void 
PetManager::reducePetLevelExp(const Guid& petId, UInt32 offset)
{
    //Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
    //    return;
    //}

    //PetData* petData = iter->mValue;
    //if (!petData)
    //{
    //    LOG_WARN("Pet pointer is null.");
    //    return;
    //}

    //if (petData->mBaseData.mLevelExp < offset)
    //{
    //    LOG_WARN("Pet exp overflow %u %u", petData->mBaseData.mLevelExp, offset);
    //    offset = petData->mBaseData.mLevelExp;
    //    petData->mBaseData.mLevelExp = 0;
    //}
    //else
    //{
    //    petData->mBaseData.mLevelExp -= offset;
    //}

    //petData->mDirtyFlag = true;
    ////calculateRuneData(petId);

    //SyncPetLevelExpNotify notify;
    //notify.mPetId = petId;
    //notify.mLevelExp = petData->mBaseData.mLevelExp;
    //mPlayer->sendMsgWithAck(notify);
}


void 
PetManager::increasePetStarLevelExp(const Guid& petId, UInt32 offset)
{
    //Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
    //    return;
    //}

    //PetData* petData = iter->mValue;
    //if (!petData)
    //{
    //    LOG_WARN("Pet pointer is null.");
    //    return;
    //}

    //if (petData->mBaseData.mStarLevelExp + offset >= PET_STAR_LEVEL_EXP_MAX_VALUE)
    //{
    //    LOG_WARN("Pet star exp overflow %u %u", petData->mBaseData.mStarLevelExp, offset);
    //    offset = PET_STAR_LEVEL_EXP_MAX_VALUE - petData->mBaseData.mStarLevelExp;
    //    petData->mBaseData.mStarLevelExp = PET_STAR_LEVEL_EXP_MAX_VALUE;
    //}
    //else
    //{
    //    petData->mBaseData.mStarLevelExp += offset;
    //}

    //petData->mDirtyFlag = true;
    ////calculateRuneData(petId);

    //SyncPetStarLevelExpNotify notify;
    //notify.mPetId = petId;
    //notify.mStarLevelExp = petData->mBaseData.mStarLevelExp;
    //mPlayer->sendMsgWithAck(notify);
}

void 
PetManager::reducePetStarLevelExp(const Guid& petId, UInt32 offset)
{
    //Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
    //    return;
    //}

    //PetData* petData = iter->mValue;
    //if (!petData)
    //{
    //    LOG_WARN("Pet pointer is null.");
    //    return;
    //}

    //if (petData->mBaseData.mStarLevelExp < offset)
    //{
    //    LOG_WARN("Pet star exp overflow %u %u", petData->mBaseData.mStarLevelExp, offset);
    //    offset = petData->mBaseData.mStarLevelExp;
    //    petData->mBaseData.mStarLevelExp = 0;
    //}
    //else
    //{
    //    petData->mBaseData.mStarLevelExp -= offset;
    //}

    //petData->mDirtyFlag = true;
    ////calculateRuneData(petId);

    //SyncPetStarLevelExpNotify notify;
    //notify.mPetId = petId;
    //notify.mStarLevelExp = petData->mBaseData.mStarLevelExp;
    //mPlayer->sendMsgWithAck(notify);
}

ErrorId 
PetManager::awakenPet(const Guid& petId, const List<Guid>& petList, const List<Guid>& petDebrisList,
                      UInt32& upLevel, TemplateId& newTemplateId)
{
    //Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
    //    return LynxErrno::PetNotExist;
    //}

    //PetData* petData = iter->mValue;
    //if (!petData)
    //{
    //    LOG_WARN("Pet pointer is null.");
    //    return LynxErrno::PetNotExist;
    //}

    //// 判断宠物是否存在
    //List<const PetData*> tmpPetList;
    //for (List<Guid>::Iter* iter = petList.begin();
    //    iter != NULL; iter = petList.next(iter))
    //{
    //    const PetData* pet = getPetDataByGuid(iter->mValue);
    //    if (!pet)
    //    {
    //        LOG_WARN("Failed to get pet [%llu] for not exist.", iter->mValue);
    //        return LynxErrno::PetNotExist;
    //    }
    //    //进过阶的不能被吞噬，升过级的可以被吞噬
    //    if (pet->mBaseData.mStarLevel != 0)
    //    {
    //        LOG_WARN("Failed to awaken pet: %llu is advanced.", pet->mBaseData.mPetId);
    //        return LynxErrno::UnknownLogicError;
    //    }

    //    tmpPetList.insertTail(pet);
    //}

    //// 判断碎片是否存在
    //List<const PetDebrisData*> tmpPetDebrisList;
    //for (List<Guid>::Iter* iter = petDebrisList.begin();
    //    iter != NULL; iter = petDebrisList.next(iter))
    //{
    //    const PetDebrisData* petDebris = mPlayer->getPetDebrisManager().getPetDebrisByGuid(iter->mValue);
    //    if (!petDebris)
    //    {
    //        LOG_WARN("Failed to get pet debris [%llu]", iter->mValue);
    //        return LynxErrno::PetDebrisNotExist;
    //    }
    //    tmpPetDebrisList.insertTail(petDebris);
    //}

    ////UInt32 totalStarLevelExp = 0;
    //UInt32 totalLevelExp = 0;

    //// 计算宠物经验与星级经验
    //for (List<const PetData*>::Iter* iter = tmpPetList.begin();
    //    iter != NULL; iter = tmpPetList.next(iter))
    //{
    //    PetTemplate* petTemplate = PET_TABLE().get(iter->mValue->mBaseData.mTemplateId);
    //    if (!petTemplate)
    //    {
    //        LOG_WARN("Failed to get pet template %u", iter->mValue->mBaseData.mTemplateId);
    //        return LynxErrno::TemplateNotExist;
    //    }

    //    // 计算宠物等级经验 = (宠物当前等级所有经验+ 当前剩余经验)
    //    {
    //        UInt32 currentLevel = iter->mValue->mBaseData.mLevel;
    //        PetLevelGrowTemplate* petLevelGrowTemplate = PET_LEVEL_GROW_TABLE().get(currentLevel);
    //        if (!petLevelGrowTemplate)
    //        {
    //            LOG_WARN("Failed to get pet level grow template %u", currentLevel);
    //            return LynxErrno::TemplateNotExist;
    //        }

    //        UInt32 petLevelExp = petLevelGrowTemplate->mTotalExp;
    //        petLevelExp += iter->mValue->mBaseData.mLevelExp;

    //        // 计算宠物销毁总经验 = 宠物等级经验 + 宠物销毁经验
    //        totalLevelExp += petLevelExp + petTemplate->mDestroyExp;
    //    }

    //    // 计算宠物星级经验 = (宠物当前星级所有经验 + 当前剩余星级经验 + 销毁返回的星级经验)
    //    {
    //        UInt32 currentStarLevel = iter->mValue->mBaseData.mStarLevel;
    //        PetStarGrowTemplate* petStarGrowTemplate = PET_STAR_GROW_TABLE().get(currentStarLevel);
    //        if (!petStarGrowTemplate)
    //        {
    //            LOG_WARN("Failed to get pet star grow template %u", currentStarLevel);
    //            return LynxErrno::TemplateNotExist;
    //        }

    //        UInt32 petStarExp = petStarGrowTemplate->mTotalExp;
    //        petStarExp += iter->mValue->mBaseData.mStarLevelExp;

    //        // 计算宠物销毁总星级经验 = 宠物星级经验 + 宠物销毁星经验
    //        totalStarLevelExp += petStarExp + petTemplate->mDestroyExp;
    //    }
    //}

    //// 计算碎片 只有销毁碎片返回星级经验
    //for (List<const PetDebrisData*>::Iter* iter = tmpPetDebrisList.begin();
    //    iter != NULL; iter = tmpPetDebrisList.next(iter))
    //{
    //    PetDebrisTemplate* petDebrisTemplate = PET_DEBRIS_TABLE().get(iter->mValue->mTemplateId);
    //    if (!petDebrisTemplate)
    //    {
    //        LOG_WARN("Failed to get pet debris template %u", iter->mValue->mTemplateId);
    //        return LynxErrno::TemplateNotExist;
    //    }
    //    totalLevelExp += petDebrisTemplate->mDestroyExp * iter->mValue->mCount;
    //}

    //// 执行操作
    //newTemplateId = 0;
    //if (addStarLevelExp(petId, newTemplateId,  totalStarLevelExp))
    //{
    //    LOG_WARN("Failed to add star level exp.");
    //    return LynxErrno::InvalidOperation;
    //}

    //if (newTemplateId != petData->mBaseData.mTemplateId)
    //{
    //    petData->mBaseData.mTemplateId = newTemplateId;
    //    petData->mDirtyFlag = true;
    //    SyncPetTemplateIdNotify notify;
    //    notify.mPetId = petData->mBaseData.mPetId;
    //    notify.mTemplateId = newTemplateId;
    //    mPlayer->sendMsgWithAck(notify);
    //}

    //upLevel = 0;
    //if (addLevelExp(petId, upLevel, totalLevelExp))
    //{
    //    LOG_WARN("Failed to add level exp.");
    //    return LynxErrno::InvalidOperation;
    //}    

    //// 删除宠物
    //for (List<Guid>::Iter* iter = petList.begin();
    //    iter != NULL; iter = petList.next(iter))
    //{
    //    destroyPet(iter->mValue);
    //}

    //// 删除宠物碎片
    //for (List<Guid>::Iter* iter = petDebrisList.begin();
    //    iter != NULL; iter = petDebrisList.next(iter))
    //{
    //    mPlayer->getPetDebrisManager().destroyPetDebris(iter->mValue); 
    //}

    return LynxErrno::None;
}

ErrorId 
PetManager::refinePet(const Guid& petId, UInt32& upLevelCount, const Guid& itemId)
{
    //Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
    //    return LynxErrno::PetNotExist;
    //}

    //PetData* petData = iter->mValue;
    //if (!petData)
    //{
    //    LOG_WARN("Pet pointer is null.");
    //    return LynxErrno::PetNotExist;
    //}

    //ConstItemIdTemplate* constItemIdTemplate = CONST_ITEM_ID_TABLE().get(1);
    //if (!constItemIdTemplate)
    //{
    //    LOG_WARN("Failed to get const item id template.");
    //    return LynxErrno::TemplateNotExist;
    //}

    //const ItemData* itemData = mPlayer->getItemManager().getItemByGuid(itemId);
    //if (!itemData)
    //{
    //    LOG_WARN("Failed to get item data by id %llu", itemId);
    //    return LynxErrno::ItemNotExist;
    //}

    //if (itemData->mCount <= 0)
    //{
    //    LOG_WARN("Item %llu count invalid.", itemData->mItemId);
    //    return LynxErrno::InvalidOperation;
    //}

    ////if (!(itemData->mTemplateId == constItemIdTemplate->mPetFoodId1 ||
    ////    itemData->mTemplateId == constItemIdTemplate->mPetFoodId2 ||
    ////    itemData->mTemplateId == constItemIdTemplate->mPetFoodId3 ||
    ////    itemData->mTemplateId == constItemIdTemplate->mPetFoodId4 ||
    ////    itemData->mTemplateId == constItemIdTemplate->mPetFoodId5))
    ////{
    ////    LOG_WARN("Item type isn't match. %u", itemData->mTemplateId);
    ////    return LynxErrno::InvalidOperation;
    ////}

    //ItemTemplate* itemTemplate = ITEM_TABLE().get(itemData->mTemplateId);
    //if (!itemTemplate)
    //{
    //    LOG_WARN("Failed to get item by template id %u", itemData->mTemplateId);
    //    return LynxErrno::TemplateNotExist;
    //}

    //// 执行操作
    //UInt32 incLevelExp = itemTemplate->mUseEffectValue;
    //upLevelCount = 0;

    //if (addLevelExp(petId, upLevelCount,  incLevelExp))
    //{
    //    LOG_WARN("Failed to add pet level exp.");
    //    return LynxErrno::InvalidOperation;
    //}

    //mPlayer->getItemManager().delItem(itemId, 1);

    return LynxErrno::None;
}

ErrorId 
PetManager::refinePet(const Guid& petId, UInt32& upLevelCount)
{
    //Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
    //    return LynxErrno::PetNotExist;
    //}

    //PetData* petData = iter->mValue;
    //if (!petData)
    //{
    //    LOG_WARN("Pet pointer is null.");
    //    return LynxErrno::PetNotExist;
    //}

    //ConstItemIdTemplate* constItemIdTemplate = CONST_ITEM_ID_TABLE().get(1);
    //if (!constItemIdTemplate)
    //{
    //    LOG_WARN("Failed to get const item id template.");
    //    return LynxErrno::TemplateNotExist;
    //}

    //const ItemData* itemData1 = mPlayer->getItemManager().getItemByTemplateId(constItemIdTemplate->mPetFoodId1);
    //const ItemData* itemData2 = mPlayer->getItemManager().getItemByTemplateId(constItemIdTemplate->mPetFoodId2);
    //const ItemData* itemData3 = mPlayer->getItemManager().getItemByTemplateId(constItemIdTemplate->mPetFoodId3);
    //const ItemData* itemData4 = mPlayer->getItemManager().getItemByTemplateId(constItemIdTemplate->mPetFoodId4);
    //const ItemData* itemData5 = mPlayer->getItemManager().getItemByTemplateId(constItemIdTemplate->mPetFoodId5);

    //UInt32 incLevelExp = 0;

    //if (itemData1)
    //{
    //    ItemTemplate* itemTemplate = ITEM_TABLE().get(itemData1->mTemplateId);
    //    if (!itemTemplate)
    //    {
    //        LOG_WARN("Failed to get item by template id %u", itemData1->mTemplateId);
    //        return LynxErrno::TemplateNotExist;
    //    }

    //    incLevelExp += itemTemplate->mUseEffectValue * itemData1->mCount;
    //}

    //if (itemData2)
    //{
    //    ItemTemplate* itemTemplate = ITEM_TABLE().get(itemData2->mTemplateId);
    //    if (!itemTemplate)
    //    {
    //        LOG_WARN("Failed to get item by template id %u", itemData2->mTemplateId);
    //        return LynxErrno::TemplateNotExist;
    //    }

    //    incLevelExp += itemTemplate->mUseEffectValue * itemData2->mCount;
    //}

    //if (itemData3)
    //{
    //    ItemTemplate* itemTemplate = ITEM_TABLE().get(itemData3->mTemplateId);
    //    if (!itemTemplate)
    //    {
    //        LOG_WARN("Failed to get item by template id %u", itemData3->mTemplateId);
    //        return LynxErrno::TemplateNotExist;
    //    }

    //    incLevelExp += itemTemplate->mUseEffectValue * itemData3->mCount;
    //}

    //if (itemData4)
    //{
    //    ItemTemplate* itemTemplate = ITEM_TABLE().get(itemData4->mTemplateId);
    //    if (!itemTemplate)
    //    {
    //        LOG_WARN("Failed to get item by template id %u", itemData4->mTemplateId);
    //        return LynxErrno::TemplateNotExist;
    //    }

    //    incLevelExp += itemTemplate->mUseEffectValue * itemData4->mCount;
    //}

    //if (itemData5)
    //{
    //    ItemTemplate* itemTemplate = ITEM_TABLE().get(itemData5->mTemplateId);
    //    if (!itemTemplate)
    //    {
    //        LOG_WARN("Failed to get item by template id %u", itemData5->mTemplateId);
    //        return LynxErrno::TemplateNotExist;
    //    }

    //    incLevelExp += itemTemplate->mUseEffectValue * itemData5->mCount;
    //}

    //// 执行操作
    //upLevelCount = 0;

    //if (addLevelExp(petId, upLevelCount,  incLevelExp))
    //{
    //    LOG_WARN("Failed to add pet level exp.");
    //    return LynxErrno::InvalidOperation;
    //}

    //if (itemData1)
    //{
    //    mPlayer->getItemManager().destroyItem(itemData1->mItemId);
    //}

    //if (itemData2)
    //{
    //    mPlayer->getItemManager().destroyItem(itemData2->mItemId);
    //}

    //if (itemData3)
    //{
    //    mPlayer->getItemManager().destroyItem(itemData3->mItemId);
    //}

    //if (itemData4)
    //{
    //    mPlayer->getItemManager().destroyItem(itemData4->mItemId);
    //}

    //if (itemData5)
    //{
    //    mPlayer->getItemManager().destroyItem(itemData5->mItemId);
    //}

    //petData->mDirtyFlag = true;

    return LynxErrno::None;
}

//ErrorId 
//PetManager::getCommonTalent(const Guid& petId, PetTalentData& petTalentData)
//{
//    //sec注释原游戏代码，获得普通人
//	Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
//    if (iter == NULL)
//    {
//        LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
//        return LynxErrno::PetNotExist;
//    }
//
//    PetData* petData = iter->mValue;
//    if (!petData)
//    {
//        LOG_WARN("Pet pointer is null.");
//        return LynxErrno::PetNotExist;
//    }
//
//    PetTemplate* petTemplate = PET_TABLE().get(petData->mBaseData.mTemplateId);
//    if (!petTemplate)
//    {
//        LOG_WARN("Failed to get pet template %u", petData->mBaseData.mTemplateId);
//        return LynxErrno::TemplateNotExist;
//    }
//
//    ConstItemIdTemplate* cit = CONST_ITEM_ID_TABLE().get(1);
//    if (!cit)
//    {
//        LOG_WARN("Failed to get const item id template Id:1");
//        return LynxErrno::TemplateNotExist;
//    }
//
//    const ItemData* id = mPlayer->getItemManager().getItemByTemplateId(cit->mPetTalentItemId);
//    if (!id)
//    {
//        LOG_WARN("Pet talent item is not exist. id:%u", cit->mPetTalentItemId);
//        return LynxErrno::ItemNotExist;
//    }
//
//    if (id->mCount < cit->mPetTalentItemDiamondCount)
//    {
//        LOG_WARN("Pet talent item is not enough.");
//        return LynxErrno::UnknownLogicError;
//    }
//
//    mPlayer->getItemManager().delItem(cit->mPetTalentItemId, cit->mPetTalentItemDiamondCount);
//    // 目前用存随机计算
//    UInt32 talentHpUp = petTemplate->mTalent.mHpUpper + petTemplate->mTalent.mHpUpperUp * petData->mBaseData.mLevel;
//    UInt32 talentAttackUp = petTemplate->mTalent.mAttackUpper + petTemplate->mTalent.mAttackUpperUp * petData->mBaseData.mLevel;
//    UInt32 talentPhysicsDefenseUp = petTemplate->mTalent.mPhysicsDefenseUpper + petTemplate->mTalent.mPhysicsDefenseUpperUp * petData->mBaseData.mLevel;
//    UInt32 talentMagicDefenseUp = petTemplate->mTalent.mMagicDefenseUpper + petTemplate->mTalent.mMagicDefenseUpperUp * petData->mBaseData.mLevel;
//
//    UInt32 randomHp = Random::getSingleton().getUInt32Random(0, talentHpUp);
//    UInt32 randomAttack = Random::getSingleton().getUInt32Random(0, talentAttackUp);
//    UInt32 randomPhysicsDefense = Random::getSingleton().getUInt32Random(0, talentPhysicsDefenseUp);
//    UInt32 randomMagicDefense = Random::getSingleton().getUInt32Random(0, talentMagicDefenseUp);
//
//    petTalentData.mHp = randomHp;
//    petTalentData.mAttack = randomAttack;
//    petTalentData.mPhysicsDefense = randomPhysicsDefense;
//    petTalentData.mMagicDefense = randomPhysicsDefense;
//
//    mPetTalentDataMap.insert(petId, petTalentData);
//    return LynxErrno::None;
//}

//ErrorId 
//PetManager::getAdvanceTalent(const Guid& petId, PetTalentData& petTalentData)
//{
//	//获取超级人才 sec注释原游戏代码   
//	Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
//    if (iter == NULL)
//    {
//        LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
//        return LynxErrno::PetNotExist;
//    }
//
//    PetData* petData = iter->mValue;
//    if (!petData)
//    {
//        LOG_WARN("Pet pointer is null.");
//        return LynxErrno::PetNotExist;
//    }
//
//    PetTemplate* petTemplate = PET_TABLE().get(petData->mBaseData.mTemplateId);
//    if (!petTemplate)
//    {
//        LOG_WARN("Failed to get pet template %u", petData->mBaseData.mTemplateId);
//        return LynxErrno::TemplateNotExist;
//    }
//
//    ConstItemIdTemplate* cit = CONST_ITEM_ID_TABLE().get(1);
//    if (!cit)
//    {
//        LOG_WARN("Failed to get const item id template Id:1");
//        return LynxErrno::TemplateNotExist;
//    }
//
//    const ItemData* id = mPlayer->getItemManager().getItemByTemplateId(cit->mPetTalentItemId);
//    if (!id)
//    {
//        LOG_WARN("Pet talent item is not exist. id:%u", cit->mPetTalentItemId);
//        return LynxErrno::ItemNotExist;
//    }
//
//    if (id->mCount < cit->mPetTalentItemDiamondCount)
//    {
//        LOG_WARN("Pet talent item is not enough.");
//        return LynxErrno::UnknownLogicError;
//    }
//
//    mPlayer->getItemManager().delItem(id->mItemId, cit->mPetTalentItemDiamondCount);
//
//    // 目前用存随机计算
//    UInt32 talentHpUp = petTemplate->mTalent.mHpUpper + petTemplate->mTalent.mHpUpperUp * petData->mBaseData.mLevel;
//    UInt32 talentAttackUp = petTemplate->mTalent.mAttackUpper + petTemplate->mTalent.mAttackUpperUp * petData->mBaseData.mLevel;
//    UInt32 talentPhysicsDefenseUp = petTemplate->mTalent.mPhysicsDefenseUpper + petTemplate->mTalent.mPhysicsDefenseUpperUp * petData->mBaseData.mLevel;
//    UInt32 talentMagicDefenseUp = petTemplate->mTalent.mMagicDefenseUpper + petTemplate->mTalent.mMagicDefenseUpperUp * petData->mBaseData.mLevel;
//
//    UInt32 randomHp = Random::getSingleton().getUInt32Random(0, talentHpUp);
//    UInt32 randomAttack = Random::getSingleton().getUInt32Random(0, talentAttackUp);
//    UInt32 randomPhysicsDefense = Random::getSingleton().getUInt32Random(0, talentPhysicsDefenseUp);
//    UInt32 randomMagicDefense = Random::getSingleton().getUInt32Random(0, talentMagicDefenseUp);
//
//    petTalentData.mHp = randomHp;
//    petTalentData.mAttack = randomAttack;
//    petTalentData.mPhysicsDefense = randomPhysicsDefense;
//    petTalentData.mMagicDefense = randomPhysicsDefense;
//
//    mPetTalentDataMap.insert(petId, petTalentData);
//
//    return LynxErrno::None;
//}

ErrorId 
PetManager::saveTalent(const Guid& petId)
{
   /* Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
        return LynxErrno::PetNotExist;
    }

    PetData* petData = iter->mValue;
    if (!petData)
    {
        LOG_WARN("Pet pointer is null.");
        return LynxErrno::PetNotExist;
    }

    Map<Guid, PetTalentData>::Iter* talentDataIter = mPetTalentDataMap.find(petId);
    if (!talentDataIter)
    {
        LOG_WARN("Failed to get pet talent data by pet %llu", petId);
        return LynxErrno::InvalidOperation;
    }

    petData->mTalentData.mHp = talentDataIter->mValue.mHp;
    petData->mTalentData.mAttack = talentDataIter->mValue.mAttack;
    petData->mTalentData.mPhysicsDefense = talentDataIter->mValue.mPhysicsDefense;
    petData->mTalentData.mMagicDefense = talentDataIter->mValue.mMagicDefense;

    mPetTalentDataMap.erase(talentDataIter);

    SyncPetTalentNotify notify;

    notify.mPetId = petId;
    notify.mTalentData = petData->mTalentData;
    mPlayer->sendMsgWithAck(notify);

    petData->mDirtyFlag = true;*/

    return LynxErrno::None;
}

ErrorId 
PetManager::cancelTalent(const Guid& petId)
{
   /* Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
        return LynxErrno::PetNotExist;
    }

    PetData* petData = iter->mValue;
    if (!petData)
    {
        LOG_WARN("Pet pointer is null.");
        return LynxErrno::PetNotExist;
    }

    Map<Guid, PetTalentData>::Iter* talentDataIter = mPetTalentDataMap.find(petId);
    if (!talentDataIter)
    {
        LOG_WARN("Failed to get pet talent data by pet %llu", petId);
        return LynxErrno::InvalidOperation;
    }

    mPetTalentDataMap.erase(talentDataIter);*/

    return LynxErrno::None;
}

ErrorId 
PetManager::addLevelExp(const Guid& petId, UInt32& upLevelCount, UInt32 incLevelExp)
{
    //Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
    //    return LynxErrno::PetNotExist;
    //}

    //PetData* petData = iter->mValue;
    //if (!petData)
    //{
    //    LOG_WARN("Pet pointer is null.");
    //    return LynxErrno::PetNotExist;
    //}

    //// 计算增加的经验可以升几级
    //UInt32 remainExp = petData->mBaseData.mLevelExp + incLevelExp;
    //upLevelCount = 0;
    //do
    //{
    //    PetLevelGrowTemplate* petLevelGrowTemplate = PET_LEVEL_GROW_TABLE().get(petData->mBaseData.mLevel + upLevelCount);
    //    if (!petLevelGrowTemplate)
    //    {
    //        LOG_WARN("Failed to get pet grow template by current level %u", 
    //            petData->mBaseData.mLevel + upLevelCount);
    //        break;
    //    }

    //    if (remainExp >= petLevelGrowTemplate->mExperience)
    //    {
    //        remainExp -= petLevelGrowTemplate->mExperience;
    //        upLevelCount++;
    //    }
    //    else if (remainExp < petLevelGrowTemplate->mExperience)
    //    {
    //        break;
    //    }
    //} while (1);

    //if (upLevelCount > 0)
    //{
    //    increasePetLevel(petId, upLevelCount);
    //}

    //if (remainExp > petData->mBaseData.mLevelExp)
    //{
    //    increasePetLevelExp(petId, remainExp - petData->mBaseData.mLevelExp);
    //}
    //else if (remainExp < petData->mBaseData.mLevelExp)
    //{
    //    reducePetLevelExp(petId, petData->mBaseData.mLevelExp - remainExp);
    //}

    //petData->mDirtyFlag = true;
    return LynxErrno::None;
}

ErrorId
PetManager::addStarLevelExp(const Guid& petId, TemplateId& templateId, UInt32 incStarLevelExp)
{
    //Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
    //    return LynxErrno::PetNotExist;
    //}

    //PetData* petData = iter->mValue;
    //if (!petData)
    //{
    //    LOG_WARN("Pet pointer is null.");
    //    return LynxErrno::PetNotExist;
    //}

    //// 计算增加的星级经验可以升几级
    //templateId = petData->mBaseData.mTemplateId;
    //UInt32 remainStarExp = petData->mBaseData.mStarLevelExp + incStarLevelExp;
    //do
    //{
    //    PetTemplate* petTemplate = PET_TABLE().get(templateId);
    //    if (!petTemplate)
    //    {
    //        LOG_WARN("Failed to get pet template %u", templateId);
    //        return LynxErrno::TemplateNotExist;
    //    }

    //    PetStarGrowTemplate* petStarGrowTemplate = PET_STAR_GROW_TABLE().get(petTemplate->mStarLevel);
    //    if (!petStarGrowTemplate)
    //    {
    //        LOG_WARN("Failed to get pet grow template by current level %u", 
    //            petTemplate->mStarLevel);
    //        break;
    //    }

    //    if (remainStarExp >= petStarGrowTemplate->mExperience)
    //    {
    //        remainStarExp -= petStarGrowTemplate->mExperience;
    //        templateId = petTemplate->mAwaken;
    //    }
    //    else if (remainStarExp < petStarGrowTemplate->mExperience)
    //    {
    //        break;
    //    }
    //} while (1);

    //if (remainStarExp > petData->mBaseData.mStarLevelExp)
    //{
    //    increasePetStarLevelExp(petId, remainStarExp - petData->mBaseData.mStarLevelExp);
    //}
    //else if (remainStarExp < petData->mBaseData.mStarLevelExp)
    //{
    //    reducePetStarLevelExp(petId, petData->mBaseData.mStarLevelExp - remainStarExp);
    //}

    //petData->mDirtyFlag = true;
    return LynxErrno::None;
}

ErrorId 
PetManager::setPetPresentPos(const Guid& petId, UInt8 pos)
{
    /*Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    if (iter == NULL)
    {
        LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
        return LynxErrno::PetNotExist;
    }

    PetData* petData = iter->mValue;
    if (!petData)
    {
        LOG_WARN("Pet pointer is null.");
        return LynxErrno::PetNotExist;
    }

    Guid petId2 = 0;
    if (mPresentPetData)
    {
        petId2 = mPresentPetData->mBaseData.mPetId;
        mPresentPetData->mBaseData.mPresentPos = 0;
        
        SyncPetPresentPosNotify notify;
        notify.mPetGuid = mPresentPetData->mBaseData.mPetId;
        notify.mPresentPos = 0;
        mPlayer->sendMsgWithAck(notify);
    }

    petData->mBaseData.mPresentPos = pos;

    mPlayer->updatePlayerPresentPet();
    if (petId2)
    {
        calculatePetData(petId2);
    }
    calculatePetData(petData->mBaseData.mPetId);
    SyncPetPresentPosNotify notify;
    notify.mPetGuid = petData->mBaseData.mPetId;
    notify.mPresentPos = pos;
    mPlayer->sendMsgWithAck(notify);
    
    petData->mDirtyFlag = true;*/

    return LynxErrno::None;
}

ErrorId
PetManager::advancePet(const Guid& petId)
{
    //宠物进阶，sec注释原游戏
	//Map<Guid, PetData*>::Iter* iter = mPlayerPetDataMap.find(petId);
    //if (iter == NULL)
    //{
    //    LOG_WARN("Failed to get pet [%llu] for not exist.", petId);
    //    return LynxErrno::PetNotExist;
    //}

    //PetData* petData = iter->mValue;

    //PetTemplate* petTemplate = PET_TABLE().get(petData->mBaseData.mTemplateId);
    //if (!petTemplate)
    //{
    //    LOG_WARN("Failed to get pet template, id: %u", petData->mBaseData.mTemplateId);
    //    return LynxErrno::TemplateNotExist;
    //}

    //PetAdvanceTemplate* petAdvanceTemplate = PET_ADVANCE_TABLE().get(petTemplate->mPetAdvanceTemplateId);
    //if (!petAdvanceTemplate)
    //{
    //    LOG_WARN("Failed to get pet advance template, id: %u", petTemplate->mPetAdvanceTemplateId);
    //    return LynxErrno::TemplateNotExist;
    //}
    //UInt16 starLevel = petData->mBaseData.mStarLevel / 10;
    //UInt16 starLevelFractionalPart = petData->mBaseData.mStarLevel % 10;

    //// 最高就是60
    //if (petData->mBaseData.mStarLevel == 60)
    //{
    //    LOG_WARN("Advance full.");
    //    return LynxErrno::UnknownLogicError;
    //}
    //const ItemData* itemData[5] = {0};
    //UInt32 itemCount[5] = {0};
    //if (petAdvanceTemplate->mItemId1 != 0)
    //{
    //    itemData[0] = mPlayer->getItemManager().getItemByTemplateId(petAdvanceTemplate->mItemId1);
    //    itemCount[0] = petAdvanceTemplate->mItemCount1;
    //}
    //if (petAdvanceTemplate->mItemId2 != 0)
    //{
    //    itemData[1] = mPlayer->getItemManager().getItemByTemplateId(petAdvanceTemplate->mItemId2);
    //    itemCount[1] = petAdvanceTemplate->mItemCount2;
    //}
    //if (petAdvanceTemplate->mItemId3 != 0)
    //{
    //    itemData[2] = mPlayer->getItemManager().getItemByTemplateId(petAdvanceTemplate->mItemId3);
    //    itemCount[2] = petAdvanceTemplate->mItemCount3;
    //}
    //if (petAdvanceTemplate->mItemId4 != 0)
    //{
    //    itemData[3] = mPlayer->getItemManager().getItemByTemplateId(petAdvanceTemplate->mItemId4);
    //    itemCount[3] = petAdvanceTemplate->mItemCount4;
    //}
    //if (petAdvanceTemplate->mItemId5 != 0)
    //{
    //    itemData[4] = mPlayer->getItemManager().getItemByTemplateId(petAdvanceTemplate->mItemId5);
    //    itemCount[4] = petAdvanceTemplate->mItemCount5;
    //}

    //// 判断物品是否足够
    //bool canAdvance = true;
    //for (int i = 0; i < 5; i++)
    //{
    //    if (itemData[i])
    //        canAdvance = canAdvance && (itemData[i]->mCount >= itemCount[i]);
    //}
    //if (!canAdvance)
    //{
    //    LOG_WARN("not enough item.");
    //    return LynxErrno::InvalidOperation;
    //}

    //// 可以进阶
    //

    //if (((petData->mBaseData.mStarLevel + 1) % 10) == 0)
    //{
    //    if (petTemplate->mAwaken == 0)
    //    {
    //        //这个宠物不能进阶了
    //        return LynxErrno::InvalidOperation;
    //    }
    //    petData->mBaseData.mTemplateId = petTemplate->mAwaken;
    //    petData->mDirtyFlag = true;
    //    SyncPetTemplateIdNotify notify;
    //    notify.mPetId = petData->mBaseData.mPetId;
    //    notify.mTemplateId = petTemplate->mAwaken;
    //    mPlayer->sendMsgWithAck(notify);
    //}
    //petData->mBaseData.mStarLevel++;

    //for (int i = 0; i < 5; i++)
    //{
    //    if (itemData[i])
    //    {
    //        mPlayer->getItemManager().delItem(itemData[i]->mItemId, itemCount[i]);
    //    }
    //}

    //AdvancePetResp ap;
    //ap.mErrno = 0;
    //ap.mStarLevel = petData->mBaseData.mStarLevel;
    //ap.mPetId = petData->mBaseData.mPetId;
    //mPlayer->sendMsgWithAck(ap);

    //petData->mDirtyFlag = true;

    return 0;
}