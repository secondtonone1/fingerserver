#ifndef __LYNX_GAME_SERVER_MAIN_THREAD_H__
#define __LYNX_GAME_SERVER_MAIN_THREAD_H__

#include "CommonLib.h"
#include "GameEvent.h"

#define THREAD_MSG_POOL_SIZE 256

namespace Lynx
{
	class MainThread 
	{
	public:
        MainThread() {}
        virtual ~MainThread() {}

    protected:
        static void initial();
        static void release();
        static void processEvents();

	protected:
		ThreadMsg* allocThreadMsg(UInt16 id, UInt16 channel)
		{
			ThreadMsg* threadMsg = NULL;
			if (sThreadMsgPoolIndex > 0)
			{
				threadMsg = sThreadMsgPool[sThreadMsgPoolIndex - 1];
				threadMsg->reset(id, channel);
				sThreadMsgPoolIndex--;
			}
			else
			{
				threadMsg = XNEW(ThreadMsg)(id, channel);
			}
			return threadMsg;
		}

		void deallocThreadMsg(ThreadMsg* threadMsg)
		{
			if (sThreadMsgPoolIndex < sThreadMsgPoolSize)
			{
				sThreadMsgPool[sThreadMsgPoolIndex] = threadMsg;
				sThreadMsgPoolIndex++;
			}
			else
			{
				XDELETE(threadMsg);
			}
		}

	protected:
		void pushEvent(Int32 eventId);
		void pushEvent(Int32 eventId, const String& arg0);
		void pushEvent(Int32 eventId, const String& arg0, const String& arg1);
		void pushEvent(Int32 eventId, const String& arg0, const String& arg1, const String& arg2);
		void pushEvent(Int32 eventId, const Vector<String>& args);
		void pushEvent(Int32 eventId, const UInt32& arg0);
		void pushEvent(Int32 eventId, const UInt32& arg0, const UInt32& arg1);
		void pushEvent(Int32 eventId, const UInt32& arg0, const UInt32& arg1, const UInt32& arg2);
	    void pushEvent(Int32 eventId, const UInt64& arg0);
		void pushEvent(Int32 eventId, const UInt64& arg0, const UInt64& arg1);
		void pushEvent(Int32 eventId, const UInt64& arg0, const UInt64& arg1, const UInt64& arg2);

	protected:
		static ThreadMsg* sThreadMsgPool[THREAD_MSG_POOL_SIZE];
		static UInt16 sThreadMsgPoolIndex;
		static UInt16 sThreadMsgPoolSize;
		static EventManager sEventManager;
	};
} // namespace Lynx

#define REGISTER_EVENT(id, func) LYNX_REGISTER_EVENT(sEventManager, id, this, func)
#define DEREGISTER_EVENT(id, func) LYNX_DEREGISTER_EVENT(sEventManager, id, this, func)

#endif // __LYNX_GAME_SERVER_MAIN_THREAD_H__

