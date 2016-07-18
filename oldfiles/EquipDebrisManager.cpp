#include "EquipDebrisManager.h"
#include "Player.h"
#include "GameServer.h"


using namespace Lynx;

EquipDebrisManager::EquipDebrisManager() : mPlayer(NULL), mEquipDebrisList(NULL)
{

}


EquipDebrisManager::~EquipDebrisManager()
{

}

bool 
EquipDebrisManager::initial(Player* player)
{
    //装备碎片注释sec
	/*if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }
    mPlayer = player;
    mEquipDebrisList = &mPlayer->mPlayerData.mEquipDebrisData.mEquipDebrisList;
    for (List<EquipDebrisData>::Iter* it = mEquipDebrisList->begin();
        it != mEquipDebrisList->end();
        it = mEquipDebrisList->next(it))
    {
        mEquipDebrisDataMap.insert(it->mValue.mEquipDebrisId, &it->mValue);
        mEquipDebrisTemplateMap.insert(it->mValue.mTemplateId, &it->mValue);
    }*/
    return true;
}

void 
EquipDebrisManager::release()
{
    mEquipDebrisDataMap.clear();
    mEquipDebrisTemplateMap.clear();
}

void 
EquipDebrisManager::clearBag()
{
    //Playerdata equipDebrisData字段注释了
	//mPlayer->mPlayerData.mEquipDebrisData.mEquipDebrisList.clear();
}

void 
EquipDebrisManager::destroyEquipDebris(Guid equipDebrisId)
{
    delEquipDebris(equipDebrisId, 0);
}

void 
EquipDebrisManager::delEquipDebris(Guid equipDebrisId, UInt32 count/* count = 0 */)
{

    for (List<EquipDebrisData>::Iter* it = mEquipDebrisList->begin();
        it != mEquipDebrisList->end();
        it = mEquipDebrisList->next(it))
    {
        // 删除指定数量道具
        if (it->mValue.mEquipDebrisId == equipDebrisId)
        {
            if (count && count < it->mValue.mCount)
            {
                it->mValue.mCount -= count;
                UpdateEquipDebrisNotify notify;
                notify.mEquipDebrisGuid = it->mValue.mEquipDebrisId;
                notify.mCount = it->mValue.mCount;
                mPlayer->sendMsgWithAck(notify);

                PersistUpdateEquipDebrisNotify dbNotify;
                dbNotify.mEquipDebrisData = it->mValue;
                dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
                PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
                return;
            }
            mEquipDebrisDataMap.erase(equipDebrisId);
            mEquipDebrisTemplateMap.erase(it->mValue.mTemplateId);
            mEquipDebrisList->erase(it);

            DelEquipDebrisNotify notify;
            notify.mEquipDebrisGuid = equipDebrisId;
            mPlayer->sendMsgWithAck(notify);

            PersistDelEquipDebrisNotify dbNotify;
            dbNotify.mEquipDebrisId = equipDebrisId;
            PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
            break;
        }
    }
}

void 
EquipDebrisManager::lockEquipDebris(const Guid& itemId)
{
    Map<Guid, EquipDebrisData*>::Iter* it = mEquipDebrisDataMap.find(itemId);
    if (!it)
    {
        LOG_WARN("Failed to find item from temp map by id:%llu", itemId);
        return;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to find item by id:%llu", itemId);
        return;
    }
    it->mValue->mLock = true;
}

void 
EquipDebrisManager::unLockEquipDebris(const Guid& itemId)
{

    Map<Guid, EquipDebrisData*>::Iter* it = mEquipDebrisDataMap.find(itemId);
    if (!it)
    {
        LOG_WARN("Failed to find item from temp map by id:%llu", itemId);
        return;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to find item by id:%llu", itemId);
        return;
    }
    it->mValue->mLock = false;
}

bool 
EquipDebrisManager::isEquipDebrisFull()
{
    return (BAG_MAX_SIZE <= mEquipDebrisList->size());
}

bool 
EquipDebrisManager::isEquipDebrisFull(List<TemplateId>& equipDebrisList)
{
    UInt32 curCount = equipDebrisList.size();
    for (List<TemplateId>::Iter* it = equipDebrisList.begin();
        it != equipDebrisList.end();
        it = equipDebrisList.next(it))
    {
        const EquipDebrisData* pt = getEquipDebrisByTemplateId(it->mValue);
        if (pt)
        {
            curCount --;
        }
    }
    return ((equipDebrisList.size() + curCount) > BAG_MAX_SIZE);
}

bool 
EquipDebrisManager::isEquipDebrisFull(List<AwardLine>& awardList)
{
    List<TemplateId> equipDebrisList;
    for (List<AwardLine>::Iter* it = awardList.begin();
        it != NULL; it = awardList.next(it))
    {
        if (it->mValue.mItemType = EQUIP_DEBRIS_TYPE)
        {
            equipDebrisList.insertTail(it->mValue.mItemId);
        }
    }
    return isEquipDebrisFull(equipDebrisList);
}

const EquipDebrisData*
EquipDebrisManager::getEquipDebrisByTemplateId(TemplateId templateId)
{
    Map<TemplateId, EquipDebrisData*>::Iter* iter = mEquipDebrisTemplateMap.find(templateId);
    if (iter == NULL)
    {
        return NULL;
    }
    return iter->mValue;
}

const EquipDebrisData* 
EquipDebrisManager::getEquipDebrisByGuid(const Guid& guid)
{
    Map<Guid, EquipDebrisData*>::Iter* iter = mEquipDebrisDataMap.find(guid);
    if (!iter)
    {
        return NULL;
    }
    return iter->mValue;
}

const EquipDebrisData* 
EquipDebrisManager::createEquipDebris(TemplateId templateId, UInt32 count, UInt8 source, bool promp/*promp = true*/)
{
    EquipDebrisTemplate* equipDebrisTemplate = EQUIP_DEBRIS_TABLE().get(templateId);
    if (!equipDebrisTemplate)
    {
        LOG_WARN("Failed to get equip debris template %u", templateId);
        return NULL;
    }

    Map<TemplateId, EquipDebrisData*>::Iter* iter = mEquipDebrisTemplateMap.find(templateId);
    if (iter && iter->mValue)
    {
        EquipDebrisData* equipDebrisData = iter->mValue;
        equipDebrisData->mCount = equipDebrisData->mCount + count > ITEM_MAX_COUNT ? ITEM_MAX_COUNT : equipDebrisData->mCount + count;

        UpdateEquipDebrisNotify notify;
        notify.mEquipDebrisGuid = equipDebrisData->mEquipDebrisId;
        notify.mCount = equipDebrisData->mCount;
        notify.mPromp = promp;
        mPlayer->sendMsgWithAck(notify);

        PersistUpdateEquipDebrisNotify dbNotify;
        dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        dbNotify.mEquipDebrisData = *equipDebrisData;
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
        return iter->mValue;
    }
    else
    {
        EquipDebrisData tempData;
        tempData.mCount = count;
        tempData.mTemplateId = templateId;
        tempData.mEquipDebrisId = LogicSystem::getSingleton().generateItemGuid();
        List<EquipDebrisData>::Iter* it = mEquipDebrisList->insertTail(tempData);
        mEquipDebrisDataMap.insert(tempData.mEquipDebrisId, &it->mValue);
        mEquipDebrisTemplateMap.insert(tempData.mTemplateId, &it->mValue);

        AddEquipDebrisNotify notify;
        notify.mEquipDebrisData = tempData;
        notify.mPromp = promp;
        mPlayer->sendMsgWithAck(notify);

        PersistInsertEquipDebrisNotify dbNotify;
        dbNotify.mEquipDebrisData = tempData;
        dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
        return &it->mValue;
    }
}

bool 
EquipDebrisManager::useEquipDebris(const Guid& guid, UInt32 count)
{
    Map<Guid, EquipDebrisData*>::Iter* it = mEquipDebrisDataMap.find(guid);
    if (!it)
    {
        LOG_WARN("Failed to get equipdebrisdata by guid:%llu", guid);
        return false;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to get equipdebrisdata by guid:%llu", guid);
        return false;
    }

    if (it->mValue->mCount < count)
    {
        LOG_WARN("Not enough debris!");
        return false;
    }

    it->mValue->mCount -= count;
    UpdateEquipDebrisNotify notify;
    notify.mEquipDebrisGuid = it->mValue->mEquipDebrisId;
    notify.mCount = it->mValue->mCount;
    mPlayer->sendMsgWithAck(notify);

    PersistUpdateEquipDebrisNotify dbNotify;
    dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    dbNotify.mEquipDebrisData = *it->mValue;
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());

    return true;
}

ErrorId 
EquipDebrisManager::combineEquip(const Guid& equipDebrisId, Guid& equipId)
{
    Map<Guid, EquipDebrisData*>::Iter* it = mEquipDebrisDataMap.find(equipDebrisId);
    if (!it)
    {
        LOG_WARN("Failed to get equipdebrisdata by guid:%llu", equipDebrisId);
        return LynxErrno::EquipNotExist;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to get equipdebrisdata by guid:%llu", equipDebrisId);
        return LynxErrno::EquipNotExist;
    }

    EquipDebrisTemplate* equipDebrisTemplate = EQUIP_DEBRIS_TABLE().get(it->mValue->mTemplateId);
    if (!equipDebrisTemplate)
    {
        LOG_WARN("Failed to get equipdebristemplate by id:%u", it->mValue->mTemplateId);
        return LynxErrno::TemplateNotExist;
    }

    EquipTemplate* equipTemplate = EQUIP_TABLE().get(equipDebrisTemplate->mEquipId);
    if (!equipTemplate)
    {
        LOG_WARN("Failed to get equiptemplate by id:%u", equipDebrisTemplate->mEquipId);
        return LynxErrno::TemplateNotExist;
    }

    if (it->mValue->mCount < equipDebrisTemplate->mCount)
    {
        LOG_WARN("Not enough debris!");
        return LynxErrno::InvalidOperation;
    }

    if (!useEquipDebris(equipDebrisId, equipDebrisTemplate->mCount))
    {
        LOG_WARN("Failed to combine equip!");
        return LynxErrno::InvalidOperation;
    }

    const EquipData* equipData = mPlayer->getEquipManager().createEquip(equipDebrisTemplate->mEquipId, 0);
    if (!equipData)
    {
        LOG_WARN("Failed to create equip !");
        return LynxErrno::EquipNotExist;
    }

    equipId = equipData->mEquipId;

	//成就系统已关闭sec
   /* mPlayer->mPlayerData.mAchievementData.mBaseData.mCombineEquipCnt ++;
    mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_COMBINE_EQUIP_CNT);*/

    return LynxErrno::None;
}