#ifndef __LYNX_COMMON_LIB_RHYMACUPOINT_TABLE_H__
#define __LYNX_COMMON_LIB_RHYMACUPOINT_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"


namespace Lynx
{
	struct RhymeAcupointTemplate 
	{
		UInt64 mId;
		String mName;
		UInt32 mActiveNeedGrass;
		UInt32 mActiveNeedStep;
		UInt32 mActiveNeedLevel;
		UInt32 mattr1;
		UInt32 mattr2;
		UInt32 mattr3;
		UInt32 mattr4;
		UInt32 mattr5;
		UInt32 mGrassType;

		LYNX_S11N_11(RhymeAcupointTemplate, UInt64, mId,String, mName,UInt32, mActiveNeedGrass,UInt32, mActiveNeedStep,
			UInt32,mActiveNeedLevel,UInt32, mattr1,UInt32, mattr2,UInt32, mattr3,UInt32, mattr4,UInt32, mattr5, UInt32, mGrassType	
			);

	};

	typedef Map<UInt64, RhymeAcupointTemplate> RhymeAcupointTemplateMap;
	
	struct RhymeAcupointTable
	{
		RhymeAcupointTemplateMap mMap;
		
		LYNX_S11N_1(RhymeAcupointTable, RhymeAcupointTemplateMap, mMap);

		RhymeAcupointTemplate* get(UInt64 id)
		{
			RhymeAcupointTemplateMap::Iter* iter = mMap.find(id);

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


#endif //__LYNX_COMMON_LIB_RHYMELEVEL_TABLE_H__