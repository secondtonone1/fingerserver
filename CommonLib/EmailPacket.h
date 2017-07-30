#ifndef  __LYNX_COMMON_LIB_EMAIL_PACKET_H__
#define __LYNX_COMMON_LIB_EMAIL_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		EMAIL_LIST_REQ					    = sEmailMsgBase + 1,
		EMAIL_LIST_RESP					= sEmailMsgBase + 2,
		EMAIL_GM_SENDREQ            = sEmailMsgBase + 3,
		EMAIL_DEL_REQ						= sEmailMsgBase + 4,
		EMAIL_DEL_RESP						= sEmailMsgBase + 5,
		EMAIL_OPEN_REQ					= sEmailMsgBase + 6,
		EMAIL_OPEN_RESP					= sEmailMsgBase + 7,
		EMAIL_GETCONTANT_REQ		= sEmailMsgBase + 8,
		EMAIL_GETCONTANT_RESP	= sEmailMsgBase + 9,
		EMAIL_DELONCE_REQ			= sEmailMsgBase + 10,
		EMAIL_DELONCE_RESP			= sEmailMsgBase + 11,
		EMAIL_GETONCE_REQ			= sEmailMsgBase + 12,
		EMAIL_GETONCE_RESP			= sEmailMsgBase + 13,
		EMAIL_NOTIFY                        = sEmailMsgBase + 14   //邮件通知，指定发送给某一个玩家
	};

	//-----------------CG-------------------
	struct GMSendReq
	{
		
		GMSendReq(): mPacketID(EMAIL_GM_SENDREQ), mReqJsonStr("") {}

		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(EMAIL_GM_SENDREQ, GMSendReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};

	struct DelEmailReq
	{

		DelEmailReq(): mPacketID(EMAIL_DEL_REQ), mReqJsonStr("") {}

		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(EMAIL_DEL_REQ, DelEmailReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};

	struct EmailOpenReq
	{
		EmailOpenReq(): mPacketID(EMAIL_OPEN_REQ), mReqJsonStr("") {}

		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(EMAIL_OPEN_REQ, EmailOpenReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};

	struct EmailGetContantReq
	{
		EmailGetContantReq(): mPacketID(EMAIL_GETCONTANT_REQ), mReqJsonStr("") {}

		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(EMAIL_GETCONTANT_REQ, EmailGetContantReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};

	struct EmailDelOnceReq
	{
		EmailDelOnceReq(): mPacketID(EMAIL_DELONCE_REQ), mReqJsonStr("") {}

		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(EMAIL_DELONCE_REQ, EmailDelOnceReq, UInt16, mPacketID,std::string, mReqJsonStr);
		
	};

	struct EmailGetOnceReq
	{
		EmailGetOnceReq(): mPacketID(EMAIL_GETONCE_REQ), mReqJsonStr("") {}

		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(EMAIL_GETONCE_REQ, EmailGetOnceReq, UInt16, mPacketID,std::string, mReqJsonStr);
		
	};
	
	//-----------------GC-------------------

	struct NewEmailNotify
	{
		NewEmailNotify(): mPacketID(EMAIL_NOTIFY), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(EMAIL_NOTIFY, NewEmailNotify, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct EmailListResp
	{
		EmailListResp(): mPacketID(EMAIL_LIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(EMAIL_LIST_RESP, EmailListResp, UInt16, mPacketID, std::string, mRespJsonStr);
		
	};
	
	struct EmailGetContantResp
	{
		EmailGetContantResp(): mPacketID(EMAIL_GETCONTANT_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(EMAIL_GETCONTANT_RESP, EmailGetContantResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	
	struct EmailGetOnceResp
	{
		EmailGetOnceResp(): mPacketID(EMAIL_GETONCE_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(EMAIL_GETONCE_RESP, EmailGetOnceResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	
	struct EmailDelResp
	{
		EmailDelResp(): mPacketID(EMAIL_DEL_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string 	mRespJsonStr;
		LYNX_MESSAGE_2(EMAIL_DEL_RESP, EmailDelResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct EmailDelOnceResp
	{
		EmailDelOnceResp(): mPacketID(EMAIL_DELONCE_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string 	mRespJsonStr;
		LYNX_MESSAGE_2(EMAIL_DELONCE_RESP, EmailDelOnceResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	
	
	

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_EMAIL_PACKET_H__