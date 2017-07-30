#ifndef __LYNX_COMMON_LIB_GM_PACKET_H__
#define __LYNX_COMMON_LIB_GM_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_GM_REQ					    = sGMMsgBase + 1,
		BOC_GM_RESP					= sGMMsgBase + 2,

		//新增的GM需求，通过网站修改玩家属性和数据，发邮件等等
		BOC_GM_FORBID_REQ		=  sGMMsgBase + 3 ,//禁止登陆
		BOC_GM_KICKPLAYER_REQ	= sGMMsgBase+5,
		BOC_GM_FORBID_CHAT_REQ = sGMMsgBase + 7,
		BOC_GM_SEND_EMAIL  =   sGMMsgBase + 9,
		BOC_GM_KICKALL_REQ	= sGMMsgBase + 11,
		BOC_GM_NOTICE_REQ  = sGMMsgBase + 13,
		BOC_GM_RESET_REQ =   sGMMsgBase + 15,
		BOC_GM_SEND_AWARDBAG = sGMMsgBase + 17

	
	};

	//-----------------CG-------------------
	struct CGGMReq
	{
		CGGMReq(): mPacketID(BOC_GM_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_GM_REQ, CGGMReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGGMForbidLoginReq
	{
		CGGMForbidLoginReq(): mPacketID(BOC_GM_FORBID_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_GM_FORBID_REQ, CGGMForbidLoginReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGGMKickPlayerReq
	{
		CGGMKickPlayerReq(): mPacketID(BOC_GM_KICKPLAYER_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_GM_KICKPLAYER_REQ, CGGMKickPlayerReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGGMForbidChatReq
	{
		CGGMForbidChatReq(): mPacketID(BOC_GM_FORBID_CHAT_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_GM_FORBID_CHAT_REQ, CGGMForbidChatReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGGMSendEmailReq
	{
		CGGMSendEmailReq(): mPacketID(BOC_GM_SEND_EMAIL), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_GM_SEND_EMAIL, CGGMSendEmailReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGGMKickAllReq
	{
		CGGMKickAllReq(): mPacketID(BOC_GM_KICKALL_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_GM_KICKALL_REQ, CGGMKickAllReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGGMNoticeReq
	{
		CGGMNoticeReq(): mPacketID(BOC_GM_NOTICE_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_GM_NOTICE_REQ, CGGMNoticeReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGGMResetReq
	{
		CGGMResetReq(): mPacketID(BOC_GM_RESET_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_GM_RESET_REQ, CGGMResetReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGGAwardBagReq
	{
		CGGAwardBagReq(): mPacketID(BOC_GM_SEND_AWARDBAG), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_GM_SEND_AWARDBAG, CGGAwardBagReq, UInt16, mPacketID, std::string, mReqJsonStr);
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
