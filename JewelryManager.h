#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_JEWELRY_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_JEWELRY_MAMAGER_H__


#include "MainThread.h"
#include "../FireConfirm/Box.h"

namespace Lynx
{
	class Player;


	class JewelryManager : public MainThread,  public Singleton<JewelryManager>
	{
	public:
		JewelryManager();
		virtual ~JewelryManager();

		bool initial(Player* player);
		void release();

		void jewelryPolish(UInt64 objJewelryUid, UInt64 materialJewelryUid, UInt32 objRdAttrIndex);

		void jewelryReset(UInt64 objJewelryUid );

		bool playerCostJewelry( UInt64 jewelryUid);

		List<JewelryData *> playerGetJewelry(const UInt64 &jewelryId, const UInt32 &count);

		void jewelrySet(UInt32 jewelryPos, UInt64 jewelryUid);

		void jewelryUnload(UInt32 jewelryPos);

		void jewelrySell(List<UInt64> sellList);

		std::string convertDataToJson();

		void checkLvAcitve();

		bool getJewelrySlotOpenState(UInt32 jewelrySlot);

		//勿改
		void jewelryUpdateToClient(List<JewelryData*> getList);

		//勿改
		Json::Value getJewelryJsonNode(Json::Value &jewelryRoot,JewelryData* jewelryData);

		//程庆银掉落发包
		void diaoluoSend(List<JewelryData *> getList);

		void diaoluoSendTest(List<JewelryData *> getList);

		void jewelryEquipOnce(Map<UInt32 ,UInt64> equipMap);

		UInt32 getJewelryCount();

	private:

		void creatQualityMap(JewelryData * materialJewelryData,AttrPolishTemplate *materialPolishTemp,bool& orangeFlag,
			Map<UInt32, JewelryAttr*> &orangeRdMap,
			UInt32& orangeRate, bool& purpleFlag,Map<UInt32, JewelryAttr*> &purpleRdMap,
			UInt32& purpleRate,
			Map<UInt32, JewelryAttr*> &totalRdMap,UInt32& totalRate);

		void orangeSuccess(Map<UInt32, JewelryAttr*> &orangeRdMap,JewelryData * objectItemData,JewelryAttr * &materialRdFind,UInt32 &orangeRate);

		void purpleSuccess(Map<UInt32, JewelryAttr*> &purpleRdMap,JewelryData * objectItemData,JewelryAttr * &materialRdFind,UInt32 &purpleRate);
		
		void totalAttrRandom(Map<UInt32, JewelryAttr*> &totalRdMap,JewelryData * objectItemData,JewelryAttr * &materialRdFind,UInt32 &totalRate);

		UInt64 getNewAttrId(const JewelryAttr &randomAttr, NewJewelryTemplate * objectTemplate);

		void postUpdateMsg(JewelryData * jewelryData, UInt64 playerUid);

		void postAddMsg(JewelryData * jewelryData, UInt64 playerUid);

		void postDelMsg(UInt64 jewelryUid, UInt64 playerUid);

		void postJewelryEquipMsg();

		UInt32 getRandomValue(UInt32 minValue, UInt32 maxValue);

		JewelryData * addJewelrys(const UInt64 &jewelryId);

		bool delJewelrys(const UInt64& jewelryUid);

		void generateJewelryAttr(JewelryData * jewelryData, NewJewelryTemplate * newJewelryTemp);

		void generateRandomAttr(JewelryData * jewelryData, NewJewelryTemplate * newJewelryTemp);

		void addRandomAttribute(JewelryData* jewelryData, UInt32 randomID);

		//获取新的随机属性在目标装备中的randomattrid
		UInt64 getPARdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr );

		UInt64 getMARdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr );

		UInt64 getPFRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr);

		UInt64 getMFRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr);

		UInt64 getMaxHPRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr);

		UInt64 getMaxMPRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr);

		UInt64 getMaxSPRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr);

		UInt64 getCritRateRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr);

		UInt64 getCirtDamageRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr);

		UInt64 getBaseToughnessRdId(const UInt32 &jewelryLv, const JewelryAttr &randomAttr);

		Player* m_pPlayer;
		List<JewelryData>*  m_pJewelryList;
		Map<UInt64, JewelryData*> m_jewelryDataMap;

		JewelryEquipData * m_pJewelryEquipData;
		UInt64 m_nPlayerGuid;
	};
};

#endif //__LYNX_GAME_SERVER_GAME_MANAGER_JEWELRY_MAMAGER_H__
