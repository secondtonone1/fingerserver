#ifndef __LYNX_COMMON_LIB_LEVELGROW_TABLE_H__
#define __LYNX_COMMON_LIB_LEVELGROW_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct LevelGrowTemplate 
	{
		UInt64 mId;

		UInt32 mModelID;
		UInt32 mLevel;

		UInt32 mPower;
		
		UInt32 mMaxHP;
		UInt32 mMaxMP;
		UInt32 mMaxSP;

		UInt32 mPA;
		UInt32 mPF;

		UInt32 mMA;
		UInt32 mMF;

		UInt32 mMPRecover;
		UInt32 mSPRecover;
		UInt32 mAttSpeed;
		UInt32 mCombinationAtt;
		UInt32 mMoveSpeed;
		UInt32 mJumpHeight;
		UInt32 mFlyTimes;
		UInt32 mSPDamage;
		UInt32 mAttPush;
		UInt32 mAttPushDef;
		UInt32 mAttRaise;
		UInt32 mAttRaiseDef;
		UInt32 mStiff;
		UInt32 mStiffDef;
		UInt32 mBaseCrit;
		float mCritRate;
		UInt32 mCritDef;
		float mStunDef;
		UInt32 mStunTimeDef;
		UInt32 mSlowDef;
		UInt32 mSlowTimeDef;
		UInt32 mPoisonDef;
		UInt32 mBloodDef;
		UInt32 mKODef;
		UInt32 mFloatVal;
		UInt32 mFloatDef;
		UInt32 mFall;
		UInt32 mFallDef;

		LYNX_S11N_39(LevelGrowTemplate, UInt64, mId,UInt32, mMaxHP,UInt32, mMaxMP,
						UInt32, mMaxSP,UInt32, mPA,UInt32, mPF,
						UInt32, mMA,UInt32, mMF,UInt32, mMPRecover,
						UInt32, mSPRecover,UInt32, mAttSpeed,UInt32, mCombinationAtt,
						UInt32, mMoveSpeed,UInt32, mJumpHeight,UInt32, mFlyTimes,
						UInt32, mSPDamage,UInt32, mAttPush,UInt32, mAttPushDef,
						UInt32, mAttRaise,UInt32, mAttRaiseDef,UInt32, mStiff,
						UInt32, mStiffDef,UInt32, mBaseCrit,float, mCritRate,
						UInt32, mCritDef,float, mStunDef,UInt32, mStunTimeDef,
						UInt32, mSlowDef,UInt32, mSlowTimeDef,UInt32, mPoisonDef,
						UInt32, mBloodDef,UInt32, mKODef,UInt32, mFloatVal,
						UInt32, mFloatDef,UInt32, mFall,UInt32, mFallDef,
						UInt32, mModelID,
						UInt32, mLevel,UInt32, mPower
			);

	};

	typedef Map<UInt64, LevelGrowTemplate> LevelGrowTemplateMap;
	typedef Map<UInt32, Map<UInt32, LevelGrowTemplate> > LevelGrowReverseFindMap;

	struct LevelGrowTable
	{
		LevelGrowTemplateMap mMap;
		LevelGrowReverseFindMap mReverseMap;

		LYNX_S11N_2(LevelGrowTable, LevelGrowTemplateMap, mMap,LevelGrowReverseFindMap,mReverseMap);

		LevelGrowTemplate* get(UInt32 id)
		{
			LevelGrowTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		LevelGrowTemplate* reverseGet(UInt32 modelId, UInt32 level)
		{
			LevelGrowReverseFindMap::Iter * levelMapIter = mReverseMap.find(modelId);
			
			if(!levelMapIter)
			{
				return NULL;
			}
			
			Map<UInt32, LevelGrowTemplate>::Iter *levelGrowIter = levelMapIter->mValue.find(level);

			if(!levelGrowIter)
			{
				return NULL;
			}


			return &levelGrowIter->mValue;

		}

		void clear()
		{
			mMap.clear();
			mReverseMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_LEVELGROW_TABLE_H__