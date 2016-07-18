#include "TimeManager.h"
#include "PlatformLib/PlatformLib.h"
using namespace Lynx;

bool TimeManager::timeIsToday(UInt64 timeParam)
{

	UInt32 nowDays = TimeUtil::getDate(-1);
	 
	UInt32 updateDays = TimeUtil::getDate(timeParam);

	if(nowDays == updateDays)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

bool TimeManager::timeIsOneDay(UInt64 curtime, UInt64 srctime)
{
	UInt32 nowDays = TimeUtil::getDate(curtime);

	UInt32 srcDays = TimeUtil::getDate(srctime);

	if(nowDays == srcDays)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

bool TimeManager::timeIsToWeek(UInt64  timeParam)
{

	UInt32 nowDays = TimeUtil::getDate(-1)/7;

	UInt32 updateDays = TimeUtil::getDate(timeParam)/7;

	if(nowDays == updateDays)
	{
		return true;
	}
	else
	{
		return false;
	}	
	
}
//hours 小时如4
bool TimeManager::timeIsTodayDelayHours(UInt64 timeParam,UInt32 hours)
{
	
	UInt32 nowDays = TimeUtil::getDate((TimeUtil::getTimeSec() - hours*60*60));

	UInt32 updateDays = TimeUtil::getDate((timeParam - hours*60*60));

	if(nowDays == updateDays)
	{
		return true;
	}
	else
	{
		return false;
	}	
}
//time 秒
bool TimeManager::timeIsSameMonth(UInt32 time)
{

	time_t _t = TimeUtil::getTimeSec()*1000;	
	struct tm now_tm;
	struct tm _tm;

	TimeUtil::localTime(now_tm, _t);	

	TimeUtil::localTime(_tm, time*1000);

	if (now_tm.tm_year != _tm.tm_year)//year和mon顺序不能错
	{
		return false;
	}

	if (now_tm.tm_mon != _tm.tm_mon)
	{
		return false;
	}

	return true;
}
//从0开始
Int32 TimeManager::getMonth(Int32 timeSec)
{
	time_t _t;
	if (timeSec == - 1)
	{
		_t = TimeUtil::getTimeSec();	
	}
	else
	{
		_t = timeSec;
	}
	
	struct tm _tm;

	TimeUtil::localTime(_tm, _t);

	return  _tm.tm_mon;
}
