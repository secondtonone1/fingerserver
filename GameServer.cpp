#ifdef _WIN32
//#include <vld.h>
#endif
#include "GameServer.h"
#include"jsoncpp/include/json/json.h"
#include<fstream>
#include "time.h" 
#include "stdio.h" 
#include "stdlib.h" 
#include "RedisManager.h"
#include "MemDataSystem.h"
#include "RankSystem.h"

using namespace Lynx;

const char* sOptionHelp =
"\nLynx Game Server [0.0.0.1] \n\n"
"Usage: gameserver [options]\n"
"    -n, run not as a daemon\n"
"    -c, use the specified server config file\n"
"Report bugs to <hailong.xia@lynx.com>\n";

#if defined (_WIN32)
#include <windows.h>
bool
signalHandler(DWORD signo)
{
	switch (signo)
	{

	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
		LOG_WARN("Received a kill signal");
// 		GameServer::getSingleton().stop();
		break;
	case CTRL_SHUTDOWN_EVENT:
		break;
	default:
		LOG_WARN("Received a unknown signal [%u]", signo);
		return false;
	}
	return true;
}

bool 
registerSystemSignal()
{
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)signalHandler, true))
	{
		LOG_ERROR("Faield to register kill signal");
		return false;
	}
	return true;
}
#elif defined (__linux__) || defined (__FreeBSD___)
#include <signal.h>

void
signalHandler(Int32 signo)
{
	switch (signo)
	{
	case SIGTERM:
		LOG_INFO("Received a kill %d signal", signo);
		GameServer::getSingleton().stop();
		break;
	default:
		LOG_WARN("Received a unknown signal [%u]", signo);
		break;
	}
}

bool
registerSystemSignal()
{
	if (signal(SIGTERM, signalHandler) == SIG_ERR)
	{
		LOG_ERROR("Failed to register term signal");
		return false;
	}

	if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
	{
		LOG_ERROR("Failed to ignore hup signal");
		return false;
	}

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		LOG_ERROR("Failed to ignore pipe signal");
		return false;
	}

	if (signal(SIGUSR1, SIG_IGN) == SIG_ERR)
	{
		LOG_ERROR("Failed to ignore usr1 signal");
		return false;
	}

	if (signal(SIGUSR2, SIG_IGN) == SIG_ERR)
	{
		LOG_ERROR("Failed to ignore usr2 signal");
		return false;
	}
	return true;
}
#endif

GameServer::GameServer() : 
mStop(false), 
#ifdef _WIN32
mIOService(IOSERVICE_SELECT_DRIVER),
#else
mIOService(IOSERVICE_EPOLL_DRIVER),
#endif
mUpdateTimer(&mIOService),
mExitWaitTime(0)
{

}

GameServer::~GameServer()
{

	UInt32 tmp = 0;
}

bool 
GameServer::initial(int argc, char** argv)
{
	MainThread::initial();

	registerSystemSignal();

	if (!OptionParser::getSingleton().parser(argc, argv))
	{
		printf("%s\n", sOptionHelp);
		return false;
	}

	if (OptionParser::getSingleton().isDaemonize())
	{
		if (OptionParser::getSingleton().daemonize("gameserver"))
		{
			LOG_ERROR("Failed to start gameserver as deamon.");
			return false;
		}
		LOG_INITIAL(Logger::OUTPUT_THREAD, true, false);
		LOG_SET_PAGINATION(50);
		LOG_SET_OUTPUT_FILE("gameserver");
	}
	else
	{
		LOG_INITIAL(Logger::OUTPUT_STDERR, true, false);
		LOG_SET_OUTPUT_FILE("gameserver");
	}

	String configFile = OptionParser::getSingleton().getConfigFile();

	if (!ConfigParser::getSingleton().initial(configFile.c_str()))
	{
		LOG_ERROR("Failed to parser server config [%s]", configFile.c_str());
		return false;
	}

	LOG_SET_SILENCE_LEVEL(ConfigParser::getSingleton().getLogLevel());

	String assetsPath = ConfigParser::getSingleton().getAssetsPath();
	if (!ResourceGroupManager::getSingleton().initial(assetsPath, true))
	{
		LOG_ERROR("Failed to initial resource group with config file [%s]", assetsPath.c_str());
		return false;
	}

	if (!TableManager::getSingleton().load())
	{
		LOG_WARN("Failed to load template tables.");
		return false;
	}

	if(!TableManagerCopy::getSingleton().load())
	{
		LOG_WARN("Failed to load template tables.");
		return false;
	}

	if (!PersistSystem::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial PersistSystem");
		return false;
	}

	if (!NetworkSystem::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial NetworkSystem");
		return false;
	}

	if (!LogicSystem::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial LogicSystem");
		return false;
	}

	if (!PassportSystem::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial PassportSystem");
		return false;
	}

	if (!LoggerSystem::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial LoggerSystem");
		return false;
	}
	if (GlobalVarManager::getSingleton().initial() != 1)
	{
		LOG_ERROR("Failed to initial SkeySvalue.csv");
		return false;
	}

	if (!PVPSystem::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial PVPSystem");
		return false;
	}

	if (!StageManager::getSingleton().initePreStageIDs())
	{
		LOG_ERROR("Failed to initial mPreStageIDs");
		return false;
	}

	if ( RankingManager::getSingleton().initRankingData()!=1)
	{
		LOG_ERROR("Failed to initial initRankingData");
		return false;
	}

	if ( NewException::getSingleton().init()!=1)
	{
		LOG_ERROR("Failed to initial initNewException");
		return false;
	}

	if (!RedisManager::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial RedisManager");
		return false;
	}

	if(!MemDataSystem::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial MemDataSystem");
		return false;
	}
	
	if(!RankSystem::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial RankSystem");
		return false;
	}

	if(!ConsortSystem::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial ConsortSystem");
		return false;
	}

	if(!RobotManager::getSingleton().initial())
	{
		LOG_ERROR("Failed to initial RobotManager");
		return false;
	}

	if(!GlobalValueManager::getSingleton().initGlobalValue())
	{
		LOG_ERROR("Failed to initial GlobalValueManager");
		return false;
	}
	
	RankGameManager::getSingleton().initRankGame();


	Logcxx::getSingleton().initLog4cxx();

	ServerData::getSingleton().serverDatainit();//位置不能变
	
	Random::getSingleton().setSeed(0);

	srand(time(0));
	mUpdateTimer.set(10, IOTimerCallback(this, &GameServer::updateTimer), NULL);

	//mPVPThread.initial(0);

	LOG_INFO("time is  =  %llu",time(0));	

	LOG_INFO("All Manager finish.....");
	return true;
}

void 
GameServer::release()
{
	LoggerSystem::getSingleton().release();
	PassportSystem::getSingleton().release();
	PersistSystem::getSingleton().release();
	
// 	#if defined (__linux__) 
	PVPWorker::getSingleton().release();
// 	#endif

	PVPSystem::getSingleton().release();
	LogicSystem::getSingleton().release();
	RedisManager::getSingleton().release();
	RankSystem::getSingleton().release();
	ConsortSystem::getSingleton().release();
	mPVPThread.release();

	NetworkSystem::getSingleton().release();		

	mUpdateTimer.cancel();
	
	MainThread::release();
}

void 
GameServer::update()
{
	MainThread::processEvents();


	UInt64 currTime = TimeUtil::getTimeMilliSec();
	
	RankSystem::getSingleton().update(currTime);
	PersistSystem::getSingleton().update(currTime);
	LogicSystem::getSingleton().update(currTime);
	
	PassportSystem::getSingleton().update(currTime);
	LoggerSystem::getSingleton().update(currTime);
	PVPSystem::getSingleton().update(currTime);
	
	ConsortSystem::getSingleton().update(currTime);
	NetworkSystem::getSingleton().update(currTime);
}


bool
GameServer::updateTimer(void* args)
{
	if (mStop)
	{
		UInt32 currTime = TimeUtil::getTimeSec();
		if (currTime - mExitWaitTime >= 10)
		{
			mIOService.stop();
			return false;
		}
		else
		{
			update();
			return true;
		}
	}
	else
	{
		update();
	}
	return true;
}

void 
GameServer::run()
{
	mIOService.run();
	LOG_INFO("IOService run exit");
}


void 
GameServer::stop(bool isImmediate)
{
	
	mStop = true;
	if (isImmediate)
	{
		mExitWaitTime = 0;
	}
	else
	{
		mExitWaitTime = TimeUtil::getTimeSec();
		
	}
}

void GameServer::shutDown()
{
	LOG_WARN(" game server shut down begin !!!!!!!!!!");
	LOG_WARN(" game server shut down send kick out all player 15s");
	NetworkSystem::getSingleton().sendServerShutDown();
	LOG_WARN(" game server shut down can not recived msg");
	NetworkSystem::getSingleton().setCanMsgRecived(false);
	LOG_WARN(" game server kick out all player");
	LogicSystem::getSingleton().kickOutAllPlayer();	
 	TimeUtil::sleep(5*1000);
	LOG_WARN(" game server close !!!!!!!!! 5s");
 	
	stop(true);
	TimeUtil::sleep(10*1000);
}

int
main(int argc, char* argv[])
{
	
	if (!GameServer::getSingleton().initial(argc, argv))
	{
		LOG_ERROR("Failed to initial game server");
		LOG_RELEASE();
		TimeUtil::sleep(30000);
		return -1;
	}

	LOG_INFO("GameServer started...........................");
	
	GameServer::getSingleton().run();
	GameServer::getSingleton().release();

	LOG_INFO("GameServer exited............................");
	LOG_RELEASE();



	return 0;
}

void GameServer::gameExit()
{
	LOG_INFO("server will shutdown.................................................");
	LogicSystem::getSingleton().setForbidLogin(0);
	TimeUtil::sleep(3);	
	
	mIOService.stop();

// 	GameServer::getSingleton().release();	
// 	LOG_RELEASE();
// 	exit(0);
	return ;
}