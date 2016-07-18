#ifndef __LYNX_UTIL_LIB_CSVREADER_H__
#define __LYNX_UTIL_LIB_CSVREADER_H__

#include <fstream>
#include "EncodingConvert.h"

namespace Lynx
{
    class CsvReader
    {
    public:
        CsvReader(std::istream& input, String sep = ",") : mInputStream(input), mFieldSep(sep) {}
        ~CsvReader(){}

        bool getLine(String& line)
        {
            if(!mInputStream.good())
                return false;
            char c;
            for (mLine = ""; mInputStream.get(c) && !endOfLine(c);)
                mLine += c;
            split();
            line = mLine;
            return !mInputStream.eof();
        }

        bool readLine()
        {
            if(!mInputStream.good() || mInputStream.eof())
                return false;
            char c;
            for (mLine = ""; mInputStream.get(c) && !endOfLine(c);)
                mLine += c;

            if(split() == 0)
                return false;
            return true;
        }

        bool initTitle()
        {
            if(!readLine()) return false;
            if(mFields.size() == 0) return false;
            for(int i = 0; i < mFields.size(); i++)
            {
                String titleName = mFields[i];
                lynxStrTrim(titleName);
                std::transform(titleName.begin(), titleName.end(), titleName.begin(), ::tolower);
                mColumns.insert(titleName, i);
            }
            return true;
        }

        bool bind(const String& title, String& val)
        {
            String titleName = title;
            std::transform(titleName.begin(), titleName.end(), titleName.begin(), ::tolower);
            Columns::ConstIter* cit = mColumns.find(titleName);
            if (cit == mColumns.end())
            {
                return false;
            }
            val = mFields[cit->mValue];
            lynxStrTrim(val);

            std::locale langLocale("");
            std::locale old = std::locale::global(langLocale);
            String dst = EncodingConvert::ansi2Utf8(val);
            std::locale::global(old);
            val = dst;
            //mIConvUtil.gb2312ToUtf8(val, val);
            return true;
        }

        bool bind(const String& title, Vector3& val)
        {
            String titleName = title;
            std::transform(titleName.begin(), titleName.end(), titleName.begin(), ::tolower);
            Columns::ConstIter* cit = mColumns.find(titleName);
            if (cit == mColumns.end())
            {
                return false;
            }

            String tmpStr = mFields[cit->mValue];
            Vector<String> tmpVector;
            lynxStrSplit(tmpStr, ",", tmpVector, true);

            if (tmpVector.size() != 3)
            {
                return false;
            }
            val.mX = (float)atof(tmpVector[0].c_str()) * 0.75f;
            val.mY = (float)atof(tmpVector[1].c_str()) * 0.75f;
            val.mZ = (float)atof(tmpVector[2].c_str()) * 0.75f;
            return true;
        }

        bool bind(const String& title, double& val)
        {
            val = 0;
            String titleName = title;
            std::transform(titleName.begin(), titleName.end(), titleName.begin(), ::tolower);
            Columns::ConstIter* cit = mColumns.find(titleName);
            if (cit == mColumns.end())
            {
                return false;
            }
            String strVal = mFields[cit->mValue];
            lynxStrTrim(strVal);

            if (strVal.empty())
            {
                return true;
            }

            val = atof(strVal.c_str());
            return true;
        }

        bool bindFloat(const String& title, float& val)
        {
            val = 0;
            String titleName = title;
            std::transform(titleName.begin(), titleName.end(), titleName.begin(), ::tolower);
            Columns::ConstIter* cit = mColumns.find(titleName);
            if (cit == mColumns.end())
            {
                return false;
            }
            String strVal = mFields[cit->mValue];
            lynxStrTrim(strVal);

            if (strVal.empty())
            {
                return true;
            }

            val = atof(strVal.c_str());
            return true;
        }
        
        template <typename Type>
        bool bind(const String& title, Type& val)
        {
            val = 0;
            String titleName = title;
            std::transform(titleName.begin(), titleName.end(), titleName.begin(), ::tolower);
            Columns::ConstIter* cit = mColumns.find(titleName);
            if (cit == mColumns.end())
            {
                return false;
            }
            String strVal = mFields[cit->mValue];
            lynxStrTrim(strVal);

            if (strVal.empty())
            {
                return true;
            }
            val = lynxAtoi<Type>(strVal.c_str());
            return true;
        }

        bool bindBool(const String& title, bool& val)
        {
            int intVal;
            if (!bind(title, intVal))
            {
                return false;
            }
            val = intVal == 0 ? false : true;
            return true;
        }

        String& getField(UInt32 index)
        {
            return mFields[index];
        }

        UInt32 getFieldNumber() const { return mFieldNumber; }

    private:
        UInt32 split()
        {
            String fld;
            Int32 i, j;
            mFieldNumber = 0;
            if (mLine.length() == 0)
                return 0;
            i = 0;
            do {
                if (i < mLine.length() && mLine[i] == '"')
                    j = advQuoted(mLine, fld, ++i);	// skip quote
                else
                    j = advPlain(mLine, fld, i);
                if (mFieldNumber >= mFields.size())
                    mFields.push_back(fld);
                else
                    mFields[mFieldNumber] = fld;
                mFieldNumber++;
                i = j + 1;
            } while (j < mLine.length());

            return mFieldNumber;
        }

        char endOfLine(char c)
        {
            char eol;
            eol = (c=='\r' || c=='\n');
            if (c == '\r') 
            {
                mInputStream.get(c);
                if (!mInputStream.eof() && c != '\n')
                    mInputStream.putback(c);	// read too far
            }
            return eol;
        }

        Int32 advPlain(const String& line, String& fld, Int32 i)
        {    
            Int32 j;
            j = line.find_first_of(mFieldSep, i); // look for separator
            if (j > line.length()) // none found
                j = line.length();
            fld = String(line, i, j-i);
            return j;
        }
        Int32 advQuoted(const String& line, String& fld, Int32 i)
        {
            Int32 j;
            fld = "";
            for (j = i; j < line.length(); j++) 
            {
                if (line[j] == '"' && line[++j] != '"') 
                {
                    int k = (int)line.find_first_of(mFieldSep, j);
                    if (k > (int)line.length())	// no separator found
                        k = (int)line.length();
                    for (k -= j; k-- > 0;)
                        fld += line[j++];
                    break;
                }
                fld += line[j];
            }
            return j;
        }

        std::istream& mInputStream;            // input file pointer
        String mLine;                     // input mLine
        Vector<String> mFields;      // mFields strings
        typedef Map<String, int> Columns;
        Columns mColumns;
        UInt32 mFieldNumber;                  // number of fields
        String mFieldSep;                 // separator characters
        //IConvUtil mIConvUtil;
    };
} // namespace Lynx

#endif	// __LYNX_UTIL_LIB_CSVREADER_H__


