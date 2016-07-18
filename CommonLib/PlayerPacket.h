#ifndef __LYNX_COMMON_LIB_PLAYER_PACKET_H__
#define __LYNX_COMMON_LIB_PLAYER_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_PLAYER_BASE_INFO_REQ					= sPlayerMsgBase + 1,
		BOC_PLAYER_BASE_INFO_RESP					= sPlayerMsgBase + 2,

		BOC_PLAYER_DETAIL_REQ						= sPlayerMsgBase + 3,
		BOC_PLAYER_DETAIL_RESP						= sPlayerMsgBase + 4,

		BOC_DAILYRESET_REQ						= sPlayerMsgBase + 20,
		BOC_DAILYRESET_RESP						= sPlayerMsgBase + 21,

		BOC_CODE_REQ						= sPlayerMsgBase + 100,
		BOC_CODE_RESP						= sPlayerMsgBase + 101,
		
	};

	//-----------------CG-------------------
	struct CGPlayerBaseInfoReq
	{
		CGPlayerBaseInfoReq(): mPacketID(BOC_PLAYER_BASE_INFO_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;
	};
	struct CGPlayerDetailReq
	{
		CGPlayerDetailReq(): mPacketID(BOC_PLAYER_DETAIL_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;
	};


	//-----------------GC-------------------
	struct GCPlayerBaseInfoResp
	{
		GCPlayerBaseInfoResp(): mPacketID(BOC_PLAYER_BASE_INFO_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_BASE_INFO_RESP, GCPlayerBaseInfoResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCPlayerDetailResp
	{
		GCPlayerDetailResp(): mPacketID(BOC_PLAYER_DETAIL_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_DETAIL_RESP, GCPlayerDetailResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};


	struct DailyResetResp
	{
		DailyResetResp(): mPacketID(BOC_PLAYER_DETAIL_RESP),dailyresetHour(0){}

		UInt16	mPacketID;
		UInt32 dailyresetHour;
		std::string	mRespJsonStr;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["dailyresetHour"] = Json::Value(dailyresetHour);
		
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
		LYNX_MESSAGE_2(BOC_DAILYRESET_RESP, DailyResetResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_PLAYER_PACKET_H__
