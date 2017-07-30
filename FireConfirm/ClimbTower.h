#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_CLIMB_TOWER_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_CLIMB_TOWER_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"

namespace Lynx
{
	enum Road
	{
		DefaultRoad = 1,		
		CrossRoad = 2,
		EventNum = 3,

	};
	struct TowerEvent
	{
		TowerEvent(): tID(0),type(0),isRandom(0),mID(0),eID(0),typeValue(0),Ap(0),Hp(0),Sp(0),score(0),scoreCost(0){}
		UInt32 tID;//塔id
		UInt32 type;//1-12事件类型
		UInt32 typeValue;//id或值
		UInt32 isRandom;//0不是随机事件，1是
 		List<Goods> goods;//执行完事件将要给的物品
		List<TowerEvent> crossEvents;

		UInt32 mID;//消息id（0结算，1是路1，2）2路2,100随机事件出来的事件 101商店返回
		UInt32 eID;//事件id
		UInt32 Ap;//攻击力
		UInt32 Hp;//血量
		UInt32 Sp;//体力
		UInt32 score;//积分
		UInt32 scoreCost;//积分
	};

	struct TowerBuff
	{
		TowerBuff(): buffId(0),buffStartTime(0){}
		UInt32 buffId;
		UInt32 buffStartTime;
	};

	struct ShopItem
	{
		ShopItem(): high(0),shopID(0),which(0){}
		UInt32 high;
		UInt32 shopID;//商店ID
		UInt32 which;//第几个物品
	};
	struct EventList
	{
		EventList(): startTime(0),HPStartTime(0),SPStartTime(0),towerHight(0),roadType(1),lastJumpFloor(0),latestJumpFloor(0),latestCrossRoadFloor(0),reliveTimes(0){}
		UInt32 startTime;
		UInt32 HPStartTime;
		UInt32 SPStartTime;
		UInt32 towerHight;//记录哪一层的高度
		UInt32 roadType;//选择岔路所在哪一层 1是原来的路2是岔路
		UInt32 reliveTimes;
		UInt32 lastJumpFloor;
		UInt32 latestJumpFloor;
		UInt32 latestCrossRoadFloor;
		List<ShopItem> buyList;
		List<TowerBuff> towerBuff;//buff
		List<Goods> itemList;//所有获得的物品
		PlayerTowerData towerData;//实时数据
		List<TowerEvent> events;
	};

	enum TowerChangType
	{
		TOWER_SUB_HP = 1,
		TOWER_ADD_AP = 2,
		TOWER_SUB_SP = 3,
		TOWER_ADD_SCORE = 4,

	};

	struct TowerReq
	{
		TowerReq(): reqtype(0){}

		UInt32 reqtype;//1 获取信息类型 2购买次数

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqtype = root["reqtype"].asUInt();
			}
		}
		LYNX_MESSAGE_1(TOWER_INFO_REQ, TowerReq,std::string, strReceive);
	};


	struct TowerResp
	{
		TowerResp(): reqtype(0),result(0),highID(0),score(0),ap(0),hp(0),buyTimes(0),times(0){}

		UInt32 reqtype;
		UInt32 result;		
		UInt32 highID;
		UInt32 score;
		UInt32 ap;
		UInt32 hp;
		
		UInt32 times;//剩余挑战次数
		UInt32 buyTimes;//购买了次数

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqtype"] = Json::Value(reqtype);
			root["result"] = Json::Value(result);
			root["highID"] = Json::Value(highID);
			root["score"] = Json::Value(score);
			root["ap"] = Json::Value(ap);
			root["hp"] = Json::Value(hp);
			
			root["buyTimes"] = Json::Value(buyTimes);
			root["times"] = Json::Value(times);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	struct TowerBeginReq
	{
		TowerBeginReq(): id(0){}

		UInt32 id;//id	1 初等，2 中等，3 高等 
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				id = root["id"].asUInt();
			}
		}
		LYNX_MESSAGE_1(TOWER_BEGIN_REQ, TowerBeginReq,std::string, strReceive);
	};

	struct TowerBeginResp
	{
		TowerBeginResp(): result(0),id(0),score(0),times(0){}

		UInt32 result;
		UInt32 id;	//1 初等，2 中等，3 高等	
		UInt32 score;		
		UInt32 times;		
		List<TowerEvent> events;
		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["result"] = Json::Value(result);
			root["id"] = Json::Value(id);		
			root["score"] = Json::Value(score);
			root["times"] = Json::Value(times);

			for(List<TowerEvent>::Iter * iter = events.begin(); iter != NULL; iter = events.next(iter))
			{
				Json::Value son;
				son.append(iter->mValue.type);
				son.append(iter->mValue.isRandom);
// 				son.append(iter->mValue.tID);				
				son.append(iter->mValue.typeValue);			
				
				for(List<Goods>::Iter * iter1 = iter->mValue.goods.begin(); iter1 != NULL; iter1 = iter->mValue.goods.next(iter1))
				{
					Json::Value leaf;
					leaf.append(iter1->mValue.resourcestype);
					leaf.append(iter1->mValue.subtype);
					leaf.append(iter1->mValue.num);
					son.append(leaf);
					break;
				}
				
				Json::Value mySon;
				for(List<TowerEvent>::Iter * it = iter->mValue.crossEvents.begin(); it != NULL; it =  iter->mValue.crossEvents.next(it))
				{
					Json::Value leaf;
					leaf.append(it->mValue.type);
					leaf.append(it->mValue.isRandom);
// 					leaf.append(it->mValue.tID);					
					leaf.append(it->mValue.typeValue);
					
					for(List<Goods>::Iter * iter1 = it->mValue.goods.begin(); iter1 != NULL; iter1 = it->mValue.goods.next(iter1))
					{
						Json::Value leaf1;
						leaf1.append(iter1->mValue.resourcestype);
						leaf1.append(iter1->mValue.subtype);
						leaf1.append(iter1->mValue.num);
						leaf.append(leaf1);
						break;
					}

					mySon.append(leaf);
					
				}
				if (iter->mValue.crossEvents.size() != 0)
				{
					son.append(mySon);
				}				
				root["events"].append(son);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};



	struct InTowerChoiseReq
	{
		InTowerChoiseReq(): tID(0),type(0),wID(0){}

		UInt32 tID;//塔id
		UInt32 type;//1贿赂 2喝血 3不开宝箱 4购买5 复活6购买结束
		UInt32 wID;//宝物索引id 第几个
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				tID = root["tID"].asUInt();
				type = root["type"].asUInt();
				wID = root["wID"].asUInt();
			}
		}
		LYNX_MESSAGE_1(IN_TOWER_CHOICE_REQ, InTowerChoiseReq,std::string, strReceive);
	};


	struct BattleReport
	{
		int ownBlood;
		int enemyBlood;
		UInt32 flag;
	};

	struct InTowerChoiseResp
	{
		InTowerChoiseResp(): result(0),tID(0),Ap(0),Hp(0),Sp(0),score(0),isOver(0),buffID1(0),time1(0),buffID2(0),time2(0),chooseType(0){}

		UInt32 result;
		UInt32 isOver;//0,1是结束
		UInt32 tID;
		UInt32 chooseType;
		UInt32 Ap;
		UInt32 Hp;
		UInt32 Sp;
		UInt32 score;
		UInt32 buffID1;
		UInt32 time1;
		UInt32 buffID2;
		UInt32 time2;
		List<Goods> goods;
		List<BattleReport> battleReport;


		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["result"] = Json::Value(result);
			root["tID"] = Json::Value(tID);
			root["chooseType"] = Json::Value(chooseType);
			root["Ap"] = Json::Value(Ap);
			root["Hp"] = Json::Value(Hp);
			root["Sp"] = Json::Value(Sp);
			root["score"] = Json::Value(score);
			root["isOver"] = Json::Value(isOver);
			root["buffID1"] = Json::Value(buffID1);
			root["time1"] = Json::Value(time1);
			root["buffID2"] = Json::Value(buffID2);
			root["time2"] = Json::Value(time2);

			for(List<Goods>::Iter * iter = goods.begin(); iter != NULL; iter = goods.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["goods"].append(son);
			}

			for(List<BattleReport>::Iter * it = battleReport.begin(); it != NULL; it = battleReport.next(it))
			{
				Json::Value son;	
				son.append(it->mValue.ownBlood);
				son.append(it->mValue.enemyBlood);
				son.append(it->mValue.flag);
				root["battleReport"].append(son);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct TowerFloorEndReq
	{
		TowerFloorEndReq(): id(0){}

		UInt32 id;
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				id = root["id"].asUInt();
			}
		}
		LYNX_MESSAGE_1(TOWER_FLOOR_END_REQ, TowerFloorEndReq,std::string, strReceive);
	};

	struct TowerFloorEndResp
	{
		TowerFloorEndResp(): result(0){}


		UInt32 result;
		List<TowerEvent> events;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["result"] = Json::Value(result);

			for(List<TowerEvent>::Iter * iter = events.begin(); iter != NULL; iter = events.next(iter))
			{
				Json::Value son;
				son["tID"] = Json::Value(iter->mValue.tID);
				son["mID"] = Json::Value(iter->mValue.mID);
				son["eID"] = Json::Value(iter->mValue.eID);
				son["typeValue"] = Json::Value(iter->mValue.typeValue);
				son["Ap"] = Json::Value(iter->mValue.Ap);
				son["Hp"] = Json::Value(iter->mValue.Hp);
				son["Sp"] = Json::Value(iter->mValue.Sp);
				son["score"] = Json::Value(iter->mValue.score);
				for(List<Goods>::Iter * iter1 = iter->mValue.goods.begin(); iter1 != NULL; iter1 = iter->mValue.goods.next(iter1))
				{
					Json::Value leaf;
					leaf = Json::Value(iter1->mValue.resourcestype);
					leaf = Json::Value(iter1->mValue.subtype);
					leaf = Json::Value(iter1->mValue.num);
					son["goods"].append(leaf);
				}
				root["events"].append(son);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}		
	};

	struct TowerEndReq
	{
		TowerEndReq(): /*id(0),*/reqType(0){}

		/*UInt32 id;*/
		UInt32 reqType;// Normal 1,hp 2,time 3,exit 0
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				/*id = root["id"].asUInt();*/
				reqType = root["reqType"].asUInt();
			}
		}
		LYNX_MESSAGE_1(TOWER_END_REQ, TowerEndReq,std::string, strReceive);
	};

	struct TowerEndResp
	{
		TowerEndResp(): result(0),reqType(0),score(0),coin(0),nextScore(0),highID(0),currentHighID(0){}
		UInt32 result;
		UInt32 reqType;
		UInt32 score;
		UInt32 coin;
		UInt32 nextScore;
		UInt32 leftscore;
		UInt32 highID;
		UInt32 currentHighID;
		List<Goods> awardContentList;//轮盘获得物品
		List<Goods> awardShow;//轮盘显示
// 		List<UInt32> awardIndexList;


		Json::Value allAttr;
 		List<Goods> fixedList;//固定掉落
		List<Goods> goods;
		List<Goods> ends;//最总值，没有属性积分
		List<Award> awards;//中间获得物品


		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["result"] = Json::Value(result);
			root["reqType"] = Json::Value(reqType);
			root["score"] = Json::Value(score);
			root["coin"] = Json::Value(coin);
			root["nextScore"] = Json::Value(nextScore);
			root["leftscore"] = Json::Value(leftscore);
			root["highID"] = Json::Value(highID);
			root["currentHighID"] = Json::Value(currentHighID);

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

			for(List<Goods>::Iter * iter = fixedList.begin(); iter != NULL; iter = fixedList.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["fixedList"].append(son);
			}
			for(List<Goods>::Iter * iter = goods.begin(); iter != NULL; iter = goods.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["goods"].append(son);
			}

// 			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
// 			{
// 				Json::Value son;	
// 				son.append(iter->mValue.resourcestype);
// 				son.append(iter->mValue.subtype);
// 				son.append(iter->mValue.num);
// 				root["ends"].append(son);
// 			}

			for(List<Goods>::Iter * iter1 = awardContentList.begin(); iter1 != NULL; iter1 = awardContentList.next(iter1))
			{
				Json::Value son;	
				son.append(iter1->mValue.resourcestype);
				son.append(iter1->mValue.subtype);
				son.append(iter1->mValue.num);
				root["awardContentList"].append(son);
			}
			for(List<Goods>::Iter * iter1 = awardShow.begin(); iter1 != NULL; iter1 = awardShow.next(iter1))
			{
				Json::Value son;	
				son.append(iter1->mValue.resourcestype);
				son.append(iter1->mValue.subtype);
				son.append(iter1->mValue.num);
				root["awardShow"].append(son);
			}


			root["allAttr"] = Json::Value(allAttr);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}		
	};

	struct TowerBuffReq
	{
		TowerBuffReq(): id(0){}

		UInt32 id;
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				id = root["id"].asUInt();
			}
		}
		LYNX_MESSAGE_1(TOWER_BUFF_REQ, TowerBuffReq,std::string, strReceive);
	};

	struct TowerBuffResp
	{
		TowerBuffResp(): buffID1(0),time1(0),buffID2(0),time2(0){}

		UInt32 buffID1;
		int time1;
		UInt32 buffID2;
		int time2;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["buffID1"] = Json::Value(buffID1);
			root["time1"] = Json::Value(time1);
			root["buffID2"] = Json::Value(buffID2);
			root["time2"] = Json::Value(time2);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}		
	};


	struct TowerReliveReq
	{
		TowerReliveReq(){}
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
								
			}
		}
		LYNX_MESSAGE_1(TOWER_RELIVE_REQ, TowerReliveReq,std::string, strReceive);
	};


	struct TowerReliveResp
	{
		TowerReliveResp(): reliveTimes(0),result(0),gold(0),Hp(0),Sp(0),buyTimes(0){}

		UInt32 result;		
		UInt32 gold;
		UInt32 Hp;
		UInt32 Sp;
		UInt32 reliveTimes;//剩余挑战次数
		UInt32 buyTimes;//购买了次数

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reliveTimes"] = Json::Value(reliveTimes);
			root["result"] = Json::Value(result);
			root["gold"] = Json::Value(gold);
			root["Hp"] = Json::Value(Hp);
			root["Sp"] = Json::Value(Sp);
			root["buyTimes"] = Json::Value(buyTimes);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct TowerMopUpReq
	{
		TowerMopUpReq():type(1){}
		UInt32 type;
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				type = root["type"].asUInt();
			}
		}
		LYNX_MESSAGE_1(TOWER_MOPUP_REQ, TowerMopUpReq,std::string, strReceive);
	};


	struct TowerMopUpResp
	{
		TowerMopUpResp():result(0),times(0){}

		UInt32 result;
		UInt32 times;//剩余挑战次数

		Json::Value allAttr;
		List<Goods> goods;
		List<Award> awards;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["result"] = Json::Value(result);
			root["times"] = Json::Value(times);

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

// 			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
// 			{
// 				Json::Value son;	
// 				son.append(iter->mValue.resourcestype);
// 				son.append(iter->mValue.subtype);
// 				son.append(iter->mValue.num);
// 				root["ends"].append(son);
// 			}


			root["allAttr"] = Json::Value(allAttr);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};




	class ClimbTowerManager: public Singleton<ClimbTowerManager>
	{

	public:

// 		static void onTowerFloorEndReq(const  ConnId& ,TowerFloorEndReq & );		//每层结束

		static void onInTowerChoiseReq(const  ConnId& ,InTowerChoiseReq & );//选择 

		static void onTowerBeginReq(const  ConnId& ,TowerBeginReq & );		//开始 

		static void onTowerReq(const  ConnId& ,TowerReq & );		//获取信息 

// 		static void onTowerBuffReq(const  ConnId& connId,TowerBuffReq & req);


		static void onTowerEndReq(const  ConnId& ,TowerEndReq & );	//结束

		static void onTowerReliveReq(const  ConnId& ,TowerReliveReq & );	//复活

		static void onTowerMopUpReq(const  ConnId& connId,TowerMopUpReq & msg);//扫荡

		void towerEnd(Player *player,UInt32 reqType);

		void getNextEvents(Guid playerID,UInt32 towerHight,List<TowerEvent> &events,UInt32 road,UInt32 num);//获取事件

		//解析事件
		void spiltEvent(TowerEvent &towerEvent);

		//随机几个事件
		UInt32 rollEvent(UInt32 towerHight);

		//处理事件的怪或继续随机
		void dealEventfollowEvent(Guid playerID,TowerEvent &towerEvent,List<TowerEvent> &events);

		void setPlayerEvent(Guid playerID,TowerEvent towerEvent);

		void subPlayerEvent(Guid playerID,UInt32 towerHight);

		void getPlayerEvent(Guid playerID,TowerEvent &towerEvent,UInt32 towerHight,UInt32 mID);
	
		UInt32 getBattleReport(Guid playerID,List<BattleReport> &battleReportList,TowerEvent &towerEvent);
		//事件完成结算
		UInt32 addEventValues(Guid playerID,InTowerChoiseReq & req,InTowerChoiseResp &resp);

		//加物品
		void dealTowerGoods(Guid playerID,List<Goods> goodsList);//加商店购买

		UInt32 addTowerData(Guid playerID,List<Goods> goods,UInt32 dataType,int num);//加爬塔的数值，goods加到玩家掉落物品

		bool getBuffValue(Guid playerID,UInt32 dataType,int &num);

		//最高层
		UInt32 getTowerHighest(UInt32 towerHigh);

		void setEventList(Guid playerID,EventList &eventList);

		void removeEvent(Guid playerID);

		//获取buff
		void getTowerBuff(Guid playerID,UInt32 &id1,UInt32 &time1,UInt32 &id2,UInt32 &time2);

		void setTowerBuff(Guid playerID,UInt32 id);

		//HP改变
		UInt32 changAP(Guid playerID,int num,bool bFlag);

		UInt32 changHP(Guid playerID,int num,bool bFlag);

		UInt32 changSP(Guid playerID,int num);

		UInt32 changeScore(Guid playerID,int num,bool bFlag);

		UInt32 checkCanBuy(Guid playerID,UInt32 high,UInt32 shop,UInt32 which);

		UInt32 checkCanRelive(Guid playerID);

		UInt32 checkIsOver(const  ConnId& connId);

		void sendChoiseResp(const  ConnId& connId,InTowerChoiseResp resp);

		UInt32 getMopUpAwardID(UInt32 type);

		void updateClimTower(UInt32 isShutDown);


		void getAwardRoulettes(UInt32 itemID,UInt32 times,List<Goods> &itemList,List<Goods> &itemShowList);

		void getAwardRouletteByID(UInt32 itemID,UInt32 num,List<Goods> &ItemList);

		void getRouletteContentByID(UInt32 itemID,UInt32 times,List<Goods> &itemList,List<Goods> &itemShowList);


	};



	static Map<Guid,EventList> mTowerEvents;

}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_CLIMB_TOWER_MANAGER_H__
