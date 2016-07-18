#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_TOWER_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_TOWER_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class TowerManager : public MainThread
    {
    public:
        TowerManager();
        virtual ~TowerManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);

        void openTowerBuffer();
        void selectTowerBuffer(UInt32 index);
        void startTowerBattle(UInt8 tollgateDifficulty);
        void towerBattleResult(const TowerBattleResultReq& msg);
        void resetTower();
        void sweepTower();
        void syncTowerDataToClient();
        void saveTowerData();

    private:
        bool genTowerBufferData(UInt32 index, TowerBufferData& towerBufferData);
        bool addTowerBuffer(UInt32 index);
        ErrorId checkTowerTollgateId(TemplateId tollgateId);
        ErrorId genTowerBattleData(TemplateId tollgateId);
        ErrorId checkTowerResult(TemplateId tollgateId, const BattleVerifyData& battleVerifyData, UInt8 tollgateDifficulty);
        void genTowerOppBattleData(PlayerData& oppBattleData);
        void refreshTowerInfo();
        UInt32 getResetCostDiamond(UInt32 resetIndex);
        bool getAwardChests(TemplateId tollgateId, List<MsgItemBaseData>& itemList);

    private:
        Player* mPlayer;
        PlayerTowerData* mPlayerTowerData;

        List<TowerBufferData> mSelectBufferList;
        List<MonsterData> mTowerMonsterList;
   //     PetData mTowerPet;

        TemplateId mTowerIdInBattle;
        UInt32 mCheckResetInterval;
        UInt32 mTowerResetTime;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_TOWER_MAMAGER_H__
