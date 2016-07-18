#ifndef __LYNX_COMMON_LIB_MSG_FIREINFO_TO_JSON_H__
#define __LYNX_COMMON_LIB_MSG_FIREINFO_TO_JSON_H__

#include "Data.h"
#include "../PlatformLib/StringConverter.h"
#include "../jsoncpp/include/json/json.h"
#include "../FireConfirm/FireConfirmManager.h"

namespace Lynx
{
	struct GCServerTimeResp
	{
		GCServerTimeResp(): mPacketID(SYNC_TIME_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(SYNC_TIME_RESP, GCServerTimeResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};


	struct MsgServerTimeRespToJson
	{
		MsgServerTimeRespToJson():mClientTime(0),mServerTime(0) {};

		UInt32 mClientTime;
		UInt32 mServerTime;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["ServerTime"] = Json::Value(mServerTime);
			root["ClientTime"] = Json::Value(mClientTime);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

		
	};


	struct FireConfirmDataNotifyToJson	
	{
		FireConfirmDataNotifyToJson(): mConfirmLevel(0),m_NormalTimes(0),m_SpecialTimes(0),m_EliteTimes(0),m_Times(0){}

		// 		Guid mPlayerGuid;
		UInt32 mConfirmLevel;
		//复活使用次数 免费普通  
		UInt32 m_NormalTimes;
		//特别 
		UInt32 m_SpecialTimes;
		//挑战
		UInt32 m_EliteTimes;
		//不免费使用次数
		UInt32 m_Times;

		List<UInt32> ConfirmIDs;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["mConfirmLevel"] = Json::Value(m_NormalTimes);
			root["m_NormalTimes"] = Json::Value(mConfirmLevel);
			root["m_SpecialTimes"] = Json::Value(m_SpecialTimes);
			root["m_EliteTimes"] = Json::Value(m_EliteTimes);
			root["m_Times"] = Json::Value(m_Times);

			for(List<UInt32>::Iter * iter = ConfirmIDs.begin(); iter != NULL; iter = ConfirmIDs.next(iter))
			{
				root["ConfirmIDs"].append(Json::Value(iter->mValue));

			}
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				// 				mClientTime = root["ClientTime"].asUInt();
			}
		}
	};


	

	struct FireConfirmResultNotifyToJson	
	{
		FireConfirmResultNotifyToJson(): mConfirmLevel(0),ConfirmResult(0){}

		// 		Guid mPlayerGuid;
		UInt32 mConfirmLevel;
		UInt32 ConfirmResult;//0成功 1失败 2 禁止登陆

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["mConfirmLevel"] = Json::Value(mConfirmLevel);
			root["ConfirmResult"] = Json::Value(ConfirmResult);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				// 				mClientTime = root["ClientTime"].asUInt();
			}
		}
	};


	// FobiddenLogin
	struct ForbiddenLoginToJson	
	{
		ForbiddenLoginToJson(): ForbiddenLoginType(0),ForbiddenLoginTime(0){}

		UInt32 ForbiddenLoginType;
		UInt32 ForbiddenLoginTime;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["ForbidenLoginType"] = Json::Value(ForbiddenLoginType);
			root["ForbidenLoginTime"] = Json::Value(ForbiddenLoginTime);
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}

		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				// 				mClientTime = root["ClientTime"].asUInt();
			}
		}
	};

	

} // namespace Lynx

#endif//
