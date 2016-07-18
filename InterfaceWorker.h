#ifndef __LYNX_GAME_SERVER_INTERFACE_WORKER_H__
#define __LYNX_GAME_SERVER_INTERFACE_WORKER_H__

#include "CommonLib.h"
#include "InterfaceThreadMsg.h"

namespace Lynx
{
	class InterfaceWorker : public ThreadWorker
	{
	public:
		InterfaceWorker();
		virtual ~InterfaceWorker();

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

#endif // __LYNX_GAME_SERVER_INTERFACE_WORKER_H__

