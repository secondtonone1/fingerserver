#ifndef __LYNX_GAME_SERVER_PVPTHREAD_H__
#define __LYNX_GAME_SERVER_PVPTHREAD_H__

#include "../MainThread.h"
#include "../CommonLib/CommonLib.h"

#include "../LogicSystem.h"


#ifdef WIN32

#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
typedef SOCKET socketfd;

#endif


#ifdef __linux__

 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <time.h>
 #include <string.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <pthread.h>
 #include <iostream>
 #include <errno.h>
 #include <arpa/inet.h>
 #include <pthread.h>
#include <fcntl.h>

typedef int socketfd;
#endif


namespace Lynx
{

struct PVPData
{
	unsigned long long mSelfUid;
	unsigned long long mOtherUid;
	
	struct sockaddr_in mSelfAddr;
	struct sockaddr_in mOtherAddr;
	//上次心跳时间
	unsigned long long mLastTime;
	//上次检测时间
	unsigned long long mLastCheck;
	//总共超时发送次数
	int mTimeOut;
	//丢失标记，如果有人掉线了，此标记为1
	int  mLost;
	//由于收到放弃请求包需要双方都收到，此处为放弃玩家的uid，初始为0
	unsigned long long  mGiveUp;
	//由于收到结束请求包需要双方都收到，此处为玩家的uid，初始为0
	unsigned long long  mEndUp;
};

class PVPThread : public ThreadWorker , public Singleton<PVPThread>
	{

	public:
		PVPThread();
		~PVPThread();

		bool initial(UInt32 index);
		void release();

	private:
		void onThreadStarted();
		void onThreadShutdown();
		void onPreUpdate();
		void onPostUpdate();
		
		bool onUpdateTimer(void* args);
		void onDispatchMsg(ThreadMsg& threadMsg) {}
		
		bool setNoneBlock(socketfd fd);
		
		bool paraseMsg(char * msg, struct sockaddr_in  addrInfo);

		bool dealReqPVPMsg(unsigned long long msgId,  std::string dataStr, struct sockaddr_in  addrInfo);

		bool dealGiveUp(unsigned long long msgId,  std::string dataStr, struct sockaddr_in  addrInfo);

		bool dealSyncHeart(unsigned long long msgId,  std::string dataStr, struct sockaddr_in  addrInfo);

		bool dealQuitReq(unsigned long long msgId,  std::string dataStr, struct sockaddr_in  addrInfo);

		bool dealEndReq(unsigned long long msgId,  std::string dataStr, struct sockaddr_in  addrInfo);

	private:
	

	private:

		void checkTimeOut();
		void checkPVPIngTimeOut(UInt64 curTime);
		void checkWaitPVPTimeOut(UInt64 curTime);

		void dealSyncHeartPVPIng(UInt64 uid, std::string seqStr,struct sockaddr_in  addrInfo);

		void dealSyncHeartWaitPVP(UInt64 uid, std::string seqStr,struct sockaddr_in  addrInfo);
			
		IOTimer* mUpdateTimer;
		IOService* mIOService;

		#ifdef WIN32
		//std::list<SOCKADDR_IN> m_addrList;
		


		#endif



	#ifdef __linux__
	//std::list<sockaddr_in> m_addrList;
	
	#endif

	socketfd m_nSockServer;
	std::map<unsigned long long , PVPData > m_mapWaitForPVP;
	std::map<unsigned long long , PVPData >  m_mapPVPIng;
	
	};
}

#endif