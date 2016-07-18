#include "EventManager.h"

using namespace Lynx;

EventManager::EventManager()
{

}

EventManager::~EventManager()
{

}

bool
EventManager::initial()
{
	mEventIdMap.clear();
	mEventQueue.clear();
    return true;
}

void 
EventManager::release()
{
	for (EventIdMap::Iter* iter = mEventIdMap.begin();
		iter != mEventIdMap.end(); iter = mEventIdMap.next(iter))
	{
		EventInfo* eventInfo = iter->mValue;
		if (eventInfo)
		{
			XDELETE(eventInfo);
		}
	}
    mEventIdMap.clear();
    mEventQueue.clear();
}

void
EventManager::update()
{
    processAllEvents();
}

void 
EventManager::pushEvent(const Event& ev)
{
    if (!ev.mEventInfo) return;
    mEventQueue.insertTail(ev);
}
        
void 
EventManager::processEvent(const Event& ev)
{
    struct EventInfo* eventInfo = ev.mEventInfo;
    if (eventInfo == NULL) return;

    if (!eventInfo->mCallback.empty())
    {
        eventInfo->mCallback(ev);
    }
}

//void 
//EventManager::registerEventHandler(Int32 eventId, const EventCallback& callback)
//{
//	EventIdMap::Iter* iter = mEventIdMap.find(eventId);
//	if (iter == NULL)
//	{
//		EventInfo* eventInfo = XNEW(EventInfo)();
//		eventInfo->mEventId = eventId;
//		iter = mEventIdMap.insert(eventId, eventInfo);
//	}
//
//	EventInfo* eventInfo = iter->mValue;
//	eventInfo->mCallback += callback;
//}

//void
//EventManager::registerEventHandler(const String& eventName, const EventCallback& callback)
//{
//    EventNameMap::Iter* iter = mEventNameMap.find(eventName);
//    if(iter == NULL) return;
//    struct EventInfo* eventInfo = iter->mValue;
//    if (eventInfo == NULL) return;
//    eventInfo->mCallback += callback;
//}


//void 
//EventManager::unregisterEventHandler(Int32 eventId, const EventCallback& callback)
//{
//    EventIdMap::Iter* iter = mEventIdMap.find(eventId);
//    if (iter == NULL) return;
//    struct EventInfo* eventInfo = iter->mValue;
//    if (eventInfo == NULL) return;
//    eventInfo->mCallback -= callback;
//}
        
//void 
//EventManager::unregisterEventHandler(const String& eventName, const EventCallback& callback)
//{
//    EventNameMap::Iter* iter = mEventNameMap.find(eventName);
//    if(iter == NULL) return;
//    struct EventInfo* eventInfo = iter->mValue;
//    if (eventInfo == NULL) return;
//    eventInfo->mCallback -= callback;
//}

void 
EventManager::pushEvent(Int32 eventId)
{
    EventIdMap::Iter* iter = mEventIdMap.find(eventId);
    if (iter == mEventIdMap.end()) return;
    Event ev;
    ev.mEventInfo = iter->mValue;
    pushEvent(ev);
}

//void 
//EventManager::pushEvent(const String& eventId)
//{
//    EventNameMap::Iter* iter = mEventNameMap.find(eventId);
//    if (iter == mEventNameMap.end()) return;
//    Event ev;
//    ev.mEventInfo = iter->mValue;
//    pushEvent(ev);
//}

void
EventManager::pushEvent(Int32 eventId, const String& arg0)
{
    EventIdMap::Iter* iter = mEventIdMap.find(eventId);
    if (iter == mEventIdMap.end()) return;
    Event ev;
    ev.mEventInfo = iter->mValue;
    ev.mEventArgs.push_back(arg0);
    pushEvent(ev);
}

//void
//EventManager::pushEvent(const String& eventId, const String& arg0)
//{
//    EventNameMap::Iter* iter = mEventNameMap.find(eventId);
//    if (iter == mEventNameMap.end()) return;
//    Event ev;
//    ev.mEventInfo = iter->mValue;
//    ev.mEventArgs.push_back(arg0);
//    pushEvent(ev);
//}

void
EventManager::pushEvent(Int32 eventId, const String& arg0, const String& arg1)
{
    EventIdMap::Iter* iter = mEventIdMap.find(eventId);
    if (iter == mEventIdMap.end()) return;
    Event ev;
    ev.mEventInfo = iter->mValue;
    ev.mEventArgs.push_back(arg0);
    ev.mEventArgs.push_back(arg1);
    pushEvent(ev);
}

//void
//EventManager::pushEvent(const String& eventId, const String& arg0, const String& arg1)
//{
//    EventNameMap::Iter* iter = mEventNameMap.find(eventId);
//    if (iter == mEventNameMap.end()) return;
//    Event ev;
//    ev.mEventInfo = iter->mValue;
//    ev.mEventArgs.push_back(arg0);
//    ev.mEventArgs.push_back(arg1);
//    pushEvent(ev);
//}

void 
EventManager::pushEvent(Int32 eventId, const String& arg0, 
                        const String& arg1, const String& arg2)
{
    EventIdMap::Iter* iter = mEventIdMap.find(eventId);
    if (iter == mEventIdMap.end()) return;
    Event ev;
    ev.mEventInfo = iter->mValue;
    ev.mEventArgs.push_back(arg0);
    ev.mEventArgs.push_back(arg1);
    ev.mEventArgs.push_back(arg2);
    pushEvent(ev);
}

//void 
//EventManager::pushEvent(const String& eventId, const String& arg0, 
//                        const String& arg1, const String& arg2)
//{
//    EventNameMap::Iter* iter = mEventNameMap.find(eventId);
//    if (iter == mEventNameMap.end()) return;
//    Event ev;
//    ev.mEventInfo = iter->mValue;
//    ev.mEventArgs.push_back(arg0);
//    ev.mEventArgs.push_back(arg1);
//    ev.mEventArgs.push_back(arg2);
//    pushEvent(ev);
//}

void
EventManager::pushEvent(Int32 eventId, const Vector<String>& args)
{
    EventIdMap::Iter* iter = mEventIdMap.find(eventId);
    if (iter == mEventIdMap.end()) return;
    Event ev;
    ev.mEventInfo = iter->mValue;
    ev.mEventArgs = args;
    pushEvent(ev);
}

//void
//EventManager::pushEvent(const String& eventId, const Vector<String>& args)
//{
//    EventNameMap::Iter* iter = mEventNameMap.find(eventId);
//    if (iter == mEventNameMap.end()) return;
//    Event ev;
//    ev.mEventInfo = iter->mValue;
//    ev.mEventArgs = args;
//    pushEvent(ev);
//}

void 
EventManager::pushEvent(Int32 eventId, UInt32 arg0)
{
    char tmpCharArg0[200];
    lynxItoa<UInt32>(arg0, tmpCharArg0, sizeof(tmpCharArg0), 10);
    pushEvent(eventId, tmpCharArg0);
}

//void 
//EventManager::pushEvent(const String& eventId, UInt32 arg0)
//{
//    char tmpCharArg0[200];
//    lynxItoa<UInt32>(arg0, tmpCharArg0, sizeof(tmpCharArg0), 10);
//    pushEvent(eventId, tmpCharArg0);
//}

void 
EventManager::pushEvent(Int32 eventId, UInt32 arg0, UInt32 arg1)
{
    char tmpCharArg0[200];
    char tmpCharArg1[200];
    lynxItoa<UInt32>(arg0, tmpCharArg0, sizeof(tmpCharArg0), 10);
    lynxItoa<UInt32>(arg1, tmpCharArg1, sizeof(tmpCharArg1), 10);
    pushEvent(eventId, tmpCharArg0, tmpCharArg1);
}

//void 
//EventManager::pushEvent(const String& eventId, UInt32 arg0, UInt32 arg1)
//{
//    char tmpCharArg0[200];
//    char tmpCharArg1[200];
//    lynxItoa<UInt32>(arg0, tmpCharArg0, sizeof(tmpCharArg0), 10);
//    lynxItoa<UInt32>(arg1, tmpCharArg1, sizeof(tmpCharArg1), 10);
//    pushEvent(eventId, tmpCharArg0, tmpCharArg1);
//}

void 
EventManager::pushEvent(Int32 eventId, UInt32 arg0, UInt32 arg1, UInt32 arg2)
{
    char tmpCharArg0[200];
    char tmpCharArg1[200];
    char tmpCharArg2[200];
    lynxItoa<UInt32>(arg0, tmpCharArg0, sizeof(tmpCharArg0), 10);
    lynxItoa<UInt32>(arg1, tmpCharArg1, sizeof(tmpCharArg1), 10);
    lynxItoa<UInt32>(arg2, tmpCharArg2, sizeof(tmpCharArg2), 10);
    pushEvent(eventId, tmpCharArg0, tmpCharArg1, tmpCharArg2);
}

void 
EventManager::pushEvent(Int32 eventId, UInt64 arg0)
{
	char tmpCharArg0[200];
    lynxItoa<UInt64>(arg0, tmpCharArg0, sizeof(tmpCharArg0), 10);
    pushEvent(eventId, tmpCharArg0);
}

void 
EventManager::pushEvent(Int32 eventId, UInt64 arg0, UInt64 arg1)
{
	char tmpCharArg0[200];
    char tmpCharArg1[200];
    lynxItoa<UInt64>(arg0, tmpCharArg0, sizeof(tmpCharArg0), 10);
    lynxItoa<UInt64>(arg1, tmpCharArg1, sizeof(tmpCharArg1), 10);
    pushEvent(eventId, tmpCharArg0, tmpCharArg1);
}

void 
EventManager::pushEvent(Int32 eventId, UInt64 arg0, UInt64 arg1, UInt64 arg2)
{
	char tmpCharArg0[200];
    char tmpCharArg1[200];
    char tmpCharArg2[200];
    lynxItoa<UInt64>(arg0, tmpCharArg0, sizeof(tmpCharArg0), 10);
    lynxItoa<UInt64>(arg1, tmpCharArg1, sizeof(tmpCharArg1), 10);
    lynxItoa<UInt64>(arg2, tmpCharArg2, sizeof(tmpCharArg2), 10);
    pushEvent(eventId, tmpCharArg0, tmpCharArg1, tmpCharArg2);
}

//void 
//EventManager::pushEvent(const String& eventId, UInt32 arg0, UInt32 arg1, UInt32 arg2)
//{
//    char tmpCharArg0[200];
//    char tmpCharArg1[200];
//    char tmpCharArg2[200];
//    lynxItoa<UInt32>(arg0, tmpCharArg0, sizeof(tmpCharArg0), 10);
//    lynxItoa<UInt32>(arg1, tmpCharArg1, sizeof(tmpCharArg1), 10);
//    lynxItoa<UInt32>(arg2, tmpCharArg2, sizeof(tmpCharArg2), 10);
//    pushEvent(eventId, tmpCharArg0, tmpCharArg1, tmpCharArg2);
//}

//void
//EventManager::processAllEvents()
//{
//    for (EventQueue::Iter* iter = mEventQueue.begin();
//        iter != mEventQueue.end(); iter = mEventQueue.next(iter))
//    {
//        if(iter->mValue.mExpire == true) continue;
//        if (iter->mValue.mEventInfo->mRepeatable == false)
//        {
//            bool repeated = false;
//            EventQueue::Iter* subNode = iter;
//            subNode = mEventQueue.next(subNode);
//            for (; subNode != mEventQueue.end(); subNode = mEventQueue.next(subNode))
//            {
//                if (subNode->mValue == iter->mValue)
//                {
//                    repeated = true;
//                    break;
//                }
//            }
//            if (repeated == true) continue;
//        }
//        iter->mValue.mExpire = true;
//        processEvent(iter->mValue);
//    }
//    mEventQueue.clear();
//}

void
EventManager::processAllEvents()
{
    for (EventQueue::Iter* iter = mEventQueue.begin();
        iter != mEventQueue.end(); iter = mEventQueue.next(iter))
    {
        if(iter->mValue.mExpire == true) continue;
        iter->mValue.mExpire = true;
        processEvent(iter->mValue);
    }
    mEventQueue.clear();
}
