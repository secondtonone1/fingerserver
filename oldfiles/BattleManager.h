#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_BATTLE_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_BATTLE_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class BattleManager : public MainThread
    {
    public:
        BattleManager();
        virtual ~BattleManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);

        bool battleSimulation(const PlayerData& playerData, const PlayerData& enemyData, 
            bool isOppRobot, BattleSimulationData& battleSimulationData);
        bool verifyBattleResult(const PlayerData& challengerData, const PlayerData& oppData, 
            bool isOppRobot, const BattleVerifyData& battleVerifyData, bool isTower = false);

        UInt32 getMyDps() { return mMyDps; }
        bool getTowerPass() { return mIsTowerPass; }

    private:
        bool initPlayerLineup(const PlayerData& playerData);
        bool initEnemyLineup(const PlayerData& enemyData);
        bool genBattleDataFromHeroData(const HeroData& heroData, BattleData& battleData, bool isPlayer);
     //   bool genBattleDataFromPetData(const PetData& petData, BattleData& battleData);

        void initBattleBufferData(BattleData& battleData);
        void initBattleTowerBuffer(BattleData& battleData);
        bool updateProperty(UInt32 bufferId, BattleData& battleData);
        bool checkExistEffectType(UInt8 effectType, const Map<UInt32, BufferData>& bufferMap);
        void updateBufferActionCount(BattleData& battleData);
        void delImmediatelyBuffer(BattleData& battleData);
        void updateNextBeginBuffer(BattleData& battleData);
        void getBufferListByPriority(List<BufferData*>& bufferList, BattleData& battleData);
        BufferData* getBufferById(UInt32 bufferId, const BattleData& battleData);
        void addBuffer(BattleData& selfData, UInt32 bufferId, BattleData& skillUser, bool nextBegin);
        void delBuffer(UInt32 bufferId, BattleData& battleData);
        void delAllBuffer(BattleData& battleData);
        bool updateBufferBeforeAttack(BattleData& battleData);
        bool updateBufferOnHit(BattleData& battleData);
        bool updateBufferBeHit(BattleData& skillUser, float hurt, BattleData& selfData);
        void getCleanList(List<BufferData*>& cleanList, UInt8 bufferType, BattleData& battleData);
        void addPassivityBufferByTriggerType(BattleData& selfData, BattleData& skillUser, UInt8 triggerType);
        void triggerPassivityOnBeHit(BattleData& selfData, BattleData& skillUser);
        void triggerPassivityOnDodge(BattleData& selfData, BattleData& skillUser);
        void triggerPassivityOnCritical(BattleData& selfData, BattleData& skillUser);
        void triggerPassivityOnKillInOnce(BattleData& selfData, BattleData& skillUser);
        bool revertProperty(BattleData& battleData, BufferData bufferData);
        UInt32 getEffectValue(UInt32 bufferId, BattleData& skillUser);

        void addMyselfDeathCount();
        void oneAttackEnd(bool isSuperSkill, bool isEnterSkill, UInt8 relationship, 
            UInt8 characterType, String curEndTag);
        List<BattleData>::Iter* getEnemyBench();
        List<BattleData>::Iter* getPlayerBench();
        bool checkBenchEnter();
        bool superSkillUse(UInt8 relationShip, UInt32 index);
        BattleData* getUser(String curEndTag, UInt8 relationship);
        BattleData* getPlayerUser();
        BattleData* getEnemyUser();
        bool checkResult(UInt8 relationship);
        bool checkOverRound();
        bool getTarget(List<BattleData*>& targetList, BattleSkillItem skillItem, String currentTag, UInt8 relationship);
        BattleData* getPositionTarget(String currentTag, UInt8 relationship, UInt8 skillRelationship);
        BattleData* getRandomOneTarget(UInt8 relationship, UInt8 skillRelationship);
        void getRandomTwoTarget(List<BattleData*>& targetList, UInt8 relationship, UInt8 skillRelationship);
        void getAllTarget(List<BattleData*>& targetList, UInt8 relationship, UInt8 skillRelationship);
        BattleData* getMostBloodOneTarget(UInt8 relationship, UInt8 skillRelationship);
        BattleData* getLeastBloodOneTarget(UInt8 relationship, UInt8 skillRelationship);
        BattleData* getMostBloodEnemy();
        BattleData* getMostBloodPlayer();
        BattleData* getLeastBloodEnemy();
        BattleData* getLeastBloodPlayer();
        void getAliveEnemy(List<BattleData*>& characters);
        void getAlivePlayer(List<BattleData*>& characters);
        BattleData* getAnyPlayer();
        BattleData* getPlayerByIndex(UInt32 index);
        BattleData* getAnyEnemy();
        BattleData* getEnemyByIndex(UInt32 index);
        void getConvergedEnemy(List<BattleData*>& characters);
        void getConvergedPlayer(List<BattleData*>& characters);
        BattleData* getRandomOneConvergedEnemy();
        BattleData* getRandomOneConvergedPlayer();
        void checkTowerResultAfterWin();
        void showResult(UInt32 result);

        void dead(BattleData* selfData);
        void useSuperSkill(BattleData* userData, BattleSkillItem skillItem);
        void useCommonSkill(BattleData* userData, BattleSkillItem skillItem);
        void useAutoSkill(BattleData* userData, BattleSkillItem skillItem);
        void useEnterSkill(BattleData* userData, BattleSkillItem skillItem);
        void addAnger(BattleData* userData);
        bool checkAutoSkill(BattleData* userData);
        void attack(BattleData* userData, BattleSkillItem skill);
        void onHit(BattleData* selfData, UInt32 index, UInt32 count);
        void onFlyerHit(BattleData* selfData, UInt32 index, UInt32 count, bool isEnd);
        void hit(BattleData* selfData, BattleData* target, bool isEnd);
        void onAttackActionEnd(BattleData* userData);
        void attackEnd(BattleData* userData);
        void deathForBufferBeforeAttack(BattleData* userData);
        void deathForBufferInAttack(BattleData& battleData);
        void addSelfSkillBuffer(BattleData* selfData);
        void addTargetSkillBuffer(BattleData* selfData, BattleData* target);
        UInt32 getAttackHitPoint(BattleData* userData);

    private:
        Player* mPlayer;
        BattleData mEnemyArray[3];
        BattleData mPlayerArray[3];
        List<BattleData> mEnemyBench;
        List<BattleData> mPlayerBench;
        BattleData mPlayerPet;
        BattleData mEnemyPet;
        // 记录技能使用前结束的回合
        String mLastEndTag;
        // 攻击次数，释放大招
        UInt32 mAttackNum;
        // 回合次数，释放神兽技能
        UInt32 mRoundNum;
        // 本方神兽是否释放完技能
        bool mPlayerPetSkillUsed;
        // 敌方神兽是否释放完技能
        bool mEnemyPetSkillUsed;
        // 记录死亡人数
        UInt32 mDeathCount;
        // 记录我方释放大招
        bool mMySuperSkillUsed;
        // 记录敌方释放大招
        bool mOppSuperSkillUsed;
        // 记录我方UserIndex
        UInt8 mPlayerUserIndex;
        // 记录敌方UserIndex
        UInt8 mEnemyUserIndex;
        // 对手为机器人
        bool mIsOppRobot;
        bool mIsSuperSkill;
        bool mIsCommonSkill;
        bool mIsAutoSkill;
        bool mIsEnterSkill;
        String mAttackAction;
        // 目标列表
        List<BattleData*> mTargetList;
        // 攻击次数
        UInt32 mAttackCount;
        // 用于技能多段攻击动作计数
        UInt32 mAttackMultiIndex;
        // 当前技能
        BattleSkillItem mCurrentSkill;
        // 模拟数据
        BattleSimulationData* mSimulationData;
        // 标记是否为验证流程
        bool mIsVerifyBattle;
        // 验证数据
        BattleVerifyData mBattleVerifyData;
        // 验证是否通过
        bool mIsVerifyPass;
        // 战斗中我方累计输出伤害
        UInt32 mMyDps;
        // 是否是爬塔 
        bool mIsTower;
        // 爬塔Buff数据
        List<TowerBufferData> mTowerBufferList;
        // 爬塔通关条件是否满足
        bool mIsTowerPass;
        // 爬塔关卡ID
        UInt32 mTowerId;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_TOLLGATE_MAMAGER_H__
