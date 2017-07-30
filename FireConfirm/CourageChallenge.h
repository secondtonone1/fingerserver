#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_COURAGECHALLENGE_CHALLENGE_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_COURAGECHALLENGE_CHALLENGE_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "../TimeManager.h"


#include "Gift.h"

namespace Lynx
{
	enum CourageChallenge
	{
		CourageID1 = 5,//勇气挑战关卡id
		CourageID2 = 6,
		CourageID3 = 7,

	};
	struct CourageChallengeReq
	{
		CourageChallengeReq(): reqType(0),id(0),needSend(true){}

		UInt32 reqType;//0请求信息 1刷新 2购买 3激活
		UInt32 id;//关卡id，组合id
		bool needSend;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
				id = root["id"].asUInt();
			}
		}
		LYNX_MESSAGE_1(COURAGE_CHALLENGE_REQ, CourageChallengeReq,std::string, strReceive);
	};



	struct CourageChallengeResp
	{
		CourageChallengeResp(): reqType(0),id(0),result(0),gold(0),leftChallengeTimes(0),needTime(0),buyTimes(0),lightOfLife(0),monsterID1(0),monsterID2(0),
			monsterID3(0),refreshTimes(0),maxChallengeTimes(0),maxFreeRefreshTimes(0),nextRefreshCost(0),recoverTime(0),nextBuyCost(0),maxBuyTimes(0)
		,luckyValues1(0),luckyValues2(0),luckyValues3(0){}

		UInt32 reqType;
		UInt32 id;//关卡id，组合id
		UInt32 result;		
		UInt32 gold;
		UInt32 leftChallengeTimes;
		UInt32 needTime;
		UInt32 buyTimes;
		UInt32 lightOfLife;
		UInt32 monsterID1;
		UInt32 monsterID2;
		UInt32 monsterID3;		
		UInt32 refreshTimes;
		UInt32 maxChallengeTimes;
		UInt32 maxFreeRefreshTimes;

		UInt32 nextRefreshCost;
		UInt32 recoverTime;
		UInt32 nextBuyCost;
		UInt32 maxBuyTimes;
		Int32 luckyValues1;
		Int32 luckyValues2;
		Int32 luckyValues3;

		List<UInt32> coinGroup1;
		List<UInt32> coinGroup2;
		List<UInt32> coinGroup3;
		List<CatCoin> catCoins;	

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["id"] = Json::Value(id);
			root["result"] = Json::Value(result);
			root["gold"] = Json::Value(gold);
			root["leftChallengeTimes"] = Json::Value(leftChallengeTimes);
			root["needTime"] = Json::Value(needTime);
			root["buyTimes"] = Json::Value(buyTimes);
			root["lightOfLife"] = Json::Value(lightOfLife);
			root["monsterID1"] = Json::Value(monsterID1);
			root["monsterID2"] = Json::Value(monsterID2);			
			root["monsterID3"] = Json::Value(monsterID3);
			root["refreshTimes"] = Json::Value(refreshTimes);
			root["maxChallengeTimes"] = Json::Value(maxChallengeTimes);
			root["maxFreeRefreshTimes"] = Json::Value(maxFreeRefreshTimes);
			root["nextRefreshCost"] = Json::Value(nextRefreshCost);
			root["nextBuyCost"] = Json::Value(nextBuyCost);
			root["recoverTime"] = Json::Value(recoverTime);
			root["maxBuyTimes"] = Json::Value(maxBuyTimes);
			root["luckyValues1"] = Json::Value(luckyValues1);
			root["luckyValues2"] = Json::Value(luckyValues2);
			root["luckyValues3"] = Json::Value(luckyValues3);
			for(List<UInt32>::Iter * iter = coinGroup1.begin(); iter != NULL; iter = coinGroup1.next(iter))
			{				
				root["coinGroup1"].append(iter->mValue);
			}
			for(List<UInt32>::Iter * iter = coinGroup2.begin(); iter != NULL; iter = coinGroup2.next(iter))
			{				
				root["coinGroup2"].append(iter->mValue);
			}
			for(List<UInt32>::Iter * iter = coinGroup3.begin(); iter != NULL; iter = coinGroup3.next(iter))
			{				
				root["coinGroup3"].append(iter->mValue);
			}
			for(List<CatCoin>::Iter * iter1 = catCoins.begin(); iter1 != NULL; iter1 = catCoins.next(iter1))
			{
				Json::Value leaf;
				leaf.append(iter1->mValue.catCoinID);
				leaf.append(iter1->mValue.num);
				root["catCoins"].append(leaf);				
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct CatCoinReq
	{
		CatCoinReq(): reqType(0), catCoinID(0),num(0){}
		UInt32 reqType;//1兑换2一键兑换
		UInt32 catCoinID;
		UInt32 num;


		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
				catCoinID = root["catCoinID"].asUInt();
				num = root["num"].asUInt();
			}
		}


		LYNX_MESSAGE_1(CAT_COIN_REQ, CatCoinReq,std::string, strReceive);

	};
	



	struct CatCoinResp
	{
		CatCoinResp(): reqType(0),result(0),leftChallengeTimes(0),lightOfLife(0){}

		UInt32 reqType;
		UInt32 result;				
		UInt32 leftChallengeTimes;	
		UInt32 lightOfLife;
		List<CatCoin> catCoins;	
		Json::Value allAttr;//发送物品属性

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["lightOfLife"] = Json::Value(lightOfLife);
			root["leftChallengeTimes"] = Json::Value(leftChallengeTimes);

			for(List<CatCoin>::Iter * iter1 = catCoins.begin(); iter1 != NULL; iter1 = catCoins.next(iter1))
			{
				Json::Value leaf;
				leaf.append(iter1->mValue.catCoinID);
				leaf.append(iter1->mValue.num);
				root["catCoins"].append(leaf);
				
			}
			root["allAttr"] = Json::Value(allAttr);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	
	


	struct CourageChallengeBeginReq
	{
		CourageChallengeBeginReq(): id(0){}

		UInt32 id;//关卡id

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
		LYNX_MESSAGE_1(COURAGE_CHALLENG_BEGIN_REQ, CourageChallengeBeginReq,std::string, strReceive);
	};

	struct CourageChallengeBeginResp
	{
		CourageChallengeBeginResp(): id(0),leftChallengeTimes(0),needTime(0),result(0){}


		UInt32 id;	//1 初等，2 中等，3 高等	
		UInt32 leftChallengeTimes ;//
		UInt32 needTime ;// 
		UInt32  result ;//
		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["result"] = Json::Value(result);
			root["id"] = Json::Value(id);		
			root["leftChallengeTimes"] = Json::Value(leftChallengeTimes);
			root["needTime"] = Json::Value(needTime);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	



	struct CourageChallengeEndReq
	{
		CourageChallengeEndReq(): id(0),doType(0){}

		UInt32 id;
		UInt32 doType;
		List<FireConfirm> fireConfirmData;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;  
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				doType = root["doType"].asUInt();
				id = root["id"].asUInt();
			}
			for(int i =0; i <root["fireConfirmData"].size();i++)
			{
				FireConfirm fireConfirm;
				fireConfirm.index  = root["fireConfirmData"][i]["index"].asUInt();
				fireConfirm.count  = root["fireConfirmData"][i]["count"].asUInt();
				fireConfirm.keyValue.key  = root["fireConfirmData"][i]["keyValue"]["key"].asUInt();
				fireConfirm.keyValue.value  = root["fireConfirmData"][i]["keyValue"]["value"].asUInt();

				for(int ii =0; ii <root["fireConfirmData"][i]["groupList"].size();ii++)
				{
					IndexList indexList;
					List<UInt32> valueList;
					indexList.subIndex = root["fireConfirmData"][i]["groupList"][ii]["subIndex"].asUInt();
					for(int iii =0; iii <root["fireConfirmData"][i]["groupList"][ii]["valueList"].size();iii++)
					{							
						valueList.insertTail(root["fireConfirmData"][i]["groupList"][ii]["valueList"][iii].asUInt());
					}
					fireConfirm.groupList.insertTail(indexList);
				}
				fireConfirmData.insertTail(fireConfirm);
			}
		}
		LYNX_MESSAGE_1(COURAGE_CHALLENG_END_REQ, CourageChallengeEndReq,std::string, strReceive);
	};
	

	struct CourageChallengeEndResp
	{
		CourageChallengeEndResp(): doType(0),result(0),id(0){}

		UInt32 doType;
		UInt32 result;
		UInt32 id;
		List<Goods> goodsList;
		List<CatCoin> catCoins;

		List<Goods> fixedList;
		List<Goods> ends;
		List<Award> awards;
		Json::Value allAttr;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["doType"] = Json::Value(doType);
			root["result"] = Json::Value(result);
			root["id"] = Json::Value(id);


			for(List<Goods>::Iter * iter = goodsList.begin(); iter != NULL; iter = goodsList.next(iter))
			{
				Json::Value son;
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num	);

				root["goodsList"].append(son);
			}
			for(List<CatCoin>::Iter * iter1 = catCoins.begin(); iter1 != NULL; iter1 = catCoins.next(iter1))
			{
				Json::Value son;
				son.append(iter1->mValue.catCoinID);
				son.append(iter1->mValue.num);

				root["catCoins"].append(son);
			}
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



	struct CourageShopReq
	{
		CourageShopReq(): reqType(0),shopType(0),needSend(1){}

		UInt32 reqType;
		UInt32 shopType;
		UInt32 needSend;//1需要发送,0不需要发送

		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
				shopType = root["shopType"].asUInt();				
			}
		}
		LYNX_MESSAGE_1(COURAGE_SHOP_INFO_REQ, CourageShopReq,std::string, jsonStr);
	};


	struct CourageShopResp	
	{
		CourageShopResp(): reqType(0),result(0),shopType(0),refreshTimes(0),gold(0),refreshNeedTime(0),refreshCost(0),refreshMaxTimes(0),lightOfLife(0){}

		UInt32 reqType;
		UInt32 shopType;
		UInt32 result;
		UInt32 refreshTimes;//
		UInt32 gold;
		UInt32 refreshNeedTime;
		UInt32 refreshCost;
		UInt32 refreshMaxTimes;
		UInt32 lightOfLife;
		List<Item> items;	
		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["shopType"] = Json::Value(shopType);
			root["refreshTimes"] = Json::Value(refreshTimes);
			root["gold"] = Json::Value(gold);
			root["refreshNeedTime"] = Json::Value(refreshNeedTime);
			root["refreshCost"] = Json::Value(refreshCost);
			root["refreshMaxTimes"] = Json::Value(refreshMaxTimes);
			root["lightOfLife"] = Json::Value(lightOfLife);
			


			for(List<Item>::Iter * iter = items.begin(); iter != NULL; iter = items.next(iter))
			{
				Json::Value son;
				son["position"] = Json::Value(iter->mValue.position);
				son["buyTimes"] = Json::Value(iter->mValue.buyTimes);
				son["goods"].append(iter->mValue.goods.resourcestype);
				son["goods"].append(iter->mValue.goods.subtype);
				son["goods"].append(iter->mValue.goods.num);
				son["cost"] = Json::Value(iter->mValue.cost);

				root["items"].append(son);
			}
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct CourageShopBuyReq
	{
		CourageShopBuyReq(): shopType(0),num(0),position(0){}

		UInt32 shopType;
		UInt32 num;
		UInt32 position ;//普通关卡为星数，多人副本为完成时间
		List<AwardMonsterDamage> awardMonsterList;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				shopType = root["shopType"].asUInt();
				num = root["num"].asUInt();
				position  = root["position"].asUInt();
			}
		}
		LYNX_MESSAGE_1(COURAGE_SHOP_BUY_REQ, CourageShopBuyReq,std::string, jsonStr);
	};

	struct CourageShopBuyResp	
	{
		CourageShopBuyResp(): shopType(0),result(0),buyNum(0),position(0),lightOfLife(0){}

		UInt32 shopType;
		UInt32 result;
		UInt32 buyNum;//
		UInt32 position ;//
		UInt32 lightOfLife;
		Json::Value allAttr;//
		List<Award> awards;
		List<Goods> ends;
		List<CatCoin> catCoins;


		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["shopType"] = Json::Value(shopType);
			root["result"] = Json::Value(result);
			root["buyNum"] = Json::Value(buyNum);
			root["position"] = Json::Value(position);
			root["lightOfLife"] = Json::Value(lightOfLife);

			for(List<CatCoin>::Iter * iter1 = catCoins.begin(); iter1 != NULL; iter1 = catCoins.next(iter1))
			{
				Json::Value leaf;
				leaf.append(iter1->mValue.catCoinID);
				leaf.append(iter1->mValue.num);
				root["catCoins"].append(leaf);				
			}

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

	




	class CourageChallengeManager: public Singleton<CourageChallengeManager>
	{

	public:

		UInt32 courageChallengeRefresh(Guid playerID, PlayerCourageChallengeData &CourageChallengeData,UInt32 &cost,UInt32 id);

		UInt32 courageChallengeBuy(Guid playerID, PlayerCourageChallengeData &CourageChallengeData,UInt32 &cost,UInt32 id);

		UInt32 courageChallengeActivate(Guid playerID, PlayerCourageChallengeData &CourageChallengeData,UInt32 id);


		void refreshTimes(Player *player,PlayerCourageChallengeData &courageChallengeData);

		static void onCourageChallengeReq(const  ConnId& ,CourageChallengeReq & );		//获取信息 

		static void onCatCoinReq(const  ConnId& ,CatCoinReq & );		//获取猫币信息

		static void onCourageChallengeBeginReq(const  ConnId& ,CourageChallengeBeginReq & );		//开始 		

		static void onCourageChallengeEndReq(const  ConnId& ,CourageChallengeEndReq & );	//结束

		void setCatCoins(Guid playerID,PlayerCourageChallengeData &courageChallengeData, Goods goods, bool needSave );

		void getCourageContentByID(UInt32 itemID,List<Goods> &ItemList);

		void initCourageMonster(PlayerCourageChallengeData &CourageChallengeData,UInt32 id);

// 		void checkLuckyValueReset(PlayerCourageChallengeData &courageChallengeData,UInt32 id);

		UInt32 getNextRefreshCost(UInt32 refreshTimes);

		UInt32 checkLimit(Guid playerID,UInt32 id);

		UInt32 getCatCoinNum(Guid playerID,UInt32 id);

		void divisionCoinGrop(List<UInt32> &group1,List<UInt32> &group2,List<UInt32> &group3,List<UInt32> group);





		static void onCourageShopReq(const  ConnId& connId, CourageShopReq & msg);

		static void onCourageShopBuyReq(const  ConnId& connId, CourageShopBuyReq & msg);

		UInt32 getCourageShopInfo(Guid playerID,UInt32 refresh,CourageShopResp &resp);

// 		UInt32 checkCourageRefreshTime(Guid playerID,UInt32 shopType,UInt32 &refreshNeedTime);

		UInt32 getCourageShopID(UInt32 position,UInt32 level,UInt32 vipLevel,UInt32 refreshTimes,UInt32 refresh);

// 		UInt32 dealMysticalshopbuy(Guid playerID,UInt32 shopType,UInt32 position,CourageShopBuyResp &resp);

// 		void getCourageShopNewItems(Guid playerID, UInt32 position,UInt32 acID,UInt32 shopType,Item &item);

		UInt32 dealCourageshopbuy(Guid playerID,UInt32 shopType,UInt32 position,CourageShopBuyResp &resp);

		


	};
}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_COURAGECHALLENGE_CHALLENGE_MANAGER_H__
