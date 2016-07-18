#ifndef __LYNX_COMMON_LIB_GEM_PACKET_H__
#define __LYNX_COMMON_LIB_GEM_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_GEM_LIST_REQ					= sGemMsgBase + 1,
		BOC_GEM_LIST_RESP					= sGemMsgBase + 2,
		BOC_GEM_COMBINE_REQ				    = sGemMsgBase + 3,
		BOC_GEM_COMBINE_RESP				= sGemMsgBase + 4,
		
		BOC_GEM_COMBINEONCE_REQ				= sGemMsgBase + 7,
		BOC_GEM_COMBINEONCE_RESP			= sGemMsgBase + 8
	};

	//-----------------CG-------------------
	struct CGGemListReq
	{
		CGGemListReq(): mPacketID(BOC_GEM_LIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_GEM_LIST_REQ, CGGemListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	struct CGGemCombineReq
	{
		CGGemCombineReq():mPacketID(BOC_GEM_COMBINE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_GEM_COMBINE_REQ, CGGemCombineReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};
	
	struct CGGemCombineOnceReq
	{
		CGGemCombineOnceReq():mPacketID(BOC_GEM_COMBINEONCE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_GEM_COMBINEONCE_REQ, CGGemCombineOnceReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};

	

	//-----------------GC-------------------
	struct GCGemListResp
	{
		GCGemListResp(): mPacketID(BOC_GEM_LIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_GEM_LIST_RESP, GCGemListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCGemCombineResp
	{
		GCGemCombineResp():mPacketID(BOC_GEM_COMBINE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_GEM_COMBINE_RESP, GCGemCombineResp, UInt16, mPacketID,std::string, mRespJsonStr);
	};
	
	struct GCGemCombineOnceResp
	{
		GCGemCombineOnceResp(): mPacketID(BOC_GEM_COMBINEONCE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_GEM_COMBINEONCE_RESP,GCGemCombineOnceResp, UInt16, mPacketID, std::string, mRespJsonStr);
		
	};


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_GEM_PACKET_H__

