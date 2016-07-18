#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;


	class EquipManager : public MainThread ,public Singleton<EquipManager>
	{
	public:
		EquipManager();
		virtual ~EquipManager();

		bool initial(Player* player);
		void release();

		std::string convertEquipStateToJson();

	private:
		
		Player* m_pPlayer;

		List<ItemData>* m_pListItemDatas;
		
		//×°±¸UID µÄmap
		Map<UInt64, ItemData*> *m_pMapUidEquipType;

	};
};

#endif //__LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_MAMAGER_H__ 
