#ifndef __LYNX_GAME_SERVER_CONNECTION_PROXY_H__
#define __LYNX_GAME_SERVER_CONNECTION_PROXY_H__

#include "MainThread.h"
#include "CommonLib.h"

namespace Lynx
{
	class ClientConnection : public MainThread
	{
	public:
		ClientConnection();
		virtual ~ClientConnection();

		friend class NetworkSystem;

		bool initial();
		void release();

        bool isGMConnection() const { return !!mType; }

		ConnId getConnId() const { return mConnId; }
        String getFromIp() const { return mFromIp; }
        UInt16 getFromPort() const { return mFromPort; }
        const UInt64& getKeepaliveTime() const { return mKeepaliveTime; }

		static void registerMsgs();
		static void deregisterMsgs();

        void onReceived();

    private:
       
       
        static void registerCommonMsgs();
             
        static void registerLoginMsgs();
        
        static void registerItemMsgs();

		static void registerSkillMsgs();

 		static void registerFireMsgs();

		static void registerPVPMsgs();
       
		static void registerRhymeMsgs();

		static void registerHoarStoneMsgs();

		static void registerHeroEquipMsgs();

		static void registerGemMsgs();

		static void registerJewelryMsgs();

		static void registerServantMsgs();

		static void registerChatMsgs();

		static void registerDetailMsgs();

		static void registerGMMsgs();

		static void registerFriendsMsgs();

		static void registerEmailMsgs();

		static void registerFashionMsgs();

		static void registerCharactorMsgs();

		static void registerAchieveMsgs();
	

#ifdef DEBUG
        static void registerDebugMsgs();
#endif
        

	private:
		struct HandlerWrapper
		{
			HandlerWrapper() : mRecvWrapper(NULL), mFunc(NULL) {}

			bool (*mRecvWrapper)(ClientConnection& conn, char*);
			char* mFunc;
		};

		template <typename MsgType >
		static bool recvWrapper(ClientConnection& conn, char* func)
		{
			MsgType msg;
			if (!unserialize(msg, conn.mIStream, conn.mMsgHdr.mId))
			{
				LOG_WARN("Failed to unserialize msg %u", conn.mMsgHdr.mId);
				return false;
			}

			((void (*)(const ConnId&, MsgType&))func)(conn.mConnId, msg);
			return true;
		}

		template <typename MsgType >
		static void registerMsgHandler(void (*func)(const ConnId&, MsgType&))
		{
			HandlerWrapper* handler = XNEW(HandlerWrapper)();
			handler->mFunc = (char*)func;
			handler->mRecvWrapper = &recvWrapper<MsgType >;
			MsgId msgId = MessageId<MsgType >::sMsgId;
			ASSERT(msgId < LYNX_MAX_MSG_ID);
			ASSERT(!sHandlerWrapperArray[msgId]);
			sHandlerWrapperArray[msgId] = handler;
		}

	private:
		ConnId mConnId;
        UInt8 mType; // 0 player, 1 gm
		StreamBuffer mIStream;
		StreamBuffer mOStream;
		bool mReceivePending;
		bool mBrokenFlag;
		MsgHdr mMsgHdr;
		static HandlerWrapper* sHandlerWrapperArray[LYNX_MAX_MSG_ID + 1];
        String mFromIp;
        UInt16 mFromPort;
        UInt64 mKeepaliveTime;
	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_CONNECTION_PROXY_H__


