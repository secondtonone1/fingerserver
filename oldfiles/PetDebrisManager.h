#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_PET_DEBRIS_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_PET_DEBRIS_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class PetDebrisManager : public MainThread
    {
    public:
        PetDebrisManager();
        virtual ~PetDebrisManager();

        bool initial(Player* player);
        void release();

    public:
        // 检查宠物碎片栏是否满格
        bool isPetDebrisFull();
        bool isPetDebrisFull(List<TemplateId>& petDebrisList);
        bool isPetDebrisFull(List<AwardLine>& awardList);
        // 获得宠物碎片栏空格子数
        UInt8 getPetDebrisFieldSpaceSlot();
        // 锁定宠物
        void lockPetDebris(const Guid& itemId);
        // 解锁宠物
        void unLockPetDebris(const Guid& itemId);

        // 获得碎片
        const PetDebrisData* getPetDebrisByTemplateId(TemplateId templateId);
        const PetDebrisData* getPetDebrisByGuid(const Guid& guid);

        // 销毁碎片，这堆碎片全部销毁
        void destroyPetDebris(Guid petDebrisId);
        // 删除碎片, 数量为0表示全部删除
        void delPetDebris(Guid petDebrisId, UInt32 count = 0);
        // 清空背包
        void clearBag();

        // 放入碎片
        const PetDebrisData* createPetDebris(TemplateId templateId, UInt32 count, UInt8 source, bool promp = true);

        // 合成宠物
        ErrorId combinePet(const Guid& petDebrisId, Guid& petId);
    private:
        // 使用碎片
        bool usePetDebris(const Guid& guid, UInt32 count);
    private:
        Player* mPlayer;
        List<PetDebrisData>* mPetDebrisDataList;
        Map<TemplateId, PetDebrisData*> mPetDebrisTemplateMap;
        Map<Guid, PetDebrisData*> mPetDebrisDataMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_PET_DEBRIS_MAMAGER_H__
