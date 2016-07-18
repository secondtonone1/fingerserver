#include "CommonLib.h"

using namespace Lynx;

//#if __CSharp__
//        void testFunc()
//        {
//            
//        }
//#endif

struct ClassInfo
{
	ClassInfo() : mIsMsgFlag(false), mClassName(""), mParentClassName(""), mCSharpCodes("") {}

	bool mIsMsgFlag;
	String mClassName;
	String mParentClassName;
    String mCSharpCodes;
};

void 
trimComment(String& str)
{
	Int32 pos1, pos2;
	String tmpStr;

	// 去掉//
	{
		while (1)
		{
			String str1 = "";
			pos1 = str.find("//");
			if (pos1 == std::string::npos)
			{
				break;
			}
			str1 += str.substr(0, pos1);
			tmpStr = str.substr(pos1);
			pos2 = tmpStr.find("\n");
			if (pos2 == std::string::npos)
			{
				break;
			}
			str1 += tmpStr.substr(pos2 + 1);
			str = str1;
		}
	}

	// 去掉 /* */
	{
		while (1)
		{
			String str1 = "";
			pos1 = str.find("/*");
			if (pos1 == std::string::npos)
			{
				break;
			}
			str1 += str.substr(0, pos1);
			tmpStr = str.substr(pos1);
			pos2 = tmpStr.find("*/");
			if (pos2 == std::string::npos)
			{
				break;
			}
			str1 += tmpStr.substr(pos2 + 2);
			str = str1;
		}
	}

	// 去掉#if 0
	{
		while (1)
		{
			String str1 = "";
			pos1 = str.find("#if ");
			if (pos1 == std::string::npos)
			{
				break;
			}
			str1 += str.substr(0, pos1);
			tmpStr = str.substr(pos1 + 3);
			lynxStrTrim(tmpStr);
			if (tmpStr[0] != '0')
			{
                str1 += "#fi ";
				str1 += tmpStr;
				str = str1;
				continue;
			}
			pos2 = tmpStr.find("#endif");
			if (pos2 == std::string::npos)
			{
				break;
			}
			str1 += tmpStr.substr(pos2 + 6);
			str = str1;
		}
	}

    // 去掉#if 1
	{
		while (1)
		{
			String str1 = "";
			pos1 = str.find("#fi ");
			if (pos1 == std::string::npos)
			{
				break;
			}
			str1 += str.substr(0, pos1);
			tmpStr = str.substr(pos1 + 3);
			lynxStrTrim(tmpStr);
			if (tmpStr[0] != '1')
			{
                //str1 += "#fi ";
				str1 += tmpStr;
				str = str1;
				continue;
			}
			pos2 = tmpStr.find("#endif");
			if (pos2 == std::string::npos)
			{
				break;
			}
			str1 += tmpStr.substr(pos2 + 6);
			str = str1;
		}
	}
}

void 
trimInvalidChar(String& src)
{
	Lynx::Int32 i = 0;
    Lynx::Int32 tailPos = 0;

    for (Lynx::Int32 cpyPos = i; i < src.length(); i++)
    {
        if (src[i] != 32 && src[i] != 9 && src[i] != 13 && src[i] != 10)
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

void 
getClassInfo(String str, List<ClassInfo>& classInfoList)
{
	trimComment(str);

	Int32 pos1, pos2;
	String tmpStr;

	// 取结构体
	while (1)
	{
		pos1 = str.find("struct");
		if (pos1 == std::string::npos)
		{
			break;
		}

		tmpStr = str.substr(pos1);
		pos2 = tmpStr.find("{");
		if (pos2 == std::string::npos)
		{
			break;
		}
		tmpStr = tmpStr.substr(0, pos2);

		trimInvalidChar(tmpStr);
		str = str.substr(pos1 + pos2);

        

		// 检查结构体是否有序列化宏
		bool isMsgFlag = false;
		pos1 = str.find("struct");
		String s = str.substr(0, pos1);
		pos2 = s.find("LYNX_S11N");
		if (pos2 == std::string::npos)
		{
			pos2 = s.find("LYNX_MESSAGE");
			if (pos2 == std::string::npos)
			{
			    continue;
			}
			isMsgFlag = true;
		}

        // 拷贝#if CSharp 部分， CSharp代码
        String csharpCodes = "";
        Int32 p1 = s.find("__CSharp__");
        if (p1 != std::string::npos)
        {
            String s1 = s.substr(p1 + 10);

            Int32 p2 = s1.find("#endif");
            if (p2 != std::string::npos)
            {
                csharpCodes = s1.substr(0, p2);
            }
        }

        ///////////////////

		Vector<String> strVector;
		lynxStrSplit(tmpStr, ":", strVector, true);
		if (strVector.size() == 1)
		{
			Vector<String> v;
			lynxStrSplit(strVector[0], " ", v, true);
			ClassInfo classInfo;
			classInfo.mIsMsgFlag = isMsgFlag;
			classInfo.mClassName = v[1];
            classInfo.mCSharpCodes = csharpCodes;
			classInfoList.insertTail(classInfo);
		}
		else if (strVector.size() == 2)
		{
			Vector<String> v;
			lynxStrSplit(strVector[0], " ", v, true);
			ClassInfo classInfo;
			classInfo.mIsMsgFlag = isMsgFlag;
			classInfo.mClassName = v[1];
			v.clear();
			lynxStrSplit(strVector[1], " ", v, true);
			classInfo.mParentClassName = v[1];
            classInfo.mCSharpCodes = csharpCodes;
			classInfoList.insertTail(classInfo);
		}
	}
}

void
getErrorDefinitionInfo(String str, Map<String, UInt32>& errorInfoMap)
{
    trimComment(str);

    Int32 pos1, pos2, pos3;
	String tmpStr;

    pos1 = str.find("namespace LynxErrno");
    if (pos1 == std::string::npos)
    {
        return;
    }

    tmpStr = str.substr(pos1);
    pos2 = tmpStr.find("{");
    if (pos2 == std::string::npos)
    {
        return;
    }
    tmpStr = tmpStr.substr(0, pos2);

    trimInvalidChar(tmpStr);
    str = str.substr(pos1 + pos2 + 1);

    pos3 = str.find("}");
    str = str.substr(0, pos3);

    trimInvalidChar(str);

    Vector<String> lines;
    lynxStrSplit(str, ";", lines, true);

    for (Vector<String>::iterator it = lines.begin();
        it != lines.end(); ++it)
    {
        String line = it->c_str();
        lynxStrTrim(line);

        int p = line.find("ErrorId");
        if (p != std::string::npos)
        {
            line = line.substr(p + 7);
            Vector<String> vvv;
            lynxStrSplit(line, "=", vvv, true);
            if (vvv.size() == 2)
            {
                UInt32 value = lynxAtoi<UInt32>(vvv[1].c_str());
                String key = vvv[0].c_str();
                lynxStrTrim(key);
                errorInfoMap.insert(key, value);
            }
        }

    }
}


int main(int argc, char** argv)
{
	LOG_INITIAL(Logger::OUTPUT_FILE, true, false);
	LOG_SET_OUTPUT_FILE("PreBuildTool");

    if (argc != 2)
    {
        LOG_WARN("Invalid command argument.");
        return -1;
    }

    bool isForCSharp = false;
    bool isForCPlusPlus= false;
    String param = argv[1];
    if (param == "-csharp")
    {
        isForCSharp = true;
    }
    else if (param == "-cplusplus")
    {
        isForCPlusPlus = true;
    }
    else
    {
        return -1;
    }

	List<String> filePathList;
	FileUtil::findFiles("../../CommonLib", "*Data.h", filePathList, false);
    FileUtil::findFiles("../../CommonLib", "*Table.h", filePathList, false);
	FileUtil::findFiles("../../CommonLib", "*Msg.h", filePathList, false);

	char buf[409600] = { 0 };

    sprintf(buf, "%s#include \"CommonLib.h\"\r\n\r\n", buf);

    if (isForCSharp)
    {
        sprintf(buf, "%s#include \"CSharpCoder.h\"\r\n", buf);
    }

    if (isForCPlusPlus)
    {
        StringMap<String> tableFileMap;
        FileUtil::findFiles("../../CommonLib", "*Table.h", tableFileMap, false);
        tableFileMap.erase("Table.h");
        for (StringMap<String>::Iter* iter = tableFileMap.begin();
            iter != NULL; iter = tableFileMap.next(iter))
        {
            sprintf(buf, "%s#include \"%s\"\r\n", buf, lynxGetFileName(iter->mKey.c_str()).c_str());
        }
    }
    sprintf(buf, "%s\r\n", buf);

	sprintf(buf, "%susing namespace Lynx;\r\n\r\n", buf);

    if (isForCSharp)
    {
        // ADD_CLASS
        sprintf(buf, "%s#define ADD_CLASS(fileName, namespaceName, className, parentClassName) \\\r\n", buf);
        sprintf(buf, "%sdo \\\r\n", buf);
        sprintf(buf, "%s{ \\\r\n", buf);
        sprintf(buf, "%s    Map<String, List<String> >::Iter* iter = fileMap.find(#fileName); \\\r\n", buf);
        sprintf(buf, "%s    if (iter == NULL) \\\r\n", buf);
        sprintf(buf, "%s    { \\\r\n", buf);
        sprintf(buf, "%s        List<String> classList; \\\r\n", buf);
        sprintf(buf, "%s        iter = fileMap.insert(#fileName, classList); \\\r\n", buf);
        sprintf(buf, "%s    } \\\r\n", buf);
        sprintf(buf, "%s    memset(buf, 0, 409600); \\\r\n", buf);
        sprintf(buf, "%s    className obj; \\\r\n", buf);
        sprintf(buf, "%s    CSharpElement xmlObj(#className); \\\r\n", buf);
        sprintf(buf, "%s    if (String(\"NULL\") == #parentClassName) \\\r\n", buf);
        sprintf(buf, "%s        gParentClassObjectSize = 0; \\\r\n", buf);
        sprintf(buf, "%s    else \\\r\n", buf);
        sprintf(buf, "%s        gParentClassObjectSize = sizeof(parentClassName); \\\r\n", buf);
        sprintf(buf, "%s    gS11NObjectAddress = (char*)&obj; \\\r\n", buf);
        sprintf(buf, "%s    serialize(obj, xmlObj); \\\r\n", buf);
        sprintf(buf, "%s    Map<String, String>::Iter* jter = csharpCodeMap.find(#className);\\\r\n", buf);
        sprintf(buf, "%s    coder.Generate(buf, &xmlObj, #namespaceName, #className, #parentClassName, jter->mValue, 0); \\\r\n", buf);
        sprintf(buf, "%s    iter->mValue.insertTail(buf); \\\r\n", buf);
        sprintf(buf, "%s} while (0) \r\n\r\n", buf);

        // ADD_TABLE
        sprintf(buf, "%s#define ADD_TABLE(fileName, namespaceName, className, parentClassName) \\\r\n", buf);
        sprintf(buf, "%sdo \\\r\n", buf);
        sprintf(buf, "%s{ \\\r\n", buf);
        sprintf(buf, "%s    Map<String, List<String> >::Iter* iter = fileMap.find(#fileName); \\\r\n", buf);
        sprintf(buf, "%s    if (iter == NULL) \\\r\n", buf);
        sprintf(buf, "%s    { \\\r\n", buf);
        sprintf(buf, "%s        List<String> classList; \\\r\n", buf);
        sprintf(buf, "%s        iter = fileMap.insert(#fileName, classList); \\\r\n", buf);
        sprintf(buf, "%s    } \\\r\n", buf);
        sprintf(buf, "%s    memset(buf, 0, 409600); \\\r\n", buf);
        sprintf(buf, "%s    className obj; \\\r\n", buf);
        sprintf(buf, "%s    CSharpElement xmlObj(#className); \\\r\n", buf);
        sprintf(buf, "%s    if (String(\"NULL\") == #parentClassName) \\\r\n", buf);
        sprintf(buf, "%s        gParentClassObjectSize = 0; \\\r\n", buf);
        sprintf(buf, "%s    else \\\r\n", buf);
        sprintf(buf, "%s        gParentClassObjectSize = sizeof(parentClassName); \\\r\n", buf);
        sprintf(buf, "%s    gS11NObjectAddress = (char*)&obj; \\\r\n", buf);
        sprintf(buf, "%s    serialize(obj, xmlObj); \\\r\n", buf);
        sprintf(buf, "%s    Map<String, String>::Iter* jter = csharpCodeMap.find(#className);\\\r\n", buf);
        sprintf(buf, "%s    coder.GenerateTable(buf, &xmlObj, #namespaceName, #className, #parentClassName, jter->mValue); \\\r\n", buf);
        sprintf(buf, "%s    iter->mValue.insertTail(buf); \\\r\n", buf);
        sprintf(buf, "%s} while (0) \r\n\r\n", buf);

        // ADD_MESSAGE
        sprintf(buf, "%s#define ADD_MESSAGE(fileName, namespaceName, className, parentClassName) \\\r\n", buf);
        sprintf(buf, "%sdo \\\r\n", buf);
        sprintf(buf, "%s{ \\\r\n", buf);
        sprintf(buf, "%s    Map<String, List<String> >::Iter* iter = fileMap.find(#fileName); \\\r\n", buf);
        sprintf(buf, "%s    if (iter == NULL) \\\r\n", buf);
        sprintf(buf, "%s    { \\\r\n", buf);
        sprintf(buf, "%s        List<String> classList; \\\r\n", buf);
        sprintf(buf, "%s        iter = fileMap.insert(#fileName, classList); \\\r\n", buf);
        sprintf(buf, "%s    } \\\r\n", buf);
        sprintf(buf, "%s    memset(buf, 0, 409600); \\\r\n", buf);
        sprintf(buf, "%s    className obj; \\\r\n", buf);
        sprintf(buf, "%s    CSharpElement xmlObj(#className); \\\r\n", buf);
        sprintf(buf, "%s    if (String(\"NULL\") == #parentClassName) \\\r\n", buf);
        sprintf(buf, "%s        gParentClassObjectSize = 0; \\\r\n", buf);
        sprintf(buf, "%s    else \\\r\n", buf);
        sprintf(buf, "%s        gParentClassObjectSize = sizeof(parentClassName); \\\r\n", buf);
        sprintf(buf, "%s    gS11NObjectAddress = (char*)&obj; \\\r\n", buf);
        sprintf(buf, "%s    serialize(obj, xmlObj); \\\r\n", buf);
        sprintf(buf, "%s    Map<String, String>::Iter* jter = csharpCodeMap.find(#className);\\\r\n", buf);
        sprintf(buf, "%s    coder.Generate(buf, &xmlObj, #namespaceName, #className, #parentClassName, jter->mValue, className::sMsgId); \\\r\n", buf);
        sprintf(buf, "%s    iter->mValue.insertTail(buf); \\\r\n", buf);
        sprintf(buf, "%s} while (0) \r\n\r\n", buf);
    }

	// Main
	sprintf(buf, "%sint main(int argc, char** argv) \r\n", buf);
	sprintf(buf, "%s{\r\n", buf);
    if (isForCSharp)
    {
        sprintf(buf, "%s    CSharpCoder coder; \r\n", buf);
    }
	sprintf(buf, "%s    char buf[409600]; \r\n", buf);
	sprintf(buf, "%s    Map<String, List<String> > fileMap; \r\n", buf);
    if (isForCSharp)
    {
        sprintf(buf, "%s    Map<String, String> csharpCodeMap; \r\n", buf);
    }

    sprintf(buf, "%s    StringMap<String> tableFileMap; \r\n\r\n", buf);
    sprintf(buf, "%s    FileUtil::findFiles(\"../../CommonLib\", \"*Table.h\", tableFileMap, false);\r\n", buf);
    sprintf(buf, "%s    tableFileMap.erase(\"Table.h\");\r\n", buf);

    sprintf(buf, "%s    StreamBuffer stream;\r\n", buf);

    if (isForCSharp)
    {
        sprintf(buf, "%s    FileUtil::loadFile(\"../Binaries/codegenerator_tmp\", stream); \r\n", buf);
        sprintf(buf, "%s    unserialize(csharpCodeMap, stream); \r\n", buf);
    }

    Map<String, String> csharpCodeMap;
    Map<String, UInt32> tableFileMap;

    if (isForCSharp)
    {
        for (List<String>::Iter* iter = filePathList.begin();
            iter != NULL; iter = filePathList.next(iter))
        {
            String fileName = lynxGetFileName(iter->mValue.c_str());

            if (fileName == "GMTData.h" || fileName == "GMTMsg.h")
            {
                continue;
            }

            char* buffer = NULL;
            Int32 fileLen = FileUtil::loadFile(iter->mValue, &buffer);
            if (fileLen < 0)
            {
                LOG_ERROR("Failed to read file %s", fileName.c_str());
                return -1;
            }
            else if (fileLen == 0)
            {
                continue;
            }

            String str = buffer;
            FileUtil::freeBuffer(buffer);
            List<ClassInfo> classInfoList;
            getClassInfo(str, classInfoList);

            if (!classInfoList.empty())
            {
                sprintf(buf, "%s    //%s\r\n", buf, fileName.c_str());
                for (List<ClassInfo>::Iter* jter = classInfoList.begin();
                    jter != classInfoList.end(); jter = classInfoList.next(jter))
                {
                    lynxStrReplace(jter->mValue.mCSharpCodes, "\n    ", "\n");
                    csharpCodeMap.insert(jter->mValue.mClassName, jter->mValue.mCSharpCodes);
                    if (!jter->mValue.mIsMsgFlag)
                    {
                        int pos = fileName.find("Table.h");
                        if (pos > 0)
                        {
                            tableFileMap.insert(lynxGetMainFileName(fileName.c_str()), 0);
                            if (jter->mValue.mParentClassName.empty())
                            {
                                sprintf(buf, "%s    ADD_TABLE(%s, NULL, %s, %s);\r\n", 
                                    buf, fileName.c_str(), jter->mValue.mClassName.c_str(), "NULL");
                            }
                            else
                            {
                                sprintf(buf, "%s    ADD_TABLE(%s, NULL, %s, %s);\r\n", 
                                    buf, fileName.c_str(), jter->mValue.mClassName.c_str(), 
                                    jter->mValue.mParentClassName.c_str());
                            }
                        }
                        else
                        {

                            if (jter->mValue.mParentClassName.empty())
                            {
                                sprintf(buf, "%s    ADD_CLASS(%s, NULL, %s, %s);\r\n", 
                                    buf, fileName.c_str(), jter->mValue.mClassName.c_str(), "NULL");
                            }
                            else
                            {
                                sprintf(buf, "%s    ADD_CLASS(%s, NULL, %s, %s);\r\n", 
                                    buf, fileName.c_str(), jter->mValue.mClassName.c_str(), 
                                    jter->mValue.mParentClassName.c_str());
                            }
                        }
                    }
                    else
                    {
                        if (jter->mValue.mParentClassName.empty())
                        {
                            sprintf(buf, "%s    ADD_MESSAGE(%s, NULL, %s, %s);\r\n", 
                                buf, fileName.c_str(), jter->mValue.mClassName.c_str(), "NULL");
                        }
                        else
                        {
                            sprintf(buf, "%s    ADD_MESSAGE(%s, NULL, %s, %s);\r\n", 
                                buf, fileName.c_str(), jter->mValue.mClassName.c_str(), 
                                jter->mValue.mParentClassName.c_str());
                        }
                    }
                }
                sprintf(buf, "%s\r\n", buf);
            }
        }
    }

    StreamBuffer streamBuffer;
    if (isForCSharp)
    {
        serialize(csharpCodeMap, streamBuffer);
        FileUtil::saveFile("../Binaries/codegenerator_tmp", streamBuffer);
    }

    if (isForCSharp)
    {
        // Output to files
        sprintf(buf, "%s    StreamBuffer streamBuffer;\r\n", buf);
        sprintf(buf, "%s    for (Map<String, List<String> >::Iter* iter = fileMap.begin(); \r\n", buf);
        sprintf(buf, "%s        iter != NULL; iter = fileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);     
        sprintf(buf, "%s        streamBuffer.clear(); \r\n", buf);
        sprintf(buf, "%s        char timestamp[512] = { 0 };\r\n", buf);
        sprintf(buf, "%s        TimeUtil::getTimestamp(timestamp, 512, TimeUtil::getTimeMilliSec());\r\n\r\n", buf);

        sprintf(buf, "%s        streamBuffer.write(\"//-----------------------------------------------------------------------\\r\\n\",\r\n", buf);
        sprintf(buf, "%s            strlen(\"//-----------------------------------------------------------------------\\r\\n\"));\r\n", buf);
        sprintf(buf, "%s        String fileHeadInfo = \"//Lynx code generated by c++\";\r\n", buf);
        //sprintf(buf, "%s        fileHeadInfo += timestamp;\r\n", buf);
        sprintf(buf, "%s        fileHeadInfo += \"\\r\\n\";\r\n", buf);
        sprintf(buf, "%s        streamBuffer.write(fileHeadInfo.c_str(), fileHeadInfo.length());\r\n", buf);
        sprintf(buf, "%s        streamBuffer.write(\"//-----------------------------------------------------------------------\\r\\n\\r\\n\",\r\n", buf);
        sprintf(buf, "%s            strlen(\"//-----------------------------------------------------------------------\\r\\n\\r\\n\"));\r\n", buf);

        sprintf(buf, "%s        streamBuffer.write(\"using UnityEngine;\\r\\n\", strlen(\"using UnityEngine;\\r\\n\"));\r\n", buf);
        sprintf(buf, "%s        streamBuffer.write(\"using System.Collections;\\r\\n\", strlen(\"using System.Collections;\\r\\n\"));\r\n", buf);
        sprintf(buf, "%s        streamBuffer.write(\"using System.Collections.Generic;\\r\\n\", strlen(\"using System.Collections.Generic;\\r\\n\"));\r\n", buf);
        sprintf(buf, "%s        streamBuffer.write(\"using System;\\r\\n\", strlen(\"using System;\\r\\n\"));\r\n", buf);
        sprintf(buf, "%s        streamBuffer.write(\"using System.IO;\\r\\n\", strlen(\"using System.IO;\\r\\n\"));\r\n", buf);
        sprintf(buf, "%s        streamBuffer.write(\"using System.Runtime.InteropServices;\\r\\n\\r\\n\", strlen(\"using System.Runtime.InteropServices;\\r\\n\\r\\n\"));\r\n", buf);

        sprintf(buf, "%s        streamBuffer.write(\"//-----------------------------------------------------------------------\\r\\n\",\r\n", buf);
        sprintf(buf, "%s            strlen(\"//-----------------------------------------------------------------------\\r\\n\"));\r\n", buf);
        sprintf(buf, "%s        for (List<String>::Iter* jter = iter->mValue.begin(); \r\n", buf);
        sprintf(buf, "%s            jter != NULL; jter = iter->mValue.next(jter)) \r\n", buf);
        sprintf(buf, "%s        {\r\n", buf);
        sprintf(buf, "%s            streamBuffer.write(jter->mValue.c_str(), jter->mValue.length());\r\n", buf);
        sprintf(buf, "%s            streamBuffer.write(\"//-----------------------------------------------------------------------\\r\\n\",\r\n", buf);
        sprintf(buf, "%s                strlen(\"//-----------------------------------------------------------------------\\r\\n\"));\r\n", buf);
        sprintf(buf, "%s        }\r\n", buf);
        sprintf(buf, "%s        String outputFileName = lynxGetMainFileName(iter->mKey);\r\n", buf);
        sprintf(buf, "%s        String outputFilePath = \"../../../Client/Assets/Script/Global/NetStruct/\";\r\n", buf);
        sprintf(buf, "%s        outputFilePath += outputFileName + \".cs\";\r\n", buf);
        sprintf(buf, "%s        FileUtil::saveFile(outputFilePath, streamBuffer);\r\n", buf);
        sprintf(buf, "%s    }\r\n\r\n", buf);
    }

    // 生产Table.cs文件
    if (isForCSharp)
    {
        sprintf(buf, "%s    memset(buf, 0, sizeof(buf));\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s//-----------------------------------------------------------------------\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s//Lynx code generated by c++\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s//-----------------------------------------------------------------------\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%susing UnityEngine;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%susing System.Collections;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%susing System.Collections.Generic;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%susing System;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%susing System.IO;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%susing System.Runtime.InteropServices;\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%spublic class TableManager : MonoBehaviour\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s{\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s    public static bool load()\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s    {\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s         sprintf(buf, \"%s        if (!%s.GetSingleton().LoadFromDbc(\\\"%s\\\"))\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s", "%s");
        sprintf(buf, "%s         sprintf(buf, \"%s        {\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s         sprintf(buf, \"%s            Debug.LogWarning(\\\"Failed to load %s table.\\\");\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s");
        sprintf(buf, "%s         sprintf(buf, \"%s            return false;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s         sprintf(buf, \"%s        }\\r\\n\", buf);\r\n", buf, "%s");

        sprintf(buf, "%s    }\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s         return true;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s    }\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s}\\r\\n\", buf);\r\n", buf, "%s");


        sprintf(buf, "%s    String tableFilePath = \"../../../Client/Assets/Script/Global/NetStruct/TableManager.cs\";\r\n", buf);
        sprintf(buf, "%s    FileUtil::saveFile(tableFilePath, buf, strlen(buf));\r\n", buf);

    }
    // 产生Table.h文件
    if (isForCPlusPlus)
    {
        sprintf(buf, "%s    memset(buf, 0, sizeof(buf));\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s#ifndef __LYNX_GAME_COMMON_LIB_TABLE_H__\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s#define __LYNX_GAME_COMMON_LIB_TABLE_H__\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s         sprintf(buf, \"%s#include \\\"%s.h\\\"\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s");
        sprintf(buf, "%s    }\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s\\r\\nnamespace Lynx\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s{\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s    class TableManager : public Singleton<TableManager>\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s    {\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s    public:\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s        TableManager() {}\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s        virtual ~TableManager() {}\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s        bool load();\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s        void unload();\\r\\n\", buf); \r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s        bool reload(const String& fileName);\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s        bool reloadAll();\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s    public:\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s         sprintf(buf, \"%s        %s m%s;\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s", "%s");
        sprintf(buf, "%s    }\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s    };\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s}\\r\\n\\r\\n\", buf);\r\n", buf, "%s");

        sprintf(buf, "%s    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s         sprintf(buf, \"%sextern Lynx::%s* g%s;\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s", "%s");
        sprintf(buf, "%s    }\r\n", buf);

        sprintf(buf, "%s    sprintf(buf, \"%s\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s         sprintf(buf, \"%s#define %s() (*g%s)\\r\\n\", buf, lynxGetMacroFileName(lynxGetMainFileName(iter->mKey)).c_str(), lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s", "%s");
        sprintf(buf, "%s    }\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s#endif\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    FileUtil::saveFile(\"../../CommonLib/Table.h\", buf, strlen(buf));\r\n", buf);

        // 产生Table.cpp 文件
        sprintf(buf, "%s    memset(buf, 0, sizeof(buf));\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s#include \\\"Table.h\\\"\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%susing namespace Lynx;\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s         sprintf(buf, \"%s%s* g%s = NULL;\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s", "%s");
        sprintf(buf, "%s    }\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%sbool\\r\\nTableManager::load()\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s{\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s         sprintf(buf, \"%s    if (!m%s.loadFromDbc(\\\"%s.dbc\\\"))\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s", "%s");
        sprintf(buf, "%s         sprintf(buf, \"%s    {\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s         sprintf(buf, \"%s        LOG_WARN(\\\"Failed to load %s table.\\\");\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s");
        sprintf(buf, "%s         sprintf(buf, \"%s        return false;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s         sprintf(buf, \"%s    }\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s         sprintf(buf, \"%s    g%s = &m%s;\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s", "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    }\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s    return true;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s}\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s\\r\\n\", buf);\r\n", buf, "%s");

        sprintf(buf, "%s    sprintf(buf, \"%svoid\\r\\nTableManager::unload()\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s{\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s    m%s.clear();\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s    g%s = NULL;\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s");
        sprintf(buf, "%s    }\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s}\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s\\r\\n\", buf);\r\n", buf, "%s");

        sprintf(buf, "%s    sprintf(buf, \"%sbool\\r\\nTableManager::reload(const String& fileName)\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s{\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s    String fileNameFix = fileName;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s    lynxToLowerCase(fileNameFix);\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s        String lowerName = lynxGetMainFileName(iter->mKey);\r\n", buf);
        sprintf(buf, "%s        lynxToLowerCase(lowerName);\r\n", buf);
        sprintf(buf, "%s        sprintf(buf, \"%s    if (fileNameFix == \\\"%s.dbc\\\")\\r\\n\", buf, lowerName.c_str());\r\n", buf, "%s", "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s    {\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s        if (!m%s.reloadFromDbc(fileName))\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s        {\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s           LOG_WARN(\\\"Failed to reload %s table.\\\");\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s           return false;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s        }\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s        return true;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s    }\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    }\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s    return false;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s}\\r\\n\", buf);\r\n", buf, "%s");

        sprintf(buf, "%s    sprintf(buf, \"%sbool\\r\\nTableManager::reloadAll()\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s{\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))\r\n", buf);
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s        sprintf(buf, \"%s    if (!m%s.reloadFromDbc(\\\"%s.dbc\\\"))\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s", "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s    {\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s       LOG_WARN(\\\"Failed to reload %s table.\\\");\\r\\n\", buf, lynxGetMainFileName(iter->mKey).c_str());\r\n", buf, "%s", "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s       return false;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s        sprintf(buf, \"%s    }\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    }\r\n", buf);
        sprintf(buf, "%s    sprintf(buf, \"%s    return true;\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    sprintf(buf, \"%s}\\r\\n\\r\\n\", buf);\r\n", buf, "%s");
        sprintf(buf, "%s    FileUtil::saveFile(\"../../CommonLib/Table.cpp\", buf, strlen(buf));\r\n", buf);
    }

    // 产生ErrorDefinition文件
    char* tmpBuf;
    FileUtil::loadFile("../../CommonLib/ErrorDefinition.h", &tmpBuf);
    Map<String, UInt32> errorInfoMap;
    getErrorDefinitionInfo(tmpBuf, errorInfoMap);
    FileUtil::freeBuffer(tmpBuf);

    char tmpBuffer[204800] = { 0 };
    sprintf(tmpBuffer, "%s//Lynx code generated by c++\r\n\r\n", tmpBuffer);
    sprintf(tmpBuffer, "%susing System.Collections;\r\n", tmpBuffer);
    sprintf(tmpBuffer, "%susing System;\r\n", tmpBuffer);
    sprintf(tmpBuffer, "%susing System.Collections.Generic;\r\n\r\n", tmpBuffer);
    sprintf(tmpBuffer, "%spublic class ErrorDefinition\r\n", tmpBuffer);
    sprintf(tmpBuffer, "%s{\r\n", tmpBuffer);
    for (Map<String, UInt32>::Iter* iter = errorInfoMap.begin();
        iter != NULL; iter = errorInfoMap.next(iter))
    {
        sprintf(tmpBuffer, "%s    public const UInt16 %s = %d;\r\n", tmpBuffer, iter->mKey.c_str(), iter->mValue);
    }
    sprintf(tmpBuffer, "%s}\r\n", tmpBuffer);

    FileUtil::saveFile("../../../Client/Assets/Script/Global/NetStruct/ErrorDefinition.cs", tmpBuffer, strlen(tmpBuffer));

    // 产生ErrorDefinition 结束

	sprintf(buf, "%s\r\n    return 0;\r\n", buf); 
	sprintf(buf, "%s}\r\n", buf);

    if (isForCSharp)
    {
	    FileUtil::saveFile("../../CodeGenerator/Main.cpp", buf, strlen(buf));
    }

    if (isForCPlusPlus)
    {
        FileUtil::saveFile("../../TableGenerator/Main.cpp", buf, strlen(buf));
    }

	LOG_RELEASE();
	return 0;
}

/*
#include "CSharpCoder.h"
#include "CommonLib.h"

using namespace Lynx;

#define ADD_CLASS(fileName, namespaceName, className, parentClassName, mask) \
do \
{ \
    Map<String, List<String> >::Iter* iter = fileMap.find(#fileName); \
    if (iter == NULL) \
    { \
        List<String> classList; \
        iter = fileMap.insert(#fileName, classList); \
    } \
    memset(buf, 0, 409600); \
    className obj; \
    CSharpElement xmlObj(#className); \
    serialize(obj, xmlObj); \
    coder.Generate(buf, &xmlObj, #namespaceName, #className, #parentClassName, 0); \
    iter->mValue.insertTail(buf); \
} while (0) 

int main(int argc, char** argv) 
{
    CSharpCoder coder; 
    char buf[409600]; 
    Map<String, List<String> > fileMap; 

    ADD_CLASS(PlayerData.h, NULL, AAAA, NULL, 0);
    ADD_CLASS(PlayerData.h, NULL, DDDD, AAAA, 0);

    return 0;
}

*/