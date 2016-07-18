#ifndef __HELP_HANDLER__
#define __HELP_HANDLER__

#include "PlatformLib.h"
#include <string>
#include <comutil.h>

namespace Lynx
{
    class HelpHandler
    {
    public:
        // String to std::wstring
        static std::wstring s2ws(const String& s);

        // _bstr_t to String
        static String const to_string(_bstr_t const & b);

        // Ìæ»»×Ö·û´®
        static String Replace(const String& src, 
            const String& replaceSrc, const String& replaceDest);
    };

}; // namespace Lynx

#endif //__HELP_HANDLER__