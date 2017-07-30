#ifndef __LYNX_CORE_LIB_MSG_HDR_H__
#define __LYNX_CORE_LIB_MSG_HDR_H__

#include "ErrorCode.h"
#include "StreamBufferSerializeImpls.h"

#define LYNX_MAX_MSG_ID               32767 // 2^16
#define LYNX_MAX_MSG_LENGTH           (1024 * 1024) 

namespace Lynx
{
	struct MsgHdr
	{
		MsgHdr() : mId(0), mBodyLen(0), mAckId(0) {}

        UInt16 mId;
		UInt32 mAckId;
		UInt32 mBodyLen;
		
		bool serialize(StreamBuffer& ostream)
		{
			if (mId >= LYNX_MAX_MSG_ID)
			{
				LOG_WARN("MessageId %u overflow.", mId);
				return false;
			}

			if (mBodyLen >= LYNX_MAX_MSG_LENGTH)
			{
				LOG_WARN("Msg len %u overflow", mBodyLen);
				return false;
			}

			ostream << mAckId;
			ostream << mId;
			ostream << mBodyLen;
			return true;
		}

		bool unserialize(StreamBuffer& istream)
		{
			if (istream.length() < sizeof(UInt32) + sizeof(UInt16) + sizeof(UInt32))
			{
				return false;
			}

		
			istream >> mAckId;
			istream >> mId;
			istream >> mBodyLen;

		

			return true;
		}
	}; 

    template <typename Type = void>
    struct MessageId 
    {
        static const UInt16 sMsgId = Type::sMsgId;
    };
} // namespace

#define LYNX_MESSAGE_0(msgid, classname) LYNX_S11N_0(classname)                                     \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_0(msgid, classname) LYNX_S11N_TRAITS_0(classname)                       \
namespace Lynx {template<> struct MessageId<classname > { static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_1(msgid, classname, member1type, member1name)                                   \
LYNX_S11N_1(classname, member1type, member1name)                                                     \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_1(msgid, classname, member1type, member1name)                            \
LYNX_S11N_TRAITS_1(classname, member1type, member1name)                                              \
namespace Lynx { template <> struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_2(msgid, classname, member1type, member1name, member2type, member2name)         \
LYNX_S11N_2(classname, member1type, member1name, member2type, member2name)                           \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_2(msgid, classname, member1type, member1name, member2type, member2name)  \
LYNX_S11N_TRAITS_2(classname, member1type, member1name, member2type, member2name)                    \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_3(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name)                                                                             \
LYNX_S11N_3(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name)                                                                             \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_3(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name)                                                                             \
LYNX_S11N_TRAITS_3(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name)                                                                             \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_4(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name)                                                   \
LYNX_S11N_4(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name)                                                   \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_4(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name)                                                   \
LYNX_S11N_TRAITS_4(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name)                                                   \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_5(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name, member5type, member5name)                         \
LYNX_S11N_5(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name, member5type, member5name)                         \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_5(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name, member5type, member5name)                         \
LYNX_S11N_TRAITS_5(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name, member5type, member5name)                         \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_6(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name)                                                                                          \
LYNX_S11N_6(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name)                                                                                          \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_6(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name)                                                                                          \
LYNX_S11N_TRAITS_6(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name)                                                                                          \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_7(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name)                                                                \
LYNX_S11N_7(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name)                                                                \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_7(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name)                                                                \
LYNX_S11N_TRAITS_7(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name)                                                                \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_8(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name)                                      \
LYNX_S11N_8(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name)                                      \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_8(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name)                                      \
LYNX_S11N_TRAITS_8(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name)                                      \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_9(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name)            \
LYNX_S11N_9(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name)            \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_9(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name)            \
LYNX_S11N_TRAITS_9(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name)            \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_10(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name)                                                                           \
LYNX_S11N_10(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name)                                                                           \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_10(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name)                                                                           \
LYNX_S11N_TRAITS_10(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name)                                                                           \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_11(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name)                                               \
LYNX_S11N_11(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name)                                               \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_11(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name)                                               \
LYNX_S11N_TRAITS_11(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name)                                               \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_12(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name)                   \
LYNX_S11N_12(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name)                   \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_12(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name)                   \
LYNX_S11N_TRAITS_12(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name)                   \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_13(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name)                                                                           \
LYNX_S11N_13(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name)                                                                           \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_13(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name)                                                                           \
LYNX_S11N_TRAITS_13(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name)                                                                           \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_14(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name)                                               \
LYNX_S11N_14(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name)                                               \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_14(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name)                                               \
LYNX_S11N_TRAITS_14(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name)                                               \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_15(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name)                   \
LYNX_S11N_15(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name)                   \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_15(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name)                   \
LYNX_S11N_TRAITS_15(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name)                   \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_16(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name)                                                                                         \
LYNX_S11N_16(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name)                                                                                         \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_16(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name)                                                                                         \
LYNX_S11N_TRAITS_16(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name)                                                                                         \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_17(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name)                                                             \
LYNX_S11N_17(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name)                                                             \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_17(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name)                                                             \
LYNX_S11N_TRAITS_17(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name)                                                             \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_18(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name)                                 \
LYNX_S11N_18(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name)                                 \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_18(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name)                                 \
LYNX_S11N_TRAITS_18(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name)                                 \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_19(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name)     \
LYNX_S11N_19(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name)     \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_19(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name)     \
LYNX_S11N_TRAITS_19(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name)     \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_20(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name)                                                                           \
LYNX_S11N_20(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name)                                                                           \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_20(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name)                                                                           \
LYNX_S11N_TRAITS_20(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name)                                                                           \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_21(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name)                                               \
LYNX_S11N_21(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name)                                               \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_21(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name)                                               \
LYNX_S11N_TRAITS_21(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name)                                               \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_22(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name)                   \
LYNX_S11N_22(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name)                   \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_22(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name)                   \
LYNX_S11N_TRAITS_22(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name)                   \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_23(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name)                                                                                         \
LYNX_S11N_23(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name)                                                                                         \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_23(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name)                                                                                         \
LYNX_S11N_TRAITS_23(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name)                                                                                         \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_24(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name)                                                             \
LYNX_S11N_24(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name)                                                             \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_24(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name)                                                             \
LYNX_S11N_TRAITS_24(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name)                                                             \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#define LYNX_MESSAGE_25(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name, member25type, member25name)                                 \
LYNX_S11N_25(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name, member25type, member25name)                                 \
static const Lynx::UInt16 sMsgId = msgid

#define LYNX_MESSAGE_TRAITS_25(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name, member25type, member25name)                                 \
LYNX_S11N_TRAITS_25(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name, member25type, member25name)                                 \
namespace Lynx {template<>  struct MessageId<classname > {static const Lynx::UInt16 sMsgId = msgid;};}

#endif // __LYNX_CORE_LIB_MSG_HDR_H__
