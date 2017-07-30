#ifndef __LYNX_PLATFORM_LIB_TIME_H__
#define __LYNX_PLATFORM_LIB_TIME_H__

#include "MemoryManager.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LEGAL_TIME(t)       (t >= 0 && t <=2713881599u)

#define TIMERCMP(tvp, uvp, cmp)                                 \
        (((tvp)->tv_sec == (uvp)->tv_sec) ?                     \
        ((tvp)->tv_usec cmp (uvp)->tv_usec) :                   \
        ((tvp)->tv_sec cmp (uvp)->tv_sec))

#define TIMERADD(tvp, uvp, vvp)                                 \
do {                                                            \
        (vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;          \
        (vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;       \
        if ((vvp)->tv_usec >= 1000000) {                        \
                (vvp)->tv_sec++;                                \
                (vvp)->tv_usec -= 1000000;                      \
        }                                                       \
} while (0)

#define TIMERSUB(tvp, uvp, vvp)                                 \
do {                                                            \
        (vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;          \
        (vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;       \
        if ((vvp)->tv_usec < 0) {                               \
                (vvp)->tv_sec--;                                \
                (vvp)->tv_usec += 1000000;                      \
        }                                                       \
} while (0)

struct Timeval {
        long    tv_sec;         /* seconds */
        long    tv_usec;        /* and microseconds */
};

struct Timezone {
        int     tz_minuteswest; /* minutes west of Greenwich */
        int     tz_dsttime;     /* type of dst correction */
};

#ifdef __cplusplus
}
#endif

namespace Lynx
{
    class _PlatformExport TimeUtil
    {
    public:
        static void SetTimeOffset(Int64 timeOffset);

        /********** 
         *基础函数* 
         **********/

        // 取距离UTC Epoch过去过去的秒数相当于当前时区的日期, _t缺省时表示当前时刻
        // _t取值范围: [0, 2713881599] ([1970.1.1 08:00:00, 2055.12.31 23:59:59])
        static bool localTime(struct tm& tm_time, time_t _t = -1);

        // 去当前时区某个日期对应距离UTC Epoch过去了多少秒
        // _tm取值范围: [1971.1.1 00:00:00, 2055.12.31 23:59:59]
        static time_t makeTime(struct tm& _tm);

        // 取当前时刻距UTC Epoch过去过去了多少秒
        static time_t time();

        // 拿秒/毫秒/微秒(UTC)
        static UInt64 getTimeMicroSec();
        static UInt64 getTimeMilliSec();
        static UInt32 getTimeSec();

        static UInt64 getLocalTimeMilliSec();
        static UInt32 getLocalTimeSec();

        
        /********** 
         *日期无关* 
         **********/

        // 以下函数 _t 的取值范围和localTime相同
        // 取当前时区当日00:00:00时刻距UTC Epoch过去过去了多少秒, 0表示无效值
        static UInt32 getTodayBaseSec(time_t _t = -1);

        // 取当前时区今日距UTC Epoch过去过去了多少天, 0表示无效值
        static UInt32 getDateNum(time_t _t = -1);

        // 取当前时区当前时刻为当前时区今日第几秒, -1表示无效值
        static Int32 getSecNum(time_t _t = -1);

        // 将当前时区当前时刻用四位数字形式表示, 如17:34 -> 1734, 08:02 -> 802, -1表示无效值
        static Int32 getTime(time_t _t = -1);


        /********** 
         *日期相关* 
         **********/

        // 以下函数 _t 的取值范围和localTime相同
        // 取字符串格式时间戳
        static bool getTimestamp(char* buf, Int32 size, UInt64 millisec);

        // 取数字格式时间戳(包含年月日时分), 0表示无效值
        static UInt64 getTimestamp64(time_t _t = -1);

        // 取字符串格式星期(Mon,Tue,Wed,Thu,Fri,Sat,Sun), Err表示无效值
        static String getDay(time_t _t = -1);

        // 取数字格式星期(0-6, 0为周日), -1表示无效值
        static Int32 getDayNum(time_t _t = -1);

		// 取数字格式星期(1-7), -1表示无效值
		static Int32
		getSevenDayNum(time_t _t/* = -1*/);

        // 取四位数字格式日期(年月), -1表示无效值
        static Int32 getDate(time_t _t = -1);


        /********** 
         *  其他  * 
         **********/
        static UInt32 getTimeOfDay(Timeval* tv, Timezone* tz);
        static void sleep(UInt64 millisec);

		static Int32 getSecOfToday();
		//王戊辰，将秒数转化为当天的秒数

		static	Int32 getSecOfToThatday(UInt32 timeSec);
		static Int32 convertSecToTodaySec(time_t  seconds);

		//王戊辰, 将秒数转化为当天的小时
		static Int32 convertSecToTodayHour(time_t  seconds);

		//获得时间字符串
		static bool getTimeStr(char* buf, Int32 size, UInt64 ms);

#if defined (_WIN32)
        static UInt32 getTickCount();
#endif
        static UInt64 getSystemTime();
	private:
		static Int64 sTimeOffset;
    };

	class Tick
    {
    public:
        Tick();
        virtual ~Tick();

        bool initial();
        void release();
        void update();

        // 获取上一帧花费的时间
        inline float getDeltaTime() const { return mDeltaTime; }
        // 得到累计时间, 从程序启动开始计算
        inline float getAccumulateTime() const { return mAccumulateTime; }
        // 获得帧率
        inline UInt32 getFps() const { return (UInt32)mFps; }
        // 得到帧数
        inline UInt64 getFrameCount() const { return mFrameCount; }
        
    private:
		// 进程启动时间记录
		UInt64 mStartTime;
        // 当前时间(毫秒, 从启动开始累计)
        UInt32 mCurrentTime;
        // 最近20ms以来每帧平均花费的时间
        float mLoopTime;
        // 上一帧花费的时间
        float mDeltaTime;
        // 当前帧率
        float mFps;
        // 当前总帧数
        UInt64 mFrameCount;
        // 上一帧时间(毫秒)
        UInt32 mLastTime;
        // 帧累计计数
        UInt32 mLoopCnt;
        // 时间累计技术
        UInt32 mLoopHop;
        // 程序启动后累计时间
        float mAccumulateTime;
    };

} // namespace KGame 

#endif // __LYNX_PLATFORM_LIB_TIME_H__


