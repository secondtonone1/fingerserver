#ifndef __LYNX_GAME_SERVER_LOGGER_SYSTEM_H__
#define __LYNX_GAME_SERVER_LOGGER_SYSTEM_H__

#include "MainThread.h"
#include "LoggerWorker.h"

#define LOGGER_THREAD_WORKER_COUNT 2

namespace Lynx
{
	class LoggerSystem : public MainThread, public Singleton<LoggerSystem>
	{
	public:
		LoggerSystem();
		virtual ~LoggerSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);
		
		template <typename MsgType >
		void postThreadMsg(const MsgType& msg, UInt16 channel)
		{
			UInt32 index = channel % LOGGER_THREAD_WORKER_COUNT;
			LoggerWorker& worker = mWorkerArray[index];
			if (!worker.isShutdown())
			{
				ThreadMsg* threadMsg = allocThreadMsg(MessageId<MsgType >::sMsgId, channel);
				serialize(msg, threadMsg->mBody, 0);
				worker.postMsgToInputQueue(threadMsg);
			}
		}


		void writeFile(String fileName,char buf[409600]);

		void writeLog(String fileName,Guid playerID,string playerName,string str);

		void stringNameLog(string fileName,Guid playerID,string playerName,string str);

		
	private:
		void dispatchThreadMsg(ThreadMsg& msg);
		

	private:
		ThreadMsgHandler mThreadMsgHandler;
		LoggerWorker mWorkerArray[LOGGER_THREAD_WORKER_COUNT];

	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_LOGGER_SYSTEM_H__

