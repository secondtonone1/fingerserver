#include "Utilex.h"
#if defined(_WIN32)
# include <windows.h>
# include <winbase.h>
# include <sys/timeb.h>
#else
# include <sys/time.h>
# include <unistd.h>
#endif

#include <iostream>
#include <cassert>
#include <time.h>

#include "TimeUtil.h"

using namespace Lynx;

Int64 TimeUtil::sTimeOffset = 0;

const char Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static int day_tab[2][12]={{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}}; 

int myleap(int year) 
{ 
    if(year%4==0 && year%100!=0 || year%400==0) 
        return 1; 
    else 
        return 0; 
}

#ifdef _WIN32
static int
gettimeofday(struct timeval* tv, void* tz)
{
    struct _timeb tb;

    if (tv == NULL) return -1;

    _ftime(&tb);
    tv->tv_sec = (long)tb.time;
    tv->tv_usec = ((int)tb.millitm) * 1000;
    return 0;
}
#endif 

void
TimeUtil::SetTimeOffset(Int64 timeOffset)
{
    sTimeOffset = timeOffset;
}

bool
TimeUtil::localTime(struct tm& tm_time, time_t _t/* = -1*/)
{
    if (_t == -1)
    {
        _t = time();
    }
    else if (!LEGAL_TIME(_t))
    {
        return false;
    }

    UInt32 n32_Pass4year;
    UInt32 n32_hpery;

    tm_time.tm_isdst = 0;

    //计算时差
#if defined (_WIN32)
    TIME_ZONE_INFORMATION zi;
    GetTimeZoneInformation(&zi);
    _t -= zi.Bias * 60;
#else
    struct timezone tz;
    Int32 result = gettimeofday(NULL, &tz);
    _t -= tz.tz_minuteswest * 60;
#endif

    //先把星期算出来
    int days = _t/86400;
    tm_time.tm_wday = (days % 7 + 4) >= 7 ? (days % 7 + 4 - 7) : (days % 7 + 4);
    //取秒时间
    tm_time.tm_sec=(int)(_t % 60);
    _t /= 60;
    //取分钟时间
    tm_time.tm_min=(int)(_t % 60);
    _t /= 60;
    //取过去多少个四年，每四年有 1461*24 小时
    n32_Pass4year=((unsigned int)_t / (1461L * 24L));
    //计算年份
    tm_time.tm_year=(n32_Pass4year << 2)+70;
    //四年中剩下的小时数
    _t %= 1461L * 24L;
    //校正闰年影响的年份，计算一年中剩下的小时数
    for (;;)
    {
        //一年的小时数
        n32_hpery = 365 * 24;
        //判断闰年
        if ((tm_time.tm_year & 3) == 0)
        {
            //是闰年，一年则多24小时，即一天
            n32_hpery += 24;
        }
        if (_t < n32_hpery)
        {
            break;
        }
        tm_time.tm_year++;
        _t -= n32_hpery;
    }
    //小时数
    tm_time.tm_hour=(int)(_t % 24);
    //一年中剩下的天数
    _t /= 24;
    tm_time.tm_yday = _t;
    //假定为闰年
    _t++;
    //校正润年的误差，计算月份，日期
    if ((tm_time.tm_year & 3) == 0)
    {
        if (_t > 60)
        {
            _t--;
        }
        else
        {
            if (_t == 60)
            {
                tm_time.tm_mon = 1;
                tm_time.tm_mday = 29;
                tm_time.tm_yday = 59;
                return true;
            }
        }
    }
    //计算月日
    for (tm_time.tm_mon = 0; Days[tm_time.tm_mon] < _t;tm_time.tm_mon++)
    {
        _t -= Days[tm_time.tm_mon];
    }

    tm_time.tm_mday = (int)(_t);

    return true;
}

time_t
TimeUtil::makeTime(struct tm& _tm)
{
    int mon, day, year, hour, min, sec;
    mon = _tm.tm_mon + 1;
    day = _tm.tm_mday;
    year = _tm.tm_year + 1900;
    hour = _tm.tm_hour;
    min = _tm.tm_min;
    sec = _tm.tm_sec;

    // 合法性判断
    if (year < 1971 || year > 2055 ||
        mon < 1 || mon > 12 ||
        day < 1 || day > day_tab[myleap(year)][mon - 1] ||
        hour < 0 || hour > 23 ||
        min < 0 || min > 59 ||
        sec < 0 || sec > 59)
    {
        return -1;
    }

    if (0 >= (int) (mon -= 2)) {    /* 1..12 -> 11,12,1..10 */
        mon += 12;      /* Puts Feb last since it has leap day */
        year -= 1;
    }

    time_t tv;
#if defined (_WIN32)
    TIME_ZONE_INFORMATION zi;
    GetTimeZoneInformation(&zi);
    tv = zi.Bias * 60;
#else
    struct timezone tz;
    Int32 result = gettimeofday(NULL, &tz);
    tv = tz.tz_minuteswest * 60;
#endif

    return (((
        (unsigned long) (year/4 - year/100 + year/400 + 367*mon/12 + day) +
        year*365 - 719499
        )*24 + hour /* now have hours */
        )*60 + min /* now have minutes */
        )*60 + sec /* finally seconds */ + tv;
}

time_t
TimeUtil::time()
{
    return ::time(NULL) + sTimeOffset / 1000;
}

UInt64
TimeUtil::getTimeMicroSec()
{
    return getSystemTime();
}

UInt64
TimeUtil::getTimeMilliSec()
{
    return getSystemTime() / 1000;
}

UInt32
TimeUtil::getTimeSec()
{
    return time();
}

UInt64 
TimeUtil::getLocalTimeMilliSec()
{
#ifdef _WIN32
    return ::timeGetTime();
#else
    return getSystemTime() / 1000;
#endif
}

UInt32 
TimeUtil::getLocalTimeSec()
{
#ifdef _WIN32
    return ::timeGetTime() / 1000;
#else
    return time();
#endif
}

UInt32
TimeUtil::getTodayBaseSec(time_t _t/* = -1*/)
{
    if (_t == -1)
    {
        _t = time();
    }
    else if (!LEGAL_TIME(_t))
    {
        return 0;
    }

    //计算时差
    time_t tv;
#if defined (_WIN32)
    TIME_ZONE_INFORMATION zi;
    GetTimeZoneInformation(&zi);
    tv = zi.Bias * 60;
#else
    struct timezone tz;
    Int32 result = gettimeofday(NULL, &tz);
    tv = tz.tz_minuteswest * 60;
#endif

    _t -= tv;
    UInt32 todaybase = _t - _t % 86400;

    return todaybase + tv;
}

UInt32
TimeUtil::getDateNum(time_t _t/* = -1*/)
{
    if (_t == -1)
    {
        _t = time();
    }
    else if (!LEGAL_TIME(_t))
    {
        return 0;
    }

    //计算时差
#if defined (_WIN32)
    TIME_ZONE_INFORMATION zi;
    GetTimeZoneInformation(&zi);
    _t -= zi.Bias * 60;
#else
    struct timezone tz;
    Int32 result = gettimeofday(NULL, &tz);
    _t -= tz.tz_minuteswest * 60;
#endif

    return _t / 86400;
}

Int32
TimeUtil::getSecNum(time_t _t/* = -1*/)
{
    if (_t == -1)
    {
        _t = time();
    }
    else if (!LEGAL_TIME(_t))
    {
        return -1;
    }

    return _t - getTodayBaseSec(_t);
}

Int32
TimeUtil::getTime(time_t _t/* = -1*/)
{
    Int32 secNum = getSecNum(_t);

    if (secNum == -1)
    {
        return -1;
    }

    Int32 hour = secNum / 3600;
    Int32 min = (secNum % 3600) / 60;

    return hour * 100 + min;
}

bool 
TimeUtil::getTimestamp(char* buf, Int32 size, UInt64 ms)
{
    const time_t timeInSeconds = ms/1000;

    struct tm _tm;
    if (localTime(_tm, timeInSeconds))
    {
        strftime(buf, size, "%Y-%m-%d %H:%M:%S", &_tm);
        return true;
    }
    return false;
}

UInt64
TimeUtil::getTimestamp64(time_t _t/* = -1*/)
{
    if (_t == -1)
    {
        _t = time();
    }

    struct tm _tm;
    if (!localTime(_tm, _t))
    {
        return 0;
    }

    return (UInt64)(_tm.tm_year + 1900) * 100000000 + (_tm.tm_mon + 1) * 1000000 + _tm.tm_mday * 10000 +
        _tm.tm_hour * 100 + _tm.tm_min;
}

String
TimeUtil::getDay(time_t _t/* = -1*/)
{
    if (_t == -1)
    {
        _t = time();
    }
    else if (!LEGAL_TIME(_t))
    {
        return "Err";
    }

    struct tm _tm;
    localTime(_tm, _t);
    char buf[32];
    strftime(buf, sizeof(buf), "%a", &_tm);
    return buf;
}

Int32
TimeUtil::getDayNum(time_t _t/* = -1*/)
{
    if (_t == -1)
    {
        _t = time();
    }
    else if (!LEGAL_TIME(_t))
    {
        return -1;
    }

    struct tm _tm;
    localTime(_tm, _t);
    return _tm.tm_wday;
}

Int32
TimeUtil::getDate(time_t _t/* = -1*/)
{
    if (_t == -1)
    {
        _t = time();
    }
    else if (!LEGAL_TIME(_t))
    {
        return -1;
    }

    struct tm _tm;
    localTime(_tm, _t);
    return (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
}

Int32
TimeUtil::getSecOfToday()
{
	time_t _t = time();	
	struct tm _tm;

	localTime(_tm, _t);

	return  _tm.tm_hour * 3600 + _tm.tm_min * 60 + _tm.tm_sec;
}

UInt32
TimeUtil::getTimeOfDay(Timeval* tv, Timezone* tz)
{
#if defined (_WIN32)
    //struct _timeb tb;
    //
    //if (tv == NULL) return -1;
    //
    //_ftime(&tb);
    //tv->tv_sec = (long)tb.time;
    //tv->tv_usec = ((long)tb.millitm) * 1000;
    //return 0;

    UInt32 currTime = ::timeGetTime();
    tv->tv_sec = currTime / 1000;
    tv->tv_usec = (currTime % 1000) * 1000;
    return 0;
#else
    return gettimeofday((struct timeval *)tv, (struct timezone *)tz);
#endif
}

void
TimeUtil::sleep(UInt64 ms)
{
#if defined (_WIN32)
    ::Sleep((DWORD)ms);
#else
    ::usleep(1000 * ms);
#endif
}

#if defined (_WIN32)
UInt32 
TimeUtil::getTickCount()
{
    return ::GetTickCount();
}
#endif

UInt64
TimeUtil::getSystemTime()
{
    UInt64 time = 0;

    struct timeval now;
    gettimeofday(&now, NULL);
    time = now.tv_sec;

    time = time * 1000000;
    time += now.tv_usec;

    time += sTimeOffset * 1000;
    return time;
}

// 计算时间差， 考虑时间阙值(49.71天)
static UInt32 
calculateTimeDiff(UInt32 startTime, UInt32 endTime)
{
    if (endTime < startTime)
    {
        return ((UInt32)0xFFFFFFFF - startTime) + endTime;
    }
    else
    {
        return endTime - startTime;
    }
}

Tick::Tick() : mLastTime(0), mLoopCnt(0), mLoopHop(0)
{

}

Tick::~Tick()
{

}

bool
Tick::initial()
{
    mLoopTime          = 0.0f;
    mDeltaTime         = 0.0f;
    mFps               = 0.0f;
    mFrameCount        = 0;
    mCurrentTime       = 0;
    mAccumulateTime    = 0.0f;
    mStartTime = TimeUtil::getTimeMilliSec();

    LOG_INFO("Initial Tick success.");
    return true;
}

void
Tick::release()
{
    LOG_INFO("Release Tick.");
}

void
Tick::update()
{
	//桢数加一
	mFrameCount++;

	//记录当前时间
    mCurrentTime = TimeUtil::getTimeMilliSec() - mStartTime;
	//记录距上一桢所花费的时间,考虑跨时间阙的问题(49.71天)
	UInt32 diffTime = calculateTimeDiff(mLastTime, mCurrentTime);
	mDeltaTime = (float)diffTime * 0.001f;

    {
        mLoopCnt++;
        mLoopHop += diffTime;

        if(mLoopHop > 1000) //每1秒计算一次fps
        {
            mFps = (float)(mLoopCnt * 1000) / mLoopHop;
            mLoopCnt = 0;
            mLoopHop = 0;
        }
    }

	mLastTime = mCurrentTime;
    mAccumulateTime = mCurrentTime * 0.001f;
}
