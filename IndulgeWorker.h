#ifndef __LYNX_GAME_SERVER_INDULGE_WORKER_H__
#define __LYNX_GAME_SERVER_INDULGE_WORKER_H__

#include "CommonLib.h"
#include "IndulgeThreadMsg.h"

namespace Lynx
{
	class IndulgeWorker : public ThreadWorker
	{
	public:
		IndulgeWorker();
		virtual ~IndulgeWorker();

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

#endif // __LYNX_GAME_SERVER_INDULGE_WORKER_H__

