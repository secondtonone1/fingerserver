#ifndef __LYNX_COMMON_LIB_DETAILINFO_PACKET_H__
#define __LYNX_COMMON_LIB_DETAILINFO_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_CLIENTDETAILINFO_REQ				= sDetailInfoMsgBase + 1,
		BOC_CLIENTDETAILINFO_RESP              = sDetailInfoMsgBase + 2 ,
		BOC_CLIENTUPDATEINFO_REQ              = sDetailInfoMsgBase + 3,
		BOC_PLAYERRENAME_REQ						=  sDetailInfoMsgBase + 4,
		BOC_PLAYERRENAME_RESP					=  sDetailInfoMsgBase + 5
	};

	//-----------------CG-------------------
	struct CGClientDetailInfoReq
	{
		CGClientDetailInfoReq(): mPacketID(BOC_CLIENTDETAILINFO_REQ), mReqJsonStr("") {}
        
		UInt16 mPacketID;
		std::string 	mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CLIENTDETAILINFO_REQ, CGClientDetailInfoReq, UInt16, mPacketID, std::string ,mReqJsonStr);
	};

	struct CGClientUpdateInfoReq
	{
		CGClientUpdateInfoReq(): mPacketID(BOC_CLIENTUPDATEINFO_REQ), mReqJsonStr("") {}

		UInt16 mPacketID;
		std::string 	mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CLIENTUPDATEINFO_REQ, CGClientUpdateInfoReq, UInt16, mPacketID, std::string ,mReqJsonStr);
	};
	
	struct CGPlayerRenameReq
	{
		CGPlayerRenameReq(): mPacketID(BOC_PLAYERRENAME_REQ), mReqJsonStr("") {}

		UInt16 mPacketID;
		std::string 	mReqJsonStr;

		LYNX_MESSAGE_2(BOC_PLAYERRENAME_REQ, CGPlayerRenameReq, UInt16, mPacketID, std::string ,mReqJsonStr);
	};

	
	//-----------------GC-------------------
	struct GCClientDetailInfoResp
	{
		GCClientDetailInfoResp(): mPacketID(BOC_CLIENTDETAILINFO_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CLIENTDETAILINFO_RESP, GCClientDetailInfoResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCPlayerRenameResp
	{
		GCPlayerRenameResp(): mPacketID(BOC_PLAYERRENAME_RESP), mRespJsonStr("") {}

		UInt16 mPacketID;
		std::string 	mRespJsonStr;

		LYNX_MESSAGE_2(BOC_PLAYERRENAME_RESP, GCPlayerRenameResp, UInt16, mPacketID, std::string ,mRespJsonStr);
	};
	





}// namespace Lynx

#endif // __LYNX_COMMON_LIB_DETAILINFO_PACKET_H__

