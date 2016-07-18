#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_GIFT_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_GIFT_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
namespace Lynx
{

	class Player;
	class AwardTemplate;



	struct GiftReq
	{
		GiftReq(): giftID(0),giftNum(0){}

		UInt32 giftID;
		UInt32 giftNum;
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				giftID = root["giftID"].asUInt();
				giftNum = root["giftNum"].asUInt();
			}
		}

		LYNX_MESSAGE_1(OPEN_GIFT_REQ, GiftReq,std::string, strReceive);

	}

	struct GiftResp
	{
		GiftResp(): ID(0){}

		UInt32 ID;
		List<Goods> goodsList;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["ID"] = Json::Value(ID);

			for(List<Goods>::Iter * iter = goodsList.begin(); iter != NULL; iter = goodsList.next(iter))
			{
				root["resourcestype"].append(iter->mValue.resourcestype);
				root["subtype"].append(iter->mValue.subtype);
				root["num"].append(iter->mValue.num);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

	};

	struct ChangResp
	{
		ChangResp():ChangType(0), Value(0){}
		UInt32 ChangType;//1金币 2元宝 3 4 5 6 7 
		Int Value;

		std::string convertDataToJson()
		{
			Json::Value root;    
			root["ChangType"] = Json::Value(ChangType);
			root["Value"] = Json::Value(Value);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

	};

	class GiftManager
	{
		static void onOpenGiftReq(const  ConnId& ,GiftReq & );
		// 		 static void onOpenBoxReq(const  ConnId& ,BoxResp & );
	};










}


#endif // __LYNX_GAME_SERVER_GAME_MANAGER_GIFT_MANAGER_H__
