
#include "RankGame.h"
#include "../LogicSystem.h"
#include "../PersistSystem.h"
#include "GlobalValue.h"
#include "Shop.h"
// #include "Robot.h"
#include "../RedisManager.h"
#include "../ServerData.h"
#include "BaseChang.h"
#include "../RankSystem.h"

using namespace Lynx;

void RankGameManager::initRankGame()
{

	PersistGetScoreRank getScoreRank;
	getScoreRank.m_nTime = 0;
	PersistSystem::getSingleton().postThreadMsg(getScoreRank, 0);

// 	PersistGetRankGameReport getRankGameReport;
// 	getRankGameReport.m_nTime = 0;
// 	PersistSystem::getSingleton().postThreadMsg(getRankGameReport, 0);

	

	
}

void RankGameManager::onRankGameReq(const  ConnId& connId,RankGameReq & req)
{

	

	bool needSave = false;
	Map<Guid,RankData>::Iter *guidIter;
	RankGameResp resp;

	req.convertJsonToData(req.strReceive);
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}

	//for test
// 	List<BaseInfoData> getList;
// 	RankSystem::getSingleton().getLvSimilarPlayers(player->getPlayerLeval(), player, getList);
// 





if (RankGameManager::getSingleton().checkSystemOpen(player->getPlayerGuid(),SYSTEM_RANK) == false)
	{	
		
		//LOG_INFO("Open rankGame level not enough");
		if (req.needSend == true)
		{
			resp.result =  LynxErrno::SytemNotOpen;
			std::string jsonStr = resp.convertDataToJson();
			NetworkSystem::getSingleton().sender( connId,RANKGAME_INFO_RESP,jsonStr);
		}	
		
		return;		
	}

	

	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();
	PlayerRankGameData rankGameData = player->getRankGameData();

	if (rankGameData.m_Time == 0)
	{
		RankGameManager::getSingleton().initPlayerRankGame(player->getPlayerGuid());
	}

	if (rankGameData.m_LastChallengeGuid != 0)
	{

		RankGameEndReq endReq;
		endReq.result = LynxErrno::TimeOut;
		endReq.needSend = false;
		RankGameManager::getSingleton().onRankGameEndReq(connId,endReq );
		//onRankGameEndReq里有重新请求onRankGameReq
		return;
	}

	

	guidIter = mGuidRankGameRankingData.find(player->getPlayerGuid());//initPlayerRankGame有修改index
	if (guidIter == NULL)
	{
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,RANKGAME_INFO_RESP,sendStr);
		return;
	}

	resp.lastIndex = rankGameData.m_LastIndex;
	
	if (req.isLogin == false)
	{
		RankGameManager::getSingleton().getRankGameReward(player->getPlayerGuid(),rankGameData.m_LastIndex,resp.resIndex,resp.ends);//这里有加积分
		if (resp.ends.size() >0)
		{
			GiftManager::getSingleton().saveEndsGetAttr(player->getPlayerGuid(),resp.ends,resp.allAttr,MiniLog112);
		}
		rankGameData = player->getRankGameData();//要在这重新获取
		rankGameData.m_LastIndex = 0;
		rankGameData.m_LastTime = 0;
	}
	
	if (rankGameData.m_PlayerIds == "")
	{
		needSave = true;
	}
	
	RankGameManager::getSingleton().getRankGameCanChallage(player->getPlayerGuid(),rankGameData.m_PlayerIds,resp.otherRoleInfo);

	if (resp.lastIndex > 0 || needSave == true)
	{
		player->setRankGameData(rankGameData);
		player->getPersistManager().setDirtyBit(RANKGAMEDATABIT);
	}
	resp.restChalTimes = dailyResetData.m_nRankGameLeftTimes;
	resp.playerScore = rankGameData.m_Score;
	resp.curRank = guidIter->mValue.index;
	resp.buyTimes = dailyResetData.m_nRankGameBuyTimes;
	resp.sessions =  rankGameData.m_Sessions;
	resp.winSessions =  rankGameData.m_WinSessions;
	resp.maskResetTimes = 0;	
	resp.maskNum = rankGameData.m_MaskNum;
	resp.newReportFlag = rankGameData.newReportFlag;

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,RANKGAME_INFO_RESP,jsonStr);
	LOG_INFO("RANKGAME_INFO_RESP= %s",jsonStr.c_str());
	
	
}


void RankGameManager::getRankGameCanChallage(Guid playerID, String& playerIds,List<RankData*> &otherRoleInfo)
{
	Map<Guid,RankData>::Iter *guidIter;
	List<Guid> playerIDList;

	RankGameManager::getSingleton().getRankGamePlayers(playerID,playerIDList);
	
	for (List<Guid>::Iter * it = playerIDList.begin();it != NULL ;it = playerIDList.next(it))
	{
		guidIter = mGuidRankGameRankingData.find(it->mValue);
		if (guidIter == NULL)
		{
			continue;
		}
		otherRoleInfo.insertTail(&guidIter->mValue);
	}
}

void RankGameManager::onRankGameRankingReq(const  ConnId& connId,RankGameRankingReq & req)
{
	
	RankGameRankingResp resp;
	Map<Guid,RankData>::Iter* iter;
	Map<UInt32,RankData*>::Iter* it;

	LOG_INFO("onRankGameRankingReq %s",req.strReceive.c_str());
	req.convertJsonToData(req.strReceive);
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}
	

	iter = mGuidRankGameRankingData.find(player->getPlayerGuid());
	if (iter == NULL)
	{
		//todo
		resp.flag = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,RANKGAME_RANKING_RESP,sendStr);

		return;
	}
	

	if (req.playerIDs.size() == 0)
	{
		for (UInt32 i = 1;i<=100;i++)
		{
			 it = mRankingIndexID.find(i);
			 if (it == NULL)
			 {
				 LOG_INFO("rankgame index not found %d",i);
				 return;
			 }
			 resp.otherRoleInfo.insertTail(*it->mValue); 
		}
		
	}
	else if (req.playerIDs.size() == 100)
	{
		resp.flag = 1;
		UInt32 ii = 0;
		for (List<UInt64>::Iter *item = req.playerIDs.begin();item!=NULL;item = req.playerIDs.next(item))
		{
			ii ++;
			it = mRankingIndexID.find(ii);
			if (it == NULL)
			{
				LOG_INFO("rankgame index not found %d",ii);
				return;
			}
			if (it->mValue->playerUid != item->mValue)
			{
				resp.otherRoleInfo.insertTail(*it->mValue); 
				if (resp.otherRoleInfo.size() >  10)
				{
					resp.flag = 0;
					resp.otherRoleInfo.clear();
					for (UInt32 i = 1;i<=100;i++)
					{
						it = mRankingIndexID.find(i);
						if (it == NULL)
						{
							LOG_INFO("rankgame index not found %d",i);
							return;
						}
						resp.otherRoleInfo.insertTail(*it->mValue); 
					}
					break;
				}
			}
			
			
		}
	}
	else
	{
		LOG_INFO("playerIDs size error %d",req.playerIDs.size());
		return;
	}


	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,RANKGAME_RANKING_RESP,jsonStr);
	LOG_INFO("RANKGAME_RANKING_RESP= %s",jsonStr.c_str());
	LOG_INFO("RANKGAME_RANKING_RESP strlen= %d",strlen(jsonStr.c_str()));

}
void RankGameManager::onRankGameReportReq(const  ConnId& connId,RankGameReportReq & req)
{
	RankGameReportResp resp;

	req.convertJsonToData(req.strReceive);
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}
	PlayerRankGameData rankGameData = player->getRankGameData();


	resp.flag = req.flag;
	List<ReportData> reportData = rankGameData.m_ReportData;
	for (List<ReportData>::Iter *it = reportData.begin();it !=NULL;it = reportData.next(it) )
	{
		if (req.flag < it->mValue.id)
		{
			resp.reportData.insertTail(it->mValue);
			resp.flag = it->mValue.id;
		}
	}

	if (rankGameData.newReportFlag == 1)
	{
		rankGameData.newReportFlag = 0;
		player->setRankGameData(rankGameData);
		player->getPersistManager().setDirtyBit(RANKGAMEDATABIT);
	}

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,RANKGAME_REPORT_RESP,jsonStr);
	LOG_INFO("RANKGAME_REPORT_RESP= %s",jsonStr.c_str());

}

void RankGameManager::onRankGameShopReq(const  ConnId& connId,RankGameShopReq &req )
{
	UInt32 refreshTimes = 0;
	RankGameShopResp resp;

	req.convertJsonToData(req.strReceive);
	resp.reqType = req.reqType;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);

	if (player == NULL)
	{
		return;
	}
	
	resp.result = RankGameManager::getSingleton().getRankGameShopInfo( player->getPlayerGuid(),req.reqType,resp);

	resp.gold = player->getPlayerGold();


 	if (req.needSend == true)
	{
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( connId,RANKGAME_SHOP_RESP,jsonStr);
	}	

}

void RankGameManager::onRankGameBeginReq(const  ConnId& connId,RankGameBeginReq & req)
{
	char dest[64] = {};
	String playerUidStr;
	RankGameBeginResp resp;
	LOG_INFO("onRankGameBeginReq =  %s",req.strReceive.c_str());

	req.convertJsonToData(req.strReceive);
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}
	PlayerRankGameData rankGameData = player->getRankGameData();
	LOG_INFO("rankGameData1 =  %d",rankGameData.m_Sessions);
	 LOG_INFO("rankGameData11 =  %d",rankGameData.m_WinSessions);
	PlayerDailyResetData dailyResetData= player->getPlayerDailyResetData();
	if (rankGameData.m_LastChallengeGuid != 0)
	{
	
		RankGameEndReq endReq;
		endReq.result = LynxErrno::TimeOut;
		endReq.needSend = false;
		RankGameManager::getSingleton().onRankGameEndReq(connId,endReq );
	}
	player->ResetFireConfirmData();
	

	if(dailyResetData.m_nRankGameLeftTimes < 1)
	{
		resp.result  = LynxErrno::RankGameTimesNotEnough;		
	}


	if (resp.result == LynxErrno::None)
	{
		FireConfirmManager::getSingleton().SetCopyStart(player,RANKGAMECOPYID);	
		 dailyResetData= player->getPlayerDailyResetData();

		dailyResetData.m_nRankGameLeftTimes --;
		player->setPlayerDailyResetData(dailyResetData);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);
		PlayerFireConfirmData mFireConfirmData =  player->GetFireConfirmData();
		mFireConfirmData.m_OtherPlayerID = req.playerId;
		player->SetFireConfirmData(mFireConfirmData);

		 rankGameData = player->getRankGameData();
		 rankGameData.m_LastChallengeGuid = req.playerId;
		 player->setRankGameData(rankGameData);
		 LOG_INFO("rankGameData2 =  %d",rankGameData.m_Sessions);
		 LOG_INFO("rankGameData21 =  %d",rankGameData.m_WinSessions);
		 player->getPersistManager().setDirtyBit(RANKGAMEDATABIT);
		
		sprintf(dest,"%llu",req.playerId);
		playerUidStr = (String)dest;	
		if (req.playerId  > RobotMinRoleID && req.playerId <RobotMaxRoleID)
		{
			resp.isRobot = 1;
		}

		resp.otherPlayer =   LogicSystem::getSingleton().getDetailInfo(playerUidStr.c_str()).c_str();
		LOG_INFO("RANKGAME_BEGIN_RESP playerUidStr=  %s",playerUidStr.c_str());
		LOG_INFO("RANKGAME_BEGIN_RESP otherPlayer=  %s",resp.otherPlayer.c_str());


	}
	

	resp.chalTimes = dailyResetData.m_nRankGameLeftTimes;
// 	resp.confirmIDs =  FireConfirmManager::getSingleton().getFireConfirmCondition(connId);

	std::string jsonStr = resp.convertDataToJson();			
	LOG_INFO("RANKGAME_BEGIN_RESP =  %s",jsonStr.c_str());
	NetworkSystem::getSingleton().sender( connId,RANKGAME_BEGIN_RESP,jsonStr);


}


void RankGameManager::checkScoreLowest(UInt64 playerID,UInt32 &score)
{


	if (playerID > RobotMinRoleID && playerID <RobotMaxRoleID)
	{
		RobotTableTemplate *robotTableTemplate = gRobotTable->get(playerID);
		if (score < robotTableTemplate->score)
		{
			score =  robotTableTemplate->score;
		}
	}
	else
	{
		if (score < RankGameInitScore)
		{
			score = RankGameInitScore;
		}
	}

}


void RankGameManager::onRankGameEndReq(const  ConnId& connId,RankGameEndReq & req)
{

	int winSessions = 0;
	UInt32 otherPlayerLastIndex = 0;
	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 otherPlayerScore  = 0;
	UInt64 otherPlayerID = 0;	
	ReportData reportData;
	ReportData oterReportData;
	Player *otherPlayer;
	PlayerRankGameData otherPlayerRankGameData;
	Map<Guid,RankData>::Iter * it;
	Map<Guid,RankData>::Iter * iter;
	RankGameEndResp resp;


	LOG_INFO("onRankGameEndReq =  %s",req.strReceive.c_str());
	LOG_INFO("onRankGameEndReq result =  %d",req.result);

	BaseChangManager::getSingleton().rankGameEndconvertJsonToData(req);

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	//判断是否作弊
	UInt32 checkRet =  FireConfirmManager::getSingleton().checkIsCheat(player->getPlayerGuid(), req.fireConfirmData,req.result);	
	if(checkRet != LynxErrno::None&&req.result == 0)
	{
		FireConfirmManager::getSingleton().UpdataConfirmLevel(connId,true);
		resp.result = checkRet;
		std::string jsonStr = resp.convertDataToJson();	
		NetworkSystem::getSingleton().sender( connId,RANKGAME_END_RESP,jsonStr);
		return;
	}

	PlayerRankGameData rankGameData = player->getRankGameData();
	if (rankGameData.m_LastChallengeGuid == 0)
	{
		resp.result = LynxErrno::ClienServerDataNotMatch;
		std::string sendStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender(connId,RANKGAME_END_RESP,sendStr);
		return;
	}
	otherPlayerID = rankGameData.m_LastChallengeGuid;
	rankGameData.m_LastChallengeGuid = 0;
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	PlayerFireConfirmData mFireConfirmData = player->GetFireConfirmData();
	
	it =  mGuidRankGameRankingData.find(player->getPlayerGuid());
	resp.lastIndex = it->mValue.index;
	it =  mGuidRankGameRankingData.find(otherPlayerID);
	if (it == NULL)
	{
		LOG_INFO("onRankGameEndReq no found otherPlayerID =  %llu",otherPlayerID);

		return;
	}


	otherPlayerLastIndex = it->mValue.index;

	otherPlayerScore = it->mValue.score;
	rankGameData.m_Sessions ++;

	if (req.result == LynxErrno::None)
	{		
		resp.flag = 0;
		reportData.attackType = 1;//1：攻击成功  2：防御成功		3：攻击失败  4：防御失败
		oterReportData.attackType = 4;
		rankGameData.m_WinSessions ++;
		
		rankGameData.m_Score += globalValue.uRankGamewinScore;
		resp.changScore = globalValue.uRankGamewinScore;
		reportData.score = globalValue.uRankGamewinScore;

	
		oterReportData.score =  globalValue.uRankGameFailScoreCost;
		otherPlayerScore -= globalValue.uRankGameFailScoreCost;
		RankGameManager::getSingleton().checkScoreLowest(otherPlayerID,otherPlayerScore);
	}
	else
	{
		resp.flag = 1;
		reportData.attackType = 3;
		oterReportData.attackType = 2;
		winSessions ++ ;
		reportData.score = globalValue.uRankGameFailScoreCost;

		rankGameData.m_Score -= globalValue.uRankGameFailScoreCost;
		RankGameManager::getSingleton().checkScoreLowest(player->getPlayerGuid(),rankGameData.m_Score);
		
		
		resp.changScore = 0 -  globalValue.uRankGameFailScoreCost;
		oterReportData.score =  globalValue.uRankGamewinScore;
		otherPlayerScore += globalValue.uRankGamewinScore;
	}

	rankGameData.m_Time = nowTime;
	reportData.level =it->mValue.level;
	reportData.happenTime = nowTime;
	reportData.modelID = it->mValue.modelId;
	reportData.name = it->mValue.name.c_str();
	reportData.power =it->mValue.power;
	reportData.roleId = otherPlayerID;

	oterReportData.level =player->getPlayerLeval();
	oterReportData.happenTime = nowTime;
	oterReportData.modelID = player->getPlayerModelID();
	oterReportData.name = player->getPlayerName().c_str();
	oterReportData.power =player->getPlayerPower();
	oterReportData.roleId = player->getPlayerGuid();

	player->ResetFireConfirmData();
	rankGameData.newReportFlag = 1;
	player->setRankGameData(rankGameData);
	player->getPersistManager().setDirtyBit(RANKGAMEDATABIT);
	otherPlayer = LogicSystem::getSingleton().getPlayerByGuid(otherPlayerID);
	if (otherPlayer == NULL)
	{
		RankGameManager::getSingleton().saveRankGameScore(otherPlayerID,otherPlayerScore,nowTime,0,0,winSessions);
	}
	else
	{
		otherPlayerRankGameData = otherPlayer->getRankGameData();
		otherPlayerRankGameData.m_Score = otherPlayerScore;
		otherPlayerRankGameData.newReportFlag = 1;
		otherPlayerRankGameData.m_WinSessions += winSessions;
		otherPlayerRankGameData.m_Sessions ++;		
		otherPlayerRankGameData.m_Time = nowTime;
		otherPlayer->setRankGameData(otherPlayerRankGameData);
		otherPlayer->getPersistManager().setDirtyBit(RANKGAMEDATABIT);
	}

	RankGameManager::getSingleton().changRankGameRanking(player->getPlayerGuid(),rankGameData.m_Score);//位置不能错，先保存再插入修改
	RankGameManager::getSingleton().changRankGameRanking(otherPlayerID,otherPlayerScore);

	
	it =  mGuidRankGameRankingData.find(player->getPlayerGuid());
	reportData.rank = it->mValue.index;

	if (resp.lastIndex < it->mValue.index)
	{
		reportData.flag = 2;
	}
	if (resp.lastIndex < it->mValue.index)
	{
		reportData.flag = 2;
	}
	if (resp.lastIndex > it->mValue.index)
	{
		reportData.flag = 1;
	}
	if (resp.lastIndex == it->mValue.index)
	{
		reportData.flag = 0;
	}
	
	resp.index = it->mValue.index;
	RankGameManager::getSingleton().addRankGameReport(player->getPlayerGuid(),reportData,reportData.id);

	iter =  mGuidRankGameRankingData.find(otherPlayerID);
	oterReportData.rank = iter->mValue.index;
	if (otherPlayerLastIndex < iter->mValue.index)
	{
		oterReportData.flag = 2;
	}
	if (otherPlayerLastIndex > iter->mValue.index)
	{
		oterReportData.flag = 1;
	}
	if (otherPlayerLastIndex == it->mValue.index)
	{
		oterReportData.flag = 0;
	}


	RankGameManager::getSingleton().addRankGameReport(otherPlayerID,oterReportData,oterReportData.id);




	resp.score = rankGameData.m_Score;
	std::string jsonStr = resp.convertDataToJson();
	LOG_INFO("RANKGAME_END_RESP =  %s",jsonStr.c_str());

	if (req.result == LynxErrno::None)
	{
		LOG_INFO("RANKGAME_END_RESP it->mValue.index  =  %d",it->mValue.index );
		if(it->mValue.index <= 5)
		{
			LogicSystem::getSingleton().sendSystemMsg(4, player->getPlayerName(), it->mValue.index );
		}
		//更新成就系统 wwc
		player->getAchieveManager().updateAchieveData(QUALIFYSUCCESS,1);
	}

	if (req.needSend == true)
	{		
		NetworkSystem::getSingleton().sender( connId,RANKGAME_END_RESP,jsonStr);
	}

	RankGameReq rankGameReq;
	RankGameManager::getSingleton().onRankGameReq(connId,rankGameReq);

	//排位赛每日计数打点
	player->getAchieveManager().updateDailyTaskData(DLYDOQUALIFYCNT, 1);

	LogicSystem::getSingleton().updateSevenDayTask(player->getPlayerGuid(),SDT17,1);


}




void RankGameManager::saveRankGameScore(Guid playerID,UInt32 score,UInt32 time, UInt32 lastIndex,UInt32 lastTime,UInt32 addWinCount)
{
	PersistRankGameScoreSave req;
	req.m_nPlayerUid = playerID;
	req.m_score = score;
	req.m_time = time;
 	req.m_lastTime = lastTime;
	req.m_lastIndex = lastIndex;
	req.m_addWinCount = addWinCount;

	PersistSystem::getSingleton().postThreadMsg(req, 0);
}
	
void RankGameManager::addRankGameReport(Guid playerID,ReportData reportData,UInt32 &id)
{
	UInt32 i = 0;
	UInt32 maxID = 0;
	PersistRankGameReportSave req;

	if (playerID < RobotMaxRoleID)
	{
		LOG_INFO("playerID lower RobotMaxRoleID  not save RankGameReport %llu",playerID);
		return;
	}

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		req.saveID =  100000000;
		req.m_nPlayerUid = playerID;
		req.reportData = reportData;
		PersistSystem::getSingleton().postThreadMsg(req, 0);
		return;
	}
 	PlayerRankGameData rankGameData = player->getRankGameData();

	//有10个要删除
	if (rankGameData.m_ReportData.size() >= 10)
	{
		for (List<ReportData>:: Iter *iter = rankGameData.m_ReportData.begin();iter!=NULL;iter= rankGameData.m_ReportData.next(iter) )
		{	
			i++;
			if (i == 1)
			{
				req.deleteID = iter->mValue.id;
				rankGameData.m_ReportData.erase(iter);
			}
		
		}
	}

	for (List<ReportData>:: Iter *iter = rankGameData.m_ReportData.begin();iter!=NULL;iter= rankGameData.m_ReportData.next(iter) )
	{	
		maxID =  iter->mValue.id;
	}
	id = maxID + 1;
	reportData.id = maxID + 1;
	rankGameData.m_ReportData.insertTail(reportData);
	player->setRankGameData(rankGameData);
	req.saveID = reportData.id;
	req.m_nPlayerUid = playerID;
	req.reportData = reportData;
	PersistSystem::getSingleton().postThreadMsg(req, 0);
}

	
void RankGameManager::changRankGameRanking(Guid playerID,UInt32 score)
{
	RankData rankData;
	Map<Guid,RankData>::Iter *iter;

	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	iter = mGuidRankGameRankingData.find(playerID);
	if (iter == NULL)
	{		
		if (player == NULL)
		{
			//不在线 没初始化的只有新玩家
			return;
		}
		else
		{
			//在线没初始化的玩家
			rankData.playerUid = playerID;
			rankData.modelId = player->getPlayerModelID();
			rankData.level = player->getPlayerLeval();
			rankData.vipLv = player->getVipLevel();
			rankData.name = player->getPlayerName().c_str();
			rankData.score =  score;
			rankData.time = TimeUtil::getTimeSec();
			rankData.power = player->getPlayerPower();
			Map<UInt32,RankData*>::Iter *lastIter = mRankingIndexID.getMax();
			rankData.index = lastIter->mKey + 1;
			RankGameManager::getSingleton().addNewRankGamePlayer(rankData);
			iter = mGuidRankGameRankingData.find(playerID);
		}

	}
	else
	{
		if (player != NULL)
		{
			PlayerRankGameData rankGameData = player->getRankGameData();
			iter->mValue.level = player->getPlayerLeval();
			iter->mValue.vipLv = player->getVipLevel();
			iter->mValue.name =  player->getPlayerName().c_str();
			iter->mValue.score =score;
			iter->mValue.time = TimeUtil::getTimeSec();
			iter->mValue.power = player->getPlayerPower();
		}
		else//机器人
		{
			iter->mValue.score = score;
			iter->mValue.time = TimeUtil::getTimeSec();
		}
	}

	changRanking(iter->mValue);	

}

void RankGameManager::getRankGameReward(Guid playerID,UInt32 &index,UInt32 &resIndex,List<Goods> &ends)
{
	UInt32 getIt = 0;
	Goods goods;

	if (index == 0)
	{
		return;
	}
	for(Map<UInt64, RankRewardTemplate>::Iter * iter = RANKREWARD_TABLE().mMap.begin();iter!= NULL;iter = RANKREWARD_TABLE().mMap.next(iter))
	{
		if (index >= iter->mValue.min && index <= iter->mValue.max)
		{
			getIt = 1;		
			resIndex = iter->mKey;
			goods.resourcestype = AWARD_BASE;
			goods.subtype = AWARD_BASE_COIN;
			goods.num = iter->mValue.coin;
			ends.insertTail(goods);
			goods.resourcestype = AWARD_BASE;
			goods.subtype = AWARD_BASE_GOLD;
			goods.num = iter->mValue.gold;
			ends.insertTail(goods);
			goods.resourcestype = AWARD_BASE;
			goods.subtype = AWARD_BASE_COMPETITIVE_POINT;
			goods.num = iter->mValue.score;
			ends.insertTail(goods);			
		}
	}

	if (getIt == 1)
	{	
		
	}
}



void RankGameManager::initPlayerRankGame(Guid playerID)
{
	Player * player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		return;
	}
	SystemInfoTemplate *systemInfoTemplate  = SYSTEMINFO_TABLE().get(SYSTEM_FOOD);
	if (systemInfoTemplate == NULL)
	{
		return;
	}

	if (player->getPlayerLeval() >= systemInfoTemplate->openConditionValue)
	{
	}
	else
	{
		return;
	}


	PlayerRankGameData rankGameData = player->getRankGameData();
	if (rankGameData.m_Time != 0)
	{
		return;
	}

	rankGameData.m_Time = TimeUtil::getTimeSec();
	rankGameData.m_Score = 1000;	
	player->setRankGameData(rankGameData);
	player->getPersistManager().setDirtyBit(RANKGAMEDATABIT);

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();
	dailyResetData.m_nRankGameLeftTimes = globalValue.uRankGameTimes;
	player->setPlayerDailyResetData(dailyResetData);
	player->getPersistManager().setDirtyBit(DAILYRESETBIT);

	changRankGameRanking(playerID,rankGameData.m_Score);
}
void RankGameManager::resetRankGameRewardFlag()
{
	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 dayBeiginTime =  nowTime  - TimeUtil::getSecOfToday();
	UInt32 todayResetTime = dayBeiginTime +globalValue.uRankGameRewardResetTime*3600;

	if (mRankingIndexID.size() == 0)
	{
		return;
	}
// 	now>=9&&time<9
// 	now  <9 && time < 9 -24
	if ( (nowTime >= todayResetTime && serverPublicData.gRankGameRewardresetTime < todayResetTime ) ||
		(nowTime <todayResetTime && serverPublicData.gRankGameRewardresetTime < (todayResetTime - 24*3600)		)	)
	{
		UInt64 lastTime =TimeUtil::getTimeMilliSec();
		saveRankGameScore(0 ,0,0, 0,nowTime,0);
		Player *player;
		for (Map<UInt32,RankData*>::Iter *iter = mRankingIndexID.begin();iter!=NULL;iter = mRankingIndexID.next(iter) )
		{
			player = LogicSystem::getSingleton().getPlayerByGuid(iter->mValue->playerUid);
			if (player != NULL)
			{
				PlayerRankGameData rankGameData = player->getRankGameData();
				rankGameData.m_LastIndex =  iter->mKey;
				rankGameData.m_LastTime = nowTime;
				player->setRankGameData(rankGameData);
			}
		}
		serverPublicData.gRankGameRewardresetTime = nowTime;
		ServerData::getSingleton().setServerPublicData(serverPublicData,true);


	}
	
}

void RankGameManager::addNewRankGamePlayer(RankData rankData)
{
	
	mGuidRankGameRankingData.insert(rankData.playerUid,rankData);
	Map<Guid,RankData>::Iter *iter = mGuidRankGameRankingData.find(rankData.playerUid);
	mRankingIndexID.insert(rankData.index,&iter->mValue);
}


void RankGameManager::rankingInc(UInt32 begin)
{
	for (Map<UInt32,RankData*>::Iter *iter = mRankingIndexID.begin();iter!=NULL;iter = mRankingIndexID.next(iter) )
	{
		if (iter->mKey >= begin )
		{
			iter->mKey = iter->mKey - 1;
			iter->mValue->index = iter->mKey;
		}
	}

}
void RankGameManager::rankingDec(UInt32 begin)
{

	Map<UInt32,RankData*>::Iter *lastIter = mRankingIndexID.getMax();
	for (Map<UInt32,RankData*>::Iter *iter = mRankingIndexID.prev(lastIter);iter!= NULL;iter = mRankingIndexID.prev(iter) )
	{
		if (iter->mKey >= begin)
		{
			iter->mKey = iter->mKey + 1;
			iter->mValue->index = iter->mKey;
		}
	}

}

UInt32 RankGameManager::changRanking(RankData &rankData)
{
	UInt32 nowIndex = 0;
	UInt32 index  = 0;

	index = rankData.index;
	Map<UInt32,RankData*>::Iter * it = mRankingIndexID.find(index);
	if (it != NULL)
	{
		mRankingIndexID.erase(it);
		rankingInc(index);
	}
	else
	{	
	}
	
	for (Map<UInt32,RankData*>::Iter * iter = mRankingIndexID.begin();iter!= NULL;iter = mRankingIndexID.next(iter))
	{
		
		if (iter->mValue->score < rankData.score || (iter->mValue->score == rankData.score&& iter->mValue->time > rankData.time) )
		{
			nowIndex = iter->mKey;
			break;
		}
	}
	//有排名
	if (nowIndex != 0)
	{
		rankingDec(nowIndex);
		rankData.index = nowIndex;
//		item->mValue.index =  nowIndex;
		mRankingIndexID.insert(nowIndex,&rankData);
	}
	else//排名最后
	{
		UInt32 last = 0;
		Map<UInt32,RankData*>::Iter * iter = mRankingIndexID.getMax();
		if (iter != NULL)
		{
			last = iter->mKey + 1;
		}
		else
		{
			last = 0 + 1;
		}
		rankData.index = last;
//		item->mValue.index =  last;
		mRankingIndexID.insert(last,&rankData);

	}
	return 0;

}


void RankGameManager::getRankGamePlayers(Guid playerID, List<Guid> &playerIDs)
{
	UInt32 inv = 0;
	UInt32 index = 0;
	UInt32 j = 0;
	UInt64 ID = 0;
	int rankGameNum[32];
	List<UInt32> indexList;
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	Map<Guid,RankData>::Iter *guidIter;
	Map<UInt32,RankData*>::Iter *  indexIter;

	guidIter = mGuidRankGameRankingData.find(playerID);
	if (guidIter == NULL)
	{
		return;
	}
	UInt32 i = 0;
	if (globalValue.uRankGameNum.size() < 9)
	{
		return;
	}
	for (List<UInt32>::Iter * iter = globalValue.uRankGameNum.begin();iter!=NULL;iter = globalValue.uRankGameNum.next(iter))
	{
		rankGameNum[i] = iter->mValue;
		i++;
	}

// 		当玩家进入前10名时，名次间隔变为1
// 		当玩家到达第3名时，只显示第1、第2名玩家
// 		当玩家到达第2名时，只显示第1名玩家
//uRankGameNum = {5,8,10,1,3,1,2,2,1}

	if (guidIter->mValue.index >rankGameNum[2])
	{
		double tmp = guidIter->mValue.index;
		inv = rankGameNum[0]*sqrt(tmp)/rankGameNum[1];
	}
	else if (guidIter->mValue.index <= rankGameNum[2] && guidIter->mValue.index > rankGameNum[4])
	{

		UInt32 getNum = 5;
		if (guidIter->mValue.index < 6)
		{
			getNum = guidIter->mValue.index - 1;
		}
		inv = rankGameNum[3];

		
		for(j = guidIter->mValue.index-1;j>0;j--)
		{
			indexList.insertTail(j);
		}
		RankGameManager::getSingleton().ChoiseNFromNums(getNum ,indexList);

		FireConfirmManager::getSingleton().ChoiseNFromEnum(getNum,indexList.size(),indexList);
		for (List<UInt32>::Iter * it =indexList.begin();it!=NULL;it= indexList.next(it) )
		{
			indexIter = mRankingIndexID.find(it->mValue);
			if (it==NULL)
			{
				continue;
			}
			playerIDs.insertTail(indexIter->mValue->playerUid);
		}
		
		return;
	}
	else if (guidIter->mValue.index == rankGameNum[4])
	{
	
		indexIter = mRankingIndexID.find(rankGameNum[5]);
		if (indexIter != NULL)
		{
			playerIDs.insertTail(indexIter->mValue->playerUid);
		}		

		indexIter = mRankingIndexID.find(rankGameNum[6]);
		if (indexIter != NULL)
		{
			playerIDs.insertTail(indexIter->mValue->playerUid);
		}	
		return;
	}
	else if (guidIter->mValue.index == rankGameNum[7])
	{
		indexIter = mRankingIndexID.find(rankGameNum[8]);
		if (indexIter != NULL)
		{
			playerIDs.insertTail(indexIter->mValue->playerUid);
		}	
		return;
	}
	else if (guidIter->mValue.index == 1)
	{
		return;
	}

//N-M~N-M*1.1，N-2M~N-2M*1.1，N-3M~N-3M*1.1，N-4M~N-4M*1.1，N-5M~N-5M*1.1

	for (UInt32 ii=1;ii<=5;ii++)
	{
		indexList.clear();
		for(j=( guidIter->mValue.index - ii*inv*1.1);j<=( guidIter->mValue.index - ii*inv);j++)
		{
			indexList.insertTail(j );
		}
		ID = getPriorityID(indexList);	
		indexIter = mRankingIndexID.find(ID);
		if (indexIter != NULL)
		{
			playerIDs.insertTail(indexIter->mValue->playerUid);
		}	
	}


}

UInt32 RankGameManager::getPriorityID(List<UInt32> indexList)
{

	List<UInt64> robotIDList;
	List<UInt64> playerIDList;
	Map<UInt32,RankData*>::Iter *  indexIter;


	for (List<UInt32>::Iter *iter =indexList.begin();iter!=NULL;iter = indexList.next(iter))
	{
		indexIter = mRankingIndexID.find(iter->mValue);

		if (indexIter == NULL)
		{
			continue;
		}

		if (indexIter->mValue->playerUid > RobotMinRoleID && indexIter->mValue->playerUid <RobotMaxRoleID)
		{
			robotIDList.insertTail(iter->mValue);
		}
		else
		{
			playerIDList.insertTail(iter->mValue);
		}
	}
	if (playerIDList.size()!=0)
	{
		UInt32 count = rand()% playerIDList.size();
		UInt32 i = 0;
		for (List<UInt64>::Iter *IDIter = playerIDList.begin();IDIter!=NULL;IDIter= playerIDList.next(IDIter))
		{
			if (i == count)
			{
				return IDIter->mValue;
			}
			i++;		
		}
	}

	if (robotIDList.size()!=0)
	{
		UInt32 count = rand()% robotIDList.size();
		UInt32 i = 0;
		for (List<UInt64>::Iter *IDIter = robotIDList.begin();IDIter!=NULL;IDIter= robotIDList.next(IDIter))
		{
			if (i == count)
			{
				return IDIter->mValue;
			}
			i++;
		}
	
	}

	return 0;	

}



//refresh 0获取信息 1刷新 2 时间到刷新
//refreshTimes 第几个

UInt32 RankGameManager::getRankGameShopInfo(Guid playerID,UInt32 refresh,RankGameShopResp &resp)
{
	UInt32 acID = 0;
	UInt32 level = 0;
	UInt32 vipLevel = 0;
	UInt32 refreshTimes = 0;
	double cost = 0;
	UInt32 getIt = 0;
	UInt32 refreshTime = 0;
	UInt32 refreshMaxTimes = 0;
	UInt32 j = 0;
	Goods goods;
	List<Goods> addGoodsList;
	List<UInt32> awardcontentIDs;
	List<Goods> goodsList;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return LynxErrno::NotFound;
	}
	UInt32 refreshNeedTime;

	UInt32 flag = checkRankGameRefreshTime(playerID,RankGameShop,refreshNeedTime);

	refreshTimes = flag;
	if(refresh == 1)
	{
		refreshTimes ++;
	}
	else
	{
		if (flag == 10000)
		{
			refresh = 2;
			refreshTimes = 0;			 
		}
	}

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	refreshMaxTimes = globalValue.uRankGameShopRefreshTimes;

	if (refreshTimes > refreshMaxTimes)
	{
		refreshTimes = refreshMaxTimes;
		return LynxErrno::RefreshTimesNotEnough;
	}
	resp.resetTimes = refreshTimes;
	if (refresh != 0)
	{
		refreshTime = TimeUtil::getTimeSec();
		ShopManager::getSingleton().setShopItems(playerID,RankGameShop,refreshTimes,refreshTime,true);
	}	

	if (refresh ==1 )
	{
		cost = globalValue.uRankGameShopRefreshCost;
		if (player->getPlayerGold() < cost)
		{
			return LynxErrno::RmbNotEnough;
		}

		goods.resourcestype = AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num = 0 - cost;
		addGoodsList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),1,addGoodsList,MiniLog114);
	}

	UInt32 max = gRankedShopTable->mMap.size();
	for (UInt32 i = 1;i <= max;i++)
	{
		acID = 0;
		if (refresh != 0)
		{
			acID = i;
			// 			acID = getRankGameShopID(i,player->getPlayerLeval(),player->getVipLevel(),refreshTimes,refresh);
		}

		Item item;
		RankGameManager::getSingleton().getRankGameShopNewItems(playerID, i,acID,RankGameShop,item);

		if (acID != 0)
		{
			item.buyTimes = 0;
			ShopManager::getSingleton().setShopItem(playerID,RankGameShop,i,item,true);
		}
		if (item.goods.subtype != 0)
		{
			resp.items.insertTail(item);
		}
	}
// 	resp.refreshCost = globalValue.uTERRITORYLrmbrefresh;
	return LynxErrno::None;
}



UInt32 RankGameManager::checkRankGameRefreshTime(Guid playerID,UInt32 shopType,UInt32 &refreshNeedTime)
{
	UInt32 lastRefreshTime = 0;
	UInt32 nextRefreshTime = 0;
	UInt32 refreshTime = 0;
	UInt32 refreshTimes = 0;
	UInt32 tmp = TimeUtil::getSecOfToday();
	UInt32 dayBeiginTime =  TimeUtil::getTimeSec() - TimeUtil::getSecOfToday();
	Vector<String> strVector;
	Item item;


	UInt32 nowTime = TimeUtil::getTimeSec();
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return 0;
	}

	ShopManager::getSingleton().getPlayerItem( playerID,shopType, 0,item,refreshTime,refreshTimes);

// 	if (shopType == RankGameShop)
// 	{
// 		RankedShopTemplate *rankGameShopTemplate;//只是为了找时间，其他用途则要重新赋值
// 		for ( Map<UInt32, RankedShopTemplate>::Iter *iter = gRankedShopTable->mMap.begin();iter != NULL;iter = gRankedShopTable->mMap.next(iter))
// 		{
// 			if (iter->mValue.id == 1)
// 			{
// 				rankGameShopTemplate = &iter->mValue;
// 				break;
// 			}
// 		}
// 		lynxStrSplit(rankGameShopTemplate->refreshTime, ";", strVector, true);
// 	}

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();

	char dest[64] ={0};
	sprintf(dest,"%llu",globalValue.uRankGameShopRefreshTime);


	lynxStrSplit(dest, ";", strVector, true);

	UInt32 getIt = 0;

	for (UInt32 i = 0; i < strVector.size();i++)
	{
		if (nowTime < (dayBeiginTime + atoi(strVector[i].c_str()) * 3600 ) )
		{
			nextRefreshTime = (dayBeiginTime + atoi(strVector[i].c_str()) * 3600 );
			if (i == 0)
			{
				lastRefreshTime = dayBeiginTime - 24*3600 + atoi(strVector[strVector.size() -1].c_str())* 3600;//昨天最后一个
			}
			else
			{
				lastRefreshTime = dayBeiginTime + atoi(strVector[i -1].c_str())* 3600;

			}
			getIt = 1;
			break;			
		}		
	}
	if (getIt == 0)
	{
		nextRefreshTime = (dayBeiginTime +  (24+atoi(strVector[0].c_str())) * 3600 );//明天第一次
		lastRefreshTime = dayBeiginTime  + atoi(strVector[strVector.size() -1].c_str())* 3600;//今天最后一次刷新
	}
	refreshNeedTime = nextRefreshTime - nowTime;

	if(refreshTime < lastRefreshTime )
	{
		return 10000;		
	}

	return refreshTimes;
}


void RankGameManager::onRankGameShopBuyReq(const  ConnId& connId, RankGameShopBuyReq & msg)
{
	UInt32 cost = 0;
	RankGameShopBuyResp resp;
	msg.convertJsonToData(msg.strReceive);
	resp.position = msg.position;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}

	//先刷新	
	RankGameShopReq  rankGameShopReq;
	rankGameShopReq.reqType = 0;
	rankGameShopReq.needSend = false;
	RankGameManager::getSingleton().onRankGameShopReq(connId,rankGameShopReq);
	
	resp.result = RankGameManager::getSingleton().dealRankGameShopbuy(player->getPlayerGuid(),RankGameShop,msg.position,resp);

	PlayerRankGameData rankGameData = player->getRankGameData();

	resp.maskNum = rankGameData.m_MaskNum;
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,RANKGAME_SHOPBUY_RESP,jsonStr);
}



void RankGameManager::onRankGameBuyTimesReq(const  ConnId& connId, RankGameBuyTimesReq & msg)
{
	UInt32 cost = 0;
	RankGameBuyTimesResp resp;

	msg.convertJsonToData(msg.strReceive);

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (player == NULL)
	{
		return;
	}
	PlayerRankGameData rankGameData = player->getRankGameData();

	PlayerDailyResetData dailyResetData = player->getPlayerDailyResetData();

	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();


	 UInt32 j = 0;
	 for(List<UInt32>::Iter *iter = globalValue.uRankGameBuyTimesCost.begin();iter!= NULL;iter = globalValue.uRankGameBuyTimesCost.next(iter))
	{
		j++;
		cost = iter->mValue;
		if (j == (dailyResetData.m_nRankGameBuyTimes+1))
		{			
			break;
		}
	}
	 if (player->getPlayerGold()< cost)
	 {
		 resp.result = LynxErrno::RmbNotEnough;
	 }

	if ( resp.result == LynxErrno::None)
	{
		Goods goods;
		List<Goods> itemList;
		goods.resourcestype = AWARD_BASE;
		goods.subtype = AWARD_BASE_GOLD;
		goods.num = 0 - cost;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog110);

		dailyResetData.m_nRankGameBuyTimes ++;
		dailyResetData.m_nRankGameLeftTimes ++;
		player->setPlayerDailyResetData(dailyResetData);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);
	}

	resp.buyTimes = dailyResetData.m_nRankGameBuyTimes;
	resp.gold = player->getPlayerGold();
	resp.restChalTimes  = dailyResetData.m_nRankGameLeftTimes;
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,RANKGAME_BUYTIMES_RESP,jsonStr);
}


void RankGameManager::getRankGameShopNewItems(Guid playerID, UInt32 position,UInt32 acID,UInt32 shopType,Item &item)
{
	UInt32 refreshTime;
	UInt32 refreshTimes;
	UInt32 nowTime = TimeUtil::getTimeSec();
	UInt32 dayBeiginTime =  TimeUtil::getTimeSec() - TimeUtil::getSecOfToday();
	List<Goods> goodsList;
	RankedShopTemplate *rankedShopTemplate;
	rankedShopTemplate =  gRankedShopTable->get(position);
	if (rankedShopTemplate == NULL)
	{
		return;
	}

	ShopManager::getSingleton().getPlayerItem( playerID,shopType, position,item,refreshTime,refreshTimes);

	if (item.goods.subtype == 0)
	{
		item.itemID = acID;
		item.position = position;
		item.shopType = shopType;
		item.buyTime = 0;
		item.buyTimes = 0;

		item.goods.resourcestype = rankedShopTemplate->resType;
		item.goods.subtype = rankedShopTemplate->subType;
		item.goods.num = rankedShopTemplate->num;

		item.cost = rankedShopTemplate->costnum;

		ShopManager::getSingleton().setShopItem(playerID,shopType,position,item,true);
	}
	else
	{
		item.cost = rankedShopTemplate->costnum;
	}

}



UInt32 RankGameManager::dealRankGameShopbuy(Guid playerID,UInt32 shopType,UInt32 position,RankGameShopBuyResp &resp)
{
	Goods goods;
	List<Goods> itemList;
	RankedShopTemplate *rankedShopTemplate;

	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return LynxErrno::NotFound;
	}

	PlayerRankGameData rankGameData = player->getRankGameData();

	ShopItems  shopItems = ShopManager::getSingleton().getShopItems( playerID,  shopType);

	Item  item = ShopManager::getSingleton().getShopItem( playerID, shopType, position);

	if(item.position == 0)
	{
		return LynxErrno::NotFound;
	}

	if (item.buyTimes > 0)
	{
		return LynxErrno::HadBuy;
	}

	rankedShopTemplate =  gRankedShopTable->get(position);
	if (rankedShopTemplate == NULL)
	{
		return LynxErrno::NotFound;
	}

	if (rankedShopTemplate->costsubType == AWARD_BASE_COIN)
	{
		if (player->getPlayerCoin() < rankedShopTemplate->costnum)
		{
			return LynxErrno::CoinNotEnough;
		}
	}
	if (rankedShopTemplate->costsubType == AWARD_BASE_GOLD)
	{
		if (player->getPlayerGold() < rankedShopTemplate->costnum)
		{
			return LynxErrno::RmbNotEnough;
		}
	}
	if (rankedShopTemplate->costsubType == AWARD_BASE_COMPETITIVE_POINT)
	{
		if (rankGameData.m_MaskNum < rankedShopTemplate->costnum)
		{
			return LynxErrno::MaskNotEnough;
		}
	}
	



	item.buyTimes += item.goods.num;
	ShopManager::getSingleton().setShopItem(playerID,shopType,position,item,true);

	goods.resourcestype = rankedShopTemplate->costresType;
	goods.subtype = rankedShopTemplate->costsubType;
	goods.num = 0 - rankedShopTemplate->costnum;	
	itemList.insertTail(goods);
	goods.resourcestype = rankedShopTemplate->resType;
	goods.subtype = rankedShopTemplate->subType;
	goods.num = rankedShopTemplate->num;	
	itemList.insertTail(goods);

	GiftManager::getSingleton().saveEndsGetAttr(player->getPlayerGuid(),itemList,resp.allAttr,MiniLog111);
	resp.ends = itemList;

	return LynxErrno::None;
}

Map<UInt32,RankData*>RankGameManager::getRankingIndexID()
{
	return mRankingIndexID;

}

bool RankGameManager::checkSystemOpen(Guid playerID,UInt32 systemID)
{

	SystemInfoTemplate *systemInfoTemplate;
	systemInfoTemplate = SYSTEMINFO_TABLE().get(systemID);
	if (systemInfoTemplate == NULL)
	{
		return false;
	}
	Player * player  = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player == NULL)
	{
		return false;
	}
	switch(systemInfoTemplate->openConditionType)
	{
	case	  1 ://20170208发现修改了
		{
			if (player->getPlayerLeval() <systemInfoTemplate->openConditionValue)
			{
				return false;
			}
			
			return true;
		}

		break;


	default:
		break;
	}

	
	return true;

}

void RankGameManager::getNullPlayerID()
{
	std::string str;
	String playerUidStr;
	String detailStr;
	char dest[64] = {};
	char dest1[64] = {};
	char* dst = NULL;


	char tmp[8];
	std::string outputStr = "";

	List<UInt64> playerids;
	for(Map<UInt32,RankData*>::Iter * iter = mRankingIndexID.begin();iter!= NULL;iter = mRankingIndexID.next(iter))
	{
		if (iter->mValue->playerUid < RobotMaxRoleID)
		{
			continue;
		}
		sprintf(dest,"%llu",iter->mValue->playerUid);
		playerUidStr = (String)dest;
		str = LogicSystem::getSingleton().getDetailInfo(playerUidStr.c_str());
		if (str == "")
		{
			playerids.insertTail(iter->mValue->playerUid);

			LogicSystem::getSingleton().initRoleInfoReq(iter->mValue->playerUid,iter->mValue->name.c_str(),iter->mValue->modelId);	

		}
	}

	for(List<UInt64>::Iter *it = playerids.begin();it != NULL;it = playerids.next(it))
	{
		snprintf(tmp, sizeof(tmp),"%llu",it->mValue);
		outputStr += (std::string)tmp + ",";
	}

	LOG_INFO("getDetailInfo null is  %s",outputStr.c_str());


}


void RankGameManager::ChoiseNFromNums(UInt32 num ,List<UInt32> &outList)
{

	UInt32 randomnum;
	UInt32 maxnum = outList.size();
	char Arry[32];
	List<UInt32> getList;


	getList =  outList;
	outList.clear();

	for (int i =0;i< maxnum; i++)
	{
		Arry[i] = '0';
	}

	if (num<1)
	{
		return ;
	}
	if (num > maxnum)
	{
		return ;
	}
	UInt32 ii =0;
	do
	{
		randomnum = rand()%(maxnum);
		if (Arry[randomnum] == '0')
		{
			Arry[randomnum] = '1';
			ii = 0;
			for(List<UInt32>::Iter * iter = getList.begin();iter !=NULL;iter = getList.next(iter))
			{
				if (ii == randomnum)
				{
					outList.insertTail(iter->mValue);
				}
				ii++;
			}
		}
	}while ( outList.size() < num);
	return ;
}

void RankGameManager::getRandomOrderNums(UInt32 num ,List<UInt32> &outList)
{
	UInt32 maxnum = num;	
	UInt32 randomnum1 = 0;
	UInt32 randomnum2 = 0;
	UInt32 tmp = 0;
	UInt32 Arry[1024] = {0};



	for (int i =0;i< maxnum; i++)
	{
		Arry[i] = i;
	}

	for (int i =0;i< maxnum*10; i++)
	{
		randomnum1 = rand()%(maxnum);
		randomnum2 = rand()%(maxnum);

		tmp = Arry[randomnum1];
		Arry[randomnum1] = Arry[randomnum2];
		Arry[randomnum2] = tmp;
	}

	for (int i =0;i< maxnum; i++)
	{
		outList.insertTail(Arry[i]);
	}


}