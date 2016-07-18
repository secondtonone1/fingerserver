#ifndef __LYNX_COMMON_LIB_ATTRRANDOM_TABLE_H__
#define __LYNX_COMMON_LIB_ATTRRANDOM_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	
	struct AttrRandomTemplate 
	{
		UInt64 mId;
		UInt32  mLevel;
		UInt32 mQuality;

		UInt32 mPAMin;
		UInt32 mPAMax;
		UInt32 mPARate;

		UInt32 mMAMin;
		UInt32 mMAMax;
		UInt32 mMARate;

		UInt32 mPFMin;
		UInt32 mPFMax;
		UInt32 mPFRate;

		UInt32 mMFMin;
		UInt32 mMFMax;
		UInt32 mMFRate;

		UInt32 mMaxHPMin;
		UInt32 mMaxHPMax;
		UInt32 mMaxHPRate;

		UInt32 mMaxMPMin;
		UInt32 mMaxMPMax;
		UInt32 mMaxMPRate;

		UInt32 mMaxSPMin;
		UInt32 mMaxSPMax;
		UInt32 mMaxSPRate;

		UInt32 mCritRateMin;
		UInt32 mCritRateMax;
		UInt32 mCritRateRate;

		UInt32 mCritDamageMin; //暴击几率万分比最小值
		UInt32 mCritDamageMax; //暴击几率万分比最大值
		UInt32 mCritDamageRate;

		UInt32 mCritDefMin;
		UInt32 mCritDefMax;
		UInt32 mCritDefRate;

		UInt32 mEtcMin;
		UInt32 mEtcMax;
		UInt32 mEtcRate;

		LYNX_S11N_36(AttrRandomTemplate, UInt64, mId, UInt32, mLevel,UInt32, mQuality,
			UInt32, mPAMin,UInt32,mPAMax,UInt32, mPARate,UInt32,mMAMin,UInt32, mMAMax,
			UInt32 ,mMARate,UInt32 ,mPFMin,UInt32 ,mPFMax,UInt32, mPFRate,UInt32, mMFMin,
			UInt32 ,mMFMax,UInt32, mMFRate,UInt32,mMaxHPMin,UInt32,mMaxHPMax,UInt32 ,mMaxHPRate,
			UInt32, mMaxMPMin, UInt32, mMaxMPMax,UInt32, mMaxMPRate,UInt32, mMaxSPMin,UInt32,
			mMaxSPMax,UInt32, mMaxSPRate,UInt32, mCritRateMin,UInt32, mCritRateMax,UInt32, mCritRateRate,
			UInt32, mCritDamageMin,UInt32, mCritDamageMax,UInt32, mCritDamageRate,UInt32, mCritDefMin,
			UInt32, mCritDefMax, UInt32, mCritDefRate, UInt32, mEtcMin, UInt32, mEtcMax,UInt32, mEtcRate
			);

	};

	typedef Map<UInt64, AttrRandomTemplate> AttrRandomTemplateMap;
	typedef Map<UInt32,Map<UInt32,UInt64> > AttrRandomLvQualityMap;

	struct AttrRandomTable
	{
		AttrRandomTemplateMap mMap;
		AttrRandomLvQualityMap mLvQualityMap;

		LYNX_S11N_2(AttrRandomTable, AttrRandomTemplateMap, mMap,AttrRandomLvQualityMap,mLvQualityMap);

		AttrRandomTemplate* get(UInt64 id)
		{
			AttrRandomTemplateMap::Iter* iter = mMap.find(id);
			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		UInt64 getAttrRandomID(UInt32 level, UInt32 quality)
		{
			AttrRandomLvQualityMap::Iter * iter = mLvQualityMap.find(level);
			Map<UInt32,UInt64>::Iter * iterSec = iter->mValue.find(quality);
			return iterSec->mValue;
		}

		void clear()
		{
			mMap.clear();
			mLvQualityMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ATTRRANDOM_TABLE_H__