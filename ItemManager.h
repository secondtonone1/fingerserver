#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_ITEM_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_ITEM_MAMAGER_H__

#include "MainThread.h"
namespace Lynx
{
struct polishStoneEle
	{
		UInt64 uid;
		UInt32 count;
	};

    class Player;
	class EquipManager;
	class ItemManager : public MainThread,public Singleton<ItemManager>
    {
    public:
        ItemManager();
        virtual ~ItemManager();

        bool initial(Player* player);
        void release();
      
		friend class EquipManager;
    public:
  
        //背包物品是否占满
		bool isItemFull(UInt32 bagTab);
		//空背包个数
		UInt32 getEmputyNum(UInt32 bagTab);
		// 销毁道具, 这堆道具全部销毁
        void destroyItem(Guid itemId);
        // 删除道具，数量为0表示全部删除
        UInt32 delItem(const Guid itemUId, UInt32 count = 0);

		void delOneItem(ItemData* itemData);

		//根据物品的标签页，从对应的类型map里删除该物品，并且做和该类物品相关的统计操作
		void delTypeMapEle(ItemData* itemData);

		//根据物品的标签页，从对应的类型map里添加该物品，并且做和该物品的统计操作以及该类型属性和数据的生成
		void addTypeMapEle(ItemData* itemData);

		UInt32 getRandomValue(UInt32 minValue, UInt32 maxValue);

        // 获得道具通过GUID
         ItemData* getItemByGuid(const Guid& itemGuid);

        // 使用道具
        errorId useItem(const Guid& itemUId, UInt32 count);
        // 放入道具
		//原游戏
       // void addItem(const ItemData& itemData, UInt8 source);
        // 新游戏获取物品,templateId 为itemid
		List<UInt64> getNewItems(Guid templateId, UInt32 count);
	
		List<UInt64> getNewItemsDefrent(List<IdAndCount> itemsList);

		List<UInt64> getNewItemsSame(Guid templateId, UInt32 count);

		//根据itemid添加一个格子的物品
		UInt64 addOneItem(Guid templateId, UInt32 StackAmount);

		
		//更新某个物品的信息
		void updateOneItem(ItemData* itemData, UInt32 StackAmount);
		
		//添加 count 个 itemid 的物品
		List<UInt64> addItems(Guid templateId, UInt32 count);

		//获取非空的背包格数量
		UInt32 getNoneEmptyBags(UInt32 bagTap);

		//计算占用几个背包格
		UInt32 calculateCount(Guid templateId, UInt32 count);

		List<ItemData>* getItemDatasList(void);

      
        errorId sellItem(const List<Guid>& itemList, UInt32& getGold);

		std::string convertItemDataToJson();

		void addEquipAttribute(ItemData* itemData);

		void addRandomAttribute(ItemData* itemData, UInt32 randomID);
		
		//装备强化
		void enhanceEquip(UInt64 equipID, List<UInt64> materialLists);

		UInt32 getEnhanceLevelLimit(UInt32 starLv);

		UInt32 getTotalEnhanceLv(UInt32 starLv, UInt32 curEnhanceLv);
		
		//洗练装备
		void  polishEquip(UInt64 objEquipUid, UInt64 materialEquipUid, UInt32 objRdAttrIndex);

		//恢复成洗练之前的原属性
		void  polishResetEquip(UInt64 objEquipUid);

		//获取新的随机属性在目标装备中的品质
		UInt32 getAttrQuality(const RandomAttribute &randomAttr, EquipTemplate * materialTemplate);

		UInt32 getPAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr );

		UInt32 getMAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr );

		UInt32 getPFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMaxHPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMaxMPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMaxSPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getCritRateQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getCirtDamageQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getBaseToughnessQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		//创建品质map

		void creatQualityMap(ItemData * materialItemData,AttrPolishTemplate *materialPolishTem,bool& orangeFlag,Map<UInt32, RandomAttribute*> &orangeRdMap,
			UInt32& orangeRate, bool& purpleFlag,Map<UInt32, RandomAttribute*> &purpleRdMap,UInt32& purpleRate,
			Map<UInt32, RandomAttribute*> &totalRdMap,UInt32& totalRate);
		
		//金色属性必成功替换逻辑

		void orangeSuccess(Map<UInt32, RandomAttribute*> &orangeRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &orangeRate);

		//紫色属性必成功替换逻辑

		void purpleSuccess(Map<UInt32, RandomAttribute*> &purpleRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &purpleRate);

		//白绿蓝紫金属性随机替换逻辑

		void totalAttrRandom(Map<UInt32, RandomAttribute*> &totalRdMap,ItemData * objectItemData,RandomAttribute * &materialRdFind,UInt32 &totalRate);
		
		void gemCombine(UInt64 gem1Uid, UInt64 gem2Uid);

		void gemCombineOnce(void);

		//宝石镶嵌在装备上
		void gemSetEquip(UInt64 equipUid,UInt32 equipGemSlot, UInt64 gemUid);

		//宝石卸下, 返回卸下后新的宝石uid，如果为0，则表示卸下失败
		UInt64 unload(ItemData * equipData, UInt32 gemIndex);

		//装备宝石合成
		void equipGemCombine(UInt64 equipUid, UInt32 gemSlot);

		//合成多个宝石
		List<UInt64> combineGemsList(UInt64 gemItemID, UInt32 count);

		void combineGems( List<UInt64> &gemUidList, List<UInt64> &newGemList,GemCombineTemplate * gemCombineTemplate,const UInt32 &newGemCount, UInt32 & count);

		void constructGemList(List<UInt64> &uidList,ItemTemplate * gemTemplate, List<UInt64> &gemUidList);

		void sendCombineRes(List<UInt64> &newGemList, bool success = true, errorId errorId = LynxErrno::None);

		void equipGemUnload(UInt64 equipUid, UInt32 gemSlot);

  private:
	

        Player* mPlayer;
        List<ItemData>* m_pListItemDatas;
        Map<Guid, ItemData*> m_mapItemUuid;
		//方便从ItemID查找uuid,判断背包中是否存在此物品，以及物品满格的情况
	    Map<Guid,List<Guid> > m_mapItemId2Uids;
		
		//装备UID 的map
		Map<UInt64, ItemData*> m_mapUidEquipType;

		//非装备的物品,消耗品UID map
		Map<UInt64, ItemData*> m_mapUidItemType;

		//其他类型的UID map 目前没用到
		Map<UInt64, ItemData*> m_mapUidOtherType;
		
		//装备或其他物品占有的第一个标签页背包格个数
		UInt32 m_nEquipOccupies;

		//道具占有的第二个标签页背包格个数
		UInt32 m_nItemOccupies;

		//其他物品占用的第三个背包格个数

		UInt32 m_nOtherOccupies;
		
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_ITEM_MAMAGER_H__
