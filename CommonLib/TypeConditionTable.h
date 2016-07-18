#ifndef __LYNX_COMMON_LIB_TYPECONDITION_TABLE_H__
#define __LYNX_COMMON_LIB_TYPECONDITION_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct TypeNum
	{
		UInt32 type;
		UInt32 num;
		LYNX_S11N_2(TypeNum, UInt32, type,UInt32, num);
	};
	struct TypeConditionTemplate 
	{
		UInt32 id;

		String name;
		
		List<TypeNum> typeNums;
		
		LYNX_S11N_3(TypeConditionTemplate, UInt32, id,String, name,List<TypeNum> ,typeNums);

	};

	typedef Map<UInt32, TypeConditionTemplate> TypeConditionTemplateMap;


	struct TypeConditionTable
	{
		TypeConditionTemplateMap mMap;


		LYNX_S11N_1(TypeConditionTable, TypeConditionTemplateMap, mMap);

		TypeConditionTemplate* get(UInt32 id)
		{
			TypeConditionTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_TYPECONDITION_TABLE_H__