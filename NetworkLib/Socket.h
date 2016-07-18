#ifndef __LYNX_CORE_LIB_SOCKET_H__
#define __LYNX_CORE_LIB_SOCKET_H__

#include "MemoryManager.h"
#include "ErrorCode.h"

#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
#endif

const char* inet_ntop(int af, void const *src, char *dst, size_t size);

#ifdef __cplusplus
}
#endif
#endif

namespace Lynx
{
    class Socket
    {
    public:
        enum TransportType { TRANSPORT_TCP = 1, TRANSPORT_UDP };

        Socket(TransportType transportType);
        virtual ~Socket();

        friend class IOService;

        virtual Int32 open(bool isBlocking = false);
        virtual void  close();

    public:
        Int32 createSocket(TransportType transportType, bool isBlocking);
        Int32 closeSocket();
        Int32 bindSocket(int isReuse);
        Int32 bindSocket(const char* ip, UInt16 port, int isReuse);
        Int32 setSocketBlocking();
        Int32 setSocketNonblocking();
        Int32 tcpListen(UInt32 backlog = 200);
        Int32 tcpAccept(UInt32& dstIp, UInt16& dstPort, bool isBlocking = false);
        Int32 tcpConnect(const char* dstIp, UInt16 dstPort);
        Int32 setTcpKeepalive(Int32 keepalive, Int32 keepidle, Int32 keepintvl, Int32 keepcnt);
        Int32 tcpRecv(char* buffer, UInt32 len);
        Int32 tcpSend(const char* buffer, UInt32 len);
        Int32 udpConnect(const char* dstIp, UInt16 dstPort);
        Int32 udpRecv(char* buffer, UInt32 len);
        Int32 udpSend(const char* buffer, UInt32 len);
        Int32 udpSend(UInt32 dstIp, UInt16 dstPort, const char* buffer, UInt32 len);
        Int32 udpRecv(char* buffer, UInt32 len, UInt32& fromIp, UInt16& fromPort, UInt32 ms);

    public:
        void*               mUserData;
        UInt32              mErrno;
    protected:
        Int32               mFd;
        TransportType       mTransportType;
        UInt32              mSrcIp;
        String              mStrSrcIp;
        UInt32              mDstIp;
        String              mStrDstIp;
        UInt16              mSrcPort;
        UInt16              mDstPort;
    };
} // namespace Lynx

#endif // __LYNX_CORE_LIB_SOCKET_H__


