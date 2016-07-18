#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_AWARD_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_AWARD_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;

	class AllItemManager : public MainThread ,public Singleton<AllItemManager>
	{
	public:
		AllItemManager();
		virtual ~AllItemManager();

		bool initial(Player* player);
		void release();

		std::string convertAllMaterialDataToJson();
		
		//resType 资源的类型，对应不同的表,ConstDefinition.h里有对应的类型宏
		//subTypeID 子类型，对应一类资源中不同的id，即不同配置表里的id
		//获得物品的数量
		void addAwardMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count);

		UInt32 getAwardCount(UInt32 resType, UInt32 subTypeID);

		void addAwardMaterialsList(const List<AwardData> & awardList);

		//消耗物品逻辑
		void costMaterialsList(const List<AwardData> & awardList);

		void costMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count);

		void checkExp(const  ConnId& connId);

		void levelUp(const  ConnId& connId,UInt32 level);

		void addAwardGetSendJson(const List<AwardData> & awardList);

		private:
		

		Player* m_pPlayer;

		

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_AWARD_MAMAGER_H__