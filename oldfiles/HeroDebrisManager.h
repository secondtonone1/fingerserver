#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_HERO_DEBRIS_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_HERO_DEBRIS_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class HeroDebrisManager : public MainThread
    {
    public:
        HeroDebrisManager();
        virtual ~HeroDebrisManager();

        bool initial(Player* player);
        void release();

    public:
        // 检查英雄碎片栏是否满格
        bool isHeroDebrisFull();
        bool isHeroDebrisFull(List<TemplateId>& heroDebrisList);
        bool isHeroDebrisFull(List<AwardLine>& awardList);
        // 获得英雄碎片栏空格子数
        UInt8 getHeroDebrisFieldSpaceSlot();
        // 锁定道具
        void lockHeroDebris(const Guid& itemId);
        // 解锁道具
        void unLockHeroDebris(const Guid& itemId);

        // 获得碎片
        const HeroDebrisData* getHeroDebrisByTemplateId(TemplateId templateId);
        const HeroDebrisData* getHeroDebrisByGuid(const Guid& guid);

        // 销毁碎片，这堆碎片全部销毁
        void destroyHeroDebris(Guid heroDebrisId);
        // 删除碎片, 数量为0表示全部删除
        void delHeroDebris(Guid heroDebrisId, UInt32 count = 0);
        // 清空背包
        void clearBag();

        // 放入碎片
        const HeroDebrisData*  createHeroDebris(TemplateId templateId, UInt32 count, UInt8 source, bool promp = true);

        // 合成英雄
        ErrorId combineHero(const Guid& heroDebrisId, Guid& heroId);
    private:
        // 使用碎片
        bool useHeroDebris(const Guid& guid, UInt32 count);
    private:
        Player* mPlayer;
        List<HeroDebrisData>* mHeroDebrisList;
        Map<Guid, HeroDebrisData*> mHeroDebrisDataMap;
        Map<TemplateId, HeroDebrisData*> mHeroDebrisTemplateMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_HERO_DEBRIS_MAMAGER_H__
