#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_MATERIAL_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_MATERIAL_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;


	class MaterialManager : public MainThread,  public Singleton<MaterialManager>
	{
	public:
		MaterialManager();
		virtual ~MaterialManager();

		bool initial(Player* player);
		void release();

		void addMaterial(UInt64 materialID, UInt32 count);

		bool delMaterial(UInt64 materialID, UInt32 count);

		List<MaterialData>* getMaterialList(void);

		UInt32 getMaterialCount(UInt64 materialID);

		MaterialData* getMaterialData(UInt64 materialID);
		
		std::string convertDataToJson(void);

		UInt32 getMaterial(UInt64 materialID);

	private:
		
		Player* m_pPlayer;

		List<MaterialData>* m_pMaterialDataList;

		
		Map<UInt64, MaterialData *> m_mapIdMaterialData;

		//Map<>
		

	};
};

#endif //__LYNX_GAME_SERVER_GAME_MANAGER_MATERIAL_MAMAGER_H__
