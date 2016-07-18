#include "HeroDebrisManager.h"
#include "Player.h"
#include "GameServer.h"


using namespace Lynx;

HeroDebrisManager::HeroDebrisManager() : mPlayer(NULL), mHeroDebrisList(NULL)
{

}


HeroDebrisManager::~HeroDebrisManager()
{

}

bool 
HeroDebrisManager::initial(Player* player)
{
    if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }
    mPlayer = player;
	//mPlayerData.mHeroDebrisData 注释 sec
   /* mHeroDebrisList = &mPlayer->mPlayerData.mHeroDebrisData.mHeroDebrisList;
    for (List<HeroDebrisData>::Iter* it = mHeroDebrisList->begin();
        it != mHeroDebrisList->end();
        it = mHeroDebrisList->next(it))
    {
        mHeroDebrisDataMap.insert(it->mValue.mHeroDebrisId, &it->mValue);
        mHeroDebrisTemplateMap.insert(it->mValue.mTemplateId, &it->mValue);
    }*/
    return true;
}

void 
HeroDebrisManager::release()
{
    mHeroDebrisDataMap.clear();
    mHeroDebrisTemplateMap.clear();
}

void 
HeroDebrisManager::clearBag()
{
    //mPlayerData.mHeroDebrisData 字段注释 sec
	//mPlayer->mPlayerData.mHeroDebrisData.mHeroDebrisList.clear();
}

void 
HeroDebrisManager::destroyHeroDebris(Guid heroDebrisId)
{
    delHeroDebris(heroDebrisId, 0);
}

void 
HeroDebrisManager::delHeroDebris(Guid heroDebrisId, UInt32 count/* count = 0 */)
{

    for (List<HeroDebrisData>::Iter* it = mHeroDebrisList->begin();
        it != mHeroDebrisList->end();
        it = mHeroDebrisList->next(it))
    {
        // 删除指定数量道具
        if (it->mValue.mHeroDebrisId == heroDebrisId)
        {
            if (count && count < it->mValue.mCount)
            {
                it->mValue.mCount -= count;
                UpdateHeroDebrisNotify notify;
                notify.mHeroDebrisGuid = it->mValue.mHeroDebrisId;
                notify.mCount = it->mValue.mCount;
                mPlayer->sendMsgWithAck(notify);

                PersistUpdateHeroDebrisNotify dbNotify;
                dbNotify.mHeroDebrisData = it->mValue;
                dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
                PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
                return;
            }
            mHeroDebrisDataMap.erase(heroDebrisId);
            mHeroDebrisTemplateMap.erase(it->mValue.mTemplateId);
            mHeroDebrisList->erase(it);

            DelHeroDebrisNotify notify;
            notify.mHeroDebrisGuid = heroDebrisId;
            mPlayer->sendMsgWithAck(notify);

            PersistDelHeroDebrisNotify dbNotify;
            dbNotify.mItemId = heroDebrisId;
            dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
            break;
        }
    }
}

void 
HeroDebrisManager::lockHeroDebris(const Guid& itemId)
{
    Map<Guid, HeroDebrisData*>::Iter* it = mHeroDebrisDataMap.find(itemId);
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
HeroDebrisManager::unLockHeroDebris(const Guid& itemId)
{

    Map<Guid, HeroDebrisData*>::Iter* it = mHeroDebrisDataMap.find(itemId);
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
HeroDebrisManager::isHeroDebrisFull()
{
    return (BAG_MAX_SIZE <= mHeroDebrisList->size());
}

bool 
HeroDebrisManager::isHeroDebrisFull(List<TemplateId>& heroDebrisList)
{
    UInt32 curCount = heroDebrisList.size();
    for (List<TemplateId>::Iter* it = heroDebrisList.begin();
        it != heroDebrisList.end();
        it = heroDebrisList.next(it))
    {
        const HeroDebrisData* pt = getHeroDebrisByTemplateId(it->mValue);
        if (pt)
        {
            curCount --;
        }
    }
    return ((heroDebrisList.size() + curCount) > BAG_MAX_SIZE);
}

bool 
HeroDebrisManager::isHeroDebrisFull(List<AwardLine>& awardList)
{
    List<TemplateId> heroDebrisList;
    for (List<AwardLine>::Iter* it = awardList.begin();
        it != NULL; it = awardList.next(it))
    {
        if (it->mValue.mItemType = HERO_DEBRIS_TYPE)
        {
            heroDebrisList.insertTail(it->mValue.mItemId);
        }
    }
    return isHeroDebrisFull(heroDebrisList);
}

const HeroDebrisData*
HeroDebrisManager::getHeroDebrisByTemplateId(TemplateId templateId)
{
    Map<TemplateId, HeroDebrisData*>::Iter* iter = mHeroDebrisTemplateMap.find(templateId);
    if (iter == NULL)
    {
        return NULL;
    }
    return iter->mValue;
}

const HeroDebrisData* 
HeroDebrisManager::getHeroDebrisByGuid(const Guid& guid)
{
    Map<Guid, HeroDebrisData*>::Iter* iter = mHeroDebrisDataMap.find(guid);
    if (!iter)
    {
        return NULL;
    }
    return iter->mValue;
}

const HeroDebrisData* 
HeroDebrisManager::createHeroDebris(TemplateId templateId, UInt32 count, UInt8 source, bool promp/*promp = true*/)
{
    HeroDebrisTemplate* heroDebrisTemplate = HERO_DEBRIS_TABLE().get(templateId);
    if (!heroDebrisTemplate)
    {
        LOG_WARN("Failed to get hero debris template %u", templateId);
        return NULL;
    }

    Map<TemplateId, HeroDebrisData*>::Iter* iter = mHeroDebrisTemplateMap.find(templateId);
    if (iter && iter->mValue)
    {
        HeroDebrisData* heroDebrisData = iter->mValue;
        heroDebrisData->mCount = heroDebrisData->mCount + count > ITEM_MAX_COUNT ? ITEM_MAX_COUNT : heroDebrisData->mCount + count;

        UpdateHeroDebrisNotify notify;
        notify.mHeroDebrisGuid = heroDebrisData->mHeroDebrisId;
        notify.mCount = heroDebrisData->mCount;
        notify.mPromp = promp;
        mPlayer->sendMsgWithAck(notify);

        PersistUpdateHeroDebrisNotify dbNotify;
        dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        dbNotify.mHeroDebrisData = *heroDebrisData;
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
        return iter->mValue;
    }
    else
    {
        HeroDebrisData tempData;
        tempData.mCount = count;
        tempData.mTemplateId = templateId;
        tempData.mHeroDebrisId = LogicSystem::getSingleton().generateItemGuid();
        List<HeroDebrisData>::Iter* it = mHeroDebrisList->insertTail(tempData);
        mHeroDebrisDataMap.insert(tempData.mHeroDebrisId, &it->mValue);
        mHeroDebrisTemplateMap.insert(tempData.mTemplateId, &it->mValue);

        AddHeroDebrisNotify notify;
        notify.mHeroDebrisData = tempData;
        notify.mPromp = promp;
        mPlayer->sendMsgWithAck(notify);

        PersistInsertHeroDebrisNotify dbNotify;
        dbNotify.mHeroDebrisData = tempData;
        dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
        return &it->mValue;
    }
}

bool 
HeroDebrisManager::useHeroDebris(const Guid& guid, UInt32 count)
{
    Map<Guid, HeroDebrisData*>::Iter* it = mHeroDebrisDataMap.find(guid);
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
    UpdateHeroDebrisNotify notify;
    notify.mHeroDebrisGuid = it->mValue->mHeroDebrisId;
    notify.mCount = it->mValue->mCount;
    mPlayer->sendMsgWithAck(notify);
    
    PersistUpdateHeroDebrisNotify dbNotify;
    dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    dbNotify.mHeroDebrisData = *it->mValue;
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
    
    return true;
}

ErrorId 
HeroDebrisManager::combineHero(const Guid& heroDebrisId, Guid& heroId)
{
    Map<Guid, HeroDebrisData*>::Iter* it = mHeroDebrisDataMap.find(heroDebrisId);
    if (!it)
    {
        LOG_WARN("Failed to get herodebrisdata by guid:%llu", heroDebrisId);
        return LynxErrno::HeroNotExist;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to get herodebrisdata by guid:%llu", heroDebrisId);
        return LynxErrno::HeroNotExist;
    }

    HeroDebrisTemplate* heroDebrisTemplate = HERO_DEBRIS_TABLE().get(it->mValue->mTemplateId);
    if (!heroDebrisTemplate)
    {
        LOG_WARN("Failed to get herodebristemplate by id:%u", it->mValue->mTemplateId);
        return LynxErrno::TemplateNotExist;
    }

    HeroTemplate* heroTemplate = HERO_TABLE().get(heroDebrisTemplate->mHeroId);
    if (!heroTemplate)
    {
        LOG_WARN("Failed to get herotemplate by id:%u", heroDebrisTemplate->mHeroId);
        return LynxErrno::TemplateNotExist;
    }

    if (it->mValue->mCount < heroDebrisTemplate->mCount)
    {
        LOG_WARN("Not enough debris!");
        return LynxErrno::InvalidOperation;
    }

    if (!useHeroDebris(heroDebrisId, heroDebrisTemplate->mCount))
    {
        LOG_WARN("Failed to combine hero!");
        return LynxErrno::InvalidOperation;
    }

    const HeroData* heroData = mPlayer->getHeroManager().createHero(heroDebrisTemplate->mHeroId);
    if (!heroData)
    {
        LOG_WARN("Failed to create hero !");
        return LynxErrno::HeroNotExist;
    }
    
    heroId = heroData->mBaseData.mHeroId;
    //成就系统关闭 sec
    //mPlayer->mPlayerData.mAchievementData.mBaseData.mCombineHeroCnt ++;
    //mPlayer->getAchievementManager().checkAchievement(ACHIEVEMENT_TYPE_COMBINE_HERO_CNT);

    return LynxErrno::None;
}