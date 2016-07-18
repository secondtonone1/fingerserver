#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_HEROEQUIP_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_HEROEQUIP_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;


	class HeroEquipManager : public MainThread, public Singleton<HeroEquipManager>
	{
	public:
		HeroEquipManager();
		virtual ~HeroEquipManager();

		bool initial(Player* player);
		void release();

		//当玩家升级的时候调用这个升级检测，检测装备解锁，宝石槽解锁
		void checkLvUpActive();

		//当玩家获得成就时，调用成就检查

		void checkAchievementActive();

		//当玩家手动激活某个宝石槽

		bool manualActive(UInt32 equipPos, UInt32 gemSlot);
		
		//宝石槽位从1到6，list取数值应该用该值减一
		bool getGemSlotOpenState(UInt32 equipPos, UInt32 gemSlotPos);

		bool getGemSlotOpenState(Map<UInt32, HeroEquipData *>::Iter * equipMapIter, UInt32 gemSlotPos);
		
		void enhanceEquip(UInt64 playerUid, UInt32 equipPos, UInt32 enhanceType);

		errorId enhanceLvUp(HeroEquipData& equipData, UInt32& playerLv, Map<UInt64, MaterialData*> &changeMaterialData, NewEquipLvTemplate * newEquipLvTemp);

		List<HeroEquipData> * getHeroEquipList(void);
		
		std::string convertDataToJson(void);

		void equipSetGem(UInt32 equipPos, UInt32 gemPos,UInt64 gemId);

		void sendSetRespToClient(errorId  errorId, UInt64 gemId, UInt32 equipPos, UInt32 equipGemSlot);

		void equipSetGemOnce(UInt32 equipPos);

		void sendSetOnceRespToClient(errorId errorId, Map<UInt32, GemData*>* sameGemMap = NULL, Map<UInt32, HeroEquipData *>::Iter * heroEquipData = NULL);

		void equipGemLvUp(UInt32 equipPos, UInt32 gemSlot);

		void equipGemUnload(UInt32 equipPos, UInt32 gemSlot);

		UInt64 changeCharacterEquip(UInt64 charactorId);

	private:

		void checkEquipActive(UInt32 equipPos, UInt32 heroModelId, Map<UInt32, HeroEquipData *>::Iter * equipMapIter, bool & changeFlag);

		void dealEquipActive(UInt32 heroModelId, Map<UInt32, HeroEquipData *>::Iter *equipMapIter, UInt64 equipId, UInt32 equipUnlockLv,bool & changeFlag );

		void initEquipGemBit(HeroEquipData &heroEquipData,  UInt32& level,UInt32 & condKey, UInt32 gemSlotBit);

		void checkGemActive(HeroEquipData &heroEquipData,bool & changeFlag);

		//处理等级升级时候，激活可以激活的宝石槽（不包含成就和金币，元宝手动解锁）
		void dealGemActive(HeroEquipData &heroEquipData, UInt32 dirtyBit, UInt32 gemUnlockLv, UInt32 gemCondKey,bool & changeFlag);

		//处理手动解锁的宝石槽（铜钱或者元宝或者解锁）
		void dealGemManualActive(HeroEquipData &heroEquipData, UInt32 dirtyBit, UInt32 gemUnlockLv, UInt32 gemCondKey,UInt32 gemCondValue,errorId & errorId);

		static bool lvCompare(UInt32 lv1, UInt32 lv2); 

		Player* m_pPlayer;
		//装备位置对应的装备信息,map key从1开始，最后到6
		Map<UInt32, HeroEquipData *> m_mapHeroEquipData;
		List<HeroEquipData> * m_pHeroEquipList;
		
		UInt32 *m_pHeroModel;
		UInt32 *m_pPlayerLv;
	
	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_HEROEQUIP_MAMAGER_H__
