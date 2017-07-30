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

		BOC_FISHEAT_REQ						= sPlayerMsgBase + 102,
		BOC_FISHEAT_RESP					= sPlayerMsgBase + 103,
		
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
// 		DailyResetResp():{}

		UInt16	mPacketID;
		List<UInt32> refreshList;//到时间点让客户端刷新的id列表
		UInt32 serverTime;
		std::string	mRespJsonStr;

		std::string convertDataToJson()
		{
			Json::Value root;     

			Json::Value son; 
			for (List<UInt32>::Iter * iter = refreshList.begin();iter!=NULL; iter= refreshList.next(iter))
			{
				son.append(iter->mValue);				
			}
			root["refreshList"] = son;
			root["serverTime"] = serverTime;
		
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
		LYNX_MESSAGE_2(BOC_DAILYRESET_RESP, DailyResetResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_PLAYER_PACKET_H__
