#ifndef __LYNX_COMMON_LIB_SERVANTASSISTBATTLE_TABLE_H__
#define __LYNX_COMMON_LIB_SERVANTASSISTBATTLE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{

	struct ServantAssistBattleTemplate 
	{
		UInt64 mId;
		UInt32 mPos;
		UInt32 mLevel;
		UInt32 mQuality;
		UInt32 mCost;
		UInt32 mAttrType;
		UInt32 mAttrValue;
		UInt32 mPower;

		LYNX_S11N_8(ServantAssistBattleTemplate, UInt64, mId, UInt32 , mPos ,
			    UInt32, mLevel, UInt32, mQuality, UInt32, mCost, UInt32, mAttrType,
                UInt32, mAttrValue,UInt32, mPower
			);

	};

	typedef Map<UInt64, ServantAssistBattleTemplate> ServantAssistBattleTemplateMap;

	typedef Map<UInt32, Map<UInt32, ServantAssistBattleTemplate> > ReverseAssistBattleTemplateMap;


	struct ServantAssistBattleTable
	{
		ServantAssistBattleTemplateMap mMap;

		ReverseAssistBattleTemplateMap mReverseMap;

		LYNX_S11N_2(ServantAssistBattleTable, ServantAssistBattleTemplateMap, mMap, ReverseAssistBattleTemplateMap, mReverseMap);

		ServantAssistBattleTemplate* get(UInt32 id)
		{
			ServantAssistBattleTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		ServantAssistBattleTemplate* reverseGet(UInt32 pos, UInt32 qualityLv)
		{
			ReverseAssistBattleTemplateMap::Iter * reverseIter = mReverseMap.find(pos);
			if(!reverseIter)
			{
				return NULL;
			}

			Map<UInt32, ServantAssistBattleTemplate>::Iter * assistBattleIter = reverseIter->mValue.find(qualityLv);

			if(!assistBattleIter)
			{
				return NULL;
			}

			return &assistBattleIter->mValue;

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

#endif //__LYNX_COMMON_LIB_SERVANTASSISTBATTLE_TABLE_H__