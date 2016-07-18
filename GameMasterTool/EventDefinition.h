#ifndef __LYNX_GAME_MASTER_TOOL_EVENT_DEFINITION_H__
#define __LYNX_GAME_MASTER_TOOL_EVENT_DEFINITION_H__

#include "CommonLib.h"

namespace Lynx
{
    enum
    {
        UPDATE_SERVER_PANEL_EVENT = 0,
        WINDOW_LOG_PRINT_EVENT,
        WINDOW_LOG_CLEAR_EVENT,
        SERVER_SELECT_EVENT,
        LOGIN_SUCCESS_EVENT,
        LOGIN_BROKEN_EVENT,
        GET_PLAYER_BY_ONLINE_EVENT,
        GET_PLAYER_BY_PLAYER_GUID_EVENT,
        GET_PLAYER_BY_PLAYER_NAME_EVENT,
        GET_PLAYER_BY_ACCOUNT_GUID_EVENT,
        GET_PLAYER_BY_ACCOUNT_NAME_EVENT,
        GET_PLAYER_DATA_EVENT,
        HORSE_LAMP_NOTIFY_EVENT,
    };

    struct UpdateServerPanelEvent
    {
        UpdateServerPanelEvent() {}

        LYNX_MESSAGE_0(UPDATE_SERVER_PANEL_EVENT, UpdateServerPanelEvent);
    };

    struct WindowLogPrintEvent
    {
        WindowLogPrintEvent() : mLogContents("") {}

        String mLogContents;

        LYNX_MESSAGE_1(WINDOW_LOG_PRINT_EVENT, WindowLogPrintEvent,
            String, mLogContents);
    };

    struct WindowLogClearEvent
    {
        WindowLogClearEvent() {}

        LYNX_MESSAGE_0(WINDOW_LOG_CLEAR_EVENT, WindowLogClearEvent);
    };

    struct ServerSelectEvent
    {
        ServerSelectEvent() : mSelected(true), mServerName("") {}

        bool mSelected;
        String mServerName;

        LYNX_MESSAGE_2(SERVER_SELECT_EVENT, ServerSelectEvent, 
            bool, mSelected, String, mServerName);
    };

    struct LoginSuccessEvent
    {
        LoginSuccessEvent() : mServerName("") {}

        String mServerName;

        LYNX_MESSAGE_1(LOGIN_SUCCESS_EVENT, LoginSuccessEvent,
            String, mServerName);
    };

    struct LoginBrokenEvent
    {
        LoginBrokenEvent() : mServerName("") {}

        String mServerName;

        LYNX_MESSAGE_1(LOGIN_BROKEN_EVENT, LoginBrokenEvent,
            String, mServerName);
    };

    struct GetPlayerByOnlineEvent
    {
        GetPlayerByOnlineEvent() : mErrno(LynxErrno::None) {}

        ErrorId mErrno;
        List<PlayerBaseData> mPlayerList;

        LYNX_MESSAGE_1(GET_PLAYER_BY_ONLINE_EVENT, GetPlayerByOnlineEvent,
            List<PlayerBaseData>, mPlayerList);
    };

    struct GetPlayerByPlayerGuidEvent
    {
        GetPlayerByPlayerGuidEvent() : mErrno(LynxErrno::None), mOffline(false) {}

        ErrorId mErrno;
        bool mOffline;

        LYNX_MESSAGE_2(GET_PLAYER_BY_PLAYER_GUID_EVENT, GetPlayerByPlayerGuidEvent,
            ErrorId, mErrno, bool, mOffline);
    };

    struct GetPlayerByPlayerNameEvent
    {
        GetPlayerByPlayerNameEvent() : mErrno(LynxErrno::None), mOffline(false) {}

        ErrorId mErrno;
        bool mOffline;

        LYNX_MESSAGE_2(GET_PLAYER_BY_PLAYER_NAME_EVENT, GetPlayerByPlayerNameEvent,
            ErrorId, mErrno, bool, mOffline);
    };

    struct GetPlayerByAccountGuidEvent
    {
        GetPlayerByAccountGuidEvent() : mErrno(LynxErrno::None), mOffline(false) {}

        ErrorId mErrno;
        bool mOffline;

        LYNX_MESSAGE_2(GET_PLAYER_BY_ACCOUNT_GUID_EVENT, GetPlayerByAccountGuidEvent,
            ErrorId, mErrno, bool, mOffline);
    };

    struct GetPlayerByAccountNameEvent
    {
        GetPlayerByAccountNameEvent() : mErrno(LynxErrno::None), mOffline(false) {}

        ErrorId mErrno;
        bool mOffline;

        LYNX_MESSAGE_2(GET_PLAYER_BY_ACCOUNT_NAME_EVENT, GetPlayerByAccountNameEvent,
            ErrorId, mErrno, bool, mOffline);
    };

    struct GetPlayerDataEvent
    {
        GetPlayerDataEvent() : mErrno(LynxErrno::None) {}

        ErrorId mErrno;

        LYNX_MESSAGE_1(GET_PLAYER_DATA_EVENT, GetPlayerDataEvent,
            ErrorId, mErrno);
    };

    struct SystemNoticeNotifyEvent
    {
        SystemNoticeNotifyEvent() : mContents("") {}

        String mContents;

        LYNX_MESSAGE_1(HORSE_LAMP_NOTIFY_EVENT, SystemNoticeNotifyEvent,
            String, mContents);
    };


} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_EVENT_DEFINITION_H__

