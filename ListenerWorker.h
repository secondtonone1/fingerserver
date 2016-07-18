#ifndef __LYNX_GAME_SERVER_LISTEN_WORKER_H__
#define __LYNX_GAME_SERVER_LISTEN_WORKER_H__

#include "NetworkThreadMsg.h"

namespace Lynx
{
	class ListenerWorker : public ThreadWorker
	{
	public:
		ListenerWorker();
		~ListenerWorker();

        bool initial(UInt32 index, const String& mainListenIp, UInt16 mainListenPort,
            const String& gmListenIp, UInt16 gmListenPort);
        void release();

	private:
		void onThreadStarted();
		void onThreadShutdown();
		void onPreUpdate();
		void onPostUpdate();
		void onDispatchMsg(ThreadMsg& threadMsg);
        bool onUpdateTimer(void* args);

	private:
		void onMainAccepted(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
		void onMainAcceptFailed(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
		void onMainDisconnected(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
		void onMainReceived(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);

        void onGMAccepted(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
		void onGMAcceptFailed(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
		void onGMDisconnected(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
		void onGMReceived(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);

	private:
		IOService* mIOService;
		TcpListener* mMainTcpListener;
        TcpListener* mGMTcpListener;
		Set<TcpConnection*> mMainConnectionSet;
        Set<TcpConnection*> mGMConnectionSet;
		UInt32 mLastSecurityCheckTime;
        IOTimer* mUpdateTimer;
	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_LISTEN_WORKER_H__

