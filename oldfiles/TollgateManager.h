#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_TOLLGATE_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_TOLLGATE_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class TollgateManager : public MainThread
    {
    public:
        TollgateManager();
        virtual ~TollgateManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);

        bool addTollgate(const TollgateData& tollgateData);
        bool updateTollgate(const TollgateData& tollgateData);

        void startTollgateBattle(const Guid& tollgateGuid, UInt8 tollgateDifficulty);
        void startFiveTollgateSweep(const Guid& tollgateGuid, UInt8 tollgateDifficulty);
        void tollgateBattleResult(const TollgateBattleResultReq& msg);
        void againTollgateBattle(const Guid& tollgateGuid, UInt8 tollgateDifficulty);
        void getScenePassAward(TemplateId sceneId, UInt8 chestsIndex);

        bool genTollgateMonsterData(MonsterData& monsterData, TemplateId templateId, UInt32 pos);
       // bool genTollgatePetData(PetData& petData, TemplateId templateId);
        void changeMonsterDataToHeroData(HeroData& heroData, const MonsterData& monsterData);
        bool getDropItemList(TemplateId tollgateId, List<MsgItemBaseData>& dropItemlist);

        void startStoreHouseBattle(TemplateId tollgateId);
        void storeHouseBattleResult(const StoreHouseBattleResultReq& msg);

        bool checkTemplateInvalid(TemplateId templateId, UInt8 itemType);

    private:
        ErrorId checkTollgate(const Guid& tollgateGuid, UInt8 tollgateDifficulty);
        TollgateData* getTollgateDataById(const Guid& tollgateGuid);
        //void tollgateIncreaseHeroExp(List<Guid>& heroLevelUpList, UInt32 heroExp);
        //bool tollgateIncreasePlayerExp(UInt32 teamExp);
        void genOppBattleData(PlayerData& oppBattleData);
        ErrorId checkTollgateResult(const Guid& tollgateGuid, UInt8 tollgateDifficulty, 
            const BattleVerifyData& battleVerifyData);
        ErrorId genTollgateBattleData(TemplateId tollgateId);
        TollgateData* getNextTollgateByTemplateId(TemplateId templateId);
        ErrorId checkStoreHouseTollgateId(TemplateId tollgateId);
        void resetTollgateBattleCount();
        ErrorId checkStoreHouseResult(TemplateId tollgateId, const BattleVerifyData& battleVerifyData);
        void resetStoreHouseBattleCount();

    private:
        Player* mPlayer;
        PlayerTollgateData* mPlayerTollgateData;
        Map<Guid, TollgateData*> mTollgateMap;
        // 正在战斗中的关卡Id
        TemplateId mTollgateIdInBattle;
        //bool mTollgateFighting;
        List<MonsterData> mTollgateMonsterList;
     //   PetData mTollgatePet;
        UInt32 mCheckResetInterval;
        UInt32 mResetTime;
        UInt32 mResetStoreHouseTime;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_TOLLGATE_MAMAGER_H__
