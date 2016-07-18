#ifndef __LYNX_UTIL_LIB_STRING_CONVERTER_H__
#define __LYNX_UTIL_LIB_STRING_CONVERTER_H__

#include "PlatformLib.h"

namespace Lynx
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup General
	*  @{
	*/
	/** Class for converting the core Ogre data types to/from Strings.
    @remarks
        The code for converting values to and from strings is here as a separate
        class to avoid coupling String to other datatypes (and vice-versa) which reduces
        compilation dependency: important given how often the core types are used.
    @par
        This class is mainly used for parsing settings in text files. External applications
        can also use it to interface with classes which use the StringInterface template
        class.
    @par
        The String formats of each of the major types is listed with the methods. The basic types
        like int and float just use the underlying C runtime library atof and lynxAtoi family methods,
        however custom types like Vector3, ColourValue and Matrix4 are also supported by this class
        using custom formats.
    @author
        Steve Streeting
    */

    class _PlatformExport StringConverter
    {
    public:
        typedef Vector<String> StringVector;
        /** Converts a float to a String. */
        static String toString(float val, unsigned short precision = 6, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        /** Converts an int to a String. */
        static String toString(int val, unsigned short width = 0, 
            char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));

        /** Converts an unsigned int to a String. */
        static String toString(unsigned int val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        /** Converts a long to a String. */
        static String toString(long val, 
            unsigned short width = 0, char fill = ' ', 
            std::ios::fmtflags flags = std::ios::fmtflags(0));
        /** Converts a boolean to a String. 
        @param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
        */
        static String toString(bool val, bool yesNo = false);
		/** Converts a Vector2 to a String. 
        @remarks
            Format is "x y" (i.e. 2x float values, space delimited)
        */
        static String toString(const Vector2& val);
        /** Converts a Vector3 to a String. 
        @remarks
            Format is "x y z" (i.e. 3x float values, space delimited)
        */
        static String toString(const Vector3& val);

        /** Converts a StringVector to a String.
        @remarks
            Strings must not contain spaces since space is used as a delimiter in
            the output.
        */
        static String toString(const StringVector& val);

        /** Converts a String to a float. 
        @returns
            0.0 if the value could not be parsed, otherwise the float version of the String.
        */
        static float parsefloat(const String& val, float defaultValue = 0);
        /** Converts a String to a whole number. 
        @returns
            0.0 if the value could not be parsed, otherwise the numeric version of the String.
        */
        static int parseInt(const String& val, int defaultValue = 0);
        /** Converts a String to a whole number. 
        @returns
            0.0 if the value could not be parsed, otherwise the numeric version of the String.
        */
        static unsigned int parseUnsignedInt(const String& val, unsigned int defaultValue = 0);
        /** Converts a String to a whole number. 
        @returns
            0.0 if the value could not be parsed, otherwise the numeric version of the String.
        */
        static long parseLong(const String& val, long defaultValue = 0);
        /** Converts a String to a whole number. 
        @returns
            0.0 if the value could not be parsed, otherwise the numeric version of the String.
        */
        static unsigned long parseUnsignedLong(const String& val, unsigned long defaultValue = 0);
        /** Converts a String to a boolean. 
        @remarks
            Returns true if case-insensitive match of the start of the String
			matches "true", "yes" or "1", false otherwise.
        */
        static bool parseBool(const String& val, bool defaultValue = 0);
		/** Parses a Vector2 out of a String.
        @remarks
            Format is "x y" ie. 2 float components, space delimited. Failure to parse returns
            Vector2::ZERO.
        */
		static Vector2 parseVector2(const String& val, const Vector2& defaultValue = Vector2::ZERO);
		/** Parses a Vector3 out of a String.
        @remarks
            Format is "x y z" ie. 3 float components, space delimited. Failure to parse returns
            Vector3::ZERO.
        */
        static Vector3 parseVector3(const String& val, const Vector3& defaultValue = Vector3::ZERO);

        /** Pareses a StringVector from a String.
        @remarks
            Strings must not contain spaces since space is used as a delimiter in
            the output.
        */
        static StringVector parseStringVector(const String& val);
        /** Checks the String is a valid number value. */
        static bool isNumber(const String& val);

        /** Returns a StringVector that contains all the substrings delimited
        by the characters in the passed <code>delims</code> argument.
        @param
        delims A list of delimiter characters to split by
        @param
        maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
        parameters is > 0, the splitting process will stop after this many splits, left to right.
        */
        static StringVector split(const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);

        /** Returns whether the String begins with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the start of the String will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

        static const String BLANK;
    };

}
#endif //__LYNX_UTIL_LIB_STRING_CONVERTER_H__