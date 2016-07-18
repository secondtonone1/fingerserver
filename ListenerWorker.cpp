#include "ListenerWorker.h"


using namespace Lynx;

#define CLIENT_CONNECTION_SECURITY_CHECK_INTERVAL 5

ListenerWorker::ListenerWorker() : mIOService(NULL), mMainTcpListener(NULL), mLastSecurityCheckTime(0)
                                 , mUpdateTimer(NULL), mGMTcpListener(NULL)
{

}

ListenerWorker::~ListenerWorker()
{

}

bool 
ListenerWorker::initial(UInt32 index, const String& mainListenIp, UInt16 mainListenPort,
                        const String& gmListenIp, UInt16 gmListenPort)
{
#ifdef __linux__
	mIOService = XNEW(IOService)(IOSERVICE_EPOLL_DRIVER);
#else
	mIOService = XNEW(IOService)();
#endif
	ASSERT(mIOService);

    mMainTcpListener = XNEW(TcpListener)(mIOService, mainListenIp.c_str(), mainListenPort);
	ASSERT(mMainTcpListener);
	LYNX_REGISTER_ACCEPTED(mMainTcpListener, this, &ListenerWorker::onMainAccepted);
	mMainTcpListener->asyncAccept(XNEW(TcpConnection)(mIOService));

    mGMTcpListener = XNEW(TcpListener)(mIOService, gmListenIp.c_str(), gmListenPort);
    ASSERT(mGMTcpListener);
    LYNX_REGISTER_ACCEPTED(mGMTcpListener, this, &ListenerWorker::onGMAccepted);
	mGMTcpListener->asyncAccept(XNEW(TcpConnection)(mIOService));

    mWorkerIndex = index + 1;
    startup();

    LOG_INFO("Start ListenWorker [%u]", mWorkerIndex);
    return true;
}

void 
ListenerWorker::release()
{
    
    if (mIOService)
    {
        mIOService->stop();
    }

    shutdown();
    join();

    if (mMainTcpListener)
	{
        if (mMainTcpListener->mRecvEvent.mOwner)
        {
            TcpConnection* tcpConnection = (TcpConnection*)mMainTcpListener->mRecvEvent.mOwner;
            if (tcpConnection)
            {
                XDELETE(tcpConnection);
            }
        }
        LYNX_DEREGISTER_ACCEPTED(mMainTcpListener, this, &ListenerWorker::onMainAccepted);
		XSAFE_DELETE(mMainTcpListener);
	}

    if (mGMTcpListener)
	{
        if (mGMTcpListener->mRecvEvent.mOwner)
        {
            TcpConnection* tcpConnection = (TcpConnection*)mGMTcpListener->mRecvEvent.mOwner;
            if (tcpConnection)
            {
                XDELETE(tcpConnection);
            }
        }
        LYNX_DEREGISTER_ACCEPTED(mGMTcpListener, this, &ListenerWorker::onGMAccepted);
		XSAFE_DELETE(mGMTcpListener);
	}

	if (mIOService)
	{
		XSAFE_DELETE(mIOService);
	}
    LOG_INFO("Shutdown ListenWorker [%u]", mWorkerIndex);
}


void 
ListenerWorker::onThreadStarted()
{
	mUpdateTimer = XNEW(IOTimer)(mIOService);
    mUpdateTimer->set(50, IOTimerCallback(this, &ListenerWorker::onUpdateTimer), NULL);
}

void 
ListenerWorker::onThreadShutdown()
{
    if (mUpdateTimer)
    {
        mUpdateTimer->cancel();
        XDELETE(mUpdateTimer);
        mUpdateTimer = NULL;
    }
}

void 
ListenerWorker::onPreUpdate()
{
	if (mIOService)
    {
        mIOService->run();
    }
}

void 
ListenerWorker::onPostUpdate()
{
	
}

void 
ListenerWorker::onDispatchMsg(ThreadMsg& threadMsg)
{

}

bool 
ListenerWorker::onUpdateTimer(void* args)
{
    UInt32 currTime = TimeUtil::getTimeSec();

	if (currTime - mLastSecurityCheckTime >= 300000)
	{
		mLastSecurityCheckTime = currTime;

		for (Set<TcpConnection*>::Iter* iter = mMainConnectionSet.begin(); iter != NULL;)
		{
			TcpConnection* conn = iter->mValue;
			if (conn)
			{
				if (currTime - conn->getRecvTimeStamp() >= 
					CLIENT_CONNECTION_SECURITY_CHECK_INTERVAL)
				{
					LOG_WARN("Connection [%s:%u] was closed for timeout.", 
						conn->getFromIp().c_str(), conn->getFromPort());
					LYNX_DEREGISTER_RECEIVED(conn, this, &ListenerWorker::onMainReceived);
					LYNX_DEREGISTER_CONNECT_BROKEN(conn, this, &ListenerWorker::onMainDisconnected);
					conn->close();
					iter = mMainConnectionSet.erase(iter);
					XDELETE(conn);
				}
				else
				{
					iter = mMainConnectionSet.next(iter);
				}
			}
		}

        for (Set<TcpConnection*>::Iter* iter = mGMConnectionSet.begin(); iter != NULL;)
		{
			TcpConnection* conn = iter->mValue;
			if (conn)
			{
				if (currTime - conn->getRecvTimeStamp() >= 
					CLIENT_CONNECTION_SECURITY_CHECK_INTERVAL)
				{
					LOG_WARN("Connection [%s:%u] was closed for timeout.", 
						conn->getFromIp().c_str(), conn->getFromPort());
					LYNX_DEREGISTER_RECEIVED(conn, this, &ListenerWorker::onGMReceived);
					LYNX_DEREGISTER_CONNECT_BROKEN(conn, this, &ListenerWorker::onGMDisconnected);
					conn->close();
					iter = mGMConnectionSet.erase(iter);
					XDELETE(conn);
				}
				else
				{
					iter = mGMConnectionSet.next(iter);
				}
			}
		}
	}
    return true;
}

void 
ListenerWorker::onMainAccepted(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
	LOG_INFO("Player Connection arrived from [%s:%u]", conn->getFromIp().c_str(), conn->getFromPort());
	mMainConnectionSet.insert(conn);
	conn->mRecvTimeStamp = TimeUtil::getTimeSec();
	//旧的连接用于绑定mOnReceived事件了 王戊辰
	LYNX_REGISTER_RECEIVED(conn, this, &ListenerWorker::onMainReceived);
	LYNX_REGISTER_CONNECT_BROKEN(conn, this, &ListenerWorker::onMainDisconnected);
	//新new一个conn用于监听新的客户端连接 王戊辰
	mMainTcpListener->asyncAccept(XNEW(TcpConnection)(mIOService));
	
}

void 
ListenerWorker::onMainAcceptFailed(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
	LOG_WARN("Failed to accept a tcp connection.");
	XDELETE(conn);
}

void 
ListenerWorker::onMainDisconnected(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
	LOG_INFO("Player Connection leaved from [%s:%u], ConnectionSetSize: %u",
		conn->getFromIp().c_str(), conn->getFromPort(), mMainConnectionSet.size());

    mMainConnectionSet.erase(conn);
	XDELETE(conn);
}

void 
ListenerWorker::onMainReceived(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
#if 0
	if (istream.length() < sizeof(UInt32) + strlen(sClientConnectionStr))
	{
		return;
	}

	UInt32 connStrLen = 0;
	istream >> connStrLen;
	connStrLen = connStrLen ^ 2865;

	if (connStrLen != strlen(sClientConnectionStr))
	{
		LOG_WARN("Player Connection string length don't match.");
		conn->close();
		return;
	}

	String connStr;
	for (UInt32 i = 0; i < connStrLen; ++i)
	{
		unsigned char c;
		istream >> c;
		c = c ^ 103;
		connStr += c;
	}

	LOG_DEBUG("Received a Player connection string [%s]", connStr.c_str());

	if (connStr != sClientConnectionStr)
	{
		LOG_WARN("Player Connection string don't match.");
		conn->close();
		return;
	}
#endif
	//删除回调函数，绑定别的回调函数做准备 王戊辰
	LYNX_DEREGISTER_RECEIVED(conn, this, &ListenerWorker::onMainReceived);
	LYNX_DEREGISTER_CONNECT_BROKEN(conn, this, &ListenerWorker::onMainDisconnected);
	//从主链接上删除，绑定到network的连接上 王戊辰
	mMainConnectionSet.erase(conn);
	conn->pause();
	//把io服务断开 王戊辰
    conn->setService(NULL);

	ConnectionAcceptedNotify notify;
    notify.mType = 0;
	notify.mConnPointer = conn;
	postMsgToOutputQueue(notify, 0);
}

void 
ListenerWorker::onGMAccepted(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    LOG_INFO("GMClient Connection arrived from [%s:%u]", conn->getFromIp().c_str(), conn->getFromPort());
	mGMConnectionSet.insert(conn);
	conn->mRecvTimeStamp = TimeUtil::getTimeSec();
	LYNX_REGISTER_RECEIVED(conn, this, &ListenerWorker::onGMReceived);
	LYNX_REGISTER_CONNECT_BROKEN(conn, this, &ListenerWorker::onGMDisconnected);
	mGMTcpListener->asyncAccept(XNEW(TcpConnection)(mIOService));
}

void 
ListenerWorker::onGMAcceptFailed(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    LOG_WARN("Failed to accept a tcp connection.");
	XDELETE(conn);
}

void 
ListenerWorker::onGMDisconnected(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    LOG_INFO("GMClient Connection leaved from [%s:%u], ConnectionSetSize: %u",
		conn->getFromIp().c_str(), conn->getFromPort(), mGMConnectionSet.size());

    mGMConnectionSet.erase(conn);
	XDELETE(conn);
}

void 
ListenerWorker::onGMReceived(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
#if 0
	if (istream.length() < sizeof(UInt32) + strlen(sGMConnectionStr))
	{
		return;
	}

	UInt32 connStrLen = 0;
	istream >> connStrLen;
	connStrLen = connStrLen ^ 2865;

	if (connStrLen != strlen(sGMConnectionStr))
	{
		LOG_WARN("GMClient Connection string length don't match.");
		conn->close();
		return;
	}

	String connStr;
	for (UInt32 i = 0; i < connStrLen; ++i)
	{
		unsigned char c;
		istream >> c;
		c = c ^ 103;
		connStr += c;
	}

	LOG_DEBUG("Received a GMClient connection string [%s]", connStr.c_str());

	if (connStr != sGMConnectionStr)
	{
		LOG_WARN("GMClient Connection string don't match.");
		conn->close();
		return;
	}
#endif
	LYNX_DEREGISTER_RECEIVED(conn, this, &ListenerWorker::onGMReceived);
	LYNX_DEREGISTER_CONNECT_BROKEN(conn, this, &ListenerWorker::onGMDisconnected);
	mGMConnectionSet.erase(conn);
	conn->pause();
    conn->setService(NULL);

	ConnectionAcceptedNotify notify;
    notify.mType = 1;
	notify.mConnPointer = conn;
	postMsgToOutputQueue(notify, 0);
}
