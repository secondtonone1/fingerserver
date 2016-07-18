#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_GM_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_GM_MAMAGER_H__


#include "MainThread.h"
#include "DBInterface.h"

namespace Lynx
{
	class Player;


	class GMManager : public MainThread ,public Singleton<GMManager>
	{
	public:
		GMManager();
		virtual ~GMManager();

		bool initial(Player* player);
		void release();

		void dealGMRequest(UInt64 etype, UInt64 param1, UInt64 param2, std::string param3, std::string param4);

		void dealLvAdd(UInt64 lv);

		void dealVipAdd(UInt64 lv);

		void dealCoinChange(UInt64);

		void dealGoldChange(UInt64);

		//改体力值
		void dealStrengthChange(UInt64);

		//修改美食屋
		void dealfoodsChange(UInt64  flag);

		void dealRhymeSoulChange(UInt64 count);

		void dealRhymeGrassChange(UInt64 type, UInt64 count);

		void dealHoarStonePieceChange(UInt64 hoarStoneId, UInt32 pieceCount);

		void dealRhymeStep();

		void dealRhymeLvUp(UInt32 addLv);

		void dealGemCountSet(UInt64 gemId, UInt64 gemCount);

		void dealHoarStoneRuneSet(UInt64 runeId, UInt64 runeCount);

		void  GMsetServantPiece(UInt64 servantId, UInt64 servantCount);

		void GMsetServantEquip(UInt64 equipId, UInt64 equipCount);
		
		void GMsetServantFood(UInt64 foodType, UInt64 foodCount);

		void GMsetrechargeNum(UInt64 foodType, UInt64 foodCount);

		void GMEmailSend(UInt64 tempid, UInt64 toPlayer, std::string des, std::string contant);

		//十二宗宫重置
		void dealTwelvePalaceChange(UInt64 type, UInt64 count);

		void dealTwelvePalaceHaoJiaoChange();

		void dealMultipleCopyChange(UInt64 type, UInt64 count);

		void dealWelfaleMarketChange(UInt64 type, UInt64 count);

		void dealStageChange(UInt64 type, UInt64 count);

		void dealClimbTowerChange(UInt64 type, UInt64 count);

		
		
	private:

		Player* m_pPlayer;
		



	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_GM_MAMAGER_H__