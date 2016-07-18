#ifndef __LYNX_CORE_LIB_TCP_CONNECTION_H__
#define __LYNX_CORE_LIB_TCP_CONNECTION_H__

#include "Socket.h"
#include "StreamBuffer.h"
#include "Delegate.h"
#include "IOService.h"

#define DEFAULT_MAX_MSG_LEN  1024 * 1024 * 128

namespace Lynx
{
    class IOTimer;
    class TcpConnection;
    class TcpListener;
    typedef Delegate<void (TcpConnection*, StreamBuffer&, StreamBuffer&)> TcpConnCallback;
    typedef Delegate<void (TcpConnection*, StreamBuffer&, StreamBuffer&)> TcpRecvCallback;

    class TcpConnection : public Socket
    {
    public:
        TcpConnection(IOService* ioService, UInt32 istreamBufferLen = 1024, UInt32 ostreamBufferLen = 1024,
			          UInt32 maxInMsgLen = DEFAULT_MAX_MSG_LEN, UInt32 maxOutMsgLen = DEFAULT_MAX_MSG_LEN);
        virtual ~TcpConnection();

        friend class TcpListener;

        virtual void close();

        IOService* getIOService() { return mIOService; }
        void setService(IOService* ioService) { mIOService = ioService; }

        // pause and resume only support EPOLL AND SELECT.
        void pause();
        void resume();
        void clean();

        void setReadUtilNull(bool readUtilNull) { mReadUtilNull = readUtilNull; }
        void resetSendByteCount() { mSendByteCount = 0; }
        void resetRecvByteCount() { mRecvByteCount = 0; }
        UInt32 getSendByteCount() const { return mSendByteCount; }
        UInt32 getRecvByteCount() const { return mRecvByteCount; }

        Int32 getLastError() { return mErrno; }
        String& getFromIp() { return mStrDstIp; }
        UInt16 getFromPort() { return mDstPort; }
        UInt32 getRecvTimeStamp() { return mRecvTimeStamp; }
        UInt32 getSendTimeStamp() { return mSendTimeStamp; }

        void asyncConnect(const String& ip, UInt16 port);
        void asyncReceive();
        void asyncSend();
        void asyncSend(StreamBuffer& streamBuffer, UInt32 len);
        void asyncSend(const char* buffer, UInt32 len);

        inline UInt32 read(char* buffer, UInt32 len)
        {
            return mIStream.read(buffer, len);
        }

        TcpConnCallback mOnConnected;
        TcpConnCallback mOnConnectFailed;
        TcpConnCallback mOnConnectBroken;
        TcpRecvCallback mOnReceived;

        StreamBuffer mIStream;
        StreamBuffer mOStream;

        IOEvent      mRecvEvent;
        IOEvent      mSendEvent;

		UInt64       mConnId;
		UInt32       mMaxInMsgLen;
        UInt32       mMaxOutMsgLen;
        UInt32       mRecvTimeStamp;
        UInt32       mSendTimeStamp;
		bool         mMsgPending;

    private:
        static void onConnected(IOEvent* ev, UInt32 eventType);
        static void onReceived(IOEvent* ev, UInt32 eventType);
        static void onSent(IOEvent* ev, UInt32 eventType);
        static bool onTimer(void* args);

        void cleanConnection();
    private:
        IOService*     mIOService;
        TcpListener*   mIOTcpListener;
        IOTimer*       mIOTimer;
        bool           mSendPending;
        bool           mRecvPending;
        bool           mValid;
        bool           mReadUtilNull;
        bool           mCloseTimerFlag;
        UInt32         mSendByteCount;
        UInt32         mRecvByteCount;
    };
} // namespace Lynx

#define LYNX_REGISTER_CONNECTED(c, o, f) (c)->mOnConnected += std::make_pair(o, f)
#define LYNX_DEREGISTER_CONNECTED(c, o, f) (c)->mOnConnected -= std::make_pair(o, f)
#define LYNX_REGISTER_CONNECT_FAILED(c, o, f) (c)->mOnConnectFailed += std::make_pair(o, f)
#define LYNX_DEREGISTER_CONNECT_FAILED(c, o, f) (c)->mOnConnectFailed -= std::make_pair(o, f)
#define LYNX_REGISTER_CONNECT_BROKEN(c, o, f) (c)->mOnConnectBroken += std::make_pair(o, f)
#define LYNX_DEREGISTER_CONNECT_BROKEN(c, o, f) (c)->mOnConnectBroken -= std::make_pair(o, f)
#define LYNX_REGISTER_RECEIVED(c, o, f) (c)->mOnReceived += std::make_pair(o, f)
#define LYNX_DEREGISTER_RECEIVED(c, o, f) (c)->mOnReceived -= std::make_pair(o, f)

#endif // __LYNX_CORE_LIB_TCP_CONNECTION_H__

