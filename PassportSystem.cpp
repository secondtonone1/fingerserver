#include "PassportSystem.h"
#include "GameServer.h"
#include "FireConfirm/Code.h"

using namespace Lynx;

PassportSystem::PassportSystem()
{

}

PassportSystem::~PassportSystem()
{

}

bool 
PassportSystem::initial()
{

    // 注册线程消息
    REGISTER_THREAD_MSG(mThreadMsgHandler, PassportAuthResp, PassportSystem::onPassportAuthResp);

	
	REGISTER_THREAD_MSG(mThreadMsgHandler, PassportCodeResp, PassportSystem::onPassporCodeResp);
   // REGISTER_THREAD_MSG(mThreadMsgHandler, PassportGMAuthResp, PassportSystem::onPassportGMAuthResp);


    for (UInt32 i = 0; i < PASSPORT_THREAD_WORKER_COUNT; ++i)
    {
        if (!mWorkerArray[i].initial(i))
        {
            LOG_WARN("Failed to initial passport thread worker [%u]", i);
            return false;
        }
    }
	LOG_INFO("Initial PassportSystem");
	return true;
}

void 
PassportSystem::release()
{

    // 注销线程消息
    DEREGISTER_THREAD_MSG(mThreadMsgHandler, PassportAuthResp);

	DEREGISTER_THREAD_MSG(mThreadMsgHandler, PassportCodeResp);
  

    for (UInt32 i = 0; i < PASSPORT_THREAD_WORKER_COUNT; ++i)
    {
        mWorkerArray[i].release();
    }
	LOG_INFO("Release PassportSystem.h");
}

void 
PassportSystem::update(const UInt64& accTime)
{
	for (UInt32 i = 0; i < PASSPORT_THREAD_WORKER_COUNT; ++i)
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
PassportSystem::dispatchThreadMsg(ThreadMsg& msg)
{
	mThreadMsgHandler.onDispatchMsg(msg);
}

void 
PassportSystem::onPassportAuthResp(PassportAuthResp& msg)
{
    if (msg.merrorId == LynxErrno::None)
    {
        LogicSystem::getSingleton().passportAuthSuccess(msg.mConnId, msg.mAccountId, msg.mAccountName);
    }
    else
    {
        LogicSystem::getSingleton().passportAuthFail(msg.mConnId, msg.mAccountName);
    }
}

void 
PassportSystem::onPassporCodeResp(PassportCodeResp& msg)
{
	CodeManager::getSingleton().codeResp(msg);
}



