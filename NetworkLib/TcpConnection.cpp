#include "TcpConnection.h"
#include "IOService.h"
#include "Logger.h"
#include "TcpListener.h"
#include "IOTimer.h"

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <mswsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

using namespace Lynx;

TcpConnection::TcpConnection(IOService* ioService, UInt32 istreamBufferLen, UInt32 ostreamBufferLen,
							 UInt32 maxInMsgLen, UInt32 maxOutMsgLen) 
: Socket(TRANSPORT_TCP), mIOService(ioService), mIOTcpListener(0), mSendPending(false)
, mRecvPending(false), mMsgPending(false), mIStream((istreamBufferLen - sizeof(StreamBuffer::Node)), 2)
, mOStream((ostreamBufferLen - sizeof(StreamBuffer::Node)), 2), mIOTimer(NULL)
, mRecvTimeStamp(0), mSendTimeStamp(0), mValid(false), mReadUtilNull(false)
, mMaxInMsgLen(maxInMsgLen), mMaxOutMsgLen(maxOutMsgLen), mCloseTimerFlag(false)
, mSendByteCount(0), mRecvByteCount(0), mConnId(0)
{

}

TcpConnection::~TcpConnection()
{
    if (mIOTimer)
    {
        mIOTimer->cancel();
        XSAFE_DELETE(mIOTimer);
    }

    // clean read and write event remain invalid data.
    if (mRecvEvent.mData)
    {
        mIStream.deallocNode(mRecvEvent.mData);
        mRecvEvent.mData = NULL;
    }
    if (mSendEvent.mData)
    {
        mOStream.deallocNode(mSendEvent.mData);
        mSendEvent.mData = NULL;
    }

    mIStream.clear();
    mOStream.clear();

    cleanConnection();
}

void 
TcpConnection::close()
{
    if (mFd > 0)
    {
        cleanConnection();

        //if (mIOService->getType() != IOSERVICE_IOCP_DRIVER)
        //{
        //    if (mIOTimer == NULL)
        //    {
        //        if (!mOnConnectBroken.empty() || !mOnConnectFailed.empty())
        //        {
        //            mCloseTimerFlag = true;
        //            mIOTimer = XNEW(IOTimer)(mIOService);
        //            mIOTimer->set(1, IOTimerCallback(&TcpConnection::onTimer), this);
        //        }
        //    }
        //}
    }
}

void 
TcpConnection::clean()
{
    if (mFd > 0)
    {
        cleanConnection();
    }
}

void 
TcpConnection::cleanConnection()
{
    // Del connection add read and write event.
	if (mIOService)
	{
        mIOService->delEvent(&mRecvEvent);
        mIOService->delEvent(&mSendEvent);
	}

    //mRecvPending = mSendPending = false;
    
    // close socket file description.
    Socket::close();
}

void 
TcpConnection::pause()
{
    mValid = false;
    // Temp del read and write event.
    mIOService->delEvent(&mRecvEvent);
    mIOService->delEvent(&mSendEvent);

    mRecvPending = mSendPending = false;

    // clean read event remain invalid data.
    if (mRecvEvent.mData)
    {
        mIStream.deallocNode(mRecvEvent.mData);
        mRecvEvent.mData = NULL;
    }

    // clean write event remain data.
    if (mSendEvent.mData)
    {
        if (mSendEvent.mData->mLen > 0)
        {
            mOStream.insertNodeAtHead(mSendEvent.mData);
            mSendEvent.mData = NULL;
        }
        else
        {
            mOStream.deallocNode(mSendEvent.mData);
            mSendEvent.mData = NULL;
        }
        mSendEvent.mData = NULL;
    } 
}

void 
TcpConnection::resume()
{
    mValid = true;
    mRecvEvent.mFd = mFd;
    mRecvEvent.mEventType = IOSERVICE_READ_EVENT;
    mRecvEvent.mOwner = this;
    mRecvEvent.mCallback = &onReceived;
    mRecvPending = false;

    mSendEvent.mFd = mFd;
    mSendEvent.mEventType = IOSERVICE_WRITE_EVENT;
    mSendEvent.mOwner = this;
    mSendEvent.mCallback = &onSent;
    mSendPending = false;

    //if (mIOService->getType() == IOSERVICE_IOCP_DRIVER)
    //{
    //    mRecvEvent.mPostListen = mRecvEvent.mPostAccept = 0;
    //    mRecvEvent.mDataSize = mIStream.mNodeDataSize;
    //    mSendEvent.mPostConnect = 0;
    //}
    //else
    {
        // Add read event for connection persist.
        if (mIOService->addEvent(&mRecvEvent) < 0)
        {
            LOG_WARN("Failed to add read event, fd=%d.", mFd);
            if (mIOTimer == NULL)
            {
                mIOTimer = XNEW(IOTimer)(mIOService);
                mIOTimer->set(1, IOTimerCallback(&TcpConnection::onTimer), this);
            }
        }
    }
}

void 
TcpConnection::asyncConnect(const String& ip, UInt16 port)
{
    Int32 fd = open();
    if (fd < 0)
    {
        LOG_WARN("Failed to open a socket.");
        if (!mOnConnectFailed.empty())
        {
            mOnConnectFailed(this, mIStream, mOStream);
        }
        return;
    }

    mStrDstIp = ip;
    mDstIp = inet_addr(ip.c_str());
    mDstPort = port;

    //if (mIOService->getType() == IOSERVICE_IOCP_DRIVER)
    //{
    //    mSendEvent.mFd = mFd;
    //    mSendEvent.mEventType = IOSERVICE_WRITE_EVENT;
    //    mSendEvent.mOwner = this;
    //    mSendEvent.mCallback = &onConnected;
    //    mSendEvent.mDstIp = mDstIp;
    //    mSendEvent.mDstPort = mDstPort;
    //    mSendEvent.mPostConnect = 1;
    //
    //    if (mIOService->addEvent(&mSendEvent) < 0)
    //    {
    //        LOG_WARN("Failed to add wrtie event, fd=%d.", mFd);
    //        if (!mOnConnectFailed.empty())
    //        {
    //            mOnConnectFailed(this, mIStream, mOStream);
    //        }
    //        return;
    //    }
    //}
    //else // EPOLL AND SELECT
    {
        Int32 ret = tcpConnect(ip.c_str(), port);
        if (ret < 0)
        {
            LOG_WARN("Failed to establish tcp connection, dstip: %s, dstport: %d", 
                ip.c_str(), port);
            if (!mOnConnectFailed.empty())
            {
                mOnConnectFailed(this, mIStream, mOStream);
            }
            return;
        }
        else if (ret == 0) // Send async connection request.
        {
            mSendEvent.mFd = mFd;
            mSendEvent.mEventType = IOSERVICE_WRITE_EVENT;
            mSendEvent.mOwner = this;
            mSendEvent.mCallback = &onConnected;

            // Add write event for connect.
            if (mIOService->addEvent(&mSendEvent) < 0)
            {
                LOG_WARN("Failed to add a io event, fd=%d.", mFd);
                if (!mOnConnectFailed.empty())
                {
                    mOnConnectFailed(this, mIStream, mOStream);
                }
                return;
            }
        }
        else // Connection block success.
        {
            onConnected(&mSendEvent, IOSERVICE_WRITE_EVENT);
        }
    }
}

void 
TcpConnection::asyncReceive()
{
    if (!mValid) // Check connection is or not valid
    {
        LOG_WARN("Connection has invalid, fd %d", mFd);
        return;
    }

    if (mRecvPending == false)
    {
        mRecvPending = true;
        //if (mIOService->getType() == IOSERVICE_IOCP_DRIVER)
        //{
        //    mRecvEvent.mData = mIStream.allocNode();
        //    if (mIOService->addEvent(&mRecvEvent) < 0)
        //    {
        //        LOG_WARN("Failed to add read event, fd=%d.", mFd);
        //        if (mIOTimer == NULL)
        //        {
        //            mIOTimer = XNEW(IOTimer)(mIOService);
        //            mIOTimer->set(1, IOTimerCallback(&TcpConnection::onTimer), this);
        //        }
        //    }
        //}
    }
}
        
void 
TcpConnection::asyncSend()
{
    if (!mValid) // Check connection is or not valid
    {
        LOG_WARN("Connection has invalid fd %d", mFd);
        return;
    }

    if (mSendPending == false)
    {
        mSendEvent.mData = mOStream.popNode();
        // If ostream is null then del write event.
        if (!mSendEvent.mData)
        {
            // If write event wasn't add then nothing to do.
            mIOService->delEvent(&mSendEvent);
            return;
        }

        if (mSendEvent.mData->mLen == 0)
        {
            mIOService->delEvent(&mSendEvent);
            return;
        }

        mSendPending = true;
        //if (mIOService->getType() == IOSERVICE_IOCP_DRIVER)
        //{
        //    if (mIOService->addEvent(&mSendEvent) < 0)
        //    {
        //        LOG_WARN("Failed to add wrtie event, fd=%d.", mFd);
        //        if (mIOTimer == NULL)
        //        {
        //            mIOTimer = XNEW(IOTimer)(mIOService);
        //            mIOTimer->set(1, IOTimerCallback(&TcpConnection::onTimer), this);
        //        }
        //    }
        //}
        //else // EPOLL AND SELECT
        {
            Int32 nBytes = tcpSend((char*)mSendEvent.mData + 
                sizeof(StreamBuffer::Node) + mSendEvent.mData->mOff, 
                mSendEvent.mData->mLen);
            if (nBytes >= 0)
            {
                mSendByteCount += nBytes;
                mSendEvent.mData->mOff += nBytes;
                mSendEvent.mData->mLen -= nBytes;
                // Call onSent to continue send or would block.
                onSent(&mSendEvent, IOSERVICE_WRITE_EVENT);
            }
            else // < 0, tcpSend faild, start a timer to wait call on broken
            {
                mOStream.insertNodeAtHead(mSendEvent.mData);
                mSendEvent.mData = NULL;
                if (mIOTimer == NULL)
                {
                    mIOTimer = XNEW(IOTimer)(mIOService);
                    mIOTimer->set(1, IOTimerCallback(&TcpConnection::onTimer), this);
                }
            }
        }
    }
}
        
void 
TcpConnection::asyncSend(StreamBuffer& streamBuffer, UInt32 len)
{
    streamBuffer.read(mOStream, len);
    asyncSend();
}

void 
TcpConnection::asyncSend(const char* buffer, UInt32 len)
{
    mOStream.write((const Int8*)buffer, len);
    asyncSend();
}

bool
TcpConnection::onTimer(void* args)
{
    TcpConnection* conn = (TcpConnection*)args;

    if (conn->mCloseTimerFlag)
    {
        conn->mCloseTimerFlag = false;
        if (conn->mValid)
        {
            if (!conn->mOnConnectBroken.empty())
            {
                conn->mValid = false;
                conn->mOnConnectBroken(conn, conn->mIStream, conn->mOStream);
            }
        }
        else
        {
            if (!conn->mOnConnectFailed.empty())
            {
                conn->mOnConnectFailed(conn, conn->mIStream, conn->mOStream);
            }
        }
    }
    else
    {
        conn->cleanConnection();

        if (conn->mValid && !conn->mOnConnectBroken.empty())
        {
            conn->mValid = false;
            conn->mOnConnectBroken(conn, conn->mIStream, conn->mOStream);
        }
    }
    return false;
}

void 
TcpConnection::onConnected(IOEvent* ev, UInt32 eventType)
{
    if (eventType & IOSERVICE_WRITE_EVENT)
    {
        TcpConnection* conn = (TcpConnection *)ev->mOwner;
        if (conn == NULL)
        {
            LOG_WARN("Exception for invalid conn pointer");
            return;
        }

        // Del write event for connect request.
        conn->mIOService->delEvent(ev);

        if (ev->mErrno != LYNX_ERROR_NONE)
        {
            conn->mErrno = ev->mErrno;
            conn->cleanConnection();
            if (!conn->mOnConnectFailed.empty())
            {
                conn->mOnConnectFailed(conn, conn->mIStream, conn->mOStream);
            }
            return;
        }

        // clean istream and ostream when connection success.
        conn->mIStream.clear();
        conn->mOStream.clear();

        // Get destination socket address.
        char tmpBuf[128];
        inet_ntop(AF_INET, &conn->mDstIp, tmpBuf, 128);
        conn->mStrDstIp = tmpBuf;

        if (conn->mOnConnected.empty())
        {
            LOG_WARN("Exception for invalid on connected function pointer");
            return;
        }
        else
        {
            conn->resume();
            conn->asyncReceive();
            conn->mOnConnected(conn, conn->mIStream, conn->mOStream);
        }
    }
    else
    {
        LOG_WARN("Exception for unknown logic.");
        ASSERT(0);
    }
}

void 
TcpConnection::onReceived(IOEvent* ev, UInt32 eventType)
{
    if (eventType & IOSERVICE_READ_EVENT)
    {
        TcpConnection* conn = (TcpConnection *)ev->mOwner;
        if (conn == NULL)
        {
            LOG_WARN("Exception for invalid conn pointer");
            return;
        }

        conn->mRecvPending = false;
        //conn->mRecvTimeStamp = TimeUtil::GetTimeSec();

        if (ev->mErrno != LYNX_ERROR_NONE)
        {
            conn->mErrno = ev->mErrno;
            conn->cleanConnection();
            //if (conn->mIOService->getType() == IOSERVICE_IOCP_DRIVER)
            //{
            //    if (conn->mSendPending == true)
            //    {
            //        return;
            //    }
            //}

            if (conn->mValid && !conn->mOnConnectBroken.empty())
            {
                conn->mValid = false;
                if (conn->mIOTimer)
                {
                    conn->mIOTimer->cancel();
                }
                conn->mOnConnectBroken(conn, conn->mIStream, conn->mOStream);
            }
            return;
        }

        //if (conn->mIOService->getType() == IOSERVICE_IOCP_DRIVER)
        //{
        //    conn->mIStream.pushNode(ev->mData);
        //    ev->mData = NULL;
        //    conn->asyncReceive();
        //    if (!conn->mOnReceived.empty())
        //    {
        //        conn->mOnReceived(conn, conn->mIStream, conn->mOStream);
        //    }
        //}
        //else // EPOOL AND SELECT
        {
            if (conn->mReadUtilNull) // read until tcp buffer is null when read event arrived.
            {
                while (1)
                {
                    ev->mData = conn->mIStream.allocNode();
                    Int32 nBytes = conn->tcpRecv((char *)ev->mData + sizeof(StreamBuffer::Node),
                        conn->mIStream.mNodeDataSize);
                    if (nBytes > 0) 
                    {
                        conn->mRecvByteCount += nBytes;
                        ev->mData->mLen = nBytes;
                        conn->mIStream.pushNode(ev->mData);
                        ev->mData = NULL;
                    }
                    else if (nBytes < 0)
                    {
                        conn->mIStream.deallocNode(ev->mData);
                        ev->mData = NULL;
                        conn->cleanConnection();
                        if (conn->mValid && !conn->mOnConnectBroken.empty())
                        {
                            conn->mValid = false;
                            if (conn->mIOTimer)
                            {
                                conn->mIOTimer->cancel();
                            }
                            conn->mOnConnectBroken(conn, conn->mIStream, conn->mOStream);
                        }
                        break;
                    }
                    else // == 0, would block, tcp receive buffer is null. callback application.
                    {
                        conn->mIStream.deallocNode(ev->mData);
                        ev->mData = NULL;
                        if (!conn->mOnReceived.empty())
                        {
                            conn->mOnReceived(conn, conn->mIStream, conn->mOStream);
                        }
                        break;
                    }
                }
            }
            else // read one time when read event arrived.
            {
                ev->mData = conn->mIStream.allocNode();
                Int32 nBytes = conn->tcpRecv((char *)ev->mData + sizeof(StreamBuffer::Node),
                    conn->mIStream.mNodeDataSize);
                if (nBytes > 0) 
                {
                    conn->mRecvByteCount += nBytes;
                    ev->mData->mLen = nBytes;
                    conn->mIStream.pushNode(ev->mData);
                    ev->mData = NULL;
                    if (!conn->mOnReceived.empty())
                    {
                        conn->mOnReceived(conn, conn->mIStream, conn->mOStream);
                    }
                }
                else if (nBytes < 0)
                {
                    conn->mIStream.deallocNode(ev->mData);
                    ev->mData = NULL;
                    conn->cleanConnection();
                    if (conn->mValid && !conn->mOnConnectBroken.empty())
                    {
                        conn->mValid = false;
                        if (conn->mIOTimer)
                        {
                            conn->mIOTimer->cancel();
                        }
                        conn->mOnConnectBroken(conn, conn->mIStream, conn->mOStream);
                    }
                }
                else // == 0, would block, tcp receive buffer is null. callback application.
                {
                    conn->mIStream.deallocNode(ev->mData);
                    ev->mData = NULL;
                    if (!conn->mOnReceived.empty())
                    {
                        conn->mOnReceived(conn, conn->mIStream, conn->mOStream);
                    }
                }
            }
        }
    }
    else
    {
        LOG_WARN("Exception for unknown logic.");
        ASSERT(0);
    }
}

void 
TcpConnection::onSent(IOEvent* ev, UInt32 eventType)
{
    if (eventType & IOSERVICE_WRITE_EVENT)
    {
        TcpConnection* conn = (TcpConnection *)ev->mOwner;
        if (conn == NULL)
        {
            LOG_WARN("Exception for invalid conn pointer");
            return;
        }

        conn->mIOService->delEvent(ev);

        conn->mSendPending = false;
        //conn->mSendTimeStamp = TimeUtil::GetTimeSec();

        if (ev->mErrno != LYNX_ERROR_NONE)
        {
            conn->mErrno = ev->mErrno;
            conn->cleanConnection();

            //if (conn->mIOService->getType() == IOSERVICE_IOCP_DRIVER)
            //{
            //    if (conn->mRecvPending == true)
            //    {
            //        return;
            //    }
            //}

            if (conn->mValid && !conn->mOnConnectBroken.empty())
            {
                conn->mValid = false;
                if (conn->mIOTimer)
                {
                    conn->mIOTimer->cancel();
                }
                conn->mOnConnectBroken(conn, conn->mIStream, conn->mOStream);
            }
            return;
        }

        //if (conn->mIOService->getType() == IOSERVICE_IOCP_DRIVER)
        //{
        //    if (ev->mData == NULL)
        //    {
        //        conn->asyncSend();
        //        return;
        //    }
        //
        //    if (ev->mData->mLen == 0)
        //    {
        //        conn->mOStream.deallocNode(ev->mData);
        //        ev->mData = NULL;
        //        conn->asyncSend();
        //    }
        //    else // would block
        //    {
        //        conn->mOStream.insertNodeAtHead(ev->mData);
        //        ev->mData = NULL;
        //        conn->asyncSend();
        //    }
        //}
        //else // EPOOLL AND SELECT
        {
            if (ev->mData == NULL)
            {
                conn->asyncSend();
                return;
            }

            if (ev->mData->mLen == 0)
            {
                conn->mOStream.deallocNode(ev->mData);
                ev->mData = NULL;
                conn->asyncSend();
            }
            else // Would block, tcp send buffer has full. so we add a writeable event to wait. 
            {
                conn->mSendPending = true;
                conn->mOStream.insertNodeAtHead(ev->mData);
                ev->mData = NULL;
                if (conn->mIOService->addEvent(ev) < 0)
                {
                    LOG_WARN("Failed to add write event.");
                    if (conn->mIOTimer == NULL)
                    {
                        conn->mIOTimer = XNEW(IOTimer)(conn->mIOService);
                        conn->mIOTimer->set(1, IOTimerCallback(&TcpConnection::onTimer), conn);
                    }
                    return;
                }
            }
        }
    }
    else
    {
        LOG_WARN("Exception for unknown logic.");
        ASSERT(0);
    }
}





