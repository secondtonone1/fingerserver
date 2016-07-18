#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class EquipManager : public MainThread
    {
    public:
        EquipManager();
        virtual ~EquipManager();

        bool initial(Player* player);
        void release();
        void calculateEquipData(UInt8 equipPresentPos);

    public:
        // 检查装备栏是否满格
        bool isEquipFull();
        bool isEquipFull(List<TemplateId>& equipList);
        bool isEquipFull(List<AwardLine>& awardList);
        // 获得背包装备栏空格子数
        UInt8 getEquipFieldSpaceSlot();
        // 锁定符文
        void lockEquip(const Guid& equipId);
        // 解锁符文
        void unLockEquip(const Guid& equipId);

        // 摧毁装备
        void destroyEquip(Guid equipId);
        // 摧毁全部装备
        void destroyAllEquip();

        // 使用装备
        ErrorId useEquip(Guid equipId, UInt8 targetPos);
        // 添加装备
        const EquipData* createEquip(TemplateId equipTemplateId, UInt8 source, bool promp = true);
        // 设置装备强化等级
        void setEquipRefine(const Guid& equipId, UInt32 level);
        // 装备附魔
        ErrorId enchantEquip(const Guid& equipId, UInt32& enchantLevel);
        // 强化装备
        ErrorId refineEquip(const Guid& equipId, UInt32& refineLevel);
        // 自动强化装备
        ErrorId autoRefineEquip(const Guid& equipId, UInt32& refineLevel);

        const EquipData* getEquipDataByGuid(const Guid& equipId);

        // 熔炼接口
        ErrorId smeltEquip(const Guid& equipId, UInt32& gold, UInt32& soul);

        // 重生接口
        ErrorId rebornEquip(const Guid& equipId, UInt32& gold, List<Guid>& itemList);

        // 设置装备出场位置
        void setEquipPresentPos(const Guid& equipId, UInt8 pos);

        ErrorId sellEquip(List<Guid>& equipList, UInt32& getGold);
     
    private:
        Player* mPlayer;
        List<EquipData>* mEquipList;
        Map<Guid, EquipData*> mEquipMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_MAMAGER_H__
