#include "InterfaceWorker.h"

using namespace Lynx;

InterfaceWorker::InterfaceWorker()
{

}

InterfaceWorker::~InterfaceWorker()
{

}

bool 
InterfaceWorker::initial(UInt32 index)
{
    mWorkerIndex = index + 1;
    startup();

    LOG_INFO("Start InterfaceWorker [%u]", mWorkerIndex);
    return true;
}

void 
InterfaceWorker::release()
{
    shutdown();
    join();

    LOG_INFO("Shutdown InterfaceWorker [%u]", mWorkerIndex);
}

void 
InterfaceWorker::onThreadStarted()
{
	
}

void 
InterfaceWorker::onThreadShutdown()
{
	
}

void 
InterfaceWorker::onPreUpdate()
{

}

void 
InterfaceWorker::onPostUpdate()
{

}

void 
InterfaceWorker::onDispatchMsg(ThreadMsg& threadMsg)
{
	mThreadMsgHandler.onDispatchMsg(threadMsg);
}
