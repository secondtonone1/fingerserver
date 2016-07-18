#ifdef _WIN32

//#include "IOService.h"
//#include "Logger.h"
//#include "Container.h"
//#include "Errno.h"
//
//#include <winsock2.h>
//#include <windows.h>
//#include <mswsock.h>
//
//using namespace Lynx;
//
//struct IocpOperation
//{
//    IocpOperation() 
//    {
//        mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
//    }
//
//    ~IocpOperation()
//    {
//        if (mIocpHandle != NULL)
//        {
//            CloseHandle(mIocpHandle);
//        }
//    }
//
//    HANDLE                     mIocpHandle;
//    LPFN_ACCEPTEX              mAcceptEx;
//    LPFN_GETACCEPTEXSOCKADDRS  mGetAcceptExSockAddrs;
//    LPFN_CONNECTEX             mConnectEx;
//};
//
//static void*  iocpInitial(IOService* ioService);
//static void   iocpRelease(void* op);
//static Int32  iocpAddEvent(IOEvent* ev, void* op);
//static Int32  iocpDelEvent(IOEvent* ev, void* op);
//static Int32  iocpDispatch(IOService* ioService, void* op, Timeval* tv);
//
//IOEventOperator iocpOperator = 
//{
//    IOSERVICE_IOCP_DRIVER,
//    "IOCP",
//    iocpInitial,
//    iocpRelease,
//    iocpAddEvent,
//    iocpDelEvent,
//    iocpDispatch
//};
//
//void*  
//iocpInitial(IOService* ioService)
//{
//    WSADATA wsaData;
//    WORD version = MAKEWORD(2, 2);
//    WSAStartup(version, &wsaData);
//
//    IocpOperation* operation = XNEW(IocpOperation)();
//    ASSERT(operation);
//
//    GUID guidAcceptEx = WSAID_ACCEPTEX;
//    GUID guidConnectEx = WSAID_CONNECTEX;
//    GUID getAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
//    
//    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
//    if (s == -1)
//    {
//        return NULL;
//    }
//
//    DWORD nBytes;
//    WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER,
//        &guidAcceptEx, sizeof(guidAcceptEx),
//        &operation->mAcceptEx, sizeof(operation->mAcceptEx), 
//        &nBytes, 0, 0);
//
//    WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER,
//        &guidConnectEx, sizeof(guidConnectEx),
//        &operation->mConnectEx, sizeof(operation->mConnectEx), 
//        &nBytes, 0, 0);
//
//    WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER,
//        &getAcceptExSockaddrs, sizeof(getAcceptExSockaddrs),
//        &operation->mGetAcceptExSockAddrs, sizeof(operation->mGetAcceptExSockAddrs), 
//        &nBytes, 0, 0);
//
//    closesocket(s);
//    return operation;
//}
//
//void   
//iocpRelease(void* op)
//{
//    IocpOperation* operation = (IocpOperation*)op;
//    if (operation == NULL)
//    {
//        LOG_WARN("Invalid IocpOperation pointer");
//        return;
//    }
//    XDELETE(operation);
//}
//
//Int32  
//iocpAddEvent(IOEvent* ev, void* op)
//{
//    if (ev == NULL)
//    {
//        LOG_WARN("Invalid IOEvent pointer");
//        return -1;
//    }
//
//    IocpOperation* operation = (IocpOperation*)op;
//    if (operation == NULL)
//    {
//        LOG_WARN("Invalid IocpOperation pointer");
//        return -1;
//    }
//
//    if (ev->mPostListen)
//    {
//        CreateIoCompletionPort((HANDLE)(SOCKET)ev->mFd, operation->mIocpHandle, 0, 0);
//        ev->mErrno = LYNX_ERROR_NONE;
//        ev->mCallback(ev, IOSERVICE_READ_EVENT);
//        return 0;
//    }
//
//    if (ev->mEventType & IOSERVICE_READ_EVENT)
//    {
//        if (ev->mPostAccept)
//        {
//            ev->mFd = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
//            if (ev->mFd < 0)
//            {
//                LOG_WARN("Failed to call WSASocket.");
//                return -1;
//            }
//
//            Int32 mode = 1;
//            if (ioctlsocket(ev->mFd, FIONBIO, (u_long FAR*)&mode) != 0)
//            {
//                LOG_WARN("Failed to set socket nonblocking.");
//                return -1;
//            }
//
//            DWORD nBytes;
//            BOOL result = operation->mAcceptEx(
//                ev->mListenFd, 
//                ev->mFd, 
//                (char*)ev->mData + sizeof(StreamBuffer::Node) + ev->mData->mOff,
//                0,
//                sizeof(SOCKADDR_IN) + 16,
//                sizeof(SOCKADDR_IN) + 16, 
//                &nBytes, 
//                (OVERLAPPED*)&ev->mOverlapped);
//
//            if (result != NO_ERROR)
//            {
//                Int32 errCode = WSAGetLastError();
//                if (errCode != WSA_IO_PENDING)
//                {
//                    LOG_WARN("Failed to call mAcceptEx, error: %d", errCode);
//                    return -1;
//                }
//            }    
//        }
//        else
//        {
//            DWORD nBytes, flags = 0;
//            WSABUF wsaBuf;
//            wsaBuf.buf = (char*)ev->mData + sizeof(StreamBuffer::Node) + ev->mData->mOff;
//            wsaBuf.len = ev->mDataSize - ev->mData->mOff;
//
//            Int32 result = WSARecv(ev->mFd, 
//                &wsaBuf, 
//                1, 
//                &nBytes, 
//                &flags, 
//                (OVERLAPPED*)&ev->mOverlapped, 
//                NULL);
//
//            if (result != NO_ERROR)
//            {
//                Int32 errCode = WSAGetLastError();
//                if (errCode != WSA_IO_PENDING)
//                {
//                    LOG_WARN("Failed to call WSARecv, error: %d", errCode);
//                    return -1;
//                }
//            }    
//        }
//    }
//    else if (ev->mEventType & IOSERVICE_WRITE_EVENT)
//    {
//        if (ev->mPostConnect)
//        {
//            SOCKADDR_IN srcAddr;
//            srcAddr.sin_family = AF_INET;
//            srcAddr.sin_port = 0;
//            srcAddr.sin_addr.S_un.S_addr = INADDR_ANY;
//
//            if (bind(ev->mFd, (LPSOCKADDR)&srcAddr, sizeof(srcAddr)) < 0)
//            {
//                LOG_WARN("Failed to call bind.");
//                return -1;
//            }
//
//            CreateIoCompletionPort((HANDLE)(SOCKET)ev->mFd, operation->mIocpHandle, 0, 0);
//
//            // set SO_SNDBUF to zero for zero-copy network stack (we maintain the buffers)
//            int bufSize = 0;
//            if (setsockopt((SOCKET)ev->mFd, SOL_SOCKET, SO_SNDBUF, (char*)&bufSize, sizeof(bufSize)))
//            {
//                LOG_WARN("Failed to setsocketopt for SO_SNDBUF to zero-copy");
//            }
//
//            SOCKADDR_IN dstAddr;
//            dstAddr.sin_family = AF_INET;
//            dstAddr.sin_addr.s_addr = ev->mDstIp;
//            dstAddr.sin_port = htons(ev->mDstPort);
//
//            BOOL result = operation->mConnectEx((SOCKET)ev->mFd, 
//                (LPSOCKADDR)&dstAddr, 
//                sizeof(dstAddr),
//                NULL, 
//                0, 
//                NULL, (
//                OVERLAPPED*)&ev->mOverlapped);
//
//            if (result != NO_ERROR)
//            {
//                Int32 errCode = WSAGetLastError();
//                if (errCode != WSA_IO_PENDING)
//                {
//                    LOG_WARN("Failed to call mConnectEx, error: %d", errCode);
//                    return -1;
//                }
//            }    
//        }
//        else
//        {
//            DWORD nBytes, flags = 0;
//            WSABUF wsaBuf;
//            wsaBuf.buf = (char*)ev->mData + sizeof(StreamBuffer::Node) + ev->mData->mOff;
//            wsaBuf.len = ev->mData->mLen;
//
//            Int32 result = WSASend(ev->mFd, 
//                &wsaBuf, 
//                1, 
//                &nBytes, 
//                flags, 
//                (OVERLAPPED*)&ev->mOverlapped, 
//                NULL);
//
//            if (result != NO_ERROR)
//            {
//                Int32 errCode = WSAGetLastError();
//                if (errCode != WSA_IO_PENDING)
//                {
//                    LOG_WARN("Failed to call WSASendEx, error: %d", errCode);
//                    return -1;
//                }
//            }    
//        }
//    }
//    else
//    {
//        LOG_WARN("Unknown event type %d\n", ev->mEventType);
//        return -1;
//    }
//
//    return 0;
//}
//
//Int32  
//iocpDelEvent(IOEvent* ev, void* op)
//{
//    if (ev == NULL)
//    {
//        LOG_WARN("Invalid IOEvent pointer");
//        return -1;
//    }
//
//    IocpOperation* operation = (IocpOperation*)op;
//    if (operation == NULL)
//    {
//        LOG_WARN("Invalid IocpOperation pointer");
//        return -1;
//    }
//
//    //ev->mOverlapped.hEvent = (HANDLE)((DWORD)ev->mOverlapped.hEvent | 0x1);
//    // Nothing to do.
//
//    return 0;
//}
//
//Int32  
//iocpDispatch(IOService* ioService, void* op, Timeval* tv)
//{
//    if (ioService == NULL)
//    {
//        LOG_WARN("Invalid IOService pointer");
//        return -1;
//    }
//
//    IocpOperation* operation = (IocpOperation*)op;
//    if (operation == NULL)
//    {
//        LOG_WARN("Invalid IocpOperation pointer");
//        return -1;
//    }
//
//    Int32 timeout = 0;
//
//    if (tv != NULL)
//    {
//        timeout = tv->tv_sec * 1000 + (tv->tv_usec) / 1000;
//    }
//
//    //if (timeout == 0) timeout = 10;
//
//    DWORD key, nBytes;
//    LPOVERLAPPED ol;
//    
//    BOOL result = GetQueuedCompletionStatus(operation->mIocpHandle,
//        &nBytes, (LPDWORD)&key, (LPOVERLAPPED*)&ol, timeout);
//    if (result == -1)
//    {
//        LOG_WARN("Failed to call GetQueuedCompletionStatus.");
//        return -1;
//    }
//
//    if (result == 0 && ol == 0) // timeout
//    {
//        return 0;
//    }
//    
//    IOEvent* ev = CONTAINING_RECORD(ol, IOEvent, mOverlapped);
//
//    if (!result)
//    {
//        ev->mErrno = WSAGetLastError();
//    }
//    else
//    {
//        if (nBytes == 0 && ev->mPostConnect == 0 && ev->mPostAccept == 0)
//        {
//            ev->mErrno = LYNX_ERROR_EOF;
//        }
//    }
//
//    if (ev->mEventType & IOSERVICE_READ_EVENT)
//    {
//        if (ev->mPostAccept == 0)
//        {
//            ev->mData->mLen = nBytes;
//        }
//        else
//        {
//            LPSOCKADDR srcAddr, dstAddr;
//            Int32 srcAddrLen, dstAddrLen;
//            operation->mGetAcceptExSockAddrs(
//                (char *)ev->mData + sizeof(StreamBuffer::Node) + ev->mData->mOff,
//                0,
//                sizeof(SOCKADDR_IN) + 16,
//                sizeof(SOCKADDR_IN) + 16,
//                (SOCKADDR**)&srcAddr, &srcAddrLen,
//                (SOCKADDR**)&dstAddr, &dstAddrLen);
//            
//            SOCKADDR_IN addr;
//            memcpy(&addr, dstAddr, dstAddrLen);
//            ev->mDstIp = addr.sin_addr.s_addr;
//            ev->mDstPort = ntohs(addr.sin_port);
//            CreateIoCompletionPort((HANDLE)(SOCKET)ev->mFd, operation->mIocpHandle, 0, 0);
//
//            // set SO_SNDBUF to zero for zero-copy network stack (we maintain the buffers)
//            int bufSize = 0;
//            if (setsockopt((SOCKET)ev->mFd, SOL_SOCKET, SO_SNDBUF, (char*)&bufSize, sizeof(bufSize)))
//            {
//                LOG_WARN("Failed to setsocketopt for SO_SNDBUF to zero-copy");
//            }
//        }
//        ev->mCallback(ev, IOSERVICE_READ_EVENT);
//    }
//    else if (ev->mEventType & IOSERVICE_WRITE_EVENT)
//    {
//        if (ev->mPostConnect == 0)
//        {
//            ev->mData->mLen -= nBytes;
//            ev->mData->mOff += nBytes;
//        }
//        ev->mCallback(ev, IOSERVICE_WRITE_EVENT);
//    }
//    else
//    {
//        LOG_WARN("Unknown event type %d\n", ev->mEventType);
//        return -1;
//    }
//    return 0;
//}

#endif 


