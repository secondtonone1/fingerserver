#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_FRIENDBLACK_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_FRIENDBLACK_MAMAGER_H__


#include "MainThread.h"
#include "DBInterface.h"

namespace Lynx
{
	class Player;
	class BaseInfoData;

	struct FriendBlackInfo
	{
		String friendstr ;
		String blackstr; 
		UInt32 friendCount ;
		UInt64 playerUid;
	};

	class FriendBlackManager : public MainThread ,public Singleton<FriendBlackManager>
	{
	public:
		FriendBlackManager();
		virtual ~FriendBlackManager();

		bool initial(Player* player);
		void release();

		List<UInt64> &getFriendList();

		UInt32 getFriendCount();

		UInt32 getBlackCount();
	
		List<UInt64> &getBlackList();

		bool delFriend(UInt64 playerUid);

		bool addFriend(UInt64 playerUid);

		bool delBlack(UInt64 playerUid);

		bool addBlack(UInt64 playerUid);

	
		bool getOtherFriendInfo(UInt64 playerUid, FriendBlackInfo & friendBlackInfo);

		bool getBaseInfoFromDb(UInt64 playerUid, BaseInfoData & baseDataInfo);

		std::string convertDataToJson();

		bool judgeFriend(UInt64 playerUid);

		bool judgeBlack(UInt64 playerUid);
		

	private:

		void notifySaveDb(UInt64 playerUid);

		Player* m_pPlayer;
		FriendData *m_pFriendData;
		List<UInt64> *m_pFriendList;
		List<UInt64> * m_pBlackList;
		DBInterface mDBInterface;
		UInt64 m_nSelfUid;
	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_FRIENDBLACK_MAMAGER_H__