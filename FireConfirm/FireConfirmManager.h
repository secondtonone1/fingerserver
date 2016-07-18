#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_FIRE_CONFIRM_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_FIRE_CONFIRM_MANAGER_H__

#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/FirePacket.h"
#include "../CommonLib/StagePacket.h"


#define OUT_OF_RANGE 1.2 //与最高值误差系数

namespace Lynx
{

	class Player;
	class PlayerFireConfirmData;


	struct CGConfirmDataReq
	{
		CGConfirmDataReq(): confirmID(0){}

		UInt32 confirmID;
 		List<Int32> confirmDataList;
		std::string strConfirmData;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				confirmID = root["confirmID"].asUInt();
				int itmp = 0;
				for(int i =0; i <root["confirmDataList"].size();i++)
				{
					itmp  = root["confirmDataList"].operator [](i).asInt();
					confirmDataList.insertTail(itmp);
				}
			}
		}
		 	
		LYNX_MESSAGE_1(FIRE_CONFIRM_DATA_REQ, CGConfirmDataReq,std::string, strConfirmData);
	};
	
	struct CGChapterStart
	{
		CGChapterStart(): stageID(0){}

		UInt32 stageID;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				stageID = root["chapterID"].asUInt();				
			}
		}

		LYNX_MESSAGE_1(CHAPTER_START_REQ, CGChapterStart,std::string, jsonStr);
	};
	struct ChapterStartResp	
	{
		ChapterStartResp(): chapterID(0),result(0){}

		UInt32 chapterID;
		UInt32 result;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["chapterID"] = Json::Value(chapterID);
			root["result"] = Json::Value(result);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

	};
	struct CGChapterLeave
	{
		CGChapterLeave(): chapterID(0){}

		UInt32 chapterID;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				chapterID = root["chapterID"].asUInt();
			}			
		}
		LYNX_MESSAGE_1(CHAPTER_LEAVE_REQ, CGChapterLeave,std::string, jsonStr);
	};
	struct ChapterLeaveResp	
	{
		ChapterLeaveResp(): chapterID(0),result(0){}

		UInt32 chapterID;
		UInt32 result;
		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["chapterID"] = Json::Value(chapterID);
			root["result"] = Json::Value(result);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

	};
	struct AwardMonsterDamage
	{
		UInt32 ID;	//怪的ID
		UInt32 damageType;//受伤害状态
		UInt32 times;//次数

	};
	struct CGChapterEnd
	{
		CGChapterEnd(): chapterID(0),result(0),star(0){}

		UInt32 chapterID;
		UInt32 result;
		UInt32 star;//普通关卡为星数，多人副本为完成时间
		List<AwardMonsterDamage> awardMonsterList;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				chapterID = root["chapterID"].asUInt();
				result = root["result"].asUInt();
				star = root["star"].asUInt();

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
		LYNX_MESSAGE_1(CHAPTER_END_REQ, CGChapterEnd,std::string, jsonStr);
	};
	
	struct ChapterEndResp	
	{
		ChapterEndResp(): chapterID(0),result(0),star(0),recordBreaking(0){}

		UInt32 chapterID;
		UInt32 result;
		UInt32 star;//普通关卡为星数，多人副本为完成时间
		UInt32 recordBreaking;//1 破纪录 0 未破
		Json::Value allAttr;//发送物品属性
		List<Award> firstAwards;
		List<Award> awards;
		List<Goods> cost;
		List<Card> cards;
		List<Goods> intruderAwardList;
		List<Goods> monsterDropList;
		List<Goods> fixedList;
		List<Goods> ends;
		

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["chapterID"] = Json::Value(chapterID);
			root["result"] = Json::Value(result);
			root["star"] = Json::Value(star);
			root["recordBreaking"] = Json::Value(recordBreaking);

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
			for(List<Goods>::Iter * iter = intruderAwardList.begin(); iter != NULL; iter = intruderAwardList.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["intruderAwardList"].append(son);
			}

			for(List<Goods>::Iter * iter = cost.begin(); iter != NULL; iter = cost.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["cost"].append(son);
			}

			
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

	struct CGisRecievedReq
	{
		CGisRecievedReq(): recievePacketID(0){}

		UInt32 recievePacketID;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				recievePacketID = root["recievePacketID"].asUInt();
			}			
		}
		LYNX_MESSAGE_1(RECIEVED_PACKET_REQ, CGisRecievedReq,std::string, jsonStr);
	};

	struct ChapterCounterResp	
	{
		List<KeyValue> keyValue;
		std::string convertDataToJson()
		{
			Json::Value root;     
			for (List<KeyValue>::Iter *it = keyValue.begin();it!= NULL;it=keyValue.next(it))
			{
				Json::Value son;
				son["key"] = Json::Value(it->mValue.key);
				son["key"] = Json::Value(it->mValue.value);
				root["keyValue"].append(son);
			}		
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct CGChapterAwardCard
	{
		CGChapterAwardCard(): chapterID(0),flag(0){}

		UInt32 chapterID;
		UInt32 flag;
		std::string jsonStr;

		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				chapterID = root["chapterID"].asUInt();
				flag = root["flag"].asUInt();
			}
		}
		LYNX_MESSAGE_1(CHAPTER_AWARD_CARD_REQ, CGChapterAwardCard,std::string, jsonStr);
	};
	struct ChapterAwardCardResp
	{
		ChapterAwardCardResp(): chapterID(0),result(0){}

		UInt32 flag;
		UInt32 chapterID;
		UInt32 result; 
		List<Goods> award;
		List<Goods> cost;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["chapterID"] = Json::Value(chapterID);
			root["flag"] = Json::Value(flag);
			root["result"] = Json::Value(result);
			for(List<Goods>::Iter * iter = award.begin(); iter != NULL; iter = award.next(iter))
			{
				Json::Value son;
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["award"].append(son);
			}

			for(List<Goods>::Iter * iter = cost.begin(); iter != NULL; iter = cost.next(iter))
			{
				Json::Value son;
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["cost"].append(son);
			}
			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
			{
				Json::Value son;
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["ends"].append(son);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct CGRelive
	{
		CGRelive(): flag (0){}

		UInt32 flag ;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				flag  = root["flag"].asUInt();
			}			
		}
		LYNX_MESSAGE_1(CHAPTER_RELIVE_REQ, CGRelive,std::string, jsonStr);
	};
	struct ReliveResp	
	{
		ReliveResp(): flag (0),result(0),reliveTimes(0),yuanbaoCost(0),coinCost(0){}

		UInt32 flag ;
		UInt32 result;
		UInt32 reliveTimes;//复活次数
		UInt32 yuanbaoCost;//元宝最终
		UInt32 coinCost;//金币最终
		
		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["flag "] = Json::Value(flag );
			root["result"] = Json::Value(result);
			root["reliveTimes"] = Json::Value(reliveTimes);
			root["yuanbaoCost"] = Json::Value(yuanbaoCost);
			root["coinCost"] = Json::Value(coinCost);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	

	struct CGFoods
	{
		CGFoods(): reqType(0),num(0){}
		UInt32 reqType;//1获取信息，2烹饪，3购买 4 开箱子获得 5 使用食物1 6 使用食物2 7使用食物3
		UInt32 num;//个数
		
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{				
				reqType = root["reqType"].asUInt();	
				num = root["num"].asUInt();	
			}			
		}
		LYNX_MESSAGE_1(CHAPTER_FOOD_REQ, CGFoods,std::string, jsonStr);
	};

	struct FoodsResp
	{
		FoodsResp(): flag(0),needTime(0),viableAmount(0),viableNum(0),restbuyNumber(0),vipFoodLeftNumber(0),gold(0){}
		UInt32 flag;
		UInt32 needTime;//等待时间
		UInt32 viableAmount;//总次数
		UInt32 viableNum ;//可使用次数	
		UInt32 gold ;//元宝数
		List<UInt32> foodList ;//食物数量
		UInt32 restbuyNumber ;//vip剩余购买次数
		UInt32 vipFoodLeftNumber;//vip购买食物剩余个数
		std::string convertDataToJson()
		{
			Json::Value root;     
			root["flag"] = Json::Value(flag);
			root["needTime"] = Json::Value(needTime);
			root["viableAmount"] = Json::Value(viableAmount);
			root["viableNum"] = Json::Value(viableNum);
			root["gold"] = Json::Value(gold);
			for (List<UInt32>::Iter *iter = foodList.begin();iter != NULL; iter = foodList.next(iter))
			{
				root["foodList"].append(iter->mValue);
			}
			root["restbuyNumber"] = Json::Value(restbuyNumber);
			root["vipFoodLeftNumber"] = Json::Value(vipFoodLeftNumber);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct CGStrength
	{
		CGStrength(): reqType(0),num(0),needSend(true){}
		UInt32 reqType;//1获取信息，3购买 
		UInt32 num;//个数
		bool needSend;//true = send

		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{				
				reqType = root["reqType"].asUInt();	
				num = root["num"].asUInt();	
			}			
		}
		LYNX_MESSAGE_1(CHAPTER_STRENGTH_REQ, CGStrength,std::string, jsonStr);
	};
	struct StrengthResp
	{
		StrengthResp(): flag(0),reqType(0),needTime(0),viableAmount(0),restbuyNumber(0),vipStrengthLeftNumber(0),gold(0){}
		UInt32 flag;//和result有区别，1是成功
		UInt32 reqType;//1获取信息，3购买 
		UInt32 needTime;//等待时间
		UInt32 viableAmount;//体力最大值
		UInt32 gold ;//剩余元宝数
		UInt32 strength;//体力
		UInt32 restbuyNumber ;//vip购买次数
		UInt32 vipStrengthLeftNumber;//可以购买总次数，登录的时候发一次，其他时间客户要自己加

		std::string convertDataToJson()
		{
			Json::Value root;     
			root["flag"] = Json::Value(flag);
			root["reqType"] = Json::Value(reqType);
			root["needTime"] = Json::Value(needTime);
			root["viableAmount"] = Json::Value(viableAmount);
			root["gold"] = Json::Value(gold);
			root["strength"] = Json::Value(strength);
			
			root["restbuyNumber"] = Json::Value(restbuyNumber);
			root["vipStrengthLeftNumber"] = Json::Value(vipStrengthLeftNumber);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	struct CGStageMopUp
	{
		CGStageMopUp(): id(0),mopUpType(0){}
		UInt32 id;//关卡ID
		UInt32 mopUpType;//类型
		List<UInt32> useList;//食物1，食物2，食物3，精英

		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{		
				id = root["id"].asUInt();	
				mopUpType = root["mopUpType"].asUInt();	
				for(UInt32 i=0;i<root["useList"].size();i++)
				{
					useList.insertTail(root["useList"][i].asUInt());
				}
			}			
		}
		LYNX_MESSAGE_1(CHAPTER_MOP_UP_REQ,CGStageMopUp,std::string, jsonStr);
	};

	struct StageMopUpResp
	{
		StageMopUpResp(): id(0),mopUpType(0),result(0),leftTimes(0),mopupTimes(0){}
		UInt32 id;
		UInt32 mopUpType;
		UInt32 result;
		UInt32 leftTimes;//剩余次数	
		UInt32 mopupTimes;
		Json::Value allAttr;//发送物品属性
		List<UInt32> foodList ;//食物数量

		List<Goods> fixedList;
		List<Award> awards;
		List<Goods> cost;
		List<Card> cards;
		List<Goods> monsterDropList;
		List<Award> firstAwards;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["id"] = Json::Value(id );
			root["mopUpType"] = Json::Value(mopUpType);
			root["result"] = Json::Value(result );		
			root["leftTimes"] = Json::Value(leftTimes);
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

			for(List<Goods>::Iter * iter = fixedList.begin(); iter != NULL; iter = fixedList.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["fixedList"].append(son);
			}

			for(List<Goods>::Iter * iter = cost.begin(); iter != NULL; iter = cost.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["cost"].append(son);
			}


			for (List<UInt32>::Iter *iter = foodList.begin();iter != NULL; iter = foodList.next(iter))
			{
				root["foodList"].append(iter->mValue);
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


	struct WelfareMarket
	{
		UInt32 id;//类型
		UInt32 difficulty;//开启的最高难度		
		UInt32 finishTimes;//已完成次数
		UInt32 maxTimes;//可完成次数
		UInt32 times;//奖励倍数
	};
	struct WelfareResp	
	{
		List<WelfareMarket> welfaleMarket;//招福集市数据

		std::string convertDataToJson()
		{
			Json::Value root; 
			for(List<WelfareMarket>::Iter *it = welfaleMarket.begin();it!=NULL;it= welfaleMarket.next(it))
			{
				Json::Value son; 
				son["id "] = Json::Value(it->mValue.id );
				son["difficulty"] = Json::Value(it->mValue.difficulty);				
				son["finishTimes"] = Json::Value(it->mValue.finishTimes);
				son["maxTimes"] = Json::Value(it->mValue.maxTimes);
				son["times"] = Json::Value(it->mValue.times);
				root["welfaleMarket"].append(son);
			}		

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	struct CGWelfareBegin
	{
		CGWelfareBegin(): id (0),difficulty (0){}

		UInt32 id;//类型
		UInt32 difficulty;//难度
		
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				id  = root["id"].asUInt();	
				difficulty  = root["difficulty"].asUInt();
			}			
		}
		LYNX_MESSAGE_1(WELFALE_MARKET_BEGIN_REQ, CGWelfareBegin,std::string, jsonStr);
	};
	struct WelfareBeginResp	
	{
		WelfareBeginResp(): id (0),difficulty (0),result (0){}

		UInt32 id;//类型
		UInt32 difficulty;//难度
		UInt32 result;//结果

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["id"] = Json::Value(id );
			root["difficulty"] = Json::Value(difficulty);
			root["result"] = Json::Value(result);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	struct CGWelfareEnd
	{
		CGWelfareEnd(): doType(0),id(0),difficulty(0){}
	
		UInt32 doType;//0扫荡 1完成挑战 2挑战中途离开 3挑战失败
		UInt32 id;//类型
		UInt32 difficulty;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				doType  = root["doType"].asUInt();	
				id  = root["id"].asUInt();
				difficulty  = root["difficulty"].asUInt();
			}			
		}
		LYNX_MESSAGE_1(WELFALE_MARKET_END_REQ, CGWelfareEnd,std::string, jsonStr);
	};
	struct WelfareEndResp	
	{
		WelfareEndResp(): id(0),doType(0),difficulty(0),result(0){}

		UInt32 id;//类型
		UInt32 doType;//0扫荡 1完成挑战 2挑战中途离开 3挑战失败
		UInt32 difficulty;
		UInt32 result;//
		UInt32 chaiCount;
		UInt32 miCount;
		UInt32 youCount;
		UInt32 yanCount;


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
			root["doType"] = Json::Value(doType );		
			root["difficulty"] = Json::Value(difficulty);
			root["result"] = Json::Value(result);
			root["chaiCount"] = Json::Value(chaiCount);
			root["miCount"] = Json::Value(miCount);
			root["youCount"] = Json::Value(youCount);
			root["yanCount"] = Json::Value(yanCount);

			Json::Value son; 
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
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	class FireConfirmManager: public Singleton<FireConfirmManager>
	{
	public:
		FireConfirmManager();
		virtual ~FireConfirmManager();

		bool initial(Player* player);

		void release();

		void ChoiseNFromEnum(UInt32 num  ,UInt32 maxnum,List<UInt32> &OutList);		

		UInt32 GetConfirmTimes(UInt32 ConfirmLevel);

		void UpdataConfirmLevel(ConnId connId,bool flag);

		void SendFireConfirmCondition(const ConnId& connId);

		UInt32 CheckCopyFinishTime(Player* player);

		void SetCopyStart(Player* player,UInt32 CopyID);

		static void onConfirmDataReq(const  ConnId& , CGConfirmDataReq& );

		void ConfirmDataReq(const ConnId& connId, CGConfirmDataReq& msg);

		void SendForbiddenLogin(const ConnId& connId);

		void updateFireConfirmData(const ConnId& connId);

		
		static void onStartCopy(const  ConnId& ,CGChapterStart & );		//挑战关卡

		static void onLeaveCopy(const  ConnId& , CGChapterLeave& );		//离开关卡

		static void onEndCopy(const  ConnId& , CGChapterEnd & );		//关卡结束 

		static void onRelive(const  ConnId& , CGRelive & );		//复活

	    static void onisRecieve(const  ConnId& connId, CGisRecievedReq & msg);

		static void onChapterAwardCard(const  ConnId& ,CGChapterAwardCard & );		//翻牌

		static void onStageMopUp(const  ConnId& ,CGStageMopUp & );		//关卡扫荡
		
		void getAwardOnce(const  ConnId& connId,UInt32 copyID);//扫荡一次获取掉落

		
		void foodGain(const  ConnId& connId, UInt32 foodType);//美食奖励加成
		

		void getChapterCounter(const  ConnId& connId);//获取关卡次数

		void saveAndGetResult(const  ConnId& connId,ChapterEndResp &resp,UInt32 fanPaiType);

		static void onFoodsReq(const  ConnId&  ,CGFoods & );

		void getfoods(UInt32 num,List<UInt32> &foods);//获取随机值

		static void onStrengthReq(const  ConnId&  ,CGStrength & );

		void onStageSaodangReq(const  ConnId&  ,CGFoods &);//扫荡



		static void onWelfaleBeginReq(const  ConnId&  ,CGWelfareBegin & );
		static void onWelfaleEndReq(const  ConnId&  ,CGWelfareEnd & );

		UInt32 refreshgetStrength(const ConnId& connId);//刷新并获取体力

		UInt32 getBuyStrengthPrice(Guid playerID,UInt32 nowBuyTimes,UInt32 &cost,UInt32 &num);//获取购买体力价格

		UInt32 getBuyCoinPrice(Guid playerID,UInt32 nowBuyTimes,UInt32 &cost,UInt32 &num);


		
	private:

		PlayerFireConfirmData FireConfirmData;
	};

	enum ConfirmErrorTimes
	{
		CONFIRM_LEVEL1_ERROR_TIMES	=1,//监控等级对应的允许作弊次数
		CONFIRM_LEVEL2_ERROR_TIMES	=3,
		CONFIRM_LEVEL3_SEND_MAIL	=5,
		CONFIRM_LEVEL3_FORBID_LOGIN =10,
		CONFIRM_LEVEL_RESTET_TIMES  =50,
	};
	enum ConfirmNeedNum
	{
		CONFIRM_LEVEL1_NEED_NUM	=2,//监控等级对应的监控个数
		CONFIRM_LEVEL2_NEED_NUM	=5,
		CONFIRM_LEVEL3_NEED_NUM	=10,

	};
	enum Confirm_Level
	{
		CONFIRM_LEVEL1=1,//监控等级 3个等级
		CONFIRM_LEVEL2=2,
		CONFIRM_LEVEL3=3,
	};
	enum ConfirmCondition
	{
		//移动速度、攻击速度、技能CD、技能伤害、暴击率、暴击伤害最大值、技能范围、位置点、HP、MP、SP、buff、副本结束时间
		CONFIRM_SPEED							=0,
		CONFIRM_ATTACK_RATE						=1,
		CONFIRM_SKILL_CD						=2,
		CONFIRM_SKILL_DAMAGE					=3,
		CONFIRM_CRITICAL_STRIKE_RATE			=4, //暴击率
		CONFIRM_CRITICAL_STRIKE_MAX_DAMAGE		=5,//暴击伤害最大值
		CONFIRM_SKILL_RANGE						=6,
		CONFIRM_POS								=7,//位置点
		CONFIRM_HP								=8,
		CONFIRM_MP								=9,
		CONFIRM_SP								=10,
		CONFIRM_BUFFER							=11,

		CONFIRM_MAX								=12,
	};

	
}
#endif // __LYNX_GAME_SERVER_GAME_MANAGER_FIRE_CONFIRM_MANAGER_H__
