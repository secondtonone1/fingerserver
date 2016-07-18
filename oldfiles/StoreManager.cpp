#include "StoreManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

UInt8 mResetSecretStoreFlag[SECRET_STORE_RESET_TIMES_DAILY] = {0};

StoreManager::StoreManager(): mPlayer(NULL), mPlayerStoreData(NULL), mCurTimeSec(0)
{

}

StoreManager::~StoreManager()
{

}

bool 
StoreManager::initial(Player* player)
{
    if (!player)
    {
        return false;
    }
    mPlayer = player;

	//mPlayerData.mStoreData sec 注释
   /* mPlayerStoreData = &mPlayer->mPlayerData.mStoreData;
    for (List<StoreItemData>::Iter* it = mPlayerStoreData->mStoreItemList.begin();
        it != NULL; it = mPlayerStoreData->mStoreItemList.next(it))
    {
        mStoreItemMap.insert(it->mValue.mTemplateId, &it->mValue);
    }

    UInt32 dateNum = TimeUtil::getDateNum();
    if (dateNum > mPlayerStoreData->mShoppingTime)
    {
        mPlayerStoreData->mShoppingTime = dateNum;
        PersistUpdateStoreDateFlagNotify updateNotify;
        updateNotify.mDateFlag = mPlayerStoreData->mShoppingTime;
        updateNotify.mPlayerId = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(updateNotify, mPlayer->getPlayerGuid());

        PersistDelStoreItemNotify resetNotify;
        resetNotify.mPlayerId = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(resetNotify, mPlayer->getPlayerGuid());

        mPlayerStoreData->mResetSecretStoreCnt = 0;
        PersistUpdateSecretStoreItemNotify dbNotify;
        dbNotify.mResetCnt = mPlayerStoreData->mResetSecretStoreCnt;
        dbNotify.mItemList = mPlayerStoreData->mSecretStoreItemList;
        dbNotify.mPlayerId = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());

        resetDaily();
    }

    if (!mPlayerStoreData->mSecretStoreItemList.size())
    {
        resetSecretStore();
    }
    if (TimeUtil::getTimeSec() / 7200 > mPlayerStoreData->mSecretStoreResetTime)
    {
        resetSecretStore();
    }*/
    return true;
}

void 
StoreManager::release()
{
    mStoreItemMap.clear();
}

void 
StoreManager::update(const UInt64& accTime)
{
    UInt32 curTime = TimeUtil::getTimeSec();
    if (curTime > mCurTimeSec)
    {
        mCurTimeSec = curTime;
        UInt32 dateNum = TimeUtil::getDateNum();
        if (dateNum > mPlayerStoreData->mShoppingTime)
        {
            mPlayerStoreData->mShoppingTime = dateNum;
            PersistUpdateStoreDateFlagNotify updateNotify;
            updateNotify.mDateFlag = mPlayerStoreData->mShoppingTime;
            updateNotify.mPlayerId = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(updateNotify, mPlayer->getPlayerGuid());

            PersistDelStoreItemNotify resetNotify;
            resetNotify.mPlayerId = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(resetNotify, mPlayer->getPlayerGuid());

            resetDaily();
            ResetStoreDateNotify notify;
            mPlayer->sendMsgWithAck(notify);

            mPlayerStoreData->mResetSecretStoreCnt = 0;
            PersistUpdateSecretStoreItemNotify dbNotify;
            dbNotify.mResetCnt = mPlayerStoreData->mResetSecretStoreCnt;
            dbNotify.mItemList = mPlayerStoreData->mSecretStoreItemList;
            dbNotify.mPlayerId = mPlayer->getPlayerGuid();
            PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
        }
        // 刷新灵力商店
        UInt32 todaySec = TimeUtil::getTimeSec() - TimeUtil::getTodayBaseSec();
        if (todaySec % 7200 == 0 && !mResetSecretStoreFlag[todaySec / 7200])
        {
            mResetSecretStoreFlag[todaySec / 7200] = todaySec;
            resetSecretStore();
            
            UpdatePlayerSecretStoreDataNotify notify;
            notify.mResetCnt = mPlayerStoreData->mResetSecretStoreCnt;
            notify.mItemList = mPlayerStoreData->mSecretStoreItemList;
            mPlayer->sendMsgWithAck(notify);

            ResetSecretStoreItemNotify resetNotify;
            mPlayer->sendMsgWithAck(resetNotify);
        
        }
    }
}

ErrorId 
StoreManager::buyStoreItem(UInt32& id, UInt8& type, UInt32& count)
{
    StoreTemplate* storeTemplate = STORE_TABLE().get(id);
    if (!storeTemplate)
    {
        LOG_WARN("Failed to get storeTemplate by id: %u", id);
        return LynxErrno::InvalidParameter;
    }
    ItemTemplate* itemTemplate = ITEM_TABLE().get(storeTemplate->mTemplateId);
    if (!itemTemplate)
    {
        LOG_WARN("Failed to get itemTemplate by id: %u", storeTemplate->mTemplateId);
        return LynxErrno::TemplateNotExist;
    }
    // 判断限时道具
    if (storeTemplate->mLimitTimeType == STORE_ITEM_LIMIT_TIME_SELL)
    {
        if (storeTemplate->mLimitTimeStart >= storeTemplate->mLimitTimeEnd)
        {
            LOG_WARN("invalid storetemplate value");
            return LynxErrno::InvalidParameter;
        }
        UInt32 curTime = TimeUtil::getTimeSec();
        if (curTime < storeTemplate->mLimitTimeStart || curTime > storeTemplate->mLimitTimeEnd)
        {
            return LynxErrno::InvalidParameter;
        }
    }
    UInt32 limitCnt = 0;
    // 判断限次道具
    if (storeTemplate->mLimitNumberType != STORE_ITEM_LIMIT_NUM_NONE)
    {
        switch(mPlayer->mPlayerData.mBaseData.m_nVipLevel)
        {
        case 0:
            limitCnt = storeTemplate->mV0Cnt;
            break;
        case 1:
            limitCnt = storeTemplate->mV1Cnt;
            break;
        case 2:
            limitCnt = storeTemplate->mV2Cnt;
            break;
        case 3:
            limitCnt = storeTemplate->mV3Cnt;
            break;
        case 4:
            limitCnt = storeTemplate->mV4Cnt;
            break;
        case 5:
            limitCnt = storeTemplate->mV5Cnt;
            break;
        case 6:
            limitCnt = storeTemplate->mV6Cnt;
            break;
        case 7:
            limitCnt = storeTemplate->mV7Cnt;
            break;
        case 8:
            limitCnt = storeTemplate->mV8Cnt;
            break;
        case 9:
            limitCnt = storeTemplate->mV9Cnt;
            break;
        case 10:
            limitCnt = storeTemplate->mV10Cnt;
            break;
        case 11:
            limitCnt = storeTemplate->mV11Cnt;
            break;
        case 12:
            limitCnt = storeTemplate->mV12Cnt;
            break;
        }
        Map<TemplateId, StoreItemData*>::Iter* it = mStoreItemMap.find(itemTemplate->mTemplateId);
        if (it)
        {
            if ((it->mValue->mCount + count) > limitCnt)
            {
                return LynxErrno::InvalidParameter;
            }
        }        
    }
    // 购买道具,判断折扣
    float discount = 1.0;
    if (storeTemplate->mLimitTimeType == STORE_ITEM_LIMIT_TIME_DISCOUNT)
    {
        UInt32 curTime = TimeUtil::getTimeSec();
        if(curTime >= storeTemplate->mLimitTimeStart && curTime <= storeTemplate->mLimitTimeEnd)
        {
            discount = storeTemplate->mLimitTimeDiscount;
        }
    }
	// RMB判断
    //if(mPlayer->getPlayerData().mAccountData.mRmb < (UInt32)(storeTemplate->mCurrentPrice * discount * count))
    //{
    //    //return LynxErrno::RmbNotEnough;
    //}
#if 0


    if (storeTemplate->mType == STORE_ITEM_TYPE_ITEM)
    {
        mPlayer->getItemManager().createItem(itemTemplate->mTemplateId, storeTemplate->mCount * count, 0);
    }
    else if (storeTemplate->mType == STORE_ITEM_TYPE_GIFTBAG)
    {
        AwardTemplate* awardTemplate = AWARD_TABLE().get(itemTemplate->mAwardId);
        if (!awardTemplate)
        {
            LOG_WARN("failed to get awardtemplate by id: %u", itemTemplate->mAwardId);
            return LynxErrno::TemplateNotExist;
        }
        List<AwardLine> awardList = awardTemplate->mAwardItemList;
        for (List<AwardLine>::Iter* iter = awardList.begin();
            iter != NULL; iter = awardList.next(iter))
        {
            switch(iter->mValue.mItemType)
            {
            case ITEM_TYPE:
                {
                    ItemTemplate* awardTemplate = ITEM_TABLE().get(iter->mValue.mItemId);
                    if(!awardTemplate)
                    {
                        LOG_WARN("Failed to get itemtemplate by id: %u", iter->mValue.mItemId);
                        break;
                    }
                    mPlayer->getItemManager().createItem(iter->mValue.mItemId, iter->mValue.mCount, 0);
                }
                break;
            case EQUIP_TYPE:
                {
                    EquipTemplate* equipTemplate = EQUIP_TABLE().get(iter->mValue.mItemId);
                    if(!equipTemplate)
                    {
                        LOG_WARN("Failed to get EuqipTemplate by id: %u", iter->mValue.mItemId);
                        break;
                    }
                    mPlayer->getEquipManager().createEquip(iter->mValue.mItemId, 0);
                }
                break;
            case HERO_TYPE:
                {
                    HeroTemplate* heroTemplate = HERO_TABLE().get(iter->mValue.mItemId);
                    if (!heroTemplate)
                    {
                        LOG_WARN("failed to get heroTemplate by id: %u", iter->mValue.mItemId);
                        break;
                    }
                    mPlayer->getHeroManager().createHero(iter->mValue.mItemId);
                }
                break;
            case PET_TYPE:
                {
                    PetTemplate* petTemplate = PET_TABLE().get(iter->mValue.mItemId);
                    if (!petTemplate)
                    {
                        LOG_WARN("Failed to get petTemplate by id: %u", iter->mValue.mItemId);
                        break;
                    }
                    mPlayer->getPetManager().createPet(iter->mValue.mItemId);
                }
                break;
            case HERO_DEBRIS_TYPE:
                {
                    HeroDebrisTemplate* heroDebrisTemplate = HERO_DEBRIS_TABLE().get(iter->mValue.mItemId);
                    if (!heroDebrisTemplate)
                    {
                        LOG_WARN("failed to get heroDebrisTemplate by id: %u", iter->mValue.mItemId);
                        break;
                    }
                    mPlayer->getHeroDebrisManager().createHeroDebris(iter->mValue.mItemId, iter->mValue.mCount, 0);
                }
                break;
            case PET_DEBRIS_TYPE:
                {
                    PetDebrisTemplate* petDebrisTemplate = PET_DEBRIS_TABLE().get(iter->mValue.mItemId);
                    if (!petDebrisTemplate)
                    {
                        LOG_WARN("Failed to get petdebrisTemplate by id: %u", iter->mValue.mItemId);
                        break;
                    }
                    mPlayer->getPetDebrisManager().createPetDebris(iter->mValue.mItemId, iter->mValue.mCount, 0);
                }
                break;
            default:
                LOG_WARN("invalid awardtemplate itemtype!");
            }
        }

    }

    else
    {
        return LynxErrno::InvalidParameter;
    }
#endif

	//神秘商店小号RMB或者存盘account操作，目前注释 sec
    //mPlayer->getItemManager().createItem(storeTemplate->mTemplateId, count * storeTemplate->mCount, 0);
    ////mPlayer->mPlayerData.mAccountData.mRmb -= (UInt32)(storeTemplate->mCurrentPrice * discount * count);
    //SyncPlayerRmbNotify notify;
    //notify.mRmb = mPlayer->mPlayerData.mAccountData.mRmb;
    //mPlayer->sendMsgWithAck(notify);

    //PersistUpdatePlayerAccountDataNotify dbNotify;
    //dbNotify.mAccountData = mPlayer->mPlayerData.mAccountData;
    //PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());

    Map<TemplateId, StoreItemData*>::Iter* mapIter = mStoreItemMap.find(storeTemplate->mTemplateId);
    if(!mapIter)
    {
        StoreItemData storeItemData;
        storeItemData.mTemplateId = storeTemplate->mTemplateId;
        storeItemData.mLimitType = storeTemplate->mLimitNumberType;
        storeItemData.mCount = count;
        List<StoreItemData>::Iter* listIter = mPlayerStoreData->mStoreItemList.insertTail(storeItemData);
        mStoreItemMap.insert(storeTemplate->mTemplateId, &(listIter->mValue));

        AddStoreItemNotify notify;
        notify.mStoreItemData = storeItemData;
        mPlayer->sendMsgWithAck(notify);

        PersistInsertStoreItemNotify dbNotify;
        dbNotify.mStoreItemData = storeItemData;
        dbNotify.mPlayerId = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
    }
    else
    {
        mapIter->mValue->mCount += count;

        UpdateStoreItemNotify notify;
        notify.mTemplateId = storeTemplate->mTemplateId;
        notify.mCount = mapIter->mValue->mCount;
        mPlayer->sendMsgWithAck(notify);

        PersistUpdateStoreItemNotify dbNotify;
        dbNotify.mPlayerId = mPlayer->getPlayerGuid();
        dbNotify.mStoreItemData = *mapIter->mValue;
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
    }

    return LynxErrno::None;
}

ErrorId
StoreManager::buySecretStoreItem(TemplateId& templateId)
{
    bool match = false;
    for (List<TemplateId>::Iter* it = mPlayerStoreData->mSecretStoreItemList.begin();
        it != NULL; it = mPlayerStoreData->mSecretStoreItemList.next(it))
    {   
        if (it->mValue == templateId)
        {
            match = true;
        }
    }
    if (!match)
    {
        return LynxErrno::InvalidParameter;
    }
    SecretStoreTemplate* secretStoreTemplate = SECRET_STORE_TABLE().get(templateId);
    if (!secretStoreTemplate)
    {
        LOG_WARN("Failed to get secretstoreTemplate by id:%u", templateId);
        return LynxErrno::TemplateNotExist;
    }
	
	//买东西消耗RMB操作，sec注释account

   /* if (mPlayer->getPlayerData().mAccountData.mRmb < secretStoreTemplate->mCostDiamond)
    {
        return LynxErrno::RmbNotEnough;
    }*/


	//soul sec
   /* if(mPlayer->getPlayerData().mBaseData.mSoul < secretStoreTemplate->mCostSoul)
    {
        return LynxErrno::SoulNotEnough;
    }*/
    switch(secretStoreTemplate->mType)
    {
    case ITEM_TYPE:
        {
            ItemTemplate* itemTemplate = ITEM_TABLE().get(secretStoreTemplate->mTemplateId);
            if (!itemTemplate)
            {
                LOG_WARN("Failed to get itemTemplate by id:%u", secretStoreTemplate->mTemplateId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getItemManager().createItem(secretStoreTemplate->mTemplateId, secretStoreTemplate->mCount, 0);
        }
        break;
    case EQUIP_TYPE:
        {
            EquipTemplate* equipTemplate = EQUIP_TABLE().get(secretStoreTemplate->mTemplateId);
            if (!equipTemplate)
            {
                LOG_WARN("Failed to get equipTemplate by id:%u", secretStoreTemplate->mTemplateId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getEquipManager().createEquip(secretStoreTemplate->mTemplateId, 0);
        }
        break;
    case HERO_TYPE:
        {
            HeroTemplate* heroTemplate = HERO_TABLE().get(secretStoreTemplate->mTemplateId);
            if (!heroTemplate)
            {
                LOG_WARN("Failed to get heroTemplate by id:%u", secretStoreTemplate->mTemplateId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getHeroManager().createHero(secretStoreTemplate->mTemplateId);
        }
        break;
    case PET_TYPE:
        {
            PetTemplate* petTemplate = PET_TABLE().get(secretStoreTemplate->mTemplateId);
            if (!petTemplate)
            {
                LOG_WARN("Failed to get PetTemplate by id:%u", secretStoreTemplate->mTemplateId);
                return LynxErrno::TemplateNotExist;
            }
          //  mPlayer->getPetManager().createPet(secretStoreTemplate->mTemplateId);
        }
        break;
    case HERO_DEBRIS_TYPE:
        {
            HeroDebrisTemplate* heroDebrisTemplate = HERO_DEBRIS_TABLE().get(secretStoreTemplate->mTemplateId);
            if (!heroDebrisTemplate)
            {
                LOG_WARN("Failed to get herodebrisTemplate by id:%u", secretStoreTemplate->mTemplateId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getHeroDebrisManager().createHeroDebris(secretStoreTemplate->mTemplateId, secretStoreTemplate->mCount, 0);
        }
        break;
    case PET_DEBRIS_TYPE:
        {
            PetDebrisTemplate* petDebrisTemplate = PET_DEBRIS_TABLE().get(secretStoreTemplate->mTemplateId);
            if (!petDebrisTemplate)
            {
                LOG_WARN("Failed to get petDebrisTemplate by id:%u", secretStoreTemplate->mTemplateId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getPetDebrisManager().createPetDebris(secretStoreTemplate->mTemplateId, secretStoreTemplate->mCount, 0);
        }
        break;
    case RUNE_TYPE:
        {
            RuneTemplate* runeTemplate = RUNE_TABLE().get(secretStoreTemplate->mTemplateId);
            if (!runeTemplate)
            {
                LOG_WARN("Failed to get runeTemplate by id: %u", secretStoreTemplate->mTemplateId);
                return LynxErrno::TemplateNotExist;
            }
            mPlayer->getRuneManager().createRune(secretStoreTemplate->mTemplateId, 0);
        }
        break;
    default:
        LOG_WARN("invalid item type:%u", secretStoreTemplate->mType);
        return LynxErrno::InvalidParameter;
    }

	//RMB account注释，sec
   /* mPlayer->mPlayerData.mAccountData.mRmb -= secretStoreTemplate->mCostDiamond;*/
    if (secretStoreTemplate->mCostSoul)
    {
        mPlayer->reducePlayerSoul(secretStoreTemplate->mCostSoul);
    }


	//钻石相关account sec
  /*  if (secretStoreTemplate->mCostDiamond)
    {
        SyncPlayerRmbNotify notify;
        notify.mRmb = mPlayer->mPlayerData.mAccountData.mRmb;
        mPlayer->sendMsgWithAck(notify);

        PersistUpdatePlayerAccountDataNotify dbNotify;
        dbNotify.mAccountData = mPlayer->mPlayerData.mAccountData;
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
    }*/


    return LynxErrno::None;
}

ErrorId
StoreManager::resetSecretStoreItemByUseItem()
{

	//原游戏刷新神秘商店 sec注释
 //   // 无剩余次数(次数条件待修改)
 //   if (mPlayerStoreData->mResetSecretStoreCnt >= (5 + mPlayer->mPlayerData.mBaseData.m_nVipLevel * 2))
 //   {
 //       return LynxErrno::InvalidParameter;
 //   }
 //   bool resetValid = false;
 //   const ItemData* itemData = mPlayer->getItemManager().getItemByTemplateId(100001);
 //   if (itemData)
 //   {
 //       resetValid = true;
 //   }

	////account rmb相关操作注释sec
 //   if (mPlayer->mPlayerData.mAccountData.mRmb >= 20)
 //   {
 //       resetValid = true;
 //   }

 //   if (!resetValid)
 //   {
 //       return LynxErrno::InvalidParameter;
 //   }
 //   if(itemData)
 //   {
 //       mPlayer->getItemManager().delItem(itemData->mItemId, 1);
 //   }
 //   else
 //   {
 //       mPlayer->reducePlayerRmb(20);
 //   }
 //   
 //   mPlayerStoreData->mResetSecretStoreCnt ++;
 //   resetSecretStore();
 //   
 //   UpdatePlayerSecretStoreDataNotify notify;
 //   notify.mResetCnt = mPlayerStoreData->mResetSecretStoreCnt;
 //   notify.mItemList = mPlayerStoreData->mSecretStoreItemList;
 //   mPlayer->sendMsgWithAck(notify);
    return LynxErrno::None;
}

void 
StoreManager::resetDaily()
{
    for (List<StoreItemData>::Iter* it = mPlayerStoreData->mStoreItemList.begin();
        it != NULL;)
    {
        if(it->mValue.mLimitType != STORE_ITEM_LIMIT_NUM_CAREER)
        {
            mStoreItemMap.erase(it->mValue.mTemplateId);
            it = mPlayerStoreData->mStoreItemList.erase(it);
            
        }
        else
        {
            it = mPlayerStoreData->mStoreItemList.next(it);
        }
    }
}

void 
StoreManager::resetSecretStore()
{
    mPlayerStoreData->mSecretStoreItemList.clear();
    List<SecretStoreTemplate> list1;
    List<SecretStoreTemplate> list2;
    List<SecretStoreTemplate> list3;
    List<SecretStoreTemplate> resultList;
    UInt32 totalProperty1 = 0;
    UInt32 totalProperty2 = 0;
    UInt32 totalProperty3 = 0;
    for (SecretStoreTemplateMap::Iter* it = SECRET_STORE_TABLE().mMap.begin();
        it != NULL; it = SECRET_STORE_TABLE().mMap.next(it))
    {
        if (it->mValue.mBagId == 1)
        {
            list1.insertTail(it->mValue);
            totalProperty1 += it->mValue.mProperty;
        }
        else if (it->mValue.mBagId == 2)
        {
            list2.insertTail(it->mValue);
            totalProperty2 += it->mValue.mProperty;
        }
        else if (it->mValue.mBagId == 3)
        {
            list3.insertTail(it->mValue);
            totalProperty3 += it->mValue.mProperty;
        }
    }
    // 第一个包中抽取2个
    for (UInt32 i = 0; i < 2; ++i)
    {
        UInt32 tempValue = Random::getSingleton().getUInt32Random(0, totalProperty1);
        UInt32 curTotalProperty = 0;
        for (List<SecretStoreTemplate>::Iter* it = list1.begin();
            it != NULL; it = list1.next(it))
        {
            if (tempValue >= curTotalProperty && tempValue <= curTotalProperty + it->mValue.mProperty)
            {
                resultList.insertTail(it->mValue);
                totalProperty1 -= it->mValue.mProperty;
                list1.erase(it);
                break;
            }
            curTotalProperty += it->mValue.mProperty;
        }
    }
    // 第二个包中抽取2个活3个
    UInt32 bagTwoCnt = Random::getSingleton().getUInt32Random(2, 3);
    for (UInt32 i = 0; i < bagTwoCnt; ++i)
    {
        UInt32 tempValue = Random::getSingleton().getUInt32Random(0, totalProperty2);
        UInt32 curTotalProperty = 0;
        for (List<SecretStoreTemplate>::Iter* it = list2.begin();
            it != NULL; it = list2.next(it))
        {
            if (tempValue >= curTotalProperty && tempValue <= curTotalProperty + it->mValue.mProperty)
            {
                resultList.insertTail(it->mValue);
                totalProperty2 -= it->mValue.mProperty;
                list2.erase(it);
                break;
            }
            curTotalProperty += it->mValue.mProperty;
        }
    }
    // 剩下从第三个包中抽取
    for (UInt32 i = 0; i < (SECRET_STORE_ITEM_MAX_COUNT - 2 - bagTwoCnt); ++i)
    {
        UInt32 tempValue = Random::getSingleton().getUInt32Random(0, totalProperty3);
        UInt32 curTotalProperty = 0;
        for (List<SecretStoreTemplate>::Iter* it = list3.begin();
            it != NULL; it = list3.next(it))
        {
            if (tempValue >= curTotalProperty && tempValue <= curTotalProperty + it->mValue.mProperty)
            {
                resultList.insertTail(it->mValue);
                totalProperty3 -= it->mValue.mProperty;
                list3.erase(it);
                break;
            }
            curTotalProperty += it->mValue.mProperty;
        }
    }
    // 随机排序
    do 
    {
        UInt32 index = Random::getSingleton().getUInt32Random(0, resultList.size() - 1);
        List<SecretStoreTemplate>::Iter* it = resultList.getn(index);
        if (!it)
        {
            mPlayerStoreData->mSecretStoreItemList.clear();
            LOG_WARN("create secret item list error, out of index");
            return;
        }
        mPlayerStoreData->mSecretStoreItemList.insertTail(it->mValue.mId);
        resultList.erase(it);
    } while (resultList.size());   

    mPlayerStoreData->mSecretStoreResetTime = TimeUtil::getTimeSec();
    PersistUpdateSecretStoreItemNotify dbNotify;
    dbNotify.mSecretStoreItemResetTime = mPlayerStoreData->mSecretStoreResetTime;
    dbNotify.mPlayerId = mPlayer->getPlayerGuid();
    dbNotify.mItemList = mPlayerStoreData->mSecretStoreItemList;
    dbNotify.mResetCnt = mPlayerStoreData->mResetSecretStoreCnt;
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
}