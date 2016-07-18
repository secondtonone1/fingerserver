#include "InterfaceSystem.h"

using namespace Lynx;

InterfaceSystem::InterfaceSystem()
{

}

InterfaceSystem::~InterfaceSystem()
{

}

bool 
InterfaceSystem::initial()
{
    for (UInt32 i = 0; i < INTERFACE_THREAD_WORKER_COUNT; ++i)
    {
        if (!mWorkerArray[i].initial(i))
        {
            LOG_WARN("Failed to initial interface thread worker [%u]", i);
            return false;
        }
    }
	LOG_INFO("Initial InterfaceSystem");
	return true;
}

void 
InterfaceSystem::release()
{
    for (UInt32 i = 0; i < INTERFACE_THREAD_WORKER_COUNT; ++i)
    {
        mWorkerArray[i].release();
    }
	LOG_INFO("Release InterfaceSystem");
}

void 
InterfaceSystem::update(const UInt64& accTime)
{
	for (UInt32 i = 0; i < INTERFACE_THREAD_WORKER_COUNT; ++i)
	{
		StreamBuffer& threadMsgStream = mWorkerArray[i].getOutputMsgQueue().getMsgs();
		UInt32 msgCount = threadMsgStream.length() / sizeof(void*);
		void* threadMsgPointer;
		ThreadMsg* threadMsg;
		for (UInt32 j = 0; j < msgCount; ++j)
		{
			threadMsgStream >> threadMsgPointer;
			threadMsg = (ThreadMsg*)threadMsgPointer;
			if (threadMsg)
			{
				dispatchThreadMsg(*threadMsg);
				deallocThreadMsg(threadMsg);
			}
		}
	}
}

void 
InterfaceSystem::dispatchThreadMsg(ThreadMsg& msg)
{
	mThreadMsgHandler.onDispatchMsg(msg);
}
