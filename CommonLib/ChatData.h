#ifndef __LYNX_COMMON_LIB_CHAT_DATA_H__
#define __LYNX_COMMON_LIB_CHAT_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct ChatData
	{
		ChatData() : mChannel(0), mSenderGuid(0), mReceiverGuid(0), mSenderName(""), 
            mReceiverName(""), mSenderVipLevel(0), mContents(""), mSenderIcon(""),
            mReceiverIcon(""), mSenderLevel(0), mSenderBattleValue(0) {}

        UInt8 mChannel;                     // 0 世界， 1 公会， 2 私聊
        Guid mSenderGuid;                   // 发送玩家GUID, 如果是系统填0
        Guid mReceiverGuid;                 // 频道为0该值无意义， 频道为1表示目标公会GUID, 频道为2表示目标玩家GUID
		NameString mSenderName;             // 发送者名称，客户端不需要填写
        IconString mSenderIcon;             // 发送者头像
		NameString mReceiverName;           // 接受者名称，客户端需要填写
        IconString mReceiverIcon;           // 接受者头像
        UInt8 mSenderVipLevel;              // 发送者VIP, 客户端不需要填写
        UInt32 mSenderLevel;                // 发送者等级
        UInt32 mSenderBattleValue;          // 发送者战力
		String mContents;                   // 聊天内容，最大80个字节

		LYNX_S11N_11(ChatData, UInt8, mChannel, NameString, mSenderName, Guid, mSenderGuid, 
            Guid, mReceiverGuid, UInt8, mSenderVipLevel, NameString, mReceiverName, String, mContents,
            IconString, mSenderIcon, IconString, mReceiverIcon, UInt32, mSenderLevel, UInt32, mSenderBattleValue);
	};

#if 1
    typedef Map<UInt32, ChatData> ChatHistoryMap;

    struct WorldChatHistoryData
    {
        ChatHistoryMap mChatHistoryMap;

        LYNX_S11N_1(WorldChatHistoryData, ChatHistoryMap, mChatHistoryMap);
    };

    struct PlayerChatHistoryData
    {
        PlayerChatHistoryData() : mLastReadWorldChatId(0), mLastReadGuildChatId(0), mLastReadPlayerChatId(0) {}

        UInt32 mLastReadWorldChatId;
        UInt32 mLastReadGuildChatId;
        UInt32 mLastReadPlayerChatId;
        ChatHistoryMap mChatHistoryMap;

        LYNX_S11N_4(PlayerChatHistoryData, UInt32, mLastReadWorldChatId,
            UInt32, mLastReadGuildChatId, UInt32, mLastReadPlayerChatId,
            ChatHistoryMap, mChatHistoryMap);
    };

    struct GuildChatHistoryData
    {
        ChatHistoryMap mChatHistoryMap;

        LYNX_S11N_1(GuildChatHistoryData, ChatHistoryMap, mChatHistoryMap);
    };

    typedef Map<Guid, PlayerChatHistoryData> PlayerChatHistoryDataMap;
    typedef Map<Guid, GuildChatHistoryData> GuildChatHistoryDataMap;

    struct ChatHistoryData
    {
        WorldChatHistoryData mWorldChatHistoryData;
        PlayerChatHistoryDataMap mPlayerChatHistoryDataMap;
        GuildChatHistoryDataMap mGuildChatHistoryDataMap;

        LYNX_S11N_3(ChatHistoryData, WorldChatHistoryData, mWorldChatHistoryData,
            PlayerChatHistoryDataMap, mPlayerChatHistoryDataMap,
            GuildChatHistoryDataMap, mGuildChatHistoryDataMap);

    };
#endif
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_CHAT_DATA_H__


