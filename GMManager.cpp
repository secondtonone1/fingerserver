#include "GMManager.h"
#include "Player.h"
#include "LogicSystem.h"
#include "../FireConfirm/StageCalc.h"
#include "../FireConfirm/InlineActivity.h"
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
		case 100:
			GMsetrechargeNum(param1, param2);
			break;
		case 505:
			GMEmailSend(param1, param2, param3, param4);
			break;
			
		default:
			break;
		}
}

void GMManager::dealLvAdd(UInt64 lv)
{
		m_pPlayer->mPlayerData.mBaseData.m_nLevel += lv;
		if(m_pPlayer->mPlayerData.mBaseData.m_nLevel > MAXLV)
		{
			m_pPlayer->mPlayerData.mBaseData.m_nLevel = MAXLV;
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
		cout << resp.mRespJsonStr;
		resp.mPacketID = BOC_GM_RESP;
		const ConnId & connId = m_pPlayer->getConnId();
		NetworkSystem::getSingleton().sendMsg(resp, connId );
		
}

void GMManager::dealVipAdd(UInt64 lv)
{
	m_pPlayer->mPlayerData.mBaseData.m_nVipLevel += lv;
	
	if(m_pPlayer->mPlayerData.mBaseData.m_nVipLevel > MAXVIPLV)
	{
		m_pPlayer->mPlayerData.mBaseData.m_nVipLevel = MAXVIPLV;
	}

	m_pPlayer->mPlayerData.mBaseData.m_nVipExp = 0;
	m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);
	m_pPlayer->checkLvActiveConditon();
	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 2;
	root["param1"] = lv;
	root["param2"] = 0;
	root["extra"] = m_pPlayer->mPlayerData.mBaseData.m_nVipLevel;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	cout << resp.mRespJsonStr;
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::dealCoinChange(UInt64 coin)
{
	m_pPlayer->mPlayerData.mBaseData.m_nCoin = coin;
	m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 3;
	root["param1"] = coin;
	root["param2"] = 0;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	cout << resp.mRespJsonStr;
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::dealGoldChange(UInt64 gold)
{
	m_pPlayer->mPlayerData.mBaseData.m_nGold = gold;
	m_pPlayer->getPersistManager().setDirtyBit(BASEDATABIT);

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 4;
	root["param1"] = gold;
	root["param2"] = 0;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
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
		cout << resp.mRespJsonStr;
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
		cout << resp.mRespJsonStr;
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
		cout << resp.mRespJsonStr;
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
		cout << resp.mRespJsonStr;
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
		cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
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
		cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
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
		cout << resp.mRespJsonStr;
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
		cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}


void GMManager::dealTwelvePalaceHaoJiaoChange()
{
	StageCalcManager::getSingleton().resetHaoJiao(m_pPlayer->getPlayerGuid());

	GCGMResp resp;
	Json::Value root;
	root["errorId"] = LynxErrno::None;
	root["etype"] = 17;
	root["extra"] = GlobalVarManager::getSingleton().getTwelvePalace().resetlowerlimit;
	
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
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
	cout << resp.mRespJsonStr;
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::dealClimbTowerChange(UInt64 type, UInt64 count)
{
	UInt32 ret = m_pPlayer->getStageManager().resetClimbTowerchallegeTimes();
	GCGMResp resp;
	Json::Value root;
	root["errorId"] = ret;
	root["etype"] = 12;
	root["param1"] = type;
	root["param2"] = count;
	root["extra"] = GlobalVarManager::getSingleton().getclimbtower().resetlowerlimit;
	Json::StyledWriter writer;
	resp.mRespJsonStr = writer.write(root);
	cout << resp.mRespJsonStr;
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

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
	cout << resp.mRespJsonStr;
	resp.mPacketID = BOC_GM_RESP;
	const ConnId & connId = m_pPlayer->getConnId();
	NetworkSystem::getSingleton().sendMsg(resp, connId );
}

void GMManager::GMEmailSend(UInt64 tempid, UInt64 toPlayer, std::string des, std::string contant)
{
	UInt64 times = time(0);
	//判断是否为发送给单个人
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
				emailRoot["getstate"] = 0;
				emailRoot["des"] = des;
				emailRoot["contant"] = contant;
				
				emailRoot["time"] = times;

				Json::StyledWriter writer;

				emailNotify.mRespJsonStr = writer.write(emailRoot);
				cout << emailNotify.mRespJsonStr;
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
				emailRoot["getstate"] = 0;
				emailRoot["des"] = des;
				emailRoot["contant"] = contant;
				emailRoot["time"] = times;

				Json::StyledWriter writer;

				emailNotify.mRespJsonStr = writer.write(emailRoot);

				NetworkSystem::getSingleton().sendMsg(emailNotify, sendToCon);
			}
		}
	}

	
}