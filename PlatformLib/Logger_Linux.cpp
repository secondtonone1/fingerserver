#if defined(__linux__) && !defined(__ANDROID__)

#include "Logger.h"
#include "Mutex.h"
#include "Utilex.h"
#include "TimeUtil.h"
#include "ThreadMsgQueue.h"
#include <execinfo.h>

int
LynxVsnprintf(char* _DstBuf, size_t _SizeInBytes, const char* _Format, va_list _ArgList)
{
    return vsnprintf(_DstBuf, _SizeInBytes, _Format, _ArgList);
}

using namespace Lynx;

bool 
Logger::initial(UInt32 outputType, bool outputFileName, bool append)
{
    // 创建AssertFile
    if (mAssertOutputFile == NULL)
    {
        mAssertOutputFile = fopen("assert.log", "w+");
        if (mAssertOutputFile == NULL)
        {
            return false;
        }
    }
    mOutputFileName = outputFileName;
    mAppend = append;
    mOutputType = outputType;
    setPagination();

    if (mOutputType == OUTPUT_THREAD)
    {
        mThreadMsgQueue = XNEW(ThreadMsgQueue)();
        ASSERT(mThreadMsgQueue);
        startup();
    }
    return true;
}

void 
Logger::release()
{
    if (mOutputType == OUTPUT_THREAD)
    {
        shutdown();
        join();
        if (mThreadMsgQueue)
        {
            XSAFE_DELETE(mThreadMsgQueue);
        }
    }
    else if (mOutputType == OUTPUT_FILE)
    {
        fclose(mOutputFile);
        mOutputFile = NULL;
    }

    if (mAssertOutputFile)
    {
        fclose(mAssertOutputFile);
        mAssertOutputFile = NULL;
    }
}

void 
Logger::dispatchThreadMsg(ThreadMsg* msg)
{
    if (mOutputFile)
    {
        int nBytes = 0;
        while (1)
        {
            StreamBuffer::Node* node = msg->mBody.popNode();
            if (node == NULL)
            {
                break;
            }
            nBytes = fwrite((char*)node + sizeof(StreamBuffer::Node) + node->mOff, 
                1, node->mLen, mOutputFile);
            if (nBytes != node->mLen)
            {
                msg->mBody.deallocNode(node);
                break;
            }
            msg->mBody.deallocNode(node);
        }

        int ret = fflush(mOutputFile);
        ASSERT(!ret);
    }

    if (ftell(mOutputFile) >= mPaginationSize)
    {
        
        char tmpFilePath[256];
        snprintf(tmpFilePath, sizeof(tmpFilePath), "%s(%d).log", mOutputFilePath, mOutputFileIndex + 1);
        FILE* tmpFilePointer = fopen(tmpFilePath, "w+");
        if (tmpFilePointer)
        {
            fclose(mOutputFile);
            mOutputFile = tmpFilePointer;
            mOutputFileIndex++;
        }
    }
}

void 
Logger::setOutputFile(const char* filePath)
{
    switch (mOutputType)
    {
    case OUTPUT_STDERR:
        snprintf(mOutputFilePath, sizeof(mOutputFilePath), "STDERR");
        mOutputFile = stderr;
        break;
    case OUTPUT_VSOUTPUT:
        break;
    case OUTPUT_FILE:
        {
            ASSERT(strlen(filePath) < 256);
            String fileName = lynxGetFileName(filePath);
            String mainFileName = lynxGetMainFileName(fileName);
            snprintf(mOutputFilePath, sizeof(mOutputFilePath), "%s", mainFileName.c_str());

            char tmpFilePath[256];
            snprintf(tmpFilePath, sizeof(tmpFilePath), "%s(%d).log", mOutputFilePath, mOutputFileIndex++);

            if (mOutputFile)
            {
                fclose(mOutputFile);
            }

            if (mAppend)
            {
                mOutputFile = fopen(tmpFilePath, "a+");
            }
            else
            {
                mOutputFile = fopen(tmpFilePath, "w+");
            }
            ASSERT(mOutputFile);
        }
        break;
    case OUTPUT_THREAD:
        {
            ASSERT(strlen(filePath) < 256);
            String fileName = lynxGetFileName(filePath);
            String mainFileName = lynxGetMainFileName(fileName);
            snprintf(mOutputFilePath, sizeof(mOutputFilePath), "%s", mainFileName.c_str());

            char tmpFilePath[256] = { 0 };
            snprintf(tmpFilePath, sizeof(tmpFilePath), "%s(%d).log", mOutputFilePath, mOutputFileIndex++);

            if (mOutputFile)
            {
                fclose(mOutputFile);
            }

            if (mAppend)
            {
                mOutputFile = fopen(tmpFilePath, "a+");
            }
            else
            {
                mOutputFile = fopen(tmpFilePath, "w+");
            }
            ASSERT(mOutputFile);
        }
        break;
    default:
        ASSERT(0);
    };
}

void
Logger::logString(const char* contents)
{
    if (!contents)
    {
        return;
    }

    switch (mOutputType)
    {
    case OUTPUT_STDERR:
        break;
    case OUTPUT_VSOUTPUT:
        break;
    case OUTPUT_FILE:
        {
            if (!mOutputFile) return;
            SpinLock lock(mSpinMutex);
            fprintf(mOutputFile, "%s", contents);
            fflush(mOutputFile);

            if (ftell(mOutputFile) >= mPaginationSize)
            {
                char tmpFilePath[256] = { 0 };
                snprintf(tmpFilePath, sizeof(tmpFilePath), "%s(%d).log", mOutputFilePath, mOutputFileIndex++);
                fclose(mOutputFile);
                mOutputFile = fopen(tmpFilePath, "w+");
                ASSERT(mOutputFile);
            }
        }
        break;
    case OUTPUT_THREAD:
        break;
    default:
        ASSERT(0);
    };
}

void 
Logger::assertFunc(const char* file, Int32 line)
{
    time_t t;
    time(&t);

    // localtime 在linux下为非线程安全函数,　所以多线程下要上琐
	struct tm *tim = localtime(&t);
    if (tim == NULL)
    {
        ASSERT(tim);
        return;
    }

    if (!mAssertOutputFile)
    {
        return;
    }

    String fileName = "";
    fileName = lynxGetFileName(file);

    String funcStack = getFuncCallStack();

    SpinLock lock(mAssertSpinMutex);
    fprintf(mAssertOutputFile, "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i ASSERT [%s:%i] %s", 
        tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour,
        tim->tm_min, tim->tm_sec, fileName.c_str(), line, funcStack.c_str());
    fprintf(mAssertOutputFile, "\n");
    fflush(mAssertOutputFile);
}

String 
Logger::getFuncCallStack()
{
    char retValue[1024] = { 0 };
    void* stack[32];
    int stackDepth = backtrace(stack, 32);
    for (int i = 0; i < stackDepth; i++)
    {
        sprintf(retValue, "%s%p,", retValue, stack[i]);
    }
    return retValue;
}

void 
Logger::logFunc(const char* subsys, const char* file, Int32 line, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

    time_t t;
    time(&t);
	// localtime 在linux下为非线程安全函数,　所以多线程下要上琐
	struct tm *tim = localtime(&t);
    if (tim == NULL)
    {
        ASSERT(tim);
        return;
    }

    String fileName = "";

    if (mOutputFileName)
    {
        fileName = lynxGetFileName(file);
    }

    switch (mOutputType)
    {
    case OUTPUT_STDERR:
        {
            if (!mOutputFile) return;
            if (mOutputFileName)
            {
                fprintf(mOutputFile, "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i %s [%s:%i] ", 
                    tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour,
                    tim->tm_min, tim->tm_sec, subsys, fileName.c_str(), line);
            }
            else
            {
                fprintf(mOutputFile, "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i %s [%i] ", 
                    tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour,
                    tim->tm_min, tim->tm_sec, subsys, line);
            }
            vfprintf(mOutputFile, format, ap);
            fprintf(mOutputFile, "\n"); 
            fflush(mOutputFile);
        }
        break;
    case OUTPUT_VSOUTPUT:
        break;
    case OUTPUT_FILE:
       {
            if (!mOutputFile) return;

            SpinLock lock(mSpinMutex);
            if (mOutputFileName)
            {
                fprintf(mOutputFile, "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i %s [%s:%i] ", 
                    tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour,
                    tim->tm_min, tim->tm_sec, subsys, fileName.c_str(), line);
            }
            else
            {
                fprintf(mOutputFile, "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i %s [%i] ", 
                    tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour,
                    tim->tm_min, tim->tm_sec, subsys, line);
            }
            vfprintf(mOutputFile, format, ap);
            fprintf(mOutputFile, "\n"); 
            fflush(mOutputFile);

            if (ftell(mOutputFile) >= mPaginationSize)
            {
                char tmpFilePath[256] = { 0 };
                snprintf(tmpFilePath, sizeof(tmpFilePath), "%s(%d).log", mOutputFilePath, mOutputFileIndex++);
                fclose(mOutputFile);
                mOutputFile = fopen(tmpFilePath, "w+");
                ASSERT(mOutputFile);
            }
        }
        break;
    case OUTPUT_THREAD:
        {
            char buffer[LYNX_LOG_MAX_BUFFER_LEN];
            unsigned int len1, len2;
            if (mOutputFileName)
            {
                snprintf(buffer, sizeof(buffer), "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i %s [%s:%i] ", 
                    tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour,
                    tim->tm_min, tim->tm_sec, subsys, fileName.c_str(), line);
            }
            else
            {
                snprintf(buffer, sizeof(buffer), "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i %s [%i] ", 
                    tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour,
                    tim->tm_min, tim->tm_sec, subsys, line);
            }
            len1 = strlen(buffer);
            LynxVsnprintf(buffer + len1, sizeof(buffer) - len1 - 1, format, ap);
            len2 = strlen(buffer);

            buffer[len2] = '\n';
            buffer[len2 + 1] = 0;

            ThreadMsg* threadMsg = XNEW(ThreadMsg)(0, 0);
            threadMsg->mBody.write(buffer, len2 + 1);
            mThreadMsgQueue->postMsg(threadMsg);
        }
        break;
    default:
        ASSERT(0);
    };
    va_end(ap);
}

#endif
