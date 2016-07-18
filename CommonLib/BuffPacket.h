#ifndef __LYNX_COMMON_LIB_BUFF_PACKET_H__
#define __LYNX_COMMON_LIB_BUFF_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_BUFF_LIST_REQ					= sBuffMsgBase + 1,
		BOC_BUFF_LIST_RESP					= sBuffMsgBase + 2
	};

	//-----------------CG-------------------
	struct CGBuffListReq
	{
		CGBuffListReq(): mPacketID(BOC_BUFF_LIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_BUFF_LIST_REQ, CGBuffListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	//-----------------GC-------------------
	struct GCBuffListResp
	{
		GCBuffListResp(): mPacketID(BOC_BUFF_LIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_BUFF_LIST_RESP, GCBuffListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_BUFF_PACKET_H__