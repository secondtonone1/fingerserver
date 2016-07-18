#ifndef __LYNX_GAME_SERVER_RANKSYSTEM_H__
#define __LYNX_GAME_SERVER_RANKSYSTEM_H__

#include "MainThread.h"

#include "Guild.h"

namespace Lynx
{
	class BaseInfoData;
	class RankSystem : public MainThread, public Singleton < RankSystem >
	{
	public:

		RankSystem();
		virtual ~RankSystem();

		bool initial();
		void release();

		void update(const UInt64& accTime);
	
		void createOnlineLvs(const List<BaseInfoData> &playerInfoList);

		void getOnlinePlayers(UInt32 srcLv, Player * selfPlayer,  List<BaseInfoData>&getList);

		void clearPowerRank();

		void creatPowerRank(UInt64 playerUid, UInt32 rank);

		std::string sendPowerRankJson(UInt64 playerUid);

	private:
		void getPlayerFromSet(const Set<UInt64> &playerSet, UInt32 count, Player * selfPlayer,   List<BaseInfoData> &getList);

		UInt64 m_nLastSyncTime;
		Map<UInt32, Set<UInt64> > m_onLineLvMap;
		List<UInt64> m_first50List;
		Map<UInt64, UInt32> m_powerRankMap;
		
		
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_RANKSYSTEM_H__

