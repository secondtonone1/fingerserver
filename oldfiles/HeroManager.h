#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_HERO_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_HERO_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class HeroManager : public MainThread
    {
    public:
        HeroManager();
        virtual ~HeroManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);
        void calculateHeroData(const Guid& heroId);

        const HeroData* createHero(const TemplateId& templateId, bool promp = true);
        bool destroyHero(Guid guid);
        const HeroData* getHeroDataByGuid(const Guid& guid) const;
        const HeroData* getHeroDataByPresentPos(const UInt8& presentPos) const;
        const Map<UInt8, HeroData*>& getPresentHeroDataMap() const { return mPlayerPresentHeroDataMap; }


        bool isHeroFull(List<TemplateId>& heroList);
        // 为出场英雄统一添加经验， 触发升级
        void refinePresentHeros(List<Guid>& heroLevelUpList, UInt32 incExp);
        // 为一个英雄添加经验，触发升级
        ErrorId refineHero(const Guid& heroId, UInt32& upLevelCount, UInt32 incExp);
        // 增加英雄等级接口
        ErrorId refineHero(const Guid& heroId, UInt32 levelCount);
        // 进阶英雄接口
        ErrorId advancedHero(const Guid& heroId, const List<Guid>& heroList, const List<Guid>& itemList);
        // 觉醒英雄接口
        ErrorId awakenHero(const Guid& heroId, const List<Guid>& heroList, const List<Guid>& itemList);
        // 熔炼接口
        ErrorId smeltHero(const Guid& heroId, UInt32& gold, UInt32& levelExp, UInt32& soul);
        // 重生接口
        ErrorId rebornHero(const Guid& heroId, UInt32& gold, UInt32& levelExp,
            List<Guid>& heroList, List<Guid>& itemList);

        ErrorId setHeroPresentPos(const Guid& heroId, UInt8 presentPos);
        ErrorId exchangeHeroPresentPos(const Guid& heroGuid1, const Guid& heroGuid2);
        ErrorId sellHero(List<Guid>& sellList, UInt32& getGold);

    public:
        // 以下为存属性同步接口
        void increaseHeroLevel(const Guid& heroId, UInt16 offset);
        void reduceHeroLevel(const Guid& heroId, UInt16 offset);
        void increaseHeroLevelExp(const Guid& heroId, UInt32 offset);
        void reduceHeroLevelExp(const Guid& heroId, UInt32 offset);
        void increaseHeroAdvanced(const Guid& heroId, UInt32 offset);
        void reduceHeroAdvanced(const Guid& heroId, UInt32 offset);
        void increaseHeroConsumeGold(const Guid& heroId, UInt32 offset);
        void reduceHeroConsumeGold(const Guid& heroId, UInt32 offset);

    private:
        struct HeroRequireCondition
        {
            HeroRequireCondition() : mRequireHeroLevel(0), mRequireGold(0) {}

            UInt32 mRequireHeroLevel;
            List<std::pair<TemplateId, UInt32> > mRequireItems;
            List<std::pair<TemplateId, UInt32> > mRequireHeros;
            UInt32 mRequireGold;
        };

        void getConditionTemplate(const HeroData& heroData, HeroRequireCondition& condition);
        
    private:
        Player* mPlayer;
        PlayerHeroData* mPlayerHeroData;
        Map<Guid, HeroData*> mPlayerHeroDataMap;
        Map<UInt8, HeroData*> mPlayerPresentHeroDataMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_HERO_MAMAGER_H__
