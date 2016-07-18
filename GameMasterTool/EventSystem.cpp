#include "EventSystem.h"

using namespace Lynx;


EventSystem::EventSystem() : mStreamBuffer(8192 - sizeof(StreamBuffer::Node), 5)
                           , mPendingHandleFlag(false)
{
    
}

EventSystem::~EventSystem()
{
    
}

bool
EventSystem::initial()
{
    for (Int32 i = 0; i < LYNX_MAX_MSG_ID + 1; ++i)
    {
        mHandlerArray[i] = NULL;
    }
    mStreamBuffer.clear();
    return true;
}

void
EventSystem::release()
{
    for (Int32 i = 0; i < LYNX_MAX_MSG_ID + 1; ++i)
    {
        if (mHandlerArray[i])
        {
            XDELETE(mHandlerArray[i]->mFunc);
        }
        XDELETE(mHandlerArray[i]);
    }
    mStreamBuffer.clear();
}

bool
EventSystem::update()
{
    while (1)
    {
        if (mPendingHandleFlag == false)
        {
            if (!mMsgHdr.unserialize(mStreamBuffer))
            {
                break;
            }
        }

        if (mMsgHdr.mBodyLen > mStreamBuffer.length())
        {
            mPendingHandleFlag = true;
            break;
        }

        mPendingHandleFlag = false;

        if (mMsgHdr.mId >= LYNX_MAX_MSG_ID)
        {
            LOG_WARN("Msg id overflow: id=%u, len=%u", mMsgHdr.mId, mMsgHdr.mBodyLen);
            return false;
        }

        HandlerWrapper* handler = mHandlerArray[mMsgHdr.mId];
        if (handler)
        {
            if (!handler->mHandlerWrapper(mStreamBuffer, handler->mFunc))
            {
                LOG_WARN("Failed to call handler wrapper, msgid=%u, msglen=%u",
                    mMsgHdr.mId, mMsgHdr.mBodyLen);
                return false;
            }
        }
        else
        {
            LOG_WARN("Don't match this message, msgid=%u, msglen=%u", 
                mMsgHdr.mId, mMsgHdr.mBodyLen);
            mStreamBuffer.drain(mMsgHdr.mBodyLen);
        }

    }
    return true;
}

