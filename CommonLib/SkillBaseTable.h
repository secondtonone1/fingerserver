#ifndef __LYNX_COMMON_LIB_SKILLLBASE_TABLE_H__
#define __LYNX_COMMON_LIB_SKILLLBASE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{

	struct SkillBaseTemplate 
	{
		UInt64 mSkillID;
		String mName;
		String mDesc;

		UInt32 mType;
		UInt32 mSubType;
		UInt32 mTarget;

		UInt32 mMaxLevel;
		UInt32 mFrameStart;
		UInt32 mFrameCount;

		UInt32 mEffectId;
		String mResPrefix;
		String mIcon;

		String mAnchorPoint;
		UInt32 mControl;
		UInt32 mControlSpeed;

		UInt32 mLoopNum;
		UInt32 mLoopFrameStart;
		UInt32 mLoopFrameEnd;

		UInt32 mCanTurnFace;
		
		UInt32 width;
		UInt32 height;



		LYNX_S11N_21(SkillBaseTemplate, UInt64, mSkillID,String, mName,String, mDesc,
			UInt32, mType,UInt32, mSubType,UInt32, mTarget,
			UInt32, mMaxLevel,UInt32, mFrameStart,UInt32, mFrameCount,
			UInt32, mEffectId,String, mResPrefix,String, mIcon,
			String, mAnchorPoint,UInt32, mControl,UInt32, mControlSpeed,
			UInt32, mLoopNum,UInt32, mLoopFrameStart,UInt32, mLoopFrameEnd,
			UInt32, mCanTurnFace,UInt32, width,UInt32, height
			);

	};

	typedef Map<UInt64, SkillBaseTemplate> SkillBaseTemplateMap;

	struct SkillBaseTable
	{
		SkillBaseTemplateMap mMap;

		LYNX_S11N_1(SkillBaseTable, SkillBaseTemplateMap, mMap);

		SkillBaseTemplate* get(UInt32 id)
		{
			SkillBaseTemplateMap::Iter* iter = mMap.find(id);

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

#endif