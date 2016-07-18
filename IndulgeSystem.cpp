#include "IndulgeSystem.h"

using namespace Lynx;

IndulgeSystem::IndulgeSystem()
{

}

IndulgeSystem::~IndulgeSystem()
{

}

bool 
IndulgeSystem::initial()
{
    for (UInt32 i = 0; i < INDULGE_THREAD_WORKER_COUNT; ++i)
    {
        if (!mWorkerArray[i].initial(i))
        {
            LOG_WARN("Failed to initial indulge thread worker [%u]", i);
            return false;
        }
    }
	LOG_INFO("Initial IndulgeSystem");
	return true;
}

void 
IndulgeSystem::release()
{
    for (UInt32 i = 0; i < INDULGE_THREAD_WORKER_COUNT; ++i)
    {
        mWorkerArray[i].release();
    }
	LOG_INFO("Release IndulgeSystem");
}

void 
IndulgeSystem::update(const UInt64& accTime)
{
	for (UInt32 i = 0; i < INDULGE_THREAD_WORKER_COUNT; ++i)
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
IndulgeSystem::dispatchThreadMsg(ThreadMsg& msg)
{
	mThreadMsgHandler.onDispatchMsg(msg);
}
