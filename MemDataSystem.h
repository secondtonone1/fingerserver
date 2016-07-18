#ifndef __LYNX_GAME_SERVER_MEMDATASYSTEM_H__
#define __LYNX_GAME_SERVER_MEMDATASYSTEM_H__

#include "MainThread.h"
#include "Player.h"
#include "Guild.h"
#include "Data.h"

namespace Lynx
{
	class MemDataSystem : public MainThread, public Singleton < MemDataSystem >
	{
	public:

		MemDataSystem();
		virtual ~MemDataSystem();

		bool initial();
		void release();

		FriendData* getPlayerFandData(UInt64 playerUid);
		
		FriendData * insertPlayerFandData(UInt64 playerUid, FriendData friendData);

	private:

		//好友和黑名单
		Map<UInt64,  FriendData  > m_mapPlayerFandBs;

	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_MEMDATASYSTEM_H__

