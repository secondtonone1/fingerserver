AM_CPPFLAGS = -Wno-deprecated -I./ -I../PlatformLib -D_LYNX_BYTE_ORDER_NOT_CONVERT  -D_LYNX_DEBUG -D_LYNX_HAS_LIBICONV
              #-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free

noinst_LIBRARIES = libnetwork.a 

libnetwork_a_SOURCES = \
	IOEventOperatorEpollImpl.cpp \
	IOEventOperatorIocpImpl.cpp \
	IOEventOperatorSelectImpl.cpp \
	IOService.cpp \
	IOTimer.cpp \
	Socket.cpp \
	TcpConnection.cpp \
	TcpListener.cpp
