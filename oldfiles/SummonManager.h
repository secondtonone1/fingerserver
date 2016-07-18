#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_SUMMON_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_SUMMON_MAMAGER_H__

#include "MainThread.h"

/* 抽英雄包说明
   1 道具抽包， 只有一个， ID 1
   2 免费砖石抽包，有5个包，ID分别为2，3，4，5，6, 前5次按顺序使用， 第六次开始， 永远使用6
   3 钻石抽包, 有5个包，ID分别为 7，8，9，10，11，前5次按顺序使用， 第六次开始， 永远使用11
   4 十连抽包, 有两个，ID分别为 12， 13，每次十连抽， 使用一次5星包， 使用一次13， 其余次数都使用12抽取
   5 五星抽包, 有一个, ID为 14
*/

#define ITEM_SUMMON_PKG 1

#define FREE_DIAMOND_SUMMON_PKG_0 2
#define FREE_DIAMOND_SUMMON_PKG_1 3
#define FREE_DIAMOND_SUMMON_PKG_2 4
#define FREE_DIAMOND_SUMMON_PKG_3 5
#define FREE_DIAMOND_SUMMON_PKG_4 6
#define FREE_DIAMOND_SUMMON_PKG   6

#define DIAMOND_SUMMON_PKG_0 7
#define DIAMOND_SUMMON_PKG_1 8
#define DIAMOND_SUMMON_PKG_2 9
#define DIAMOND_SUMMON_PKG_3 10
#define DIAMOND_SUMMON_PKG_4 11
#define DIAMOND_SUMMON_PKG   11

#define BATCH_SUMMON_PKG_0 12
#define BATCH_SUMMON_PKG_1 13

#define SUPPER_HERO_PKG    14

namespace Lynx
{
    class Player;
    class SummonManager : public MainThread
    {
    public:
        SummonManager();
        virtual ~SummonManager();

        bool initial(Player* player);
        void release();

        // 召唤英雄接口
        ErrorId summonHeroByItem(List<Guid>& heroGuidList);
        ErrorId summonHeroByDiamond(List<Guid>& heroGuidList);
        ErrorId summonHeroByFreeDiamond(List<Guid>& heroGuidList);
        ErrorId summonHeroByBatch(List<Guid>& heroGuidList);

    private:
        Guid randPkg(const TemplateId& pkgId);

    private:
        Player* mPlayer;
        PlayerSummonData* mPlayerSummonData;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_SUMMON_MAMAGER_H__
