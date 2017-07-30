#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_CHAT_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_CHAT_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;

	
	class ChatManager : public MainThread ,public Singleton<ChatManager>
	{
	public:
		ChatManager();
		virtual ~ChatManager();

		bool initial(Player* player);
		void release();

		void insertPlayerToChatList(UInt64 playerUid);

		void checkChatListFull();

		void setBlackList(UInt64 playeruid);

		void addFriendList(UInt64 playeruid);

		void showDetailInfo(UInt64 playeruid);

		std::string convertDataToJson();

		void receiveMsg(UInt32 channelType, UInt64 fromPlayer, UInt64 toPlayer, std::string ChatMsg);

		void clientForbidChat(UInt64 playerUid, UInt32 timelong, UInt64 timebegin);

		void clientResetChat(UInt64 playerUid);

	

		//GM发送的公告信息
		void sendGMNotifyMsg(std::string msg, UInt64 dealline,  UInt64 terminal);

		void checkOnline(UInt64 playerUid);

	private:
		
		Player* m_pPlayer;
		List<ChatData> * m_pChatList;


		

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_CHAT_MAMAGER_H__