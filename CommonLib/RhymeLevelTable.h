#ifndef __LYNX_COMMON_LIB_RHYMELEVEL_TABLE_H__
#define __LYNX_COMMON_LIB_RHYMELEVEL_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct RhymeLevelTemplate 
	{
		UInt64 mRhymeID;
		UInt32 mStep;
		UInt32 mLevel;
		UInt32 mNextNeedExp;

		UInt32 mNextNeedCoin;
		UInt32 mNextNeedSoul;
		UInt32 mGetExp;
		UInt32 mLevelUpToStep;

		UInt32 mLevelUpToLevel;
		UInt32 mAcupoint;
		String critRate;
		Vector<UInt32> mRateEnhanceVec;

		UInt32 mActiveSkillIndex;
		UInt32 mMaxHp;
		UInt32 mMaxMp;
		UInt32 mMaxSp;

		UInt32 mPA;
		UInt32 mMA;
		UInt32 mPF;
		UInt32 mMF;

		UInt32 mHPRecover;
		UInt32 mMPRecover;
		UInt32 mSPRecover;
		UInt32 mMaxHPAdd;

		UInt32 mMaxMPAdd;
		UInt32 mMaxSPAdd;
		UInt32 mPAAdd;
		UInt32 mMAAdd;

		UInt32 mPFAdd;
		UInt32 mMFAdd;
		UInt32 mHPRecoverAdd;
		UInt32 mMPRecoverAdd;

		UInt32 mSPRecoverAdd;

		UInt32 mPower;

		LYNX_S11N_34(RhymeLevelTemplate, UInt64, mRhymeID,UInt32, mStep,UInt32, mLevel,UInt32, mNextNeedExp,UInt32, mNextNeedCoin,
			UInt32, mNextNeedSoul,UInt32, mGetExp,UInt32, mLevelUpToStep,UInt32, mLevelUpToLevel,UInt32, mAcupoint,
			String, critRate,Vector<UInt32>, mRateEnhanceVec,UInt32, mActiveSkillIndex,UInt32, mMaxHp,UInt32, mMaxMp,
			UInt32, mMaxSp,UInt32, mPA,UInt32, mMA,UInt32, mPF,UInt32, mMF,
			UInt32, mHPRecover,UInt32, mMPRecover,UInt32, mSPRecover,UInt32, mMaxHPAdd,UInt32, mMaxMPAdd,
			UInt32, mMaxSPAdd,UInt32, mPAAdd,UInt32, mMAAdd,UInt32, mPFAdd,UInt32, mMFAdd,
			UInt32, mHPRecover,UInt32, mMPRecoverAdd,UInt32, mSPRecoverAdd, UInt32, mPower
			);

	};

	typedef Map<UInt64, RhymeLevelTemplate> RhymeLevelTemplateMap;
	typedef Map<UInt32, Map<UInt32, UInt64> > RhymeReverseFindMap;
	typedef Map<UInt32,  RhymeLevelTemplate>  RhymeStepMaxMap;
	struct RhymeLevelTable
	{
		RhymeLevelTemplateMap mMap;
		RhymeReverseFindMap mReverseFindMap;
		RhymeStepMaxMap mRhymeStepMaxMap;

		LYNX_S11N_3(RhymeLevelTable, RhymeLevelTemplateMap, mMap,RhymeReverseFindMap,mReverseFindMap,
			RhymeStepMaxMap, mRhymeStepMaxMap);

		RhymeLevelTemplate* get(UInt64 id)
		{
			RhymeLevelTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}

			return &iter->mValue;
		}

		RhymeLevelTemplate * getStepMax(UInt32 step)
		{
			RhymeStepMaxMap::Iter * iter = mRhymeStepMaxMap.find(step);
			if(iter == mRhymeStepMaxMap.end() )
			{
				return NULL;
			}

			return &iter->mValue;
		}
		
		RhymeLevelTemplate * getNext(UInt64 id)
		{
			RhymeLevelTemplateMap::Iter* iter = mMap.find(id);
			if(iter == mMap.end())
			{
				return NULL;
			}
			else
			{
				if(mMap.next(iter))
				{
					return &(mMap.next(iter)->mValue);
				}
				else
				{
					return NULL;
				}
				
			}
		}

		UInt64 * getIdByStepLv(UInt32 step, UInt32 level)
		{
			RhymeReverseFindMap::Iter *mapFindIter = mReverseFindMap.find(step);
			
			if(mapFindIter == NULL)
			{
				return NULL;
			}
			else
			{
				Map<UInt32, UInt64>::Iter * idIter =  mapFindIter->mValue.find(level);
				if(idIter)
				{
					return &idIter->mValue;
				}
				else
				{
					return NULL;
				}
				
			}
		}

		void clear()
		{
			mMap.clear();
			mReverseFindMap.clear();
			mRhymeStepMaxMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx


#endif //__LYNX_COMMON_LIB_RHYMELEVEL_TABLE_H__