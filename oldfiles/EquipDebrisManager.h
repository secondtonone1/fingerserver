#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_DEBRIS_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_DEBRIS_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class EquipDebrisManager : public MainThread
    {
    public:
        EquipDebrisManager();
        virtual ~EquipDebrisManager();

        bool initial(Player* player);
        void release();

    public:
        // 检查碎片栏是否满格
        bool isEquipDebrisFull();
        bool isEquipDebrisFull(List<TemplateId>& equipDebrisList);
        bool isEquipDebrisFull(List<AwardLine>& awardList);
        // 获得碎片栏空格子数
        UInt8 getEquipDebrisFieldSpaceSlot();
        // 锁定道具
        void lockEquipDebris(const Guid& itemId);
        // 解锁道具
        void unLockEquipDebris(const Guid& itemId);

        // 获得碎片
        const EquipDebrisData* getEquipDebrisByTemplateId(TemplateId templateId);
        const EquipDebrisData* getEquipDebrisByGuid(const Guid& guid);

        // 销毁碎片，这堆碎片全部销毁
        void destroyEquipDebris(Guid equipDebrisId);
        // 删除碎片, 数量为0表示全部删除
        void delEquipDebris(Guid equipDebrisId, UInt32 count = 0);
        // 清空背包
        void clearBag();

        // 放入碎片
        const EquipDebrisData*  createEquipDebris(TemplateId templateId, UInt32 count, UInt8 source, bool promp = true);

        // 合成
        ErrorId combineEquip(const Guid& equipDebrisId, Guid& equipId);
    private:
        // 使用碎片
        bool useEquipDebris(const Guid& guid, UInt32 count);
    private:
        Player* mPlayer;
        List<EquipDebrisData>* mEquipDebrisList;
        Map<Guid, EquipDebrisData*> mEquipDebrisDataMap;
        Map<TemplateId, EquipDebrisData*> mEquipDebrisTemplateMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_EQUIP_DEBRIS_MAMAGER_H__
