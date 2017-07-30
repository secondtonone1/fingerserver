#ifndef __LYNX_GAME_SERVER_H__
#define __LYNX_GAME_SERVER_H__

#include "OptionParser.h"
#include "ConfigParser.h"
#include "LoggerSystem.h"
#include "LogicSystem.h"
#include "NetworkSystem.h"
#include "PassportSystem.h"
#include "PersistSystem.h"
#include "ServerData.h"
#include "./FireConfirm/PVPSystem.h"
#include "./Ranking/Ranking.h"
#include "./FireConfirm/NewException.h"
#include "./FireConfirm/ClimbTower.h"
#include "./FireConfirm/StageCalc.h"
#include "./FireConfirm/Gift.h"
#include "./FireConfirm/CourageChallenge.h"
#include "./FireConfirm/InlineActivity.h"
#include "./FireConfirm/Robot.h"
#include "./FireConfirm/GlobalValue.h"
#include "./FireConfirm/RankGame.h"
#include "./PKSystem/pvpthread.h"
#include "./ConsortSystem/ConsortSystem.h"
#include "DetailInfoMsgHandler.h"
#include "Logcxx.h"


namespace Lynx
{
    class GameServer : public MainThread, public Singleton<GameServer>
	{
	public:
		GameServer();
		virtual ~GameServer();

		bool initial(int argc, char** argv);
		void release();

		void run();
		void stop(bool isImmediate = true);
		void shutDown();

		void gameExit();


		

    private:
        bool updateTimer(void* args);
        void update();


	private:
		bool mStop;
        IOService mIOService;
        IOTimer mUpdateTimer; 
        UInt32 mExitWaitTime;
		PVPThread mPVPThread;

	};

} // namespace Lynx

#endif // __LYNX_GAME_SERVER_H__

