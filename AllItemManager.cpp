#include "AllItemManager.h"
#include "Player.h"
#include "FireConfirm/Gift.h"
#include "FireConfirm/BaseChang.h"
using namespace Lynx;


AllItemManager::AllItemManager()
{
	m_pPlayer = NULL;
	
}


AllItemManager:: ~AllItemManager()
{

}


bool AllItemManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	

	return true;
}

void AllItemManager::release()
{
	m_pPlayer = NULL;
	
}


std::string AllItemManager::convertAllMaterialDataToJson()
{
	
	return "";

}

void AllItemManager::addAwardMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count)
{
	switch(resType)
	{
	case AWARD_GEM:
		{
			m_pPlayer->getGemManager().playerGetGems(subTypeID,count);
		}
		break;
	case AWARD_JEWELRY:
		{
			List<JewelryData *> jewelryDatas = m_pPlayer->getJewelryManager().playerGetJewelry(subTypeID,count);
			
			m_pPlayer->getJewelryManager().diaoluoSend(jewelryDatas);
		}
		break;
	case AWARD_BASE:
		{
			if(subTypeID == 1)
			{
				//铜钱
				m_pPlayer->mPlayerData.mBaseData.m_nCoin += count;
					
			}

			else if(subTypeID == 2)
			{
				//元宝
				m_pPlayer->mPlayerData.mBaseData.m_nGold += count;
			}

			else if(subTypeID == 3)
			{
				//体力
// 				m_pPlayer->mPlayerData.mBaseData.m_nStrength += count;
				CGStrength strengthmsg;
				strengthmsg.reqType = 8;
				strengthmsg.num = count;
				strengthmsg.needSend = false;
				FireConfirmManager::onStrengthReq(m_pPlayer->getConnId(),strengthmsg);
			}

			else if(subTypeID == 4)
			{
				//经验
				m_pPlayer->mPlayerData.mFireConfirmData.m_SendList  += m_pPlayer->getExpAdd(count);
				

// 				checkExp(m_pPlayer->getConnId());
				
			}

			else if(subTypeID == 5)
			{
				//
			}

			else if(subTypeID == 6)
			{
				//韵魂
				m_pPlayer->getRhymeManager().addRhymeSoul(count);
			}

			m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);//存档不能少
			
		}
		break;

	case AWARD_GRASS:
		{
			m_pPlayer->getRhymeManager().addRhymeGrass(subTypeID,count);
		}
		break;
	
	case AWARD_HOARSTONEPIECE:
		{		
			m_pPlayer->getHoarStoneManager().addHoarStonePiece(subTypeID,count);
		}
    
		break;

	case AWARD_ENHANCEMATERIAL:
		{
			m_pPlayer->getMaterialManager().addMaterial(subTypeID,count);
		}

		break;

	case AWARD_HOARSTONEEQUIP:
		{
			m_pPlayer->getHoarStoneManager().addHoarStoneRune(subTypeID,count);
		}

		break;

	case AWARD_STAGEDATA:
		{
			StageManager::getSingleton().addStageData(m_pPlayer->getPlayerGuid(),subTypeID,count);
		}

		break;

	case AWARD_TWELVEPALACE_STAGEDATA:
		{
			StageManager::getSingleton().addStageData(m_pPlayer->getPlayerGuid(),subTypeID,count);
		}

		break;
	case AWARD_COUNTER:
		{
			GiftManager::getSingleton().updateCounter(m_pPlayer->getPlayerGuid(),subTypeID,count);
		}

		break;
	case AWARD_REWARDCOUNTER:
		{
			GiftManager::getSingleton().updateRewardCounter(m_pPlayer->getPlayerGuid(),subTypeID,count);
		}

		break;
	case AWARD_FOODDATA:
		{
			m_pPlayer->getPersistManager().setDirtyBit(FOODDATABIT );
		}
		break;
	case AWARD_STRENGTHDATA:
		{
			m_pPlayer->getPersistManager().setDirtyBit(STRENGTHDATABIT );	
		}
		break;
	case AWARD_SERVANTMATERIAL:
		{
			if(subTypeID == ServantFood1)
			{
				m_pPlayer->getServantManager().addServantFood1(count);
			}

			if(subTypeID == ServantFood2)
			{
				m_pPlayer->getServantManager().addServantFood2(count);
			}

			if(subTypeID == ServantFood3)
			{
				m_pPlayer->getServantManager().addServantFood3(count);
			}

			if(subTypeID == ServantFood4)
			{
				m_pPlayer->getServantManager().addServantFood4(count);
			}

			if(subTypeID == ServantFood5)
			{
				m_pPlayer->getServantManager().addServantFood5(count);
			}

			if(subTypeID == ServantBattleStone)
			{
				m_pPlayer->getServantManager().addServantBattleStone(count);
			}
		
		}

		break;
		case AWARD_SERVANT:
			{
				Goods  goods;
				bool ret = m_pPlayer->getServantManager().addServants(subTypeID,count,goods);			
			}

		break;

		case AWARD_SERVANTPIECE:
			{			
				m_pPlayer->getServantManager().addServantPiece(subTypeID,count);
			
			}

			break;

		case AWARD_SERVANTTREASURE:
			{
				m_pPlayer->getServantManager().addTreasure(subTypeID,count);

			}
			break;

		case AWARD_SERVANTSWITCH:
			{

				m_pPlayer->getServantManager().addServantSwitch(count);

			}
			break;
	
	default:
		break;
	}

}

UInt32 AllItemManager::getAwardCount(UInt32 resType, UInt32 subTypeID)
{
	switch(resType)
	{
	case AWARD_GEM:
		{
			return m_pPlayer->getGemManager().getGems(subTypeID);
		}
		break;
	
	case AWARD_BASE:
		{
			if(subTypeID == 1)
			{
				//铜钱		
				return m_pPlayer->mPlayerData.mBaseData.m_nCoin;
			}

			else if(subTypeID == 2)
			{
				//元宝
				return m_pPlayer->mPlayerData.mBaseData.m_nGold ;
			}

			else if(subTypeID == 6)
			{
				//韵魂
				return m_pPlayer->getRhymeManager().getRhymeSoul();
			}

			return 0;

		}
		break;

	case AWARD_GRASS:
		{
			return m_pPlayer->getRhymeManager().getRhymeGrass(subTypeID);
		}
		break;

	case AWARD_HOARSTONEPIECE:
		{		
			return m_pPlayer->getHoarStoneManager().getHoarStonePiece(subTypeID);
		}

		break;

	case AWARD_ENHANCEMATERIAL:
		{
			return m_pPlayer->getMaterialManager().getMaterial(subTypeID);
		}

		break;

	case AWARD_HOARSTONEEQUIP:
		{
			return m_pPlayer->getHoarStoneManager().getHoarStoneRune(subTypeID);
		}

		break;

	case AWARD_SERVANTMATERIAL:
		{
			return m_pPlayer->getServantManager().getServantFood(subTypeID);

		}

		break;
	

	case AWARD_SERVANTTREASURE:
		{
			return (m_pPlayer->getServantManager().getTreasure(subTypeID))->count;

		}
		break;

	case AWARD_SERVANTSWITCH:
		{

			return m_pPlayer->getServantManager().getServantSwitch();

		}
		break;

	default:
		return 0;
		break;
	}
}

void AllItemManager::addAwardMaterialsList(const List<AwardData> & awardList)
{
	//处理奖励增加物品逻辑
	for(List<AwardData>::Iter * awardDataIter = awardList.begin(); awardDataIter != NULL;
		awardDataIter = awardList.next(awardDataIter))
	{
		addAwardMaterial(awardDataIter->mValue.resType, awardDataIter->mValue.resID, awardDataIter->mValue.resCount);
	}
}

//基本不会用到，请不要刻意使用
void AllItemManager::costMaterialsList(const List<AwardData> & awardList)
{
	//处理奖励增加物品逻辑
	for(List<AwardData>::Iter * awardDataIter = awardList.begin(); awardDataIter != NULL;
		awardDataIter = awardList.next(awardDataIter))
	{
		costMaterial(awardDataIter->mValue.resType, awardDataIter->mValue.resID, awardDataIter->mValue.resCount);
	}
}

//基本不会用到，请不要刻意使用
void AllItemManager::costMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count)
{
	switch(resType)
	{
	case AWARD_BASE:
		{
			if(subTypeID == 1)
			{
				//铜钱
				m_pPlayer->mPlayerData.mBaseData.m_nCoin -= count;

			}

			else if(subTypeID == 2)
			{
				//元宝
				m_pPlayer->mPlayerData.mBaseData.m_nGold -= count;
			}

			else if(subTypeID == 3)
			{
				//体力
// 				m_pPlayer->mPlayerData.mBaseData.m_nStrength -= count;
				CGStrength strengthmsg;
				strengthmsg.reqType = 9;
				strengthmsg.num = count;
				strengthmsg.needSend = false;
				FireConfirmManager::onStrengthReq(m_pPlayer->getConnId(),strengthmsg);				
			}

			else if(subTypeID == 4)
			{
				//经验
				m_pPlayer->mPlayerData.mBaseData.m_nLevelExp -= count;
			}

			else if(subTypeID == 5)
			{
				//
			}

			else if(subTypeID == 6)
			{
				//韵魂
				m_pPlayer->getRhymeManager().delRhymeSoul(count);
			}
			
		}
		break;

	case AWARD_GEM:
		{
			m_pPlayer->getGemManager().playerConsumeGems(subTypeID,count);
		}
		break;
	case AWARD_JEWELRY:
		{
			m_pPlayer->getJewelryManager().playerCostJewelry(subTypeID);
		}
		break;
	case AWARD_STAGEDATA:
		{
			StageManager::getSingleton().addStageData(m_pPlayer->getPlayerGuid(),subTypeID,count);
		}

		break;
	case AWARD_COUNTER:
		{
			GiftManager::getSingleton().updateCounter(m_pPlayer->getPlayerGuid(),subTypeID,count);
		}

		break;
	case AWARD_REWARDCOUNTER:
		{
			GiftManager::getSingleton().updateRewardCounter(m_pPlayer->getPlayerGuid(),subTypeID,count);
		}

		break;
	case AWARD_FOODDATA:
		{
			GiftManager::getSingleton().updateFood(m_pPlayer->getPlayerGuid(),subTypeID,count);//美食屋存档
		}
	case AWARD_SERVANTSWITCH:
		{
			m_pPlayer->getServantManager().costServantSwitch(count);			
		}

		break;
	default:
		break;
	}
}



void AllItemManager::checkExp(const  ConnId& connId)
{

// 	UInt64 maxLevel = gPlayerExpTable->mMap.getMax()->mKey;	
// 	if (m_pPlayer->mPlayerData.mBaseData.m_nLevel > maxLevel)
// 	{
// 		m_pPlayer->mPlayerData.mBaseData.m_nLevel = maxLevel;
// 	}
// 	m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);

// 	GCPlayerDetailResp detailInfoResp;
// 	detailInfoResp.mPacketID = BOC_PLAYER_DETAIL_RESP;
// 	detailInfoResp.mRespJsonStr = m_pPlayer->getPlayerInfoManager().convertDetailInfoToJson();
// 	NetworkSystem::getSingleton().sendMsg(detailInfoResp, connId);

	//sendbasedate


}

void AllItemManager::levelUp(const  ConnId& connId,UInt32 level)
{
	m_pPlayer->mPlayerData.mBaseData.m_nLevel = level;
	//角色属性，技能改变


}


void AllItemManager::addAwardGetSendJson(const List<AwardData> & awardList)
{
	UInt64 timebegin = TimeUtil::getTimeMicroSec();
	UInt64 timebigbegin = time(0);
	cout << "......................................................begin!"<<timebegin<<endl;
	PlayerFireConfirmData mFireConfirmData;
	
	//处理奖励增加物品逻辑
	for(List<AwardData>::Iter * awardDataIter = awardList.begin(); awardDataIter != NULL;
		awardDataIter = awardList.next(awardDataIter))
	{
		if (awardDataIter->mValue.resType == AWARD_JEWELRY|| awardDataIter->mValue.resType == AWARD_STAGEDATA|| awardDataIter->mValue.resType == AWARD_SERVANT
			||awardDataIter->mValue.resType == AWARD_SERVANTTREASURE||awardDataIter->mValue.resType == AWARD_TWELVEPALACE_STAGEDATA)
		{
			mFireConfirmData = m_pPlayer->GetFireConfirmData();
			mFireConfirmData.m_AddSendGoods.resourcestype = awardDataIter->mValue.resType;
			mFireConfirmData.m_AddSendGoods.subtype = awardDataIter->mValue.resID;
			mFireConfirmData.m_AddSendGoods.num = awardDataIter->mValue.resCount;
			m_pPlayer->SetFireConfirmDataTest(mFireConfirmData);
		}
		addAwardMaterial(awardDataIter->mValue.resType, awardDataIter->mValue.resID, awardDataIter->mValue.resCount);
	}

	UInt64 timebegin2 = TimeUtil::getTimeMicroSec();
	UInt64 costTime = timebegin2 - timebegin;
	UInt64 timebigbegin2 = time(0);
	UInt64 bigcost = timebigbegin2 - timebigbegin;
	cout << ".................................______________________.............end!costtime =  "<<costTime<<"\nbig"<<bigcost <<endl;
// 	cout << "......................................................end!"<<costTime<<"big"<<bigcost <<endl;
}