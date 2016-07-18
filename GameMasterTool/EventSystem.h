#ifndef __LYNX_GAME_MASTER_TOOL_EVENT_QUEUE_H__
#define __LYNX_GAME_MASTER_TOOL_EVENT_QUEUE_H__

#include "CommonLib.h"

namespace Lynx
{
    class EventSystem : public Singleton<EventSystem>
    {
    public:
        EventSystem();
        virtual ~EventSystem();

        bool initial();
        void release();
        bool update();

        template <typename EventType >
        void registerHandler(Delegate<void (const EventType&)>& fn)
        {
            UInt16 msgId = MessageId<EventType >::sMsgId;
            if (mHandlerArray[msgId])
            {
                HandlerWrapper* handler = mHandlerArray[msgId];
                if (handler->mFunc == NULL)
                {
                    handler->mFunc = XNEW(Delegate<void (const EventType&)>);
                    assert(handler->mFunc);
                }
                (*(Delegate<void (const EventType&)>*)handler->mFunc) += fn;
                return;
            }

            HandlerWrapper* handler = XNEW(HandlerWrapper)();
            assert(handler);
            mHandlerArray[msgId] = handler;
            handler->mHandlerWrapper = &handlerWrapper<EventType >;
            if (handler->mFunc == NULL)
            {
                handler->mFunc = XNEW(Delegate<void (const EventType&)>);
                assert(handler->mFunc);
            }
            (*(Delegate<void (const EventType&)>*)handler->mFunc) += fn;
        }

        template <typename EventType>
        void deregisterHandler(Delegate<void (const EventType&)>& fn)
        {
            UInt16 msgId = MessageId<EventType >::sMsgId;
            HandlerWrapper* handler = mHandlerArray[msgId];
            if (handler)
            {
                if (handler->mFunc)
                {
                    XDELETE((Delegate<void (const EventType&)>*)handler->mFunc);
                }
                XSAFE_DELETE(mHandlerArray[msgId]);
            }
        }

        template <typename EventType >
        void postEvent(const EventType& ev)
        {
            MsgHdr hdr;
            hdr.mId = MessageId<EventType >::sMsgId;
            hdr.mBodyLen = serializeSize(ev, mStreamBuffer);
            hdr.serialize(mStreamBuffer);
            serialize(ev, mStreamBuffer);
        }
    public:
        template <typename EventType >
        static bool handlerWrapper(StreamBuffer& streamBuffer, void* func)
        {
            EventType msg;
            if (!unserialize(msg, streamBuffer))
            {
                LOG_WARN("Failed to unserialize msg.");
                return false;
            }
            ((Delegate<void (const EventType&)>*)func)->operator () (msg);
            return true;
        }

        struct HandlerWrapper
        {
            bool (*mHandlerWrapper)(StreamBuffer& streamBuffer, void* func);
            void* mFunc;
            HandlerWrapper() : mHandlerWrapper(NULL), mFunc(NULL) {}
        };

    private:
        StreamBuffer mStreamBuffer;
        HandlerWrapper* mHandlerArray[LYNX_MAX_MSG_ID + 1];
        MsgHdr mMsgHdr;
        bool mPendingHandleFlag;
    };
} // namespace Lynx

#define REGISTER_EVENT(msg, obj, fn) \
do { \
    EventSystem::getSingleton().registerHandler<msg>(Lynx::Delegate<void (const msg&)>(obj, fn)); \
} while (0)

#define DEREGISTER_EVENT(msg, obj, fn) \
do { \
    EventSystem::getSingleton().deregisterHandler<msg>(Lynx::Delegate<void (const msg&)>(obj, fn)); \
} while (0)

#endif // __LYNX_GAME_MASTER_TOOL_EVENT_QUEUE_H__

