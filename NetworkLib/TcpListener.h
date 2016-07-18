#ifndef __LYNX_CORE_LIB_TCP_LISTENER_H__
#define __LYNX_CORE_LIB_TCP_LISTENER_H__

#include "TcpConnection.h"

namespace Lynx
{
    class TcpListener;
    typedef Delegate<void (TcpConnection*, StreamBuffer&, StreamBuffer&)> AcceptCallback;

    class TcpListener : public Socket
    {
    public:
        TcpListener(IOService* ioService, const String& srcIp, UInt16 srcPort);
        virtual ~TcpListener();

        friend class IOTcpClient;

        virtual void close();

        IOService* getIOService() { return mIOService; }
        void asyncAccept(TcpConnection* conn);
        Int32 getLastError() { return mErrno; }
        String& getBindIp() { return mStrSrcIp; }
        UInt16 getBindPort() { return mSrcPort; }

        AcceptCallback  mOnAccepted;
        AcceptCallback  mOnAcceptFailed;

        StreamBuffer    mIStream;
        IOEvent         mRecvEvent;
    private:
        static void onAccepted(IOEvent* ev, UInt32 eventType);

    private:
        IOService* mIOService;
    };
} // namespace Lynx

#define LYNX_REGISTER_ACCEPTED(l, o, f) (l)->mOnAccepted += std::make_pair(o, f)
#define LYNX_DEREGISTER_ACCEPTED(l, o, f) (l)->mOnAccepted -= std::make_pair(o, f)

#endif // __LYNX_CORE_LIB_TCP_LISTENER_H__



