#ifndef __LYNX_PLATFORM_LIB_ERRNO_H__
#define __LYNX_PLATFORM_LIB_ERRNO_H__

#define LYNX_ERROR_NONE                         0
#define LYNX_ERROR_ACCEPT                       1   /* Tcp accept fail */
#define LYNX_ERROR_CONNECT                      2   /* Tcp connection fail */
#define LYNX_ERROR_TCPSEND                      3   /* Tcp send fail */
#define LYNX_ERROR_TCPRECV                      4   /* Tcp recv fail */
#define LYNX_ERROR_S11N                         5   /* serialize fail */
#define LYNX_ERROR_MSGID                        6   /* Don't match message id */
#define LYNX_ERROR_MSGHDR                       7   /* Don't match message hdr */
#define LYNX_ERROR_MSGLEN                       8   /* Invalid message len */
#define LYNX_ERROR_WOULDBLOCK                   9   /* send or receive would block */
#define LYNX_ERROR_DISPATCH                     10
#define LYNX_ERROR_EOF                          11

#endif // LYNX_UTILERRNO_H__


