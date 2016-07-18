#include "XLHandler.h"
#include "Data.h"
#include "TableExport.h"
#include "DBCMaker.h"
#include <stdio.h> 
#include "HelpHandler.h"
#include "ResourceGroupManager.h"

using namespace Lynx;

//const String g_DictionaryTemplateName = "DictionaryTemplate.xlsx";

static unsigned int g_ConvertCount = 0;
static unsigned int g_SuccessCount = 0;
static std::string g_ConvertPrompt = "";

static XLHandler s_XLHanlder;

// 记录从CSV转换为DBC的转换状态
#define PROMPT_CONVERT(param1)    do{                                   \
    String fileName;                                               \
    if (param1 == 1)                                                    \
    {                                                                   \
        g_ConvertPrompt += "\n转换文件: ";                              \
        fileName = lynxGetFileName(iter->mValue.c_str());                  \
        g_ConvertPrompt += fileName;                                    \
        g_ConvertCount++;                                               \
    }                                                                   \
    else if (param1 == 2)                                               \
    {                                                                   \
        g_SuccessCount++;                                               \
        g_ConvertPrompt += "\n转换成功:";                               \
    }                                                                   \
    else if (param1 == 3)                                               \
    {                                                                   \
        g_ConvertPrompt += "\n转换失败:";                               \
    }                                                                   \
}while(0)

Data::Data() : m_StringSheetCount(0)
{
    m_FieldTableName.clear();
    //m_FieldTablePath.clear();
    m_DictionaryName.clear();
    m_DictionaryPath.clear();
}

Data::~Data()
{
    Release();
}

bool 
Data::Initial()
{
    m_FieldTableName = CONFIG_MANAGER().getAsString("TableName");
    //m_FieldTablePath = CONFIG_MANAGER().getAsString("TablePath");

    m_DictionaryName = "DictionaryTemplate.xlsx";

    ResourceGroup dataResourceGroup;
    if (!ResourceGroupManager::getSingleton().getResourceGroup("Table", dataResourceGroup))
    {
        LOG_WARN("Failed to get Data resource group.");
        return false;
    }

    // 加载所有excel文件
    for (ResourceGroup::Iter* iterData = dataResourceGroup.begin();
        iterData != dataResourceGroup.end(); iterData = dataResourceGroup.next(iterData))
    {
        FileUtil::findFiles(iterData->mKey, "*.xlsx", m_ExcelFileMap);
    }
#if 0
    // 处理m_FieldTablePath和m_FieldTableName
    FilePathMap::Iter* iter = m_ExcelFileMap.find(m_FieldTableName);
    if (iter != m_ExcelFileMap.end())
    {
        m_FieldTablePath = iter->mValue;
        m_ExcelFileMap.erase(iter);
    }
    // 转换m_FieldTablePath为绝对路径
    if (!m_FieldTablePath.empty())
    {
        m_FieldTablePath = GetFullPath(m_FieldTablePath);
    }
    else
    {
        ::MessageBox(NULL, _T("替换字段表绝对路径为空!"), _T("错误提示"), MB_OK);
        return false;
    }
#endif
    // 处理字典模板
    //FilePathMap::Iter* iter_dictionary = m_ExcelFileMap.find(m_DictionaryName);
    //if (iter_dictionary != m_ExcelFileMap.end())
    //{
    //    m_DictionaryPath = iter_dictionary->mValue;
    //    m_ExcelFileMap.erase(iter_dictionary);
    //}

    //// 取字典表的绝对路径
    //if (!m_DictionaryPath.empty())
    //{
    //    m_DictionaryPath = GetFullPath(m_DictionaryPath);
    //}
    //else
    //{
    //    ::MessageBox(NULL, _T("字典表绝对路径为空!"), _T("错误提示"), MB_OK);
    //    return false;
    //}

    // 初始化excel文件选择map，默认都为false
    for(FilePathMap::Iter* iterExcel = m_ExcelFileMap.begin(); iterExcel != m_ExcelFileMap.end();
        iterExcel = m_ExcelFileMap.next(iterExcel))
    {
        if (!iterExcel)
        {
            continue;
        }
        m_ExcelSelectedMap.insert(iterExcel->mKey, false);
    }
    RegisterMsgHandler();

    // EXCEL初始化
    InitXL();

    return true;
}

void 
Data::Release()
{
    ReleaseXL();
}

void 
Data::Update()
{

}

void 
Data::RegisterMsgHandler()
{

}

bool 
Data::Export(String& filePath)
{
    const std::wstring wName = HelpHandler::s2ws(filePath);
    const CString name = wName.c_str();
    CString tmpName = s_XLHanlder.SaveToExcel(filePath);    

    s_XLHanlder.SetSheetCount(0);
    s_XLHanlder.Proc(tmpName);
    s_XLHanlder.RenameFile(filePath);    

    return true;
}

void 
Data::ExportAllSelected()
{
    for (FileSelectedMap::Iter* iterSelect = m_ExcelSelectedMap.begin(); iterSelect != m_ExcelSelectedMap.end();
        iterSelect = m_ExcelSelectedMap.next(iterSelect))
    {
        if (!iterSelect || !(iterSelect->mValue))
        {
            continue;
        }

        FilePathMap::Iter* iterFile = m_ExcelFileMap.find(iterSelect->mKey);
        if (!iterFile)
        {
            continue;
        }

        String absolutePath = GetFullPath(iterFile->mValue);
        String csvFilePath = HelpHandler::Replace(iterFile->mValue, ".xlsx", ".csv");

        int posReplace = csvFilePath.find("Table");
        csvFilePath.replace(posReplace, 5, "Data", 0, 4);

        int pos = csvFilePath.find("StringTemplate.csv");
        if (pos != String::npos 
            && csvFilePath.substr(pos).compare("StringTemplate.csv") == 0)
        { // stringtemplate 这个需要特殊处理
            ResourceGroup resourceGroup;
            ResourceGroupManager::getSingleton().getResourceGroup("Data", resourceGroup);
            String path = resourceGroup.begin()->mKey;
            List<String> csvFileList;
            FileUtil::findFiles(path, "StringTemplate*.csv", csvFileList, false);
            for (List<String>::Iter* iter = csvFileList.begin();
                iter != csvFileList.end(); iter = csvFileList.next(iter))
            {
                const std::wstring wCsvFilePath= HelpHandler::s2ws(iter->mValue.c_str());
                ::DeleteFile(wCsvFilePath.c_str());
            }
        }
        else
        {
            const std::wstring wCsvFilePath= HelpHandler::s2ws(csvFilePath);
            ::DeleteFile(wCsvFilePath.c_str());
        }        
		
        Export(absolutePath);
        ConvertCsvToDbc(csvFilePath);
    }

    PromptInfo();
}

void 
Data::SelectAllExcel(bool selected)
{
    for (FileSelectedMap::Iter* iter = m_ExcelSelectedMap.begin(); iter != m_ExcelSelectedMap.end();
        iter = m_ExcelSelectedMap.next(iter))
    {
        if (!iter || iter->mValue == selected)
        {
            continue;
        }
        iter->mValue = selected;
    }

    // 刷新MainPanel界面数据
    ReBuildMainPanelNotify notify;
    MSG_QUEUE().SendMsg(notify);
}

String 
Data::GetFullPath(const String& path)
{
    wchar_t buffSource[1024] = {0};
    wchar_t buffDest[1024] = {0};
    char buff[1024] = {0};
    DWORD dwNum; 

    dwNum = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, buffSource, dwNum);
    GetFullPathName(buffSource, 1024, buffDest, NULL);
    dwNum = WideCharToMultiByte(CP_OEMCP,NULL, buffDest, -1, NULL, 0, NULL, FALSE);
    WideCharToMultiByte(CP_OEMCP,NULL, buffDest, -1, buff, dwNum, NULL, FALSE);

    String temp(buff);
    return temp;
}

void
Data::InitXL(void)
{
    ::CoInitialize(NULL);
    s_XLHanlder.Init();
    s_XLHanlder.Open();
    //s_XLHanlder.LoadFieldTable(m_FieldTablePath);
    //s_XLHanlder.LoadDictionnaryTable(m_DictionaryPath);
}

void        
Data::ReleaseXL(void)
{
    ::CoUninitialize();
}

String GetNumInString(const char* str, int size)
{
    String result;
    for (int i = 0; i < size; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            result += str[i];
        }
    }
    return result;
}

void 
Data::ConvertCsvToDbc(const String& filePath)
{
    if (!ResourceGroupManager::getSingleton().initial("./ResourceConfig.xml", true))
    {
        LOG_ERROR("Failed to initial resource config file.");
        return;
    }

    ResourceGroup resourceGroup;
    ResourceGroupManager::getSingleton().getResourceGroup("Data", resourceGroup);
    String path = resourceGroup.begin()->mKey;
    printf("进入目录%s\n", path.c_str());

    DBCMaker maker;
    maker.Initial(path);

    List<String> csvFileList;
    FileUtil::findFiles(path, "*.csv", csvFileList, false);

    // 处理单个工作表
    for (List<String>::Iter* iter = csvFileList.begin();
        iter != csvFileList.end(); iter = csvFileList.next(iter))
    {      
        if (iter->mValue.compare(filePath) != 0)
        {
            continue;
        }
    
        PROMPT_CONVERT(1);
        
        const String& fileName = lynxGetFileName(iter->mValue.c_str());
        if (maker.Convert(fileName))
        {
            PROMPT_CONVERT(2);
        }
        else
        {
            //PROMPT_CONVERT(3);
            g_ConvertPrompt += "\n转换失败";                         
            //if (gRepeatKeyInt)
            //{
            //    g_ConvertPrompt += "   重复的key: ";
            //    char key[255] = {0};
            //    sprintf(key, "%d", gRepeatKeyInt);
            //    g_ConvertPrompt += key;
            //}
            //if (gRepeatKeyString.size())
            //{
            //    g_ConvertPrompt += "   重复的key: ";
            //    g_ConvertPrompt += gRepeatKeyString;
            //}

            break;
        }
    }

    // 处理多个工作表
    // stringTemplate
    bool isClearMap = true;
    List<String> csvStringTemplateFileList;
    FileUtil::findFiles(path, "StringTemplate*.csv", csvStringTemplateFileList, false);
    for(List<String>::Iter* iter = csvStringTemplateFileList.begin(); 
        iter != csvStringTemplateFileList.end(); iter = csvStringTemplateFileList.next(iter))
    {
        if (filePath.find("StringTemplate", 0) != String::npos)
        {
            PROMPT_CONVERT(1);
            String no = GetNumInString(iter->mValue.c_str(), iter->mValue.size());

            Int32 repeatKey = 0;
            if (maker.ConvertString(lynxGetFileName(iter->mValue.c_str()), atoi(no.c_str()), isClearMap))
            {
                if (isClearMap = true)
                {
                    PROMPT_CONVERT(2);
                    isClearMap = false;
                }
            }
            else
            {
                g_ConvertPrompt += "\n转换失败";                         
                //if (gRepeatKeyInt)
                //{
                //    g_ConvertPrompt += "   重复的key: ";
                //    char key[255] = {0};
                //    sprintf(key, "%d", gRepeatKeyInt);
                //    g_ConvertPrompt += key;
                //}
                //if (gRepeatKeyString.size())
                //{
                //    g_ConvertPrompt += "   重复的key: ";
                //    g_ConvertPrompt += gRepeatKeyString;
                //}
                PROMPT_CONVERT(3);
            }
        }
    }

/* 张宇 2013-4-15 没看懂
    // questInfoTemplate
    isClearMap = true;
    List<String> csvQuestInfoTemplateFileList;
    FileUtil::findFiles(path, "QuestInfoTemplate*.csv", csvQuestInfoTemplateFileList, false);
    for(List<String>::Iter* iter = csvQuestInfoTemplateFileList.begin(); 
        iter != csvQuestInfoTemplateFileList.end(); iter = csvQuestInfoTemplateFileList.next(iter))
    {
        if (filePath.find("QuestInfoTemplate", 0) != String::npos)
        {
            PROMPT_CONVERT(1);
            String no = GetNumInString(iter->mValue.c_str(), iter->mValue.size());
            if (maker.ConvertQuestInfo(lynxGetFileName(iter->mValue.c_str()), atoi(no.c_str()), isClearMap))
            {
                PROMPT_CONVERT(2);
                isClearMap = false;
            }
            else
            {
                PROMPT_CONVERT(3);
            }
        }
    }
*/

    // 删除生成的临时文件
    DeleteFileByStr(path, "*_tmp.xlsx");
    DeleteFileByStr(path, "*_tmp.csv");
}

void
Data::DeleteFileByStr(const String& path, const String& del)
{
    List<String> DelCsvFileList;
    FileUtil::findFiles(path, del, DelCsvFileList, false);

    for (List<String>::Iter* iter = DelCsvFileList.begin();
        iter != DelCsvFileList.end(); iter = DelCsvFileList.next(iter))
    {
        std::wstring stemp = HelpHandler::s2ws(iter->mValue);
        LPCWSTR result = stemp.c_str();
        ::DeleteFile(result);
    }
}

void 
Data::PromptInfo(void)
{
    char convertCount[100] = {0};
    char successCount[100] = {0};

    String convert;
    sprintf(convertCount, "\n\n转换总数: %d\n", g_ConvertCount);
    sprintf(successCount, "转换成功数量: %d", g_SuccessCount);

	//String temp = "";

	//temp += convertCount;
	//temp += successCount;
    
    g_ConvertPrompt += convertCount;
    g_ConvertPrompt += successCount;
    
    wxMessageBox(g_ConvertPrompt.c_str());

    g_ConvertCount = 0;

	

    g_ConvertPrompt.clear();
}