#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_RANKGAME_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_RANKGAME_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "FireConfirmManager.h"


namespace Lynx
{

	struct RankData
	{
		RankData():index(0),playerUid(0),modelId(0), score(0), name(""), level(0),vipLv(0), time(0), power(0){}

		UInt32 index;
		UInt64 playerUid;
		UInt32 modelId;
		UInt32 score;
		String name;
		UInt32 level;
		UInt32 vipLv;
		UInt32 time;
		UInt32 power;

		LYNX_S11N_9(RankData, UInt32, index,UInt64 ,playerUid,
			UInt32, modelId,UInt32, score,String, name, UInt32, level, UInt32, vipLv, UInt32 ,time,UInt32, power);
	};

	struct RankGameReq
	{
		RankGameReq(): roleId(0),needSend(true),isLogin(false){}

		UInt64 roleId;//1 获取信息类型 2购买次数
		bool needSend;
		bool isLogin;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				roleId = root["roleId"].asUInt64();
			}
		}
		LYNX_MESSAGE_1(RANKGAME_INFO_REQ , RankGameReq,std::string, strReceive);
	};


	struct RankGameResp
	{
		RankGameResp(): restChalTimes(0),result(0),maskResetTimes(0),curRank(0),playerScore(0),sessions(0),winSessions(0),maskNum(0),buyTimes(0),lastIndex(0),resIndex(0),newReportFlag(0){}

		UInt32 result;
		UInt32 restChalTimes;
		UInt32 maskResetTimes;		
		UInt32 curRank;
		UInt32 playerScore;
		UInt32 sessions;
		UInt32 winSessions;

		UInt32 maskNum;
		UInt32 buyTimes;
		UInt32 lastIndex;
		UInt32 resIndex;
		UInt32 newReportFlag;
		Json::Value allAttr;
		List<Goods> ends;
		List<RankData*> otherRoleInfo;


		std::string convertDataToJson()
		{
			Json::Value root;   
			root["result"] = Json::Value(result);
			root["restChalTimes"] = Json::Value(restChalTimes);
			root["maskResetTimes"] = Json::Value(maskResetTimes);
			root["curRank"] = Json::Value(curRank);
			root["playerScore"] = Json::Value(playerScore);
			root["sessions"] = Json::Value(sessions);
			root["winSessions"] = Json::Value(winSessions);
			root["maskNum"] = Json::Value(maskNum);
			root["buyTimes"] = Json::Value(buyTimes);
			root["lastIndex"] = Json::Value(lastIndex);
			root["resIndex"] = Json::Value(resIndex);
			root["newReportFlag"] = Json::Value(newReportFlag);
			for (List<RankData*>::Iter *iter= otherRoleInfo.begin();iter!= NULL;iter = otherRoleInfo.next(iter))
			{
				Json::Value son;
				son["roleId"] = iter->mValue->playerUid;
				son["level"] = iter->mValue->level;
				son["vipLevel"] = iter->mValue->vipLv;
				son["name"] = iter->mValue->name.c_str();
				son["score"] = iter->mValue->score;
				son["power"] = iter->mValue->power;
				son["modelId"] = iter->mValue->modelId;
				son["index"] = iter->mValue->index;

				root["otherRoleInfo"].append(son);
			}
			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["ends"].append(son);
			}
			root["allAttr"] = Json::Value(allAttr);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct RankGameRankingReq
	{
// 		RankGameRankingReq():{}

		List<UInt64> playerIDs;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				for (UInt32 i = 0;i<root["playerIDs"].size();i++)
				{
					playerIDs.insertTail(root["playerIDs"][i].asUInt64());
				}
			}
		}
		LYNX_MESSAGE_1(RANKGAME_RANKING_REQ, RankGameRankingReq,std::string, strReceive);
	};


	struct RankGameRankingResp
	{
		RankGameRankingResp(): flag(0){}
		
		UInt32 flag;
		List<RankData>otherRoleInfo;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqtype"] = Json::Value(flag);
			for (List<RankData>::Iter *iter= otherRoleInfo.begin();iter!= NULL;iter = otherRoleInfo.next(iter))
			{
				Json::Value son;
				son.append(iter->mValue.playerUid);
				son.append(iter->mValue.level);
				son.append(iter->mValue.vipLv);
				son.append( iter->mValue.name.c_str());
				son.append(iter->mValue.score);
				son.append(iter->mValue.power);
				son.append(iter->mValue.modelId);
				son.append(iter->mValue.index);
			
				root["otherRoleInfo"].append(son);
			}	

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct RankGameReportReq
	{
		RankGameReportReq(): flag(0){}

		UInt32 flag ;//1 获取信息类型 2购买次数

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				flag  = root["flag"].asUInt();
			}
		}
		LYNX_MESSAGE_1(RANKGAME_REPORT_REQ, RankGameReportReq,std::string, strReceive);
	};


	struct RankGameReportResp
	{
		RankGameReportResp(): flag(0){}

		UInt32 flag;
		List<ReportData> reportData;
		
		std::string convertDataToJson()
		{
			Json::Value root;   
			root["flag"] = Json::Value(flag);

			for (List<ReportData>::Iter *iter= reportData.begin();iter!= NULL;iter = reportData.next(iter))
			{
				Json::Value son;
				son["id"] = iter->mValue.id;
				son["roleId"] = iter->mValue.roleId;
				son["modelId"] = iter->mValue.modelID;
				son["score"] = iter->mValue.score;
				son["name"] = iter->mValue.name.c_str();
				son["level"] = iter->mValue.level;
				son["vipLv"] = iter->mValue.vipLv;
				son["happenTime"] = iter->mValue.happenTime;
				son["power"] = iter->mValue.power;
				son["attackType"] = iter->mValue.attackType;
				son["rank"] = iter->mValue.rank;
				son["flag"] = iter->mValue.flag;
				
				root["reportData"].append(son);
			}	
			
			

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct RankGameShopReq
	{
		RankGameShopReq(): reqType(0),needSend(true){}

		UInt32 reqType;
		bool needSend;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
			}
		}
		LYNX_MESSAGE_1(RANKGAME_SHOP_REQ, RankGameShopReq,std::string, strReceive);
	};


	struct RankGameShopResp
	{
		RankGameShopResp(): reqType(0),result(0),resetTimes(0),gold(0){}

		UInt32 reqType;
		UInt32 result;		
		UInt32 resetTimes;
		UInt32 gold;	
		List<Item> items;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["resetTimes"] = Json::Value(resetTimes);
			root["gold"] = Json::Value(gold);

			for (List<Item>::Iter *iter= items.begin();iter!= NULL;iter = items.next(iter))
			{
				Json::Value son;
				son["position"] = iter->mValue.position;
				son["buyTimes"] = iter->mValue.buyTimes;
				
				root["items"].append(son);
			}	
			
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct RankGameShopBuyReq
	{
		RankGameShopBuyReq(): position(0){}

		UInt32 position;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				position = root["position"].asUInt();
			}
		}
		LYNX_MESSAGE_1(RANKGAME_SHOPBUY_REQ, RankGameShopBuyReq,std::string, strReceive);
	};


	struct RankGameShopBuyResp
	{
		RankGameShopBuyResp(): position(0),result(0),maskNum(0){}

		UInt32 position;
		UInt32 result;		
		UInt32 maskNum;
		Json::Value allAttr;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["position"] = Json::Value(position);
			root["result"] = Json::Value(result);
			root["maskNum"] = Json::Value(maskNum);

			for (List<Goods>::Iter *iter= ends.begin();iter!= NULL;iter = ends.next(iter))
			{
				Json::Value son;
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);

				root["ends"].append(son);
			}	

			root["allAttr"] = Json::Value(allAttr);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	

	struct RankGameBeginReq
	{
		RankGameBeginReq(): playerId(0){}

		UInt64 playerId;
		

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				playerId = root["playerId"].asUInt64();
			}
		}
		LYNX_MESSAGE_1(RANKGAME_BEGIN_REQ, RankGameBeginReq,std::string, strReceive);
	};


	struct RankGameBeginResp
	{
		RankGameBeginResp(): chalTimes(0),result(0),isRobot(0),otherPlayer(""){}

		UInt32 chalTimes;
		UInt32 result;	
		UInt32 isRobot;
		String otherPlayer;
		List<UInt32> confirmIDs;
		

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["chalTimes"] = Json::Value(chalTimes);
			root["result"] = Json::Value(result);
			root["isRobot"] = Json::Value(isRobot);
			root["otherPlayer"] = Json::Value(otherPlayer.c_str());
			for(List<UInt32>::Iter * iter =  confirmIDs.begin();iter !=NULL;iter = confirmIDs.next(iter))
			{
				root["confirmIDs"].append(iter->mValue);

			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct RankGameEndReq
	{
		RankGameEndReq(): result(0), needSend(true){}

		UInt32 result;
		bool needSend;
		List<FireConfirm> fireConfirmData;

		std::string strReceive;
	
		LYNX_MESSAGE_1(RANKGAME_END_REQ, RankGameEndReq,std::string, strReceive);
	};


	struct RankGameEndResp
	{
		RankGameEndResp(): score(0),result(0),index(0),lastIndex(0),flag(0){}

		UInt32 score;
		UInt32 result;
		UInt32 index;
		UInt32 lastIndex;
		UInt32 flag;
		int changScore;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["score"] = Json::Value(score);
			root["result"] = Json::Value(result);
			root["index"] = Json::Value(index);
			root["lastIndex"] = Json::Value(lastIndex);
			root["flag"] = Json::Value(flag);
			root["changScore"] = Json::Value(changScore);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct RankGameBuyTimesReq
	{
		RankGameBuyTimesReq(): roleId(0){}

		UInt64 roleId;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				roleId = root["roleId"].asUInt64();
			}
		}
		LYNX_MESSAGE_1(RANKGAME_BUYTIMES_REQ, RankGameBuyTimesReq,std::string, strReceive);
	};


	struct RankGameBuyTimesResp
	{
		RankGameBuyTimesResp(): buyTimes(0),result(0),restChalTimes(0),gold(0){}

		UInt32 buyTimes;
		UInt32 result;
		UInt32 restChalTimes;
		UInt32 gold;


		std::string convertDataToJson()
		{
			Json::Value root;   
			root["buyTimes"] = Json::Value(buyTimes);
			root["result"] = Json::Value(result);
			root["restChalTimes"] = Json::Value(restChalTimes);
			root["gold"] = Json::Value(gold);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	class RankGameManager:public Singleton<RankGameManager>
	{
	
	public:
		void initRankGame();

		void changRankGameRanking(Guid playerID,UInt32 score);
		void saveRankGameScore(Guid playerID,UInt32 score,UInt32 time, UInt32 lastIndex,UInt32 lastTime,UInt32 addWinCount);
		void addNewRankGamePlayer(RankData rankData);
		void rankingDec(UInt32 begin);
		void rankingInc(UInt32 begin);
		UInt32 changRanking(RankData &rankData);
		void getRankGamePlayers(Guid playerID, List<Guid> &playerIDs);

		void getRankGameReward(Guid playerID,UInt32 &index,UInt32 &resIndex,List<Goods> &ends);
		void getRankGameCanChallage(Guid playerID, String& playerIds,List<RankData*> &otherRoleInfo);

		void initPlayerRankGame(Guid playerID);
		void resetRankGameRewardFlag();
		void addRankGameReport(Guid playerID,ReportData reportData,UInt32 &id);

		Map<UInt32,RankData*> getRankingIndexID();
		bool checkSystemOpen(Guid playerID,UInt32 systemID);


		static void onRankGameReq(const  ConnId&,RankGameReq & );
		static void onRankGameRankingReq(const  ConnId& ,RankGameRankingReq & );
		static void onRankGameReportReq(const  ConnId& ,RankGameReportReq & );
		static void onRankGameShopReq(const  ConnId& ,RankGameShopReq & );
		static void onRankGameShopBuyReq(const  ConnId& ,RankGameShopBuyReq & );
		static void onRankGameBeginReq(const  ConnId& ,RankGameBeginReq & );
		static void onRankGameEndReq(const  ConnId& ,RankGameEndReq & );
		static void onRankGameBuyTimesReq(const  ConnId& ,RankGameBuyTimesReq & );

		UInt32 getRankGameShopInfo(Guid playerID,UInt32 refresh,RankGameShopResp &resp);
		UInt32 checkRankGameRefreshTime(Guid playerID,UInt32 shopType,UInt32 &refreshNeedTime);
		void getRankGameShopNewItems(Guid playerID, UInt32 position,UInt32 acID,UInt32 shopType,Item &item);
		UInt32 dealRankGameShopbuy(Guid playerID,UInt32 shopType,UInt32 position,RankGameShopBuyResp &resp);


		void checkScoreLowest(UInt64 playerID,UInt32 &score);
		UInt32 getPriorityID(List<UInt32> indexList);

		void getNullPlayerID();

		void ChoiseNFromNums(UInt32 num ,List<UInt32> &outList);

		void getRandomOrderNums(UInt32 num ,List<UInt32> &outList);



		
	};

	static Map<Guid,RankData> mGuidRankGameRankingData;
	static Map<UInt32,RankData*> mRankingIndexID;

}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_RANKGAME_MANAGER_H__
