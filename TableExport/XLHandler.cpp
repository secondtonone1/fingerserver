#include "XLHandler.h"
#include <comdef.h>
#include <comip.h>
#include <stdio.h>
#include "HelpHandler.h"
#include "WXLib.h"
#include <algorithm>
#include "DBCMaker.h"

using namespace Lynx;

#define MAX_ROW_FIELD_TABLE 1500
#define MAX_ROW_DIRCTION_TABLE 1500

#define REPACE_ROW_FIELD_TABLE 2500
#define REPACE_COLUMN_FIELD_TABLE 200

static bool isDelString = true;

XLHandler::XLHandler(void)
    :m_SheetCount(0)
{
}

XLHandler::~XLHandler(void)
{
}

void    
XLHandler::Init(void)
{
    m_XL = NULL;
    m_flags = FLAG_NULL;
    m_fileName = "";
}

void
XLHandler::Release(void)
{
    if(m_XL) 
    {
        m_XL->Quit();
        m_XL->Release();
        m_XL = NULL;
    }
}

bool
XLHandler::Open(void)
{
    bool bRet = false;
    Excel::_ApplicationPtr XL;

    // 说明:
    //      CLSCTX_SERVER: 指定组件类别，可以指定进程内组件进程
    //      外组件或者进程内控制对象
    HRESULT hr = XL.CreateInstance(L"Excel.Application", NULL, CLSCTX_SERVER);

    if (XL)
    {
        m_XL = XL.Detach();
    }
    else
    {
        ::MessageBox(NULL, _T("获取 Application 失败!"), _T("错误"), MB_OK);
        return false;
    }

    return true;
}

bool
XLHandler::Proc(LPCTSTR fileName, int flags, bool* cacelNotify)
{
    if (m_XL == NULL)
    {
        ::MessageBox(NULL, _T("获取Excel应用程序失败!"), _T("错误"), MB_OK);
        return false;
    }

    CString file = fileName;
    CString sequenceNumber;
    sequenceNumber.Format(_T("%s"), _T(".xlsx") );

    int pos = file.ReverseFind(_T('.'));
    if(pos > 0)
    {
        file = file.Left(pos);
    } 
    else
    {
        file = file;        
    }

    file += sequenceNumber;

    m_fileName                  = file;
    m_cacelNotify               = cacelNotify;
    m_flags                     = flags;
    bool bRet                   = false;

    _variant_t vReadOnley(true);

    // 获得工作薄
    Excel::_Workbook* workbook  = NULL;
    workbook = m_XL->Workbooks->Open(_bstr_t(fileName), vtMissing).Detach();

    if (workbook == NULL)
    {
        ::MessageBox(NULL, _T("获得工作薄失败"), _T("错误"), MB_OK);
        return false;
    }

    Excel::SheetsPtr sheets = workbook->Sheets;
    if (sheets == NULL)
    {
        ::MessageBox(NULL, _T("获取标签失败!"), _T("错误"), MB_OK);
        return false;
    }

    Excel::_WorksheetPtr dataSheet;
    for(long i = 1;  i<= sheets->Count; i++) 
    {
        // SHEET(excel 中的标签)
        IDispatchPtr sheetDisp = sheets->Item[i];

        if (sheetDisp == NULL)
        {
            ::MessageBox(NULL, _T("Sheets获取失败"), _T("错误"), MB_OK);
            return false;
        }

        Excel::_WorksheetPtr sheet(sheetDisp);

        if (sheet == NULL)
        {
            ::MessageBox(NULL, _T("Sheet获取失败"), _T("错误"), MB_OK);
            return false;
        }

        // _bstr_t name = sheet->Name;

        dataSheet = sheetDisp;
        if(dataSheet)
        {
            bRet = SaveToCsv(dataSheet, i);
            if(bRet)
            {
                // Excel 用完了要保存
                workbook->put_Saved(0, VARIANT_TRUE);
                m_SheetCount++;
            }
        }

        String strFilename = HelpHandler::to_string(sheet->Name);
        if (strFilename.compare(0, 11, "StringTable", 11) == 0)
        {            
            continue;
        }

        break;
    }

    if(workbook)
    {
        workbook->Close();
        workbook->Release();
    }
    return bRet;
}

bool
XLHandler::SaveToCsv(Excel::_Worksheet* sheet, int No)
{
    if (sheet == NULL)
    {
        ::MessageBox(NULL, _T("保存 csv 时参数为空!"), _T("错误"), MB_OK);
        return false;
    }
    
    CString fileName;
    CString sequenceNumber;
    int pos = 0;

    const String sheetname = HelpHandler::to_string(sheet->Name);
    if (sheetname.compare(0, 5, "StrID=", 5) == 0)
    {
        sequenceNumber.Format(_T("%d%s"), No,_T("_tmp.csv") );
        pos = m_fileName.ReverseFind(_T('_'));
    }
    else
    {        
        sequenceNumber.Format(_T("%s"), _T(".csv") );
        pos = m_fileName.ReverseFind(_T('.'));
    }
    
    if(pos > 0)
    {
        fileName = m_fileName.Left(pos);
    } 
    else
    {
        fileName = m_fileName;        
    }

    fileName += sequenceNumber;
    if(fileName != m_fileName)
    {
        ::DeleteFile(fileName);
        sheet->SaveAs(_bstr_t(fileName), _variant_t((long)Excel::xlCSV));
    }

    return true;
}

void
XLHandler::LoadFieldTable(const String& FieldTableName)
{
    CString fileName = HelpHandler::s2ws(FieldTableName).c_str();

    Excel::_Workbook* workbook  = NULL;
    workbook = m_XL->Workbooks->Open(_bstr_t(fileName), vtMissing).Detach();
    
    if (workbook == NULL)
    {
        ::MessageBox(NULL, _T("载入字段表失败!"), _T("错误"), MB_OK);
        return;
    }

    Excel::SheetsPtr sheets = workbook->Sheets;
    if (sheets == NULL)
    {
        ::MessageBox(NULL, _T("获取表集失败!"), _T("错误"), MB_OK);
        return;
    }
   
    IDispatchPtr sheetDisp = sheets->Item[1];
    if (sheetDisp == NULL)
    {
        ::MessageBox(NULL, _T("载入 FieldTable 时获取 sheetDisp 失败!"), _T("错误"), MB_OK);
        return;
    }

    Excel::_WorksheetPtr sheet(sheetDisp);
    if (sheet)
    {
        String tableName("");
        FieldTableList_t fieldTableList;

        Excel::RangePtr pRange = sheet->GetRange(_bstr_t(_T("A1")), _bstr_t(_T("Z1")));
        for (int rows = 1; rows < MAX_ROW_FIELD_TABLE; rows++)
        {
            // 策划用字段
            _variant_t vItem1 = pRange->Item[rows][1];
            _bstr_t bstrText1(vItem1);

            // 程序用字段
            _variant_t vItem2 = pRange->Item[rows][2];
            _bstr_t bstrText2(vItem2);

            // 是否替换
            _variant_t vItem3 = pRange->Item[rows][3];
            _bstr_t bstrText3(vItem3);

            if (bstrText1.length() == 0 || bstrText2.length() == 0)
            {
                break;
            }
            else
            {
                const String text1 = HelpHandler::to_string(bstrText1);
                const String text2 = HelpHandler::to_string(bstrText2);
                const String text3 = HelpHandler::to_string(bstrText3);
    
                if (text1.compare("TableName") == 0)
                {
                    fieldTableList.clear();
                    tableName = text2;
                    m_FieldTableMap.insert(make_pair(tableName, fieldTableList));
                }
                else
                {
                    FieldTableMap_t::iterator it_find = m_FieldTableMap.find(tableName);
                    if (it_find != m_FieldTableMap.end() )
                    {
                        FieldTable_t fieldTable;
                        fieldTable.m_FieldName1 = text1;
                        fieldTable.m_FieldName2 = text2;
                        fieldTable.m_bReplace   = ( (text3.compare("1") == 0) ? true : false );
                        ((*it_find).second).push_back(fieldTable);
                    }
                }
            }
        }
    }

    if(workbook)
    {
        workbook->Close();
        workbook->Release();
    }
}

void
XLHandler::LoadDictionnaryTable(const String& dictionnaryTableName)
{
    CString fileName = HelpHandler::s2ws(dictionnaryTableName).c_str();
    
    Excel::_Workbook* pBook = NULL;
    pBook = m_XL->Workbooks->Open(_bstr_t(fileName), vtMissing).Detach();
    if (pBook == NULL)
    {
        ::MessageBox(NULL, _T("LoadDictionnaryTable函数, 载入字段表失败!"), _T("错误"), MB_OK);
        return;
    }

    Excel::SheetsPtr pSheets = pBook->Sheets;
    if (pSheets == NULL)
    {
        ::MessageBox(NULL, _T("LoadDictionnaryTable函数, 获取表集失败!"), _T("错误"), MB_OK);
        return;
    }

    IDispatchPtr sheetDisp = pSheets->Item[1];
    if (sheetDisp == NULL)
    {
        ::MessageBox(NULL, _T("LoadDictionnaryTable函数, 获取sheetDisp失败!"), _T("错误"), MB_OK);
        return;
    }

    Excel::_WorksheetPtr pSheet(sheetDisp);
    if (pSheet)
    {
        Excel::RangePtr pRange = pSheet->GetRange(_bstr_t(_T("A1")), _bstr_t(_T("Z1")));
        for (int rows = 2; rows < MAX_ROW_DIRCTION_TABLE; rows++)
        {
            _variant_t vItem1 = pRange->Item[rows][1];
            _variant_t vItem2 = pRange->Item[rows][2];
            _bstr_t bstrText1(vItem1);
            _bstr_t bstrText2(vItem2);

            if (bstrText1.length() == 0 || bstrText2.length() == 0)
            {
                break;
            }
            else
            {
                int id = atoi( (const char*)bstrText1);
                DictionaryTableMap_t::const_iterator it_find = m_DictionaryTableMap.find(id);
                if (it_find != m_DictionaryTableMap.end())
                {
                    String promptInfo("错误, 载入字典字段重复,ID = ");
                    promptInfo += HelpHandler::to_string(bstrText1);
                    ::wxMessageBox(promptInfo.c_str());
                    break;                    
                }
                else
                {
                    m_DictionaryTableMap.insert(make_pair(id, HelpHandler::to_string(bstrText2) ));
                }
            }
        }
    }
    
    if(pBook)
    {
        pBook->Close();
        pBook->Release();
    }
}

CString
XLHandler::SaveToExcel(const String& xlFile)
{
    std::wstring wstrFileName = HelpHandler::s2ws(xlFile);
    const CString name = wstrFileName.c_str();    

    CString fileName;
    CString sequenceNumber;
    
    Excel::_Workbook* workbook  = NULL;
    Excel::WorkbooksPtr ptr = m_XL->Workbooks;
    
    if (ptr == NULL)
    {
        ::MessageBox(NULL, _T("SaveToExcel 工作薄指针为空!"), _T("错误"), MB_OK);
        return "";
    }

    workbook = m_XL->Workbooks->Open(_bstr_t(name), vtMissing).Detach();

    if (workbook == NULL)
    {
        ::MessageBox(NULL, _T("SaveToExcel 工作薄为空!"), _T("错误"), MB_OK);
        return "";
    }

    Excel::SheetsPtr sheets = workbook->Sheets;
    if (sheets == NULL)
    {
        ::MessageBox(NULL, _T("SaveToExcel 获取标签失败!"), _T("错误"), MB_OK);
        return "";
    }

    for (int i = 1; i <= sheets->Count; i++)
    {
        IDispatchPtr sheetDisp = sheets->Item[i];
        if (sheetDisp == NULL)
        {
            ::MessageBox(NULL, _T("SaveToExcel 获取 sheetDisp 失败!"), _T("错误"), MB_OK);
            return "";
        }

        Excel::_WorksheetPtr sheet(sheetDisp);
        if(sheet)
        {   
            Excel::RangePtr pRange = sheet->GetRange(_bstr_t(_T("A1")), _bstr_t(_T("AZ1"))); 

            if (pRange == NULL)
            {
                ::MessageBox(NULL, _T("SaveToExcel 获取 pRange 失败!"), _T("错误"), MB_OK);
                return "";
            } 

            for (int iColumn = 1; iColumn < REPACE_COLUMN_FIELD_TABLE; iColumn++)
            {
                _variant_t vItem = pRange->Item[1][iColumn];
                _bstr_t bstrText( vItem );
                if (bstrText.length() == 0)
                {
                    break;
                }
                else
                {
                    FieldTableMap_t::const_iterator it_find_sheet_name = m_FieldTableMap.find(HelpHandler::to_string(sheet->Name));
                    if (it_find_sheet_name != m_FieldTableMap.end() )
                    {                    
                        FieldTableCompare compareName(HelpHandler::to_string(bstrText));

                        FieldTableList_t tableList = it_find_sheet_name->second;
                        FieldTableList_t::const_iterator it = find_if(tableList.begin(), tableList.end(), compareName);

                        if (it != tableList.end())
                        {
                            // 替换字段
                            //pRange->Item[1][iColumn] = _variant_t((*it).m_FieldName2.c_str());

                            // 替换内容
                            int replaceColumn = iColumn;
                            if (it->m_bReplace == true)
                            {
                                for (int iRow = 3; iRow < REPACE_ROW_FIELD_TABLE; iRow++)
                                {
                                    _bstr_t bstrRelpaceText(pRange->Item[iRow][replaceColumn]);
                                    if (bstrRelpaceText.length() == 0)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        const String strReplaceText = HelpHandler::to_string(bstrRelpaceText);
                                        DictionaryTableMap_t::iterator it_Replace = m_DictionaryTableMap.find(atoi(strReplaceText.c_str()));
                                        if (it_Replace != m_DictionaryTableMap.end())
                                        {
                                            pRange->Item[iRow][replaceColumn] = _variant_t( (*it_Replace).second.c_str() );
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        //String sheetName = HelpHandler::to_string(sheet->Name) + "第一个标签名出错, 或FieldTable.xlsx表中没有该表名!";
                        //::wxMessageBox(sheetName);
                        //break;
                    }
                }
            }

            // 删除第一行
            long xlUp = -4162;
            pRange->GetEntireRow()->Delete(xlUp);

            // 另存为
            sequenceNumber.Format(_T("_tmp%s"), _T(".xlsx") );
            int pos = name.ReverseFind(_T('.'));
            if(pos > 0)
            {
                fileName = name.Left(pos);
            } 
            else
            {
                fileName = name;        
            }
            fileName += sequenceNumber;
            String srcFile;
            _WideCharToMultiByte()((std::wstring)(LPCTSTR)fileName, srcFile);
            int posReplace = srcFile.find("Table");
            srcFile.replace(posReplace, 5, "Data", 0, 4);
            fileName = HelpHandler::s2ws(srcFile).c_str();
            if(fileName != name)
            {
                ::remove(srcFile.c_str());
                //::DeleteFile(fileName);
                //int tmp = sheet->SaveAs(_bstr_t(fileName));
                workbook->put_Saved(0, VARIANT_TRUE);
            }
        }
    }

    if (workbook)
    {
        workbook->SaveCopyAs(_variant_t(fileName));
        workbook->Close();
        workbook->Release();
        
    }

    return fileName;
}

bool
XLHandler::RenameFile(const String& src)
{
    if (src.find("StringTable", 0) != String::npos)
    {
        RenameStringTable(src);
    }

    if (src.find("QuestInfoTemplate", 0) != String::npos)
    {
        RenameQuestInfoTable(src);
    }


    String addSrcStr = "_tmp.csv";
    String addDestStr = ".csv";

    String tmpPath(src);
    int pos = tmpPath.find(".xlsx");
    tmpPath.replace(pos, pos + 5, "");

    addSrcStr = tmpPath + addSrcStr;
    addDestStr = tmpPath + addDestStr;

    int posReplace = addSrcStr.find("Table");
    addSrcStr.replace(posReplace, 5, "Data", 0, 4);
    posReplace = addDestStr.find("Table");
    addDestStr.replace(posReplace, 5, "Data", 0, 4);

    rename(addSrcStr.c_str(), addDestStr.c_str());

    return true;
}

void
XLHandler::DelString(const String& dest,  const String& rplaceStr)
{
}

void
XLHandler::SetSheetCount(unsigned long count)
{
    m_SheetCount = count;
}

unsigned long
XLHandler::GetSheetCount(void) const
{
    return m_SheetCount;
}

void
XLHandler::RenameStringTable(const String& filepath)
{
    if (m_SheetCount == 0)
    {
        return;
    }    

    for (unsigned long i = 1; i <= m_SheetCount; i++)
    {
        String addSrcStr("_tmp.csv");
        String addDestStr(".csv");

        char buf[10] = {};
        _itoa(i, buf, 10);
        String tmpPath(filepath);
        int pos = tmpPath.find(".xlsx");
        tmpPath.replace(pos, pos + 5, buf);

        addSrcStr = tmpPath + addSrcStr;
        addDestStr = tmpPath + addDestStr;

        int posReplace = addSrcStr.find("Table");
        addSrcStr.replace(posReplace, 5, "Data", 0, 4);
        posReplace = addDestStr.find("Table");
        addDestStr.replace(posReplace, 5, "Data", 0, 4);

        rename(addSrcStr.c_str(), addDestStr.c_str());
    }
}

void
XLHandler::RenameQuestInfoTable(const String& filepath)
{
    if (m_SheetCount == 0)
    {
        return;
    }

    for (unsigned long i = 1; i <= m_SheetCount; i++)
    {
        String addSrcStr("_tmp.csv");
        String addDestStr(".csv");

        char buf[10] = {};
        _itoa(i, buf, 10);
        String tmpPath(filepath);
        int pos = tmpPath.find(".xlsx");
        tmpPath.replace(pos, pos + 5, buf);

        addSrcStr = tmpPath + addSrcStr;
        addDestStr = tmpPath + addDestStr;

        int posReplace = addSrcStr.find("Table");
        addSrcStr.replace(posReplace, 5, "Data", 0, 4);
        posReplace = addDestStr.find("Table");
        addDestStr.replace(posReplace, 5, "Data", 0, 4);

        rename(addSrcStr.c_str(), addDestStr.c_str());
    }    
}

