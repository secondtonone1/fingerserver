#include "GMClient.h"
#include "GameMasterTool.h"

using namespace Lynx;

GMClient::GMClient(IOService* ioService) : TcpConnection(ioService),
mLoginState(UNLOGIN_STATE), mPlayerCount(0)
{

}

GMClient::~GMClient()
{

}

bool 
GMClient::initial(const GMServerInfo& serverInfo)
{
    mGMServerInfo = serverInfo;

    mUserData = &mMsgHdr;
    LYNX_REGISTER_CONNECTED(this, this, &GMClient::onConnected);
    LYNX_REGISTER_CONNECT_FAILED(this, this, &GMClient::onConnectFailed);
    LYNX_REGISTER_CONNECT_BROKEN(this, this, &GMClient::onConnectBroken);
    mMsgHandler.initialize(this);

    registerMsgHandler();

    mLoginState = UNLOGIN_STATE;
    return true;
}

void 
GMClient::release()
{
    mLoginState = UNLOGIN_STATE;
    deregisterMsgHandler();
}

void
GMClient::update(const UInt64& accTime)
{
    
}

void 
GMClient::connectServer()
{
    close();
    mLoginState = LOGINING_STATE;
    asyncConnect(mGMServerInfo.mServerIp, mGMServerInfo.mServerPort);
}

void 
GMClient::disconnectServer()
{
    mLoginState = UNLOGIN_STATE;
    close();
    UpdateServerPanelEvent ev;
    EventSystem::getSingleton().postEvent(ev);

    LoginBrokenEvent loginBrokenEvent;
    loginBrokenEvent.mServerName = getServerName();
    EventSystem::getSingleton().postEvent(loginBrokenEvent);
}

void 
GMClient::registerMsgHandler()
{
    REGISTER_MSG(mMsgHandler, GMLoginSuccessResp, GMClient::onLoginSuccessResp);
    REGISTER_MSG(mMsgHandler, GMLoginFailResp, GMClient::onLoginFailResp);
    REGISTER_MSG(mMsgHandler, GMAccountWasLoginNotify, GMClient::onAccountWasLoginNotify);
    REGISTER_MSG(mMsgHandler, GMGetPlayerByOnlineResp, GMClient::onGetPlayerByOnlineResp);
    REGISTER_MSG(mMsgHandler, GMGetPlayerByPlayerGuidResp, GMClient::onGetPlayerByPlayerGuidResp);
    REGISTER_MSG(mMsgHandler, GMGetPlayerByPlayerNameResp, GMClient::onGetPlayerByPlayerNameResp);
    REGISTER_MSG(mMsgHandler, GMGetPlayerByAccountGuidResp, GMClient::onGetPlayerByAccountGuidResp);
    REGISTER_MSG(mMsgHandler, GMGetPlayerByAccountNameResp, GMClient::onGetPlayerByAccountNameResp);
    REGISTER_MSG(mMsgHandler, GMGetPlayerDataResp, GMClient::onGetPlayerDataResp);
    REGISTER_MSG(mMsgHandler, GMCommandResp, GMClient::onGMCommandResp);
    REGISTER_MSG(mMsgHandler, SystemNoticeNotify, GMClient::onSystemNoticeNotify);
}

void 
GMClient::deregisterMsgHandler()
{
    DEREGISTER_MSG(mMsgHandler, GMLoginSuccessResp);
    DEREGISTER_MSG(mMsgHandler, GMLoginFailResp);
    DEREGISTER_MSG(mMsgHandler, GMAccountWasLoginNotify);
    DEREGISTER_MSG(mMsgHandler, GMGetPlayerByOnlineResp);
    DEREGISTER_MSG(mMsgHandler, GMGetPlayerByPlayerGuidResp); 
    DEREGISTER_MSG(mMsgHandler, GMGetPlayerByPlayerNameResp);
    DEREGISTER_MSG(mMsgHandler, GMGetPlayerByAccountGuidResp);
    DEREGISTER_MSG(mMsgHandler, GMGetPlayerByAccountNameResp);
    DEREGISTER_MSG(mMsgHandler, GMGetPlayerDataResp);
    DEREGISTER_MSG(mMsgHandler, GMCommandResp);
}


void 
GMClient::onConnected(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    LOG_WARN("Connection Login [%s:%u] success.", conn->getFromIp().c_str(), conn->getFromPort());
    mLoginState = LOGINING_STATE;

    GMLoginReq loginReq;
    loginReq.mAccountName = LogicSystem::getSingleton().getAccountName();
    loginReq.mAccountPassword = LogicSystem::getSingleton().getAccountPassword();
    sendMsg(loginReq);
}

void 
GMClient::onConnectFailed(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    LOG_WARN("Connection Login [%s:%u] failed.", conn->getFromIp().c_str(), conn->getFromPort());
    mLoginState = UNLOGIN_STATE;
}

void 
GMClient::onConnectBroken(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    LOG_WARN("Connecton Broken with server [%s:%u]", conn->getFromIp().c_str(), conn->getFromPort());
    mLoginState = UNLOGIN_STATE;

    UpdateServerPanelEvent ev;
    EventSystem::getSingleton().postEvent(ev);

    LoginBrokenEvent loginBrokenEvent;
    loginBrokenEvent.mServerName = getServerName();
    EventSystem::getSingleton().postEvent(loginBrokenEvent);
}

void 
GMClient::onLoginSuccessResp(GMLoginSuccessResp& msg)
{
    mGMData = msg.mGMData;
    mLoginState = LOGINED_STATE;

    UpdateServerPanelEvent ev;
    EventSystem::getSingleton().postEvent(ev);

    LoginSuccessEvent loginSuccessEvent;
    loginSuccessEvent.mServerName = getServerName();
    EventSystem::getSingleton().postEvent(loginSuccessEvent);
}

void 
GMClient::onLoginFailResp(GMLoginFailResp& msg)
{
    LOG_WARN("Failed to login for errno: %u", msg.mErrno);
    close();
    mLoginState = UNLOGIN_STATE;

    UpdateServerPanelEvent ev;
    EventSystem::getSingleton().postEvent(ev);
}

void 
GMClient::onAccountWasLoginNotify(GMAccountWasLoginNotify& msg)
{
    LOG_WARN("Account was login.");
}

void 
GMClient::onGetPlayerByOnlineResp(GMGetPlayerByOnlineResp& msg)
{
    GetPlayerByOnlineEvent ev;
    ev.mErrno = msg.mErrno;
    ev.mPlayerList = msg.mPlayerList;
    EventSystem::getSingleton().postEvent(ev);
}

void 
GMClient::onGetPlayerByPlayerGuidResp(GMGetPlayerByPlayerGuidResp& msg)
{
    GetPlayerByPlayerGuidEvent ev;
    ev.mErrno = msg.mErrno;
    ev.mOffline = msg.mOffline;

    if (msg.mErrno == LynxErrno::None)
    {
        mPlayerData.mBaseData = msg.mPlayerDataList.begin()->mValue;
    }

    EventSystem::getSingleton().postEvent(ev);
}

void 
GMClient::onGetPlayerByPlayerNameResp(GMGetPlayerByPlayerNameResp& msg)
{
    GetPlayerByPlayerNameEvent ev;
    ev.mErrno = msg.mErrno;
    ev.mOffline = msg.mOffline;

    if (msg.mErrno == LynxErrno::None)
    {
        mPlayerData.mBaseData = msg.mPlayerDataList.begin()->mValue;
    }

    EventSystem::getSingleton().postEvent(ev);
}

void 
GMClient::onGetPlayerByAccountGuidResp(GMGetPlayerByAccountGuidResp& msg)
{
    GetPlayerByAccountGuidEvent ev;
    ev.mErrno = msg.mErrno;
    ev.mOffline = msg.mOffline;

    if (msg.mErrno == LynxErrno::None)
    {
        mPlayerData.mBaseData = msg.mPlayerDataList.begin()->mValue;
    }

    EventSystem::getSingleton().postEvent(ev);
}

void 
GMClient::onGetPlayerByAccountNameResp(GMGetPlayerByAccountNameResp& msg)
{
    GetPlayerByAccountNameEvent ev;
    ev.mErrno = msg.mErrno;
    ev.mOffline = msg.mOffline;

    if (msg.mErrno == LynxErrno::None)
    {
        mPlayerData.mBaseData = msg.mPlayerDataList.begin()->mValue;
    }

    EventSystem::getSingleton().postEvent(ev);
}

void 
GMClient::onGetPlayerDataResp(GMGetPlayerDataResp& msg)
{
    GetPlayerDataEvent ev;
    ev.mErrno = msg.mErrno;

    if (msg.mErrno == LynxErrno::None)
    {
        mPlayerData = msg.mPlayerDataList.begin()->mValue;
    }

    EventSystem::getSingleton().postEvent(ev);
}

void
GMClient::onGMCommandResp(GMCommandResp& msg)
{
    WindowLogPrintEvent printEvent;
    printEvent.mLogContents = msg.mContents;
    EventSystem::getSingleton().postEvent(printEvent);
}

void
GMClient::onSystemNoticeNotify(SystemNoticeNotify& msg)
{
    SystemNoticeNotifyEvent ev;
    ev.mContents = msg.mContents;
    EventSystem::getSingleton().postEvent(ev);
}

