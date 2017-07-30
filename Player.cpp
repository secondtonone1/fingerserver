#include "Player.h"
#include "GameServer.h"
#include "../FireConfirm/Box.h"

using namespace Lynx;
extern float  getVal(Vector<String> strVector,UInt32 level);

Player::Player() : mAckIndex(1), mConnectCloseTime(0),mLastPostState(0),mNeedDel(0)
{

}

Player::~Player()
{
    
}

bool 
Player::initial(const ConnId& connId, const PlayerData& playerData,
                const UInt64& loginToken)
{
	
    mConnId = connId;
    mLoginToken = loginToken;
    mPlayerData = playerData;

    mLastChatTime = TimeUtil::getTimeSec();

    if (!mItemManager.initial(this))
    {
        LOG_WARN("Failed to initial ItemManager");
        return false;
    }

	if(!mPlayerInfoManager.initial(this))
	{
		LOG_WARN("Failed to initial playerinfoManager");
		return false;
	}

	if(!mSkillManager.initial(this))
	{
		LOG_WARN("Failed to initial skillManager");
		return false;
	}

	if(!mBuffManager.initial(this))
	{
		LOG_WARN("Failed to initial buffManager");
		return false;
	}

	if(!mStageManager.initial(this))
	{
		LOG_WARN("Failed to initial stageManager");
		return false;
	}

	if(!mEquipManager.initial(this))
	{
		LOG_WARN("Failed to initial equipManager");
		return false;
	}

	if(!mFashionManager.initial(this))
	{
		LOG_WARN("Failed to initial fashionManager");
		return false;
	}
	
	if(!mFireConfirmManager.initial(this))
	{
		LOG_WARN("Failed to initial FireConfirmManager");
		return false;
	}


	if(!mPersistManager.initial(this))
	{
		LOG_WARN("Failed to initial PersistManager");
		return false;
	}

	if(!mRhymeManager.initial(this))
	{
		LOG_WARN("Failed to initial RhymeManager");
		return false;
	}
	
	if(!mHoarStoneManager.initial(this))
	{
		LOG_WARN("Failed to initial HoarStoneManager");
		return false;
	}

	if(!mHeroEquipManager.initial(this))
	{
		LOG_WARN("Failed to initial HeroEquipManager");
		return false;
	}

	if(!mMaterialManager.initial(this))
	{
		LOG_WARN("Failed to initial MaterialManager");
		return false;
	}


	if(!mGemManager.initial(this))
	{
		LOG_WARN("Failed to initial GemManager");
		return false;
	}

	if(!mJewelryManager.initial(this))
	{
		LOG_WARN("Failed to initial JewelryManager");
		return false;
	}

	if(!mAllItemManager.initial(this))
	{
		LOG_WARN("Failed to initial AllItemManager");
		return false;
	}

	if(!mServantManager.initial(this))
	{
		LOG_WARN("Failed to initial ServantManager");
		return false;
	}
	
	if(!mChatManager.initial(this))
	{
		LOG_WARN("Failed to initial ChatManager");
		return false;
	}

	if(!mDetailInfoManager.initial(this))
	{
		LOG_WARN("Failed to initial DetailInfoManager");
		return false;
	}

	if(!mGMManager.initial(this))
	{
		LOG_WARN("Failed to initial mGMManager");
		return false;
	}

	if(!mRankingPersonalManager.initial(this))
	{
		LOG_WARN("Failed to initial mRankingPersonalManager");
		return false;
	}

	if(!mFriendBlackManager.initial(this))
	{
		LOG_WARN("Failed to initial mFriendBlackManager");
		return false;
	}

	if(!mEmailManager.initial(this))
	{
		LOG_WARN("Failed to initial mEmailManager");
		return false;
	}

	if(!mCharactorManager.initial(this))
	{
		LOG_WARN("Failed to initial mCharactorManager");
		return false;
	}

	if(!mAchieveManager.initial(this))
	{
		LOG_WARN("Failed to initial mAchieveManager");
		return false;

	}

	if(!mConsortManager.initial(this))
	{
		LOG_WARN("Failed to initial mConsortManager");
		return false;
	}

	//calNakedAttr();
	updatePlayerInfo();
	
	return true;
}

//检测界石开启条件，1位needcoin，可以查询对应的宏
bool Player::conditionRes(UInt32 conditionType, UInt64 conditionValue1, UInt32 conditionValue2)
{
	switch(conditionType)
	{
		case NeedCoin :
			{
				if(getPlayerCoin() < conditionValue1)
				{
					return false;
				}
				else
				{
					return true;
				}
			}

			break;
		case PlayerLv:
			{
				if(getPlayerLeval() < conditionValue1)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			break;
		case HoarStoneLv:
			{
				HoarStoneData * hoarStoneData = mHoarStoneManager.getSingleton().getHoarStoneData(conditionValue1);
				if(hoarStoneData)
				{
					if(hoarStoneData->mStoneLv < conditionValue2)
					{
						return false;
					}
					else
					{
						return true;
					}
				}
				else
				{
					LOG_WARN("Invalid hoarstone Id %llu", conditionValue1);
					return false;
				}
				
			}
		    break;
		default:
			break;
	}

	return false;
}

void 
Player::release()
{
    
    mConnId = 0;
	//player释放前先释放和他指针有关系的管理类 sec
    mItemManager.release();
	mPlayerInfoManager.release();
	mBuffManager.release();
	mSkillManager.release();
	mStageManager.release();
	mEquipManager.release();
	mFashionManager.release();
	mFireConfirmManager.release();

	mPersistManager.release();

	mRhymeManager.release();

	mHoarStoneManager.release();

	mHeroEquipManager.release();

	mMaterialManager.release();

	mGemManager.release();

	mJewelryManager.release();

	mAllItemManager.release();

	mServantManager.release();

	mChatManager.release();

	mDetailInfoManager.release();

	mGMManager.release();

	mRankingPersonalManager.release();

	mFriendBlackManager.release();

	mEmailManager.release();

	mCharactorManager.release();

	mAchieveManager.release();

	mConsortManager.release();

    for (Map<UInt32, StreamBuffer*>::Iter* iter = mWaitAckMsgMap.begin();
        iter != NULL; iter = mWaitAckMsgMap.next(iter))
    {
        StreamBuffer* streamBuffer = iter->mValue;
        if (streamBuffer)
        {
            XDELETE(streamBuffer);
        }
    }
    mWaitAckMsgMap.clear();
}

void 
Player::update(const UInt64& accTime)
{
   
	mPersistManager.update(accTime);
}


void 
Player::onMsgAckNotify(UInt32 ackId)
{
	Map<UInt32, StreamBuffer*>::Iter* iter = mWaitAckMsgMap.find(ackId);
	if (!iter)
	{
		LOG_WARN("Msg ack id %u don't match.", ackId);
		return;
	}

	StreamBuffer* streamBuffer = iter->mValue;
	mWaitAckMsgMap.erase(iter);
    XDELETE(streamBuffer);
}

UInt16 Player::getLastPostState()
{
	return mLastPostState;
}

void Player::setLastPostState(UInt16 postState)
{
	mLastPostState = postState;
}

void 
Player::onClientConnectClosed()
{
    mConnId = 0;
    mConnectCloseTime = TimeUtil::getTimeMilliSec();
	updatePlayerInfo();
	InlineActivityManager::getSingleton().checkLogOut(mPlayerData.mBaseData.m_nPlayerID,TimeUtil::getTimeSec());
	LogicSystem::getSingleton().logPlayerLogout(mPlayerData.mBaseData);

	//
	mConsortManager.clearWoodBattle();
}

void Player::updatePlayerInfo()
{	
	setPlayerLeaveTime(mConnectCloseTime);
	LogicSystem::getSingleton().updateBaseInfo(mPlayerData.mBaseData);
	LogicSystem::getSingleton().updateConsortInfo(mPlayerData.mBaseData.m_nPlayerID, mPlayerData.mConsortData);
	
	PersistUpdateLeaveTime updateLeaveTime;
	updateLeaveTime.m_nPlayerUid = mPlayerData.mBaseData.m_nPlayerID;
	updateLeaveTime.m_nLeaveTime = mConnectCloseTime;
	PersistSystem::getSingleton().postThreadMsg(updateLeaveTime, updateLeaveTime.m_nPlayerUid);
}

void 
Player::onClientConnectReconnected(const ConnId& connId, const UInt64& loginToken, bool isLogin)
{
    mConnId = connId;
    mLoginToken = loginToken;
    mConnectCloseTime = 0;
	mLastPostState = 0;
	mNeedDel = 0;

    if (isLogin)
    {
        for (Map<UInt32, StreamBuffer*>::Iter* iter = mWaitAckMsgMap.begin();
            iter != NULL; iter = mWaitAckMsgMap.next(iter))
        {
            StreamBuffer* streamBuffer = iter->mValue;
            if (streamBuffer)
            {
                XDELETE(streamBuffer);
            }
        }
        mWaitAckMsgMap.clear();
        mAckIndex = 1;
    }
    else
    {
        for (Map<UInt32, StreamBuffer*>::Iter* iter = mWaitAckMsgMap.begin();
            iter != NULL; iter = mWaitAckMsgMap.next(iter))
        {
            StreamBuffer* streamBuffer = iter->mValue;
            if (streamBuffer)
            {
                NetworkSystem::getSingleton().sendMsg(*streamBuffer, mConnId);
            }
        }
    }


	updatePlayerInfo();

}

void 
Player::updateLastChatTime()
{
    mLastChatTime = TimeUtil::getTimeSec();
}

void Player::ResetFireConfirmData() 
{	
	mPlayerData.mFireConfirmData.m_CopyID = 0;
	mPlayerData.mFireConfirmData.m_CopyStartTime = 0;
	mPlayerData.mFireConfirmData.m_ConfirmIDs.clear();
	mPlayerData.mFireConfirmData.m_RecConfirmIDs.clear();
	mPlayerData.mFireConfirmData.m_ReliveTimes = 0;
	mPlayerData.mFireConfirmData.m_Player2ReliveTimes = 0;
	mPlayerData.mFireConfirmData.m_trialOtherTimes = 0;
	mPlayerData.mFireConfirmData.m_AwardTimes = 0;
	mPlayerData.mFireConfirmData.m_Star = 0;
	mPlayerData.mFireConfirmData.m_Gain = 10000;
	
	mPlayerData.mFireConfirmData.m_AwardTypes.clear();	
	mPlayerData.mFireConfirmData.m_FixedList.clear();	
	mPlayerData.mFireConfirmData.m_AwardsList.clear();
	mPlayerData.mFireConfirmData.m_CostList.clear();
	mPlayerData.mFireConfirmData.m_CardsList.clear();
 	mPlayerData.mFireConfirmData.m_MonsterAwardList.clear();
	mPlayerData.mFireConfirmData.m_FirstAwardsList.clear();
	mPlayerData.mFireConfirmData.m_IntruderAwardList.clear();
// 	mPlayerData.mFireConfirmData.m_LastSendItemList.clear();//发送了才能清

	mPlayerData.mFireConfirmData.m_LastAwardsList.clear();
	mPlayerData.mFireConfirmData.m_SendList.clear();
	mPlayerData.mFireConfirmData.m_AddSendGoods.resourcestype = 0;	
	mPlayerData.mFireConfirmData.m_AddSendGoods.subtype = 0;	
	mPlayerData.mFireConfirmData.m_AddSendGoods.num = 0;	
	mPlayerData.mFireConfirmData.m_AddSendjs.clear();
	mPlayerData.mFireConfirmData.m_JewlryCanAddNum = MAXJEWELRYCOUNT - mPlayerData.mJewelryData.mJewelryList.size();

	mPlayerData.mFireConfirmData.m_IsMopUp = 0;

	mStageManager.resetgDocument();
// 	player->getStageManager().
	
}

//王戊辰 2016.1.19
UInt32  Player::getExpAdd(UInt32 count,UInt32 &addStrength)
{
	Goods goods;
// 	List<Goods> itemList;
	
	UInt64 maxLevel = gPlayerExpTable->mMap.getMax()->mKey;	
	mPlayerData.mBaseData.m_nLevelExp += count;

	//临时保存之前的等级
	UInt32 level = mPlayerData.mBaseData.m_nLevel;
	UInt32 upToLevel = mPlayerData.mBaseData.m_nLevel;
	PlayerExpTemplate * playerExpTemp = PLAYEREXP_TABLE().get(level);
	if (playerExpTemp == NULL)
	{
		return upToLevel;
	}

	//判断升级逻辑
	while(playerExpTemp && (mPlayerData.mBaseData.m_nLevelExp >= playerExpTemp->mExp) )
	{
		mPlayerData.mBaseData.m_nLevel++;

		if (mPlayerData.mBaseData.m_nLevel > maxLevel)
		{
			mPlayerData.mBaseData.m_nLevel = maxLevel;
			break;
		}

		levelUpcheck(mPlayerData.mBaseData.m_nPlayerID);

		upToLevel = mPlayerData.mBaseData.m_nLevel;

		mPlayerData.mBaseData.m_nLevelExp -= playerExpTemp->mExp;
		addStrength += playerExpTemp->mStrengthAdd;
	

		playerExpTemp = PLAYEREXP_TABLE().get(mPlayerData.mBaseData.m_nLevel);
		if (playerExpTemp == NULL)
		{
			LOG_WARN("playerExpTemp not found!!");
			return upToLevel;
		}
	}

	if(level < mPlayerData.mBaseData.m_nLevel)
	{
		checkLvActiveConditon();
		//更新成就信息
		mAchieveManager.updateAchieveData(PLAYERLV,mPlayerData.mBaseData.m_nLevel );
	}

	return upToLevel;
}



void Player::levelUpcheck(UInt64 playerID)
{
	FireConfirmManager::getSingleton().foodsInit(mPlayerData.mBaseData.m_nPlayerID);

	//第一次开启
	RankGameManager::getSingleton().initPlayerRankGame(mPlayerData.mBaseData.m_nPlayerID);

}
//计算玩家角色等级裸体属性
void Player::calNakedAttr()
{
	  PlayerAttrData playerAttrData(mPlayerData.mBaseData.m_nModelID,mPlayerData.mBaseData.m_nLevel);
	  m_playerAttrData = playerAttrData;
}

void Player::checkLvActiveConditon()
{
	mServantManager.checkActiveCondition();
	mJewelryManager.checkLvAcitve();
	mHeroEquipManager.checkLvUpActive();
	mSkillManager.activeSkill();
}

//新增新手引导配置材料
void Player::checkGuidInit()
{
	if(mPlayerData.mBaseData.m_nGuidGift)
	{
		return;
	}

	ReturnItemEle rtitem;
	//加薄荷草
	mAllItemManager.addAwardMaterial(AWARD_GRASS,1,50,rtitem,MiniLog136);
	//加铜钱
	mAllItemManager.addAwardMaterial(AWARD_BASE,1,50000,rtitem,MiniLog136);
	//加元宝
	//mAllItemManager.addAwardMaterial(AWARD_BASE,2,5000,rtitem,MiniLog136);
	//加佣兵
	mAllItemManager.addAwardMaterial(AWARD_SERVANT,1,1,rtitem,MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_SERVANT,5,1,rtitem,MiniLog136);
	//加玉器
	mAllItemManager.addAwardMaterial(AWARD_GEM,1001,2,rtitem,MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_GEM,2001,2,rtitem,MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_GEM,3001,2,rtitem,MiniLog136);
	//添加韵魂
	mAllItemManager.addAwardMaterial(AWARD_BASE,AWARD_BASE_RHYME_SOUL,100,rtitem,MiniLog136);
	//添加强化石
	mAllItemManager.addAwardMaterial(AWARD_ENHANCEMATERIAL,1,20,rtitem,MiniLog136);

	mAllItemManager.addAwardMaterial(AWARD_JEWELRY,2051,2,rtitem,MiniLog136);

	mAllItemManager.addAwardMaterial(AWARD_SERVANTTREASURE, 1005,1,rtitem, MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_SERVANTTREASURE, 1006,1,rtitem, MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_SERVANTTREASURE, 1007,1,rtitem, MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_SERVANTTREASURE, 1008,1,rtitem, MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_SERVANTMATERIAL, 1,1,rtitem, MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_SERVANTPIECE, 10,30,rtitem, MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_SERVANTTREASURE, 1003,1,rtitem, MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_SERVANTTREASURE, 1004,1,rtitem, MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_SERVANTTREASURE, 1005,1,rtitem, MiniLog136);
	mAllItemManager.addAwardMaterial(AWARD_SERVANTTREASURE, 1007,1,rtitem, MiniLog136);

	mPlayerData.mBaseData.m_nGuidGift = 1;
	//存盘
	PersistGuidFlagUpdateMsg guidflagupdate;
	guidflagupdate.playerUid = mPlayerData.mBaseData.m_nPlayerID;
	guidflagupdate.giftflag = 1;
	PersistSystem::getSingleton().postThreadMsg(guidflagupdate, guidflagupdate.playerUid);

}


const PlayerAttrData &Player::getLevelNakedAttr()
{
	//PlayerAttrData* playerdata = new PlayerAttrData(modelId, level);

	 
	
	
	return m_playerAttrData;
}

void Player::setDelDirty()
{
	mNeedDel = 1;
}

void Player::clearDelDirty()
{
	mNeedDel = 0;
}

bool Player::isToDel()
{
	if(mNeedDel)
	{
		return true;
	}

	return false;
}




UInt32  Player::getVipExpAdd(UInt32 count,UInt32 &change)
{
	Goods goods;
	// 	List<Goods> itemList;

	UInt64 maxLevel = VIP_TABLE().getMaxVipLevel();
	mPlayerData.mBaseData.m_nVipExp += count;

	//临时保存之前的等级
	UInt32 vipLevel = mPlayerData.mBaseData.m_nVipLevel;
	UInt32 upToVipLevel = mPlayerData.mBaseData.m_nVipLevel;
	VipTemplate * vipTemplate = VIP_TABLE().get(vipLevel);
	if (vipTemplate == NULL)
	{
		return upToVipLevel;
	}

	//判断升级逻辑
	while(vipTemplate && (mPlayerData.mBaseData.m_nVipExp >= vipTemplate->charge)&&vipTemplate->charge != 0 )
	{
		mPlayerData.mBaseData.m_nVipLevel++;

		if (mPlayerData.mBaseData.m_nVipLevel > maxLevel)
		{
			mPlayerData.mBaseData.m_nVipLevel = maxLevel;
			break;
		}

// 		levelUpcheck(mPlayerData.mBaseData.m_nPlayerID);

		upToVipLevel = mPlayerData.mBaseData.m_nVipLevel;

 		mPlayerData.mBaseData.m_nVipExp -= vipTemplate->charge;

// 		addStrength += vipTemplate->mStrengthAdd;


		vipTemplate = VIP_TABLE().get(mPlayerData.mBaseData.m_nVipLevel);
		if (vipTemplate == NULL)
		{
			return upToVipLevel;
		}
	}

// 	if(vipLevel < mPlayerData.mBaseData.m_nVipLevel)
// 	{
// 		checkLvActiveConditon();
// 		//更新成就信息
// 		mAchieveManager.updateAchieveData(PLAYERLV,mPlayerData.mBaseData.m_nLevel );
// 	}

	return upToVipLevel;
}