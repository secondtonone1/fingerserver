#ifndef __LYNX_GAME_SERVER_PASSPORT_WORKER_H__
#define __LYNX_GAME_SERVER_PASSPORT_WORKER_H__

#include "CommonLib.h"
#include "PassportThreadMsg.h"
#include "DBInterface.h"

namespace Lynx
{
	class PassportWorker : public ThreadWorker, public Singleton<PassportWorker>
	{
	public:
		PassportWorker();
		virtual ~PassportWorker();

        bool initial(UInt32 index);
        void release();

		DBInterface getDBInterface();

	private:
		void onThreadStarted();
		void onThreadShutdown();
		void onPreUpdate();
		void onPostUpdate();
		void onDispatchMsg(ThreadMsg& msg);

        void onPassportAuthReq(PassportAuthReq& msg);
//		void onPassportGMAuthReq(PassportGMAuthReq& msg);

		 void onPassportCodeReq(PassportCodeReq& msg);

		  void onGetServerIDReq(PassportServerIDReq& msg);

	private:
		ThreadMsgHandler mThreadMsgHandler;
        DBInterface mDBInterface;
	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PASSPORT_WORKER_H__

