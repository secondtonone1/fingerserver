#include "NetworkWorker.h"

using namespace Lynx;

NetworkWorker::NetworkWorker() : mUpdateTimer(NULL)
{

}

NetworkWorker::~NetworkWorker()
{

}

bool 
NetworkWorker::initial(UInt32 index)
{
#ifdef __linux__
	mIOService = XNEW(IOService)(IOSERVICE_EPOLL_DRIVER);
#else
	mIOService = XNEW(IOService)();
#endif
	ASSERT(mIOService);

	REGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionOpenNotify, NetworkWorker::onConnectionOpenNotify);
	REGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionSendNotify, NetworkWorker::onConnectionSendNotify);
    REGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionBroadcastNotify, NetworkWorker::onConnectionBroadcastNotify);
	REGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionCloseNotify, NetworkWorker::onConnectionCloseNotify);

    mWorkerIndex = index + 1;
    startup();

    LOG_INFO("Start NetworkWorker [%u]", mWorkerIndex);
    return true;
}

void 
NetworkWorker::release()
{
    if (mIOService)
    {
        mIOService->stop();
    }

    shutdown();
    join();

    DEREGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionOpenNotify);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionSendNotify);
    DEREGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionBroadcastNotify);
	DEREGISTER_THREAD_MSG(mThreadMsgHandler, ConnectionCloseNotify);

    if (mIOService)
	{
		XSAFE_DELETE(mIOService);
	}

    LOG_INFO("Shutdown NetworkWorker [%u]", mWorkerIndex);
}

void 
NetworkWorker::onThreadStarted()
{
	mUpdateTimer = XNEW(IOTimer)(mIOService);
    mUpdateTimer->set(10, IOTimerCallback(this, &NetworkWorker::onUpdateTimer), NULL);
}

void 
NetworkWorker::onThreadShutdown()
{
    if (mUpdateTimer)
    {
        mUpdateTimer->cancel();
        XDELETE(mUpdateTimer);
        mUpdateTimer = NULL;
    }
}

void 
NetworkWorker::onPreUpdate()
{
    if (mIOService)
    {
        mIOService->run();
    }
}

void 
NetworkWorker::onPostUpdate()
{

}

void 
NetworkWorker::onDispatchMsg(ThreadMsg& threadMsg)
{
	mThreadMsgHandler.onDispatchMsg(threadMsg);
}

bool 
NetworkWorker::onUpdateTimer(void* args)
{
    StreamBuffer& threadMsgStream = mInputMsgQueue.getMsgs();
    UInt32 msgCount = threadMsgStream.length() / sizeof(void*);
    if (msgCount == 0)
    {
        return true;
    }

    void* threadMsgPointer;
    ThreadMsg* threadMsg;

    for (UInt32 i = 0; i < msgCount; ++i)
    {
        threadMsgStream >> threadMsgPointer;
        threadMsg = (ThreadMsg*)threadMsgPointer;
        if (threadMsg)
        {
            onDispatchMsg(*threadMsg);
            deallocThreadMsg(threadMsg);
        }
    }

    return true;
}

void 
NetworkWorker::onReceived(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
	if (conn->mIStream.length() > TCP_MAX_CACHE_ISTREAM_LENGTH ||
		conn->mOStream.length() > TCP_MAX_CACHE_OSTREAM_LENGTH)
	{
		LOG_WARN("Tcp connection stream overflow.");
		conn->close();
		return;
	}

	Map<ConnId, TcpConnection*>::Iter* iter = mTcpConnectionMap.find(conn->mConnId);
	if (iter == NULL)
	{
		LOG_WARN("Tcp connection don't exist.");
		conn->close();
		return;
	}

	UInt32 nodeCount = istream.getNodeCount();
	if (nodeCount == 0)
	{
		LOG_WARN("Logic error for istream node count == 0");
		conn->close();
		return;
	}

	ConnectionReceiveNotify notify;
	notify.mConnId = conn->mConnId;
	StreamBuffer::Node* node = NULL;
	while (node = istream.popNode())
	{
		notify.mNodeList.push_back(node);
	}

	postMsgToOutputQueue(notify, 0);
}

void
NetworkWorker::onConnectionBroken(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
	ConnId connId = conn->mConnId;
	Map<ConnId, TcpConnection*>::Iter* iter = mTcpConnectionMap.find(connId);
	if (iter == NULL)
	{
		LOG_WARN("Tcp connection isn't exist.");
		return;
	}

	TcpConnection* tcpConnection = iter->mValue;
	if (tcpConnection == NULL)
	{
		LOG_WARN("Tcp connection pointer is NULL.");
		return;
	}

	ConnectionBrokenNotify notify;
	notify.mConnId = connId;
	postMsgToOutputQueue(notify, 0);

	LYNX_DEREGISTER_RECEIVED(tcpConnection, this, &NetworkWorker::onReceived);
	LYNX_DEREGISTER_CONNECT_BROKEN(tcpConnection, this, &NetworkWorker::onConnectionBroken);

	tcpConnection->close();
	mTcpConnectionMap.erase(iter);
	XDELETE(tcpConnection);
}

void 
NetworkWorker::onConnectionOpenNotify(ConnectionOpenNotify& msg)
{
	TcpConnection* tcpConnection = (TcpConnection*)msg.mConnPointer;
	if (!tcpConnection)
	{
		LOG_WARN("Tcp connection pointer is NULL.");
		return;
	}

	//Map<ConnId, TcpConnection*>::Iter* iter = mTcpConnectionMap.find(msg.mConnId);
	//if (iter)
	//{
	//	LOG_WARN("Tcp connection id %llu repeated.", msg.mConnId);
	//	tcpConnection->close();
	//	XDELETE(tcpConnection);
	//	return;
	//}

	mTcpConnectionMap.insert(msg.mConnId, tcpConnection);
	//设置connid  王戊辰
	tcpConnection->mConnId = msg.mConnId;
	//将connection绑定新的回调函数  王戊辰
    LYNX_REGISTER_RECEIVED(tcpConnection, this, &NetworkWorker::onReceived);
	LYNX_REGISTER_CONNECT_BROKEN(tcpConnection, this, &NetworkWorker::onConnectionBroken);
	tcpConnection->setService(mIOService);
	//回复读写事件 王戊辰
    tcpConnection->resume();
	//考虑到epoll et模式 EPOLLIN边缘触发，这次不调用，就要漏掉数据了。王戊辰
    onReceived(tcpConnection, tcpConnection->mIStream, tcpConnection->mOStream);
}

void 
NetworkWorker::onConnectionSendNotify(ConnectionSendNotify& msg)
{
	ConnId connId = msg.mConnId;
	Map<ConnId, TcpConnection*>::Iter* iter = mTcpConnectionMap.find(connId);
	if (iter == NULL)
	{
		LOG_DEBUG("Tcp connection isn't exist.");
		for (Vector<void*>::iterator jter = msg.mNodeList.begin();
			jter != msg.mNodeList.end(); ++jter)
		{
			XDELETE((StreamBuffer::Node*)(*jter));
		}

		ConnectionBrokenNotify notify;
		notify.mConnId = connId;
		postMsgToOutputQueue(notify, 0);
		return;
	}

	TcpConnection* tcpConnection = iter->mValue;
	if (tcpConnection == NULL)
	{
		LOG_DEBUG("Tcp connection pointer is NULL.");
		for (Vector<void*>::iterator jter = msg.mNodeList.begin();
			jter != msg.mNodeList.end(); ++jter)
		{
			XDELETE((StreamBuffer::Node*)(*jter));
		}

		mTcpConnectionMap.erase(iter);

		ConnectionBrokenNotify notify;
		notify.mConnId = connId;
		postMsgToOutputQueue(notify, 0);
		return;
	}
	
	for (Vector<void*>::iterator i = msg.mNodeList.begin();
		i != msg.mNodeList.end(); ++i)
	{
		StreamBuffer::Node* node = (StreamBuffer::Node*)(*i);
		if (node)
		{
			tcpConnection->mOStream.pushNode(node);
		}
	}

	if (tcpConnection->mIStream.length() > TCP_MAX_CACHE_ISTREAM_LENGTH ||
		tcpConnection->mOStream.length() > TCP_MAX_CACHE_OSTREAM_LENGTH)
	{
		LOG_WARN("Tcp connection stream overflow");
		tcpConnection->close();
	}
	else
	{
		tcpConnection->asyncSend();
	}
}

void 
NetworkWorker::onConnectionBroadcastNotify(ConnectionBroadcastNotify& msg)
{
    char* buffer = (char*)msg.mBuffer;
    if (!buffer) return;

    for (Map<ConnId, TcpConnection*>::Iter* iter = mTcpConnectionMap.begin();
        iter != mTcpConnectionMap.end(); iter = mTcpConnectionMap.next(iter))
    {
        TcpConnection* tcpConnection = iter->mValue;
        if (tcpConnection)
        {
            tcpConnection->asyncSend(buffer, msg.mBufferLen);
        }
    }
    XFREE(buffer);
}

void 
NetworkWorker::onConnectionCloseNotify(ConnectionCloseNotify& msg)
{
	ConnId connId = msg.mConnId;
	Map<ConnId, TcpConnection*>::Iter* iter = mTcpConnectionMap.find(connId);
	if (iter == NULL)
	{
		LOG_DEBUG("Tcp connection isn't exist.");
		return;
	}

	TcpConnection* tcpConnection = iter->mValue;
	if (tcpConnection == NULL)
	{
		LOG_DEBUG("Tcp connection pointer is NULL.");
		return;
	}

	LYNX_DEREGISTER_RECEIVED(tcpConnection, this, &NetworkWorker::onReceived);
	LYNX_DEREGISTER_CONNECT_BROKEN(tcpConnection, this, &NetworkWorker::onConnectionBroken);

	tcpConnection->close();
	mTcpConnectionMap.erase(iter);
	XDELETE(tcpConnection);
}


