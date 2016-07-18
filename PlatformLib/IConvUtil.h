#ifndef __LYNX_PLATFORM_LIB_ICONV_LIB_H__
#define __LYNX_PLATFORM_LIB_ICONV_LIB_H__

#include "Compat.h"
#include "MemoryManager.h"

#ifdef _LYNX_HAS_LIBICONV
#include "iconv.h"
#endif

#define ICONV_CONST_BUFFER_LEN 512

namespace Lynx
{
    class IConvUtil
    {
    public:
        IConvUtil();
        ~IConvUtil();

        void gb2312ToUtf8(const BasicString& inStr, BasicString& outStr);
        void utf8ToGb2312(const BasicString& inStr, BasicString& outStr);

    private:
        void gb2312ToUnicode(const BasicString& inStr, BasicStringW& outStr);
        void unicodeToGb2312(const BasicStringW& inStr, BasicString& outStr);
        void utf8ToUnicode(const BasicString& inStr, BasicStringW& outStr);
        void unicodeToUtf8(const BasicStringW& inStr, BasicString& outStr);

    private:
#ifdef _LYNX_HAS_LIBICONV
        iconv_t mGb2312ToUtf8Id;
        iconv_t mUtf8ToGb2312Id;
#endif
        char mConstConvBuffer[ICONV_CONST_BUFFER_LEN];
    };
} // namespace Lynx;


#endif // __LYNX_PLATFORM_LIB_ICONV_LIB_H__


