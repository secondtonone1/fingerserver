#include "GMManager.h"
#include "Player.h"
#include "LogicSystem.h"
#include "../FireConfirm/StageCalc.h"
#include "../FireConfirm/InlineActivity.h"
#include "../FireConfirm/GlobalValue.h"
using namespace Lynx;

#define  MAXLV  80
#define  MAXVIPLV 14

GMManager::GMManager()
{
	m_pPlayer = NULL;
}

GMManager:: ~GMManager()
{

}

bool GMManager::initial(Player* player)
{
	if (player == NULL)
	{
		LOG_WARN("Player pointer is NULL.");
		return false;
	}

	m_pPlayer = player;

	return true;
}

void GMManager::release()
{
	m_pPlayer = NULL;
}

void GMManager::dealGMRequest(UInt64 etype, UInt64 param1, UInt64 param2,  std::string param3, std::string param4)
{
		switch(etype)
		{
		case 1:
			      dealLvAdd(param1);
				  break;
		case 2:
			      dealVipAdd(param1);
			      break;
		case 3:
				dealCoinChange(param1);
			break;
		case 4:
			    dealGoldChange(param1);
				break;
		case 5:
                dealStrengthChange(param1);
			break;
		case 6:
			
				dealfoodsChange(param1);
			break;
		case 7:
			    dealRhymeSoulChange(param1);
			break;
		case 8:
			    dealRhymeGrassChange(param1,param2);
				break;
		case 9:
			    dealHoarStonePieceChange(param1, param2);
			break;
		case  10:
				dealWelfaleMarketChange(param1,param2);
			break;
		case  11:
				dealMultipleCopyChange(param1,param2);
			break;
		case  12:
				dealClimbTowerChange(param1,param2);
			break;
		case  13:
				dealStageChange(param1,param2);
			break;
		case 14:
				dealRhymeStep();
			break;
		case 15:
			dealRhymeLvUp(param1);
			break;
		case  16:
				dealTwelvePalaceChange(param1,param2);
			break;
		case  17:
				dealTwelvePalaceHaoJiaoChange();
				break;
		case 18:
			dealGemCountSet(param1, param2);
			break;
		case 19:
			dealHoarStoneRuneSet(param1, param2);
			break;
		case 21:
			GMsetServantPiece(param1, param2);
			break;
		case 22:
			GMsetServantEquip(param1, param2);
			break;
		case 23:
			GMsetServantFood(param1, param2);
			break;
		case  24:
// 			dealAllAdd(param1,param2,param3,);
			break;
		case 100:
			GMsetrechargeNum(param1, param2);
			break;
		case 505:
			GMEmailSend(param1, param2, param3, param4);
			break;
		case 507:
			GMMsgNotify(param3, param2, param1);
			break;
		
			
		default:
			break;
		}
}

void GMManager::dealLvAdd(UInt64 lv)
{
	
		UInt32 oldLevel = m_pPlayer->mPlayerData.mBaseData.m_nLevel;
		m_pPlayer->mPlayerData.mBaseData.m_nLevel += lv;
		if(m_pPlayer->mPlayerData.mBaseData.m_nLevel > MAXLV)
		{
			m_pPlayer->mPlayerData.mBaseData.m_nLevel = MAXLV;
		}


		char dest[1024]={0};
		if (oldLevel != m_pPlayer->mPlayerData.mBaseData.m_nLevel )
		{
			snprintf(dest,sizeof(dest),"%d,%d",oldLevel,m_pPlayer->mPlayerData.mBaseData.m_nLevel );
			LogicSystem::getSingleton().write_log(LogType100,m_pPlayer->getPlayerGuid(), dest,LogInfo);
		}

		m_pPlayer->mPlayerData.mBaseData.m_nLevelExp = 0;
		m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);
		m_pPlayer->checkLvActiveConditon();
		m_pPlayer->getAchieveManager().updateAchieveData(PLAYERLV,m_pPlayer->mPlayerData.mBaseData.m_nLevel );
		GCGMResp resp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["etype"] = 1;
		root["param1"] = lv;
		root["param2"] = 0;
		root["extra"] = m_pPlayer->mPlayerData.mBaseData.m_nLevel;
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
		resp.mPacketID = BOC_GM_RESP;
		const ConnId & connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp, connId );
		
}

void GMManager::dealVipAdd(UInt64 lv)
{
	UInt32 oldviplv = m_pPlayer->mPlayerData.mBaseData.m_nVipLevel;
	m_pPlayer->mPlayerData.mBaseData.m_nVipLevel += lv;
	
	if(m_pPlayer->mPlayerData.mBaseData.m_nVipLevel > MAXVIPLV)
	{
		m_pPlayer->mPlayerData.mBaseData.m_nVipLevel = MAXVIPLV;
	}

	char dest[1024]={0};
	if (oldviplv != m_pPlayer->mPlayerData.mBaseData.m_nVipLevel )
	{
		snprintf(dest,sizeof(dest),"%d,%d",oldviplv,m_pPlayer->mPlayerData.mBaseData.m_nVipLevel );
		LogicSystem::getSingleton().write_log(LogType102,m_pPlayer->getPlayerGuid(), dest,LogInfo);
	}


	m_pPlayer->mPlayerData.mBaseData.m_nVipExp = 0;
	m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);
	m_pPlayer->checkLvActiveConditon();

	//vip成就更新打点
	m_pPlayer ->getAchieveManager().updateAchieveData(VIPLV,m_pPlayer->mPlayerData.mBaseData.m_nVipLevel - oldviplv);

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 2;
	root["param1"] = lv;
	root["param2"] = 0;
	root["extra"] = m_pPlayer->mPlayerData.mBaseData.m_nVipLevel;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);

	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());

	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::dealCoinChange(UInt64 coin)
{
	
	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_COIN;
	goods.num = coin - m_pPlayer->mPlayerData.mBaseData.m_nCoin;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog94);

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 3;
	root["param1"] = coin;
	root["param2"] = 0;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::dealGoldChange(UInt64 gold)
{

	Goods goods;
	List<Goods> itemList;

	goods.resourcestype =AWARD_BASE;
	goods.subtype = AWARD_BASE_GOLD;
	goods.num = gold - m_pPlayer->mPlayerData.mBaseData.m_nGold;
	itemList.insertTail(goods);
	GiftManager::getSingleton().addToPlayer(m_pPlayer->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog3);

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 4;
	root["param1"] = gold;
	root["param2"] = 0;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );

}

void GMManager::dealStrengthChange(UInt64  strength)
{
		
	CGStrength strengthmsg;
	strengthmsg.needSend = false;

	if (strength == 0)
	{
		strengthmsg.reqType = 10;
	}
	else if(strength == 1)
	{
		strengthmsg.reqType = 11;
	}
	else
	{
		return;
	}

	FireConfirmManager::onStrengthReq(m_pPlayer->getConnId(),strengthmsg);
	PlayerStrengthData strengthData = m_pPlayer->getStrengthData();	

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 5;
	root["param1"] = strength;
	root["param2"] = 0;
	root["extra"] = strengthData.strength;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}
void GMManager::dealfoodsChange(UInt64  flag)
{

	
	CGFoods foodsmsg;
	
	if (flag == 0)
	{
		foodsmsg.reqType = 10;
	}
	else if(flag == 1)
	{
		foodsmsg.reqType = 11;
	}
	else
	{
		return;
	}

	FireConfirmManager::onFoodsReq(m_pPlayer->getConnId(),foodsmsg);
	
	PlayerFoodsData foodsData = m_pPlayer->getFoodsData();	

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 6;
	root["param1"] = flag;
	root["param2"] = 0;
	root["extra"] = foodsData.leftTimes;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::dealRhymeSoulChange(UInt64 count)
{
	m_pPlayer->mPlayerData.mRhymeData.m_RhymeSoul = count;
	m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT);

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 7;
	root["param1"] = count;
	root["param2"] = 0;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::dealRhymeGrassChange(UInt64 grassType, UInt64 count)
{
	
	m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT);

	switch(grassType)
	{
	case SmallGrass:
		{
			m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrass = count;
		}
		break;

	case MidGrass:
		{
			m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassMid = count;
		}
		break;

	case LargeGrass:
		{
			m_pPlayer->mPlayerData.mRhymeData.m_RhymeGrassHigh = count;
		}
		break;

	default:
		break;
	}

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 8;
	root["param1"] = grassType;
	root["param2"] = count;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}


void GMManager::dealHoarStonePieceChange(UInt64 hoarStoneId, UInt32 pieceCount)
{
	
	bool success = m_pPlayer->getHoarStoneManager().setHoarStonePiece(hoarStoneId, pieceCount);
	
	if(success)
	{
		GCGMResp resp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["etype"] = 9;
		root["param1"] = hoarStoneId;
		root["param2"] = pieceCount;
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
		resp.mPacketID = BOC_GM_RESP;
		const ConnId & connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp, connId );
	}
	else
	{
		GCGMResp resp;
		Json::Value root;
		root["errorId"] = LynxErrno::InvalidIdentify;
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
		resp.mPacketID = BOC_GM_RESP;
		const ConnId & connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp, connId );
	}
	

}

void GMManager::dealRhymeStep()
{
	UInt64* rhymeNextStep = RHYMELEVEL_TABLE().getIdByStepLv(m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep + 1, 0);

	if(!rhymeNextStep)
	{
		GCGMResp resp;
		Json::Value root;
		root["errorId"] = LynxErrno::RhymeStepLimit;
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
		resp.mPacketID = BOC_GM_RESP;
		const ConnId & connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp, connId );

		
		return ;
	}


	m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv = 0;
	m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep ++;
	
		m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointStr.clear();

		m_pPlayer->mPlayerData.mRhymeData.m_RhymeAcupointList.clear();

		m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT|BASEDATABIT);

		//发送信息给玩家

		GCGMResp resp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["param1"] = 0;
		root["param2"] = 0;
		root["etype"] = 14;
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
		resp.mPacketID = BOC_GM_RESP;
		const ConnId & connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp, connId );

		return;
	

}


void GMManager::dealRhymeLvUp(UInt32 addLv)
{
	UInt64 * rymePoint = RHYMELEVEL_TABLE().getIdByStepLv(m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep, m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv +addLv);

	if(!rymePoint)
	{
		RhymeLevelTemplate * rhymeStepMax = RHYMELEVEL_TABLE().getStepMax(m_pPlayer->mPlayerData.mRhymeData.m_RhymeStep);
		assert(rhymeStepMax);

		m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv =  rhymeStepMax->mLevel;
		m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp = 0;

		GCGMResp resp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["param1"] = addLv;
		root["param2"] = 0;
		root["etype"] = 15;
		root["extra"] = rhymeStepMax->mLevel;
		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
		resp.mPacketID = BOC_GM_RESP;
		const ConnId & connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp, connId );

		m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT|BASEDATABIT);

		return ;
	}

	UInt64 rhymeID = *rymePoint;

	RhymeLevelTemplate * rhymeLevelNext = RHYMELEVEL_TABLE().getNext(rhymeID);

	m_pPlayer->mPlayerData.mRhymeData.m_RhymeLv =  rhymeLevelNext->mLevel;
	m_pPlayer->mPlayerData.mRhymeData.m_RhymeExp = 0;

	m_pPlayer->getPersistManager().setDirtyBit(RHYMEDATABIT|BASEDATABIT);

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["param1"] = addLv;
	root["param2"] = 0;
	root["etype"] = 15;
	root["extra"] = rhymeLevelNext->mLevel;

	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );

	return;
	

}


void GMManager::dealGemCountSet(UInt64 gemId, UInt64 gemCount)
{
	GemData* gemData = m_pPlayer->getGemManager().getGemData(gemId);
	//存在该宝石
	if(gemData)
	{
		gemData->m_nCount = gemCount;
	}
	else
	{
		//不存在该宝石
		m_pPlayer->getGemManager().playerGetGems(gemId, gemCount);
	}
	
	m_pPlayer->getPersistManager().setDirtyBit(GEMDATABIT);

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["param1"] = gemId;
	root["param2"] = gemCount;
	root["etype"] = 18;

	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
	
}

void GMManager::dealHoarStoneRuneSet(UInt64 runeId, UInt64 runeCount)
{
		m_pPlayer->getHoarStoneManager().GMHoarStoneRuneSet(runeId, runeCount);

		GCGMResp resp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["param1"] = runeId;
		root["param2"] = runeCount;
		root["etype"] = 19;

		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
		resp.mPacketID = BOC_GM_RESP;
		const ConnId & connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp, connId );
}


void GMManager::GMsetServantPiece(UInt64 servantId, UInt64 servantCount)
{
	m_pPlayer->getServantManager().GMsetServantPiece(servantId, servantCount);

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["param1"] = servantId;
	root["param2"] = servantCount;
	root["etype"] = 21;

	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::GMsetServantEquip(UInt64 equipId, UInt64 equipCount)
{
		ServantTreasure * treasureFind = m_pPlayer->getServantManager().getTreasure(equipId);

		ServantTreasure * servantTreasure = m_pPlayer->getServantManager().GMsetTreasure(equipId, equipCount);

		GCGMResp resp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["param1"] = equipId;
		root["param2"] = equipCount;
		root["etype"] = 22;
	    if(!treasureFind)
		{
			root["extra"] = servantTreasure->treasureUid;
		}

		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
		resp.mPacketID = BOC_GM_RESP;
		const ConnId & connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::GMsetServantFood(UInt64 foodType, UInt64 foodCount)
{
		m_pPlayer->getServantManager().GMsetServantFood(foodType, foodCount);

		GCGMResp resp;
		Json::Value root;
		root["errorId"] = LynxErrno::None;
		root["param1"] = foodType;
		root["param2"] = foodCount;
		root["etype"] = 23;
	

		Json::StyledWriter writer;
		resp.mRespJsonStr = writer.write(root);
		LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
		resp.mPacketID = BOC_GM_RESP;
		const ConnId & connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::GMsetrechargeNum(UInt64 foodType, UInt64 foodCount)
{
	InlineActivityManager::getSingleton().incRechargeNum(m_pPlayer->getPlayerGuid());

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
// 	root["param1"] = foodType;
// 	root["param2"] = foodCount;
	root["etype"] = 100;


	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::dealTwelvePalaceChange(UInt64 type, UInt64 count)
{
	UInt32 stageID = 0;
	UInt32 chapterbase = 500;
	stageID = (chapterbase +type)*100 + count;

	UInt32 ret = m_pPlayer->getStageManager().resetStagechallegeTimes(stageID);
	GCGMResp resp;
	Json::Value root;
	root["errorId"] = ret;
	root["etype"] = 16;
	root["param1"] = type;
	root["param2"] = count;
	root["extra"] = 0;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}


void GMManager::dealTwelvePalaceHaoJiaoChange()
{
	StageCalcManager::getSingleton().resetHaoJiao(m_pPlayer->getPlayerGuid());

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 17;
	root["extra"] = globalValue.uTPresetlowerlimit;
	
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}


void GMManager::dealWelfaleMarketChange(UInt64 type, UInt64 count)
{

	UInt32 ret = m_pPlayer->getStageManager().resetWelfaleMarketchallegeTimes((UInt32)type);
	GCGMResp resp;
	Json::Value root;
	root["errorId"] = ret;
	root["etype"] = 10;
	root["param1"] = type;
	root["param2"] = count;
	root["extra"] = 0;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::dealMultipleCopyChange(UInt64 type, UInt64 count)
{

	UInt32 ret = m_pPlayer->getStageManager().resetMultipleCopychallegeTimes();
	GCGMResp resp;
	Json::Value root;
	root["errorId"] = ret;
	root["etype"] = 11;
	root["param1"] = type;
	root["param2"] = count;
	root["extra"] = 0;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::dealClimbTowerChange(UInt64 type, UInt64 count)
{
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	UInt32 ret = m_pPlayer->getStageManager().resetClimbTowerchallegeTimes();
	GCGMResp resp;
	Json::Value root;
	root["errorId"] = ret;
	root["etype"] = 12;
	root["param1"] = type;
	root["param2"] = count;
	root["extra"] = globalValue.uCLIMBTresetlowerlimit;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());

	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}
// 
// void GMManager::dealAllAdd(UInt64 resType, UInt64 subType, std::string count)
// {
// 	Json::Value allAttr;
// 	Goods goods;
// 	GCGMResp resp;
// 	List<Goods> itemList;
// 	List<ReturnItemEle> rtItemList;
// 
// 	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();	
// 	goods.resourcestype = resType;
// 	goods.subtype = subType;
// 	goods.num = lynxAtoi<Int32>(count.c_str());
// 	itemList.insertTail(goods);
// 
// 	GiftManager::getSingleton().addToPlayerAttr(m_pPlayer->getPlayerGuid(),rtItemList,allAttr, itemList,systemID);
// 	UInt32 ret = 0;
// 
// 	
// 	Json::Value root;
// 	root["errorId"] = ret;
// 	root["etype"] = 24;
// 	root["param1"] = resType;
// 	root["param2"] = subType;
// 	root["param3"] = count;
// 	root["extra"] = globalValue.uCLIMBTresetlowerlimit;
// 	root["allAttr"] = allAttr;
// 	Json::StyledWriter writer;
// 	resp.mRespJsonStr = writer.write(root);
// 	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
// 
// 	resp.mPacketID = BOC_GM_RESP;
// 	const ConnId & connId = m_pPlayer->getConnId();
// 	NetworkSystem::getSingleton().sendMsg(resp, connId );
// }


void GMManager::dealStageChange(UInt64 type, UInt64 count)
{
	UInt32 ret = m_pPlayer->getStageManager().resetStagechallegeTimes((UInt64)type);
	GCGMResp resp;
	Json::Value root;
	root["errorId"] = ret;
	root["etype"] = 13;
	root["param1"] = type;
	root["param2"] = count;
	root["extra"] = 0;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	LOG_INFO("resp.mRespJsonStr = %s",resp.mRespJsonStr.c_str());
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::GMEmailSend(UInt64 tempid, UInt64 toPlayer, std::string des, std::string contant)
{
	UInt64 times = time(0);
	//判断是否为发送给单个人

	std::string ::size_type  findIndex = des.find(';');

	std::stringstream mystream; 
	std::string titleStr = "";
	std::string fromStr = "";

	if(findIndex != std::string::npos)
	{
		titleStr = des.substr(0,findIndex);
		std::string leftstr = des.substr(findIndex +1);
		findIndex = leftstr.find(';');
		if(findIndex != std::string::npos)
		{
			fromStr = leftstr.substr(0,findIndex);
		}
	}


	if(toPlayer)
	{
		PersistAddEmail addEmail;
		EmailData emailData;

		emailData.m_nEmailUid = LogicSystem::getSingleton().generateEmailGuid();
		emailData.m_nGetState = 0;
		emailData.m_nOpenState = 0;
		emailData.m_nPlayerUid = toPlayer;
		emailData.m_nTempId = tempid;
		emailData.m_nTime = times;
		emailData.m_strContent = contant.c_str();
		emailData.m_strDes = des.c_str();
		emailData.m_strTittle = titleStr.c_str();
		emailData.m_strFrom = fromStr.c_str();
	
		if(contant.empty())
		{
			emailData.m_nGetState = 1;
		}
	

		addEmail.mEmailData = emailData;
		PersistSystem::getSingleton().postThreadMsg(addEmail, toPlayer);
		Player * sendToPlayer = LogicSystem::getSingleton().getPlayerByGuid(toPlayer);

		if(sendToPlayer)
		{
			ConnId sendToCon = sendToPlayer->getConnId();
			sendToPlayer->getEmailManager().addEmail(emailData);

			if(sendToCon)
			{
				//发送消息给玩家
				NewEmailNotify emailNotify;
				emailNotify.mPacketID = EMAIL_NOTIFY;
				Json::Value emailRoot;
				emailRoot["emailuid"] = addEmail.mEmailData.m_nEmailUid;
				emailRoot["tempid"] = tempid;
				emailRoot["openState"] = 0;
				emailRoot["getstate"] = addEmail.mEmailData.m_nGetState;
				emailRoot["des"] = des;
				emailRoot["contant"] = contant;
				
				emailRoot["time"] = times;
				emailRoot["title"] = titleStr;
				emailRoot["from"] = fromStr;
				


				Json::StyledWriter writer;

				emailNotify.mRespJsonStr = writer.write(emailRoot);
				LOG_INFO("emailNotify.mRespJsonStr = %s",emailNotify.mRespJsonStr.c_str());
				NetworkSystem::getSingleton().sendMsg(emailNotify, sendToCon);
			}

		}


		return;
	}

	UInt64 playerGuid = LogicSystem::getSingleton().getPlayerGuid();
	for(UInt32 i = 1000; i < playerGuid; i++)
	{
		PersistAddEmail addEmail;
		EmailData emailData;

		emailData.m_nEmailUid = LogicSystem::getSingleton().generateEmailGuid();
		emailData.m_nGetState = 0;
		emailData.m_nOpenState = 0;
		emailData.m_nPlayerUid = i;
		emailData.m_nTempId = tempid;
		emailData.m_nTime = times;
		emailData.m_strContent = contant.c_str();
		emailData.m_strDes = des.c_str();
		
		emailData.m_strTittle = titleStr.c_str();
		emailData.m_strFrom = fromStr.c_str();

		if(contant.empty())
		{
			emailData.m_nGetState = 1;
		}
	
		addEmail.mEmailData = emailData;

		PersistSystem::getSingleton().postThreadMsg(addEmail, addEmail.mEmailData.m_nPlayerUid);
		Player * sendToPlayer = LogicSystem::getSingleton().getPlayerByGuid(addEmail.mEmailData.m_nPlayerUid);

		//判断玩家是否在线
		if(sendToPlayer)
		{
			ConnId sendToCon = sendToPlayer->getConnId();
			sendToPlayer->getEmailManager().addEmail(emailData);

			if(sendToCon)
			{
				//发送消息给玩家
				NewEmailNotify emailNotify;
				emailNotify.mPacketID = EMAIL_NOTIFY;
				Json::Value emailRoot;
				emailRoot["emailuid"] = addEmail.mEmailData.m_nEmailUid;
				emailRoot["tempid"] = tempid;
				emailRoot["openState"] = 0;
				emailRoot["getstate"] = addEmail.mEmailData.m_nGetState;
				emailRoot["des"] = des;
				emailRoot["contant"] = contant;
				emailRoot["time"] = times;
			
				emailRoot["title"] = titleStr;
				emailRoot["from"] = fromStr;
		

				Json::StyledWriter writer;

				emailNotify.mRespJsonStr = writer.write(emailRoot);

				NetworkSystem::getSingleton().sendMsg(emailNotify, sendToCon);
			}
		}
	}

}

void GMManager::GMMsgNotify(std::string msg, UInt64 endtime, UInt32 term)
{
	LogicSystem::getSingleton().sendGMMsg(msg, endtime, term);
}

void GMManager::dealDbcReload(std::string dbcname)
{
	Lynx::Map<std::string,  Lynx::UInt32>::Iter * findIter = m_mapGlobleFlag.find(dbcname);
	if(!findIter)
	{
		LOG_INFO("can't find table name is %s", dbcname.c_str());
	}

	if(dbcname == "achievement")
	{
		if(findIter->mValue)
		{
			gAchievementTable->reloadFromDbc("achievement.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gAchievementTableCopy->reloadFromDbc("achievement.dbc");
			findIter->mValue = 1;
		}
		
	}

	if(dbcname == "item")
	{
		if(findIter->mValue)
		{
			gItemTable->reloadFromDbc("item.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gItemTableCopy->reloadFromDbc("item.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "attrenhance")
	{
		if(findIter->mValue)
		{
			gAttrEnhanceTable->reloadFromDbc("attrenhance.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gAttrEnhanceTableCopy->reloadFromDbc("attrenhance.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "attrrandom")
	{
		if(findIter->mValue)
		{
			gAttrRandomTable->reloadFromDbc("attrrandom.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gAttrRandomTableCopy->reloadFromDbc("attrrandom.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "buff")
	{
		if(findIter->mValue)
		{
			gBuffTable->reloadFromDbc("attrrandom.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gBuffTableCopy->reloadFromDbc("attrrandom.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "chapter")
	{
		if(findIter->mValue)
		{
			gChapterTable->reloadFromDbc("chapter.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gChapterTableCopy->reloadFromDbc("chapter.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "effect")
	{
		if(findIter->mValue)
		{
			gEffectTable->reloadFromDbc("effect.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gEffectTableCopy->reloadFromDbc("effect.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "equip")
	{
		if(findIter->mValue)
		{
			gEquipTable->reloadFromDbc("equip.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gEquipTableCopy->reloadFromDbc("equip.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "equipset")
	{
		if(findIter->mValue)
		{
			gEquipSetTable->reloadFromDbc("equipset.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gEquipSetTableCopy->reloadFromDbc("equipset.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "fashion")
	{
		if(findIter->mValue)
		{
			gFashionTable->reloadFromDbc("fashion.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gFashionTableCopy->reloadFromDbc("fashion.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "gem")
	{
		if(findIter->mValue)
		{
			gGemTable->reloadFromDbc("gem.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gGemTableCopy->reloadFromDbc("gem.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "hero")
	{
		if(findIter->mValue)
		{
			gHeroTable->reloadFromDbc("hero.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gHeroTableCopy->reloadFromDbc("hero.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "levelgrow")
	{
		if(findIter->mValue)
		{
			gLevelGrowTable->reloadFromDbc("levelgrow.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gLevelGrowTableCopy->reloadFromDbc("levelgrow.dbc");
			findIter->mValue = 1;
		}

	}

	if(dbcname == "powervalue")
	{
		if(findIter->mValue)
		{
			gPowerValueTable->reloadFromDbc("powervalue.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gPowerValueTableCopy->reloadFromDbc("powervalue.dbc");
			findIter->mValue = 1;
		}
	}

	if(dbcname == "playerexp")
	{
		if(findIter->mValue)
		{
			gPlayerExpTable->reloadFromDbc("playerexp.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gPlayerExpTableCopy->reloadFromDbc("playerexp.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "stage")
	{
		if(findIter->mValue)
		{
			gStageTable->reloadFromDbc("stage.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gStageTableCopy->reloadFromDbc("stage.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "copyfinishtime")
	{
		if(findIter->mValue)
		{
			gCopyFinishTable->reloadFromDbc("copyfinishtime.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gCopyFinishTableCopy->reloadFromDbc("copyfinishtime.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "globalvar")
	{
		if(findIter->mValue)
		{
			gGlobalVarTable->reloadFromDbc("globalvar.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gGlobalVarTableCopy->reloadFromDbc("globalvar.dbc");
			findIter->mValue = 1;
		}
	}

	if(dbcname == "award")
	{
		if(findIter->mValue)
		{
			gAwardTable->reloadFromDbc("award.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gAwardTableCopy->reloadFromDbc("award.dbc");
			findIter->mValue = 1;
		}
	}	


	if(dbcname == "awardcard")
	{
		if(findIter->mValue)
		{
			gAwardCardTable->reloadFromDbc("awardcard.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gAwardCardTableCopy->reloadFromDbc("awardcard.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "equipattrpolish")
	{
		if(findIter->mValue)
		{
			gAttrPolishTable->reloadFromDbc("equipattrpolish.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gAttrPolishTableCopy->reloadFromDbc("equipattrpolish.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "equippolishpoints")
	{
		if(findIter->mValue)
		{
			gPolishPointsTable->reloadFromDbc("equippolishpoints.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gPolishPointsTableCopy->reloadFromDbc("equippolishpoints.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "equipmaterialpoints")
	{
		if(findIter->mValue)
		{
			gMaterialPointsTable->reloadFromDbc("equipmaterialpoints.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gMaterialPointsTableCopy->reloadFromDbc("equipmaterialpoints.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "awardcontent")
	{
		if(findIter->mValue)
		{
			gAwardContentTable->reloadFromDbc("awardcontent.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gAwardContentTableCopy->reloadFromDbc("awardcontent.dbc");
			findIter->mValue = 1;
		}
	}	
	
	if(dbcname == "lottery")
	{
		if(findIter->mValue)
		{
			gLotteryTable->reloadFromDbc("lottery.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gLotteryTableCopy->reloadFromDbc("lottery.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "boxcounter")
	{
		if(findIter->mValue)
		{
			gBoxCounterTable->reloadFromDbc("boxcounter.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gBoxCounterTableCopy->reloadFromDbc("boxcounter.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "skeysvalue")
	{
		if(findIter->mValue)
		{
			gSkeySvalueTable->reloadFromDbc("skeysvalue.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gSkeySvalueTableCopy->reloadFromDbc("skeysvalue.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "gemcombine")
	{
		if(findIter->mValue)
		{
			gGemCombineTable->reloadFromDbc("gemcombine.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gGemCombineTableCopy->reloadFromDbc("gemcombine.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "gemcombineonce")
	{
		if(findIter->mValue)
		{
			gGemCombineOnceTable->reloadFromDbc("gemcombineonce.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gGemCombineOnceTableCopy->reloadFromDbc("gemcombineonce.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "vip")
	{
		if(findIter->mValue)
		{
			gVipTable->reloadFromDbc("vip.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gVipTableCopy->reloadFromDbc("vip.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "welfaremarket")
	{
		if(findIter->mValue)
		{
			gWelfareMarketTable->reloadFromDbc("welfaremarket.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gWelfareMarketTableCopy->reloadFromDbc("welfaremarket.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "rhymelevel")
	{
		if(findIter->mValue)
		{
			gRhymeLevelTable->reloadFromDbc("rhymelevel.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gRhymeLevelTableCopy->reloadFromDbc("rhymelevel.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "rhymeacupoint")
	{
		if(findIter->mValue)
		{
			gRhymeAcupointTable->reloadFromDbc("rhymeacupoint.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gRhymeAcupointTableCopy->reloadFromDbc("rhymeacupoint.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "rhymeskill")
	{
		if(findIter->mValue)
		{
			gRhymeSkillTable->reloadFromDbc("rhymeskill.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gRhymeSkillTableCopy->reloadFromDbc("rhymeskill.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "skilllevel")
	{
		if(findIter->mValue)
		{
			gSkillLevelTable->reloadFromDbc("skilllevel.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gSkillLevelTableCopy->reloadFromDbc("skilllevel.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "skillbase")
	{
		if(findIter->mValue)
		{
			gSkillBaseTable->reloadFromDbc("skillbase.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gSkillBaseTableCopy->reloadFromDbc("skillbase.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "skillcompare")
	{
		if(findIter->mValue)
		{
			gSkillConvertTable->reloadFromDbc("skillcompare.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gSkillConvertTableCopy->reloadFromDbc("skillcompare.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "hoarstonebase")
	{
		if(findIter->mValue)
		{
			gHoarstoneBaseTable->reloadFromDbc("hoarstonebase.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gHoarstoneBaseTableCopy->reloadFromDbc("hoarstonebase.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "hoarstoneequip")
	{
		if(findIter->mValue)
		{
			gHoarstoneRuneTable->reloadFromDbc("hoarstoneequip.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gHoarstoneRuneTableCopy->reloadFromDbc("hoarstoneequip.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "hoarstonelevel")
	{
		if(findIter->mValue)
		{
			gHoarstoneLvTable->reloadFromDbc("hoarstonelevel.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gHoarstoneLvTableCopy->reloadFromDbc("hoarstonelevel.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "hoarstonestar")
	{
		if(findIter->mValue)
		{
			gHoarstoneStarTable->reloadFromDbc("hoarstonestar.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gHoarstoneStarTableCopy->reloadFromDbc("hoarstonestar.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "new_equip_level")
	{
		if(findIter->mValue)
		{
			gNewEquipLvTable->reloadFromDbc("new_equip_level.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gNewEquipLvTableCopy->reloadFromDbc("new_equip_level.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "hero_equip_rule")
	{
		if(findIter->mValue)
		{
			gHeroEquipRuleTable->reloadFromDbc("hero_equip_rule.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gHeroEquipRuleTableCopy->reloadFromDbc("hero_equip_rule.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "equip_gem_rule")
	{
		if(findIter->mValue)
		{
			gEquipGemRuleTable->reloadFromDbc("equip_gem_rule.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gEquipGemRuleTableCopy->reloadFromDbc("equip_gem_rule.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "material")
	{
		if(findIter->mValue)
		{
			gMaterialTable->reloadFromDbc("material.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gMaterialTableCopy->reloadFromDbc("material.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "new_gem")
	{
		if(findIter->mValue)
		{
			gNewGemTable->reloadFromDbc("new_gem.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gNewGemTableCopy->reloadFromDbc("new_gem.dbc");
			findIter->mValue = 1;
		}
	}	


	if(dbcname == "new_jewelry")
	{
		if(findIter->mValue)
		{
			gNewJewelryTable->reloadFromDbc("new_jewelry.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gNewJewelryTableCopy->reloadFromDbc("new_jewelry.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "servantbattleopen")
	{
		if(findIter->mValue)
		{
			gServantBattleOpenTable->reloadFromDbc("servantbattleopen.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gServantBattleOpenTableCopy->reloadFromDbc("servantbattleopen.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "servant")
	{
		if(findIter->mValue)
		{
			gServantTable->reloadFromDbc("servant.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gServantTableCopy->reloadFromDbc("servant.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "servantstar")
	{
		if(findIter->mValue)
		{
			gServantStarTable->reloadFromDbc("servantstar.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gServantStarTableCopy->reloadFromDbc("servantstar.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "servantlvupmaterial")
	{
		if(findIter->mValue)
		{
			gServantMaterialTable->reloadFromDbc("servantlvupmaterial.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gServantMaterialTableCopy->reloadFromDbc("servantlvupmaterial.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "servantlvup")
	{
		if(findIter->mValue)
		{
			gServantLvUpTable->reloadFromDbc("servantlvup.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gServantLvUpTableCopy->reloadFromDbc("servantlvup.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "servantfloor")
	{
		if(findIter->mValue)
		{
			gServantFloorTable->reloadFromDbc("servantfloor.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gServantFloorTableCopy->reloadFromDbc("servantfloor.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "servanttreasure")
	{
		if(findIter->mValue)
		{
			gServantTreasureTable->reloadFromDbc("servanttreasure.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gServantTreasureTableCopy->reloadFromDbc("servanttreasure.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "servantassisbattle")
	{
		if(findIter->mValue)
		{
			gServantAssistBattleTable->reloadFromDbc("servantassisbattle.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gServantAssistBattleTableCopy->reloadFromDbc("servantassisbattle.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "servantlucky")
	{
		if(findIter->mValue)
		{
			gServantLuckTable->reloadFromDbc("servantlucky.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gServantLuckTableCopy->reloadFromDbc("servantlucky.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "attrpower")
	{
		if(findIter->mValue)
		{
			gAttrPowerTable->reloadFromDbc("attrpower.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gAttrPowerTableCopy->reloadFromDbc("attrpower.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "herofashion")
	{
		if(findIter->mValue)
		{
			gHeroFashionTable->reloadFromDbc("herofashion.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gHeroFashionTableCopy->reloadFromDbc("herofashion.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "event")
	{
		if(findIter->mValue)
		{
			gEventTable->reloadFromDbc("event.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gEventTableCopy->reloadFromDbc("event.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "dailytask")
	{
		if(findIter->mValue)
		{
			gDailyTaskTable->reloadFromDbc("dailytask.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gDailyTaskTableCopy->reloadFromDbc("dailytask.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "active")
	{
		if(findIter->mValue)
		{
			gDailyActiveTable->reloadFromDbc("active.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gDailyActiveTableCopy->reloadFromDbc("active.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "sevendaystrainning")
	{
		if(findIter->mValue)
		{
			gSevenDayTrainningTable->reloadFromDbc("sevendaystrainning.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gSevenDayTrainningTableCopy->reloadFromDbc("sevendaystrainning.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "consortmsg")
	{
		if(findIter->mValue)
		{
			gConsortMsgTable->reloadFromDbc("consortmsg.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gConsortMsgTableCopy->reloadFromDbc("consortmsg.dbc");
			findIter->mValue = 1;
		}
	}	
	if(dbcname == "rankedshop")
	{
		if(findIter->mValue)
		{
			gRankedShopTable->reloadFromDbc("rankedshop.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gRankedShopTableCopy->reloadFromDbc("rankedshop.dbc");
			findIter->mValue = 1;
		}
	}

	if(dbcname == "rankreward")
	{
		if(findIter->mValue)
		{
			gRankRewardTable->reloadFromDbc("rankreward.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gRankRewardTableCopy->reloadFromDbc("rankreward.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "systeminfo")
	{
		if(findIter->mValue)
		{
			gSystemInfoTable->reloadFromDbc("systeminfo.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gSystemInfoTableCopy->reloadFromDbc("systeminfo.dbc");
			findIter->mValue = 1;
		}
	}	

	if(dbcname == "guildlevel")
	{
		if(findIter->mValue)
		{
			gConsortLvTable->reloadFromDbc("guildlevel.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gConsortLvTableCopy->reloadFromDbc("guildlevel.dbc");
			findIter->mValue = 1;
		}
	}	


	if(dbcname == "guildsign")
	{
		if(findIter->mValue)
		{
			gConsortSignTable->reloadFromDbc("guildsign.dbc");
			findIter->mValue = 0;
		}
		else
		{
			gConsortSignTable->reloadFromDbc("guildsign.dbc");
			findIter->mValue = 1;
		}
	}	
}

