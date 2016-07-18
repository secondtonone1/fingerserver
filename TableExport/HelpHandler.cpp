#include "HelpHandler.h"
#include <windows.h>

using namespace Lynx;

std::wstring 
HelpHandler::s2ws(const String& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

String const 
HelpHandler::to_string(_bstr_t const & b)
{
    char const * const l_s(b);
    if( l_s )
    {
        return String(l_s);
    }
    else
    {
        return String();
    }
}

String
HelpHandler::Replace(const String& src,
                     const String& replaceSrc, 
                     const String& replaceDest)
{
    String tmp(src);
    int pos = tmp.find(replaceSrc);
    tmp.replace(pos, pos+5, replaceDest);
	
    return tmp;
}
