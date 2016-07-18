#ifndef __LYNX_GAME_SERVER_NETWORK_THREAD_MSG_H__
#define __LYNX_GAME_SERVER_NETWORK_THREAD_MSG_H__

#include "CommonLib.h"

namespace Lynx
{
	enum
	{
		CONNECTION_ACCEPTED_NOTIFY                       = 1,
		CONNECTION_OPEN_NOTIFY                           = 2,
		CONNECTION_CLOSE_NOTIFY                          = 3,
		CONNECTION_BROKEN_NOTIFY                         = 4,
		CONNECTION_RECEIVE_NOTIFY                        = 5,
		CONNECTION_SEND_NOTIFY                           = 6,
		CONNECTION_BROADCAST_NOTIFY                      = 7,
	};

	struct ConnectionAcceptedNotify
	{
		ConnectionAcceptedNotify() : mType(0), mConnPointer(NULL) {}

        UInt8 mType; // 0 player, 1 gm
		void* mConnPointer;

		LYNX_MESSAGE_2(CONNECTION_ACCEPTED_NOTIFY, ConnectionAcceptedNotify, 
            UInt8, mType, void*, mConnPointer);
	};

	struct ConnectionOpenNotify
	{
		ConnectionOpenNotify() : mConnPointer(NULL), mConnId(0) {}

		void* mConnPointer;
		ConnId mConnId;

		LYNX_MESSAGE_2(CONNECTION_OPEN_NOTIFY, ConnectionOpenNotify,
			void*, mConnPointer, ConnId, mConnId);
	};

	struct ConnectionCloseNotify
	{
		ConnectionCloseNotify() : mConnId(0) {}

		ConnId mConnId;

		LYNX_MESSAGE_1(CONNECTION_CLOSE_NOTIFY, ConnectionCloseNotify,
			ConnId, mConnId);
	};

	struct ConnectionBrokenNotify
	{
		ConnectionBrokenNotify() : mConnId(0) {}

		ConnId mConnId;

		LYNX_MESSAGE_1(CONNECTION_BROKEN_NOTIFY, ConnectionBrokenNotify,
			ConnId, mConnId);
	};

	struct ConnectionReceiveNotify
	{
		ConnectionReceiveNotify() : mConnId(0) {}

		ConnId mConnId;
		Vector<void*> mNodeList;

		LYNX_MESSAGE_2(CONNECTION_RECEIVE_NOTIFY, ConnectionReceiveNotify,
			ConnId, mConnId, Vector<void*>, mNodeList);
	};

	struct ConnectionSendNotify
	{
		ConnectionSendNotify() : mConnId(0) {}

		ConnId mConnId;
		Vector<void*> mNodeList;

		LYNX_MESSAGE_2(CONNECTION_SEND_NOTIFY, ConnectionSendNotify,
			ConnId, mConnId, Vector<void*>, mNodeList);
	};

	struct ConnectionBroadcastNotify
	{
        ConnectionBroadcastNotify() : mBuffer(NULL), mBufferLen(0) {}

        void* mBuffer;
        UInt32 mBufferLen;

		LYNX_MESSAGE_2(CONNECTION_BROADCAST_NOTIFY, ConnectionBroadcastNotify,
            void*, mBuffer, UInt32, mBufferLen);
	};

} // namespace Lynx

#endif // __LYNX_GAME_SERVER_NETWORK_THREAD_MSG_H__
