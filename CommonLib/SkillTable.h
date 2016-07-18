#ifndef __LYNX_COMMON_LIB_SKILL_TABLE_H__
#define __LYNX_COMMON_LIB_SKILL_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct SkillTemplate 
	{
        // 技能模板ID
		TemplateId mTemplateId;
        // 技能名称
        String mSkillName;
        // 技能描述ID
        DescripeId mDescripeId;
        // 技能描述
        String mDescription;
        // 技能等级
        UInt8 mLevel;
        // 技能战斗类型
        //UInt8 mCombatType;
        // 技能攻击类型
        //UInt8 mAttackType;
        // 技能敌我关系
        //UInt8 mRelationship;
        // 技能释放所需怒气
        UInt32 mTriggerAnger;
        // 技能释放增加怒气
        UInt32 mIncAnger;
        // 技能冷却回合数
        UInt8 mCoolDownRound;
        // 物理攻击
        UInt32 mPhysicsAttack;
        // 魔法攻击
        UInt32 mMagicAttack;
        // 治疗百分比
        float mCurePercent;
        // 吸血百分比
        float mSuckPercent;
        // 多次攻击次数
        //UInt8 mMultiAttack;
        // 攻击特效类型
        //UInt8 mAttackEffectType;
        //// 技能打击点
        //UInt8 mAttackPoint;
        //// 技能绑定点
        //UInt8 mBindPoint;
        // 相机位置类型
        UInt8 mCameraPos;
        // 多段攻击动作数
        //UInt8 mMultiNum;
        //// 飞行物特效
        //String mFlyerEffect;
        //// 被击特效
        //String mBeHitEffect;
        // 死亡特效
        String mDeadEffect;
        // 技能图标
        String mIcon;

        TemplateId mSkillBuffer1;
        TemplateId mSkillBuffer2;
        TemplateId mSkillBuffer3;

		SkillTemplate(): mTemplateId(0), mSkillName(""), mDescripeId(0), mDescription(""), mLevel(1),
            mTriggerAnger(0), mIncAnger(0), mCoolDownRound(0), mPhysicsAttack(0), mMagicAttack(0), 
            mCurePercent(0.0f), mSuckPercent(0.0f), mCameraPos(0), mDeadEffect(""), mIcon(""),
            mSkillBuffer1(0), mSkillBuffer2(0), mSkillBuffer3(0) {}

		LYNX_S11N_18(SkillTemplate, TemplateId, mTemplateId, String, mSkillName, 
            DescripeId, mDescripeId, String, mDescription,
            UInt8, mLevel, UInt32, mTriggerAnger, UInt32, mIncAnger, 
            UInt8, mCoolDownRound, UInt32, mPhysicsAttack, UInt32, mMagicAttack, 
            float, mSuckPercent, float, mCurePercent, UInt8, mCameraPos, 
            String, mDeadEffect, String, mIcon, TemplateId, mSkillBuffer1, 
            TemplateId, mSkillBuffer2, TemplateId, mSkillBuffer3);
	};

	typedef Map<UInt32, SkillTemplate> SkillTemplateMap;


	struct SkillTable
	{
		SkillTemplateMap mMap;

		LYNX_S11N_1(SkillTable, SkillTemplateMap, mMap);

		SkillTemplate* get(UInt32 id)
		{
			SkillTemplateMap::Iter* iter = mMap.find(id);
			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
        bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_SKILL_TABLE_H__


