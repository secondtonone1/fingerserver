#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_TIME_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_TIME_MAMAGER_H__

#include <time.h>
#include "PlatformLib/PlatformLib.h"

namespace Lynx
{
	class TimeManager: public Singleton<TimeManager>
	{
	public:
		//判断某个时间是否是处于今天
		static bool timeIsToday(UInt64 timeParam); 

		//判断两个时间是否处于同一天
		static bool timeIsOneDay(UInt64 curtime, UInt64 srctime);
		//判断某个时间是否处于本星期
		static bool timeIsToWeek(UInt64  timeParam);
		//判断某个时间是否是凌晨4点后才不是同一天
		static bool timeIsTodayDelayHours(UInt64 timeParam,UInt32 hours); 
		//判断时间是否和现在是同一个月
		static bool timeIsSameMonth(UInt32 time);
		//获取当前是几月 
		static Int32 getMonth(Int32 time);
		
	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_TIME_MAMAGER_H__