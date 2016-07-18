#ifndef __LYNX_COMMON_LIB_ACHIEVEDAILY_PACKET_H__
#define __LYNX_COMMON_LIB_ACHIEVEDAILY_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_ACHIEVEDAILYLIST_REQ					= sAchieveDailyBase + 1,
		BOC_ACHIEVEDAILYLIST_RESP					= sAchieveDailyBase + 2,
		BOC_ACHIEVEFINISH_REQ							= sAchieveDailyBase + 3,
		BOC_ACHIEVEFINISH_RESP                        = sAchieveDailyBase + 4,
		BOC_DAILYTASKADD_REQ							= sAchieveDailyBase + 5,
		BOC_DAILYTASKADD_RESP						= sAchieveDailyBase + 6,
		BOC_DAILYTASKFINISH_REQ						= sAchieveDailyBase + 7,
		BOC_DAILYTASKFINISH_RESP					= sAchieveDailyBase + 8,
		BOC_ACTIVEAWARDGET_REQ					= sAchieveDailyBase + 9,
		BOC_ACTIVEAWARDGET_RESP					= sAchieveDailyBase + 10

	};

	//-----------------CG-------------------
	//请求日常任务和成就列表
	struct CGAchieveDailyListReq
	{
		CGAchieveDailyListReq(): mPacketID(BOC_ACHIEVEDAILYLIST_REQ), mPlayerUid(0) {}

		UInt16	mPacketID;
		UInt64	mPlayerUid;

		LYNX_MESSAGE_2(BOC_ACHIEVEDAILYLIST_REQ, CGAchieveDailyListReq, UInt16, mPacketID, UInt64, mPlayerUid);
	};

	struct CGAchieveFinishReq
	{
		CGAchieveFinishReq(): mPacketID(BOC_ACHIEVEFINISH_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_ACHIEVEFINISH_REQ, CGAchieveFinishReq, UInt16, mPacketID, std::string, mReqJsonStr);

	};

	struct CGDailyTaskAddReq
	{
		CGDailyTaskAddReq(): mPacketID(BOC_DAILYTASKADD_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_DAILYTASKADD_REQ, CGDailyTaskAddReq, UInt16, mPacketID, std::string, mReqJsonStr);

	};

	struct CGDailyTaskFinishReq
	{
		CGDailyTaskFinishReq(): mPacketID(BOC_DAILYTASKFINISH_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_DAILYTASKFINISH_REQ, CGDailyTaskFinishReq, UInt16, mPacketID, std::string, mReqJsonStr);

	};

	struct CGActiveAwardGetReq
	{
		CGActiveAwardGetReq(): mPacketID(BOC_ACTIVEAWARDGET_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_ACTIVEAWARDGET_REQ, CGActiveAwardGetReq, UInt16, mPacketID, std::string, mReqJsonStr);

	};


					

	

	//-----------------------------GC----------------------------------

	struct GCAchieveDailyListResp
	{
		GCAchieveDailyListResp():mPacketID(BOC_ACHIEVEDAILYLIST_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_ACHIEVEDAILYLIST_RESP,GCAchieveDailyListResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCAchieveFinishResp
	{
		GCAchieveFinishResp():mPacketID(BOC_ACHIEVEFINISH_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_ACHIEVEFINISH_RESP,GCAchieveFinishResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCDailyTaskAddResp
	{
		GCDailyTaskAddResp():mPacketID(BOC_DAILYTASKADD_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_DAILYTASKADD_RESP,GCDailyTaskAddResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCDailyTaskFinishResp
	{
		GCDailyTaskFinishResp():mPacketID(BOC_DAILYTASKFINISH_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_DAILYTASKFINISH_RESP,GCDailyTaskFinishResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCActiveAwardGetResp
	{
		GCActiveAwardGetResp():mPacketID(BOC_ACTIVEAWARDGET_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_ACTIVEAWARDGET_RESP,GCActiveAwardGetResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_ACHIEVEDAILY_PACKET_H__

