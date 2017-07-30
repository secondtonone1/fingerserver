#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_GEM_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_GEM_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;
	class GemManager : public MainThread,public Singleton<GemManager>
	{
	public:
		GemManager();
		virtual ~GemManager();

		bool initial(Player* player);
		void release();

		void playerGetGems(UInt64 gemID, UInt32 count);

		bool playerConsumeGems(UInt64 gemID, UInt32 count);

		List<GemData> *  getGemDataList(void);

		//合成宝石函数，gemitemid为材料宝石id
		void combineGemsList(UInt64 gemItemID, UInt32 count);

		//合成宝石接口，gemitemid为目的宝石id
		void combineOneTypeGems(UInt64 gemItemID, UInt32 count);

		void gemCombineOnce(void);
 
		void gemCombineOnce(UInt32 equipPos);

		std::string convertDataToJson();

		GemData* getGemData(UInt64 gemItemID);

		Map<UInt32, GemData*>* getSameTypeGems(UInt32 gemType);

		UInt32 getGems(UInt64 gemID);//add by cheng

	private:

		UInt32 getGem(UInt64 gemID);//add by cheng

		GemData* addGems(UInt64 gemID, UInt32 count);

		bool delGems(UInt64 gemID, UInt32 count);

		void sendRespToClient(List<UInt64> &newGemList,  errorId  errorId);

		void addGemTypeBaseCount(UInt64 gemID, UInt32 count);
		
		void delGemTypeBaseCount(UInt64 gemID, UInt32 count);

		void createGemTypeLvToDataMap(UInt64 gemID, Map<UInt64, GemData*>::Iter* gemMapIter);

		void removeAllGems();

		Player* m_pPlayer;
		List<GemData> * m_pGemDataList;
		Map<UInt64, GemData*> m_gemDataMap;
		//同种类型的宝石，无论等级，根据配置表转化为1级宝石的数量
		Map<UInt32, UInt32> m_gemTypeToBaseCount;
		//宝石类型，宝石等级到宝石数据的map
		Map<UInt32, Map<UInt32, GemData*> > m_gemTypeLvMap;
	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_GEM_MAMAGER_H__