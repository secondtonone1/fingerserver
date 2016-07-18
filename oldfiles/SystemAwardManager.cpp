#include "SystemAwardManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

SystemAwardManager::SystemAwardManager() : mPlayer(NULL), mCurTime(0), mOnlineDateFlag(0)
{

}


SystemAwardManager::~SystemAwardManager()
{

}

bool 
SystemAwardManager::initial(Player* player)
{
	// ×¢ÊÍ½±Àø×Ö¶Îsec   
	/*if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }

    mPlayer = player;
    
    if (TimeUtil::getTodayBaseSec() > mPlayer->getPlayerData().mAccountData.mLogoutTime)
    {
        mPlayer->mPlayerData.mSystemAwardData.mAccumulateLoginDay ++;
        if (TimeUtil::getTodayBaseSec() - mPlayer->getPlayerData().mAccountData.mLogoutTime < 3600 * 24)
        {
            mPlayer->mPlayerData.mSystemAwardData.mContinousLoginDay ++;
        }
        else
        {
            mPlayer->mPlayerData.mSystemAwardData.mContinousLoginDay = 1;
            mPlayer->mPlayerData.mSystemAwardData.mLoginAwardFlag = 0;
        }
        PersistUpdatePlayerSystemAwardNotify dbNotify;
        dbNotify.mSystemAwardData = mPlayer->mPlayerData.mSystemAwardData;
        dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
        PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
    }
    mOnlineDateFlag = TimeUtil::getDateNum();*/
    return true;
}

void
SystemAwardManager::release()
{
    mPlayer = NULL;
}

void 
SystemAwardManager::update(const UInt64& accTime)
{
    //½±ÀøÏµÍ³¶¨Ê±´æ´¢Êý¾ÝÏòDB sec
	/* UInt32 curTime = TimeUtil::getTimeSec();
    if (curTime > mCurTime)
    {
        mCurTime = curTime;
        if (TimeUtil::getDateNum() > mOnlineDateFlag)
        {
            mOnlineDateFlag = TimeUtil::getDateNum();
            mPlayer->mPlayerData.mSystemAwardData.mContinousLoginDay ++;
            mPlayer->mPlayerData.mSystemAwardData.mAccumulateLoginDay ++;
            mPlayer->setPlayerDataDirty();
            SyncPlayerSystemAwardDataNotify notify;
            notify.mAwardData = mPlayer->mPlayerData.mSystemAwardData;
            mPlayer->sendMsgWithAck(notify);
            UpdatePlayerSystemAwardStatusNotify statusNotify;
            mPlayer->sendMsgWithAck(statusNotify);

            PersistUpdatePlayerSystemAwardNotify dbNotify;
            dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
            dbNotify.mSystemAwardData = mPlayer->mPlayerData.mSystemAwardData;
            PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());
        }
    }*/
}


ErrorId
SystemAwardManager::getAccumulateLoginAward(UInt32 loginDay)
{
    //getPlayerData().mSystemAwardData sec
	
	/*if (mPlayer->getPlayerData().mSystemAwardData.mAccumulateLoginDay < loginDay ||
        mPlayer->getPlayerData().mSystemAwardData.mAccumulateLoginAwardFlag >= loginDay)
    {
        return LynxErrno::InvalidParameter;
    }
    UInt32 awardTemplateId = 0;
    for (SystemAwardTemplateMap::Iter* it = SYSTEM_AWARD_TABLE().mMap.begin();
        it != NULL; it = SYSTEM_AWARD_TABLE().mMap.next(it))
    {
        if (it->mValue.mType == SYSTEM_AWARD_TYPE_ACCUMULATE_LOGIN && it->mValue.mAccumulateLodinDay == loginDay)
        {
            awardTemplateId = it->mValue.mAwardId;
            break;
        }
    }
    if (!awardTemplateId)
    {
        return LynxErrno::TemplateNotExist;
    }
    AwardTemplate* awardTemplate = AWARD_TABLE().get(awardTemplateId);
    if (!awardTemplate)
    {
        LOG_WARN("Failed to get awardTempalte by id: %u", awardTemplateId);
        return LynxErrno::TemplateNotExist;
    }
    if (mPlayer->getItemManager().isItemFull(awardTemplate->mAwardItemList) ||
        mPlayer->getEquipManager().isEquipFull(awardTemplate->mAwardItemList) ||
        mPlayer->getRuneManager().isRuneFull(awardTemplate->mAwardItemList) ||
        mPlayer->getPetDebrisManager().isPetDebrisFull(awardTemplate->mAwardItemList) ||
        mPlayer->getHeroDebrisManager().isHeroDebrisFull(awardTemplate->mAwardItemList))
    {
        return LynxErrno::BagIsFull;
    }
    for (List<AwardLine>::Iter* it = awardTemplate->mAwardItemList.begin();
        it != NULL; it = awardTemplate->mAwardItemList.next(it))
    {
        switch (it->mValue.mItemType)
        {
        case ITEM_TYPE:
            {
                mPlayer->getItemManager().createItem(it->mValue.mItemId, it->mValue.mCount, ITEM_SOURCE_SYSTEM_AWARD);
                break;
            }
        case EQUIP_TYPE:
            {
                mPlayer->getEquipManager().createEquip(it->mValue.mItemId, 0);
                break;
            }
        case RUNE_TYPE:
            {
                mPlayer->getRuneManager().createRune(it->mValue.mItemId, 0);
                break;
            }
        case HERO_DEBRIS_TYPE:
            {
                mPlayer->getHeroDebrisManager().createHeroDebris(it->mValue.mItemId, it->mValue.mCount, 0);
                break;
            }
        case PET_DEBRIS_TYPE:
            {
                mPlayer->getPetDebrisManager().createPetDebris(it->mValue.mItemId, it->mValue.mCount, 0);
                break;
            }
        default:
            LOG_WARN("Invalid item type %u", it->mValue.mItemType);
        }
    }
    mPlayer->mPlayerData.mSystemAwardData.mAccumulateLoginAwardFlag = loginDay;
    SyncPlayerSystemAwardDataNotify syncNotify;
    syncNotify.mAwardData = mPlayer->mPlayerData.mSystemAwardData;
    mPlayer->sendMsgWithAck(syncNotify);

    PersistUpdatePlayerSystemAwardNotify dbNotify;
    dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    dbNotify.mSystemAwardData = mPlayer->mPlayerData.mSystemAwardData;
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());*/
    
    return LynxErrno::None;
}

ErrorId
SystemAwardManager::getContinousLoginAward(UInt32 loginDay)
{
    //playerdata µÄmSystemAwardData×¢ÊÍÁË sec

	/*if (mPlayer->mPlayerData.mSystemAwardData.mContinousLoginDay < loginDay ||
        mPlayer->mPlayerData.mSystemAwardData.mLoginAwardFlag >= loginDay)
    {
        return LynxErrno::InvalidParameter;
    }
    UInt32 awardTemplateId = 0;
    for (SystemAwardTemplateMap::Iter* it = SYSTEM_AWARD_TABLE().mMap.begin();
        it != NULL; it = SYSTEM_AWARD_TABLE().mMap.next(it))
    {
        if (it->mValue.mType == SYSTEM_AWARD_TYPE_CONTINOUS_LOGIN && it->mValue.mContinousLoginDay == loginDay)
        {
            awardTemplateId = it->mValue.mAwardId;
            break;
        }
    }
    if (!awardTemplateId)
    {
        return LynxErrno::TemplateNotExist;
    }
    AwardTemplate* awardTemplate = AWARD_TABLE().get(awardTemplateId);
    if (!awardTemplate)
    {
        LOG_WARN("Failed to get awardTempalte by id: %u", awardTemplateId);
        return LynxErrno::TemplateNotExist;
    }
    if (mPlayer->getItemManager().isItemFull(awardTemplate->mAwardItemList) ||
        mPlayer->getEquipManager().isEquipFull(awardTemplate->mAwardItemList) ||
        mPlayer->getRuneManager().isRuneFull(awardTemplate->mAwardItemList) ||
        mPlayer->getPetDebrisManager().isPetDebrisFull(awardTemplate->mAwardItemList) ||
        mPlayer->getHeroDebrisManager().isHeroDebrisFull(awardTemplate->mAwardItemList))
    {
        return LynxErrno::BagIsFull;
    }
    for (List<AwardLine>::Iter* it = awardTemplate->mAwardItemList.begin();
        it != NULL; it = awardTemplate->mAwardItemList.next(it))
    {
        switch (it->mValue.mItemType)
        {
        case ITEM_TYPE:
            {
                mPlayer->getItemManager().createItem(it->mValue.mItemId, it->mValue.mCount, ITEM_SOURCE_SYSTEM_AWARD);
                break;
            }
        case EQUIP_TYPE:
            {
                mPlayer->getEquipManager().createEquip(it->mValue.mItemId, 0);
                break;
            }
        case RUNE_TYPE:
            {
                mPlayer->getRuneManager().createRune(it->mValue.mItemId, 0);
                break;
            }
        case HERO_DEBRIS_TYPE:
            {
                mPlayer->getHeroDebrisManager().createHeroDebris(it->mValue.mItemId, it->mValue.mCount, 0);
                break;
            }
        case PET_DEBRIS_TYPE:
            {
                mPlayer->getPetDebrisManager().createPetDebris(it->mValue.mItemId, it->mValue.mCount, 0);
                break;
            }
        default:
            LOG_WARN("Invalid item type %u", it->mValue.mItemType);
        }
    }
    mPlayer->mPlayerData.mSystemAwardData.mLoginAwardFlag = loginDay >= CONTINOUS_LOGIN_AWARD_MAX_DATE_NUM ? CONTINOUS_LOGIN_AWARD_MAX_DATE_NUM - 1 : loginDay;
    SyncPlayerSystemAwardDataNotify syncNotify;
    syncNotify.mAwardData = mPlayer->mPlayerData.mSystemAwardData;
    mPlayer->sendMsgWithAck(syncNotify);

    PersistUpdatePlayerSystemAwardNotify dbNotify;
    dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    dbNotify.mSystemAwardData = mPlayer->mPlayerData.mSystemAwardData;
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());*/

    return LynxErrno::None;
}

ErrorId
SystemAwardManager::getLevelAward(UInt32 level)
{
    // mPlayerData.mSystemAwardData ×Ö¶Î×¢ÊÍ£¬ sec
	/*if (mPlayer->mPlayerData.mBaseData.mLevel < level ||
        mPlayer->mPlayerData.mSystemAwardData.mLevelAwardFlag >= level)
    {
        return LynxErrno::InvalidParameter;
    }
    UInt32 awardTemplateId = 0;
    for (SystemAwardTemplateMap::Iter* it = SYSTEM_AWARD_TABLE().mMap.begin();
        it != NULL; it = SYSTEM_AWARD_TABLE().mMap.next(it))
    {
        if (it->mValue.mType == SYSTEM_AWARD_TYPE_LEVEL && it->mValue.mNeedLevel == level)
        {
            awardTemplateId = it->mValue.mAwardId;
            break;
        }
    }
    if (!awardTemplateId)
    {
        return LynxErrno::TemplateNotExist;
    }
    AwardTemplate* awardTemplate = AWARD_TABLE().get(awardTemplateId);
    if (!awardTemplate)
    {
        LOG_WARN("Failed to get awardTempalte by id: %u", awardTemplateId);
        return LynxErrno::TemplateNotExist;
    }
    if (mPlayer->getItemManager().isItemFull(awardTemplate->mAwardItemList) ||
        mPlayer->getEquipManager().isEquipFull(awardTemplate->mAwardItemList) ||
        mPlayer->getRuneManager().isRuneFull(awardTemplate->mAwardItemList) ||
        mPlayer->getPetDebrisManager().isPetDebrisFull(awardTemplate->mAwardItemList) ||
        mPlayer->getHeroDebrisManager().isHeroDebrisFull(awardTemplate->mAwardItemList))
    {
        return LynxErrno::BagIsFull;
    }
    for (List<AwardLine>::Iter* it = awardTemplate->mAwardItemList.begin();
        it != NULL; it = awardTemplate->mAwardItemList.next(it))
    {
        switch (it->mValue.mItemType)
        {
        case ITEM_TYPE:
            {
                mPlayer->getItemManager().createItem(it->mValue.mItemId, it->mValue.mCount, ITEM_SOURCE_SYSTEM_AWARD);
                break;
            }
        case EQUIP_TYPE:
            {
                mPlayer->getEquipManager().createEquip(it->mValue.mItemId, 0);
                break;
            }
        case RUNE_TYPE:
            {
                mPlayer->getRuneManager().createRune(it->mValue.mItemId, 0);
                break;
            }
        case HERO_DEBRIS_TYPE:
            {
                mPlayer->getHeroDebrisManager().createHeroDebris(it->mValue.mItemId, it->mValue.mCount, 0);
                break;
            }
        case PET_DEBRIS_TYPE:
            {
                mPlayer->getPetDebrisManager().createPetDebris(it->mValue.mItemId, it->mValue.mCount, 0);
                break;
            }
        default:
            LOG_WARN("Invalid item type %u", it->mValue.mItemType);
        }
    }
    mPlayer->mPlayerData.mSystemAwardData.mLevelAwardFlag = level;
    SyncPlayerSystemAwardDataNotify syncNotify;
    syncNotify.mAwardData = mPlayer->mPlayerData.mSystemAwardData;
    mPlayer->sendMsgWithAck(syncNotify);

    PersistUpdatePlayerSystemAwardNotify dbNotify;
    dbNotify.mPlayerGuid = mPlayer->getPlayerGuid();
    dbNotify.mSystemAwardData = mPlayer->mPlayerData.mSystemAwardData;
    PersistSystem::getSingleton().postThreadMsg(dbNotify, mPlayer->getPlayerGuid());*/

    return LynxErrno::None;
}