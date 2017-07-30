#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_CODE_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_CODE_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "../PassportThreadMsg.h"
#include "../PassportSystem.h"
#include "Gift.h"


namespace Lynx
{

	struct CodeReq
	{
		CodeReq(): reqType(0), code(""){}
		UInt32 reqType;//
		string code;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
				code = root["code"].asString();
			}
		}

		LYNX_MESSAGE_1(BOC_CODE_REQ, CodeReq,std::string, strReceive);
	};




	struct CodeResp
	{
		CodeResp(): reqType(0),result(0){}

		UInt32 reqType;
		UInt32 result;				
// 		List<Goods> fixedList;
// 		List<Goods> ends;
// 		List<Goods> cost;
// 		List<Award> awards;

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);

// 			for(List<Award>::Iter * it = awards.begin(); it != NULL; it = awards.next(it))	
// 			{
// 				Json::Value son;
// 				for(List<Goods>::Iter * iter = it->mValue.award.begin(); iter != NULL; iter = it->mValue.award.next(iter))
// 				{
// 					Json::Value leaf;
// 					leaf.append(iter->mValue.resourcestype);
// 					leaf.append(iter->mValue.subtype);
// 					leaf.append(iter->mValue.num);
// 
// 					son["award"].append(leaf);
// 				}
// 				root["awards"].append(son);
// 			}
// 
// 			for(List<Goods>::Iter * iter = fixedList.begin(); iter != NULL; iter = fixedList.next(iter))
// 			{
// 				Json::Value son;	
// 				son.append(iter->mValue.resourcestype);
// 				son.append(iter->mValue.subtype);
// 				son.append(iter->mValue.num);
// 				root["fixedList"].append(son);
// 			}
// 			for(List<Goods>::Iter * iter = cost.begin(); iter != NULL; iter = cost.next(iter))
// 			{
// 				Json::Value son;	
// 				son.append(iter->mValue.resourcestype);
// 				son.append(iter->mValue.subtype);
// 				son.append(iter->mValue.num);
// 				root["cost"].append(son);
// 			}
// 
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


	struct FishEatReq
	{
		FishEatReq(): kickCount(0), fishEat(0){}
		UInt32 kickCount;//
		UInt32 fishEat;

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				kickCount = root["kickCount"].asUInt();
				fishEat = root["fishEat"].asUInt();
			}
		}

		LYNX_MESSAGE_1(BOC_FISHEAT_REQ, FishEatReq,std::string, strReceive);
	};




	struct FishEatResp
	{
		FishEatResp(): result(0),coin(0),fishEatTimes(0){}

		UInt32 result;
		UInt32 coin;	
		UInt32 fishEatTimes;
		

		std::string convertDataToJson()
		{
			Json::Value root;   
			root["result"] = Json::Value(result);
			root["coin"] = Json::Value(coin);
			root["fishEatTimes"] = Json::Value(fishEatTimes);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	
	class CodeManager: public Singleton<CodeManager>
	{
	
	public:
		static void onCodeReq(const  ConnId& ,CodeReq & );

		void codeResp(PassportCodeResp msg);

		static void onFishEatReq(const  ConnId& ,FishEatReq & );

		void fishEatResp(FishEatResp msg);

		
	};



}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_CODE_MANAGER_H__
