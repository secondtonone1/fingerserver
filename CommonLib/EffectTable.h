#ifndef __LYNX_COMMON_LIB_EFFECT_TABLE_H__
#define __LYNX_COMMON_LIB_EFFECT_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct EffectTemplate 
	{
		UInt64 mId;
		UInt32 mResType;
		UInt32 mType;
		UInt32 mPlayType;
		String mAnchorPoint;
		UInt32 mReference;

		String mOffset;
		UInt32 mPlaySpeed;

		UInt32 mFrameStart;
		UInt32 mFrameCount;
		
		String mRes;

		UInt32 mInterval;

		LYNX_S11N_12(EffectTemplate, UInt64, mId, UInt32, mResType,UInt32, mType,
			UInt32, mPlayType,String,mAnchorPoint,UInt32, mReference,
			String,mOffset,UInt32, mPlaySpeed,UInt32, mFrameStart,
			UInt32, mFrameCount,String, mRes,UInt32, mInterval
			);

	};

	typedef Map<UInt64, EffectTemplate> EffectTemplateMap;


	struct EffectTable
	{
		EffectTemplateMap mMap;


		LYNX_S11N_1(EffectTable, EffectTemplateMap, mMap);

		EffectTemplate* get(UInt32 id)
		{
			EffectTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_EFFECT_TABLE_H__