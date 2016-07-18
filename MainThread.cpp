#include "MainThread.h"

using namespace Lynx;

ThreadMsg* MainThread::sThreadMsgPool[THREAD_MSG_POOL_SIZE];
UInt16 MainThread::sThreadMsgPoolIndex;
UInt16 MainThread::sThreadMsgPoolSize;
EventManager MainThread::sEventManager;

void 
MainThread::initial()
{
    memset(sThreadMsgPool, 0, sizeof(ThreadMsg*) * THREAD_MSG_POOL_SIZE);
    sThreadMsgPoolSize = THREAD_MSG_POOL_SIZE;
    sThreadMsgPoolIndex = 0;
    sEventManager.initial();
}

void 
MainThread::release()
{
    for (UInt16 i = 0; i < sThreadMsgPoolIndex; ++i)
    {
        if (sThreadMsgPool[i])
        {
            XDELETE(sThreadMsgPool[i]);
            sThreadMsgPool[i] = NULL;
        }
    }
    sThreadMsgPoolIndex = 0;
    sEventManager.release();
}

void 
MainThread::processEvents()
{
	sEventManager.processAllEvents();
}

void 
MainThread::pushEvent(Int32 eventId)
{
	sEventManager.pushEvent(eventId);
}

void 
MainThread::pushEvent(Int32 eventId, const String& arg0)
{
	sEventManager.pushEvent(eventId, arg0);
}

void 
MainThread::pushEvent(Int32 eventId, const String& arg0, const String& arg1)
{
	sEventManager.pushEvent(eventId, arg0, arg1);
}

void 
MainThread::pushEvent(Int32 eventId, const String& arg0, const String& arg1, const String& arg2)
{
	sEventManager.pushEvent(eventId, arg0, arg1, arg2);
}

void 
MainThread::pushEvent(Int32 eventId, const Vector<String>& args)
{
	sEventManager.pushEvent(eventId, args);
}

void 
MainThread::pushEvent(Int32 eventId, const UInt32& arg0)
{
	sEventManager.pushEvent(eventId, arg0);
}

void 
MainThread::pushEvent(Int32 eventId, const UInt32& arg0, const UInt32& arg1)
{
	sEventManager.pushEvent(eventId, arg0, arg1);
}

void 
MainThread::pushEvent(Int32 eventId, const UInt32& arg0, const UInt32& arg1, const UInt32& arg2)
{
	sEventManager.pushEvent(eventId, arg0, arg1, arg2);
}

void 
MainThread::pushEvent(Int32 eventId, const UInt64& arg0)
{
	sEventManager.pushEvent(eventId, arg0);
}

void 
MainThread::pushEvent(Int32 eventId, const UInt64& arg0, const UInt64& arg1)
{
	sEventManager.pushEvent(eventId, arg0, arg1);
}

void 
MainThread::pushEvent(Int32 eventId, const UInt64& arg0, const UInt64& arg1, const UInt64& arg2)
{
	sEventManager.pushEvent(eventId, arg0, arg1, arg2);
}

