#ifndef __LYNX_GAME_MASTER_TOOL_GMCLIENT_H__
#define __LYNX_GAME_MASTER_TOOL_GMCLIENT_H__

#include "CommonLib.h"
#include "GMMsgHandler.h"

namespace Lynx
{
    class GMClient : public TcpConnection
    {
    public:
        GMClient(IOService* ioService);
        virtual ~GMClient();

        enum { UNLOGIN_STATE = 0, LOGINING_STATE = 1, LOGINED_STATE = 2 };

        bool initial(const GMServerInfo& serverInfo);
        void release();
        void update(const UInt64& accTime);

        void connectServer();
        void disconnectServer();

        template <typename MsgType >
        void sendMsg(const MsgType& msg)
        {
            if (mIsConnecting)
            {
                mMsgHandler.sendMsg(*this, msg);
            }
        }

        UInt8 getLoginState() const { return mLoginState; }
        const String& getServerName() const { return mGMServerInfo.mServerName; }
        const String& getServerIp() const { return mGMServerInfo.mServerIp; }
        UInt16 getServerPort() const { return mGMServerInfo.mServerPort; }
        UInt32 getPlayerCount() const { return mPlayerCount; }
        const PlayerData& getPlayerData() const { return mPlayerData; }

        void registerMsgHandler();
        void deregisterMsgHandler();

    private:
        void onConnected(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
        void onConnectFailed(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
        void onConnectBroken(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);

    private:
        void onLoginSuccessResp(GMLoginSuccessResp& msg);
        void onLoginFailResp(GMLoginFailResp& msg);
        void onAccountWasLoginNotify(GMAccountWasLoginNotify& msg);
        void onGetPlayerByOnlineResp(GMGetPlayerByOnlineResp& msg);
        void onGetPlayerByPlayerGuidResp(GMGetPlayerByPlayerGuidResp& msg);
        void onGetPlayerByPlayerNameResp(GMGetPlayerByPlayerNameResp& msg);
        void onGetPlayerByAccountGuidResp(GMGetPlayerByAccountGuidResp& msg);
        void onGetPlayerByAccountNameResp(GMGetPlayerByAccountNameResp& msg);
        void onGetPlayerDataResp(GMGetPlayerDataResp& msg);
        void onGMCommandResp(GMCommandResp& msg);
        void onSystemNoticeNotify(SystemNoticeNotify& msg);

    private:
        IOService* mIOService;
        GMMsgHandler mMsgHandler;
        MsgHdr mMsgHdr;
        bool mIsConnecting;
        bool mStopFlag;
        GMData mGMData;
        GMServerInfo mGMServerInfo;
        UInt8 mLoginState;
        UInt32 mPlayerCount;
        PlayerData mPlayerData;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_GMCLIENT_H__

