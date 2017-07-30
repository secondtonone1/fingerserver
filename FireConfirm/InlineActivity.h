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

	enum InlineActivityState
	{
		Activity_CLOSE =		 0,//		
		Activity_WAITE =		 1,//		
		Activity_OPEN =		 2,//		
		Activity_END =			3,//			
	};


	struct Activity
	{
		Activity(): id(0),state(Activity_OPEN),data(""){}

		UInt32 id;
		UInt32 state;
		String data;
		LYNX_S11N_3(Activity,UInt32,id,UInt32,state,String, data);

	};

	struct OneActivityReq
	{
		OneActivityReq(): reqType(0),id(0),index(0){}
		UInt32 reqType;//成长基金0请求信息 1购买 2领取	月签到 0请求信息 1签到		在线时长 0请求信息 1领取		全民福利 0请求信息 1领取	7天登录 0请求信息 1领取	
		//7天活动 0请求信息 1 领取优良奖励 3 领取任务奖励 7领取所有奖励		分时段奖励 0请求信息 1领取 
		UInt32 id;//
		UInt32 index;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
				id = root["id"].asUInt();				
				index = root["index"].asUInt();
			}
		}

		LYNX_MESSAGE_1(ONE_ACTIVITY_REQ, OneActivityReq,std::string, strReceive);
	};

	struct OneActivityResp
	{
		List<Activity> activitys;

		std::string convertDataToJson()
		{
			Json::Value root;   		
			for(List<Activity>::Iter * iter = activitys.begin(); iter != NULL; iter = activitys.next(iter))
			{
				Json::Value son;	
				son["id"] = Json::Value(iter->mValue.id);
				son["state"] = Json::Value(iter->mValue.state);
				son["data"] = Json::Value(iter->mValue.data.c_str());

				root["activitys"].append(son);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
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

// 		LYNX_MESSAGE_1(GROWTH_FOUND_REQ, GrowFoundReq,std::string, strReceive);
	};




	struct GrowFoundResp
	{
		GrowFoundResp(): reqType(0),result(0),gold(0),buyFlag(0),cost(0),vipLv(0),state(Activity_OPEN){}

		UInt32 reqType;
		UInt32 result;
		UInt32 gold ;
		UInt32 buyFlag;
		UInt32 cost;
		UInt32 vipLv;
		UInt32 state;
		Json::Value allAttr;
		List<UInt32> gotList;


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

			root["allAttr"] = Json::Value(allAttr);
		

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

// 		LYNX_MESSAGE_1(MONTH_SIGN_REQ, MonthSingnReq,std::string, strReceive);
	};

	struct MonthSingnResp
	{
		MonthSingnResp(): reqType(0),result(0),signCount(0),canSignCount(0){}

		UInt32 reqType;
		UInt32 result;
		UInt32 signCount;
		UInt32 canSignCount;
		Json::Value allAttr;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["signCount"] = Json::Value(signCount);
			root["canSignCount"] = Json::Value(canSignCount);

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

	struct OnlineWelFareReq
	{
		OnlineWelFareReq(): reqType(0), id(0), isLogin(0){}
		UInt32 reqType;//0请求信息 1签到
		UInt32 id;
		UInt32 isLogin;

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

// 		LYNX_MESSAGE_1(ONLINE_WELFARE_REQ , OnlineWelFareReq,std::string, strReceive);
	};

	struct OnlineWelFareResp
	{
		OnlineWelFareResp(): reqType(0),result(0),onlineTime(0),state(Activity_OPEN){}

		UInt32 reqType;
		UInt32 result;
		UInt32 onlineTime;
		UInt32 state;
		Json::Value allAttr;
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

// 		LYNX_MESSAGE_1(PEOPLE_WELFARE_REQ , PeopleWelfareReq,std::string, strReceive);
	};

	struct PeopleWelfareResp
	{
		PeopleWelfareResp(): reqType(0),result(0),rechargeNum(0),serverFirstOpenTime(0),state(Activity_OPEN){}

		UInt32 reqType;
		UInt32 result;
		UInt32 rechargeNum ;
		UInt32 state;
		UInt32 serverFirstOpenTime;
		List<UInt32> gotList;
		List<Goods> ends;

		Json::Value allAttr;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["rechargeNum"] = Json::Value(rechargeNum);
			root["serverFirstOpenTime"] = Json::Value(serverFirstOpenTime);

// 			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
// 			{
// 				Json::Value son;	
// 				son.append(iter->mValue.resourcestype);
// 				son.append(iter->mValue.subtype);
// 				son.append(iter->mValue.num);
// 				root["ends"].append(son);
// 			}
			for(List<UInt32>::Iter * it = gotList.begin(); it != NULL; it = gotList.next(it))
			{
				Json::Value son;	
				root["gotList"].append(it->mValue);
			}
			root["allAttr"] = Json::Value(allAttr);

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

// 		LYNX_MESSAGE_1(SEVENDAY_LOGIN_AWARD_REQ  , SevenLoginAwardReq,std::string, strReceive);
	};

	struct SevenLoginAwardResp
	{
		SevenLoginAwardResp(): reqType(0),result(0),signCount(0),canSignCount(0),state(Activity_OPEN){}

		UInt32 reqType;
		UInt32 result;
		UInt32 signCount ;
		UInt32 canSignCount ;
		UInt32 state;
		Json::Value allAttr;
// 		List<UInt32> gotList;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["signCount"] = Json::Value(signCount);
			root["canSignCount"] = Json::Value(canSignCount);
// 
// 			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
// 			{
// 				Json::Value son;	
// 				son.append(iter->mValue.resourcestype);
// 				son.append(iter->mValue.subtype);
// 				son.append(iter->mValue.num);
// 				root["ends"].append(son);
// 			}
// 			for(List<UInt32>::Iter * it = gotList.begin(); it != NULL; it = gotList.next(it))
// 			{
// 				Json::Value son;	
// 				son.append(it->mValue);			
// 				root["gotList"].append(son);
// 			}
			root["allAttr"] = Json::Value(allAttr);

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

// 		LYNX_MESSAGE_1(SEVENDAY_TASK_REQ, SevenDayTaskReq,std::string, strReceive);
	};

	struct SevenDayTaskResp
	{
		SevenDayTaskResp(): reqType(0),result(0),firstLoginTime(0),day(0),dayLeftTime(0),state(Activity_OPEN){}

		UInt32 reqType;
		UInt32 result;
		UInt32 firstLoginTime ;
		List<KeyValue> gotList;
		List<Goods> ends;
		List<KeyList> finishList;
		UInt32 day;
		UInt32 dayLeftTime;
		UInt32 state;
		Json::Value allAttr;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["firstLoginTime"] = Json::Value(firstLoginTime);
			root["day"] = Json::Value(day);
			root["dayLeftTime"] = Json::Value(dayLeftTime);
// 
// 			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
// 			{
// 				Json::Value son;	
// 				son.append(iter->mValue.resourcestype);
// 				son.append(iter->mValue.subtype);
// 				son.append(iter->mValue.num);
// 				root["ends"].append(son);
// 			}
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
			root["allAttr"] = Json::Value(allAttr);


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

// 		LYNX_MESSAGE_1(TIME_AWARD_REQ, TimeAwardReq,std::string, strReceive);
	};

	struct TimeAwardResp
	{
		TimeAwardResp(): reqType(0),result(0),strengthNum(0){}

		UInt32 reqType;
		UInt32 result;
		UInt32 strengthNum;
		Json::Value allAttr;
		List<UInt32> gotList;
		List<Goods> ends;
		

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["strengthNum"] = Json::Value(strengthNum);
// 
// 			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
// 			{
// 				Json::Value son;	
// 				son.append(iter->mValue.resourcestype);
// 				son.append(iter->mValue.subtype);
// 				son.append(iter->mValue.num);
// 				root["ends"].append(son);
// 			}
			for(List<UInt32>::Iter * it = gotList.begin(); it != NULL; it = gotList.next(it))
			{
				if (it->mValue != 0)
				{
					root["gotList"].append(it->mValue);
				}				
			}
			root["allAttr"] = Json::Value(allAttr);


			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct ActivityResp
	{
		List<Activity> activitys;

		std::string convertDataToJson()
		{
			Json::Value root;   		
			for(List<Activity>::Iter * iter = activitys.begin(); iter != NULL; iter = activitys.next(iter))
			{
				Json::Value son;	
				son["id"] = Json::Value(iter->mValue.id);
				son["state"] = Json::Value(iter->mValue.state);
				son["data"] = Json::Value(iter->mValue.data.c_str());

// 				son["id"].append(iter->mValue.id);
// 				son.append(iter->mValue.state);
// 				son.append(iter->mValue.data.c_str());
				root["activitys"].append(son);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	
	class InlineActivityManager: public Singleton<InlineActivityManager>
	{
	
	public:
		static void onOneActivityReq(const  ConnId& ,OneActivityReq & );

		void getgoodsList(String awardStr1,String awardStr2,String awardStr3,List<Goods> &goodsList);

		void inlineActivitySave(Guid playerId,UInt32 type, PlayerInlineActivityData inlineActivityData);

		void incRechargeNum(Guid playerId);
		void checkLogin(Guid playerId);
		void initPlayerFirstLogin(UInt64 playerID);

		void checkLogOut(Guid playerId,UInt32 closeTime);

		UInt32 checkSevenDayTaskClose(const  ConnId& connId,UInt32 reqType);

		String stringToUtf(const String& str);

		void updateSevenDayTaskData(Guid playerId,UInt64 eventType,   UInt32 change);

		void getIdByEventType(UInt64 eventType, List<UInt32>  &idList);

		UInt32 checkSevenDayCanGotIt(UInt32 day,UInt32 id, PlayerInlineActivityData &inlineActivityData,List<Goods> &itemList);

		void getSevenDayAllAward( PlayerInlineActivityData &inlineActivityData,List<Goods> &itemList);

		void checkSevenDayTaskFinish( PlayerInlineActivityData &inlineActivityData);

// 		void addSevenDayTaskFinish( PlayerInlineActivityData &inlineActivityData);


		void sendActivitys(const  ConnId& connId);

		void growFoundReq(const  ConnId& connId,GrowFoundReq & req,GrowFoundResp &resp);


		void monthSingnReq(const  ConnId& ,MonthSingnReq & ,MonthSingnResp & );

		void onlineWelFareReq(const  ConnId& ,OnlineWelFareReq & ,OnlineWelFareResp & );

		void peopleWelfareReq(const  ConnId& ,PeopleWelfareReq & ,PeopleWelfareResp & );

		void sevenLoginAwardReq(const  ConnId& ,SevenLoginAwardReq &,SevenLoginAwardResp & );

		void sevenDayTaskReq(const  ConnId& ,SevenDayTaskReq & ,SevenDayTaskResp & );

		void timeAwardReq(const  ConnId& ,TimeAwardReq &,TimeAwardResp & );

		void checkOnlineWelfareLastTime(Player *player);

				
	};



}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_INLINE_ACTIVITY_MANAGER_H__
