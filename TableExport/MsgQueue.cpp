#include "MsgQueue.h"

using namespace Lynx;

MsgQueue::MsgQueue() : m_StreamBuffer(8192 - sizeof(StreamBuffer::Node), 5)
                     , m_PendingHandle(false)
{
    for (Int32 i = 0; i < LYNX_MAX_MSG_ID; i++)
    {
        m_HandlerArray[i] = NULL;
    }
    m_StreamBuffer.clear();
}

MsgQueue::~MsgQueue()
{
    for (Int32 i = 0; i < LYNX_MAX_MSG_ID; i++)
    {
        if (m_HandlerArray[i])
        {
            XDELETE(m_HandlerArray[i]->m_Func);
        }
        XSAFE_DELETE(m_HandlerArray[i]);
    }
}

bool 
MsgQueue::Initial()
{
    return true;
}

void 
MsgQueue::Release()
{
    
}

bool 
MsgQueue::Update()
{
    while (1)
    {
        if (m_PendingHandle == false)
        {
            if (!m_MsgHdr.unserialize(m_StreamBuffer))
            {
                break;
            }
        }

        if (m_MsgHdr.mBodyLen > m_StreamBuffer.length())
        {
            m_PendingHandle = true;
            break;
        }

        m_PendingHandle = false;

        if (m_MsgHdr.mId >= LYNX_MAX_MSG_ID)
        {
            LOG_WARN("Msg id overflow: id=%d, len=%d", m_MsgHdr.mId, m_MsgHdr.mBodyLen);
            return false;
        }

        MsgHandler* handler = m_HandlerArray[m_MsgHdr.mId];
        if (handler)
        {
            if (!handler->m_HandlerWrapper(m_StreamBuffer, handler->m_Func))
            {
                LOG_WARN("Failed to call handler wrapper, msgid=%d, msglen=%d",
                    m_MsgHdr.mId, m_MsgHdr.mBodyLen);
                return false;
            }
        }
        else
        {
            LOG_WARN("Don't match this message, id=%d, len=%d", 
                    m_MsgHdr.mId, m_MsgHdr.mBodyLen);
            m_StreamBuffer.drain(m_MsgHdr.mBodyLen);
        }
    }
    return true;
}