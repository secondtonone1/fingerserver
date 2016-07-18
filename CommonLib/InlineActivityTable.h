#ifndef __LYNX_COMMON_LIB_INLINEACTIVITY_TABLE_H__
#define __LYNX_COMMON_LIB_INLINEACTIVITY_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"
#include "Award.h"

namespace Lynx
{
	
	struct InlineActivityTemplate 
	{

		UInt32 id;

		UInt32 type;

		UInt32 activitytype;

		UInt32 neednum;

		String name;

		String award1;

		String award2;
		String award3;
		UInt32 begintime;
		UInt32 endtime;
		UInt32 daystart;
		UInt32 dayend;
		UInt32 lv;
		UInt32 gold;
		UInt32 month;
		UInt32 viplv;
		UInt32 time;

		UInt32 taskid1;
		UInt32 taskid2;
		UInt32 taskid3;
		UInt32 taskid4;
		UInt32 cost;

		LYNX_S11N_22(InlineActivityTemplate,UInt32, id,UInt32, type,
		UInt32, activitytype,UInt32, neednum,String, name,String, award1,String, award2,
		String, award3,UInt32, begintime,UInt32, endtime,UInt32, daystart,UInt32, dayend,
		UInt32, lv,UInt32, gold,UInt32, month,UInt32, viplv,UInt32, time,UInt32, taskid1,
		UInt32, taskid2,UInt32, taskid3,UInt32, taskid4,UInt32, cost);

	};

	typedef Map<UInt64, InlineActivityTemplate> InlineActivityTemplateMap;


	struct InlineActivityTable
	{
		InlineActivityTemplateMap mMap;

		LYNX_S11N_1(InlineActivityTable, InlineActivityTemplateMap, mMap);

		InlineActivityTemplate* get(UInt32 id)
		{
			InlineActivityTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_INLINEACTIVITY_TABLE_H__