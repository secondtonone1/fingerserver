#ifndef __LYNX_GAME_SERVER_CALCULATE_SYSTEM_H__
#define __LYNX_GAME_SERVER_CALCULATE_SYSTEM_H__

#include "MainThread.h"
#include "Player.h"

namespace Lynx
{
    struct OneLevelProperty
    {
        OneLevelProperty() : mStamina(0.0f), mStaminaPercent(1.0f), mIntellect(0.0f),
            mIntellectPercent(1.0f), mStrength(0.0f), mStrengthPercent(1.0f)
        {
        }

        float mStamina;
        float mStaminaPercent;
        float mIntellect;
        float mIntellectPercent;
        float mStrength;
        float mStrengthPercent;
    };

    struct TwoLevelProperty
    {
        TwoLevelProperty() : mHp(0.0f), mHpPercent(1.0f), mAttack(0.0f), mAttackPercent(1.0f),
            mPhysicsDefense(0.0f), mPhysicsDefensePercent(1.0f), mMagicDefense(0.0f),
            mMagicDefensePercent(1.0f), mCritical(0.0f), mCriticalPercent(1.0f), mRescrit(0.0f),
            mRescritPercent(1.0f), mHit(0.0f), mHitPercent(1.0f), mDodge(0.0f),
            mDodgePercent(1.0f), mSuck(0.0f), mAvoidDamage(0.0f), mCure(0.0f), mCriticalDamage(0.0f),
            mSuckPercent(1.0f), mCurePercent(1.0f), mAvoidDamagePercent(1.0f), mCriticalDamagePercent(1.0f)
        {
        }

        float mHp;
        float mHpPercent;
        float mAttack;
        float mAttackPercent;
        float mPhysicsDefense;
        float mPhysicsDefensePercent;
        float mMagicDefense;
        float mMagicDefensePercent;
        float mCritical;
        float mCriticalPercent;
        float mRescrit;
        float mRescritPercent;
        float mHit;
        float mHitPercent;
        float mDodge;
        float mDodgePercent;
        float mSuck;
        float mSuckPercent;
        float mAvoidDamage;
        float mAvoidDamagePercent;
        float mCure;
        float mCurePercent;
        float mCriticalDamage;
        float mCriticalDamagePercent;
    };

    struct HeroConsumeReport
    {
        UInt32 mGold;
        UInt32 mLevelExp;
        Map<TemplateId, UInt32> mHeroMap;
        Map<TemplateId, UInt32> mItemMap;
    };

    struct EquipConsumeReport
    {
        UInt32 mGold;
        Map<TemplateId, UInt32> mItemMap;
    };

	class CalculateUtil 
	{
	public:
    
        static void getPlayerPresentHeros(const PlayerData& playerData, Map<UInt8, HeroData*>& heroMap);

        static void getPlayerPresentEquips(const PlayerData& playerData, Map<UInt8, List<EquipData*> >& equipMap);
        static void getPlayerPresentRunes(const PlayerData& playerData, Map<UInt8, List<RuneData*> >& runeMap);

        // 计算玩家数据
        static void calculatePlayerData(PlayerData& playerData);
 
        static void calculatePlayerProduceData(PlayerData& playerData);
        static void calculatePlayerGoldMineralData(PlayerData& playerData);
        static void calculatePlayerDiamondMineralData(PlayerData& playerData);
        static void calculatePlayerExpMineralData(PlayerData& playerData);

     

        // 计算消费记录
        static bool calculateHeroConsumeReport(const HeroData& heroData, HeroConsumeReport& report);
        static bool calculateEquipConsumeReport(const EquipData& equipData, EquipConsumeReport& report);

    private:
    
      
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_CALCULATE_SYSTEM_H__

