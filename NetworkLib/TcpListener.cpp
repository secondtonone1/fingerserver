#include "TcpListener.h"
#include "IOService.h"
#include "Logger.h"

#ifdef _WIN32
# include <winsock2.h>
#else
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netinet/in.h>
#endif

using namespace Lynx;

TcpListener::TcpListener(IOService* ioService, const String& srcIp, UInt16 srcPort) 
: Socket(TRANSPORT_TCP), mIOService(ioService), mIStream((128 - sizeof(StreamBuffer::Node)))
{
    mStrSrcIp = srcIp;
    if (!mStrSrcIp.empty())
    {
        mSrcIp = inet_addr(srcIp.c_str());
    }
    mSrcPort = srcPort;

    Int32 fd = open();
    if (fd < 0)
    {
        LOG_WARN("Failed to open a socket.");
        ASSERT(0);
    }

    mFd = fd;

    if (bindSocket(true) == -1)
    {
        LOG_WARN("Failed to bind socket addr ip:%s, port: %d", mStrSrcIp.c_str(), mSrcPort);
        ASSERT(0);
    }

    if (tcpListen() != 0)
    {
        LOG_WARN("Failed to listen a tcp port.");
        ASSERT(0);
    }

    mRecvEvent.mFd = mFd;
    mRecvEvent.mEventType = IOSERVICE_READ_EVENT;
    mRecvEvent.mCallback = &onAccepted;
    mRecvEvent.mData = mIStream.allocNode();

    //if (mIOService->getType() == IOSERVICE_IOCP_DRIVER)
    //{
    //    mRecvEvent.mListenFd = mFd;
    //    mRecvEvent.mPostListen = 1;
    //}

    if (mIOService->addEvent(&mRecvEvent) < 0)
    {
        LOG_WARN("Failed to add read ev for tcp listen.");
        ASSERT(0);
    }
}

TcpListener::~TcpListener() 
{
    //if (mRecvEvent.mOwner)
    //{
    //    if (!mOnAcceptFailed.empty())
    //    {
    //        TcpConnection* conn = (TcpConnection*)mRecvEvent.mOwner;
    //        mOnAcceptFailed(conn, conn->mIStream, conn->mOStream);
    //    }
    //    mRecvEvent.mOwner = NULL;
    //}
    close();
    if (mRecvEvent.mData)
    {
        mIStream.deallocNode(mRecvEvent.mData);
        mRecvEvent.mData = NULL;
    }
}

void 
TcpListener::close()
{
    mIOService->delEvent(&mRecvEvent);
    Socket::close();
}

void 
TcpListener::asyncAccept(TcpConnection* conn)
{
    conn->mIOTcpListener = this;
    mRecvEvent.mOwner = conn;
    //if (mIOService->getType() == IOSERVICE_IOCP_DRIVER)
    //{
    //    mRecvEvent.mPostAccept = 1;
    //    mRecvEvent.mData->mLen = 0;
    //    mRecvEvent.mData->mOff = 0;
    //    if (mIOService->addEvent(&mRecvEvent) < 0)
    //    {
    //        LOG_WARN("Failed to add a io event, fd=%d.", mFd);
    //        if (!mOnAcceptFailed.empty())
    //        {
    //            mOnAcceptFailed(conn, conn->mIStream, conn->mOStream);
    //        }
    //        //mRecvEvent.mOwner = NULL;
    //    }
    //}
}

void 
TcpListener::onAccepted(IOEvent* ev, UInt32 eventType)
{
    if (eventType & IOSERVICE_READ_EVENT)
    {
        TcpConnection* conn = (TcpConnection *)ev->mOwner;
        if (conn == NULL)
        {
            LOG_WARN("Exception for invalid conn pointer");
            return;
        }

        TcpListener* listener = conn->mIOTcpListener;
        if (listener == NULL)
        {
            LOG_WARN("Exception for invalid conn pointer");
            return;
        }

        //if (listener->mIOService->getType() == IOSERVICE_IOCP_DRIVER)
        //{
        //    if (ev->mPostListen)
        //    {
        //        ev->mPostListen = 0;
        //        return;
        //    }
        //}

        if (ev->mErrno != LYNX_ERROR_NONE)
        {
            LOG_WARN("Failed to accept tcp connection errno %d", ev->mErrno);
            if (!listener->mOnAcceptFailed.empty())
            {
                listener->mOnAcceptFailed(conn, conn->mIStream, conn->mOStream);
            }
            //ev->mOwner = NULL;
            return;
        }

        //if (listener->mIOService->getType() == IOSERVICE_IOCP_DRIVER)
        //{
        //    ASSERT(conn->mIOService == listener->mIOService);
        //    conn->mFd = ev->mFd;
        //    conn->mDstIp = ev->mDstIp;
        //    conn->mDstPort = ev->mDstPort;
        //    char tmpBuf[128];
        //    inet_ntop(AF_INET, &conn->mDstIp, tmpBuf, 128);
        //    conn->mStrDstIp = tmpBuf;
        //    conn->resume();
        //    conn->asyncReceive();
        //    if (!listener->mOnAccepted.empty())
        //    {
        //        listener->mOnAccepted(conn, conn->mIStream, conn->mOStream);
        //    }
        //    //ev->mOwner = NULL;
        //}
        //else
        {
            conn->mFd = listener->tcpAccept(conn->mDstIp, conn->mDstPort);
            if (conn->mFd > 0)
            {
                ASSERT(conn->mIOService == listener->mIOService);
                char tmpBuf[128];
                inet_ntop(AF_INET, &conn->mDstIp, tmpBuf, 128);
                conn->mStrDstIp = tmpBuf;
                if (conn->setSocketNonblocking() < 0)
                {
                    LOG_WARN("Failed to set connection fd %d to non blocking", conn->mFd);
                    if (!listener->mOnAcceptFailed.empty())
                    {
                        listener->mOnAcceptFailed(conn, conn->mIStream, conn->mOStream);
                    }
                    //ev->mOwner = NULL;
                    return;
                }
				//设置connection 的mRecvEvent回调事件为onReceived 王戊辰
                conn->resume();
				//设置为pending true 王戊辰
                conn->asyncReceive();
                if (!listener->mOnAccepted.empty())
                {
                    listener->mOnAccepted(conn, conn->mIStream, conn->mOStream);
                }
                //ev->mOwner = NULL;
            }
            else if (conn->mFd <= 0)
            {
                LOG_WARN("Failed to accept tcp connection errno %d", ev->mErrno);
                if (!listener->mOnAcceptFailed.empty())
                {
                    listener->mOnAcceptFailed(conn, conn->mIStream, conn->mOStream);
                }
                //ev->mOwner = NULL;
            }
        }
    }
    else
    {
        LOG_WARN("Exception for unknown logic.");
        ASSERT(0);
    }
}


