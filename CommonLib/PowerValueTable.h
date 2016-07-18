#ifndef __LYNX_COMMON_LIB_PowerValue_TABLE_H__
#define __LYNX_COMMON_LIB_PowerValue_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	
	struct PowerValueTemplate 
	{
// 		UInt64 mId;
		
		float mMaxHP;
		float mMaxMP;
		float mMaxSP;

		float mPA;
		float mPF;

		float mMA;
		float mMF;

		float mMPRecover;
		float mSPRecover;
		float mAttSpeed;
		float mCombinationAtt;
		float mMoveSpeed;
		float mJumpHeight;
		float mFlyTimes;
		float mSPDamage;
		float mAttPush;
		float mAttPushDef;
		float mAttRaise;
		float mAttRaiseDef;
		float mStiff;
		float mStiffDef;
		float mBaseCrit;
		float mCritRate;
// 		float mCritDef;//后面的配置文件没有
// 		float mStunDef;
// 		float mStunTimeDef;
// 		float mSlowDef;
// 		float mSlowTimeDef;
// 		float mPoisonDef;
// 		float mBloodDef;
// 		float mKODef;
// 		float mFloatVal;
// 		float mFloatDef;
// 		float mFall;
// 		float mFallDef;

		LYNX_S11N_23(PowerValueTemplate, /*UInt64, mId,*/float, mMaxHP,float, mMaxMP,
						float, mMaxSP,float, mPA,float, mPF,
						float, mMA,float, mMF,float, mMPRecover,
						float, mSPRecover,float, mAttSpeed,float, mCombinationAtt,
						float, mMoveSpeed,float, mJumpHeight,float, mFlyTimes,
						float, mSPDamage,float, mAttPush,float, mAttPushDef,
						float, mAttRaise,float, mAttRaiseDef,float, mStiff,
						float, mStiffDef,float, mBaseCrit,float, mCritRate
// 						float, mCritDef,float, mStunDef,float, mStunTimeDef,
// 						float, mSlowDef,float, mSlowTimeDef,float, mPoisonDef,
// 						float, mBloodDef,float, mKODef,float, mFloatVal,
// 						float, mFloatDef,float, mFall,float, mFallDef
			);

	};

	typedef Map<UInt64, PowerValueTemplate> PowerValueTemplateMap;


	struct PowerValueTable
	{
		PowerValueTemplateMap mMap;


		LYNX_S11N_1(PowerValueTable, PowerValueTemplateMap, mMap);

		PowerValueTemplate* get(UInt32 id =1)
		{
			PowerValueTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_PowerValue_TABLE_H__