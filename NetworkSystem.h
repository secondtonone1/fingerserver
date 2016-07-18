#ifndef __LYNX_GAME_SERVER_NETWORK_SYSTEM_H__
#define __LYNX_GAME_SERVER_NETWORK_SYSTEM_H__

#include "MainThread.h"
#include "ClientConnection.h"
#include "ListenerWorker.h"
#include "NetworkWorker.h"

#define NETWORK_THREAD_WORKER_COUNT 2

namespace Lynx
{
	struct GCTemplate
	{
		
		GCTemplate(): mPacketID(0), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_S11N_2(GCTemplate, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	class NetworkSystem : public MainThread, public Singleton<NetworkSystem>
	{
	public:
		NetworkSystem();
		virtual ~NetworkSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);

        UInt64 createLoginToken(const ConnId& connId);

		void setCanMsgRecived(bool val);
			
		bool getCanMsgRecived();

		void sendServerShutDown();

		
		void sender(const ConnId& connId,UInt16 PacketID,std::string jsonStr)
		{
			GCTemplate GCResp;
			GCResp.mRespJsonStr = jsonStr;
			GCResp.mPacketID = PacketID;

			ClientConnection* clientConn = getClientConnection(connId);
			if (!clientConn)
			{
				return;
			}

			StreamBuffer& ostream = clientConn->mOStream;

			MsgHdr hdr;
			hdr.mId = PacketID;
			hdr.mBodyLen = serializeSize(GCResp, ostream);
			hdr.mAckId = 0;
			if (!hdr.serialize(ostream))
			{
				LOG_WARN("Failed to serialize msg header.");
				return;
			}
			serialize(GCResp, ostream, hdr.mId);

			UInt32 nodeCount = ostream.getNodeCount();
			if (nodeCount)
			{
				ConnectionSendNotify notify;
				notify.mConnId = connId;
				StreamBuffer::Node* node = NULL;
				while (node = ostream.popNode())
				{
					notify.mNodeList.push_back(node);
				}
				postThreadMsgToNetworkWorker(notify, connId);
			}

			clientConn->mKeepaliveTime = TimeUtil::getTimeMilliSec();
		}

		template <typename MsgType >
		void sendMsg(const MsgType& msg, const ConnId& connId)
		{
			ClientConnection* clientConn = getClientConnection(connId);
			if (!clientConn)
			{
				//pushEvent(EVENT_CLIENT_CONNECTION_NOT_EXIST, connId);
				return;
			}

			StreamBuffer& ostream = clientConn->mOStream;

			MsgHdr hdr;
			hdr.mId = MessageId<MsgType >::sMsgId;
			hdr.mBodyLen = serializeSize(msg, ostream);
			hdr.mAckId = 0;
			if (!hdr.serialize(ostream))
			{
				LOG_WARN("Failed to serialize msg header.");
				return;
			}
			serialize(msg, ostream, hdr.mId);

			UInt32 nodeCount = ostream.getNodeCount();
			if (nodeCount)
			{
				ConnectionSendNotify notify;
				notify.mConnId = connId;
				StreamBuffer::Node* node = NULL;
				while (node = ostream.popNode())
				{
					notify.mNodeList.push_back(node);
				}
				postThreadMsgToNetworkWorker(notify, connId);
			}

            clientConn->mKeepaliveTime = TimeUtil::getTimeMilliSec();
		}

		void sendMsg(StreamBuffer& stream, const ConnId& connId)
		{
			ClientConnection* clientConn = getClientConnection(connId);
			if (!clientConn)
			{
				//pushEvent(EVENT_CLIENT_CONNECTION_NOT_EXIST, connId);
				return;
			}

			StreamBuffer& ostream = clientConn->mOStream;
			ostream += stream;

			UInt32 nodeCount = ostream.getNodeCount();
			if (nodeCount)
			{
				ConnectionSendNotify notify;
				notify.mConnId = connId;
				StreamBuffer::Node* node = NULL;
				while (node = ostream.popNode())
				{
					notify.mNodeList.push_back(node);
				}
				postThreadMsgToNetworkWorker(notify, connId);
			}

            clientConn->mKeepaliveTime = TimeUtil::getTimeMilliSec();
		}

        template <typename MsgType >
        void broadcastMsg(const MsgType& msg)
        {
            StreamBuffer ostream;

            MsgHdr hdr;
			hdr.mId = MessageId<MsgType >::sMsgId;
			hdr.mBodyLen = serializeSize(msg, ostream);
			hdr.mAckId = 0;
			if (!hdr.serialize(ostream))
			{
				LOG_WARN("Failed to serialize msg header.");
				return;
			}
			serialize(msg, ostream, hdr.mId);

            for (UInt32 i = 0; i < NETWORK_THREAD_WORKER_COUNT; ++i)
            {
                UInt32 bufferLen = ostream.length();
                char* buffer = (char*)XALLOC(bufferLen + 1);
                ostream.copy(buffer, bufferLen);
                ConnectionBroadcastNotify notify;
                notify.mBuffer = buffer;
                notify.mBufferLen = bufferLen;
                postThreadMsgToNetworkWorker(notify, i);
            }
        }

        String getClientConnectionFromIp(const ConnId& connId);
        UInt16 getClientConnectionFromPort(const ConnId& connId);

	private:
		void dispatchThreadMsg(ThreadMsg& msg);
		void onConnectionAcceptedNotify(ConnectionAcceptedNotify& msg);
		void onConnectionReceiveNotify(ConnectionReceiveNotify& msg);
		void onConnectionBrokenNotify(ConnectionBrokenNotify& msg);

	private:
		void onClientConnectionClosed(const Event& ev);

	private:
		template <typename MsgType >
		void postThreadMsgToNetworkWorker(const MsgType& msg, UInt16 channel)
		{
			UInt32 index = channel % NETWORK_THREAD_WORKER_COUNT;
			NetworkWorker& worker = mNetworkWorkerArray[index];
			if (!worker.isShutdown())
			{
				ThreadMsg* threadMsg = allocThreadMsg(MessageId<MsgType >::sMsgId, channel);
				serialize(msg, threadMsg->mBody, 0);
				worker.postMsgToInputQueue(threadMsg);
			}
		}

		ClientConnection* getClientConnection(const ConnId& connId)
		{
			Map<ConnId, ClientConnection*>::Iter* iter = mClientConnectionMap.find(connId);
			if (iter)
			{
				return iter->mValue;
			}
			return NULL;
		}

	private:
	    ListenerWorker mListenerWorker;
		NetworkWorker mNetworkWorkerArray[NETWORK_THREAD_WORKER_COUNT];
		ThreadMsgHandler mThreadMsgHandler;
		Map<ConnId, ClientConnection*> mClientConnectionMap;
		ConnId mConnIdIndex;
        UInt64 mLastCheckClientConnectionKeepaliveTime;

		
	};
	static bool gCanMsgRecived;//true ø…“‘ ’
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_NETWORK_SYSTEM_H__

