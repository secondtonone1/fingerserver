#ifndef __LYNX_PLATFORM_LIB_UTILEX_H__
#define __LYNX_PLATFORM_LIB_UTILEX_H__

#include "Container.h"

namespace Compat
{
    template <bool Boolean, class Type = void>
    struct EnableIf { typedef Type type; }; 
    template <class Type> struct EnableIf<false, Type > {};

    template <class Type> struct IsInteger { static const bool sValue = false; };
#if !defined __x86_64__
    template <> struct IsInteger<long> { static const bool sValue = true; };
#endif
    template <> struct IsInteger<Lynx::Int8> { static const bool sValue = true; };
    template <> struct IsInteger<Lynx::Int16> { static const bool sValue = true; };
    template <> struct IsInteger<Lynx::Int32> { static const bool sValue = true; };
    template <> struct IsInteger<Lynx::Int64> { static const bool sValue = true; };

    template <class Type> struct IsStdBasicString { static const bool sValue = false; };
    template <class Type, class Traits, class AllocatorType>
    struct IsStdBasicString<std::basic_string<Type, Traits, AllocatorType > >
    { static const bool sValue = true; };

    template <class Type, class Enable = void>
    struct ItoaTraits
    {
        static bool lynxItoa(Type n, Lynx::Int8* buf, Lynx::UInt32 buflen, Lynx::Int32 radix)
        {
            if (buf == 0) return false;

            char* p = buf;

            do 
            {
                *p++ = "0123456789"[n % radix];
                n /= radix;
                if ((p - buf) > buflen) return 0;
            } while (n);

            buf[p - buf] = 0;

            int i, j;
            for (i = 0, j = (int)strlen(buf) - 1; i < j; i++, j--)
            {
                buf[j] = buf[i] + buf[j];
                buf[i] = buf[j] - buf[i];
                buf[j] = buf[j] - buf[i];
            }
            return false;
        }
    };

    template <class Type >
    struct ItoaTraits<Type, typename EnableIf<IsStdBasicString<Type >::sValue>::type>
    {
        static bool lynxItoa(Type n, Lynx::Int8* buf, Lynx::UInt32 buflen, Lynx::Int32 radix)
        {
            Lynx::Int32 len = n.length();
            if (len >= buflen) 
            {
                return false;
            }
            snprintf(buf, buflen, "%s", n.c_str());
            buf[len] = 0;
            return true;
        }
    };

    template <class Type >
    struct ItoaTraits<Type, typename EnableIf<IsInteger<Type >::sValue>::type>
    {
        static bool lynxItoa(Type n, Lynx::Int8* buf, Lynx::UInt32 buflen, Lynx::Int32 radix)
        {
            if (buf == 0) return false;

            bool sign = true;
            if (n < 0)
            {
                sign = false;
                n = -n;
            }

            Lynx::Int8* p = sign ? buf : buf + 1;

            do 
            {
                *p++ = "0123456789"[n % radix];
                n /= radix;
                if ((p - buf) > buflen) return 0;
            } while (n);

            buf[p - buf] = 0;


            int i, j;
            for (i = i = sign ? 0 : 1, j = (int)strlen(buf) - 1; i < j; i++, j--)
            {
                buf[j] = buf[i] + buf[j];
                buf[i] = buf[j] - buf[i];
                buf[j] = buf[j] - buf[i];
            }

            if (sign == false)
            {
                buf[0] = '-';
            }

            return true;
        }
    };
} // namespace Compat


template <typename Type>
Type lynxAtoi(const char* s)
{
    Type n;
    int i, sign;

    ASSERT(s);

    while ((*s == 32 || *s == 9)) s++;
    sign = (*s == '-') ? -1 : 1;
    while (*s == '-') s++;
    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
        n = 10 * n + (s[i] - '0');
    return sign * n;
}

#define lynxAtof atof

// 返回0表示成功
template <typename Type>
bool lynxItoa(Type n, Lynx::Int8* buf, Lynx::UInt32 buflen, Lynx::Int32 radix)
{
    return Compat::ItoaTraits<Type >::lynxItoa(n, buf, buflen, radix);
}

// 仅支持String
template <typename Type>
void lynxStrTrim(Type& src)
{
    Lynx::Int32 i = 0;
    Lynx::Int32 tailPos = 0;

    for (Lynx::Int32 cpyPos = i; i < src.length(); i++)
    {
        if (src[i] != 32 && src[i] != 9)
        {
            src[cpyPos] = src[i];
            cpyPos++;
            tailPos = cpyPos;
        }
        else if (tailPos)
        {
            src[cpyPos] = src[i];
            cpyPos++;
        }
    }

    if (tailPos) 
    {
        src.resize(tailPos);
    }
}

// 仅支持String 与const char*
template <typename Type>
void lynxStrSplit(Type& src, const Lynx::String& sep, Lynx::Vector<Lynx::String>& dst, bool trim = false)
{
    Lynx::String s;
    Lynx::String tmpSrc = src; // TODO: Optimized for performance
    for (Lynx::String::iterator it = tmpSrc.begin(); it != tmpSrc.end(); it++)
    {
        if (sep.find(*it) != Lynx::String::npos)
        {
            if (s.length()) dst.push_back(s);
            s = "";
        }
        else
        {
            s += *it;
        }
    }

    if (s.length())
    {
        if(trim)
        {
            lynxStrTrim(s);
        }
        dst.push_back(s);
    }
}

// 类型Type == String
template <typename Type>
void lynxStrReplace(Type& strOutput, const Lynx::String& strsrc, const Lynx::String& strdst)
{
    Lynx::String::size_type pos=0;
    Lynx::String::size_type srclen=strsrc.size();
    Lynx::String::size_type dstlen=strdst.size();
    while((pos = strOutput.find(strsrc, pos)) != Lynx::String::npos)
    {
        strOutput.erase(pos, srclen);
        strOutput.insert(pos, strdst);
        pos += dstlen;
    }
}

// Type must be const char*
template <typename Type> 
Lynx::String lynxGetFileName(Type fullName)
{
    if (fullName == 0){ return "";}
    Lynx::String name = fullName;
    Lynx::Int32 index  = -1;
    Lynx::Int32 index2 = -1;
    index  = name.find_last_of("/");
    index2 = name.find_last_of("\\");
    if (index == -1 && index2 == -1)
    {
        return name;
    }
    if (index2 > index)
    {
        return name.substr(index2 + 1, name.length());
    }
    else if(index2 < index)
    {
        return name.substr(index + 1, name.length());
    }
    return "";
}

// Type must be const char*
template <typename Type>
Lynx::String lynxGetFilePath(Type fullName)
{
    if (fullName == 0) { return ""; }
    Lynx::String name = fullName;
    Lynx::Int32 index  = -1;
    Lynx::Int32 index2 = -1;
    index  = name.find_last_of("/");
    index2 = name.find_last_of("\\");
    if (index == -1 && index2 == -1)
    {
        return "";
    }
    if (index2 > index)
    {
        return name.substr(0, index2 + 1);
    }
    else if(index2 < index)
    {
        return name.substr(0, index + 1);
    }
    return "";
}

// Type must be const char*
template <typename Type>
Lynx::String lynxGetMainFileName(Type fileName, bool absolute = true)
{
    Lynx::String tempFileName = fileName;
    Lynx::Vector<Lynx::String> strVector;
    lynxStrSplit(tempFileName, ".", strVector);
    int vectorSize = strVector.size();
    if (vectorSize == 0)
    {
        return "";
    }

    switch (vectorSize)
    {
    case 1:
    case 2:
        return strVector[0];
    default:
        {
            Lynx::String retStr;
            if (absolute)
            {
                retStr = strVector[0];
            }
            else
            {
                retStr = strVector[0];
                for (Lynx::UInt32 i = 1; i < vectorSize - 1; i++)
                {
                    retStr += "." + strVector[i];
                }
            }
            return retStr;
        }
    };
}

// Type must be const char*
template <typename Type>
Lynx::String lynxGetExtFileName(Type fileName, bool absolute = true)
{
    Lynx::String tempFileName = fileName;
    Lynx::Vector<Lynx::String> strVector;
    lynxStrSplit(tempFileName, ".", strVector);
    int vectorSize = strVector.size();
    if (vectorSize <= 1) 
    {
        return "";
    }

    if (absolute)
    {
        return strVector[vectorSize - 1];
    }
    else
    {
        Lynx::String retStr = strVector[1];
        for (Lynx::UInt32 i = 2; i < vectorSize; i++)
        {
            retStr += "." + strVector[i];
        }
        return retStr;
    }
}

template <typename Type>
Lynx::String lynxGetMacroFileName(const Type& str)
{
    Lynx::String out;
    Lynx::UInt32 n = 0;
    for (Lynx::String::const_iterator it = str.begin();
        it != str.end(); ++it)
    {
        if (*it >= 65 && *it <= 90)
        {
            if (n == 0)
            {
                out.push_back(*it);
            }
            else
            {
                out.push_back('_');
                out.push_back(*it);
            } 
            n++;
        }
        else if (*it >= 'a' && *it <= 'z')
        {
            out.push_back(*it - ('a' - 'A'));
        }
    }
    return out;
}

template <typename Type>
void lynxToLowerCase(Type& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

template <typename Type>
void lynxToUpperCase(Type& str)
{
    std::transform(str.begin(), str.end(), str.begin(), toupper);
}

/*
    if(lynxStringMatch(fileName, pattern, isCaseSensitive))
    {
        // do your logic.
    }
*/
template <typename Type>
bool lynxStringMatch(Type str, Type pattern, bool caseSensitive)
{
    Lynx::String tmpStr = str;
    Lynx::String tmpPattern = pattern;
    if (!caseSensitive)
    {
        lynxToLowerCase(tmpStr);
        lynxToLowerCase(tmpPattern);
    }

    Lynx::String::const_iterator strIt = tmpStr.begin();
    Lynx::String::const_iterator patIt = tmpPattern.begin();
    Lynx::String::const_iterator lastWildCardIt = tmpPattern.end();
    while (strIt != tmpStr.end() && patIt != tmpPattern.end())
    {
        if (*patIt == '*')
        {
            lastWildCardIt = patIt;
            // Skip over looking for next character
            ++patIt;
            if (patIt == tmpPattern.end())
            {
                // Skip right to the end since * matches the entire rest of the string
                strIt = tmpStr.end();
            }
            else
            {
                // scan until we find next pattern character
                while(strIt != tmpStr.end() && *strIt != *patIt)
                    ++strIt;
            }
        }
        else
        {
            if (*patIt != *strIt)
            {
                if (lastWildCardIt != tmpPattern.end())
                {
                    // The last wildcard can match this incorrect sequence
                    // rewind pattern to wildcard and keep searching
                    patIt = lastWildCardIt;
                    lastWildCardIt = tmpPattern.end();
                }
                else
                {
                    // no wildwards left
                    return false;
                }
            }
            else
            {
                ++patIt;
                ++strIt;
            }
        }

    }
    // If we reached the end of both the pattern and the string, we succeeded
    if (patIt == tmpPattern.end() && strIt == tmpStr.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif // __LYNX_PLATFORM_LIB_UTILEX_H__


