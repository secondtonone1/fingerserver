#ifndef __LYNX_GAME_SERVER_BUDDY_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_BUDDY_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct BuddyMsgHandler
    {
        static void onInviteBuddyReq(const ConnId& connId, InviteBuddyReq& msg);
        static void onAddBuddyReq(const ConnId& connId, AddBuddyReq& msg);
        static void onDelBuddyReq(const ConnId& connId, DelBuddyReq& msg);
        static void onSyncBuddyListReq(const ConnId& connId, SyncBuddyListReq& msg);
        static void onSyncInviteBuddyListReq(const ConnId& connId, SyncInviteBuddyListReq& msg);
        static void onGetRecommendBuddyListReq(const ConnId& connId, GetRecommendBuddyListReq& msg);
        static void onRejectBuddyNotify(const ConnId& connid, RejectBuddyNotify& msg);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_BUDDY_MSG_HANDLER_H__