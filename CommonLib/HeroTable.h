#ifndef __LYNX_COMMON_LIB_HERO_TABLE_H__
#define __LYNX_COMMON_LIB_HERO_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct HeroTemplate_EX
	{
		float mVertigoDef;//迷晕防御力 todoadd

		float mVertigoTimeDef;//迷晕时长防御力

		float mBaseToughness;//韧性

		float mFallDef;//击倒抗性

		String mDefenceAreaStand;//站立时防御范围

		String mDefenceAreaLie;//躺着时防御范围

		LYNX_S11N_6(HeroTemplate_EX,float, mVertigoDef,float, mVertigoTimeDef,float, mBaseToughness, float, mFallDef,
			String, mDefenceAreaStand,String, mDefenceAreaLie)
	};

	struct HeroTemplate 
	{
		UInt32 mId;//角色模型id
		String mName;//角色名称
		String mDesc;//介绍
		String mRes;//资源
		String mStrSkillList;//技能列表
		String mStrSkillListOnCreate;//

		List<UInt64> m_listOriginSkills;//初始技能

		List<UInt64> m_listLearnSkills;//可学习技能

		UInt64 mNeedLv;
		UInt64 mNeedVipLv;
		UInt64 mCost;

		float mPA;//物理攻击力
		float mPF;//物理防御力

		float mMA;//魔法攻击力
		float mMF;//魔法防御力

		float mMaxHP;//最大血量
		float mMaxMP;//最大蓝量
		float mMaxSP;//最大霸体值

		float mMPRecover;//魔法恢复力
		float mSPRecover;//霸体恢复力
		float mAttackSpeed;//攻击速度

		float mCombinationAtt;//连招间隔
		float mMoveSpeed;//移动速度

		float mJumpHeight;//跳起高度
		float mFlyTimes;//可连续浮空次数
		float mSPDamage;//sp消耗

		float mAttPush;//击退
		float mAttPushDef;//击退防御

		float mAttRaise;//挑起
		float mAttRaiseDef;//挑起防御
		
		float mStiff;//基础僵直
		float mStiffDef;//僵直防御

		float mBaseCrit;//基础暴击率
		float mCritRate;//暴击倍率

		float mCritDef;//暴击防御力
		float mStunDef;//击晕防御力

		float mStunTimeDef;//击晕时长防御

		float mSlowDef;//减速防御力

		float mSlowTimeDef;//减速时间防御力

		float mPoisonDef;//中毒防御力

		float mBloodDef;//易伤防御
		
		float mKODef;//秒杀防御

		float mFloatVal;//击飞

		float mFloatDef;//击飞抗性

		float mFall;//击倒

	

	

		HeroTemplate_EX heroEx;//！！！太多了会报错请加到这里HeroTemplate_EX

		LYNX_S11N_46(HeroTemplate, UInt32, mId,String, mName,String, mDesc,		
			String, mRes,String, mStrSkillList,String ,mStrSkillListOnCreate,
				float, mPA,float, mMF,float, mMaxHP,
				float, mMaxMP,float, mMaxSP,float, mMPRecover,
				float, mSPRecover,float, mAttackSpeed,float, mCombinationAtt,
				float, mMoveSpeed, float,mJumpHeight,float ,mFlyTimes,
				float, mSPDamage,float ,mAttPush ,float, mAttPushDef,
				float, mAttRaise,float, mAttRaiseDef,float, mStiff,
				float, mStiffDef,float, mBaseCrit,float, mCritRate,
				float, mCritDef,float, mStunDef,float, mStunTimeDef,
				float, mSlowDef,float, mSlowTimeDef,float, mPoisonDef,
				float, mBloodDef,float, mKODef,float, mFloatVal,
				float, mFloatDef,float, mFall,  float, mPF,
				float, mMA, List<UInt64>, m_listOriginSkills,List<UInt64>,m_listLearnSkills,
				HeroTemplate_EX, heroEx, UInt64, mNeedLv,
		        UInt64, mNeedVipLv, UInt64, mCost	);
	};

	typedef Map<UInt32, HeroTemplate> HeroTemplateMap;


	struct HeroTable
	{
		HeroTemplateMap mMap;


		LYNX_S11N_1(HeroTable, HeroTemplateMap, mMap);

		HeroTemplate* get(UInt32 id)
		{
			HeroTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_HERO_TABLE_H__