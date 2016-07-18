#include "CommonLib.h"

#include "TypeConditionTable.h"
#include "AwardAddRateTable.h"
#include "AttrEnhanceTable.h"
#include "AttrPolishTable.h"
#include "AttrRandomTable.h"
#include "BuffTable.h"
#include "ChapterTable.h"
#include "EffectTable.h"
#include "EquipGemRuleTable.h"
#include "EquipSetTable.h"
#include "EquipTable.h"
#include "FashionTable.h"
#include "GemCombineOnceTable.h"
#include "GemCombineTable.h"
#include "GemLvTable.h"
#include "GemTable.h"
#include "HeroEquipRuleTable.h"
#include "HeroTable.h"
#include "HoarstoneBaseTable.h"
#include "HoarstoneLvTable.h"
#include "HoarstoneRuneTable.h"
#include "HoarstoneStarTable.h"
#include "ItemTable.h"
#include "LevelGrowTable.h"
#include "MaterialPointsTable.h"
#include "MaterialTable.h"
#include "NewEquipLvTable.h"
#include "PlayerExpTable.h"
#include "PolishPointsTable.h"
#include "RhymeAcupointTable.h"
#include "RhymeLevelTable.h"
#include "RhymeSkillTable.h"
#include "SkillBaseTable.h"
#include "SkillLevelTable.h"
#include "SkillTable.h"
#include "StageTable.h"

#include "MysteryShopTable.h"
#include "TowersTable.h"



using namespace Lynx;

int main(int argc, char** argv) 
{
    char buf[409600]; 
    Map<String, List<String> > fileMap; 
    StringMap<String> tableFileMap; 

    FileUtil::findFiles("../../CommonLib", "*Table.h", tableFileMap, false);
    tableFileMap.erase("Table.h");
    StreamBuffer stream;
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s#ifndef __LYNX_GAME_COMMON_LIB_TABLE_H__\r\n", buf);
    sprintf(buf, "%s#define __LYNX_GAME_COMMON_LIB_TABLE_H__\r\n\r\n", buf);
    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))
    {
         sprintf(buf, "%s#include \"%s.h\"\r\n", buf, lynxGetMainFileName(iter->mKey).c_str());
    }
    sprintf(buf, "%s\r\nnamespace Lynx\r\n", buf);
    sprintf(buf, "%s{\r\n", buf);
    sprintf(buf, "%s    class TableManager : public Singleton<TableManager>\r\n", buf);
    sprintf(buf, "%s    {\r\n\r\n", buf);
    sprintf(buf, "%s    public:\r\n", buf);
    sprintf(buf, "%s        TableManager() {}\r\n", buf);
    sprintf(buf, "%s        virtual ~TableManager() {}\r\n\r\n", buf);
    sprintf(buf, "%s        bool load();\r\n", buf);
    sprintf(buf, "%s        void unload();\r\n", buf); 
    sprintf(buf, "%s        bool reload(const String& fileName);\r\n", buf);
    sprintf(buf, "%s        bool reloadAll();\r\n\r\n", buf);
    sprintf(buf, "%s    public:\r\n", buf);
    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))
    {
         sprintf(buf, "%s        %s m%s;\r\n", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());
    }
    sprintf(buf, "%s    };\r\n", buf);
    sprintf(buf, "%s}\r\n\r\n", buf);
    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))
    {
         sprintf(buf, "%sextern Lynx::%s* g%s;\r\n", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());
    }
    sprintf(buf, "%s\r\n", buf);
    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))
    {
         sprintf(buf, "%s#define %s() (*g%s)\r\n", buf, lynxGetMacroFileName(lynxGetMainFileName(iter->mKey)).c_str(), lynxGetMainFileName(iter->mKey).c_str());
    }
    sprintf(buf, "%s#endif", buf);
    FileUtil::saveFile("../../CommonLib/Table.h", buf, strlen(buf));
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s#include \"Table.h\"\r\n\r\n", buf);
    sprintf(buf, "%susing namespace Lynx;\r\n\r\n", buf);
    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))
    {
         sprintf(buf, "%s%s* g%s = NULL;\r\n", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());
    }
    sprintf(buf, "%s\r\n", buf);
    sprintf(buf, "%sbool\r\nTableManager::load()\r\n", buf);
    sprintf(buf, "%s{\r\n", buf);
    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))
    {
         sprintf(buf, "%s    if (!m%s.loadFromDbc(\"%s.dbc\"))\r\n", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());
         sprintf(buf, "%s    {\r\n", buf);
         sprintf(buf, "%s        LOG_WARN(\"Failed to load %s table.\");\r\n", buf, lynxGetMainFileName(iter->mKey).c_str());
         sprintf(buf, "%s        return false;\r\n", buf);
         sprintf(buf, "%s    }\r\n", buf);
         sprintf(buf, "%s    g%s = &m%s;\r\n", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());
        sprintf(buf, "%s\r\n", buf);
    }
    sprintf(buf, "%s    return true;\r\n", buf);
    sprintf(buf, "%s}\r\n", buf);
    sprintf(buf, "%s\r\n", buf);
    sprintf(buf, "%svoid\r\nTableManager::unload()\r\n", buf);
    sprintf(buf, "%s{\r\n", buf);
    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))
    {
    sprintf(buf, "%s    m%s.clear();\r\n", buf, lynxGetMainFileName(iter->mKey).c_str());
    sprintf(buf, "%s    g%s = NULL;\r\n", buf, lynxGetMainFileName(iter->mKey).c_str());
    }
    sprintf(buf, "%s}\r\n", buf);
    sprintf(buf, "%s\r\n", buf);
    sprintf(buf, "%sbool\r\nTableManager::reload(const String& fileName)\r\n", buf);
    sprintf(buf, "%s{\r\n", buf);
    sprintf(buf, "%s    String fileNameFix = fileName;\r\n", buf);
    sprintf(buf, "%s    lynxToLowerCase(fileNameFix);\r\n\r\n", buf);
    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))
    {
        String lowerName = lynxGetMainFileName(iter->mKey);
        lynxToLowerCase(lowerName);
        sprintf(buf, "%s    if (fileNameFix == \"%s.dbc\")\r\n", buf, lowerName.c_str());
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s        if (!m%s.reloadFromDbc(fileName))\r\n", buf, lynxGetMainFileName(iter->mKey).c_str());
        sprintf(buf, "%s        {\r\n", buf);
        sprintf(buf, "%s           LOG_WARN(\"Failed to reload %s table.\");\r\n", buf, lynxGetMainFileName(iter->mKey).c_str());
        sprintf(buf, "%s           return false;\r\n", buf);
        sprintf(buf, "%s        }\r\n", buf);
        sprintf(buf, "%s        return true;\r\n", buf);
        sprintf(buf, "%s    }\r\n\r\n", buf);
    }
    sprintf(buf, "%s    return false;\r\n", buf);
    sprintf(buf, "%s}\r\n", buf);
    sprintf(buf, "%sbool\r\nTableManager::reloadAll()\r\n", buf);
    sprintf(buf, "%s{\r\n", buf);
    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))
    {
        sprintf(buf, "%s    if (!m%s.reloadFromDbc(\"%s.dbc\"))\r\n", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());
        sprintf(buf, "%s    {\r\n", buf);
        sprintf(buf, "%s       LOG_WARN(\"Failed to reload %s table.\");\r\n", buf, lynxGetMainFileName(iter->mKey).c_str());
        sprintf(buf, "%s       return false;\r\n", buf);
        sprintf(buf, "%s    }\r\n", buf);
    }
    sprintf(buf, "%s    return true;\r\n", buf);
    sprintf(buf, "%s}\r\n\r\n", buf);
    FileUtil::saveFile("../../CommonLib/Table.cpp", buf, strlen(buf));

    return 0;
}
