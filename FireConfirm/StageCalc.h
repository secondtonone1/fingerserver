#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_STAGE_CALC_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_STAGE_CALC_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "../PersistSystem.h"

namespace Lynx
{
typedef Map<UInt32,UInt32> ResetStagesMap;
	struct ResetStages 
	{
		ResetStagesMap resetStages;
		LYNX_S11N_1(ResetStages,ResetStagesMap, resetStages);
	};

	

	struct CGTwelvePalaceUnlock
	{
		CGTwelvePalaceUnlock(): id(0),doType(0)/*,star(0)*/{}

		UInt32 id;
		UInt32 doType;
// 		UInt32 star;//普通关卡为星数，多人副本为完成时间
// 		List<AwardMonsterDamage> awardMonsterList;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				id = root["id"].asUInt();
				doType = root["doType"].asUInt();
// 				star = root["star"].asUInt();

// 				AwardMonsterDamage awardMonsterDamage;
// 				for(int i =0; i <root["confirmDataList"].size();i++)
// 				{
// 					awardMonsterDamage.ID  = root["awardMonsterList"][i]["ID"].asUInt();
// 					awardMonsterDamage.damageType  = root["awardMonsterList"][i]["damageType"].asUInt();
// 					awardMonsterDamage.times  = root["awardMonsterList"][i]["times"].asUInt();
// 					awardMonsterList.insertTail(awardMonsterDamage);
// 				}
			}
		}
		LYNX_MESSAGE_1(TWELVE_PALACE_UNLOCK_REQ, CGTwelvePalaceUnlock,std::string, jsonStr);
	};

// 	struct CGTwelvePalaceUnlock
// 	{
// 		CGTwelvePalaceUnlock(): id(0),doType(0){}
// 
// 		UInt32 id;
// 		UInt32 doType;//1解锁2重置次数
// 		std::string jsonStr;
// 		void convertJsonToData(std::string jsonStr)
// 		{
// 			Json::Reader reader;    
// 			Json::Value root;    
// 			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
// 			{
// 				id = root["id"].asUInt();
// 				doType = root["doType"].asUInt();
// 			}
// 		}
// 
// 		LYNX_MESSAGE_1(TWELVE_PALACE_UNLOCK_REQ, CGTwelvePalaceUnlock,std::string, jsonStr);
// 	};
	struct TwelvePalaceUnlockResp	
	{
		TwelvePalaceUnlockResp(): id(0),doType(0),num(0),result(0),gold(0),buyTimes(0){}

		UInt32 id;
		UInt32 doType;
		UInt32 num ;//号角数量
		UInt32 result;
		UInt32 gold;
		UInt32 buyTimes;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["id"] = Json::Value(id);
			root["doType"] = Json::Value(doType);
			root["num"] = Json::Value(num);
			root["result"] = Json::Value(result);
			root["gold"] = Json::Value(gold);
			root["buyTimes"] = Json::Value(buyTimes);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

	};




	struct CGTwelvePalaceStart
	{
		CGTwelvePalaceStart(): id(0),awardType(0){}

		UInt32 id;
		UInt32 awardType;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				id = root["id"].asUInt();
				awardType = root["awardType"].asUInt();
			}
		}

		LYNX_MESSAGE_1(TWELVE_PALACE_BEGIN_REQ	, CGTwelvePalaceStart,std::string, jsonStr);
	};
	struct TwelvePalaceStartResp	
	{
		TwelvePalaceStartResp(): id(0),result(0),awardType(0),num(0),otherPlayerID(0),isRobot(0),otherPlayer (""){}

		UInt32 id;
		UInt32 awardType;
		UInt32 num ;
		UInt32 result;
		Guid otherPlayerID;
		UInt32 isRobot;//1是机器人
		std::string otherPlayer;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["id"] = Json::Value(id);
			root["awardType"] = Json::Value(awardType);
			root["num"] = Json::Value(num);
			root["result"] = Json::Value(result);
			root["otherPlayerID"] = Json::Value(otherPlayerID);
			root["isRobot"] = Json::Value(isRobot);
			root["otherPlayer"] = otherPlayer;
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

	};



	struct CGTwelvePalaceEnd
	{
		CGTwelvePalaceEnd(): id(0),star(0),time(0),doType(0),result(0),mopupTimes(0){}
		UInt32 id;//关卡ID
		UInt32 time;
		UInt32 star;		
		UInt32 doType ;//0扫荡1完成挑战2挑战失败
		UInt32 result;
		UInt32 mopupTimes;
		List<AwardMonsterDamage> awardMonsterList;
		List<UInt32> awardTypes;
// 		List<UInt32> useList;//食物1，食物2，食物3，精英



		

		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{		
				 
				id = root["id"].asUInt();	
				time = root["time"].asUInt();	
				star = root["star"].asUInt();	
				doType  = root["doType"].asUInt();	
				result = root["result"].asUInt();
				mopupTimes = root["mopupTimes"].asUInt();	

				for(UInt32 i=0;i<root["awardTypes"].size();i++)
				{
					awardTypes.insertTail(root["awardTypes"][i].asUInt());
				}
				AwardMonsterDamage awardMonsterDamage;
				for(int i =0; i <root["confirmDataList"].size();i++)
				{
					awardMonsterDamage.ID  = root["awardMonsterList"][i]["ID"].asUInt();
					awardMonsterDamage.damageType  = root["awardMonsterList"][i]["damageType"].asUInt();
					awardMonsterDamage.times  = root["awardMonsterList"][i]["times"].asUInt();
					awardMonsterList.insertTail(awardMonsterDamage);
				}
			}			
		}
		LYNX_MESSAGE_1(TWELVE_PALACE_END_REQ,CGTwelvePalaceEnd,std::string, jsonStr);
	};
	
	struct TwelvePalaceEndResp
	{
		TwelvePalaceEndResp(): id(0),doType(0),result(0),time(0),star(0),recordBreaking(0),recordPlayerID(0),recordName(""),recordModleID(0),recordTime(0),mopupTimes(0){}


		UInt32 id;			
		UInt32 doType;
		UInt32 result;
		UInt32 time;
		UInt32 star;
		UInt32 recordBreaking;

		Guid recordPlayerID;
		std::string recordName;
		UInt32 recordModleID;
		UInt32 recordTime;
		UInt32 mopupTimes;


		Json::Value allAttr;//发送物品属性
		List<Award> firstAwards;
		List<Award> awards;
		List<Goods> cost;
		List<Card> cards;
		List<Goods> monsterDropList;
		List<Goods> fixedList;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["id"] = Json::Value(id );
			root["doType"] = Json::Value(doType);
			root["result"] = Json::Value(result );		
			root["time"] = Json::Value(time);
			root["star"] = Json::Value(star);
			root["recordBreaking"] = Json::Value(recordBreaking);

			root["recordPlayerID"] = Json::Value(recordPlayerID);
			root["recordName"] = Json::Value(recordName);
			root["recordModleID"] = Json::Value(recordModleID);
			root["recordTime"] = Json::Value(recordTime);
			root["mopupTimes"] = Json::Value(mopupTimes);


			for(List<Award>::Iter * it = awards.begin(); it != NULL; it = awards.next(it))			
			{
				Json::Value son;
				for(List<Goods>::Iter * iter = it->mValue.award.begin(); iter != NULL; iter = it->mValue.award.next(iter))
				{
					Json::Value leaf;
					leaf.append(iter->mValue.resourcestype);
					leaf.append(iter->mValue.subtype);
					leaf.append(iter->mValue.num);

					son["award"].append(leaf);
				}
				root["awards"].append(son);
			}

			for(List<Award>::Iter * it = firstAwards.begin(); it != NULL; it = firstAwards.next(it))			
			{
				Json::Value son;
				for(List<Goods>::Iter * iter = it->mValue.award.begin(); iter != NULL; iter = it->mValue.award.next(iter))
				{
					Json::Value leaf;
					leaf.append(iter->mValue.resourcestype);
					leaf.append(iter->mValue.subtype);
					leaf.append(iter->mValue.num);

					son["award"].append(leaf);
				}
				root["firstAwards"].append(son);
			}

			for(List<Card>::Iter * it = cards.begin(); it != NULL; it = cards.next(it))			
			{
				Json::Value son;
				for(List<Goods>::Iter * iter = it->mValue.card.begin(); iter != NULL; iter = it->mValue.card.next(iter))
				{
					Json::Value leaf;
					leaf.append(iter->mValue.resourcestype);
					leaf.append(iter->mValue.subtype);
					leaf.append(iter->mValue.num);
					son["card"].append(leaf);
				}
				root["cards"].append(son);
			}		

			for(List<Goods>::Iter * iter = monsterDropList.begin(); iter != NULL; iter = monsterDropList.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["monsterDropList"].append(son);
			}

			for(List<Goods>::Iter * iter = cost.begin(); iter != NULL; iter = cost.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["cost"].append(son);
			}

// 			for (List<UInt32>::Iter *iter = foodList.begin();iter != NULL; iter = foodList.next(iter))
// 			{
// 				root["foodList"].append(iter->mValue);
// 			}

			for(List<Goods>::Iter * iter = fixedList.begin(); iter != NULL; iter = fixedList.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["fixedList"].append(son);
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


	struct CGBattleStart
	{
		CGBattleStart():id(0){}

		UInt32 id;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				id = root["id"].asUInt();
			}
		}
		LYNX_MESSAGE_1(BATTLE_START_REQ	, CGBattleStart,std::string, jsonStr);
	};
	//BATTLE_START_RESP
	struct TwelveBattleStartResp	
	{
		TwelveBattleStartResp():id(0){}

		UInt32 id;
	
		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["id"] = Json::Value(id);
			
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	class StageCalcManager:public MainThread ,public Singleton<StageCalcManager>
	{
	public:
		static void onTwelvePalaceUnlock(const  ConnId&  ,CGTwelvePalaceUnlock & );

		static void onTwelvePalaceStartReq(const  ConnId&  ,CGTwelvePalaceStart & );

		static void onTwelvePalaceEndReq(const  ConnId&  ,CGTwelvePalaceEnd & );

		static void onBattleStart(const  ConnId& connId,CGBattleStart & msg);

		void setChapterLock(const  ConnId& connId ,UInt32 key,UInt32 value);

		void addAwardGain(const  ConnId& connId ,PlayerFireConfirmData &mFireConfirmData,List<UInt32> awardType,UInt32 time,UInt32 &recordBreaking);

		UInt32 getAwards(const  ConnId& connId ,UInt32 doType,List<AwardMonsterDamage> awardMonsterList,UInt32 stageID,UInt32 times);

// 		template <class KeyType, class ValueType>
// 		UInt32 getKey(Map<KeyType,ValueType> values,UInt32 flag );

		UInt32 getMinKey(Map<UInt64, StageTemplate> values,UInt32 flag );


		void findSimilarPowerReq(Guid playerID,UInt32 times,UInt32 high,UInt32 low);



 		Map<UInt32,UInt32> *getResetStages(UInt64 playerID);

		void addResetStages(UInt64 playerID,UInt32 stageID,UInt32 count,bool needSave);

		UInt32 getResetStageCount(UInt64 playerID,UInt32 stageID);


		void resetAllResetStages();

		void addNeedSaveResetStages(UInt64 playerID,UInt32 stageID,UInt32 val);

		List<NeedSaveResetStages>& getNeedSaveResetStages();

		void resetNeedSaveResetStages();

		UInt32 resetHaoJiao(Guid playerID);
		

		bool checkStageClearance(Guid playerID,UInt32 stageId);

		
		
		
	};

	static ResetStages mResetStages;
	static List<NeedSaveResetStages> gNeedSaveResetStages;
	static Map<UInt64,ResetStages*> gResetStages;//单个十二宗宫关卡次数重置


}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_STAGE_CALC_MANAGER_H__
