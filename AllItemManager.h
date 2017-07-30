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
		void  addAwardMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count, ReturnItemEle& rtItemEle,UInt32 systemID);

		UInt32 getAwardCount(UInt32 resType, UInt32 subTypeID);

		//将返回列表中的物品拼成字符串
		void convertItemListToStr(const List<ReturnItemEle> & rtItemEleList, Json::Value & root);

		void addAwardMaterialsList(const List<AwardData> & awardList,UInt32 systemID);

		//消耗物品逻辑
		void costMaterialsList(const List<AwardData> & awardList,UInt32 systemID);

		bool costMaterial(UInt32 resType, UInt32 subTypeID, UInt32 count,UInt32 systemID);

		
		void addAwardGetSendJson(const List<AwardData> & awardList,UInt32 systemID);

		private:
		

		Player* m_pPlayer;

		

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_AWARD_MAMAGER_H__