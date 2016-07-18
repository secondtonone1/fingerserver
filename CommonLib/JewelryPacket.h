#ifndef __LYNX_COMMON_LIB_JEWELRY_PACKET_H__
#define __LYNX_COMMON_LIB_JEWELRY_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_JEWELRY_LIST_REQ					= sJewelryBase + 1,
		BOC_JEWELRY_LIST_RESP					= sJewelryBase + 2,
		BOC_JEWELRY_POLISH_REQ				    = sJewelryBase + 3,
		BOC_JEWELRY_POLISH_RESP                 = sJewelryBase + 4,
		BOC_JEWELRY_RESET_REQ					= sJewelryBase + 5,
		BOC_JEWELRY_RESET_RESP					= sJewelryBase + 6,
		BOC_JEWELRY_SET_REQ                     = sJewelryBase + 7,//ÊÎÆ·´©´÷
	    BOC_JEWELRY_SET_RESP                    = sJewelryBase + 8,
		BOC_JEWELRY_UNLOAD_REQ                  = sJewelryBase + 9,
		BOC_JEWELRY_UNLOAD_RESP                 = sJewelryBase + 10,
		BOC_JEWELRY_SELL_REQ					= sJewelryBase + 11,
		BOC_JEWELRY_SELL_RESP					= sJewelryBase + 12,
		BOC_JEWELRYUPDATE_REQ                   = sJewelryBase + 13,
		BOC_JEWELRYUPDATE_RESP                  = sJewelryBase + 14,
		BOC_JEWELRY_SETONCE_REQ                 = sJewelryBase + 15,
		BOC_JEWELRY_SETONCE_RESP                = sJewelryBase + 16
	};

	//-----------------CG-------------------
	struct CGJewelryListReq
	{
		CGJewelryListReq(): mPacketID(BOC_JEWELRY_LIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_JEWELRY_LIST_REQ, CGJewelryListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	struct CGJewelryPolishReq
	{
		CGJewelryPolishReq(): mPacketID(BOC_JEWELRY_POLISH_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string	mReqJsonStr;

		LYNX_MESSAGE_2(BOC_JEWELRY_POLISH_REQ, CGJewelryPolishReq, UInt16, mPacketID, std::string,mReqJsonStr);
	};

	struct CGJewelryResetReq
	{
		CGJewelryResetReq(): mPacketID(BOC_JEWELRY_RESET_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string	mReqJsonStr;

		LYNX_MESSAGE_2(BOC_JEWELRY_RESET_REQ, CGJewelryResetReq, UInt16, mPacketID, std::string,mReqJsonStr);
	};

	struct CGJewelrySetReq
	{
		CGJewelrySetReq(): mPacketID(BOC_JEWELRY_SET_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string	mReqJsonStr;

		LYNX_MESSAGE_2(BOC_JEWELRY_SET_REQ, CGJewelrySetReq, UInt16, mPacketID, std::string,mReqJsonStr);
	};

	struct CGJewelryUnloadReq
	{
		CGJewelryUnloadReq(): mPacketID(BOC_JEWELRY_UNLOAD_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string	mReqJsonStr;

		LYNX_MESSAGE_2(BOC_JEWELRY_UNLOAD_REQ, CGJewelryUnloadReq, UInt16, mPacketID, std::string,mReqJsonStr);
	};

	struct CGJewelrySellReq
	{
		CGJewelrySellReq(): mPacketID(BOC_JEWELRY_SELL_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string	mReqJsonStr;

		LYNX_MESSAGE_2(BOC_JEWELRY_SELL_REQ, CGJewelrySellReq, UInt16, mPacketID, std::string,mReqJsonStr);
	};

	struct CGJewelryUpdateReq
	{
		CGJewelryUpdateReq(): mPacketID(BOC_JEWELRYUPDATE_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string	mReqJsonStr;

		LYNX_MESSAGE_2(BOC_JEWELRYUPDATE_REQ, CGJewelryUpdateReq, UInt16, mPacketID, std::string,mReqJsonStr);
	};

	struct CGJewelrySetOnceReq
	{
		CGJewelrySetOnceReq(): mPacketID(BOC_JEWELRY_SETONCE_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string	mReqJsonStr;

		LYNX_MESSAGE_2(BOC_JEWELRY_SETONCE_REQ, CGJewelrySetOnceReq, UInt16, mPacketID, std::string,mReqJsonStr);
		
		
	};
	

	//-----------------GC-------------------
	struct GCJewelryListResp
	{
		GCJewelryListResp(): mPacketID(BOC_JEWELRY_LIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_JEWELRY_LIST_RESP, GCJewelryListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCJewelryPolishResp
	{
		GCJewelryPolishResp(): mPacketID(BOC_JEWELRY_POLISH_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_JEWELRY_POLISH_RESP, GCJewelryPolishResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCJewelryResetResp
	{
		GCJewelryResetResp(): mPacketID(BOC_JEWELRY_RESET_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_JEWELRY_RESET_RESP, GCJewelryResetResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	
	struct GCJewelrySetResp
	{
		GCJewelrySetResp(): mPacketID(BOC_JEWELRY_SET_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_JEWELRY_SET_RESP, GCJewelrySetResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCJewelryUnloadResp
	{
		GCJewelryUnloadResp(): mPacketID(BOC_JEWELRY_UNLOAD_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_JEWELRY_UNLOAD_RESP, GCJewelryUnloadResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCJewelrySellResp
	{
		GCJewelrySellResp(): mPacketID(BOC_JEWELRY_SELL_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_JEWELRY_SELL_RESP, GCJewelrySellResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	
	struct GCJewelryUpdateResp
	{
		GCJewelryUpdateResp(): mPacketID(BOC_JEWELRYUPDATE_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_JEWELRYUPDATE_RESP, GCJewelryUpdateResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCJewelrySetOnceResp
	{
		GCJewelrySetOnceResp(): mPacketID(BOC_JEWELRY_SETONCE_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_JEWELRY_SETONCE_RESP, GCJewelrySetOnceResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_JEWELRY_PACKET_H__

