#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_FASHION_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_FASHION_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;


	class FashionManager : public MainThread,public Singleton<FashionManager>
	{
	public:
		FashionManager();
		virtual ~FashionManager();

		bool initial(Player* player);
		void release();

		std::string convertFashionDataToJson();

		void buyFashion(UInt64 fashionId);

		void addFashion(UInt64 fashionId, const ConnId &connId);

	private:
		
		Player* m_pPlayer;

		List<FashionData> * m_pListFashionDatas;

		Map<UInt64, FashionData* > m_mapIdToFsData;
	
	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_FASHION_MAMAGER_H__