#ifndef __LYNX_PLATFORM_LIB_EVENT_MANAGER_H__
#define __LYNX_PLATFORM_LIB_EVENT_MANAGER_H__

#include "Utilex.h"
#include "Delegate.h"
#include "Container.h"

#define LYNX_REGISTER_EVENT(eventManager, eventId, obj, func) \
do \
{ \
	EventIdMap::Iter* iter = eventManager.mEventIdMap.find(eventId); \
	if (iter == NULL) \
	{ \
	    EventInfo* eventInfo = XNEW(EventInfo)(); \
		eventInfo->mEventId = eventId; \
		iter = eventManager.mEventIdMap.insert(eventId, eventInfo); \
	} \
	EventInfo* eventInfo = iter->mValue; \
	eventInfo->mCallback += std::make_pair(obj, func); \
} while (0) 

#define LYNX_DEREGISTER_EVENT(eventManager, eventId, obj, func) \
do \
{ \
	EventIdMap::Iter* iter = eventManager.mEventIdMap.find(eventId); \
	if (iter) \
	{ \
	    EventInfo* eventInfo = iter->mValue; \
		if (eventInfo) \
		{ \
		    eventInfo->mCallback -= std::make_pair(obj, func); \
		} \
	} \
} while (0) 

namespace Lynx
{
    struct Event;
    typedef Delegate<void (const Event&)> EventCallback;

    struct EventInfo
    {
        Int32                     mEventId;
        //String                    mEventName;
        //bool                      mRepeatable;
        EventCallback             mCallback;
    };

	typedef Map<Int32, EventInfo*> EventIdMap;

    struct Event
    {
        EventInfo*                mEventInfo;
        Vector<String>            mEventArgs;
        bool                      mExpire;

        Event() : mExpire(false) {}

        bool operator == (const Event& rhs)
        {
            if (mEventInfo == NULL) return false;
            if (rhs.mEventInfo == NULL) return false;
            return (mEventInfo->mEventId == rhs.mEventInfo->mEventId);
        }
    };

    class _PlatformExport EventManager 
    {
    public:
        EventManager();
        virtual ~EventManager();

        bool initial();
        void release(); 
        void update();

        // 通过事件ID, 注册事件处理函数
        //void registerEventHandler(Int32 eventId, const EventCallback& cb);
        // 通过事件名称, 注册事件处理函数
        //void registerEventHandler(const String& eventName, const EventCallback& cb);
        // 反注册事件
        //void unregisterEventHandler(Int32 eventId, const EventCallback& cb);
        //void unregisterEventHandler(const String& eventName, const EventCallback& cb);
        
        // 投递事件函数
        void pushEvent(Int32 id);
        void pushEvent(Int32 id, const String& arg0);
        void pushEvent(Int32 id, const String& arg0, const String& arg1);
        void pushEvent(Int32 id, const String& arg0, const String& arg1, const String& arg2);
        void pushEvent(Int32 id, const Vector<String>& args);
        void pushEvent(Int32 id, UInt32 arg0);
        void pushEvent(Int32 id, UInt32 arg0, UInt32 arg1);
        void pushEvent(Int32 id, UInt32 arg0, UInt32 arg1, UInt32 arg2);

		void pushEvent(Int32 id, UInt64 arg0);
		void pushEvent(Int32 id, UInt64 arg0, UInt64 arg1);
		void pushEvent(Int32 id, UInt64 arg0, UInt64 arg1, UInt64 arg2);

        //void pushEvent(const String& id);
        //void pushEvent(const String& id, const String& arg0);
        //void pushEvent(const String& id, const String& arg0, const String& arg1);
        //void pushEvent(const String& id, const String& arg0, const String& arg1, const String& arg2);
        //void pushEvent(const String& id, const Vector<String>& args);
        //void pushEvent(const String& id, UInt32 arg0);
        //void pushEvent(const String& id, UInt32 arg0, UInt32 arg1);
        //void pushEvent(const String& id, UInt32 arg0, UInt32 arg1, UInt32 arg2);

        // 处理事件函数
        void processAllEvents();

		// 事件索引表, 通过事件ID
        EventIdMap mEventIdMap;

    private:
        void pushEvent(const Event& ev);
        void processEvent(const Event& ev);
        //typedef StringMap<EventInfo*> EventNameMap;
        typedef List<Event> EventQueue;
        // 事件索引表, 通过事件名称
        //EventNameMap  mEventNameMap;
        // 事件队列
        EventQueue    mEventQueue;
    };
} // namespace Lynx


#endif // __LYNX_PLATFORM_LIB_EVENT_MANAGER_H__

