#ifndef __LYNX_CORE_LIB_MSGHANDLER_H__ 
#define __LYNX_CORE_LIB_MSGHANDLER_H__

#include "Logger.h"
#include "Delegate.h"
#include "MsgHdr.h"
#include "TcpConnection.h"
#include "ErrorCode.h"

namespace Lynx
{
    class MsgHandler
    {
    public:
        MsgHandler() : mDefaultHandler(NULL)
        {
            for (Int32 i = 0; i < LYNX_MAX_MSG_ID; i++)
            {
                mHandlerWrapperArray[i] = NULL;
            }
        }

        virtual ~MsgHandler() 
        { 
            XSAFE_DELETE(mDefaultHandler);
            for (Int32 i = 0; i < LYNX_MAX_MSG_ID; i++)
            {
                XSAFE_DELETE(mHandlerWrapperArray[i]);
            }
        }

        void clean()
        {
            if (mDefaultHandler)
            {
                XSAFE_DELETE(mDefaultHandler);
            }
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
            LYNX_REGISTER_RECEIVED(conn, this, &MsgHandler::onReceived);
        }

        template <typename ConnType >
        void registerDefaultHandler(Delegate<void (ConnType&, MsgHdr&, StreamBuffer&)>* func)
        {
            if (mDefaultHandler)
            {
                XSAFE_DELETE(mDefaultHandler);
            }

            mDefaultHandler = XNEW(HandlerWrapper)();
            ASSERT(mDefaultHandler);
            mDefaultHandler->mFunc = (char *)func;
            mDefaultHandler->mRecvWrapper = &defaultHandlerWrapper<ConnType >;
        }

		template <typename ConnType>
		void unregisterDefaultHandler()
		{
			if (!mDefaultHandler)
			{
				return;
			}

			if (mDefaultHandler->mFunc)
			{
				XSAFE_DELETE((Delegate<void (ConnType, MsgHdr&, StreamBuffer&)>*)mDefaultHandler->mFunc);
			}
			XSAFE_DELETE(mDefaultHandler);
		}

        template <typename ConnType, typename MsgType >
        void registerHandler(const Delegate<void (ConnType&, MsgType&)>& func)
        {
			Delegate<void (ConnType&, MsgType&)>* pFunc = XNEW(Delegate<void (ConnType&, MsgType&)>)();
			*pFunc = func;
            HandlerWrapper* handler = XNEW(HandlerWrapper)();
            ASSERT(handler);
            handler->mFunc = (char *)pFunc;
            handler->mIsStatic = false;
            handler->mRecvWrapper = &RecvWrapper<ConnType, MsgType >;
            UInt16 msgId = MessageId<MsgType >::sMsgId;
            ASSERT(msgId < LYNX_MAX_MSG_ID);
            ASSERT(!mHandlerWrapperArray[msgId]);
            mHandlerWrapperArray[msgId] = handler;
        }

        template <typename ConnType, typename MsgType >
        void registerHandler(void (*func)(ConnType&, MsgType&))
        {
            HandlerWrapper* handler = XNEW(HandlerWrapper)();
            ASSERT(handler);
            handler->mFunc = (char *)func;
            handler->mIsStatic = true;
            handler->mRecvWrapper = &RecvWrapper<ConnType, MsgType >;
            UInt16 msgId = MessageId<MsgType >::sMsgId;
            ASSERT(msgId < LYNX_MAX_MSG_ID);
            ASSERT(!mHandlerWrapperArray[msgId]);
            mHandlerWrapperArray[msgId] = handler;
        }

		template <typename ConnType, typename MsgType >
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
					XDELETE((Delegate<void (ConnType&, MsgType&)>*)handler->mFunc);
				}
				XSAFE_DELETE(mHandlerWrapperArray[msgId]);
			}
		}

        template <class MsgType >
        static void s11nMsg(const MsgType& msg, StreamBuffer& streamBuffer)
        {
            MsgHdr hdr;
            hdr.mId = MessageId<MsgType >::sMsgId;
            hdr.mBodyLen = serializeSize(msg, streamBuffer);
            if (!hdr.serialize(streamBuffer))
            {
                LOG_WARN("Failed to serialize msg hdr.");
                return;
            }
            serialize(msg, streamBuffer, hdr.mId);
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

        void sendMsg(TcpConnection& conn, const StreamBuffer& streamBuffer)
        {
            conn.mOStream += streamBuffer;
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
                }
                else
                {
                    if (mDefaultHandler)
                    {
                        mDefaultHandler->mRecvWrapper(mDefaultHandler->mIsStatic, 
                            mDefaultHandler->mFunc, *conn, *hdr);
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
        }

    protected:
        template <typename ConnType >
        static bool defaultHandlerWrapper(bool isStatic, char* func, TcpConnection& conn, MsgHdr& hdr)
        {
            if (isStatic)
            {
                ((void (*)(ConnType&, MsgHdr&, StreamBuffer&))func)((ConnType&)conn, hdr, conn.mIStream);
            }
            else
            {
                ((Delegate<void (ConnType&, MsgHdr&, StreamBuffer&)>*)func)->operator () 
                    ((ConnType&)conn, hdr, conn.mIStream);
            }
            return true;
        }

        template <typename ConnType, typename MsgType >
        static bool RecvWrapper(bool isStatic, char* func, TcpConnection& conn, MsgHdr& hdr)
        {
            MsgType msg; 

            if (!unserialize(msg, conn.mIStream, hdr.mId)) 
            {
                return false;
            }

            if (isStatic)
            {
                ((void (*)(ConnType&, MsgType&))func)((ConnType&)conn, msg);
            }
            else
            {
                ((Delegate<void (ConnType&, MsgType&)>*)func)->operator () 
                    ((ConnType&)conn, msg);
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

        HandlerWrapper* mDefaultHandler;
        HandlerWrapper* mHandlerWrapperArray[LYNX_MAX_MSG_ID+1];
    };
} // namespace Lynx

#endif // __LYNX_CORE_LIB_MSGHANDLER_H__
