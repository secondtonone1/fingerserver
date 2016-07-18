#ifndef __LYNX_PLATFORM_LIB_LOGGER_H__
#define __LYNX_PLATFORM_LIB_LOGGER_H__

#include "Compat.h"
#include "Singleton.h"
#include "Mutex.h"
#include "MemoryManager.h"

/**
 * What shall I log to syslog?
 * 0 - DEBUGs, INFOs, WARNINGs and ERRORs (this is the default)
 * 1 - INFOs, WARNINGs and ERRORs and PRINTs
 * 2 - WARNINGs and ERRORs and PRINTs
 * 3 - only ERRORs and PRINTs
 * 4 - only PRINTs 
 * 5 - absolutely nothing (be careful - you will have no way to
 */

#define LYNX_LOG_MAX_BUFFER_LEN 20480

namespace Lynx
{
    struct ThreadMsg;
    class ThreadMsgQueue;
    class _PlatformExport Logger : public Thread, public Singleton<Logger>
    {
    public:
        enum { OUTPUT_STDERR = 0, OUTPUT_VSOUTPUT, OUTPUT_FILE, OUTPUT_THREAD };

        Logger();
        virtual ~Logger();

        bool initial(UInt32 outputType, bool outputFile = false, bool append = false);
        void release();

        Int32 getSilenceLevel() { return mSilenceLevel; }

        void setPagination(Int32 size = 10/*mega*/);
        void setSilenceLevel(Int32 level);
        void setOutputFile(const char* filePath);
        void logFunc(const char* subsys, const char* file, Int32 line, const char* format, ...);
        void assertFunc(const char* file, Int32 line);
        void logString(const char* contents);
        

    private:
        void threadFunc();
        void dispatchThreadMsg(ThreadMsg* msg);

        String getFuncCallStack();
    private:
        UInt32 mOutputType;
        FILE* mOutputFile;
        FILE* mAssertOutputFile;
        char mOutputFilePath[256];
        SpinMutex mSpinMutex;
        SpinMutex mAssertSpinMutex;
        UInt32 mPaginationSize;
        bool mOutputFileName;
        bool mAppend;
        UInt32 mOutputFileIndex;
        Int32 mSilenceLevel;
        ThreadMsgQueue* mThreadMsgQueue;
    };
} // namespace Lynx

#define LOG_INITIAL(outputType, outputFileName, append) Lynx::Logger::getSingleton().initial(outputType, outputFileName, append)
#define LOG_RELEASE() Lynx::Logger::getSingleton().release()
#define LOG_SET_PAGINATION(size) Lynx::Logger::getSingleton().setPagination(size)
#define LOG_SET_SILENCE_LEVEL(level) Lynx::Logger::getSingleton().setSilenceLevel(level)
#define LOG_SET_OUTPUT_FILE(filePath) Lynx::Logger::getSingleton().setOutputFile(filePath)

/**
 * Variadic Macros notice:
 * Only support for variadic macros was introduced in Visual C++ 2005.
 * Below usage only support by C99, don't compile as a c++ file.
 * There is a expand usage in GCC, it may be complie success both c and c++.
 * eg: ERROR(F...) logFunc(__FILE__, __LINE__, F)
 */

#ifdef _LYNX_DEBUG

#define LOG_DEBUG(...)                                                                  \
do {                                                                                    \
    if (Lynx::Logger::getSingleton().getSilenceLevel() < 1)                             \
        Lynx::Logger::getSingleton().logFunc("DEBUG", __FILE__, __LINE__, __VA_ARGS__); \
} while (0)

#else

#define LOG_DEBUG ;

#endif

#define LOG_INFO(...)                                                                   \
do {                                                                                    \
    if (Lynx::Logger::getSingleton().getSilenceLevel() < 2)                             \
        Lynx::Logger::getSingleton().logFunc("INFOS", __FILE__, __LINE__, __VA_ARGS__); \
} while (0)

#define LOG_WARN(...)                                                                   \
do {                                                                                    \
    if (Lynx::Logger::getSingleton().getSilenceLevel() < 3)                             \
        Lynx::Logger::getSingleton().logFunc("WARNS", __FILE__, __LINE__, __VA_ARGS__); \
} while (0)

#define LOG_ERROR(...)                                                                  \
do {                                                                                    \
    if (Lynx::Logger::getSingleton().getSilenceLevel() < 4)                             \
        Lynx::Logger::getSingleton().logFunc("ERROR", __FILE__, __LINE__, __VA_ARGS__); \
} while (0)

#define LOG_PRINT(...)                                                                  \
do {                                                                                    \
    if (Lynx::Logger::getSingleton().getSilenceLevel() < 5)                             \
        Lynx::Logger::getSingleton().logFunc("PRINT", __FILE__, __LINE__, __VA_ARGS__); \
} while (0)


#ifdef _LYNX_DEBUG
//#define ASSERT(args) assert((args))
#define ASSERT(args);
#elif _WIN32
#define ASSERT(args);
#else
#define ASSERT(args) if (!(args)) Lynx::Logger::getSingleton().assertFunc(__FILE__, __LINE__)
#endif

#endif // __LYNX_PLATFORM_LIB_LOGGER_H__


/**
 * Log Usage: 
 * int main()
 * {
 *       LOG_INIT(false, false);
 *       Log to a specfiy file. Default output to stderr.
 *       LOG_OUTPUT_FILE(OUTPUT_TYPE, "aaa");
 *
 *       Set silence level, n = 0/1/2/3/4
 *       LOG_SILENCE_LEVEL(n)
 *
 *       LOG_DEBUG("debug log test...");
 *       LOG_INFOS("infos log test...");
 *       LOG_WARNS("warns log test...");
 *       LOG_ERROR("error log test...");
 *       LOG_PRINT("print log test...");
 *       return 0;
 * }
 */



