#ifndef __LYNX_COMMON_LIB_COPY_FINISH_TIME_TABLE_H__
#define __LYNX_COMMON_LIB_COPY_FINISH_TIME_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct CopyFinishTemplate
	{
					
		//关卡ID
		UInt32 stageid;
		//buffer个数
		UInt32 buffnum;
		
		//最大用时
		UInt32 maxfinishtime;
		//系统纪录用时
		UInt32 systemrecordtime;
		//是否要纪录
		UInt32 needrecord;
		//最大攻击力
		List<UInt32> attackpower;
		//最低用时
		list<UInt32> needfinishtime;
		CopyFinishTemplate():stageid(0),buffnum(0),maxfinishtime(0),systemrecordtime(0),needrecord(0){}
 		LYNX_S11N_4(CopyFinishTemplate,UInt32, stageid,UInt32, buffnum,UInt32, maxfinishtime,UInt32, systemrecordtime);
	};

	typedef List<CopyFinishTemplate> mFinishTimeList;

	struct CopyFinishTable
	{	
		mFinishTimeList mList;

		LYNX_S11N_1(CopyFinishTable, mFinishTimeList, mList);

		void clear()
		{
			mList.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);
	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_COPY_FINISH_TIME_TABLE_H__


