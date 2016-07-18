#ifndef __LYNX_TOOL_DBCMAKER_H__
#define __LYNX_TOOL_DBCMAKER_H__

#include "XLHandler.h"
#include "CommonLib.h"

namespace Lynx
{
    class DBCMaker
    {
    public:
        DBCMaker();
        virtual ~DBCMaker();

        bool Initial(const String& filePath);
        void Release();
        bool Convert(const String& fileName);
        bool ConvertString(const String& filename, unsigned int no, bool isClearMap);
        bool ConvertQuestInfo(const String& fileName, unsigned int no, bool isClearMap);

    private:
        String m_Path;
        //QuestInfoTemplateTable m_QuestInfoTemplateTable;
    };
}; // namespace Lynx

#endif // __LYNX_TOOL_DBCMAKER_H__

