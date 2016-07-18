#ifndef __XLHANDLE_H__
#define __XLHANDLE_H__

#include "PlatformLib.h"
#include "XLInc.h"
#include <list>
#include <map>
#include <string>

namespace Lynx
{
    struct _Worksheet;
    struct _Application;

    enum FLAG
    {
        FLAG_NULL = 0,
        FLAG_CSV  = 1,
    };

    struct _WideCharToMultiByte
    {
        void operator()(std::wstring wStr, String &str)
        {
            DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wStr.c_str(), -1, NULL, 0, NULL, FALSE);
            char *psText;
            psText = new char[dwNum];

            WideCharToMultiByte (CP_OEMCP, NULL, wStr.c_str() ,-1, psText, dwNum, NULL, FALSE);
            str = psText;
            delete []psText;
        }
    };


    /*
    ============================================
    FieldTable Type Definition
    ============================================
    */
    typedef struct FieldTable
    {        
        String m_FieldName1;     
        String m_FieldName2;     
        bool        m_bReplace;   

        FieldTable(void) 
            : m_FieldName1("")
            , m_FieldName2("")
            , m_bReplace(false)
        {
        }
    } FieldTable_t;

    struct FieldTableCompare
    {
    public:
        FieldTableCompare(String name)
            : m_Name(name)
        {
        }
        
        bool operator() (const FieldTable_t& v1) const 
        {
            if (v1.m_FieldName1.compare(m_Name) == 0)
                return true;
            else
                return false;
        }

    private:
        String m_Name;
    };

    typedef std::list<FieldTable_t> FieldTableList_t;
    typedef std::map<String, FieldTableList_t> FieldTableMap_t;


    /*
    ============================================
    DictionaryTable Type Definition
    ============================================
    */
    typedef std::map<int, String> DictionaryTableMap_t;

    class XLHandler
    {
    public:
        // 构造/析构
                                XLHandler(void);
                                ~XLHandler(void);

        // 方法
        void                    Init(void);
        void                    Release(void);
        bool                    Open(void);
        bool                    Proc(LPCTSTR fileName, int flags = FLAG_CSV, bool* cacelNotify = false);
        bool                    SaveToCsv(Excel::_Worksheet* sheet, int No);
        CString                 SaveToExcel(const String& xlFile);

        void                    LoadFieldTable(const String& FieldTableName);
        void                    LoadDictionnaryTable(const String& DictionnaryTableName);

        bool                    RenameFile(const String& src);
        void                    DelString(const String& dest, const String& rplaceStr);

        // 属性
        void                    SetSheetCount(unsigned long count);
        unsigned long           GetSheetCount(void) const;

    protected:
        void                    RenameStringTable(const String& filepath);
        void                    RenameQuestInfoTable(const String& filepath);

    private:
        int                     m_flags;
        CString                 m_fileName;
        bool*                   m_cacelNotify;
        Excel::_Application*    m_XL;

        FieldTableMap_t         m_FieldTableMap;
        DictionaryTableMap_t    m_DictionaryTableMap;

        // 记录要
        unsigned long           m_SheetCount;
    };
}; // namespace Lynx

#endif // __XLHANDLE_H__