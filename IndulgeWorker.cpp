#include "IndulgeWorker.h"

using namespace Lynx;

IndulgeWorker::IndulgeWorker()
{

}

IndulgeWorker::~IndulgeWorker()
{

}

bool 
IndulgeWorker::initial(UInt32 index)
{
    mWorkerIndex = index + 1;
    startup();

    LOG_INFO("Start IndulgeWorker [%u]", mWorkerIndex);
    return true;
}

void 
IndulgeWorker::release()
{
    shutdown();
    join();

    LOG_INFO("Shutdown IndulgeWorker [%u]", mWorkerIndex);
}


void 
IndulgeWorker::onThreadStarted()
{
	
}

void 
IndulgeWorker::onThreadShutdown()
{
	
}

void 
IndulgeWorker::onPreUpdate()
{

}

void 
IndulgeWorker::onPostUpdate()
{

}

void 
IndulgeWorker::onDispatchMsg(ThreadMsg& threadMsg)
{
	mThreadMsgHandler.onDispatchMsg(threadMsg);
}
