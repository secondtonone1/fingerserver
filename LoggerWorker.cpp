#include "LoggerWorker.h"

using namespace Lynx;

LoggerWorker::LoggerWorker()
{

}

LoggerWorker::~LoggerWorker()
{

}

bool 
LoggerWorker::initial(UInt32 index)
{
    mWorkerIndex = index + 1;
    startup();

    LOG_INFO("Start LoggerWorker [%u]", mWorkerIndex);
    return true;
}

void 
LoggerWorker::release()
{
    shutdown();
    join();

    LOG_INFO("Shutdown ListenWorker [%u]", mWorkerIndex);
}


void 
LoggerWorker::onThreadStarted()
{
	
}

void 
LoggerWorker::onThreadShutdown()
{
	
}

void 
LoggerWorker::onPreUpdate()
{

}

void 
LoggerWorker::onPostUpdate()
{

}

void 
LoggerWorker::onDispatchMsg(ThreadMsg& threadMsg)
{
	mThreadMsgHandler.onDispatchMsg(threadMsg);
}
