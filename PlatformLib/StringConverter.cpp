#include "StringConverter.h"

#include <sstream>
#include <ctype.h>

using namespace Lynx;

const String StringConverter::BLANK;
//-----------------------------------------------------------------------
String StringConverter::toString(float val, unsigned short precision, 
                                 unsigned short width, char fill, std::ios::fmtflags flags)
{
    StringStream stream;
    stream.precision(precision);
    stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}
//-----------------------------------------------------------------------
String StringConverter::toString(int val, unsigned short width, char fill, std::ios::fmtflags flags)
{
    StringStream stream;
    stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}
//-----------------------------------------------------------------------
String StringConverter::toString(unsigned int val, 
                                      unsigned short width, char fill, std::ios::fmtflags flags)
{
    StringStream stream;
    stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}
String StringConverter::toString(long val, 
                                      unsigned short width, char fill, std::ios::fmtflags flags)
{
    StringStream stream;
    stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}
//-----------------------------------------------------------------------
String StringConverter::toString(const Vector2& val)
{
    StringStream stream;
    stream << val.mX << " " << val.mY;
    return stream.str();
}
//-----------------------------------------------------------------------
String StringConverter::toString(const Vector3& val)
{
    StringStream stream;
    stream << val.mX << " " << val.mY << " " << val.mZ;
    return stream.str();
}
//-----------------------------------------------------------------------
String StringConverter::toString(bool val, bool yesNo)
{
    if (val)
    {
        if (yesNo)
        {
            return "yes";
        }
        else
        {
            return "true";
        }
    }
    else
        if (yesNo)
        {
            return "no";
        }
        else
        {
            return "false";
        }
}

//-----------------------------------------------------------------------
String StringConverter::toString(const StringConverter::StringVector& val)
{
    StringStream stream;
    StringConverter::StringVector::const_iterator i, iend, ibegin;
    ibegin = val.begin();
    iend = val.end();
    for (i = ibegin; i != iend; ++i)
    {
        if (i != ibegin)
            stream << " ";

        stream << *i; 
    }
    return stream.str();
}
//-----------------------------------------------------------------------
float StringConverter::parsefloat(const String& val, float defaultValue)
{
    // Use istringstream for direct correspondence with toString
    StringStream str(val);
    float ret = defaultValue;
    str >> ret;

    return ret;
}
//-----------------------------------------------------------------------
int StringConverter::parseInt(const String& val, int defaultValue)
{
    // Use istringstream for direct correspondence with toString
    StringStream str(val);
    int ret = defaultValue;
    str >> ret;

    return ret;
}
//-----------------------------------------------------------------------
unsigned int StringConverter::parseUnsignedInt(const String& val, unsigned int defaultValue)
{
    // Use istringstream for direct correspondence with toString
    StringStream str(val);
    unsigned int ret = defaultValue;
    str >> ret;

    return ret;
}
//-----------------------------------------------------------------------
long StringConverter::parseLong(const String& val, long defaultValue)
{
    // Use istringstream for direct correspondence with toString
    StringStream str(val);
    long ret = defaultValue;
    str >> ret;

    return ret;
}
//-----------------------------------------------------------------------
unsigned long StringConverter::parseUnsignedLong(const String& val, unsigned long defaultValue)
{
    // Use istringstream for direct correspondence with toString
    StringStream str(val);
    unsigned long ret = defaultValue;
    str >> ret;

    return ret;
}
//-----------------------------------------------------------------------
bool StringConverter::parseBool(const String& val, bool defaultValue)
{
    if ((startsWith(val, "true") || startsWith(val, "yes")
        || startsWith(val, "1")))
        return true;
    else if ((startsWith(val, "false") || startsWith(val, "no")
        || startsWith(val, "0")))
        return false;
    else
        return defaultValue;
}
//-----------------------------------------------------------------------
Vector2 StringConverter::parseVector2(const String& val, const Vector2& defaultValue)
{
    // split on space
    Vector<String> vec = split(val);

    if (vec.size() != 2)
    {
        return defaultValue;
    }
    else
    {
        return Vector2(parsefloat(vec[0]),parsefloat(vec[1]));
    }
}
//-----------------------------------------------------------------------
Vector3 StringConverter::parseVector3(const String& val, const Vector3& defaultValue)
{
    // split on space
    Vector<String> vec = split(val);

    if (vec.size() != 3)
    {
        return defaultValue;
    }
    else
    {
        return Vector3(parsefloat(vec[0]),parsefloat(vec[1]),parsefloat(vec[2]));
    }
}
//-----------------------------------------------------------------------
StringConverter::StringVector StringConverter::parseStringVector(const String& val)
{
    return split(val);
}
//-----------------------------------------------------------------------
bool StringConverter::isNumber(const String& val)
{
    StringStream str(val);
    float tst;
    str >> tst;
    return !str.fail() && str.eof();
}

StringConverter::StringVector StringConverter::split(const String& str, const String& delims, unsigned int maxSplits)
{
    StringVector ret;
    // Pre-allocate some space for performance
    ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

    unsigned int numSplits = 0;

    // Use STL methods 
    size_t start, pos;
    start = 0;
    do 
    {
        pos = str.find_first_of(delims, start);
        if (pos == start)
        {
            // Do nothing
            start = pos + 1;
        }
        else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
        {
            // Copy the rest of the String
            ret.push_back(str.substr(start));
            break;
        }
        else
        {
            // Copy up to delimiter
            ret.push_back(str.substr(start, pos - start));
            start = pos + 1;
        }
        // parse up to next real data
        start = str.find_first_not_of(delims, start);
        ++numSplits;

    } while (pos != String::npos);



    return ret;
}

bool StringConverter::startsWith(const String& str, const String& pattern, bool lowerCase)
{
    size_t thisLen = str.length();
    size_t patternLen = pattern.length();
    if (thisLen < patternLen || patternLen == 0)
        return false;

    String startOfThis = str.substr(0, patternLen);
    if (lowerCase)
    {
        String lowerCasePattern = pattern;
        lynxToLowerCase(lowerCasePattern);
        lynxToLowerCase(startOfThis);
        return (startOfThis == lowerCasePattern);
    }

    return (startOfThis == pattern);
}