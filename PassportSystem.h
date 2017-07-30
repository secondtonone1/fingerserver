#ifndef __LYNX_GAME_SERVER_PASSPORT_SYSTEM_H__
#define __LYNX_GAME_SERVER_PASSPORT_SYSTEM_H__

#include "MainThread.h"
#include "PassportWorker.h"

#define PASSPORT_THREAD_WORKER_COUNT 2

namespace Lynx
{
	class PassportSystem : public MainThread, public Singleton<PassportSystem>
	{
	public:
		PassportSystem();
		virtual ~PassportSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);
		
		template <typename MsgType >
		void postThreadMsg(const MsgType& msg, UInt16 channel)
		{
			UInt32 index = channel % PASSPORT_THREAD_WORKER_COUNT;
			PassportWorker& worker = mWorkerArray[index];
			if (!worker.isShutdown())
			{
				ThreadMsg* threadMsg = allocThreadMsg(MessageId<MsgType >::sMsgId, channel);
				serialize(msg, threadMsg->mBody, 0);
				worker.postMsgToInputQueue(threadMsg);
			}
		}

	private:
		void dispatchThreadMsg(ThreadMsg& msg);

        void onPassportAuthResp(PassportAuthResp& msg);

		void onPassporCodeResp(PassportCodeResp& msg);
    

	private:
		ThreadMsgHandler mThreadMsgHandler;
		PassportWorker mWorkerArray[PASSPORT_THREAD_WORKER_COUNT];

	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PASSPORT_SYSTEM_H__

