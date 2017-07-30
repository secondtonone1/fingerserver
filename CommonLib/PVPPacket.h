#ifndef __LYNX_COMMON_LIB_PVP_PACKET_H__
#define __LYNX_COMMON_LIB_PVP_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_PVP_REQ					= sPVPBase + 1,
		BOC_PVP_RESP					= sPVPBase + 2

	};

	//-----------------CG-------------------

	struct CGPVPReq
	{
		CGPVPReq(): mPacketID(BOC_PVP_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_PVP_REQ, CGPVPReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	
			
	//-----------------GC-------------------
	struct GCPVPResp
	{
		GCPVPResp(): mPacketID(BOC_PVP_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PVP_RESP, GCPVPResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};




}// namespace Lynx

#endif // __LYNX_COMMON_LIB_PVP_PACKET_H__

