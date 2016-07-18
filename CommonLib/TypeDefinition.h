#ifndef __LYNX_COMMON_LIB_TYPE_DEFINITION_H__
#define __LYNX_COMMON_LIB_TYPE_DEFINITION_H__

#include "PlatformLib.h"

namespace Lynx
{
    enum GuidType
    {
        GUID_TYPE_ACCOUNT     = 1,
        GUID_TYPE_PLAYER      = 2,
        GUID_TYPE_HERO        = 3,
        GUID_TYPE_MONSTER     = 4,
        GUID_TYPE_PET         = 5,
        GUID_TYPE_ITEM        = 6,
        GUID_TYPE_EMAIL       = 7,
        GUID_TYPE_GUILD       = 8,
        GUID_TYPE_TOLLGATE    = 9,
    };

	// GUID
	// |------ServerId(12)------|----Type(4)----|---------Index(48)--------------|
	// |-----------------------------------64bit---------------------------------|
	typedef UInt64 Guid;

    #define GEN_GUID(serverId, type, index) (((((UInt64)(serverId)) << 52) & 0xFFF0000000000000ull) |\
        ((((UInt64)(type)) << 48) & 0x000F000000000000ull) |\
	    (((UInt64)(index)) & 0x0000FFFFFFFFFFFFull))

    #define GEN_ACCOUNT_GUID(serverId, index) (((((UInt64)(serverId)) << 52) & 0xFFF0000000000000ull) |\
        ((((UInt64)(GUID_TYPE_ACCOUNT)) << 48) & 0x000F000000000000ull) |\
        (((UInt64)(index)) & 0x0000FFFFFFFFFFFFull))

    #define GEN_PLAYER_GUID(serverId, index) (((((UInt64)(serverId)) << 52) & 0xFFF0000000000000ull) |\
       ((((UInt64)(GUID_TYPE_PLAYER)) << 48) & 0x000F000000000000ull) |\
       (((UInt64)(index)) & 0x0000FFFFFFFFFFFFull))

    #define GEN_HERO_GUID(serverId, index) (((((UInt64)(serverId)) << 52) & 0xFFF0000000000000ull) |\
       ((((UInt64)(GUID_TYPE_HERO)) << 48) & 0x000F000000000000ull) |\
       (((UInt64)(index)) & 0x0000FFFFFFFFFFFFull))

    #define GEN_MONSTER_GUID(serverId, index) (((((UInt64)(serverId)) << 52) & 0xFFF0000000000000ull) |\
        ((((UInt64)(GUID_TYPE_MONSTER)) << 48) & 0x000F000000000000ull) |\
        (((UInt64)(index)) & 0x0000FFFFFFFFFFFFull))

    #define GEN_PET_GUID(serverId, index) (((((UInt64)(serverId)) << 52) & 0xFFF0000000000000ull) |\
        ((((UInt64)(GUID_TYPE_PET)) << 48) & 0x000F000000000000ull) |\
        (((UInt64)(index)) & 0x0000FFFFFFFFFFFFull))

    #define GEN_ITEM_GUID(serverId, index) (((((UInt64)(serverId)) << 52) & 0xFFF0000000000000ull) |\
        ((((UInt64)(GUID_TYPE_ITEM)) << 48) & 0x000F000000000000ull) |\
        (((UInt64)(index)) & 0x0000FFFFFFFFFFFFull))

    #define GEN_EMAIL_GUID(serverId, index) (((((UInt64)(serverId)) << 52) & 0xFFF0000000000000ull) |\
        ((((UInt64)(GUID_TYPE_EMAIL)) << 48) & 0x000F000000000000ull) |\
        (((UInt64)(index)) & 0x0000FFFFFFFFFFFFull))

    #define GEN_GUILD_GUID(serverId, index) (((((UInt64)(serverId)) << 52) & 0xFFF0000000000000ull) |\
        ((((UInt64)(GUID_TYPE_GUILD)) << 48) & 0x000F000000000000ull) |\
        (((UInt64)(index)) & 0x0000FFFFFFFFFFFFull))

    #define GEN_TOLLGATE_GUID(serverId, index) (((((UInt64)(serverId)) << 52) & 0xFFF0000000000000ull) |\
        ((((UInt64)(GUID_TYPE_TOLLGATE)) << 48) & 0x000F000000000000ull) |\
        (((UInt64)(index)) & 0x0000FFFFFFFFFFFFull))

    #define GET_GUID_SERVER_ID(guid) (((UInt64(guid)) >> 52) & 0x0000000000000FFFull)
    #define GET_GUID_TYPE(guid)  (((UInt64(guid)) >> 48) & 0x000000000000000Full)
    #define GET_GUID_INDEX(guid) ((UInt64(guid)) & 0x0000FFFFFFFFFFFFull)

    // Á¬½ÓToken
    #define GEN_LOGIN_TOKEN(ip, port, index) ((((UInt64)((UInt32)(index) << 16 |\
        (UInt32)(ip) >> 16)) << 32) | ((UInt64)(((UInt32)(port) << 16) |\
        ((UInt32)((UInt32)(ip) << 16) >> 16))))

	typedef UInt16 MsgId;
	typedef UInt16 errorId;
	typedef UInt16 ServerId;
	typedef UInt32 SceneId;
	typedef UInt32 TemplateId;
	typedef UInt64 ConnId;
    typedef UInt32 HeroId;
    typedef UInt32 HeroClassId;
    typedef UInt32 DescripeId;
    typedef UInt32 SkillId;
    typedef UInt32 BuffId;
    typedef UInt32 NexusId;
    typedef UInt32 ConditionId;

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_TYPE_DEFINITION_H__