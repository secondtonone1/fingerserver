#ifndef __LYNX_PLATFORM_LIB_THREAD_EVENT_QUEUE_H__
#define __LYNX_PLATFORM_LIB_THREAD_EVENT_QUEUE_H__

#include "Container.h"
#include "StreamBuffer.h"
#include "Mutex.h"
#include "Delegate.h"
#include "../NetworkLib/MsgHdr.h"

#define LYNX_MAX_THREAD_MSG_ID  1024

namespace Lynx
{
    struct ThreadMsg
    {
        ThreadMsg(UInt16 id, UInt16 channel) : mId(id), mChannel(channel), 
			mBody(256 - sizeof(StreamBuffer::Node), 0) {}

        UInt16        mId;
		UInt16        mChannel; 
        StreamBuffer  mBody;

        ThreadMsg(ThreadMsg& rhs)
        {
            mId = rhs.mId;
			mChannel = rhs.mChannel;
            mBody = rhs.mBody;
        }

        ThreadMsg& operator = (ThreadMsg& rhs)
        {
            mId = rhs.mId;
			mChannel = rhs.mChannel;
            mBody = rhs.mBody;
            return *this;
        }

        void reset(UInt16 id, UInt16 channel)
        {
            mId = id;
			mChannel = channel;
            mBody.clear();
        }
    };

    class ThreadMsgQueue
    {
    public:
        ThreadMsgQueue() : mMsgStream(1024 - sizeof(StreamBuffer::Node), 1),
                           mTmpMsgStream(1024 - sizeof(StreamBuffer::Node), 1)
        {
        }

        virtual ~ThreadMsgQueue() 
        {
            if (mMsgStream.length() > 0)
            {
                UInt32 msgCount = mMsgStream.length() / sizeof(void*);
                void* threadMsgPointer;
                ThreadMsg* threadMsg;
                for (UInt32 i = 0; i < msgCount; ++i)
                {
                    mMsgStream >> threadMsgPointer;
                    threadMsg = (ThreadMsg*)threadMsgPointer;
                    if (threadMsg)
                    {
                        XDELETE(threadMsg);
                    }
                }
            }
        }

        // 投递消息进入消息列表中
        void postMsg(ThreadMsg* ev)
        {
            SpinLock lock(mMsgStreamMutex);
            mMsgStream << (void*)ev;
        }

        void postMsgs(StreamBuffer& msgStream)
        {
            SpinLock lock(mMsgStreamMutex);
            msgStream >> mMsgStream;
        }

        // 批量获得消息
        StreamBuffer& getMsgs()
        {
            SpinLock lock(mMsgStreamMutex);
            mMsgStream >> mTmpMsgStream;
            return mTmpMsgStream;
        }

        UInt32 getMsgStreamLength()
        {
            return mMsgStream.length();
        }

        UInt32 getTmpMsgStreamLength()
        {
            return mTmpMsgStream.length();
        }

    private:
        StreamBuffer          mMsgStream;
        mutable SpinMutex     mMsgStreamMutex;
        StreamBuffer          mTmpMsgStream;
    };

    class ThreadMsgHandler
	{
	public:
		ThreadMsgHandler() 
		{
			for (Int32 i = 0; i < LYNX_MAX_THREAD_MSG_ID; i++)
			{
				mHandlerWrapperArray[i] = NULL;
			}
		}

		virtual ~ThreadMsgHandler() 
		{
			for (Int32 i = 0; i < LYNX_MAX_THREAD_MSG_ID; i++)
			{
				XSAFE_DELETE(mHandlerWrapperArray[i]);
			}
		}

		template <typename MsgType >
		void registerHandler(const Delegate<void (MsgType&)>& func)
		{
			Delegate<void (MsgType&)>* pFunc = XNEW(Delegate<void (MsgType&)>)();
			*pFunc = func;
			HandlerWrapper* handler = XNEW(HandlerWrapper)();
			ASSERT(handler);
			handler->mFunc = (char *)pFunc;
			handler->mIsStatic = false;
			handler->mRecvWrapper = &recvWrapper<MsgType >;
			UInt16 msgId = MessageId<MsgType >::sMsgId;
			ASSERT(msgId < LYNX_MAX_THREAD_MSG_ID);
			ASSERT(!mHandlerWrapperArray[msgId]);
			mHandlerWrapperArray[msgId] = handler;
		}

		template <typename MsgType >
		void registerHandler(void (*func)(MsgType&))
		{
			HandlerWrapper* handler = XNEW(HandlerWrapper)();
			ASSERT(handler);
			handler->mFunc = (char*)func;
			handler->mIsStatic = true;
			handler->mRecvWrapper = &recvWrapper<MsgType >;
			UInt16 msgId = MessageId<MsgType >::sMsgId;
			ASSERT(msgId < LYNX_MAX_THREAD_MSG_ID);
			ASSERT(!mHandlerWrapperArray[msgId]);
			mHandlerWrapperArray[msgId] = handler;
		}

		template <typename MsgType >
		void deregisterHandler()
		{
			UInt16 msgId = MessageId<MsgType >::sMsgId;
			if (msgId > LYNX_MAX_THREAD_MSG_ID)
			{
				return;
			}
			HandlerWrapper* handler = mHandlerWrapperArray[msgId];
			if (handler)
			{
				if (handler->mFunc && !handler->mIsStatic)
				{
					XDELETE((Delegate<void (MsgType&)>*)(handler->mFunc));
				}
				XSAFE_DELETE(mHandlerWrapperArray[msgId]);
			}
		}

		void onDispatchMsg(ThreadMsg& threadMsg)
		{
			UInt16 msgId = threadMsg.mId;

			HandlerWrapper* handler = mHandlerWrapperArray[msgId];
			if (handler)
			{
				handler->mRecvWrapper(handler->mIsStatic, handler->mFunc, threadMsg);
			}
			else
			{
				LOG_WARN("Don't match thread msg id %u", msgId);
			}
		}

	protected:
		template <typename MsgType>
		static bool recvWrapper(bool isStatic, char* func, ThreadMsg& threadMsg)
		{
			MsgType msg;

			if (!unserialize(msg, threadMsg.mBody, 0))
			{
				LOG_WARN("Failed to unserialize thread msg %u", threadMsg.mId);
				return false;
			}

			if (isStatic)
			{
				((void (*)(MsgType&))func)(msg);
			}
			else
			{
				((Delegate<void (MsgType&)>*)func)->operator () (msg);
			}
			return true;
		}

		struct HandlerWrapper
		{
			bool mIsStatic;
			bool (*mRecvWrapper)(bool isStatic, char*, ThreadMsg&);
			char* mFunc;

			HandlerWrapper() : mRecvWrapper(NULL), mIsStatic(false), mFunc(NULL) {}
		};

		HandlerWrapper* mHandlerWrapperArray[LYNX_MAX_THREAD_MSG_ID + 1];
	};

	class ThreadWorker : public Thread
	{
	public:
		ThreadWorker(UInt32 threadMsgPoolSize = 128)
		{
			mWorkerIndex = 0;
			mThreadMsgPoolSize = threadMsgPoolSize;
			mThreadMsgPool = (ThreadMsg**)XALLOC(threadMsgPoolSize * sizeof(ThreadMsg*));
			memset(mThreadMsgPool, 0, threadMsgPoolSize * sizeof(ThreadMsg*));
			mThreadMsgPoolIndex = 0;
		}

		virtual ~ThreadWorker()
		{
			for (UInt32 i = 0; i < mThreadMsgPoolIndex; ++i)
			{
				XDELETE(mThreadMsgPool[i]);
			}
			mThreadMsgPoolIndex = 0;
			XDELETE(mThreadMsgPool);
		}

		ThreadMsg* allocThreadMsg(UInt16 id, UInt16 channel)
		{
			ThreadMsg* threadMsg = NULL;
		    if (mThreadMsgPoolIndex > 0)
			{
				threadMsg = mThreadMsgPool[mThreadMsgPoolIndex - 1];
				threadMsg->reset(id, channel);
				mThreadMsgPoolIndex--;
			}
			else
			{
				threadMsg = XNEW(ThreadMsg)(id, channel);
			}
			return threadMsg;
		}

		void deallocThreadMsg(ThreadMsg* threadMsg)
		{
			if (mThreadMsgPoolIndex < mThreadMsgPoolSize)
			{
				mThreadMsgPool[mThreadMsgPoolIndex] = threadMsg;
				mThreadMsgPoolIndex++;
			}
			else
			{
				XDELETE(threadMsg);
			}
		}

		template <typename MsgType >
		void postMsgToInputQueue(const MsgType& msg, UInt16 channel)
		{
			ThreadMsg* threadMsg = allocThreadMsg(MessageId<MsgType >::sMsgId, channel);
			serialize(msg, threadMsg->mBody, 0);
			mInputMsgQueue.postMsg(threadMsg);
		}

		void postMsgToInputQueue(ThreadMsg* threadMsg)
		{
			mInputMsgQueue.postMsg(threadMsg);
		}

		ThreadMsgQueue& getOutputMsgQueue() { return mOutputMsgQueue; }

	protected:

		template <typename MsgType >
		void postMsgToOutputQueue(const MsgType& msg, UInt16 channel)
		{
			ThreadMsg* threadMsg = allocThreadMsg(MessageId<MsgType >::sMsgId, channel);
			serialize(msg, threadMsg->mBody, 0);
			mOutputMsgQueue.postMsg(threadMsg);
		}

		void postMsgToOutputQueue(ThreadMsg* threadMsg)
		{
			mOutputMsgQueue.postMsg(threadMsg);
		}

		virtual void onThreadStarted() = 0;
		virtual void onThreadShutdown() = 0;
		virtual void onPreUpdate() = 0;
		virtual void onPostUpdate() = 0;
		virtual void onDispatchMsg(ThreadMsg& threadMsg) = 0;

	private:
	
		void threadFunc()
		{
			onThreadStarted();

			while (!isShutdown())
			{
				onPreUpdate();

				{
					StreamBuffer& threadMsgStream = mInputMsgQueue.getMsgs();
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
							onDispatchMsg(*threadMsg);
							deallocThreadMsg(threadMsg);
						}
					}
				}

				onPostUpdate();
			}

			StreamBuffer& threadMsgStream = mInputMsgQueue.getMsgs();
			UInt32 msgCount = threadMsgStream.length() / sizeof(void*);
			if (msgCount > 0)
			{
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
			}

			onThreadShutdown();
		}

	protected:
		UInt32 mWorkerIndex;
		ThreadMsgQueue mInputMsgQueue;
		ThreadMsgQueue mOutputMsgQueue;
		ThreadMsg** mThreadMsgPool;
		UInt32 mThreadMsgPoolIndex;
		UInt32 mThreadMsgPoolSize;
	};

#define REGISTER_THREAD_MSG(handler, msgtype, func) \
    handler.registerHandler<msgtype>(Delegate<void (msgtype&)>(this, &func))

#define DEREGISTER_THREAD_MSG(handler, msgtype) \
	handler.deregisterHandler<msgtype>()

} // namespace Lynx




#endif // __LYNX_PLATFORM_LIB_THREAD_EVENT_QUEUE_H__


