#ifndef __LYNX_GAME_SERVER_LOGGER_WORKER_H__
#define __LYNX_GAME_SERVER_LOGGER_WORKER_H__

#include "CommonLib.h"
#include "LoggerThreadMsg.h"

namespace Lynx
{
	class LoggerWorker : public ThreadWorker
	{
	public:
		LoggerWorker();
		virtual ~LoggerWorker();

        bool initial(UInt32 index);
        void release();

	private:
		void onThreadStarted();
		void onThreadShutdown();
		void onPreUpdate();
		void onPostUpdate();
		void onDispatchMsg(ThreadMsg& threadMsg);

	private:
		ThreadMsgHandler mThreadMsgHandler;
	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_LOGGER_WORKER_H__

