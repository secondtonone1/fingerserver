#ifndef __LYNX_COMMON_LIB_GM_PACKET_H__
#define __LYNX_COMMON_LIB_GM_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_GM_REQ					    = sGMMsgBase + 1,
		BOC_GM_RESP					= sGMMsgBase + 2,
	
	};

	//-----------------CG-------------------
	struct CGGMReq
	{
		CGGMReq(): mPacketID(BOC_GM_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_GM_REQ, CGGMReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	//--------------------GC---------------------------
	struct GCGMResp
	{
		GCGMResp(): mPacketID(BOC_GM_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_GM_RESP, GCGMResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
};

#endif //__LYNX_COMMON_LIB_GM_PACKET_H__
