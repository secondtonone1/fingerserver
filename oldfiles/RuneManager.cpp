#include "RuneManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

RuneManager::RuneManager() : mPlayer(NULL), mRuneList(NULL)
{

}


RuneManager::~RuneManager()
{

}

bool 
RuneManager::initial(Player* player)
{
    //mPlayerData.mRuneData 注释
	if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }
    mPlayer = player;
	/*
    mRuneList = &mPlayer->mPlayerData.mRuneData.mRuneDataList;
    for (List<RuneData>::Iter* it = mRuneList->begin();
        it != mRuneList->end();
        it = mRuneList->next(it))
    {
        mRuneMap.insert(it->mValue.mRuneId, &it->mValue);
    }*/
    return true;
}

void 
RuneManager::release()
{
    mRuneList->clear();
    mRuneMap.clear();
}

void 
RuneManager::calculateRuneData(UInt8 runePresentPos)
{
    if (runePresentPos != 0)
    {
        UInt8 heroPresentPos = (runePresentPos - 1) / 5 + 1;
        const HeroData* heroData = mPlayer->getHeroManager().getHeroDataByPresentPos(heroPresentPos);
        if (heroData)
        {
            mPlayer->getHeroManager().calculateHeroData(heroData->mBaseData.mHeroId);
        }
    }
}

void 
RuneManager::destroyAllRune()
{
    for (List<RuneData>::Iter* it = mRuneList->begin();
        it != mRuneList->end();
        it = mRuneList->next(it))
    {
        DelRuneNotify notify;
        notify.mRuneGuid = it->mValue.mRuneId;
        mPlayer->sendMsgWithAck(notify);

        PersistDelRuneNotify req;
        req.mRuneId = it->mValue.mRuneId;
        req.mPlayerGuid = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(req, mPlayer->getPlayerGuid());
    }
    mRuneMap.clear();
    mRuneList->clear();

}



void 
RuneManager::destroyRune(Guid itemId)
{
    
    for (List<RuneData>::Iter* it = mRuneList->begin();
        it != mRuneList->end();
        it = mRuneList->next(it))
    {
        if (it->mValue.mRuneId == itemId)
        {
            mRuneMap.erase(itemId);
            mRuneList->erase(it);

            DelRuneNotify notify;
            notify.mRuneGuid = itemId;
            mPlayer->sendMsgWithAck(notify);

            PersistDelRuneNotify dbNotify;
            dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
            dbNotify.mRuneId = itemId;
            PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
            break;
        }
    }


}

void 
RuneManager::lockRune(const Guid& runeId)
{
    Map<Guid, RuneData*>::Iter* it = mRuneMap.find(runeId);
    if (!it)
    {
        LOG_WARN("Failed to find item from temp map by id:%llu", runeId);
        return;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to find item by id:%llu", runeId);
        return;
    }
    it->mValue->mLock = true;
}

void 
RuneManager::unLockRune(const Guid& runeId)
{
    Map<Guid, RuneData*>::Iter* it = mRuneMap.find(runeId);
    if (!it)
    {
        LOG_WARN("Failed to find item from temp map by id:%llu", runeId);
        return;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to find item by id:%llu", runeId);
        return;
    }
    it->mValue->mLock = false;
}

bool 
RuneManager::isRuneFull()
{
    return (BAG_MAX_SIZE <= mRuneList->size());
}

bool 
RuneManager::isRuneFull(List<TemplateId>& runeList)
{
    return (mRuneList->size() + runeList.size()) > BAG_MAX_SIZE;
}

bool 
RuneManager::isRuneFull(List<AwardLine>& awardList)
{
    List<TemplateId> runeList;
    for (List<AwardLine>::Iter* it = awardList.begin();
        it != NULL; it = awardList.next(it))
    {
        if (it->mValue.mItemType == RUNE_TYPE)
        {
            runeList.insertTail(it->mValue.mItemId);
        }
    }
    return isRuneFull(runeList);
}

const RuneData* 
RuneManager::createRune(TemplateId runeTemplateId, UInt8 source, bool promp/*promp = true*/)
{
    RuneTemplate* runeTemplate = RUNE_TABLE().get(runeTemplateId);
    if (!runeTemplate)
    {
        LOG_WARN("Failed to get equip template %u", runeTemplateId);
        return NULL;
    }

    RuneData runeData;
    runeData.mTemplateId = runeTemplateId;
    runeData.mRuneId = LogicSystem::getSingleton().generateItemGuid();
    runeData.mPresentPos = 0;
    runeData.mRefineLevel = 0;
    runeData.mRefineLevelExp = 0;

    List<RuneData>::Iter* it = mRuneList->insertTail(runeData);
    mRuneMap.insert(it->mValue.mRuneId, &it->mValue);

    AddRuneNotify notify;
    notify.mRuneData = runeData;
    notify.mPromp = promp;
    mPlayer->sendMsgWithAck(notify);

    PersistInsertRuneNotify req;
    req.mRuneData = runeData;
    req.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(req, mPlayer->getPlayerGuid());
    return &it->mValue;
}


ErrorId 
RuneManager::useRune(const Guid& runeId, UInt8 targetPos)
{
    
    Map<Guid, RuneData*>::Iter* it = mRuneMap.find(runeId);
    if (!it)
    {
        LOG_WARN("Failed to find rune from rune map, guid:%llu", runeId);
        return LynxErrno::InvalidParameter;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to find rune from bag, guid:%llu", runeId);
        return LynxErrno::InvalidParameter;
    }

    if (!targetPos || targetPos > PLAYER_RUNE_MAX_POSITION)
    {
        LOG_WARN("invalid targetPos:%u", targetPos);
        return LynxErrno::InvalidParameter;
    }
    RuneTemplate* runeTemplate = RUNE_TABLE().get(it->mValue->mTemplateId);
    if (!runeTemplate)
    {
        LOG_WARN("Failed to get runetemplate by id:%u", it->mValue->mTemplateId);
        return LynxErrno::TemplateNotExist;
    }
    if (runeTemplate->mRuneType != (targetPos - 1) % PLAYER_RUNE_PER_HERO_MAX_COUNT + 1)
    {
        LOG_WARN("invalid targetPos, rune type not match! rune:%u, targetpos:%u", runeTemplate->mRuneType, targetPos);
        return LynxErrno::InvalidParameter;
    }


    // 原有符文替换
    for (List<RuneData>::Iter* iter = mRuneList->begin();
        iter != mRuneList->end(); iter = mRuneList->next(iter))
    {
        if (iter->mValue.mPresentPos == targetPos)
        {
            iter->mValue.mPresentPos = 0;

            UpdateRunePresentPosNotify notify;
            notify.mRuneId = iter->mValue.mRuneId;
            notify.mPresentPos = 0;
            mPlayer->sendMsgWithAck(notify);

            PersistUpdateRuneNotify request;
            request.mRuneData = iter->mValue;
            request.mPlayerGuid = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(request, mPlayer->getPlayerGuid());
            break;
        }
    }

    it->mValue->mPresentPos = targetPos;
    mPlayer->updatePlayerPresentRunes();
    calculateRuneData(targetPos);
    //mPlayer->calculatePlayerData();

    UpdateRunePresentPosNotify notify;
    notify.mRuneId = it->mValue->mRuneId;
    notify.mPresentPos = targetPos;
    mPlayer->sendMsgWithAck(notify);

    PersistUpdateRuneNotify req;
    req.mRuneData = *it->mValue;
    req.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(req, mPlayer->getPlayerGuid());

    return LynxErrno::None;
}

void 
RuneManager::setRuneRefineLevel(const Guid& runeId, UInt32 level)
{
    Map<Guid, RuneData*>::Iter* it = mRuneMap.find(runeId);
    if (!it)
    {
        LOG_WARN("Failed to find equip from equipmap %llu", runeId);
        return;
    }
    if (!it->mValue)
    {
        LOG_WARN("Failed to get equip %llu", runeId);
        return;
    }
    if (level > EQUIP_REFINE_MAX_LEVEL)
    {
        LOG_WARN("Invalid refine level %u", level);
        return;
    }
    it->mValue->mRefineLevel = level;

    SyncRuneRefineLevelNotify notify;
    notify.mRuneId = runeId;
    notify.mRefineLevel = level;
    mPlayer->sendMsgWithAck(notify);

    PersistUpdateRuneNotify dbNotify;
    dbNotify.mRuneData = *it->mValue;
    dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
}

ErrorId 
RuneManager::addRuneRefineExp(const Guid& runeId, UInt32 incExp, UInt32& refineLevel)
{
    Map<Guid, RuneData*>::Iter* iter = mRuneMap.find(runeId);
    if (!iter)
    {
        LOG_WARN("Failed to find equip from equipmap %llu", runeId);
        return LynxErrno::RuneNotExist;
    }

    RuneData* runeData = iter->mValue;
    if (!runeData)
    {
        LOG_WARN("Rune data pointer is null.");
        return LynxErrno::RuneNotExist;
    }

    RuneTemplate* runeTemplate = RUNE_TABLE().get(runeData->mTemplateId);
    if (!runeTemplate)
    {
        LOG_WARN("Failed to get rune template %u", runeData->mTemplateId);
        return LynxErrno::TemplateNotExist;
    }

    // 计算增加的经验可以升几级
    UInt32 remainExp = runeData->mRefineLevelExp + incExp;
    UInt32 upRefineLevel = 0;
    do
    {
        TemplateId runeRefineTemplateId = runeTemplate->mRuneType * 1000 + runeData->mRefineLevel + upRefineLevel;
        RuneRefineTemplate* runeRefineTemplate = RUNE_REFINE_TABLE().get(runeRefineTemplateId);
        if (!runeRefineTemplate)
        {
            LOG_WARN("Failed to get rune refine template by %u", runeRefineTemplateId);
            break;
        }

        if (runeTemplate->mStarLevel == 1)
        {
            if (remainExp >= runeRefineTemplate->mExp1)
            {
                remainExp -= runeRefineTemplate->mExp1;
                upRefineLevel++;
            }
            else if (remainExp < runeRefineTemplate->mExp1)
            {
                break;
            }
        }
        else if (runeTemplate->mStarLevel == 2)
        {
            if (remainExp >= runeRefineTemplate->mExp2)
            {
                remainExp -= runeRefineTemplate->mExp2;
                upRefineLevel++;
            }
            else if (remainExp < runeRefineTemplate->mExp2)
            {
                break;
            }
        }
        else if (runeTemplate->mStarLevel == 3)
        {
            if (remainExp >= runeRefineTemplate->mExp3)
            {
                remainExp -= runeRefineTemplate->mExp3;
                upRefineLevel++;
            }
            else if (remainExp < runeRefineTemplate->mExp3)
            {
                break;
            }
        }
        else if (runeTemplate->mStarLevel == 4)
        {
            if (remainExp >= runeRefineTemplate->mExp4)
            {
                remainExp -= runeRefineTemplate->mExp4;
                upRefineLevel++;
            }
            else if (remainExp < runeRefineTemplate->mExp4)
            {
                break;
            }
        }
        else if (runeTemplate->mStarLevel == 5)
        {
            if (remainExp >= runeRefineTemplate->mExp5)
            {
                remainExp -= runeRefineTemplate->mExp5;
                upRefineLevel++;
            }
            else if (remainExp < runeRefineTemplate->mExp5)
            {
                break;
            }
        }
        else if (runeTemplate->mStarLevel == 6)
        {
            if (remainExp >= runeRefineTemplate->mExp6)
            {
                remainExp -= runeRefineTemplate->mExp6;
                upRefineLevel++;
            }
            else if (remainExp < runeRefineTemplate->mExp6)
            {
                break;
            }
        }
    } while (1);

    if (upRefineLevel > 0)
    {
        setRuneRefineLevel(runeId, runeData->mRefineLevel + upRefineLevel);
    }

    runeData->mRefineLevelExp = remainExp;

    SyncRuneRefineExpNotify notify;
    notify.mRuneId = runeId;
    notify.mRefineLevelExp = remainExp;
    mPlayer->sendMsgWithAck(notify);

    PersistUpdateRuneNotify dbNotify;
    dbNotify.mRuneData = *runeData;
    dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());

    refineLevel = runeData->mRefineLevel;
    return LynxErrno::None;
}

ErrorId 
RuneManager::refineRune(const Guid& runeId, const List<Guid>& itemList, UInt32& refineLevel)
{
    Map<Guid, RuneData*>::Iter* iter = mRuneMap.find(runeId);
    if (!iter)
    {
        LOG_WARN("Failed to find equip from equipmap %llu", runeId);
        return LynxErrno::RuneNotExist;
    }
    //不能把本身符文当成强化材料
    for (List<Guid>::Iter* it = itemList.begin();
        it != NULL; it = itemList.next(it))
    {
        if (it->mValue == runeId)
        {
            return LynxErrno::InvalidParameter;
        }
    }

    RuneData* runeData = iter->mValue;
    if (!runeData)
    {
        LOG_WARN("Rune data pointer is null.");
        return LynxErrno::RuneNotExist;
    }

    // 检查操作是否有效
    for (List<Guid>::Iter* jter = itemList.begin();
        jter != NULL; jter = itemList.next(jter))
    {
        const RuneData* tmpRuneData = getRuneDataByGuid(jter->mValue);
        if (!tmpRuneData)
        {
            LOG_WARN("Failed to get rune data %llu", jter->mValue);
            return LynxErrno::InvalidParameter;
        }

        RuneTemplate* runeTemplate = RUNE_TABLE().get(tmpRuneData->mTemplateId);
        if (!runeTemplate)
        {
            LOG_WARN("Failed to get rune template %u", tmpRuneData->mTemplateId);
            return LynxErrno::TemplateNotExist;
        }

        TemplateId refineTemplateId = runeTemplate->mRuneType * 1000 + tmpRuneData->mRefineLevel;
        RuneRefineTemplate* runeRefineTemplate = RUNE_REFINE_TABLE().get(refineTemplateId);
        if (!runeRefineTemplate)
        {
            LOG_WARN("Failed to get rune refine template %u", refineTemplateId);
            return LynxErrno::TemplateNotExist;
        }
    }

    // 执行操作
    UInt32 incExp = 0;
    UInt32 requireGold = 0;

    for (List<Guid>::Iter* jter = itemList.begin();
        jter != NULL; jter = itemList.next(jter))
    {
        const RuneData* tmpRuneData = getRuneDataByGuid(jter->mValue);
        if (!tmpRuneData)
        {
            LOG_WARN("Failed to get rune data %llu", jter->mValue);
            return LynxErrno::InvalidParameter;
        }

        RuneTemplate* runeTemplate = RUNE_TABLE().get(tmpRuneData->mTemplateId);
        if (!runeTemplate)
        {
            LOG_WARN("Failed to get rune template %u", tmpRuneData->mTemplateId);
            return LynxErrno::TemplateNotExist;
        }

        TemplateId refineTemplateId = runeTemplate->mRuneType * 1000 + tmpRuneData->mRefineLevel;
        RuneRefineTemplate* runeRefineTemplate = RUNE_REFINE_TABLE().get(refineTemplateId);
        if (!runeRefineTemplate)
        {
            LOG_WARN("Failed to get rune refine template %u", refineTemplateId);
            return LynxErrno::TemplateNotExist;
        }

        switch (runeTemplate->mStarLevel)
        {
        case 1:
            incExp += runeRefineTemplate->mTotalExp1 + runeTemplate->mDestroyExp;
            break;
        case 2:
            incExp += runeRefineTemplate->mTotalExp2 + runeTemplate->mDestroyExp;
            break;
        case 3:
            incExp += runeRefineTemplate->mTotalExp3 + runeTemplate->mDestroyExp;
            break;
        case 4:
            incExp += runeRefineTemplate->mTotalExp4 + runeTemplate->mDestroyExp;
            break;
        case 5: 
            incExp += runeRefineTemplate->mTotalExp5 + runeTemplate->mDestroyExp;
            break;
        case 6:
            incExp += runeRefineTemplate->mTotalExp6 + runeTemplate->mDestroyExp;
            break;
        }

        destroyRune(jter->mValue);
    }

    return addRuneRefineExp(runeId, incExp, refineLevel);
}

const RuneData* 
RuneManager::getRuneDataByGuid(const Guid& runeId)
{
    Map<Guid, RuneData*>::Iter* iter = mRuneMap.find(runeId);
    if (iter)
    {
        return iter->mValue;
    }
    return NULL;
}

void 
RuneManager::setRunePresentPos(const Guid& runeId, UInt8 pos)
{
    Map<Guid, RuneData*>::Iter* iter = mRuneMap.find(runeId);
    if (!iter)
    {
        LOG_WARN("Failed to find equip from equipmap %llu", runeId);
        return;
    }

    RuneData* runeData = iter->mValue;
    if (!runeData)
    {
        LOG_WARN("Rune data pointer is null.");
        return;
    }

    if (pos < 1 || pos > 25)
    {
        LOG_WARN("Invalid pos %u", pos);
        return;
    }

    runeData->mPresentPos = pos;
}

ErrorId
RuneManager::sellRune(List<Guid>& runeList, UInt32& getGold)
{
    for (List<Guid>::Iter* it = runeList.begin();
        it != NULL; it = runeList.next(it))
    {
        Map<Guid, RuneData*>::Iter* existIter = mRuneMap.find(it->mValue);
        if(!existIter)
        {
            LOG_WARN("Failed to get rune by id:%llu", it->mValue);
            return LynxErrno::InvalidParameter;
        }
        if (existIter->mValue->mPresentPos)
        {
            LOG_WARN("Can not sell rune ,rune is on present");
            return LynxErrno::InvalidParameter;
        }
    }
    for (List<Guid>::Iter* it = runeList.begin();
        it != NULL; it = runeList.next(it))
    {
        Map<Guid, RuneData*>::Iter* existIter = mRuneMap.find(it->mValue);
        RuneTemplate* runeTemplate = RUNE_TABLE().get(existIter->mValue->mTemplateId);
        if (!runeTemplate)
        {
            LOG_WARN("Failed to get runeTemplate by id: %u", existIter->mValue->mTemplateId);
            continue;
        }
        destroyRune(existIter->mValue->mRuneId);
        //mPlayer->increasePlayerGold(runeTemplate->mDestroyPrice);
        getGold += runeTemplate->mDestroyPrice;
    }
    mPlayer->getItemManager().createItem(ITEM_GOLD_TEMPLATEID, getGold, 0);
    return LynxErrno::None;
}