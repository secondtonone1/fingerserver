#ifndef __LYNX_COMMON_LIB_MONSTER_DATA_H__
#define __LYNX_COMMON_LIB_MONSTER_DATA_H__

#include "Definition.h"
#include "SkillData.h"

namespace Lynx
{
    struct MonsterBaseData
    {
        MonsterBaseData() : mMonsterId(0), mMonsterName(""), mTemplateId(0),
            mLevel(0), mHp(0), mStarLevel(0), mAnger(0), mPresentPos(0) {}

        // 角色GUID
		Guid mMonsterId;
        // 角色名称
        String mMonsterName;
        // 模板ID
		TemplateId mTemplateId;
        // 等级
        UInt32 mLevel;
        // HP
        UInt32 mHp;
        // 星级
        UInt32 mStarLevel;
        // 怒气
        UInt32 mAnger;
        // 上阵状态 0不上阵，1，位置1（左），2，位置2（中），3， 位置3（右），4，替补1，5，替补2
        UInt8 mPresentPos;

        LYNX_S11N_8(MonsterBaseData, Guid, mMonsterId, String, mMonsterName,
            TemplateId, mTemplateId, UInt32, mLevel, UInt32, mHp, UInt32, mStarLevel, 
            UInt32, mAnger, UInt8, mPresentPos);
    };

    struct MonsterExtendData
    {
        MonsterExtendData() : mAttack(0), mPhysicsDefense(0), mMagicDefense(0), mCritical(0), 
            mHit(0), mDodge(0), mHpUpper(0), mAngerUpper(0), mRescrit(0), mSuck(0), mCure(0),
            mAvoidDamage(0), mCriticalDamage(0) {}

        // 攻击
        UInt32 mAttack;
        // 物理防御
        UInt32 mPhysicsDefense;
        // 魔法防御
        UInt32 mMagicDefense;
        // 暴击
        UInt32 mCritical;
        // 命中
        UInt32 mHit;
        // 闪避
        UInt32 mDodge;
        // 血量上限
        UInt32 mHpUpper;
        // 怒气上限
        UInt32 mAngerUpper;
        // 抗暴
        UInt32 mRescrit;
        // 吸血
        UInt32 mSuck;
        // 免伤
        UInt32 mAvoidDamage;
        // 治疗
        UInt32 mCure;
        // 暴击伤害
        UInt32 mCriticalDamage;

        LYNX_S11N_13(MonsterExtendData, UInt32, mAttack, UInt32, mPhysicsDefense, 
            UInt32, mMagicDefense, UInt32, mCritical,
            UInt32, mHit, UInt32, mDodge, UInt32, mHpUpper, UInt32, mAngerUpper,
            UInt32, mRescrit, UInt32, mSuck, UInt32, mAvoidDamage, UInt32, mCure,
            UInt32, mCriticalDamage);
    };

    struct MonsterSkillData : public SkillData
    {
        MonsterSkillData() {}

        LYNX_S11N_4(MonsterSkillData, SkillItem, mCommonSkill, SkillItem, mAutoSkill,
            SkillItem, mSuperSkill, SkillItem, mEnterSkill);
    };

    struct MonsterData 
    {
        MonsterData() {}

        MonsterBaseData mBaseData;
        MonsterExtendData mExtendData;
        MonsterSkillData mSkillData;

        LYNX_S11N_3(MonsterData, MonsterBaseData, mBaseData, MonsterExtendData, mExtendData,
            MonsterSkillData, mSkillData);
    };

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_MONSTER_DATA_H__