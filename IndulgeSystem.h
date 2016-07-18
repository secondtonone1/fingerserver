#ifndef __LYNX_GAME_SERVER_INDULGE_SYSTEM_H__
#define __LYNX_GAME_SERVER_INDULGE_SYSTEM_H__

#include "MainThread.h"
#include "IndulgeWorker.h"

#define INDULGE_THREAD_WORKER_COUNT 2

namespace Lynx
{
	class IndulgeSystem : public MainThread, public Singleton<IndulgeSystem>
	{
	public:
		IndulgeSystem();
		virtual ~IndulgeSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);
		
		template <typename MsgType >
		void postThreadMsg(const MsgType& msg, UInt16 channel)
		{
			UInt32 index = channel % INDULGE_THREAD_WORKER_COUNT;
			IndulgeWorker& worker = mWorkerArray[index];
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
		IndulgeWorker mWorkerArray[INDULGE_THREAD_WORKER_COUNT];

	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_INDULGE_SYSTEM_H__

