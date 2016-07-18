#ifndef __LYNX_GAME_SERVER_H__
#define __LYNX_GAME_SERVER_H__

#include "OptionParser.h"
#include "ConfigParser.h"
#include "LoggerSystem.h"
#include "LogicSystem.h"
#include "NetworkSystem.h"
#include "PassportSystem.h"
#include "PersistSystem.h"
#include "../FireConfirm/PVPSystem.h"
#include "../Ranking/Ranking.h"
#include "../FireConfirm/NewException.h"
#include "../FireConfirm/ClimbTower.h"
#include "../FireConfirm/StageCalc.h"
#include "../FireConfirm/Gift.h"
#include "../FireConfirm/CourageChallenge.h"
#include "../FireConfirm/InlineActivity.h"
#include "../FireConfirm/Robot.h"


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


		

    private:
        bool updateTimer(void* args);
        void update();


	private:
		bool mStop;
        IOService mIOService;
        IOTimer mUpdateTimer; 
        UInt32 mExitWaitTime;

	};

} // namespace Lynx

#endif // __LYNX_GAME_SERVER_H__

