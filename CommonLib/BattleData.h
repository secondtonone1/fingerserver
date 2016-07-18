#ifndef __LYNX_COMMON_LIB_BATTLE_DATA_H__
#define __LYNX_COMMON_LIB_BATTLE_DATA_H__

#include "Definition.h"
#include "BufferData.h"
#include "SkillData.h"

namespace Lynx
{
#if 1
    struct BattleBaseData
    {
        BattleBaseData() : mGuidId(0), mTemplateId(0), mTag(""), 
            mCharacterType(0), mRelationship(0), mHp(0), mAnger(0), 
            mParalysisId(0), mBlindId(0), mConvergedId(0), mSilentId(0),
            mSealId(0), mPhyInvincibleId(0), mMagInvincibleId(0) {}

        // 角色GUID
        Guid mGuidId;
        // 模板ID
        TemplateId mTemplateId;
        // 角色标签
        String mTag;
        // 角色类型
        UInt8 mCharacterType;
        // 敌我类型
        UInt8 mRelationship;
        // HP
        UInt32 mHp;
        // 怒气
        UInt32 mAnger;

        // 麻痹
        UInt32 mParalysisId;
        // 致盲
        UInt32 mBlindId;
        // 集火
        UInt32 mConvergedId;
        // 沉默
        UInt32 mSilentId;
        // 封印
        UInt32 mSealId;
        // 物理无敌
        UInt32 mPhyInvincibleId;
        // 魔法无敌
        UInt32 mMagInvincibleId;
    };

    struct BattleExtendData
    {
        BattleExtendData() : mHpUpper(0), mAngerUpper(0),
            mAttack(0), mPhysicsDefense(0), mMagicDefense(0), 
            mCritical(0), mRescrit(0), mHit(0), mDodge(0),
            mSuck(0), mAvoidDamage(0), mCure(0), mCriticalDamage(0) {}

        // 血量上限
        UInt32 mHpUpper;
        // 怒气上限
        UInt32 mAngerUpper;
        // 攻击力
        UInt32 mAttack;
        // 物理防御
        UInt32 mPhysicsDefense;
        // 魔法防御
        UInt32 mMagicDefense;
        // 暴击
        UInt32 mCritical;
        // 抗暴
        UInt32 mRescrit;
        // 命中
        UInt32 mHit;
        // 闪避
        UInt32 mDodge;
        // 吸血
        UInt32 mSuck;
        // 免伤
        UInt32 mAvoidDamage;
        // 治疗
        UInt32 mCure;
        // 暴击伤害
        UInt32 mCriticalDamage;

    };

    struct BattleBufferData
    {
        BattleBufferData() : mHpUpper(0), mAttack(0), mPhysicsDefense(0), mMagicDefense(0), 
            mCritical(0), mRescrit(0), mHit(0), mDodge(0),
            mSuck(0), mAvoidDamage(0), mCure(0), mCriticalDamage(0) {}

        // 血量上限
        UInt32 mHpUpper;
        // 攻击力
        UInt32 mAttack;
        // 物理防御
        UInt32 mPhysicsDefense;
        // 魔法防御
        UInt32 mMagicDefense;
        // 暴击
        UInt32 mCritical;
        // 抗暴
        UInt32 mRescrit;
        // 命中
        UInt32 mHit;
        // 闪避
        UInt32 mDodge;
        // 吸血
        UInt32 mSuck;
        // 免伤
        UInt32 mAvoidDamage;
        // 治疗
        UInt32 mCure;
        // 暴击伤害
        UInt32 mCriticalDamage;

        Map<UInt32, BufferData> mBufferMap;
        UInt32 mPassivityId[3];
    };

    struct BattleSkillItem
    {
        BattleSkillItem() : mTemplateId(0), mSkillAttackType(0),
            mCombatType(0), mSkillRelationship(0), mTriggerAnger(0),
            mAngerInc(0), mCoolDownRound(0), mMultiAttack(0), 
            mMultiNum(0), mHitPointCount(0) {}

        // 技能模板ID
        TemplateId mTemplateId;
        // 技能类型（对位，随机单体，随机2个，全体，指定）
        UInt8 mSkillAttackType;
        // 技能近程远程
        UInt8 mCombatType;
        // 技能敌我
        UInt8 mSkillRelationship;
        // 触发怒气
        UInt32 mTriggerAnger;
        // 增加怒气
        UInt32 mAngerInc;
        // 冷却回合数
        UInt32 mCoolDownRound;
        // 多次攻击
        UInt32 mMultiAttack;
        // 多段攻击
        UInt32 mMultiNum;
        // 打击点数量
        UInt32 mHitPointCount;
    };

    struct BattleSkillData
    {
        BattleSkillData(): mPassiveSkill1(0), mPassiveSkill2(0), mPassiveSkill3(0) {}

        UInt32 mPassiveSkill1;
        UInt32 mPassiveSkill2;
        UInt32 mPassiveSkill3;

        BattleSkillItem mCommonSkill;
        BattleSkillItem mAutoSkill;
        BattleSkillItem mSuperSkill;
        BattleSkillItem mEnterSkill;
		
		//sec 注释技能字段
        //ActionHitMap mActionHitMap;
    };

    struct BattleData 
    {
        BattleData() {}

        BattleBaseData mBaseData;
        BattleExtendData mExtendData;
        BattleBufferData mBufferData;
        BattleSkillData mSkillData;
    };
#endif

    struct BattleSimulationData
    {
        BattleSimulationData(): mStarCount(0), mRandomStartSeed(0) {}

        UInt32 mStarCount;
        UInt16 mRandomStartSeed;
        List<UInt32> mSuperSkillList;

        LYNX_S11N_3(BattleSimulationData, UInt32, mStarCount, UInt16, mRandomStartSeed,
            List<UInt32>, mSuperSkillList);
    };

    struct BattleVerifyData 
    {
        BattleVerifyData(): mIsVictory(false), mDeathCount(0), mRandomStartSeed(0) {}

        bool mIsVictory;
        UInt8 mDeathCount;
        UInt16 mRandomStartSeed;
        List<UInt32> mSuperSkillRecord;

        LYNX_S11N_4(BattleVerifyData, bool, mIsVictory, UInt8, mDeathCount, 
            UInt16, mRandomStartSeed, List<UInt32>, mSuperSkillRecord);
    };

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_BATTLE_DATA_H__
