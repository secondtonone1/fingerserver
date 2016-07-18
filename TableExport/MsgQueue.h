#ifndef __LYNX_SERVER_DEBUG_TOOL_MSG_QUEUE_H__
#define __LYNX_SERVER_DEBUG_TOOL_MSG_QUEUE_H__

#include "PlatformLib.h"
#include "NetworkLib.h"
#include "MsgDefine.h"

namespace Lynx
{
    class MsgQueue
    {
    public:
        MsgQueue();
        ~MsgQueue();

        bool Initial();
        void Release();
        bool Update();

        template <typename MsgType>
        void RegisterHandler(Delegate<void (const MsgType&)>& fn)
        {
            UInt32 msgId = MessageId<MsgType >::sMsgId;
            if (m_HandlerArray[msgId])
            {
                MsgHandler* handler = m_HandlerArray[msgId];
                if (handler->m_Func == NULL)
                {
                    handler->m_Func = XNEW(Delegate<void (const MsgType&)>);
                    assert(handler->m_Func);
                }
                (*(Delegate<void (const MsgType&)>*)handler->m_Func) += fn;
                return;
            }

            MsgHandler* handler = XNEW(MsgHandler)();
            assert(handler);
            m_HandlerArray[msgId] = handler;
            handler->m_HandlerWrapper = &HandlerWrapper<MsgType >;
            if (handler->m_Func == NULL)
            {
                handler->m_Func = XNEW(Delegate<void (const MsgType&)>);
                assert(handler->m_Func);
            }
            (*(Delegate<void (const MsgType&)>*)handler->m_Func) += fn;
        }

        template <typename MsgType >
        void SendMsg(const MsgType& msg)
        {
            MsgHdr hdr;
            hdr.mId = MessageId<MsgType >::sMsgId;
            hdr.mBodyLen = serializeSize(msg, m_StreamBuffer);
            hdr.serialize(m_StreamBuffer);
            serialize(msg, m_StreamBuffer);
        }

    public:
        template <typename MsgType >
        static bool HandlerWrapper(StreamBuffer& streamBuffer, void* func)
        {
            MsgType msg;
            if (!unserialize(msg, streamBuffer))
            {
                LOG_WARN("Failed to unserialize msg.");
                return false;
            }
            ((Delegate<void (const MsgType&)>*)func)->operator () (msg);
            return true;
        }

        struct MsgHandler
        {
            bool (*m_HandlerWrapper)(StreamBuffer& streamBuffer, void* func);
            void* m_Func;
            MsgHandler() : m_HandlerWrapper(NULL), m_Func(NULL) {}
        };

    private:
        StreamBuffer           m_StreamBuffer;
        MsgHandler*            m_HandlerArray[LYNX_MAX_MSG_ID + 1];
        MsgHdr                 m_MsgHdr;
        bool                   m_PendingHandle;
    };
} // namespace Lynx

#define REGISTER_MSG_HANDLER(msg, obj, fn)                                         \
do {                                                                               \
    MSG_QUEUE().RegisterHandler<msg>(Lynx::Delegate<void (const msg&)>(obj, fn)); \
} while (0) 

#endif // __LYNX_SERVER_DEBUG_TOOL_MSG_QUEUE_H__