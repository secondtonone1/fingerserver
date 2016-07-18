#include "IConvUtil.h"
#include "Logger.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace Lynx;

IConvUtil::IConvUtil() 
{
#ifdef _LYNX_HAS_LIBICONV
    mGb2312ToUtf8Id = iconv_open("UTF-8", "GB2312");
    if (mGb2312ToUtf8Id == reinterpret_cast<iconv_t>(-1))
    {
        LOG_WARN("Failed to call iconv_open from gb2312 to utf8");
    }

    mUtf8ToGb2312Id = iconv_open("GB2312", "UTF-8");
    if (mUtf8ToGb2312Id == reinterpret_cast<iconv_t>(-1))
    {
        LOG_WARN("Failed to call iconv_open from utf8 to gb2312");
        if (mGb2312ToUtf8Id != reinterpret_cast<iconv_t>(-1))
        {
            iconv_close(mGb2312ToUtf8Id);
        }
    }
#endif
}
        
IConvUtil::~IConvUtil()
{
#ifdef _LYNX_HAS_LIBICONV
    if (mGb2312ToUtf8Id != reinterpret_cast<iconv_t>(-1))
    {
        iconv_close(mGb2312ToUtf8Id);
    }

    if (mUtf8ToGb2312Id != reinterpret_cast<iconv_t>(-1))
    {
        iconv_close(mUtf8ToGb2312Id);
    }
#endif
}

void
IConvUtil::gb2312ToUnicode(const BasicString& inStr, BasicStringW& outStr)
{
#ifdef _LYNX_HAS_LIBICONV
    
#elif defined(_WIN32)
    Int32 len = MultiByteToWideChar(936, 0, inStr.c_str(), -1, 0, 0);
    if (!len)
    {
        LOG_WARN("Failed to convert string %s from Gb2312 to Unicode.", inStr.c_str());
        return;
    }

    wchar_t* buff = NULL;
    bool allocFlag = false;
    if (sizeof(wchar_t) * len >= ICONV_CONST_BUFFER_LEN)
    {
        buff = (wchar_t*)XALLOC(sizeof(wchar_t)* len);
        allocFlag = true;
    }
    else
    {
        buff = (wchar_t*)mConstConvBuffer;
    }

    MultiByteToWideChar(936, 0, inStr.c_str(), -1, 
        reinterpret_cast<LPWSTR>(buff), len);

    outStr = buff;

    if (allocFlag)
    {
        XFREE(buff);
    }
#endif
}

void
IConvUtil::unicodeToGb2312(const BasicStringW& inStr, BasicString& outStr)
{
#ifdef _LYNX_HAS_LIBICONV

#elif defined(_WIN32)
    Int32 len = WideCharToMultiByte(936, 0, 
        reinterpret_cast<LPCWSTR>(inStr.c_str()), -1, 0, 0, 0, 0);
    if (!len)
    {
        LOG_WARN("Failed to convert string %s from Unicode to Gb2312.", inStr.c_str());
        return;
    }

    char* buff = NULL;
    bool allocFlag = false;
    if (sizeof(char) * len >= ICONV_CONST_BUFFER_LEN)
    {
        buff = (char*)XALLOC(sizeof(char)* len);
        allocFlag = true;
    }
    else
    {
        buff = (char*)mConstConvBuffer;
    }

    WideCharToMultiByte(936, 0, reinterpret_cast<LPCWSTR>(inStr.c_str()),
        -1, reinterpret_cast<char*>(buff), len, 0, 0);

    outStr = buff;

    if (allocFlag)
    {
        XFREE(buff);
    }
#endif
}

void
IConvUtil::utf8ToUnicode(const BasicString& inStr, BasicStringW& outStr)
{
#ifdef _LYNX_HAS_LIBICONV

#elif defined(_WIN32)
    Int32 len = MultiByteToWideChar(CP_UTF8, 0, inStr.c_str(), -1, 0, 0);
    if (!len)
    {
        LOG_WARN("Failed to convert string %s from Utf8 to Unicode.", inStr.c_str());
        return;
    }

    wchar_t* buff = NULL;
    bool allocFlag = false;
    if (sizeof(wchar_t) * len >= ICONV_CONST_BUFFER_LEN)
    {
        buff = (wchar_t*)XALLOC(sizeof(wchar_t)* len);
        allocFlag = true;
    }
    else
    {
        buff = (wchar_t*)mConstConvBuffer;
    }

    MultiByteToWideChar(CP_UTF8, 0, inStr.c_str(), -1, 
        reinterpret_cast<LPWSTR>(buff), len);

    outStr = buff;

    if (allocFlag)
    {
        XFREE(buff);
    }
#endif
}

void
IConvUtil::unicodeToUtf8(const BasicStringW& inStr, BasicString& outStr)
{
#ifdef _LYNX_HAS_LIBICONV

#elif defined(_WIN32)
    Int32 len = WideCharToMultiByte(CP_UTF8, 0, 
        reinterpret_cast<LPCWSTR>(inStr.c_str()), -1, 0, 0, 0, 0);
    if (!len)
    {
        LOG_WARN("Failed to convert string %s from Unicode to Utf8.", inStr.c_str());
        return;
    }

    char* buff = NULL;
    bool allocFlag = false;
    if (sizeof(char) * len >= ICONV_CONST_BUFFER_LEN)
    {
        buff = (char*)XALLOC(sizeof(char)* len);
        allocFlag = true;
    }
    else
    {
        buff = (char*)mConstConvBuffer;
    }

    WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<LPCWSTR>(inStr.c_str()),
        -1, reinterpret_cast<char*>(buff), len, 0, 0);

    outStr = buff;

    if (allocFlag)
    {
        XFREE(buff);
    }
#endif
}

void
IConvUtil::gb2312ToUtf8(const BasicString& inStr, BasicString& outStr)
{
#ifdef _LYNX_HAS_LIBICONV
    if (mGb2312ToUtf8Id == reinterpret_cast<iconv_t>(-1))
    {
        return;
    }

    const char* inbuf = inStr.c_str();
    size_t inlen = inStr.length();

    char* outbuf = NULL;
    char* tmpOutbuf = NULL;
    size_t outlen = sizeof(char)* inlen * 6 + 1;
    bool allocFlag = false;
    if (outlen >= ICONV_CONST_BUFFER_LEN)
    {
        tmpOutbuf = outbuf = (char*)XALLOC(outlen);
        allocFlag = true;
    }
    else
    {
        tmpOutbuf = outbuf = (char*)mConstConvBuffer;
    }

    if (iconv(mGb2312ToUtf8Id, const_cast<char**>(&inbuf), &inlen, &outbuf, &outlen) == -1)
    {
        LOG_WARN("Failed to call iconv from gb2312 to utf8");
        if (allocFlag)
        {
            XFREE(tmpOutbuf);
        }
        return;
    }

    if (inlen == 0)
    {
        *outbuf = 0;
        outStr = tmpOutbuf;
    }

    if (allocFlag)
    {
        XFREE(tmpOutbuf);
    }

#elif defined(_WIN32)
    BasicStringW unicodeStr;
    gb2312ToUnicode(inStr, unicodeStr);
    unicodeToUtf8(unicodeStr, outStr);
#endif
}

void
IConvUtil::utf8ToGb2312(const BasicString& inStr, BasicString& outStr)
{
#ifdef _LYNX_HAS_LIBICONV
    if (mUtf8ToGb2312Id == reinterpret_cast<iconv_t>(-1))
    {
        return;
    }

    const char* inbuf = inStr.c_str();
    size_t inlen = inStr.length();

    char* outbuf = NULL;
    char* tmpOutbuf = NULL;
    size_t outlen = sizeof(char)* inlen * 6 + 1;
    bool allocFlag = false;
    if (outlen >= ICONV_CONST_BUFFER_LEN)
    {
        tmpOutbuf = outbuf = (char*)XALLOC(outlen);
        allocFlag = true;
    }
    else
    {
        tmpOutbuf = outbuf = (char*)mConstConvBuffer;
    }

    if (iconv(mUtf8ToGb2312Id, const_cast<char**>(&inbuf), &inlen, &outbuf, &outlen) == -1)
    {
        LOG_WARN("Failed to call iconv from utf8 to gb2312");
        if (allocFlag)
        {
            XFREE(tmpOutbuf);
        }
        return;
    }

    if (inlen == 0)
    {
        *outbuf = 0;
        outStr = tmpOutbuf;
    }

    if (allocFlag)
    {
        XFREE(tmpOutbuf);
    }
#elif defined(_WIN32)
    BasicStringW unicodeStr;
    utf8ToUnicode(inStr, unicodeStr);
    unicodeToGb2312(unicodeStr, outStr);
#endif
}
