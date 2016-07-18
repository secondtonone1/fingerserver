#include "Logger.h"
#include "Mutex.h"
#include "Utilex.h"
#include "TimeUtil.h"
#include "ThreadMsgQueue.h"

using namespace Lynx;

Logger::Logger() : mOutputFile(NULL), mPaginationSize(50*1024*1024),
    mOutputFileName(false), mAppend(false), mOutputFileIndex(0), 
    mSilenceLevel(0), mOutputType(OUTPUT_STDERR), mThreadMsgQueue(NULL),
    mAssertOutputFile(NULL)
{

}

Logger::~Logger()
{

}

void 
Logger::threadFunc()
{
    if (mOutputType != OUTPUT_THREAD)
    {
        return;
    }

    while (!isShutdown())
    {
        StreamBuffer& threadMsgStream = mThreadMsgQueue->getMsgs();
        UInt32 msgCount = threadMsgStream.length() / sizeof(void*);
        if (msgCount == 0)
        {
            TimeUtil::sleep(1);
            continue;
        }

        void* threadMsgPointer;
        ThreadMsg* threadMsg;
        for (UInt32 i = 0; i < msgCount; ++i)
        {
            threadMsgStream >> threadMsgPointer;
            threadMsg = (ThreadMsg*)threadMsgPointer;
            if (threadMsg)
            {
                dispatchThreadMsg(threadMsg);
                XDELETE(threadMsg);
            }
        }
    }

    // 处理消息队列里剩余的所有消息
    StreamBuffer& threadMsgStream = mThreadMsgQueue->getMsgs();
    UInt32 msgCount = threadMsgStream.length() / sizeof(void*);
    void* threadMsgPointer;
    ThreadMsg* threadMsg;
    for (UInt32 i = 0; i < msgCount; ++i)
    {
        threadMsgStream >> threadMsgPointer;
        threadMsg = (ThreadMsg*)threadMsgPointer;
        if (threadMsg)
        {
            dispatchThreadMsg(threadMsg);
            XDELETE(threadMsg);
        }
    }
}

void 
Logger::setPagination(Int32 size)
{
    ASSERT(size < 1024 * 4); // max 4G
    mPaginationSize = size * 1024 * 1024;
}

void 
Logger::setSilenceLevel(Int32 level)
{
    mSilenceLevel = level;
}
