#include "Socket.h"
#include "IOService.h"
#include "Logger.h"

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#ifdef _WIN32
# include <winsock2.h>
# include <ws2tcpip.h>
#else
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netinet/in.h>
#endif

#ifdef _WIN32
#define SIO_KEEPALIVE_VALS _WSAIOW(IOC_VENDOR, 4)

static const char*
inet_ntop4(const unsigned char* src, char* dst, size_t size)
{
        static const char fmt[] = "%u.%u.%u.%u";
        char tmp[sizeof "255.255.255.255"];

        if (snprintf(tmp, sizeof tmp, fmt, 
                src[0], src[1], src[2], src[3]) >= (int)size) 
        {
                return NULL;
        }

        return strcpy(dst, tmp);
}

const char *
inet_ntop(int af, void const* src, char* dst, size_t size)
{
        switch (af) 
        {
        case AF_INET:
                return inet_ntop4((const unsigned char*)src, dst, size);
        default:
                return NULL;
        }
}

#endif

using namespace Lynx;

Socket::Socket(TransportType transportType) : mFd(-1), mTransportType(transportType), mErrno(0)
, mSrcIp(0), mDstIp(0), mSrcPort(0), mDstPort(0), mUserData(0)
{
}

Socket::~Socket()
{

}

Int32 
Socket::open(bool isBlocking)
{
    return createSocket(mTransportType, isBlocking);
}

void 
Socket::close()
{
    closeSocket();
}

Int32 
Socket::createSocket(TransportType transportType, bool isBlocking)
{
    Int32 fd = -1;
    if (transportType == TRANSPORT_TCP)
    {
        fd = (Int32)socket(AF_INET, SOCK_STREAM, 0);
    }
    else 
    {
        fd = (Int32)socket(AF_INET, SOCK_DGRAM, 0);
    }

    mTransportType = transportType;

    if (fd == -1)
    {
        LOG_WARN("Failed to create socket, errno: %d", ERRNUM());
        return -1;
    }

    mFd = fd;
    if (!isBlocking)
    {
        if (setSocketNonblocking() < 0)
        {
            LOG_WARN("Failed to set socket to nonblocking, errno: %d", ERRNUM());
            closesocket(fd);
            return -1;
        }
    }
    return fd;
}

Int32 
Socket::closeSocket()
{
    if (mFd > 0)
    {
        closesocket(mFd);
        mFd = -1;
    }
    return 0;
}

Int32 
Socket::bindSocket(int isReuse)
{
    struct sockaddr_in addr;
    
    if (isReuse)
    {
        if (setsockopt(mFd, SOL_SOCKET, SO_REUSEADDR, (char*)&isReuse, sizeof(isReuse)) < 0)
        {
            LOG_WARN("Failed to set socket option. errno: %d", ERRNUM());
            return -1;
        }
    }

    addr.sin_family = AF_INET;
    if (mSrcIp == 0)
    {
        addr.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        addr.sin_addr.s_addr = mSrcIp;
    }
    addr.sin_port = htons(mSrcPort);

    if (bind(mFd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        LOG_WARN("Failed to bind socket addr fd: %d errno: %d", mFd, ERRNUM());
        return -1;
    }
    return 0;
}

Int32 
Socket::bindSocket(const char* ip, UInt16 port, int isReuse)
{
    struct sockaddr_in addr;

    ASSERT(ip);
    ASSERT(port != 0);
    
    if (isReuse)
    {
        if (setsockopt(mFd, SOL_SOCKET, SO_REUSEADDR, (char*)&isReuse, sizeof(isReuse)) < 0)
        {
            LOG_WARN("Failed to set socket option. errno: %d", ERRNUM());
            return -1;
        }
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    if (bind(mFd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        LOG_WARN("Failed to bind socket addr fd: %d errno: %d", mFd, ERRNUM());
        return -1;
    }
    return 0;
}

Int32 
Socket::setSocketBlocking()
{
#ifdef _WIN32
    Int32 mode = 0;
    if (ioctlsocket(mFd, FIONBIO, (u_long FAR*)&mode) != 0)
    {
        return -1;
    }
#else
    Int32 flags = fcntl(mFd, F_GETFL, 0);
    if (fcntl(mFd, F_SETFL, flags & ~O_NONBLOCK) != 0)
    {
        return -1;
    }
#endif
    return 0;
}

Int32 
Socket::setSocketNonblocking()
{
#ifdef _WIN32
    Int32 mode = 1;
    if (ioctlsocket(mFd, FIONBIO, (u_long FAR*)&mode) != 0)
    {
        return -1;
    }
#else
    Int32 flags = fcntl(mFd, F_GETFL, 0);
    if (fcntl(mFd, F_SETFL, flags | O_NONBLOCK) != 0)
    {
        return -1;
    }
#endif
    return 0;
}

Int32 
Socket::setTcpKeepalive(Int32 keepalive, Int32 keepidle, Int32 keepintvl, Int32 keepcnt)
{
#ifdef _WIN32
    struct {
        unsigned long onoff;
        unsigned long keepalivetime;
        unsigned long keepaliveinterval;
    } inKeepalive, outKeepalive;
    unsigned long inLen, outLen, returnBytes;
#endif

    if (mFd <= 0)
    {
        LOG_WARN("Invalid file descriper.");
        return -1;
    }

    if (keepalive == 0) // turn off keepalive
    {
        return 0;
    }

    if (keepidle <= 0)
    {
        LOG_WARN("Invalid idle value: %d", keepidle);
        return -1;
    }

    if (keepintvl <= 0)
    {
        LOG_WARN("Invalid interval value: %d", keepintvl);
        return -1;
    }

    if (keepcnt <= 0)
    {
        LOG_WARN("Invalid count value: %d", keepcnt);
        return -1;
    }

#ifdef _WIN32
    inLen = sizeof(inKeepalive);
    outLen = sizeof(outKeepalive);
    returnBytes = 0;
    memset(&inKeepalive, 0, sizeof(inKeepalive));
    memset(&outKeepalive, 0, sizeof(outKeepalive));
    inKeepalive.onoff = keepalive;
    inKeepalive.keepaliveinterval = keepintvl;
    inKeepalive.keepalivetime = keepidle;
    // Keepcnt default is 3 times on windows system.
    if (WSAIoctl((unsigned int)mFd, SIO_KEEPALIVE_VALS, (LPVOID)&inKeepalive, inLen,
        (LPVOID)&outKeepalive, outLen, &returnBytes, NULL, NULL) == -1)
    {
        LOG_WARN("Failed to set tcp keepalive failed, errno: %d", ERRNUM());
        return -1;
    }
#elif defined (__linux__)
    if (setsockopt(mFd, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepalive, sizeof(keepalive)) < 0)
    {
        LOG_WARN("Failed to set tcp keepalive, errno: %d", ERRNUM());
        return -1;
    }

    if (setsockopt(mFd, SOL_TCP, TCP_KEEPIDLE, (char*)&keepidle, sizeof(keepidle)) < 0)
    {
        LOG_WARN("Failed to set keepidle, errno: %d", ERRNUM());
        return -1;
    }

    if (setsockopt(mFd, SOL_TCP, TCP_KEEPINTVL, (char*)&keepintvl, sizeof(keepintvl)) < 0)
    {
        LOG_WARN("Failed to set keepinvtal, errno: %d", ERRNUM());
        return -1;
    }

    if (setsockopt(mFd, SOL_TCP, TCP_KEEPCNT, (char*)&keepcnt, sizeof(keepcnt)) < 0)
    {
        LOG_WARN("Failed to set keepcnt, errno: %d", ERRNUM());
        return -1;
    }
#endif
    return 0;
}

Int32 
Socket::tcpListen(UInt32 backlog)
{
    return listen(mFd, backlog);
}

Int32 
Socket::tcpAccept(UInt32& dstIp, UInt16& dstPort, bool isBlocking)
{
    if (mTransportType == TRANSPORT_UDP)
    {
        LOG_WARN("Transport type don't math");
        return -1;
    }

    Int32 connFd, len;
    struct sockaddr_in addr;
    len = sizeof(struct sockaddr_in);

    connFd = (Int32)accept(mFd, (struct sockaddr*)&addr, (socklen_t*)&len);
    if (connFd < 0)
    {
#ifdef _WIN32
        if (!(ERRNUM() == EWOULDBLOCK_ || ERRNUM() == WSAEINPROGRESS || ERRNUM() == WSAEINTR))
#else
        if (!(ERRNUM() == EWOULDBLOCK_ || ERRNUM() == EINTR))
#endif
        {
            LOG_WARN("Failed to accept tcp connection fd: %d errno: %d", mFd, ERRNUM());
            return -1;
        }
        return 0;
    }

    dstIp = addr.sin_addr.s_addr;
    dstPort = addr.sin_port;
    return connFd;
}

Int32 
Socket::tcpConnect(const char* dstIp, UInt16 dstPort)
{
    if (mTransportType == TRANSPORT_UDP)
    {
        LOG_WARN("Transport type don't math");
        return -1;
    }

    Int32 ret;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(dstIp);
    addr.sin_port = htons(dstPort);

    mDstIp = addr.sin_addr.s_addr;

    ret = connect(mFd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0)
    {
#ifdef _WIN32
        if (!(ERRNUM() == WSAEWOULDBLOCK || ERRNUM() == WSAEINPROGRESS))
#else
        if (ERRNUM() != EINPROGRESS)
#endif
        {
            LOG_WARN("Failed to connection tcp fd: %d errno: %d", mFd, ERRNUM());
            return -1;
        }
        return 0;
    }

    return mFd;
}

Int32 
Socket::tcpRecv(char* buffer, UInt32 len)
{
    if (mTransportType == TRANSPORT_UDP)
    {
        LOG_WARN("Transport type don't math");
        return -1;
    }

    Int32 nBytes;

#ifdef _WIN32
    nBytes = recv(mFd, buffer, len, 0);
#else
    nBytes = read(mFd, buffer, len);
#endif

    if (nBytes < 0)
    {
        if (ERRNUM() == EWOULDBLOCK_)
        {
            return 0;
        }
        LOG_WARN("Tcp read failed fd: %d, errno: %d", mFd, ERRNUM());
        return -1;
    }
    else if (nBytes == 0)
    {
        LOG_DEBUG("Tcp connection was close by peer, fd: %d", mFd);
        return -1;
    }
    return nBytes;
}

Int32 
Socket::tcpSend(const char* buffer, UInt32 len)
{
    if (mTransportType == TRANSPORT_UDP)
    {
        LOG_WARN("Transport type don't math");
        return -1;
    }

    Int32 nBytes;

#ifdef _WIN32
    nBytes = send(mFd, buffer, len, 0);
	/*
	char test[sizeof(nBytes)+1] = {0};
	memcpy(test, buffer, sizeof(nBytes));
	printf("------- len: %ld  \n", nBytes);
	int j = 0;
	for(int i=0; i<nBytes; i++)
	{
		if(j>=4)
		{
			printf("\n");
			j=1;
		}
		else
			j++;
		printf(" %x ",test[i]);
	}*/
	//printf(" \n");
#else
    nBytes = write(mFd, buffer, len);
#endif

    if (nBytes >= 0)
    {
        return nBytes;
    }

    if (ERRNUM() == EWOULDBLOCK_)
    {
        return 0;
    }
    LOG_WARN("Tcp write failed fd: %d, errno: %d", mFd, ERRNUM());
    return -1;
}

Int32 
Socket::udpConnect(const char* dstIp, UInt16 dstPort)
{
    if (mTransportType == TRANSPORT_TCP)
    {
        LOG_WARN("Transport type don't math");
        return -1;
    }

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(dstIp);
    addr.sin_port = htons(dstPort);

    if (connect(mFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        LOG_WARN("Udp connect failed, errno: %d", ERRNUM());
        return -1;
    }

    return 0;
}

Int32 
Socket::udpRecv(char* buffer, UInt32 len)
{
    if (mTransportType == TRANSPORT_TCP)
    {
        LOG_WARN("Transport type don't math");
        return -1;
    }

    return 0;
}

Int32 
Socket::udpSend(const char* buffer, UInt32 len)
{
    if (mTransportType == TRANSPORT_TCP)
    {
        LOG_WARN("Transport type don't math");
        return -1;
    }

    return 0;
}

Int32 
Socket::udpSend(UInt32 dstIp, UInt16 dstPort, const char* buffer, UInt32 len)
{
    if (mTransportType == TRANSPORT_TCP)
    {
        LOG_WARN("Transport type don't math");
        return -1;
    }

    struct sockaddr_in remoteAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);

    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_addr.s_addr = dstIp;
    remoteAddr.sin_port = dstPort;

    return sendto(mFd, buffer, len, 0, (struct sockaddr*)&remoteAddr, addrLen);
}

Int32 
Socket::udpRecv(char* buffer, UInt32 len, UInt32& fromIp, UInt16& fromPort, UInt32 ms)
{
    fd_set readFds;
    struct sockaddr_in fromAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);

    FD_ZERO(&readFds);
    FD_SET(mFd, &readFds);

    struct timeval tv;
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;

    Int32 ret = select(mFd + 1, &readFds, NULL, NULL, &tv);
    if (ret < 0)
    {
        LOG_WARN("Failed to select udp fd %d", mFd);
        return ret;
    }

    if (FD_ISSET(mFd, &readFds))
    {
        ret = recvfrom(mFd, buffer, len, 0, (struct sockaddr*)&fromAddr, &addrLen);
        fromIp = fromAddr.sin_addr.s_addr;
        fromPort = fromAddr.sin_port;
        return ret;
    }   
    return 0;
}
