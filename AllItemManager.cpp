#include "AllItemManager.h"
#include "Player.h"
#include "FireConfirm/Gift.h"
#include "FireConfirm/BaseChang.h"
#include "FireConfirm/CourageChallenge.h"
#include "LogicSystem.h"
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

void  AllItemManager::addAwardMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count, ReturnItemEle& rtItemEle,UInt32 systemID)
{
	char dest[1024]={0};
	UInt32 oldCount = 0;
	UInt32 nowCount = 0;

	switch(resType)
	{
	case AWARD_GEM:
		{
			oldCount = m_pPlayer->getGemManager().getGems(subTypeID);

			m_pPlayer->getGemManager().playerGetGems(subTypeID,count);
			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

	
			nowCount =  m_pPlayer->getGemManager().getGems(subTypeID);
			snprintf(dest,sizeof(dest),"%d,%d,%d,%d",subTypeID,oldCount,count,nowCount);
			LogicSystem::getSingleton().write_log(LogType68,m_pPlayer->getPlayerGuid(), dest,LogInfo);

			
		}
		break;
	case AWARD_JEWELRY:
		{
			if(m_pPlayer->getJewelryData().mJewelryList.size() <= MAXJEWELRYCOUNT)
			{
				List<JewelryData *> jewelryDatas = m_pPlayer->getJewelryManager().playerGetJewelry(subTypeID,count);

// 				m_pPlayer->getJewelryManager().diaoluoSend(jewelryDatas);
				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;
				rtItemEle.jewelryData = jewelryDatas.begin()->mValue;

				snprintf(dest,sizeof(dest),"%d,%d",subTypeID,count);
				LogicSystem::getSingleton().write_log(JewelryChange,m_pPlayer->getPlayerGuid(), dest,LogInfo);

			}
			else
			{				
				NewJewelryTemplate * newJewelryTemp = NEWJEWELRY_TABLE().get(subTypeID);
				if(newJewelryTemp == NULL)
				{
					return;
				}
				rtItemEle.resType = AWARD_BASE;
				rtItemEle.subType = AWARD_BASE_COIN;
				rtItemEle.addCount1 = newJewelryTemp->mPrice;
				rtItemEle.addCount2 = 0;

				PlayerBaseData baseData = m_pPlayer->getPlayerBaseData();
				oldCount =baseData.m_nCoin ;
				baseData.m_nCoin +=  newJewelryTemp->mPrice;
				m_pPlayer->setPlayerBaseData(baseData);

				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount, newJewelryTemp->mPrice,baseData.m_nCoin);
				LogicSystem::getSingleton().write_log(CoinChange,m_pPlayer->getPlayerGuid(), dest,LogInfo);

			}
			
		}
		break;
	case AWARD_BASE:
		{
			if(subTypeID == 1)
			{
				oldCount = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
				//铜钱
				m_pPlayer->mPlayerData.mBaseData.m_nCoin += count;

				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;
				
				nowCount = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(CoinChange,m_pPlayer->getPlayerGuid(), dest,LogInfo);
					
			}

			else if(subTypeID == 2)
			{
				oldCount = m_pPlayer->mPlayerData.mBaseData.m_nGold;
				//元宝
				m_pPlayer->mPlayerData.mBaseData.m_nGold += count;

				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;


				nowCount = m_pPlayer->mPlayerData.mBaseData.m_nGold;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(GoldChange,m_pPlayer->getPlayerGuid(), dest,LogInfo);
			}

			else if(subTypeID == 3)
			{
				//体力
				CGStrength strengthmsg;
				strengthmsg.reqType = 8;
				strengthmsg.num = count;
				strengthmsg.needSend = false;
				FireConfirmManager::onStrengthReq(m_pPlayer->getConnId(),strengthmsg);				
				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;
			}

			else if(subTypeID == 4)
			{

				//经验
				oldCount = m_pPlayer->mPlayerData.mBaseData.m_nLevelExp;
				char dest1[1024]={0};
				UInt32 oldLevel = m_pPlayer->getPlayerLeval();
				UInt32 strength = 0;
				UInt32 upToLevel = m_pPlayer->getExpAdd(count,strength);

				if (oldLevel != upToLevel)
				{
					snprintf(dest1,sizeof(dest1),"%d,%d",oldLevel,upToLevel);
					LogicSystem::getSingleton().write_log(LogType99,m_pPlayer->getPlayerGuid(), dest1,LogInfo);
				}
				
				
				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = upToLevel;

				if (strength > 0)
				{
					CGStrength strengthmsg;
					strengthmsg.reqType = 8;
					strengthmsg.num = strength;
					strengthmsg.needSend = true;
					FireConfirmManager::onStrengthReq(m_pPlayer->getConnId(),strengthmsg);
				}

				nowCount = m_pPlayer->mPlayerData.mBaseData.m_nLevelExp;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(LogType69,m_pPlayer->getPlayerGuid(), dest,LogInfo);
			
			}
			else if(subTypeID == 6)
			{
				//韵魂
				oldCount = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;

				m_pPlayer->getRhymeManager().addRhymeSoul(count);

				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;

			
				nowCount = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(LogType70,m_pPlayer->getPlayerGuid(), dest,LogInfo);
			}
			else if(subTypeID == AWARD_BASE_RANKEDSCORE)
			{
				PlayerRankGameData rankGameData = m_pPlayer->getRankGameData();
				oldCount = rankGameData.m_Score;
				rankGameData.m_Score += count;
				m_pPlayer->setRankGameData(rankGameData);
				m_pPlayer->getPersistManager().setDirtyBit(RANKGAMEDATABIT);

				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;

				nowCount = rankGameData.m_Score;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(LogType71,m_pPlayer->getPlayerGuid(), dest,LogInfo);
			}
			else if(subTypeID == AWARD_BASE_COMPETITIVE_POINT)
			{
				PlayerRankGameData rankGameData = m_pPlayer->getRankGameData();
				oldCount = rankGameData.m_MaskNum;
				rankGameData.m_MaskNum += count;
				m_pPlayer->setRankGameData(rankGameData);
				m_pPlayer->getPersistManager().setDirtyBit(RANKGAMEDATABIT);


				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;
				nowCount = rankGameData.m_MaskNum;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(LogType72,m_pPlayer->getPlayerGuid(), dest,LogInfo);

			}
			else if(subTypeID == AWARD_BASE_VIP_EXP)
			{

				//vip经验
				oldCount = m_pPlayer->mPlayerData.mBaseData.m_nVipExp;
				char dest1[1024]={0};
				UInt32 oldLevel = m_pPlayer->getVipLevel();
				UInt32 strength = 0;
				UInt32 upToLevel = m_pPlayer->getVipExpAdd(count,strength);

				if (oldLevel != upToLevel)
				{
					snprintf(dest1,sizeof(dest1),"%d,%d",oldLevel,upToLevel);
					LogicSystem::getSingleton().write_log(LogType101,m_pPlayer->getPlayerGuid(), dest1,LogInfo);
				}


				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = upToLevel;

			

				nowCount = m_pPlayer->mPlayerData.mBaseData.m_nVipExp;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(LogType103,m_pPlayer->getPlayerGuid(), dest,LogInfo);

			}
			

			m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);//存档不能少
			
		}
		break;

	case AWARD_GRASS:
		{
			oldCount = m_pPlayer->getRhymeManager().getRhymeGrass(subTypeID);
			m_pPlayer->getRhymeManager().addRhymeGrass(subTypeID,count);
			nowCount = m_pPlayer->getRhymeManager().getRhymeGrass(subTypeID);

			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

			snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
			LogicSystem::getSingleton().write_log(LogType73,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}
		break;
	
	case AWARD_HOARSTONEPIECE:
		{		
			oldCount = m_pPlayer->getHoarStoneManager().getHoarStonePiece(subTypeID);
			m_pPlayer->getHoarStoneManager().addHoarStonePiece(subTypeID,count);
			nowCount = m_pPlayer->getHoarStoneManager().getHoarStonePiece(subTypeID);

			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

			snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
			LogicSystem::getSingleton().write_log(LogType74,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}
    
		break;

	case AWARD_ENHANCEMATERIAL:
		{
			oldCount = m_pPlayer->getMaterialManager().getMaterialCount(subTypeID);
			m_pPlayer->getMaterialManager().addMaterial(subTypeID,count);
			nowCount = m_pPlayer->getMaterialManager().getMaterialCount(subTypeID);

			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

			snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
			LogicSystem::getSingleton().write_log(LogType75,m_pPlayer->getPlayerGuid(), dest,LogInfo);

		}

		break;

	case AWARD_HOARSTONEEQUIP:
		{
			oldCount = m_pPlayer->getHoarStoneManager().getHoarStoneRune(subTypeID);
			m_pPlayer->getHoarStoneManager().addHoarStoneRune(subTypeID,count);
			nowCount = m_pPlayer->getHoarStoneManager().getHoarStoneRune(subTypeID);

			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

			snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
			LogicSystem::getSingleton().write_log(LogType76,m_pPlayer->getPlayerGuid(), dest,LogInfo);

		}

		break;

	case AWARD_STAGEDATA:
		{
			StageManager::getSingleton().addStageData(m_pPlayer->getPlayerGuid(),subTypeID,count);

			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

			snprintf(dest,sizeof(dest),"%d,%d",subTypeID,count);
			LogicSystem::getSingleton().write_log(LogType77,m_pPlayer->getPlayerGuid(), dest,LogInfo);

		}

		break;

	case AWARD_TWELVEPALACE_STAGEDATA:
		{
			StageManager::getSingleton().addStageData(m_pPlayer->getPlayerGuid(),subTypeID,count);

			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;
			snprintf(dest,sizeof(dest),"%d,%d",subTypeID,count);
			LogicSystem::getSingleton().write_log(LogType78,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}

		break;
	case AWARD_COUNTER:
		{
			GiftManager::getSingleton().updateCounter(m_pPlayer->getPlayerGuid(),subTypeID,count);

			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

			//没用
// 			snprintf(dest,sizeof(dest),"%d,%d",subTypeID,count);
// 			LogicSystem::getSingleton().write_log(LogType000m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}

		break;
	case AWARD_REWARDCOUNTER:
		{
			GiftManager::getSingleton().updateRewardCounter(m_pPlayer->getPlayerGuid(),subTypeID,count);

			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

			//没用
// 			snprintf(dest,sizeof(dest),"%d,%d",subTypeID,count);
// 			LogicSystem::getSingleton().write_log(LogType000,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}

		break;
	case AWARD_FOOD:
		{
			
			CGFoods foodsmsg;
			foodsmsg.reqType = 4;
			foodsmsg.num = count;
			foodsmsg.subType = subTypeID;
			FireConfirmManager::onFoodsReq(m_pPlayer->getConnId() ,foodsmsg);

			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

		//已经记录log
		}
		break;
	case AWARD_STRENGTHDATA:
		{
			m_pPlayer->getPersistManager().setDirtyBit(STRENGTHDATABIT );

			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

			//已经记录log

		

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
		
			rtItemEle.resType = resType;
			rtItemEle.subType = subTypeID;
			rtItemEle.addCount1 = count;
			rtItemEle.addCount2 = 0;

			snprintf(dest,sizeof(dest),"%d,%d",subTypeID,count);
			LogicSystem::getSingleton().write_log(LogType79,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}

		break;
		case AWARD_SERVANT:
			{
	
				if(count == 0)
				{
					return ;
				}

				assert(count == 1);

				bool servantExist = m_pPlayer->getServantManager().isServantExist(subTypeID);
				if(!servantExist)
				{
					//佣兵不存在，则加入佣兵
					rtItemEle.resType = resType;
					rtItemEle.subType = subTypeID;
					rtItemEle.addCount1 = 1;
					rtItemEle.addCount2 = 0;
					
					m_pPlayer->getServantManager().addServants(subTypeID, 1);

					snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,0,count,1);
					LogicSystem::getSingleton().write_log(ServantChange,m_pPlayer->getPlayerGuid(), dest,LogInfo);
				}
				else
				{

					ServantTemplate * servantTemp = SERVANT_TABLE().get(subTypeID);
					if(!servantTemp)
					{
						LOG_WARN("design table error, servant %llu is not exist",  subTypeID);
						assert(false);
				    }

					//佣兵存在，则改为加入碎片
					rtItemEle.resType = AWARD_SERVANTPIECE;
					rtItemEle.subType = subTypeID;
					rtItemEle.addCount1 = servantTemp->mPieceCount;
					rtItemEle.addCount2 = 1;
					ServantData * servantData = m_pPlayer->getServantManager().getServantById(subTypeID);
					if(servantData == NULL)
					{
						oldCount =0;
					}
					else
					{
						oldCount = servantData->pieceCount;
					}
					m_pPlayer->getServantManager().addServantPiece(subTypeID, servantTemp->mPieceCount);
					nowCount = m_pPlayer->getServantManager().getServantById(subTypeID)->pieceCount;

					snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
					LogicSystem::getSingleton().write_log(ServantPieceChange,m_pPlayer->getPlayerGuid(), dest,LogInfo);
				}

			
			

			}

		break;

		case AWARD_SERVANTPIECE:
			{			
				ServantData * servantData = m_pPlayer->getServantManager().getServantById(subTypeID);
				if(servantData == NULL)
				{
					oldCount =0;
				}
				else
				{
					oldCount = servantData->pieceCount;
				}
				m_pPlayer->getServantManager().addServantPiece(subTypeID,count);

				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;

				nowCount = m_pPlayer->getServantManager().getServantById(subTypeID)->pieceCount;

				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(ServantPieceChange,m_pPlayer->getPlayerGuid(), dest,LogInfo);
			
			}

			break;

		case AWARD_SERVANTTREASURE:
			{
				ServantTreasure * treasureFind = m_pPlayer->getServantManager().getTreasure(subTypeID);
				if (treasureFind == NULL)
				{
					oldCount = 0;
				}
				else
				{
					oldCount = treasureFind->count;
				}
				m_pPlayer->getServantManager().addTreasure(subTypeID,count);
				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;
				nowCount = m_pPlayer->getServantManager().getTreasure(subTypeID)->count;

				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",subTypeID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(LogType80,m_pPlayer->getPlayerGuid(), dest,LogInfo);
			}
			break;

		case AWARD_SERVANTSWITCH:
			{

				oldCount = m_pPlayer->getServantManager().getServantSwitch();
				m_pPlayer->getServantManager().addServantSwitch(count);
				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;

				nowCount = m_pPlayer->getServantManager().getServantSwitch();
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(LogType81,m_pPlayer->getPlayerGuid(), dest,LogInfo);

			}
			break;

		case AWARD_CATCOIN:
			{
				oldCount =   CourageChallengeManager::getSingleton().getCatCoinNum(m_pPlayer->getPlayerGuid(),subTypeID);
				Goods goods;
				goods.resourcestype = resType;
				goods.subtype = subTypeID;
				goods.num = count;
				PlayerCourageChallengeData courageChallengeData = m_pPlayer->getCourageChallengeData();
				CourageChallengeManager::getSingleton().setCatCoins(m_pPlayer->getPlayerGuid(),courageChallengeData,goods,true);

				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;
				nowCount =   CourageChallengeManager::getSingleton().getCatCoinNum(m_pPlayer->getPlayerGuid(),subTypeID);

				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
				LogicSystem::getSingleton().write_log(LogType82,m_pPlayer->getPlayerGuid(), dest,LogInfo);

			}
			break;
		case AWARD_CURRENCY:
			{
				
				PlayerCourageChallengeData courageChallengeData = m_pPlayer->getCourageChallengeData();
				oldCount = courageChallengeData.m_LightOfLife;
				courageChallengeData.m_LightOfLife += count;
				m_pPlayer->setCourageChallengeData(courageChallengeData);
				m_pPlayer->getPersistManager().setDirtyBit(COURAGECHALLENGEDATABIT);

				rtItemEle.resType = resType;
				rtItemEle.subType = subTypeID;
				rtItemEle.addCount1 = count;
				rtItemEle.addCount2 = 0;

				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,courageChallengeData.m_LightOfLife);
				LogicSystem::getSingleton().write_log(LogType85,m_pPlayer->getPlayerGuid(), dest,LogInfo);


			}
			break;

		
		case AWARD_FREETICKET:
			{
				PlayerCounterData counterData = m_pPlayer->GetPlayerCounterData();
				if (resType == ONCEFREELOTTERYTICKET)
				{
					oldCount = counterData.m_RewardLotteryOnceTicket;
					counterData.m_RewardLotteryOnceTicket+=count;
					m_pPlayer->setPlayerCounterData(counterData);
					m_pPlayer->getPersistManager().setDirtyBit(COUNTERDATABIT);
					nowCount = counterData.m_RewardLotteryOnceTicket;
					snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
					LogicSystem::getSingleton().write_log(LogType83,m_pPlayer->getPlayerGuid(), dest,LogInfo);

				}
				else if (resType == TENFREELOTTERYTICKET)
				{
					oldCount = counterData.m_RewardLotteryOnceTicket;
					counterData.m_RewardLotteryTenTicket+=count;
					m_pPlayer->setPlayerCounterData(counterData);
					m_pPlayer->getPersistManager().setDirtyBit(COUNTERDATABIT);

					nowCount = counterData.m_RewardLotteryTenTicket;
					snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,count,nowCount);
					LogicSystem::getSingleton().write_log(LogType84,m_pPlayer->getPlayerGuid(), dest,LogInfo);
				}

			
			}
			break;

			
	
	default:
		break;
	}

}

//此处没饰品和佣兵，如果有饰品和佣兵需要单独判断
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
			else if (subTypeID == AWARD_BASE_STRENGTH )
			{			
				return FireConfirmManager::getSingleton().refreshgetStrength(m_pPlayer->getConnId());					
			}
			else if (subTypeID == AWARD_BASE_EXP)
			{				
				return m_pPlayer->getPlayerExp();			
			}
			else if (subTypeID == AWARD_BASE_COMPETITIVE_POINT)
			{
				return  m_pPlayer->getRankGameData().m_MaskNum;
			
			}
			else if(subTypeID == AWARD_BASE_RHYME_SOUL)
			{
				//韵魂
				return m_pPlayer->getRhymeManager().getRhymeSoul();
			}

		
			else if (subTypeID == AWARD_BASE_RANKEDSCORE)
			{
				return  m_pPlayer->getRankGameData().m_Score;				
			}	

			else if (subTypeID == AWARD_BASE_SCORE)
			{		
				return  m_pPlayer->getPlayerScore();			
			}	
			else if (subTypeID == AWARD_BASE_VIP_EXP)
			{				
				return m_pPlayer->getPlayerVIPExp();			
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
	case AWARD_CATCOIN:
		{		
			return CourageChallengeManager::getSingleton().getCatCoinNum(m_pPlayer->getPlayerGuid(),subTypeID);
		}		
		break;
	case AWARD_FREETICKET:
		{
			if (resType == ONCEFREELOTTERYTICKET)
			{
				PlayerCounterData counterData = m_pPlayer->GetPlayerCounterData();
				return counterData.m_RewardLotteryOnceFreeCount;
			}
			else if (resType == TENFREELOTTERYTICKET)
			{
				PlayerCounterData counterData = m_pPlayer->GetPlayerCounterData();
				return counterData.m_RewardLotteryTenFreeCount;
			}

		}
		break;

	case AWARD_CURRENCY:
		{

			PlayerCourageChallengeData courageChallengeData = m_pPlayer->getCourageChallengeData();
			return courageChallengeData.m_LightOfLife;
		}
		break;

	case AWARD_FOOD:
		{
			PlayerFoodsData foodsData = m_pPlayer->getFoodsData();
			if (resType == FOODTYPE1)
			{				
				return foodsData.leftTimes;	
			}
			else if (resType == FOODTYPE2)
			{				
				return  foodsData.food3;			
			}
			else if (resType== FOODTYPE3)
			{				
				return foodsData.food2;		
			}
			else if (resType== FOODTYPE4)
			{				
				return foodsData.food1;		
			}
			return 0;
		
		}
		break;

	case AWARD_CONSORTCONTRIBUTE:
		{
			return m_pPlayer->mPlayerData.mConsortData.m_nCurContribute;
		}
		break;
	default:
		return 0;
		break;
	}
	return 0;
}


void AllItemManager::convertItemListToStr(const List<ReturnItemEle> & rtItemEleList, Json::Value & root)
{
	std::string awardStr;

	

	for(List<ReturnItemEle>::Iter * contantIter = rtItemEleList.begin();  contantIter != NULL;
		contantIter = rtItemEleList.next(contantIter) )
	{
		if(contantIter->mValue.resType == AWARD_SERVANT ||  contantIter->mValue.resType == AWARD_SERVANTPIECE)
		{
			ServantData * servantData = m_pPlayer->getServantManager().getServantById(contantIter->mValue.subType);
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["addCount1"] = contantIter->mValue.addCount1;
			itemRoot["addCount2"] = contantIter->mValue.addCount2;
			itemRoot["count"] = servantData->pieceCount;


			////////////////////////////////////////////////////////////////////////

			itemRoot["attr"]["servantUid"] = servantData->servantUid;
			itemRoot["attr"]["servantId"] = servantData->servantId;
			itemRoot["attr"]["pieceCount"] = servantData->pieceCount;
			itemRoot["attr"]["level"] = servantData->level;
			itemRoot["attr"]["lvexp"] = servantData->lvexp;
			itemRoot["attr"]["star"] = servantData->star;
			itemRoot["attr"]["floor"] = servantData->floor;

			for(List<UInt64>::Iter * treasureIter = servantData->equipTreasures.begin(); treasureIter != NULL;
				treasureIter = servantData->equipTreasures.next(treasureIter))
			{
				itemRoot["attr"]["equipTreasures"].append(treasureIter->mValue);
			}

			root["contant"].append(itemRoot);
		} 
		else if(contantIter->mValue.resType == AWARD_JEWELRY)
		{
			Json::Value itemRoot;
			itemRoot["resType"] = contantIter->mValue.resType;
			itemRoot["subType"] = contantIter->mValue.subType;
			itemRoot["addCount1"] = contantIter->mValue.addCount1;
			itemRoot["addCount2"] = contantIter->mValue.addCount2;
			

			/////////////////////////
			

			
				itemRoot["attr"]["uuid"] = contantIter->mValue.jewelryData->m_nJewelryUid;
				itemRoot["attr"]["typeId"] = contantIter->mValue.jewelryData->m_nJewelryId;



				for(List<JewelryAttr>::Iter * jewelryAttrIter = contantIter->mValue.jewelryData->m_randomAttrList.begin(); jewelryAttrIter != NULL; 
					jewelryAttrIter = contantIter->mValue.jewelryData->m_randomAttrList.next(jewelryAttrIter))
				{
					Json::Value attrRoot;
					attrRoot["randomID"] = jewelryAttrIter->mValue.mRandomID;
					attrRoot["key"] = jewelryAttrIter->mValue.mKey;
					attrRoot["value"] = jewelryAttrIter->mValue.mValue;

					itemRoot["attr"]["randomAttrList"].append(attrRoot);

				}


				itemRoot["attr"]["staticAttrKey"] = contantIter->mValue.jewelryData->m_staticAttrKey;
				itemRoot["attr"]["staticAttrValue"] = contantIter->mValue.jewelryData->m_staticAttrValue;



				itemRoot["attr"]["purplePoints"] = contantIter->mValue.jewelryData->m_purplePoints;

				itemRoot["attr"]["purpleTimes"] = contantIter->mValue.jewelryData->m_purpleTimes;

				itemRoot["attr"]["orangePoints"] =contantIter->mValue.jewelryData->m_orangePoints;

				itemRoot["attr"]["orangeTimes"] = contantIter->mValue.jewelryData->m_orangeTimes;
				itemRoot["count"] = 1;
				
			

			///////////////////////

			root["contant"].append(itemRoot);
		}
		else if(contantIter->mValue.resType == AWARD_STAGEDATA)
		{
			Json::Value stageRoot;
			StageManager::getSingleton().getStageData(m_pPlayer->getPlayerGuid(),contantIter->mValue.subType,stageRoot );
			root["stage"].append(stageRoot);
		}
		else if( contantIter->mValue.resType == AWARD_TWELVEPALACE_STAGEDATA)
		{
			Json::Value stageRoot;
			StageManager::getSingleton().getStageData(m_pPlayer->getPlayerGuid(),contantIter->mValue.subType,stageRoot );
			root["stageTwelvePalace"].append(stageRoot);
		}
		else
		{

			UInt32 count = m_pPlayer->getAllItemManager().getAwardCount(contantIter->mValue.resType, contantIter->mValue.subType);
			if(count != -1 && contantIter->mValue.resType != 0 )
			{
				Json::Value itemRoot;
				itemRoot["resType"] = contantIter->mValue.resType;
				itemRoot["subType"] = contantIter->mValue.subType;
				itemRoot["addCount1"] = contantIter->mValue.addCount1;
				itemRoot["addCount2"] = contantIter->mValue.addCount2;
				itemRoot["count"] = count;

				root["contant"].append(itemRoot);
			}



		}//else

	}//for

	

	
	return  ;
}

void AllItemManager::addAwardMaterialsList(const List<AwardData> & awardList,UInt32 systemID)
{
	//处理奖励增加物品逻辑
	for(List<AwardData>::Iter * awardDataIter = awardList.begin(); awardDataIter != NULL;
		awardDataIter = awardList.next(awardDataIter))
	{
		if(awardDataIter->mValue.resType == AWARD_JEWELRY)
		{
			for(UInt32 i = 0; i < awardDataIter->mValue.resCount; i ++)
			{
				ReturnItemEle rtItemEle;
				addAwardMaterial(awardDataIter->mValue.resType, awardDataIter->mValue.resID, 1,rtItemEle,systemID);
			}
		}
		else if(awardDataIter->mValue.resType == AWARD_SERVANT)
		{
			for(UInt32 i = 0; i < awardDataIter->mValue.resCount; i ++)
			{
				ReturnItemEle rtItemEle;
				addAwardMaterial(awardDataIter->mValue.resType, awardDataIter->mValue.resID, 1,rtItemEle,systemID);
			}
		}
		else
		{
			ReturnItemEle rtItemEle;
			addAwardMaterial(awardDataIter->mValue.resType, awardDataIter->mValue.resID, awardDataIter->mValue.resCount,rtItemEle,systemID);
		}
		
	}
}

//基本不会用到，请不要刻意使用
void AllItemManager::costMaterialsList(const List<AwardData> & awardList,UInt32 systemID)
{
	//处理奖励增加物品逻辑
	for(List<AwardData>::Iter * awardDataIter = awardList.begin(); awardDataIter != NULL;
		awardDataIter = awardList.next(awardDataIter))
	{
		costMaterial(awardDataIter->mValue.resType, awardDataIter->mValue.resID, awardDataIter->mValue.resCount,systemID);
	}
}

//基本不会用到，请不要刻意使用
bool AllItemManager::costMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count,UInt32 systemID)
{
	char dest[1024]={0};
	UInt32 oldCount = 0;
	UInt32 nowCount = 0;

	switch(resType)
	{
	case AWARD_BASE:
		{
			if(subTypeID == 1)
			{
				if( m_pPlayer->mPlayerData.mBaseData.m_nCoin < count)
				{
					return false;
				}

				oldCount = m_pPlayer->mPlayerData.mBaseData.m_nCoin;

				//铜钱
				m_pPlayer->mPlayerData.mBaseData.m_nCoin -= count;

				Goods goods;
				nowCount = m_pPlayer->mPlayerData.mBaseData.m_nCoin;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,0-count,nowCount);
				LogicSystem::getSingleton().write_log(CoinChange,m_pPlayer->getPlayerGuid(), dest,LogInfo);


			}

			else if(subTypeID == 2)
			{
				//元宝
				if( m_pPlayer->mPlayerData.mBaseData.m_nGold < count)
				{
					return false;
				}
				oldCount = m_pPlayer->mPlayerData.mBaseData.m_nGold;

				m_pPlayer->mPlayerData.mBaseData.m_nGold -= count;

				nowCount = m_pPlayer->mPlayerData.mBaseData.m_nGold;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,0-count,nowCount);
				LogicSystem::getSingleton().write_log(GoldChange,m_pPlayer->getPlayerGuid(), dest,LogInfo);
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

				//log已有
				return true;
			}

			else if(subTypeID == 4)
			{
				//经验
				oldCount = m_pPlayer->mPlayerData.mBaseData.m_nLevelExp;
				m_pPlayer->mPlayerData.mBaseData.m_nLevelExp -= count;

				nowCount = m_pPlayer->mPlayerData.mBaseData.m_nLevelExp;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,0-count,nowCount);
				LogicSystem::getSingleton().write_log(LogType69,m_pPlayer->getPlayerGuid(), dest,LogInfo);
			}
			else if(subTypeID == 6)
			{
				//韵魂
				oldCount = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
				m_pPlayer->getRhymeManager().delRhymeSoul(count);

				nowCount = m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,0-count,nowCount);
				LogicSystem::getSingleton().write_log(LogType70,m_pPlayer->getPlayerGuid(), dest,LogInfo);
			}			
			else if(subTypeID == AWARD_BASE_RANKEDSCORE)
			{
				PlayerRankGameData rankGameData = m_pPlayer->getRankGameData();
				oldCount = rankGameData.m_Score;
				rankGameData.m_Score -= count;
				m_pPlayer->setRankGameData(rankGameData);
				m_pPlayer->getPersistManager().setDirtyBit(RANKGAMEDATABIT);

				nowCount = rankGameData.m_Score;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,0-count,nowCount);
				LogicSystem::getSingleton().write_log(LogType71,m_pPlayer->getPlayerGuid(), dest,LogInfo);

			}
			else if(subTypeID == AWARD_BASE_COMPETITIVE_POINT)
			{
				PlayerRankGameData rankGameData = m_pPlayer->getRankGameData();
				oldCount = rankGameData.m_MaskNum;
				rankGameData.m_MaskNum -= count;
				m_pPlayer->setRankGameData(rankGameData);
				m_pPlayer->getPersistManager().setDirtyBit(RANKGAMEDATABIT);

				nowCount = rankGameData.m_MaskNum;
				snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,0-count,nowCount);
				LogicSystem::getSingleton().write_log(LogType72,m_pPlayer->getPlayerGuid(), dest,LogInfo);
			}
			
		}
		m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);
		break;

	case AWARD_GEM:
		{
			oldCount = m_pPlayer->getGemManager().getGems(subTypeID);
			m_pPlayer->getGemManager().playerConsumeGems(subTypeID,count);

			nowCount =  m_pPlayer->getGemManager().getGems(subTypeID);
			snprintf(dest,sizeof(dest),"%d,%d,%d,%d",subTypeID,oldCount,0-count,nowCount);
			LogicSystem::getSingleton().write_log(LogType68,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}
		break;
	case AWARD_JEWELRY:
		{
			m_pPlayer->getJewelryManager().playerCostJewelry(subTypeID);

			snprintf(dest,sizeof(dest),"%d,%d",subTypeID,0-count);
			LogicSystem::getSingleton().write_log(JewelryChange,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}
		break;
	case AWARD_STAGEDATA:
		{
			StageManager::getSingleton().addStageData(m_pPlayer->getPlayerGuid(),subTypeID,count);

			snprintf(dest,sizeof(dest),"%d,%d",subTypeID,0-count);
			LogicSystem::getSingleton().write_log(LogType77,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}

		break;
	case AWARD_COUNTER:
		{
// 			GiftManager::getSingleton().updateCounter(m_pPlayer->getPlayerGuid(),subTypeID,count);

		}

		break;
	case AWARD_REWARDCOUNTER:
		{
// 			GiftManager::getSingleton().updateRewardCounter(m_pPlayer->getPlayerGuid(),subTypeID,count);
		}

		break;
	case AWARD_FOOD:
		{
			GiftManager::getSingleton().updateFood(m_pPlayer->getPlayerGuid(),subTypeID,count);//美食屋存档
			//已有log
		}
	case AWARD_SERVANTSWITCH:
		{
			oldCount = m_pPlayer->getServantManager().getServantSwitch();
			m_pPlayer->getServantManager().costServantSwitch(count);		
			nowCount = m_pPlayer->getServantManager().getServantSwitch();
			snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,0-count,nowCount);
			LogicSystem::getSingleton().write_log(LogType81,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}

		break;

	case AWARD_CATCOIN:
		{
			Goods goods;
			goods.resourcestype = resType;
			goods.subtype = subTypeID;
			goods.num =0 - count;
			PlayerCourageChallengeData courageChallengeData = m_pPlayer->getCourageChallengeData();
			CourageChallengeManager::getSingleton().setCatCoins(m_pPlayer->getPlayerGuid(),courageChallengeData,goods,true);
		}

		break;

	case AWARD_CURRENCY:
		{
			PlayerCourageChallengeData courageChallengeData = m_pPlayer->getCourageChallengeData();
			oldCount = courageChallengeData.m_LightOfLife;
			courageChallengeData.m_LightOfLife -= count;
			m_pPlayer->setCourageChallengeData(courageChallengeData);
			m_pPlayer->getPersistManager().setDirtyBit(COURAGECHALLENGEDATABIT);		

			nowCount = courageChallengeData.m_LightOfLife;
			snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,0-count,nowCount);
			LogicSystem::getSingleton().write_log(LogType85,m_pPlayer->getPlayerGuid(), dest,LogInfo);
	
		}

		break;
	case AWARD_CONSORTCONTRIBUTE:
	{
		 oldCount = m_pPlayer->mPlayerData.mConsortData.m_nCurContribute;
		 if(m_pPlayer->mPlayerData.mConsortData.m_nCurContribute < count)
		 {
			return false;
		 }
		 else
		 {
			m_pPlayer->mPlayerData.mConsortData.m_nCurContribute -= count;
		 }

		 nowCount = m_pPlayer->mPlayerData.mConsortData.m_nCurContribute;
		 snprintf(dest,sizeof(dest),"%d,%d,%d,%d",systemID,oldCount,0-count,nowCount);
		 LogicSystem::getSingleton().write_log(LogType86,m_pPlayer->getPlayerGuid(), dest,LogInfo);

	}
	break;	

	default:
		{
		}

		break;
	}

	return true;
}


void AllItemManager::addAwardGetSendJson(const List<AwardData> & awardList,UInt32 systemID)
{
	UInt64 timebegin = TimeUtil::getTimeMicroSec();
	
	PlayerFireConfirmData mFireConfirmData;
	
	//处理奖励增加物品逻辑
	for(List<AwardData>::Iter * awardDataIter = awardList.begin(); awardDataIter != NULL;
		awardDataIter = awardList.next(awardDataIter))
	{
		

		if(awardDataIter->mValue.resType == AWARD_SERVANT ||  awardDataIter->mValue.resType == AWARD_JEWELRY)
		{
			for(UInt32 i = 0; i < awardDataIter->mValue.resCount; i ++)
			{
				ReturnItemEle rtItemEle;
				m_pPlayer->getAllItemManager().addAwardMaterial(awardDataIter->mValue.resType,awardDataIter->mValue.resID, 1, rtItemEle,systemID);
				

			}
		}
		else
		{
				ReturnItemEle rtItemEle;
			m_pPlayer->getAllItemManager().addAwardMaterial(awardDataIter->mValue.resType,awardDataIter->mValue.resID,
			awardDataIter->mValue.resCount, rtItemEle,systemID);
			
		}


	}


}