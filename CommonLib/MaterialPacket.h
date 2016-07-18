#ifndef __LYNX_COMMON_LIB_MATERIAL_PACKET_H__
#define __LYNX_COMMON_LIB_MATERIAL_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum  
	{
		BOC_MATERIAL_LIST_REQ					= sMaterialMsgBase + 1,
		BOC_MATERIAL_LIST_RESP					= sMaterialMsgBase + 2
	};

	//-----------------CG-------------------
	struct CGMaterialListReq
	{
		CGMaterialListReq(): mPacketID(BOC_MATERIAL_LIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_MATERIAL_LIST_REQ, CGMaterialListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	//-----------------GC-------------------
	struct GCMaterialListResp
	{
		GCMaterialListResp(): mPacketID(BOC_MATERIAL_LIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_MATERIAL_LIST_RESP, GCMaterialListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_MATERIAL_PACKET_H__