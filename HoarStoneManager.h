#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_HOARSTONE_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_HOARSTONE_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;


	class HoarStoneManager : public MainThread, public Singleton<HoarStoneManager>
	{
		public:
			HoarStoneManager();
			virtual ~HoarStoneManager();

			bool initial(Player* player);
			void release();
			
			std::string convertDataToJson();

			void hoarStoneActive(UInt64 playerUid, UInt64 hoarstoneIdx);
            
			//添加界石碎片
			bool addHoarStonePiece(UInt64 hoarStoneId, UInt32 pieceCount);
           //设置界石碎片
			bool setHoarStonePiece(UInt64 hoarStoneId, UInt32 pieceCount);

			UInt32 getHoarStonePiece(UInt64 hoarStoneId);

			void hoarStoneLvUp(UInt64 hoarStoneId);

			void hoarStoneStarUp(UInt64 hoarStoneId);

			void addHoarStoneRune(UInt64 runeId, UInt32 count);

			UInt32 getHoarStoneRune(UInt64 runeId);

			void equipHoarStoneRune(List<runePosData> runeDataList, UInt64 hoarStoneID);

			void combinHoarStoneRune(UInt64 dstRuneId, UInt32 dstRuneCount, UInt32 transferId);

			HoarStoneData * getHoarStoneData(UInt64 hoarStoneID);

			void GMHoarStoneRuneSet(UInt64 runeId, UInt32 count);
	
		private:

			bool checkCondition(UInt64 playerUid);
			
			void sendStarResp(errorId errorId,const ConnId&);

			Player* m_pPlayer;
		
			List<HoarStoneData> * m_pHoarStoneList;

			Map<UInt64, HoarStoneData*> mHoarStoneMap;

			RuneDataMap * m_pRuneDataMap;
	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_HOARSTONE_MAMAGER_H__
