#ifndef __LYNX_COMMON_LIB_CONSORT_PACKET_H__
#define __LYNX_COMMON_LIB_CONSORT_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_CONSORTDATA_REQ					= sConsortBase + 1,
		BOC_CONSORTDATA_RESP				= sConsortBase + 2,
		BOC_CONSORTLIST_REQ                   = sConsortBase + 3,
		BOC_CONSORTLIST_RESP					= sConsortBase + 4,
		BOC_CONSORTFIND_REQ					= sConsortBase + 5,
		BOC_CONSORTFIND_RESP					= sConsortBase + 6,
		BOC_CONSORTQUICKJOIN_REQ		= sConsortBase + 7,
		BOC_CONSORTQUICKJOIN_RESP		= sConsortBase + 8,
		BOC_CONSORTJOIN_REQ					= sConsortBase + 9,
		BOC_CONSORTCREATE_REQ				= sConsortBase + 11,
		//BOC_CONSORTCREATE_RESP				= sConsortBase + 12
		BOC_CONSORTMEMBER_REQ			= sConsortBase + 13,
		BOC_CONSORTMEMBER_RESP			= sConsortBase + 14,
		BOC_APPLYCHECK_REQ						=	sConsortBase + 15,
		BOC_APPLYCHECK_RESP						= sConsortBase + 16,
		BOC_APPLYDEAL_REQ						= sConsortBase + 17,
		BOC_APPLYDEAL_RESP						= sConsortBase + 18,
		BOC_APPLYCLEAR_REQ						= sConsortBase + 19,
		BOC_APPLYCLEAR_RESP						= sConsortBase + 20,
		BOC_CONSORTSET_REQ						= sConsortBase + 21,
		BOC_CONSORTSET_RESP					= sConsortBase + 22,
		BOC_SETDESCS_REQ							= sConsortBase + 23,
		BOC_SETDESCS_RESP							= sConsortBase + 24,
		BOC_KICKMEMBER_REQ						= sConsortBase + 25,
		BOC_KICKMEMBER_RESP					= sConsortBase + 26,
		BOC_ISKICKED_NOTIFY						= sConsortBase + 27,
		BOC_DESTROYCONSORT_REQ			= sConsortBase + 29,
		BOC_DESTROYCONSORT_RESP			= sConsortBase + 30,
		BOC_LEAVECONSORT_REQ				= sConsortBase + 31, 
		BOC_LEAVECONSORT_RESP				= sConsortBase + 32,
		BOC_SETVICELEADER_REQ					= sConsortBase + 33,
		BOC_SETVICELEADER_RESP				= sConsortBase + 34,
		BOC_CONSORTMSG_NOTIFY				= sConsortBase + 35,
		BOC_CONSORTLOG_REQ					= sConsortBase + 36,
		BOC_CONSORTLOG_RESP					= sConsortBase + 37,
		BOC_CONSORTDETAIL_REQ				= sConsortBase + 39,
		BOC_CONSORTDETAIL_RESP				= sConsortBase + 40,

		BOC_CONSORTSIGN_REQ					= sConsortBase + 41,
		BOC_CONSORTSIGN_RESP					= sConsortBase + 42,
		//零点刷新同步活跃度和签到情况
		BOC_CONSORTSIGNSYNC_NOTIFY   = sConsortBase + 44,
		//同步活跃度情况
        BOC_CONSORTACTIVE_NOTIFY			= sConsortBase + 46,
 
		BOC_CONSORTSIGNAWARD_REQ    = sConsortBase + 47,
		BOC_CONSORTSIGNAWARD_RESP	= sConsortBase + 48,

		BOC_CONSORTJOBCHANGE			=sConsortBase + 50,
		BOC_CONSORTDATANOTIFY			= sConsortBase + 52,
		BOC_CATTASKGETREQ					= sConsortBase + 53,
		BOC_CATTASKGETRESP					= sConsortBase + 54,
		BOC_CATTASKGETAWARDREQ		= sConsortBase + 57,
		BOC_CATTASKGETAWARDRESP		= sConsortBase + 58,
		BOC_CATTASKREFRESHREQ		    = sConsortBase + 59,
		BOC_CATTASKREFRESHRESP		    = sConsortBase + 60,
		BOC_KITCHENBEGIN_REQ				= sConsortBase + 61,
		BOC_KITCHENBEGIN_RESP				= sConsortBase + 62,
		BOC_KITCHENEND_REQ				    = sConsortBase + 63,
		BOC_KITCHENEND_RESP				= sConsortBase + 64,
		BOC_BUSINESSCATSYNC_NOTIFY = sConsortBase + 66,
        BOC_BUSINESSCATRESET_REQ		= sConsortBase + 67,
		BOC_BUSINESSCATRESET_RESP     = sConsortBase + 68,
		BOC_BUSINESSCATBUY_REQ			= sConsortBase + 69,
		BOC_BUSINESSCATBUY_RESP		= sConsortBase + 70,
		BOC_BUSINESSCATTIME_REQ       =  sConsortBase + 71,
		BOC_BUSINESSCATTIME_RESP      = sConsortBase + 72,
		BOC_EYEQUE_REQ							= sConsortBase + 73,
		BOC_EYEQUE_RESP							= sConsortBase + 74,
		BOC_EYEAWARD_REQ						= sConsortBase + 75,
		BOC_EYEAWARD_RESP					= sConsortBase + 76,
		BOC_TICKETDATA_REQ					= sConsortBase + 77,
		BOC_TICKETDATA_RESP					= sConsortBase + 78,
		BOC_TICKETONEDATA_REQ			= sConsortBase + 79,
		BOC_TICKETONEDATA_RESP			= sConsortBase + 80,
		BOC_TICKETAWARD_REQ				= sConsortBase + 81,
		BOC_TICKETAWARD_RESP				= sConsortBase + 82,
		BOC_TICKETJOIN_REQ					= sConsortBase + 83,
		BOC_TICKETJOIN_RESP					= sConsortBase + 84,
		BOC_TICKETSUPPORT_REQ            = sConsortBase + 85,
		BOC_TICKETSUPPORT_RESP			= sConsortBase + 86,
       
		BOC_ELOQUENCEAWARD_REQ		= sConsortBase + 87,
		BOC_ELOQUENCEAWARD_RESP		= sConsortBase + 88,

		BOC_PLAYERNEWGUID_REQ			= sConsortBase + 89,//新手引导

		BOC_ELOQUENCEBEGIN_REQ		= sConsortBase + 91,
		BOC_ELOQUENCEBEGIN_RESP		= sConsortBase + 92,

		BOC_WOODCATBEGIN_REQ			= sConsortBase + 95,
		BOC_WOODCATBEGIN_RESP         = sConsortBase + 96,

		BOC_WOODDATA_REQ				    = sConsortBase + 97,
		
		BOC_WOODCATEND_REQ			   = sConsortBase + 99,
		BOC_WOODCATEND_RESP           = sConsortBase + 100,

		BOC_WOODENHANCE_REQ			= sConsortBase + 101,
		BOC_WOODTOTALAWARD_REQ	= sConsortBase + 103,
		BOC_WOODTOTALAWARD_RESP  = sConsortBase + 104

	};

	//-----------------CG-------------------
	struct CGConsortDataReq
	{
		CGConsortDataReq(): mPacketID(BOC_CONSORTDATA_REQ), mConsortID(0) {}

		UInt16	mPacketID;
		UInt64	mConsortID;

		LYNX_MESSAGE_2(BOC_CONSORTDATA_REQ, CGConsortDataReq, UInt16, mPacketID, UInt64, mConsortID);
	};

	struct CGConsortListReq
	{
		CGConsortListReq():mPacketID(BOC_CONSORTLIST_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTLIST_REQ, CGConsortListReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	
	struct CGConsortFindReq
	{
		CGConsortFindReq():mPacketID(BOC_CONSORTFIND_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTFIND_REQ, CGConsortFindReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};
	          
	struct CGConsortQuickJoinReq
	{
		CGConsortQuickJoinReq():mPacketID(BOC_CONSORTQUICKJOIN_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTQUICKJOIN_REQ, CGConsortQuickJoinReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};
		
	struct CGConsortJoinReq
	{
		CGConsortJoinReq():mPacketID(BOC_CONSORTJOIN_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTJOIN_REQ, CGConsortJoinReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGConsortCreateReq
	{
		CGConsortCreateReq():mPacketID(BOC_CONSORTCREATE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTCREATE_REQ, CGConsortCreateReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGConsortMemberReq
	{
		CGConsortMemberReq():mPacketID(BOC_CONSORTMEMBER_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTMEMBER_REQ, CGConsortMemberReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};
		
	struct CGApplyCheckReq
	{
		CGApplyCheckReq():mPacketID(BOC_APPLYCHECK_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_APPLYCHECK_REQ, CGApplyCheckReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGApplyDealReq
	{
		CGApplyDealReq():mPacketID(BOC_APPLYDEAL_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_APPLYDEAL_REQ, CGApplyDealReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGApplyClearReq
	{
		CGApplyClearReq():mPacketID(BOC_APPLYCLEAR_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_APPLYCLEAR_REQ, CGApplyClearReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGConsortSetReq
	{
		CGConsortSetReq():mPacketID(BOC_CONSORTSET_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTSET_REQ, CGConsortSetReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};
	
	struct CGSetDescsReq
	{
		CGSetDescsReq():mPacketID(BOC_SETDESCS_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SETDESCS_REQ, CGSetDescsReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};
	
	struct CGKickMemberReq
	{
		CGKickMemberReq():mPacketID(BOC_KICKMEMBER_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_KICKMEMBER_REQ, CGKickMemberReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGDestroyConsortReq
	{
		CGDestroyConsortReq():mPacketID(BOC_DESTROYCONSORT_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_DESTROYCONSORT_REQ, CGDestroyConsortReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGLeaveConsortReq
	{
		CGLeaveConsortReq():mPacketID(BOC_LEAVECONSORT_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_LEAVECONSORT_REQ, CGLeaveConsortReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGSetViceLeaderReq
	{
		CGSetViceLeaderReq():mPacketID(BOC_SETVICELEADER_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SETVICELEADER_REQ, CGSetViceLeaderReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGConsortlogReq
	{
		CGConsortlogReq():mPacketID(BOC_CONSORTLOG_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTLOG_REQ, CGConsortlogReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};
		
	struct CGConsortDetailReq
	{
		CGConsortDetailReq():mPacketID(BOC_CONSORTDETAIL_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTDETAIL_REQ, CGConsortDetailReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGConsortSignReq
	{
		CGConsortSignReq():mPacketID(BOC_CONSORTSIGN_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTSIGN_REQ, CGConsortSignReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGConsortSignAwardReq
	{
		CGConsortSignAwardReq():mPacketID(BOC_CONSORTSIGNAWARD_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CONSORTSIGNAWARD_REQ, CGConsortSignAwardReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGCatTaskGetReq
	{
		CGCatTaskGetReq():mPacketID(BOC_CATTASKGETREQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CATTASKGETREQ, CGCatTaskGetReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGCatTaskGetAwardReq
	{
		CGCatTaskGetAwardReq():mPacketID(BOC_CATTASKGETAWARDREQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CATTASKGETAWARDREQ, CGCatTaskGetAwardReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGCatTaskRefreshReq
	{
		CGCatTaskRefreshReq():mPacketID(BOC_CATTASKREFRESHREQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_CATTASKREFRESHREQ, CGCatTaskRefreshReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGKitchenBeginReq
	{
		CGKitchenBeginReq():mPacketID(BOC_KITCHENBEGIN_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_KITCHENBEGIN_REQ, CGKitchenBeginReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGKitchenEndReq
	{
		CGKitchenEndReq():mPacketID(BOC_KITCHENEND_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_KITCHENEND_REQ, CGKitchenEndReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGBusinessCatResetReq
	{
		CGBusinessCatResetReq():mPacketID(BOC_BUSINESSCATRESET_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_BUSINESSCATRESET_REQ, CGBusinessCatResetReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGBusinessCatBuyReq
	{
		CGBusinessCatBuyReq():mPacketID(BOC_BUSINESSCATBUY_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_BUSINESSCATBUY_REQ, CGBusinessCatBuyReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGBusinessCatTimeReq
	{
		CGBusinessCatTimeReq():mPacketID(BOC_BUSINESSCATTIME_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_BUSINESSCATTIME_REQ, CGBusinessCatTimeReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	
	struct CGEyeQueReq
	{
		CGEyeQueReq():mPacketID(BOC_EYEQUE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_EYEQUE_REQ, CGEyeQueReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGEyeawardReq
	{
		CGEyeawardReq():mPacketID(BOC_EYEAWARD_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_EYEAWARD_REQ, CGEyeawardReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGTicketDataReq
	{
		CGTicketDataReq():mPacketID(BOC_TICKETDATA_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_TICKETDATA_REQ, CGTicketDataReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};

	struct CGTicketOneDataReq
	{
		CGTicketOneDataReq():mPacketID(BOC_TICKETONEDATA_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_TICKETONEDATA_REQ, CGTicketOneDataReq,UInt16, mPacketID,std::string, mReqJsonStr);

	};


	


	struct CGPlayerNewGuidReq
	{
		CGPlayerNewGuidReq():mPacketID(BOC_PLAYERNEWGUID_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYERNEWGUID_REQ, CGPlayerNewGuidReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

	struct CGTicketJoinReq
	{
		CGTicketJoinReq():mPacketID(BOC_TICKETJOIN_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_TICKETJOIN_REQ, CGTicketJoinReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

	struct CGTicketAwardReq
	{
		CGTicketAwardReq():mPacketID(BOC_TICKETAWARD_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_TICKETAWARD_REQ, CGTicketAwardReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

	struct CGTicketSupportReq
	{
		CGTicketSupportReq():mPacketID(BOC_TICKETSUPPORT_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_TICKETSUPPORT_REQ, CGTicketSupportReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

	struct CGEloquenceAwardReq
	{
		CGEloquenceAwardReq():mPacketID(BOC_ELOQUENCEAWARD_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_ELOQUENCEAWARD_REQ, CGEloquenceAwardReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

	struct CGEloquenceBeginReq
	{
		CGEloquenceBeginReq():mPacketID(BOC_ELOQUENCEBEGIN_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_ELOQUENCEBEGIN_REQ, CGEloquenceBeginReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

	struct CGWoodDataReq
	{
		CGWoodDataReq():mPacketID(BOC_WOODDATA_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_WOODDATA_REQ, CGWoodDataReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

	struct CGWoodCatBeginReq
	{
		CGWoodCatBeginReq():mPacketID(BOC_WOODCATBEGIN_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_WOODCATBEGIN_REQ, CGWoodCatBeginReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

	struct CGWoodCatEndReq
	{
		CGWoodCatEndReq():mPacketID(BOC_WOODCATEND_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_WOODCATEND_REQ, CGWoodCatEndReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

	struct CGWoodEnhanceReq
	{
		CGWoodEnhanceReq():mPacketID(BOC_WOODENHANCE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_WOODENHANCE_REQ, CGWoodEnhanceReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

	struct CGWoodTotalAwardReq
	{
		CGWoodTotalAwardReq():mPacketID(BOC_WOODTOTALAWARD_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_WOODTOTALAWARD_REQ, CGWoodTotalAwardReq,UInt16, mPacketID,std::string, mReqJsonStr);
	
	};

		
	//-----------------GC-------------------
	struct GCConsortDataResp
	{
		GCConsortDataResp(): mPacketID(BOC_CONSORTDATA_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTDATA_RESP, GCConsortDataResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	
	struct GCConsortListResp
	{
		GCConsortListResp(): mPacketID(BOC_CONSORTLIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTLIST_RESP, GCConsortListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};


	 struct GCConsortFindResp
	{
		GCConsortFindResp(): mPacketID(BOC_CONSORTFIND_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTFIND_RESP, GCConsortFindResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	
	 struct GCConsortQuickJoinResp
	{
		GCConsortQuickJoinResp(): mPacketID(BOC_CONSORTQUICKJOIN_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTQUICKJOIN_RESP, GCConsortQuickJoinResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	/* struct GCConsortCreateResp
	{
		GCConsortCreateResp(): mPacketID(BOC_CONSORTCREATE_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTCREATE_RESP, GCConsortCreateResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};*/

	 struct GCConsortMemberResp
	{
		GCConsortMemberResp(): mPacketID(BOC_CONSORTMEMBER_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTMEMBER_RESP, GCConsortMemberResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	 struct GCApplyCheckResp
	{
		GCApplyCheckResp(): mPacketID(BOC_APPLYCHECK_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_APPLYCHECK_RESP, GCApplyCheckResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCApplyDealResp
	{
		GCApplyDealResp(): mPacketID(BOC_APPLYDEAL_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_APPLYDEAL_RESP, GCApplyDealResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};


	struct GCApplyClearResp
	{
		GCApplyClearResp(): mPacketID(BOC_APPLYCLEAR_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_APPLYCLEAR_RESP, GCApplyClearResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCConsortSetResp
	{
		GCConsortSetResp(): mPacketID(BOC_CONSORTSET_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTSET_RESP, GCConsortSetResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	
	struct GCSetDescsResp
	{
		GCSetDescsResp(): mPacketID(BOC_SETDESCS_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SETDESCS_RESP, GCSetDescsResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCKickMemberResp
	{
		GCKickMemberResp(): mPacketID(BOC_KICKMEMBER_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_KICKMEMBER_RESP, GCKickMemberResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCIsKickedNotify
	{
		GCIsKickedNotify(): mPacketID(BOC_ISKICKED_NOTIFY), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_ISKICKED_NOTIFY, GCIsKickedNotify, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCDestroyConsortResp
	{
		GCDestroyConsortResp(): mPacketID(BOC_DESTROYCONSORT_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_DESTROYCONSORT_RESP, GCDestroyConsortResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCLeaveConsortResp
	{
		GCLeaveConsortResp(): mPacketID(BOC_LEAVECONSORT_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_LEAVECONSORT_RESP, GCLeaveConsortResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCSetViceLeaderResp
	{
		GCSetViceLeaderResp(): mPacketID(BOC_SETVICELEADER_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SETVICELEADER_RESP, GCSetViceLeaderResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	
	struct GCConsortMsgNotify
	{
		GCConsortMsgNotify(): mPacketID(BOC_CONSORTMSG_NOTIFY), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTMSG_NOTIFY, GCConsortMsgNotify, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCConsortlogResp
	{
		GCConsortlogResp(): mPacketID(BOC_CONSORTLOG_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTLOG_RESP, GCConsortlogResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	
	struct GCConsortDetailResp
	{
		GCConsortDetailResp(): mPacketID(BOC_CONSORTDETAIL_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTDETAIL_RESP, GCConsortDetailResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	

	struct GCConsortSignSyncNotify
	{
		GCConsortSignSyncNotify(): mPacketID(BOC_CONSORTSIGNSYNC_NOTIFY), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTSIGNSYNC_NOTIFY, GCConsortSignSyncNotify, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCConsortActiveSyncNotify
	{
		GCConsortActiveSyncNotify(): mPacketID(BOC_CONSORTACTIVE_NOTIFY), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTACTIVE_NOTIFY, GCConsortActiveSyncNotify, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCConsortJobNotify
	{
		GCConsortJobNotify(): mPacketID(BOC_CONSORTJOBCHANGE), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTJOBCHANGE, GCConsortJobNotify, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCConsortDataNotify
	{
		GCConsortDataNotify(): mPacketID(BOC_CONSORTDATANOTIFY), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTDATANOTIFY, GCConsortDataNotify, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCConsortSignResp
	{
		GCConsortSignResp(): mPacketID(BOC_CONSORTSIGN_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTSIGN_RESP, GCConsortSignResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};


	struct GCConsortSignAwardResp
	{
		GCConsortSignAwardResp(): mPacketID(BOC_CONSORTSIGNAWARD_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CONSORTSIGNAWARD_RESP, GCConsortSignAwardResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCCatTaskGetResp
	{
		GCCatTaskGetResp(): mPacketID(BOC_CATTASKGETRESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CATTASKGETRESP, GCCatTaskGetResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCCatTaskGetAwardResp
	{
		GCCatTaskGetAwardResp(): mPacketID(BOC_CATTASKGETAWARDRESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CATTASKGETAWARDRESP, GCCatTaskGetAwardResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCCatTaskRefreshResp
	{
		GCCatTaskRefreshResp(): mPacketID(BOC_CATTASKREFRESHRESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CATTASKREFRESHRESP, GCCatTaskRefreshResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCKitchenBeginResp
	{
		GCKitchenBeginResp(): mPacketID(BOC_KITCHENBEGIN_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	 mRespJsonStr;
		LYNX_MESSAGE_2(BOC_KITCHENBEGIN_RESP, GCKitchenBeginResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct CGKitchenEndResp
	{
		CGKitchenEndResp():mPacketID(BOC_KITCHENEND_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_KITCHENEND_RESP, CGKitchenEndResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct BusinessCatNotify
	{
		BusinessCatNotify():mPacketID(BOC_BUSINESSCATSYNC_NOTIFY),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_BUSINESSCATSYNC_NOTIFY, BusinessCatNotify,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCBusinessCatResetResp
	{
		GCBusinessCatResetResp():mPacketID(BOC_BUSINESSCATRESET_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_BUSINESSCATRESET_RESP, GCBusinessCatResetResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCBusinessCatBuyResp
	{
		GCBusinessCatBuyResp():mPacketID(BOC_BUSINESSCATBUY_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_BUSINESSCATBUY_RESP, GCBusinessCatBuyResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCBusinessCatTimeResp
	{
		GCBusinessCatTimeResp():mPacketID(BOC_BUSINESSCATTIME_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_BUSINESSCATTIME_RESP, GCBusinessCatTimeResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCEyequeResp
	{
		GCEyequeResp():mPacketID(BOC_EYEQUE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_EYEQUE_RESP, GCEyequeResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCEyeawardResp
	{
		GCEyeawardResp():mPacketID(BOC_EYEAWARD_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_EYEAWARD_RESP, GCEyeawardResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCTicketDataResp
	{
		GCTicketDataResp():mPacketID(BOC_TICKETDATA_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_TICKETDATA_RESP, GCTicketDataResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCTicketOneDataResp
	{
		GCTicketOneDataResp():mPacketID(BOC_TICKETONEDATA_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_TICKETONEDATA_RESP, GCTicketOneDataResp,UInt16, mPacketID,std::string, mRespJsonStr);

	};

	

	struct GCTicketJoinResp
	{
		GCTicketJoinResp():mPacketID(BOC_TICKETJOIN_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_TICKETJOIN_RESP, GCTicketJoinResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCTicketAwardResp
	{
		GCTicketAwardResp():mPacketID(BOC_TICKETAWARD_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_TICKETAWARD_RESP, GCTicketAwardResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCTicketSupportResp
	{
		GCTicketSupportResp():mPacketID(BOC_TICKETSUPPORT_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_TICKETSUPPORT_RESP, GCTicketSupportResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCEloquenceAwardResp
	{
		GCEloquenceAwardResp():mPacketID(BOC_ELOQUENCEAWARD_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_ELOQUENCEAWARD_RESP, GCEloquenceAwardResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};
	
	struct GCEloquenceBeginResp
	{
		GCEloquenceBeginResp():mPacketID(BOC_ELOQUENCEBEGIN_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_ELOQUENCEBEGIN_RESP, GCEloquenceBeginResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCWoodCatbeginResp
	{
		GCWoodCatbeginResp():mPacketID(BOC_WOODCATBEGIN_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_WOODCATBEGIN_RESP, GCWoodCatbeginResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCWoodCatendResp
	{
		GCWoodCatendResp():mPacketID(BOC_WOODCATEND_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_WOODCATEND_RESP, GCWoodCatendResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};

	struct GCWoodCatAwardResp
	{
		GCWoodCatAwardResp():mPacketID(BOC_WOODTOTALAWARD_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;

		LYNX_MESSAGE_2(BOC_WOODTOTALAWARD_RESP, GCWoodCatAwardResp, UInt16, mPacketID,std::string, mRespJsonStr);

	};
	
	
	   


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_CONSORT_PACKET_H__

