
#include "../ConfigParser.h"
#include "PVPWorker.h"
#include "../TimeManager.h" 
#include "../Player.h"
#include "../LogicSystem.h"
#include "PVPSystem.h"

using namespace Lynx;

PVPWorker::PVPWorker() : mUpdateTimer(NULL)
{
}
PVPWorker::~PVPWorker()
{
}
bool 
PVPWorker::initial(UInt32 index)
{
	REGISTER_THREAD_MSG(mThreadMsgHandler, PVPTreadReq, PVPWorker::onPVPSystemRun);

	mWorkerIndex = index + 1;
	startup();

	//LOG_INFO("Start PVPWorker [%u]", mWorkerIndex);

	return true;
}

void 
PVPWorker::release()
{
	shutdown();
	join();

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PVPTreadReq);
	//LOG_INFO("Shutdown PVPWorker [%u]", mWorkerIndex);
}

void 
PVPWorker::onThreadStarted()
{
}
void 
PVPWorker::onThreadShutdown()
{	
}

void 
PVPWorker::onPreUpdate()
{
}

void 
PVPWorker::onPostUpdate()
{
}

void 
PVPWorker::onDispatchMsg(ThreadMsg& threadMsg)
{
	mThreadMsgHandler.onDispatchMsg(threadMsg);
}

bool 
PVPWorker::onUpdateTimer(void* args)
{
	StreamBuffer& threadMsgStream = mInputMsgQueue.getMsgs();
	UInt32 msgCount = threadMsgStream.length() / sizeof(void*);
	if (msgCount == 0)
	{
		return true;
	}

	void* threadMsgPointer;
	ThreadMsg* threadMsg;

	for (UInt32 i = 0; i < msgCount; ++i)
	{
		threadMsgStream >> threadMsgPointer;
		threadMsg = (ThreadMsg*)threadMsgPointer;
		if (threadMsg)
		{
			onDispatchMsg(*threadMsg);
			deallocThreadMsg(threadMsg);
		}
	}

	return true;
}

void PVPWorker::onPVPSystemRun( PVPTreadReq& msg )
{

	return;//todo
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(msg.connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}

	UInt32 sceneID = player->getSceneID();
	

	if (msg.typeID < 10)
	{
		PVPSystem::getSingleton().PlayerAction(msg);
	}
	else if (msg.typeID < 20)
	{
	}
}