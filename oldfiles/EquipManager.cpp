#include "EquipManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

EquipManager::EquipManager() : mPlayer(NULL), mEquipList(NULL)
{

}


EquipManager::~EquipManager()
{

}

bool 
EquipManager::initial(Player* player)
{
   
	//装备字段目前注释sec
	/* if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }
    mPlayer = player;
    mEquipList = &mPlayer->mPlayerData.mEquipData.mEquipDataList;
    for (List<EquipData>::Iter* it = mEquipList->begin();
        it != mEquipList->end(); it = mEquipList->next(it))
    {
        mEquipMap.insert(it->mValue.mEquipId, &it->mValue);
    }*/
    return true;
}

void 
EquipManager::release()
{

}

void 
EquipManager::calculateEquipData(UInt8 equipPresentPos)
{
    if (equipPresentPos != 0)
    {
        UInt8 heroPresentPos = (equipPresentPos - 1) / 5 + 1;
        const HeroData* heroData = mPlayer->getHeroManager().getHeroDataByPresentPos(heroPresentPos);
        if (heroData)
        {
            mPlayer->getHeroManager().calculateHeroData(heroData->mBaseData.mHeroId);
        }
    }
}

void 
EquipManager::destroyAllEquip()
{
    for (List<EquipData>::Iter* it = mEquipList->begin();
        it != mEquipList->end();
        it = mEquipList->next(it))
    {
            DelEquipNotify notify;
            notify.mEquipGuid = it->mValue.mEquipId;
            mPlayer->sendMsgWithAck(notify);

            PersistDelEquipNotify req;
            req.mItemId = it->mValue.mEquipId;
            req.mPlayerGuid = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(req, mPlayer->getPlayerGuid());
    }
    mEquipMap.clear();
    mEquipList->clear();
    
}

void 
EquipManager::destroyEquip(Guid equipId)
{

    for (List<EquipData>::Iter* it = mEquipList->begin();
        it != mEquipList->end();
        it = mEquipList->next(it))
    {
        if (it->mValue.mEquipId == equipId)
        {
            mEquipMap.erase(equipId);
            mEquipList->erase(it);

            DelEquipNotify notify;
            notify.mEquipGuid = equipId;
            mPlayer->sendMsgWithAck(notify);

            PersistDelEquipNotify req;
            req.mItemId = equipId;
            req.mPlayerGuid = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(req, mPlayer->getPlayerGuid());
            return;
        }
    }
}


UInt8 
EquipManager::getEquipFieldSpaceSlot()
{
    return (BAG_MAX_SIZE - mEquipList->size());
}

bool
EquipManager::isEquipFull()
{
    return (BAG_MAX_SIZE == mEquipList->size());
}

bool 
EquipManager::isEquipFull(List<TemplateId>& equipList)
{
    return (mEquipList->size() + equipList.size()) > BAG_MAX_SIZE;
}

bool 
EquipManager::isEquipFull(List<AwardLine>& awardList)
{
    List<TemplateId> equipList;
    for (List<AwardLine>::Iter* it = awardList.begin();
        it != NULL; it = awardList.next(it))
    {
        if (it->mValue.mItemType == RUNE_TYPE)
        {
            equipList.insertTail(it->mValue.mItemId);
        }
    }
    return isEquipFull(equipList);
}


void 
EquipManager::lockEquip(const Guid& equipId)
{
    
    Map<Guid, EquipData*>::Iter* it = mEquipMap.find(equipId);
    if (!it)
    {
        LOG_WARN("Failed to find equip from equip map, guid:%llu", equipId);
        return;
    }
    if (!it->mValue)
    {
        LOG_WARN("failed to find equip from bag,guid:%llu", equipId);
        return;
    }
    it->mValue->mLock = true;

}

void 
EquipManager::unLockEquip(const Guid& equipId)
{
    Map<Guid, EquipData*>::Iter* it = mEquipMap.find(equipId);
    if (!it)
    {
        LOG_WARN("Failed to find equip from equip map, guid:%llu", equipId);
        return;
    }
    if (!it->mValue)
    {
        LOG_WARN("failed to find equip from bag,guid:%llu", equipId);
        return;
    }
    it->mValue->mLock = false;

}


const EquipData* 
EquipManager::createEquip(TemplateId equipTemplateId, UInt8 source, bool promp/*promp = true*/)
{
    EquipTemplate* equipTemplate = EQUIP_TABLE().get(equipTemplateId);
    if (!equipTemplate)
    {
        LOG_WARN("Failed to get equip template %u", equipTemplateId);
        return NULL;
    }

    EquipData equipData;
    equipData.mTemplateId = equipTemplateId;
    equipData.mEquipId = LogicSystem::getSingleton().generateItemGuid();
    equipData.mEnchantLevel = 0;
    equipData.mPresentPos = 0;
    equipData.mRefineLevel = 0;

    List<EquipData>::Iter* it = mEquipList->insertTail(equipData);
    mEquipMap.insert(it->mValue.mEquipId, &it->mValue);

    AddEquipNotify notify;
    notify.mEquipData = equipData;
    notify.mPromp = promp;
    mPlayer->sendMsgWithAck(notify);

    PersistInsertEquipNotify dbNotify;
    dbNotify.mEquipData = equipData;
    dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());

    switch(equipTemplate->mStarLevel)
    {
    case 3:
        {
            mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_OWN_EQUIP_BLUE_CNT);
        }
        break;
    case 4:
        {
            mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_OWN_EQUIP_PURPLE_CNT);
        }
        break;
    case 5:
        {
            mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_OWN_EQUIP_ORANGE_CNT);
        }
        break;
    case 6:
        {
            mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_OWN_EQUIP_RED_CNT);
        }
        break;
    }

    return &it->mValue;
}

ErrorId 
EquipManager::useEquip(Guid equipId, UInt8 targetPos)
{
    Map<Guid, EquipData*>::Iter* it = mEquipMap.find(equipId);
    if (!it)
    {
        LOG_WARN("Failed to find equip from equip map, guid:%llu", equipId);
        return LynxErrno::EquipNotExist;
    }

    EquipData* equipData = it->mValue;
    if (!equipData)
    {
        LOG_WARN("Failed to find equip from bag, guid:%llu", equipId);
        return LynxErrno::EquipNotExist;
    }

    if (!targetPos || targetPos > PLAYER_EQUIP_MAX_POSITION)
    {
        LOG_WARN("invalid targetPos:%u", targetPos);
        return LynxErrno::InvalidParameter;
    }

    EquipTemplate* equipTemplate = EQUIP_TABLE().get(equipData->mTemplateId);
    if (!equipTemplate)
    {
        LOG_WARN("Failed to get equiptemplate by id:%u", equipData->mTemplateId);
        return LynxErrno::TemplateNotExist;
    }

    if (equipTemplate->mType != (targetPos - 1) % PLAYER_EQUIP_PER_HERO_MAX_COUNT + 1)
    {
        LOG_WARN("invalid targetPos, equip type not match! equiptype:%u, targetpos:%u", equipTemplate->mType, targetPos);
        return LynxErrno::InvalidParameter;
    }

    // 原有装备替换
    for (List<EquipData>::Iter* iter = mEquipList->begin();
        iter != mEquipList->end(); iter = mEquipList->next(iter))
    {
        if (iter->mValue.mPresentPos == targetPos)
        {
            iter->mValue.mPresentPos = 0;

            UpdateEquipPresentPosNotify notify;
            notify.mEquipId = iter->mValue.mEquipId;
            notify.mPresentPos = 0;
            mPlayer->sendMsgWithAck(notify);

            PersistUpdateEquipNotify request;
            request.mEquipData = iter->mValue;
            request.mPlayerGuid = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(request, mPlayer->getPlayerGuid());
            break;
        }
    }
    
    equipData->mPresentPos = targetPos;
    mPlayer->updatePlayerPresentEquips();
    calculateEquipData(targetPos);

    UpdateEquipPresentPosNotify notify;
    notify.mEquipId = equipId;
    notify.mPresentPos = targetPos;
    mPlayer->sendMsgWithAck(notify);

    PersistUpdateEquipNotify req;
    req.mEquipData = *equipData;
    req.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(req, mPlayer->getPlayerGuid());

    return LynxErrno::None;
}

ErrorId 
EquipManager::enchantEquip(const Guid& equipId, UInt32& enchantLevel)
{
	//sec 注释原有的附魔代码   
	//Map<Guid, EquipData*>::Iter* it = mEquipMap.find(equipId);
    //if (!it)
    //{
    //    LOG_WARN("Failed to find equip from equipmap %llu", equipId);
    //    return LynxErrno::EquipNotExist;
    //}

    //EquipData* equipData = it->mValue;
    //if (!equipData)
    //{
    //    LOG_WARN("Failed to get equip %llu", equipId);
    //    return LynxErrno::EquipNotExist;
    //}

    //EquipTemplate* equipTemplate = EQUIP_TABLE().get(equipData->mTemplateId);
    //if (!equipTemplate)
    //{
    //    LOG_WARN("Failed to get equip template %u", equipData->mTemplateId);
    //    return LynxErrno::TemplateNotExist;
    //}

    //UInt32 enchantTemplateId = equipTemplate->mEnchantId * 1000 + equipData->mEnchantLevel;
    //EquipEnchantTemplate* enchantTemplate = EQUIP_ENCHANT_TABLE().get(enchantTemplateId);
    //if (!enchantTemplate)
    //{
    //    LOG_WARN("Failed to get equip enchante template %u %u", equipTemplate->mEnchantId,
    //        equipData->mEnchantLevel);
    //    return LynxErrno::TemplateNotExist;
    //}

    //// 检查是否满足附魔条件
    //if (enchantTemplate->mRequreRefineLevel > equipData->mRefineLevel)
    //{
    //    LOG_WARN("Failed to enchant for equip refine level ins't full.");
    //    return LynxErrno::InvalidOperation;
    //}

    //if (enchantTemplate->mRequreGold > mPlayer->getPlayerData().mBaseData.m_nGold)
    //{
    //    LOG_WARN("Failed to enchant for gold isn't full.");
    //    return LynxErrno::GoldNotEnough;
    //}

    //Map<Guid, UInt32> tmpItemList;
    //if (enchantTemplate->mRequreItemId1 && enchantTemplate->mRequreItemCount1 > 0)
    //{
    //    const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(enchantTemplate->mRequreItemId1);
    //    if (!itemData)
    //    {
    //        LOG_WARN("Enchant require item1 %u isn't exist.", enchantTemplate->mRequreItemId1);
    //        return LynxErrno::InvalidOperation;
    //    }

    //    if (itemData->mCount < enchantTemplate->mRequreItemCount1)
    //    {
    //        LOG_WARN("Enchant require item1 %u count isn't full.", enchantTemplate->mRequreItemId1);
    //        return LynxErrno::InvalidOperation;
    //    }

    //    tmpItemList.insert(itemData->mItemId, enchantTemplate->mRequreItemCount1);
    //}

    //if (enchantTemplate->mRequreItemId2 && enchantTemplate->mRequreItemCount2 > 0)
    //{
    //    const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(enchantTemplate->mRequreItemId2);
    //    if (!itemData)
    //    {
    //        LOG_WARN("Enchant require item1 %u isn't exist.", enchantTemplate->mRequreItemId2);
    //        return LynxErrno::InvalidOperation;
    //    }

    //    if (itemData->mCount < enchantTemplate->mRequreItemCount2)
    //    {
    //        LOG_WARN("Enchant require item1 %u count isn't full.", enchantTemplate->mRequreItemId2);
    //        return LynxErrno::InvalidOperation;
    //    }

    //    tmpItemList.insert(itemData->mItemId, enchantTemplate->mRequreItemCount2);
    //}

    //if (enchantTemplate->mRequreItemId3 && enchantTemplate->mRequreItemCount3 > 0)
    //{
    //    const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(enchantTemplate->mRequreItemId3);
    //    if (!itemData)
    //    {
    //        LOG_WARN("Enchant require item1 %u isn't exist.", enchantTemplate->mRequreItemId3);
    //        return LynxErrno::InvalidOperation;
    //    }

    //    if (itemData->mCount < enchantTemplate->mRequreItemCount3)
    //    {
    //        LOG_WARN("Enchant require item1 %u count isn't full.", enchantTemplate->mRequreItemId3);
    //        return LynxErrno::InvalidOperation;
    //    }

    //    tmpItemList.insert(itemData->mItemId, enchantTemplate->mRequreItemCount3);
    //}

    //if (enchantTemplate->mRequreItemId4 && enchantTemplate->mRequreItemCount4 > 0)
    //{
    //    const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(enchantTemplate->mRequreItemId4);
    //    if (!itemData)
    //    {
    //        LOG_WARN("Enchant require item1 %u isn't exist.", enchantTemplate->mRequreItemId4);
    //        return LynxErrno::InvalidOperation;
    //    }

    //    if (itemData->mCount < enchantTemplate->mRequreItemCount4)
    //    {
    //        LOG_WARN("Enchant require item1 %u count isn't full.", enchantTemplate->mRequreItemId4);
    //        return LynxErrno::InvalidOperation;
    //    }

    //    tmpItemList.insert(itemData->mItemId, enchantTemplate->mRequreItemCount4);
    //}

    //if (enchantTemplate->mRequreItemId5 && enchantTemplate->mRequreItemCount5 > 0)
    //{
    //    const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(enchantTemplate->mRequreItemId5);
    //    if (!itemData)
    //    {
    //        LOG_WARN("Enchant require item1 %u isn't exist.", enchantTemplate->mRequreItemId5);
    //        return LynxErrno::InvalidOperation;
    //    }

    //    if (itemData->mCount < enchantTemplate->mRequreItemCount5)
    //    {
    //        LOG_WARN("Enchant require item1 %u count isn't full.", enchantTemplate->mRequreItemId5);
    //        return LynxErrno::InvalidOperation;
    //    }

    //    tmpItemList.insert(itemData->mItemId, enchantTemplate->mRequreItemCount5);
    //}

    //// 执行操作
    //equipData->mEnchantLevel += 1;

    //mPlayer->reducePlayerGold(enchantTemplate->mRequreGold);

    //for (Map<Guid, UInt32>::Iter* iter = tmpItemList.begin();
    //    iter != NULL; iter = tmpItemList.next(iter))
    //{
    //    mPlayer->getItemManager().delItem(iter->mKey, iter->mValue);
    //}

    //UpdateEquipEnchantLevelNotify updateEnchantNotify;
    //updateEnchantNotify.mEquipId = equipId;
    //updateEnchantNotify.mEnchantLevel = equipData->mEnchantLevel;
    //mPlayer->sendMsgWithAck(updateEnchantNotify);

    //PersistUpdateEquipNotify notify;
    //notify.mEquipData = *(it->mValue);
    //notify.mPlayerGuid = mPlayer->getPlayerGuid();
    //PersistSystem::getSingleton().postThreadMsg(notify, mPlayer->getPlayerGuid());

    //enchantLevel = equipData->mEnchantLevel;

    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_ONE_EQUIP_ENCHANT_LEVEL);
    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_TWO_EQUIP_ENCHANT_LEVEL);
    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_FIVE_EQUIP_ENCHANT_LEVEL);
    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_TEN_EQUIP_ENCHANT_LEVEL);

    return LynxErrno::None;
}

void 
EquipManager::setEquipRefine(const Guid& itemId, UInt32 level)
{
    Map<Guid, EquipData*>::Iter* it = mEquipMap.find(itemId);
    if (!it)
    {
        LOG_WARN("Failed to find equip from equipmap %llu", itemId);
        return;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to get equip %llu", itemId);
        return;
    }
    if (level > EQUIP_REFINE_MAX_LEVEL)
    {
        LOG_WARN("Invalid refine level %u", level);
        return;
    }
    it->mValue->mRefineLevel = level;

    UpdateEquipRefineLevelNotify notify;
    notify.mEquipId = itemId;
    notify.mRefineLevel = level;
    mPlayer->sendMsgWithAck(notify);

    PersistUpdateEquipNotify dbNotify;
    dbNotify.mEquipData = *it->mValue;
    dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
}

ErrorId 
EquipManager::refineEquip(const Guid& equipId, UInt32& refineLevel)
{
    Map<Guid, EquipData*>::Iter* it = mEquipMap.find(equipId);
    if (!it)
    {
        LOG_WARN("Failed to find equip from equipmap %llu", equipId);
        return LynxErrno::EquipNotExist;
    }

    EquipData* equipData = it->mValue;
    if (!equipData)
    {
        LOG_WARN("Failed to get equip %llu", equipId);
        return LynxErrno::EquipNotExist;
    }

    if (equipData->mRefineLevel >= EQUIP_REFINE_MAX_LEVEL)
    {
        LOG_WARN("equip refine level is max level");
        return LynxErrno::InvalidParameter;
    }

    if (equipData->mRefineLevel >= mPlayer->mPlayerData.mBaseData.m_nLevel * 2)
    {
        LOG_WARN("equip refine level is double player level");
        return LynxErrno::InvalidParameter;
    }

    EquipTemplate* equipTemplate = EQUIP_TABLE().get(equipData->mTemplateId);
    if (!equipTemplate)
    {
        LOG_WARN("Failed to get equipTemplate by id: %u", it->mValue->mTemplateId);
        return LynxErrno::TemplateNotExist;
    }

    EquipRefineTemplate* refineTemplate = EQUIP_REFINE_TABLE().get(equipData->mRefineLevel + equipTemplate->mType * 1000);
    if (!refineTemplate)
    {
        LOG_WARN("Failed to get equip refine template by id: %u", equipData->mRefineLevel + equipTemplate->mType * 1000);
        return LynxErrno::TemplateNotExist;
    }

    UInt32 requireGold = 0;
    
    switch (equipTemplate->mStarLevel)
    {
    case 1:
        requireGold = refineTemplate->mPrice1;
        break;
    case 2:
        requireGold = refineTemplate->mPrice2;
        break;
    case 3:
        requireGold = refineTemplate->mPrice3;
        break;
    case 4:
        requireGold = refineTemplate->mPrice4;
        break;
    case 5:
        requireGold = refineTemplate->mPrice5;
        break;
    case 6:
        requireGold = refineTemplate->mPrice6;
        break;
    }

    if (requireGold == 0)
    {
        LOG_WARN("Failed to refine equip for require gold is 0.");
        return LynxErrno::GoldNotEnough;
    }

    if (requireGold > mPlayer->getPlayerData().mBaseData.m_nGold)
    {
        LOG_WARN("Failed to refine equip for gold is not enough");
        return LynxErrno::GoldNotEnough;
    }

    // 执行操作
    mPlayer->reducePlayerGold(requireGold);

    equipData->mRefineLevel++;
    calculateEquipData(equipData->mPresentPos);

    UpdateEquipRefineLevelNotify updateRefineLevelNotify;
    updateRefineLevelNotify.mEquipId = equipId;
    updateRefineLevelNotify.mRefineLevel = equipData->mRefineLevel;
    mPlayer->sendMsgWithAck(updateRefineLevelNotify);

    PersistUpdateEquipNotify notify;
    notify.mEquipData = *(it->mValue);
    notify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(notify, mPlayer->getPlayerGuid());

    refineLevel = equipData->mRefineLevel;

	//成就系统注释 sec 
   //mPlayer->mPlayerData.mAchievementData.mBaseData.mRefineEquipCnt ++;
    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_REFINE_EQUIP_CNT);
    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_EQUIP_MAX_REFINE_LEVEL);

    return LynxErrno::None;
}

ErrorId 
EquipManager::autoRefineEquip(const Guid& equipId, UInt32& refineLevel)
{
    Map<Guid, EquipData*>::Iter* it = mEquipMap.find(equipId);
    if (!it)
    {
        LOG_WARN("Failed to find equip from equipmap %llu", equipId);
        return LynxErrno::EquipNotExist;
    }

    EquipData* equipData = it->mValue;
    if (!equipData)
    {
        LOG_WARN("Failed to get equip %llu", equipId);
        return LynxErrno::EquipNotExist;
    }

    if (equipData->mRefineLevel >= EQUIP_REFINE_MAX_LEVEL)
    {
        LOG_WARN("equip refine level is max level");
        return LynxErrno::InvalidParameter;
    }

    if (equipData->mRefineLevel >= mPlayer->mPlayerData.mBaseData.m_nLevel)
    {
        LOG_WARN("equip refine level is double player level");
        return LynxErrno::InvalidParameter;
    }

    EquipTemplate* equipTemplate = EQUIP_TABLE().get(equipData->mTemplateId);
    if (!equipTemplate)
    {
        LOG_WARN("Failed to get equipTemplate by id: %u", it->mValue->mTemplateId);
        return LynxErrno::TemplateNotExist;
    }

    EquipRefineTemplate* refineTemplate = EQUIP_REFINE_TABLE().get(equipData->mRefineLevel + equipTemplate->mType * 1000);
    if (!refineTemplate)
    {
        LOG_WARN("Failed to get equip refine template by id: %u", equipData->mRefineLevel + equipTemplate->mType * 1000);
        return LynxErrno::TemplateNotExist;
    }

    UInt32 requireGold = 0;

    switch (equipTemplate->mStarLevel)
    {
    case 1:
        requireGold = refineTemplate->mPrice1;
        break;
    case 2:
        requireGold = refineTemplate->mPrice2;
        break;
    case 3:
        requireGold = refineTemplate->mPrice3;
        break;
    case 4:
        requireGold = refineTemplate->mPrice4;
        break;
    case 5:
        requireGold = refineTemplate->mPrice5;
        break;
    case 6:
        requireGold = refineTemplate->mPrice6;
        break;
    }

    if (requireGold == 0)
    {
        LOG_WARN("Failed to refine equip for require gold is 0.");
        return LynxErrno::GoldNotEnough;
    }

    if (requireGold > mPlayer->getPlayerData().mBaseData.m_nGold)
    {
        LOG_WARN("Failed to refine equip for gold is not enough");
        return LynxErrno::GoldNotEnough;
    }
    UInt32 totalRequireGold = requireGold;
    // 执行操作
    //mPlayer->reducePlayerGold(requireGold);

    equipData->mRefineLevel++;

	//目前成就系统已关闭 sec
   // mPlayer->mPlayerData.mAchievementData.mBaseData.mRefineEquipCnt ++;

    while(true)
    {
        refineTemplate = EQUIP_REFINE_TABLE().get(equipData->mRefineLevel + equipTemplate->mType * 1000);
        if (!refineTemplate)
        {
            LOG_WARN("Failed to get equip refine template by id: %u", equipData->mRefineLevel + equipTemplate->mType * 1000);
            break;
        }

        requireGold = 0;

        switch (equipTemplate->mStarLevel)
        {
        case 1:
            requireGold = refineTemplate->mPrice1;
            break;
        case 2:
            requireGold = refineTemplate->mPrice2;
            break;
        case 3:
            requireGold = refineTemplate->mPrice3;
            break;
        case 4:
            requireGold = refineTemplate->mPrice4;
            break;
        case 5:
            requireGold = refineTemplate->mPrice5;
            break;
        case 6:
            requireGold = refineTemplate->mPrice6;
            break;
        }

        if (requireGold == 0)
        {
            break;
        }

        if ((requireGold + totalRequireGold) > mPlayer->getPlayerData().mBaseData.m_nGold)
        {
            break;
        }

        // 执行操作
        //mPlayer->reducePlayerGold(requireGold);
        totalRequireGold += requireGold;

        equipData->mRefineLevel++;


		//目前成就系统已关闭 sec
		//mPlayer->mPlayerData.mAchievementData.mBaseData.mRefineEquipCnt ++;

        if (equipData->mRefineLevel == EQUIP_REFINE_MAX_LEVEL)
        {
            break;
        }
        if (equipData->mRefineLevel >= mPlayer->mPlayerData.mBaseData.m_nLevel * 2)
        {
        }
        
    }
    mPlayer->reducePlayerGold(totalRequireGold);
    calculateEquipData(equipData->mPresentPos);

    UpdateEquipRefineLevelNotify updateRefineLevelNotify;
    updateRefineLevelNotify.mEquipId = equipId;
    updateRefineLevelNotify.mRefineLevel = equipData->mRefineLevel;
    mPlayer->sendMsgWithAck(updateRefineLevelNotify);

    PersistUpdateEquipNotify notify;
    notify.mEquipData = *(it->mValue);
    notify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(notify, mPlayer->getPlayerGuid());

    refineLevel = equipData->mRefineLevel;

    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_EQUIP_MAX_REFINE_LEVEL);
    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_REFINE_EQUIP_CNT);

    return LynxErrno::None;
}

const EquipData* 
EquipManager::getEquipDataByGuid(const Guid& equipId)
{
    Map<Guid, EquipData*>::Iter* iter = mEquipMap.find(equipId);
    if (iter)
    {
        return iter->mValue;
    }
    return NULL;
}

ErrorId 
EquipManager::smeltEquip(const Guid& equipId, UInt32& gold, UInt32& soul)
{
    Map<Guid, EquipData*>::Iter* iter = mEquipMap.find(equipId);
    if (!iter)
    {
        LOG_WARN("Failed to find equip from equipmap %llu", equipId);
        return LynxErrno::EquipNotExist;
    }

    EquipData* equipData = iter->mValue;
    if (!equipData)
    {
        LOG_WARN("Equip pointer is null.");
        return LynxErrno::EquipNotExist;
    }

    EquipTemplate* equipTemplate = EQUIP_TABLE().get(equipData->mTemplateId);
    if (!equipTemplate)
    {
        LOG_WARN("Failed to get equipTemplate by id: %u", equipData->mTemplateId);
        return LynxErrno::TemplateNotExist;
    }

    gold += equipTemplate->mDestroyGold;
    soul += equipTemplate->mDestroySoul;

    // 销毁装备
    destroyEquip(equipId);
    return LynxErrno::None;
}

ErrorId 
EquipManager::rebornEquip(const Guid& equipId, UInt32& gold, List<Guid>& itemList)
{
    //sec 原游戏装备重生
	//Map<Guid, EquipData*>::Iter* iter = mEquipMap.find(equipId);
    //if (!iter)
    //{
    //    LOG_WARN("Failed to find equip from equipmap %llu", equipId);
    //    return LynxErrno::EquipNotExist;
    //}

    //EquipData* equipData = iter->mValue;
    //if (!equipData)
    //{
    //    LOG_WARN("Equip pointer is null.");
    //    return LynxErrno::EquipNotExist;
    //}

    //EquipTemplate* equipTemplate = EQUIP_TABLE().get(equipData->mTemplateId);
    //if (!equipTemplate)
    //{
    //    LOG_WARN("Failed to get equipTemplate by id: %u", equipData->mTemplateId);
    //    return LynxErrno::TemplateNotExist;
    //}

    //EquipConsumeReport equipConsumeReport;
    //if (!CalculateUtil::calculateEquipConsumeReport(*equipData, equipConsumeReport))
    //{
    //    LOG_WARN("Failed to calculate equip consume report %llu", equipId);
    //    return LynxErrno::InvalidOperation;
    //}

    //for (Map<TemplateId, UInt32>::Iter* jter = equipConsumeReport.mItemMap.begin();
    //    jter != NULL; jter = equipConsumeReport.mItemMap.next(jter))
    //{
    //    const ItemData* newItemData = mPlayer->getItemManager().createItem(jter->mKey, jter->mValue, 0);
    //    if (newItemData)
    //    {
    //        itemList.insertTail(newItemData->mItemId);
    //    }
    //}

    //gold += equipConsumeReport.mGold;
    //mPlayer->getItemManager().createItem(ITEM_GOLD_TEMPLATEID, gold, 0);
    ////还原装备
    //equipData->mRefineLevel = 0;
    //equipData->mEnchantLevel = 0;
    //equipData->mConsumeGold = 0;
    //RebornEquipNotify notify;
    //notify.mEquipId = equipData->mEquipId;
    //mPlayer->sendMsgWithAck(notify);

    //PersistUpdateEquipNotify dbNotify;
    //dbNotify.mEquipData = *equipData;
    //dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    //PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());

    return LynxErrno::None;
}


void 
EquipManager::setEquipPresentPos(const Guid& equipId, UInt8 pos)
{
    Map<Guid, EquipData*>::Iter* iter = mEquipMap.find(equipId);
    if (!iter)
    {
        LOG_WARN("Failed to find equip from equipmap %llu", equipId);
        return;
    }

    EquipData* equipData = iter->mValue;
    if (!equipData)
    {
        LOG_WARN("Equip pointer is null.");
        return;
    }

    if (pos < 1 || pos > 25)
    {
        LOG_WARN("Invalid pos %u", pos);
        return;
    }

    equipData->mPresentPos = pos;
}

ErrorId
EquipManager::sellEquip(List<Guid>& equipList, UInt32& getGold)
{
    for (List<Guid>::Iter* it = equipList.begin();
        it != NULL; it = equipList.next(it))
    {
        Map<Guid, EquipData*>::Iter* existIter = mEquipMap.find(it->mValue);
        if (!existIter)
        {
            LOG_WARN("Failed to get equip by id: %llu", it->mValue);
            return LynxErrno::InvalidParameter;
        }
        if (existIter->mValue->mPresentPos)
        {
            LOG_WARN("Can not sell equip ,equip is on present!");
            return LynxErrno::InvalidParameter;
        }
        
    }
    for (List<Guid>::Iter* it = equipList.begin();
        it != NULL; it = equipList.next(it))
    {
        Map<Guid, EquipData*>::Iter* existIter = mEquipMap.find(it->mValue);
        EquipTemplate* equipTemplate = EQUIP_TABLE().get(existIter->mValue->mTemplateId);
        if(!equipTemplate)
        {
            LOG_WARN("Failed to get equiptemplate by id: %u", existIter->mValue->mTemplateId);
            continue;
        }
        destroyEquip(existIter->mValue->mEquipId);
        //mPlayer->increasePlayerGold(equipTemplate->mDestroyGold);
        
        getGold += equipTemplate->mDestroyGold;
    }
    mPlayer->getItemManager().createItem(ITEM_GOLD_TEMPLATEID, getGold, 0);
    return LynxErrno::None;
}