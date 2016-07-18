#include "PetDebrisManager.h"
#include "Player.h"
#include "GameServer.h"


using namespace Lynx;

PetDebrisManager::PetDebrisManager() : mPlayer(NULL), mPetDebrisDataList(NULL)
{

}


PetDebrisManager::~PetDebrisManager()
{

}

bool 
PetDebrisManager::initial(Player* player)
{
    if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }
    mPlayer = player;
	//mPlayerData.mPetDebrisData

    //mPetDebrisDataList = &mPlayer->mPlayerData.mPetDebrisData.mPetDebrisList;
    //for (List<PetDebrisData>::Iter* it = mPetDebrisDataList->begin();
    //    it != mPetDebrisDataList->end();
    //    it = mPetDebrisDataList->next(it))
    //{
    //    mPetDebrisDataMap.insert(it->mValue.mPetDebrisId, &it->mValue);
    //    mPetDebrisTemplateMap.insert(it->mValue.mTemplateId, &it->mValue);
    //}
    return true;
}

void 
PetDebrisManager::release()
{
    mPetDebrisDataMap.clear();
    mPetDebrisTemplateMap.clear();
}

void 
PetDebrisManager::clearBag()
{
    //注释宠物字段 sec
	/*mPlayer->mPlayerData.mPetDebrisData.mPetDebrisList.clear();*/
}

void 
PetDebrisManager::destroyPetDebris(Guid petDebrisId)
{
    delPetDebris(petDebrisId, 0);
}

void 
PetDebrisManager::delPetDebris(Guid petDebrisId, UInt32 count/* count = 0 */)
{
    for (List<PetDebrisData>::Iter* it = mPetDebrisDataList->begin();
        it != mPetDebrisDataList->end();
        it = mPetDebrisDataList->next(it))
    {
        // 删除指定数量道具
        if (it->mValue.mPetDebrisId == petDebrisId)
        {
            if (count && count < it->mValue.mCount)
            {
                it->mValue.mCount -= count;
                UpdatePetDebrisNotify notify;
                notify.mPetDebrisId = it->mValue.mPetDebrisId;
                notify.mCount = it->mValue.mCount;
                mPlayer->sendMsgWithAck(notify);

                PersistUpdatePetDebrisNotify dbNotify;
                dbNotify.mPetDebrisData = it->mValue;
                dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
                PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
                return;
            }
            mPetDebrisDataMap.erase(petDebrisId);
            mPetDebrisTemplateMap.erase(it->mValue.mTemplateId);
            mPetDebrisDataList->erase(it);

            DelPetDebrisNotify notify;
            notify.mPetDebrisGuid = petDebrisId;
            mPlayer->sendMsgWithAck(notify);

            PersistDelPetDebrisNotify dbNotify;
            dbNotify.mPetDebrisId = petDebrisId;
            dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
            break;
        }
    }
}

void 
PetDebrisManager::lockPetDebris(const Guid& itemId)
{
    Map<Guid, PetDebrisData*>::Iter* it = mPetDebrisDataMap.find(itemId);
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
PetDebrisManager::unLockPetDebris(const Guid& itemId)
{

    Map<Guid, PetDebrisData*>::Iter* it = mPetDebrisDataMap.find(itemId);
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
PetDebrisManager::isPetDebrisFull()
{
    return (BAG_MAX_SIZE <= mPetDebrisDataList->size());
}

bool 
PetDebrisManager::isPetDebrisFull(List<TemplateId>& petDebrisList)
{
    UInt32 curCount = petDebrisList.size();
    for (List<TemplateId>::Iter* it = petDebrisList.begin();
        it != petDebrisList.end();
        it = petDebrisList.next(it))
    {
        const PetDebrisData* pt = getPetDebrisByTemplateId(it->mValue);
        if (pt)
        {
            curCount --;
        }
    }
    return ((petDebrisList.size() + curCount) > BAG_MAX_SIZE);
}

bool 
PetDebrisManager::isPetDebrisFull(List<AwardLine>& awardList)
{
    List<TemplateId> petDebrisList;
    for (List<AwardLine>::Iter* it = awardList.begin();
        it != NULL; it = awardList.next(it))
    {
        if (it->mValue.mItemType = PET_DEBRIS_TYPE)
        {
            petDebrisList.insertTail(it->mValue.mItemId);
        }
    }
    return isPetDebrisFull(petDebrisList);
}

const PetDebrisData*
PetDebrisManager::getPetDebrisByTemplateId(TemplateId templateId)
{
    Map<TemplateId, PetDebrisData*>::Iter* iter = mPetDebrisTemplateMap.find(templateId);
    if (iter == NULL)
    {
        return NULL;
    }
    return iter->mValue;
}

const PetDebrisData* 
PetDebrisManager::getPetDebrisByGuid(const Guid& guid)
{
    Map<Guid, PetDebrisData*>::Iter* iter = mPetDebrisDataMap.find(guid);
    if (!iter)
    {
        return NULL;
    }
    return iter->mValue;
}

const PetDebrisData* 
PetDebrisManager::createPetDebris(TemplateId templateId, UInt32 count, UInt8 source, bool promp/*promp = true*/)
{
    PetDebrisTemplate* petDebrisTemplate = PET_DEBRIS_TABLE().get(templateId);
    if (!petDebrisTemplate)
    {
        LOG_WARN("Failed to get pet debris template %u", templateId);
        return NULL;
    }

     Map<TemplateId, PetDebrisData*>::Iter* iter = mPetDebrisTemplateMap.find(templateId);
     if (iter && iter->mValue)
     {
         PetDebrisData* petDebrisData = iter->mValue;

         petDebrisData->mCount = petDebrisData->mCount + count > ITEM_MAX_COUNT ? ITEM_MAX_COUNT : petDebrisData->mCount + count;

         UpdatePetDebrisNotify notify;
         notify.mPetDebrisId = petDebrisData->mPetDebrisId;
         notify.mCount = petDebrisData->mCount;
         notify.mPromp = promp;
         mPlayer->sendMsgWithAck(notify);

         PersistUpdatePetDebrisNotify dbNotify;
         dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
         dbNotify.mPetDebrisData = *petDebrisData;
         PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
         return iter->mValue;
     }
    else
    {
        PetDebrisData tempData;
        tempData.mCount = count;
        tempData.mTemplateId = templateId;
        tempData.mPetDebrisId = LogicSystem::getSingleton().generateItemGuid();
        List<PetDebrisData>::Iter* it = mPetDebrisDataList->insertTail(tempData);
        mPetDebrisDataMap.insert(tempData.mPetDebrisId, &it->mValue);
        mPetDebrisTemplateMap.insert(tempData.mTemplateId, &it->mValue);

        AddPetDebrisNotify notify;
        notify.mPetDebrisData = tempData;
        notify.mPromp = promp;
        mPlayer->sendMsgWithAck(notify);

        PersistInsertPetDebrisNotify dbNotify;
        dbNotify.mPetDebrisData = tempData;
        dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
        return &it->mValue;
    }
}

bool 
PetDebrisManager::usePetDebris(const Guid& guid, UInt32 count)
{
    Map<Guid, PetDebrisData*>::Iter* it = mPetDebrisDataMap.find(guid);
    if (!it)
    {
        LOG_WARN("Failed to get herodebrisdata by guid:%llu", guid);
        return false;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to get herodebrisdata by guid:%llu", guid);
        return false;
    }

    if (it->mValue->mCount < count)
    {
        LOG_WARN("Not enough debris!");
        return false;
    }

    it->mValue->mCount -= count;
    UpdatePetDebrisNotify notify;
    notify.mPetDebrisId = it->mValue->mPetDebrisId;
    notify.mCount = it->mValue->mCount;
    mPlayer->sendMsgWithAck(notify);

    PersistUpdatePetDebrisNotify dbNotify;
    dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    dbNotify.mPetDebrisData = *it->mValue;
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());

    return true;
}

ErrorId 
PetDebrisManager::combinePet(const Guid& petDebrisId, Guid& petId)
{
   /* Map<Guid, PetDebrisData*>::Iter* it = mPetDebrisDataMap.find(petDebrisId);
    if (!it)
    {
        LOG_WARN("Failed to get petdebrisdata by guid:%llu", petDebrisId);
        return LynxErrno::PetNotExist;
    }

    if (!it->mValue)
    {
        LOG_WARN("Failed to get petdebrisdata by guid:%llu", petDebrisId);
        return LynxErrno::PetNotExist;
    }

    PetDebrisTemplate* petDebrisTemplate = PET_DEBRIS_TABLE().get(it->mValue->mTemplateId);
    if (!petDebrisTemplate)
    {
        LOG_WARN("Failed to get petdebristemplate by id:%u", it->mValue->mTemplateId);
        return LynxErrno::TemplateNotExist;
    }

    PetTemplate* heroTemplate = PET_TABLE().get(petDebrisTemplate->mPetId);
    if (!heroTemplate)
    {
        LOG_WARN("Failed to get pettemplate by id:%u", petDebrisTemplate->mPetId);
        return LynxErrno::TemplateNotExist;
    }

    if (it->mValue->mCount < petDebrisTemplate->mCount)
    {
        LOG_WARN("Not enough debris!");
        return LynxErrno::InvalidOperation;
    }

    if (!usePetDebris(petDebrisId, petDebrisTemplate->mCount))
    {
        LOG_WARN("Failed to combine hero!");
        return LynxErrno::InvalidOperation;
    }

    const PetData* petData = mPlayer->getPetManager().createPet(petDebrisTemplate->mPetId);
    if (!petData)
    {
        LOG_WARN("Failed to create pet !");
        return LynxErrno::PetNotExist;
    }

    petId = petData->mBaseData.mPetId;*/
    return LynxErrno::None;
}