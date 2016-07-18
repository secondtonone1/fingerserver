#ifndef __LYNX_GAME_SERVER_PVP_WORKER_H__
#define __LYNX_GAME_SERVER_PVP_WORKER_H__

#include "../MainThread.h"
#include "../CommonLib/CommonLib.h"
#include "PVPThreadMsg.h"
#include "../LogicSystem.h"
namespace Lynx
{
	
	class PVPWorker : public ThreadWorker , public Singleton<PVPWorker>
	{

	public:
		PVPWorker();
		~PVPWorker();

		bool initial(UInt32 index);
		void release();

	private:
		void onThreadStarted();
		void onThreadShutdown();
		void onPreUpdate();
		void onPostUpdate();
		void onDispatchMsg(ThreadMsg& threadMsg);
		bool onUpdateTimer(void* args);

		
		

	private:
		void onPVPSystemRun(PVPTreadReq& msg );
// 		void onConnectionSendNotify(ConnectionSendNotify& msg);
// 		void onConnectionBroadcastNotify(ConnectionBroadcastNotify& msg);
// 		void onConnectionCloseNotify(ConnectionCloseNotify& msg);
// 
// 	private:
// 		void onReceived(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
// 		void onConnectionBroken(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);

	private:
// 		IOService* mIOService;
// 		Map<ConnId, TcpConnection*> mTcpConnectionMap;
		ThreadMsgHandler mThreadMsgHandler;
		IOTimer* mUpdateTimer;
	};


	
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_LOGGER_WORKER_H__

