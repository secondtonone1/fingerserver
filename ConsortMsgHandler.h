#ifndef __LYNX_GAME_SERVER_CONSORT_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_CONSORT_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct ConsortMsgHandler
	{
		//玩家查询公会列表
		static void onClientConsortListReq(const ConnId& connId, CGConsortListReq& msg);
		static void onClientConsortFindReq(const ConnId & connId, CGConsortFindReq& msg);
		static void onQuickJoinConsortReq(const ConnId & connId, CGConsortQuickJoinReq& msg);
		static void onJoinConsortReq(const ConnId & connId, CGConsortJoinReq& msg);
		static void onCreateConsortReq(const ConnId & connId, CGConsortCreateReq& msg);
		static void onSyncConsortMemberReq(const ConnId & connId, CGConsortMemberReq & msg);
		static void onCheckApplyReq(const ConnId & connId, CGApplyCheckReq& msg);
		static void onDealApplyReq(const ConnId & connId, CGApplyDealReq&msg);
		static void onClearApplyReq(const ConnId & connId, CGApplyClearReq&msg);
		static void onConsortSetReq(const ConnId & connId, CGConsortSetReq & msg);
		static void onSetDescsReq(const ConnId & connId, CGSetDescsReq & msg);
		static void onKickMemberReq(const ConnId & connId, CGKickMemberReq& msg);
		static void onDestroyConsortReq(const ConnId & connId, CGDestroyConsortReq& msg);
		static void onLeaveConsortReq(const ConnId & connId, CGLeaveConsortReq & msg);
		static void onSetViceLeaderReq(const ConnId & connId, CGSetViceLeaderReq & msg);
		static void onConsortLogReq(const ConnId & connId, CGConsortlogReq & msg);
		static void onConsortDetailReq(const ConnId & connId, CGConsortDetailReq & msg);
		static void onConsortSignReq(const ConnId & connId, CGConsortSignReq & msg);
		static void onConsortSignAwardReq(const ConnId & connId, CGConsortSignAwardReq & msg);
		static void onCatTaskGetReq(const ConnId & connId, CGCatTaskGetReq & msg);
		static void onCatTaskAwardGetReq(const ConnId & connId, CGCatTaskGetAwardReq & msg);
		static void onCatTaskRefreshReq(const ConnId & connId, CGCatTaskRefreshReq & msg);
		static void onKitchenBeginReq(const ConnId & connId, CGKitchenBeginReq & msg);
		static void onKitchenEndReq(const ConnId & connId, CGKitchenEndReq & msg);
		
		static void onBusinessCatResetReq(const ConnId & connId, CGBusinessCatResetReq & msg);
		static void onBusinessCatBuyReq(const ConnId & connId, CGBusinessCatBuyReq & msg);
        static void onBusinessCatTimeReq(const ConnId & connId, CGBusinessCatTimeReq & msg);
		static void onEyeQueReq(const ConnId & connId, CGEyeQueReq & msg);
		static void onEyeAwardReq(const ConnId & connId, CGEyeawardReq & msg);
		static void onUpdateGuidReq(const ConnId & connId, CGPlayerNewGuidReq & msg);
		static void onTicketJoinReq(const ConnId & connId, CGTicketJoinReq& msg);
		static void onTicketGetAwardReq(const ConnId & connId, CGTicketAwardReq& msg);
		static void onSupportReq(const ConnId & connId, CGTicketSupportReq& msg);
		static void onTicketDataReq(const ConnId & connId, CGTicketDataReq& msg);
		static void onTicketOneDataReq(const ConnId & connId, CGTicketOneDataReq& msg);

		static void onEloquenceAward(const ConnId & connId, CGEloquenceAwardReq& msg);
		static void onEloquenceBegin(const ConnId & connId, CGEloquenceBeginReq& msg);

		static void onWoodCatDataReq(const ConnId & connId, CGWoodDataReq& msg);

		static void onWoodCatBeginReq(const ConnId & connId, CGWoodCatBeginReq& msg);

		static void onWoodCatEndReq(const ConnId & connId, CGWoodCatEndReq& msg);
		
		static void onWoodCatEnhanceReq(const ConnId & connId, CGWoodEnhanceReq& msg);

		static void onWoodCatTotalAwardReq(const ConnId & connId, CGWoodTotalAwardReq& msg);

		
		//static void on

		//
		
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_CONSORT_MSG_HANDLER_H__
