#ifndef __LYNX_GAME_SERVER_INTERFACE_SYSTEM_H__
#define __LYNX_GAME_SERVER_INTERFACE_SYSTEM_H__

#include "MainThread.h"
#include "InterfaceWorker.h"

#define INTERFACE_THREAD_WORKER_COUNT 1

namespace Lynx
{
	class InterfaceSystem : public MainThread, public Singleton<InterfaceSystem>
	{
	public:
		InterfaceSystem();
		virtual ~InterfaceSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);
		
		template <typename MsgType >
		void postThreadMsg(const MsgType& msg, UInt16 channel)
		{
			UInt32 index = channel % INTERFACE_THREAD_WORKER_COUNT;
			InterfaceWorker& worker = mWorkerArray[index];
			if (!worker.isShutdown())
			{
				ThreadMsg* threadMsg = allocThreadMsg(MessageId<MsgType >::sMsgId, channel);
				serialize(msg, threadMsg->mBody, 0);
				worker.postMsgToInputQueue(threadMsg);
			}
		}

	private:
		void dispatchThreadMsg(ThreadMsg& msg);

	private:
		ThreadMsgHandler mThreadMsgHandler;
		InterfaceWorker mWorkerArray[INTERFACE_THREAD_WORKER_COUNT];

	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_INTERFACE_SYSTEM_H__

