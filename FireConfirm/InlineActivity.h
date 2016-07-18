#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_INLINE_ACTIVITY_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_INLINE_ACTIVITY_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "../PassportThreadMsg.h"
#include "../PassportSystem.h"
#include "Gift.h"


namespace Lynx
{
	enum InlineActivity
	{
		GROWTH_FOUND =		 1,//	成长基金	
		MONTH_SIGN =		 2,//	月签到	
		ONLINE_WELFARE =	 3,//	在线奖励	
		PEOPLE_WELFARE =	 4,//	全民福利	
		SEVENDAY_LOGIN_AWARD =	5,//	七日登陆奖励	
		SEVENDAY_TASK =		 6,//	纳宗七日训	
		TIME_AWARD =		 7,//	分时段奖励（三庆便当）	
		SEVENDAY_FINISHTASK =		 16,//	纳宗七日训完成的任务
	};

	struct GrowFoundReq
	{
		GrowFoundReq(): reqType(0),id(0){}
		UInt32 reqType;//0请求信息 1购买 2领取
		UInt32 id;//

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

		LYNX_MESSAGE_1(GROWTH_FOUND_REQ, GrowFoundReq,std::string, strReceive);
	};




	struct GrowFoundResp
	{
		GrowFoundResp(): reqType(0),result(0),gold(0),buyFlag(0),cost(0),vipLv(0){}

		UInt32 reqType;
		UInt32 result;
		UInt32 gold ;
		UInt32 buyFlag;
		UInt32 cost;
		UInt32 vipLv;

		List<UInt32> gotList;
// 		List<Goods> ends;


		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["gold"] = Json::Value(gold);
			root["buyFlag"] = Json::Value(buyFlag);
			root["cost"] = Json::Value(cost);
			root["vipLv"] = Json::Value(vipLv);

			for(List<UInt32>::Iter * it = gotList.begin(); it != NULL; it = gotList.next(it))
			{
				Json::Value son;	
				root["gotList"].append(it->mValue);
			}

// 			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
// 			{
// 				Json::Value son;	
// 				son.append(iter->mValue.resourcestype);
// 				son.append(iter->mValue.subtype);
// 				son.append(iter->mValue.num);
// 				root["ends"].append(son);
// 			}
		

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};




	struct MonthSingnReq
	{
		MonthSingnReq(): reqType(0){}
		UInt32 reqType;//0请求信息 1签到

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

		LYNX_MESSAGE_1(MONTH_SIGN_REQ, MonthSingnReq,std::string, strReceive);
	};

	struct MonthSingnResp
	{
		MonthSingnResp(): reqType(0),result(0),signCount(0),canSignCount(0){}

		UInt32 reqType;
		UInt32 result;
		UInt32 signCount;
		UInt32 canSignCount;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["signCount"] = Json::Value(signCount);
			root["canSignCount"] = Json::Value(canSignCount);

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

	struct OnlineWelFareReq
	{
		OnlineWelFareReq(): reqType(0), id(0){}
		UInt32 reqType;//0请求信息 1签到
		UInt32 id;

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

		LYNX_MESSAGE_1(ONLINE_WELFARE_REQ , OnlineWelFareReq,std::string, strReceive);
	};

	struct OnlineWelFareResp
	{
		OnlineWelFareResp(): reqType(0),result(0),onlineTime(0){}

		UInt32 reqType;
		UInt32 result;
		UInt32 onlineTime;
		List<UInt32> gotList;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["onlineTime"] = Json::Value(onlineTime);

			for(List<UInt32>::Iter * it = gotList.begin(); it != NULL; it = gotList.next(it))
			{
				Json::Value son;	
				root["gotList"].append(it->mValue);
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

	struct PeopleWelfareReq
	{
		PeopleWelfareReq(): reqType(0),id(0){}
		UInt32 reqType;//0请求信息 1签到
		UInt32 id;

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

		LYNX_MESSAGE_1(PEOPLE_WELFARE_REQ , PeopleWelfareReq,std::string, strReceive);
	};

	struct PeopleWelfareResp
	{
		PeopleWelfareResp(): reqType(0),result(0),rechargeNum(0){}

		UInt32 reqType;
		UInt32 result;
		UInt32 rechargeNum ;
		List<UInt32> gotList;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["rechargeNum"] = Json::Value(rechargeNum);

			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["ends"].append(son);
			}
			for(List<UInt32>::Iter * it = gotList.begin(); it != NULL; it = gotList.next(it))
			{
				Json::Value son;	
				root["gotList"].append(it->mValue);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct SevenLoginAwardReq
	{
		SevenLoginAwardReq(): reqType(0),id(0){}
		UInt32 reqType;//0请求信息 1签到
		UInt32 id;

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

		LYNX_MESSAGE_1(SEVENDAY_LOGIN_AWARD_REQ  , SevenLoginAwardReq,std::string, strReceive);
	};

	struct SevenLoginAwardResp
	{
		SevenLoginAwardResp(): reqType(0),result(0),signCount(0),canSignCount(0){}

		UInt32 reqType;
		UInt32 result;
		UInt32 signCount ;
		UInt32 canSignCount ;
// 		List<UInt32> gotList;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["signCount"] = Json::Value(signCount);
			root["canSignCount"] = Json::Value(canSignCount);

			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["ends"].append(son);
			}
// 			for(List<UInt32>::Iter * it = gotList.begin(); it != NULL; it = gotList.next(it))
// 			{
// 				Json::Value son;	
// 				son.append(it->mValue);			
// 				root["gotList"].append(son);
// 			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct SevenDayTaskReq
	{
		SevenDayTaskReq(): reqType(0),id(0),day(0){}
		UInt32 reqType;//0请求信息 1领取 2 已结束 3 领取任务奖励
		UInt32 id;
		UInt32 day;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
				id = root["id"].asUInt();
				day = root["day"].asUInt();
			}
		}

		LYNX_MESSAGE_1(SEVENDAY_TASK_REQ, SevenDayTaskReq,std::string, strReceive);
	};

	struct SevenDayTaskResp
	{
		SevenDayTaskResp(): reqType(0),result(0),firstLoginTime(0),day(0),dayLeftTime(0){}

		UInt32 reqType;
		UInt32 result;
		UInt32 firstLoginTime ;
		List<KeyValue> gotList;
		List<Goods> ends;
		List<KeyList> finishList;
		UInt32 day;
		UInt32 dayLeftTime;

		//achievementid
		//event
		//arg
		//flag

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["firstLoginTime"] = Json::Value(firstLoginTime);
			root["day"] = Json::Value(day);
			root["dayLeftTime"] = Json::Value(dayLeftTime);

			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["ends"].append(son);
			}
			for(List<KeyValue>::Iter * it = gotList.begin(); it != NULL; it = gotList.next(it))
			{
				Json::Value son;	
				son.append(it->mValue.key);
				son.append(it->mValue.value);
				root["gotList"].append(son);
			}
			for(List<KeyList>::Iter * it2 = finishList.begin(); it2 != NULL; it2 = finishList.next(it2))
			{
				Json::Value son;	
				son["id"] = Json::Value(it2->mValue.key);
				for(List<Goods>::Iter * it1 = it2->mValue.values.begin(); it1 != NULL; it1 = it2->mValue.values.next(it1))
				{
					Json::Value leaf;
					leaf.append(it1->mValue.resourcestype);
					leaf.append(it1->mValue.subtype);
					leaf.append(it1->mValue.num);	
					son["tasks"].append(leaf);
				}
				root["finishList"].append(son);
			}


			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct TimeAwardReq
	{
		TimeAwardReq(): reqType(0),id(0){}
		UInt32 reqType;//0请求信息 1签到
		UInt32 id;

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

		LYNX_MESSAGE_1(TIME_AWARD_REQ, TimeAwardReq,std::string, strReceive);
	};

	struct TimeAwardResp
	{
		TimeAwardResp(): reqType(0),result(0){}

		UInt32 reqType;
		UInt32 result;
		List<UInt32> gotList;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);

			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["ends"].append(son);
			}
			for(List<UInt32>::Iter * it = gotList.begin(); it != NULL; it = gotList.next(it))
			{
				if (it->mValue != 0)
				{
					root["gotList"].append(it->mValue);
				}				
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	
	class InlineActivityManager: public Singleton<InlineActivityManager>
	{
	
	public:
		static void onGrowFoundReq(const  ConnId& ,GrowFoundReq & );

		static void onMonthSingnReq(const  ConnId& ,MonthSingnReq & );

		static void onOnlineWelFareReq(const  ConnId& ,OnlineWelFareReq & );

		static void onPeopleWelfareReq(const  ConnId& ,PeopleWelfareReq & );

		static void onSevenLoginAwardReq(const  ConnId& ,SevenLoginAwardReq & );

		static void onSevenDayTaskReq(const  ConnId& ,SevenDayTaskReq & );

		static void onTimeAwardReq(const  ConnId& ,TimeAwardReq & );

		void getgoodsList(String awardStr1,String awardStr2,String awardStr3,List<Goods> &goodsList);

		void inlineActivitySave(Guid playerId,UInt32 type, PlayerInlineActivityData inlineActivityData);


		void incRechargeNum(Guid playerId);
		void checkLogin(Guid playerId);
		void checkLogOut(Guid playerId,UInt32 closeTime);

		UInt32 checkSevenDayTaskClose(const  ConnId& connId,UInt32 reqType);

		String stringToUtf(const String& str);

		void updateSevenDayTaskData(Guid playerId,UInt64 eventType,   UInt32 change);

		void getIdByEventType(UInt64 eventType, List<UInt32>  &idList);

		UInt32 checkSevenDayCanGotIt(UInt32 day,UInt32 id, PlayerInlineActivityData &inlineActivityData,List<Goods> &itemList);

		void getSevenDayAllAward( PlayerInlineActivityData &inlineActivityData,List<Goods> &itemList);

		void checkSevenDayTaskFinish( PlayerInlineActivityData &inlineActivityData);

		void addSevenDayTaskFinish( PlayerInlineActivityData &inlineActivityData);//for test
				
	};



}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_INLINE_ACTIVITY_MANAGER_H__
