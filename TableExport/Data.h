#ifndef __LYNX_EXCEL_TO_DBC_TOOL_DATA_H__
#define __LYNX_EXCEL_TO_DBC_TOOL_DATA_H__

#include "PlatformLib.h"
#include "NetworkLib.h"
#include "MsgDefine.h"


namespace Lynx
{
    typedef StringMap<String> FilePathMap;
    typedef StringMap<bool>        FileSelectedMap;

    class Data
    {
    public:
        Data();
        virtual ~Data();

        bool Initial();
        void Release();
        void Update();
        void RegisterMsgHandler();

        // 绝对路径（带文件名字和后缀.xlsx）
        bool Export(String& filePath);

        // 导出所有选中文件
        void ExportAllSelected();

        void ConvertCsvToDbc(const String& filePath);
        void DeleteFileByStr(const String& path, const String& del);
        void PromptInfo(void);

        // true:全部选中 false:全部取消选中
        void SelectAllExcel(bool selected = true);

    private:
        // 文件路径（绝对和相对路径均可）
        String GetFullPath(const String& path);
        void        InitXL(void);
        void        ReleaseXL(void);

    public:
        FilePathMap        m_ExcelFileMap;
        FileSelectedMap    m_ExcelSelectedMap;
        String        m_FieldTableName;
        String        m_FieldTablePath;
        String        m_DictionaryName;
        String        m_DictionaryPath;
        
        unsigned long      m_StringSheetCount;
    };
} // namespace Lynx

#endif // __LYNX_EXCEL_TO_DBC_TOOL_DATA_H__

