#ifndef __LYNX_GAME_ROBOT_MSG_HANDLER_H__ 
#define __LYNX_GAME_ROBOT_MSG_HANDLER_H__

#include "Logger.h"
#include "Delegate.h"
#include "MsgHdr.h"
#include "TcpConnection.h"
#include "ErrorCode.h"
#include "CommonLib.h"

namespace Lynx
{
    class RobotMsgHandler
    {
    public:
        RobotMsgHandler() 
        {
            for (Int32 i = 0; i < LYNX_MAX_MSG_ID; i++)
            {
                mHandlerWrapperArray[i] = NULL;
            }
        }

        virtual ~RobotMsgHandler() 
        { 
            for (Int32 i = 0; i < LYNX_MAX_MSG_ID; i++)
            {
                XSAFE_DELETE(mHandlerWrapperArray[i]);
            }
        }

        void clean()
        {
            for (Int32 i = 0; i < LYNX_MAX_MSG_ID; i++)
            {
                if (mHandlerWrapperArray[i])
                {
                     XSAFE_DELETE(mHandlerWrapperArray[i]);
                }
            }
        }

        void initialize(TcpConnection* conn)
        {
            ASSERT(conn);
            LYNX_REGISTER_RECEIVED(conn, this, &RobotMsgHandler::onReceived);
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
            handler->mRecvWrapper = &RecvWrapper<MsgType >;
            UInt16 msgId = MessageId<MsgType >::sMsgId;
            ASSERT(msgId < LYNX_MAX_MSG_ID);
            ASSERT(!mHandlerWrapperArray[msgId]);
            mHandlerWrapperArray[msgId] = handler;
        }


		template <typename MsgType >
		void unregisterHandler()
		{
			UInt16 msgId = MessageId<MsgType >::sMsgId;
			if (msgId > LYNX_MAX_MSG_ID)
			{
				return;
			}
			HandlerWrapper* handler = mHandlerWrapperArray[msgId];
			if (handler)
			{
				if (!handler->mIsStatic)
				{
					XDELETE((Delegate<void (MsgType&)>*)handler->mFunc);
				}
				XSAFE_DELETE(mHandlerWrapperArray[msgId]);
			}
		}

        template <class MsgType >
        void sendMsg(TcpConnection& conn, const MsgType& msg)
        {
            MsgHdr hdr;
            hdr.mId = MessageId<MsgType >::sMsgId;
            hdr.mBodyLen = serializeSize(msg, conn.mOStream);

            if (conn.mMaxOutMsgLen && hdr.mBodyLen >= conn.mMaxOutMsgLen)
            {
                LOG_WARN("Invalid msg body len: id=%d, len=%d, maxmsglen: %d", 
                    hdr.mId, hdr.mBodyLen, conn.mMaxOutMsgLen);
                return;
            }
            if (!hdr.serialize(conn.mOStream))
            {
                LOG_WARN("Failed to serialize msg hdr.");
                return;
            }
            serialize(msg, conn.mOStream, hdr.mId);  
            conn.asyncSend();
        }

        void onReceived(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
        {
            MsgHdr* hdr = (MsgHdr*)conn->mUserData;
            if (hdr == NULL)
            {
                LOG_WARN("This isn't valid msg header for received");
                conn->close();
                return;
            }

            while (1)
            {
                if (conn->mMsgPending == false)
                {
                    if (!hdr->unserialize(istream))
                    {
                        break;
                    }

                    if (conn->mMaxInMsgLen && hdr->mBodyLen > conn->mMaxInMsgLen)
                    {
                        LOG_WARN("Invalidmsg body length: id=%d, len=%d, maxmsglen: %d", 
                            hdr->mId, hdr->mBodyLen, conn->mMaxInMsgLen);
                        conn->mErrno = LYNX_ERROR_MSGLEN;
                        conn->close();
                        break;
                    }
                }

                if (hdr->mBodyLen > istream.length())
                {
                    conn->mMsgPending = true;
                    break;
                }

                conn->mMsgPending = false;
                
                if (hdr->mId >= LYNX_MAX_MSG_ID)
                {
                    LOG_WARN("Msg id overflow: id=%d, len=%d", hdr->mId, hdr->mBodyLen);
                    conn->mErrno = LYNX_ERROR_MSGID;
                    conn->close();
                    break;
                }

                HandlerWrapper* handler = mHandlerWrapperArray[hdr->mId];
                if (handler)
                {
                    if (!handler->mRecvWrapper(handler->mIsStatic, handler->mFunc, *conn, *hdr))
                    {
                        LOG_WARN("Failed to unserialize, id=%d, len=%d", hdr->mId, hdr->mBodyLen);
                        conn->mErrno = LYNX_ERROR_S11N;
                        conn->close();
                        break;
                    }
                    else
                    {
                        if (hdr->mAckId)
                        {
                            MsgAckNotify ackNotify;
                            ackNotify.mAckId = hdr->mAckId;
                            sendMsg(*conn, ackNotify);
                        }
                    }
                }
                else
                {
                    LOG_WARN("Don't match this message, id=%d, len=%d", 
                        hdr->mId, hdr->mBodyLen);
                    conn->mErrno = LYNX_ERROR_MSGHDR;
                    conn->close();
                    break;
                }
            }
        }

    protected:
        template <typename MsgType >
        static bool RecvWrapper(bool isStatic, char* func, TcpConnection& conn, MsgHdr& hdr)
        {
            MsgType msg; 

            if (!unserialize(msg, conn.mIStream, hdr.mId)) 
            {
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
            bool (*mRecvWrapper)(bool isStatic, char*, TcpConnection&, MsgHdr&);
            char *mFunc;
            HandlerWrapper() : mRecvWrapper(NULL), mIsStatic(false), mFunc(NULL) {}
        };
        HandlerWrapper* mHandlerWrapperArray[LYNX_MAX_MSG_ID+1];
    };
} // namespace Lynx

#define REGISTER_MSG(handler, msgtype, func) \
    handler.registerHandler<msgtype>(Delegate<void (msgtype&)>(this, &func))

#define DEREGISTER_MSG(handler, msgtype) \
	handler.deregisterHandler<msgtype>()

#endif // __LYNX_GAME_ROBOT_MSG_HANDLER_H__
