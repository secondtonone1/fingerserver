#include "ExchangeManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

ExchangeManager::ExchangeManager(): mPlayer(NULL), mPlayerExchangeData(NULL), mCurTimeSec(0)
{

}

ExchangeManager::~ExchangeManager()
{

}

bool 
ExchangeManager::initial(Player* player)
{
    if (!player)
    {
        return false;
    }
    mPlayer = player;
	//mPlayerData.mExchangeData 交易场数据字段注释 sec

   /* mPlayerExchangeData = &mPlayer->mPlayerData.mExchangeData;
    for (List<ExchangeItem>::Iter* it = mPlayerExchangeData->mExchangeItemList.begin();
        it != NULL; it = mPlayerExchangeData->mExchangeItemList.next(it))
    {
        mExchangeItemMap.insert(it->mValue.mExchangeId, &it->mValue);
    }

    UInt32 dateNum = TimeUtil::getDateNum(TimeUtil::time() + 2 * 3600);
    if (dateNum > mPlayerExchangeData->mExchangeDateFlag)
    {
        mPlayerExchangeData->mExchangeDateFlag = dateNum;
        PersistUpdateExchangeDateFlagNotify dbNotify;
        dbNotify.mDateFlag  = mPlayerExchangeData->mExchangeDateFlag;
        dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());

        PersistDelExchangeItemNotify delNotify;
        delNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(delNotify, mPlayer->getPlayerGuid());

        resetDaily();
    }*/

   /* PlayerInfo* playerInfo = LogicSystem::getSingleton().getPlayerInfo(mPlayer->getPlayerGuid());
    if (!playerInfo)
    {
        LOG_WARN("Failed to get PlyaerInfo by id: %llu", mPlayer->getPlayerGuid());
        return false;
    }*/
	//以后添加竞技场后，需要打开注释，目前playerinfo中没有竞技场兑换标记和昨日排名字段，先注释sec
   /* mPlayerExchangeData->mArenaExchangeFlag = playerInfo->mArenaInfo.mArenaExchangeFlag;
    mPlayerExchangeData->mArenaLastRank = playerInfo->mArenaInfo.mYestodayBestRank;*/

    return true;
}

void 
ExchangeManager::release()
{
    mExchangeItemMap.clear();
}

void 
ExchangeManager::update(const UInt64& accTime)
{
    UInt32 curTime = TimeUtil::getTimeSec();
    if (curTime > mCurTimeSec)
    {
        mCurTimeSec = curTime;
        UInt32 dateNum = TimeUtil::getDateNum(TimeUtil::time() + 2 * 3600);
        if (dateNum > mPlayerExchangeData->mExchangeDateFlag)
        {
            mPlayerExchangeData->mExchangeDateFlag = dateNum;
            PersistUpdateExchangeDateFlagNotify dbNotify;
            dbNotify.mDateFlag  = mPlayerExchangeData->mExchangeDateFlag;
            dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());

            PersistDelExchangeItemNotify delNotify;
            delNotify.mPlayerGuid = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(delNotify, mPlayer->getPlayerGuid());

            resetDaily();


        }
       
    }
}

void 
ExchangeManager::resetDaily()
{
    for (List<ExchangeItem>::Iter* it = mPlayerExchangeData->mExchangeItemList.begin();
        it != NULL;)
    {
        ExchangeTemplate* exchangeTemplate = EXCHANGE_TABLE().get(it->mValue.mExchangeId);
        if (!exchangeTemplate)
        {
            LOG_WARN("Failed to get ExchangeTemplate by id:%u", it->mValue.mExchangeId);
            it = mPlayerExchangeData->mExchangeItemList.next(it);
            continue;
        }
        if(exchangeTemplate->mLimitType != STORE_ITEM_LIMIT_NUM_CAREER)
        {
            mExchangeItemMap.erase(it->mValue.mExchangeId);
            it = mPlayerExchangeData->mExchangeItemList.erase(it);

        }
        else
        {
            it = mPlayerExchangeData->mExchangeItemList.next(it);
        }
    }
}

ErrorId
ExchangeManager::exchangeItem(UInt32 exchangeId, UInt32 count/*count = 1*/)
{
    ExchangeTemplate* exchangeTemplate = EXCHANGE_TABLE().get(exchangeId);
    if (!exchangeTemplate)
    {
        LOG_WARN("Failed to get ExchangeTemplate by id: %u", exchangeId);
        return LynxErrno::InvalidParameter;
    }

	//account 中的RMB不足情况 sec注释
   /* if (mPlayer->getPlayerData().mAccountData.mRmb < exchangeTemplate->mDiamondDiscountCost * count)
    {
        return LynxErrno::RmbNotEnough;
    }*/
    if (mPlayer->getPlayerData().mBaseData.m_nGold < exchangeTemplate->mGoldDiscountCost * count)
    {
        return LynxErrno::GoldNotEnough;
    }

	//玩家的Honor字段和soul字段没用，注释sec

    /*if (mPlayer->getPlayerData().mBaseData.mSoul < exchangeTemplate->mSoulDiscountCost * count)
    {
        return LynxErrno::SoulNotEnough;
    }
    if (mPlayer->getPlayerData().mBaseData.mHonor < exchangeTemplate->mHonorDiscountCost * count)
    {
        return LynxErrno::HonorNotEnough;
    }*/
	//PlayerData().mTowerData 字段注释sec
    /*if (mPlayer->getPlayerData().mTowerData.mCrystalCount < exchangeTemplate->mCrystalDiscountCost * count)
    {
        return LynxErrno::CrystalNotEnough;
    }*/


    if(exchangeTemplate->mLimitType != STORE_ITEM_LIMIT_NUM_NONE)
    {
        UInt32 limitCnt = exchangeTemplate->mLimitCount;
        Map<UInt32, ExchangeItem*>::Iter* it = mExchangeItemMap.find(exchangeId);
        if (it)
        {
            if ((it->mValue->mBuyCnt + count) > limitCnt)
            {
                return LynxErrno::InvalidParameter;
            }
        } 
    }
   
    switch (exchangeTemplate->mItemType)
    {
    case ITEM_TYPE:
        {
            ItemTemplate* itemTemplate = ITEM_TABLE().get(exchangeTemplate->mItemId);
            if (!itemTemplate)
            {
                LOG_WARN("Failed to get itemTemplate by id:%u", exchangeTemplate->mItemId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getItemManager().createItem(exchangeTemplate->mItemId, exchangeTemplate->mItemCount, 0);
        }
        break;
    case EQUIP_TYPE:
        {
            EquipTemplate* equipTemplate = EQUIP_TABLE().get(exchangeTemplate->mItemId);
            if (!equipTemplate)
            {
                LOG_WARN("Failed to get equipTemplate by id:%u", exchangeTemplate->mItemId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getEquipManager().createEquip(exchangeTemplate->mItemId, 0);
        }
        break;
    case HERO_TYPE:
        {
            HeroTemplate* heroTemplate = HERO_TABLE().get(exchangeTemplate->mItemId);
            if (!heroTemplate)
            {
                LOG_WARN("Failed to get heroTemplate by id:%u", exchangeTemplate->mItemId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getHeroManager().createHero(exchangeTemplate->mItemId);
        }
        break;
    case PET_TYPE:
        {
            PetTemplate* petTemplate = PET_TABLE().get(exchangeTemplate->mItemId);
            if (!petTemplate)
            {
                LOG_WARN("Failed to get PetTemplate by id:%u", exchangeTemplate->mItemId);
                return LynxErrno::TemplateNotExist;
            }
       //     mPlayer->getPetManager().createPet(exchangeTemplate->mItemId);
        }
        break;
    case HERO_DEBRIS_TYPE:
        {
            HeroDebrisTemplate* heroDebrisTemplate = HERO_DEBRIS_TABLE().get(exchangeTemplate->mItemId);
            if (!heroDebrisTemplate)
            {
                LOG_WARN("Failed to get herodebrisTemplate by id:%u", exchangeTemplate->mItemId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getHeroDebrisManager().createHeroDebris(exchangeTemplate->mItemId, exchangeTemplate->mItemCount, 0);
        }
        break;
    case PET_DEBRIS_TYPE:
        {
            PetDebrisTemplate* petDebrisTemplate = PET_DEBRIS_TABLE().get(exchangeTemplate->mItemId);
            if (!petDebrisTemplate)
            {
                LOG_WARN("Failed to get petDebrisTemplate by id:%u", exchangeTemplate->mItemId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getPetDebrisManager().createPetDebris(exchangeTemplate->mItemId, exchangeTemplate->mItemCount, 0);
        }
        break;
    case RUNE_TYPE:
        {
            RuneTemplate* runeTemplate = RUNE_TABLE().get(exchangeTemplate->mItemId);
            if (!runeTemplate)
            {
                LOG_WARN("Failed to get runeTemplate by id: %u", exchangeTemplate->mItemId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getRuneManager().createRune(exchangeTemplate->mItemId, 0);
        }
        break;
    default:
        LOG_WARN("invalid item type:%u", exchangeTemplate->mItemType);
        return LynxErrno::InvalidParameter;
    }
    if (exchangeTemplate->mGoldDiscountCost)
    {
        mPlayer->reducePlayerGold(exchangeTemplate->mGoldDiscountCost);
    }
    if (exchangeTemplate->mDiamondDiscountCost)
    {

    }
    if (exchangeTemplate->mSoulDiscountCost)
    {
        mPlayer->reducePlayerSoul(exchangeTemplate->mSoulDiscountCost);
    }
    if (exchangeTemplate->mHonorDiscountCost)
    {
        mPlayer->reducePlayerHonor(exchangeTemplate->mHonorDiscountCost);
    }
	//mPlayerData.mTowerData字段注释 sec
    /*if (exchangeTemplate->mCrystalDiscountCost)
    {
        mPlayer->mPlayerData.mTowerData.mCrystalCount -= exchangeTemplate->mCrystalDiscountCost;
        mPlayer->getTowerManager().syncTowerDataToClient();
        mPlayer->getTowerManager().saveTowerData();
        
    }*/

    Map<UInt32, ExchangeItem*>::Iter* it = mExchangeItemMap.find(exchangeId);
    if (!it)
    {
        ExchangeItem exchangeItem;
        exchangeItem.mExchangeId = exchangeId;
        exchangeItem.mBuyCnt = count;
        exchangeItem.mLimitType = exchangeTemplate->mLimitType;

        List<ExchangeItem>::Iter* newIter = mPlayerExchangeData->mExchangeItemList.insertTail(exchangeItem);
        mExchangeItemMap.insert(exchangeId, &newIter->mValue);

        AddExchangeItemNotify notify;
        notify.mExchangeItem = exchangeItem;
        mPlayer->sendMsgWithAck(notify);
        
        PersistInsertExchangeItemNotify dbNotify;
        dbNotify.mExchangeItem = exchangeItem;
        dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
    }
    else
    {
        it->mValue->mBuyCnt += count;

        UpdateExchangeItemNotify notify;
        notify.mExchangeItem = *it->mValue;
        mPlayer->sendMsgWithAck(notify);

        PersistUpdateExchangeItemNotify dbNotify;
        dbNotify.mExchangeItem = *it->mValue;
        dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
    }
    
    return LynxErrno::None;
}

ErrorId
ExchangeManager::receiveHonorAward(bool& flag)
{
	//原游戏用到的收到荣誉奖励的函数，目前playerinfo 里没有相关的数据，所以注释了sec   
	/*PlayerInfo* playerInfo = LogicSystem::getSingleton().getPlayerInfo(mPlayer->getPlayerGuid());
    if (!playerInfo)
    {
        LOG_WARN("Failed to get PlayerInfo by id: %llu", mPlayer->getPlayerGuid());
        return LynxErrno::InvalidParameter;
    }
    if (playerInfo->mArenaInfo.mArenaExchangeFlag)
    {
        return LynxErrno::InvalidParameter;
    }
    HonorAwardTemplate* honorAwardTemplate = HONOR_AWARD_TABLE().get(playerInfo->mArenaInfo.mYestodayBestRank);
    if (!honorAwardTemplate)
    {
        mPlayer->increasePlayerHonor(50);
    }
    else
    {
        mPlayer->increasePlayerHonor(honorAwardTemplate->mAward);
    }
    
    mPlayer->mPlayerData.mExchangeData.mArenaExchangeFlag = true;
    playerInfo->mArenaInfo.mArenaExchangeFlag = true;
    flag = playerInfo->mArenaInfo.mArenaExchangeFlag;
	*/
    return LynxErrno::None;
}