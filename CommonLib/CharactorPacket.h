#ifndef __LYNX_COMMON_LIB_CHARACTOR_PACKET_H__
#define __LYNX_COMMON_LIB_CHARACTOR_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_CHARACTORLIST_REQ					= sCharactorBase + 1,
		BOC_CHARACTORLIST_RESP					= sCharactorBase + 2,
		BOC_CHARACTORBUY_REQ					= sCharactorBase + 3,
		BOC_CHARACTORBUY_RESP					= sCharactorBase + 4,
		BOC_CHARACTORSWITCH_REQ				= sCharactorBase + 5,
		BOC_CHARACTORSWITCH_RESP			= sCharactorBase + 6,
		BOC_POWERRANK_REQ							= sCharactorBase + 7,
		BOC_POWERRANK_RESP						= sCharactorBase + 8
	};

	//-----------------CG-------------------

	struct CGCharactorListReq
	{
		CGCharactorListReq(): mPacketID(BOC_CHARACTORLIST_REQ), mPlayerUid(0) {}

		UInt16	mPacketID;
		UInt64	mPlayerUid;

		LYNX_MESSAGE_2(BOC_CHARACTORLIST_REQ, CGCharactorListReq, UInt16, mPacketID, UInt64, mPlayerUid);
	};

	struct CGCharactorBuyReq
	{
		CGCharactorBuyReq():mPacketID(BOC_CHARACTORBUY_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CHARACTORBUY_REQ, CGCharactorBuyReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGCharactorSwitchReq
	{
		CGCharactorSwitchReq():mPacketID(BOC_CHARACTORSWITCH_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CHARACTORSWITCH_REQ, CGCharactorSwitchReq, UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGPowerRankReq
	{
		CGPowerRankReq():mPacketID(BOC_POWERRANK_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_POWERRANK_REQ, CGPowerRankReq, UInt16, mPacketID,std::string, mReqJsonStr);

	};

			
	//-----------------GC-------------------
	struct GCCharactorListResp
	{
		GCCharactorListResp(): mPacketID(BOC_CHARACTORLIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CHARACTORLIST_RESP, GCCharactorListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};


	struct GCCharactorBuyResp
	{
		GCCharactorBuyResp():mPacketID(BOC_CHARACTORBUY_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CHARACTORBUY_RESP,GCCharactorBuyResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};


	struct GCCharactorSwitchResp
	{
		GCCharactorSwitchResp():mPacketID(BOC_CHARACTORSWITCH_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_CHARACTORSWITCH_RESP,GCCharactorSwitchResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCPowerRankResp
	{
		GCPowerRankResp():mPacketID(BOC_POWERRANK_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_POWERRANK_RESP,GCPowerRankResp, UInt16, mPacketID,std::string, mRespJsonStr);
				
	};




}// namespace Lynx

#endif // __LYNX_COMMON_LIB_CHARACTOR_PACKET_H__

