#include "CommandSystem.h"
#include "GameMasterTool.h"

using namespace Lynx;

const char* sCommandHelp =
"------------基本操作------------\r\n\r\n"
".Clear\r\n"
"------------玩家操作------------\r\n\r\n"
".SetPlayerExp  玩家名称  经验值\r\n"
".SetPlayerLevel  玩家名称  等级值\r\n"
".SetPlayerEnergy  玩家名称  体力值\r\n"
".SetPlayerGold  玩家名称  金币值\r\n"
".AddItem  玩家名称  模板ID  数量\r\n"
".DelItem  玩家名称  道具ID\r\n"
".AddEquip  玩家名称  模板ID  数量\r\n"
".DelEquip  玩家名称  装备ID\r\n"
".AddRune  玩家名称  模板ID  数量\r\n"
".DelRune  玩家名称  符文ID\r\n"
".AddHeroDebris  玩家名称  模板ID  数量\r\n"
".DelHeroDebris  玩家名称  碎片ID\r\n"
".AddHero  玩家名称  模板ID  数量\r\n"
".DelHero  玩家名称  模板ID  数量\r\n"
".AddPet  玩家名称  模板ID  数量\r\n"
".DelPet  玩家名称  模板ID  数量\r\n"
".AddPetDebris 玩家名称 模板ID   数量\r\r"
"\r\n------------英雄操作------------\r\n\r\n"
".SetHeroExp  玩家名称  英雄ID  经验值\r\n"
".SetHeroLevel  玩家名称  英雄ID  等级值\r\n"
".SetHeroStarLevel  玩家名称  英雄ID  星级\r\n"
".setHeroAdvanced  玩家名称  英雄ID  星级\r\n"
".SetHeroPresentPos  玩家名称  英雄ID  位置(0不上阵，1，位置1（左），2，位置2（中），3， 位置3（右），4，替补1，5，替补2)\r\n"
".SetHeroEquip  玩家名称  装备ID 装备位置(0-25)\r\n"
".SetHeroRune  玩家名称  符文ID  符文位置(0-25)\r\n"
"\r\n------------装备操作------------\r\n\r\n"
".SetEquipRefine  玩家名称  装备ID  等级值\r\n"
"\r\n------------符文操作------------\r\n\r\n"
".SetRuneRefine  玩家名称  符文ID  等级值\r\n"
".SetTollgate 玩家名称 关卡章节 关卡 星级 次数"
;

CommandSystem::CommandSystem() : mCurrentGMClient(NULL)
{

}

CommandSystem::~CommandSystem()
{

}

bool 
CommandSystem::initial()
{
    return true;
}

void 
CommandSystem::release()
{

}

void 
CommandSystem::execCommand(const String& str)
{
    if (str.empty())
    {
        return;
    }

    if (str[0] != '.')
    {
        execChat(str);
        return;
    }

    Vector<String> v;
    lynxStrSplit(str, " ", v, true);

    String cmd = v[0];
    lynxToLowerCase(cmd);

    mCurrentGMClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!mCurrentGMClient)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "执行指令失败, 当前GM未连接服务器";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    if (cmd == ".help")
    {
        execHelp(v);
    }
    else if (cmd == ".clear")
    {
        execClear(v);
    }
    else if (cmd == ".setplayerexp")
    {
        execSetPlayerExp(v);
    }
    else if (cmd == ".setplayerlevel")
    {
        execSetPlayerLevel(v);
    }
    else if (cmd == ".setplayerenergy")
    {
        execSetPlayerEnergy(v);
    }
    else if (cmd == ".setplayergold")
    {
        execSetPlayerGold(v);
    }
    else if (cmd == ".additem")
    {
        execAddItem(v);
    }
    else if (cmd == ".delitem")
    {
        execDelItem(v);
    }
    else if (cmd == ".addequip")
    {
        execAddEquip(v);
    }
    else if (cmd == ".delequip")
    {
        execDelEquip(v);
    }
    else if (cmd == ".addrune")
    {
        execAddRune(v);
    }
    else if (cmd == ".delrune")
    {
        execDelRune(v);
    }
    else if (cmd == ".addherodebris")
    {
        execAddHeroDebris(v);
    }
    else if (cmd == ".delherodebris")
    {
        execDelHeroDebris(v);
    }
    else if (cmd == ".addhero")
    {
        execAddHero(v);
    }
    else if (cmd == ".delhero")
    {
        execDelHero(v);
    }
    else if (cmd == ".addpet")
    {
        execAddPet(v);
    }
    else if (cmd == ".delpet")
    {
        execDelPet(v);
    }
    else if (cmd == ".setheroexp")
    {
        execSetHeroExp(v);
    }
    else if (cmd == ".setheroadvanced")
    {
        execSetHeroAdvanced(v);
    }
    else if (cmd == ".setherolevel")
    {
        execSetHeroLevel(v);
    }
    else if (cmd == ".setherostarlevel")
    {
        execSetHeroStarLevel(v);
    }
    else if (cmd == ".setheropresentpos")
    {
        execSetHeroPresentPos(v);
    }
    else if (cmd == ".setheroequip")
    {
        execSetHeroEquip(v);
    }
    else if (cmd == ".setherorune")
    {
        execSetHeroRune(v);
    }
    else if (cmd == ".setequiprefine")
    {
        execSetEquipRefine(v);
    }
    else if (cmd == ".setrunerefine")
    {
        execSetRuneRefine(v);
    }
    else if (cmd == ".settollgate")
    {
        execSetTollgate(v);
    }
    else if (cmd == ".addpetdebris")
    {
        execAddPetDebris(v);
    }
    else
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "无效指令【" + cmd;
        printEvent.mLogContents += "】";
        EventSystem::getSingleton().postEvent(printEvent);
    }
}

void 
CommandSystem::execChat(const String& cmd)
{

}

void 
CommandSystem::execHelp(const Vector<String>& v)
{
    WindowLogPrintEvent printEvent;
    printEvent.mLogContents = sCommandHelp;
    EventSystem::getSingleton().postEvent(printEvent);
}

void 
CommandSystem::execClear(const Vector<String>& v)
{
    WindowLogClearEvent ev;
    EventSystem::getSingleton().postEvent(ev);
}

void 
CommandSystem::execSetPlayerExp(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetPlayerExp 玩家名称  经验值】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetPlayerExpReq req;
    req.mPlayerName = v[1];
    req.mLevelExp = lynxAtoi<UInt32>(v[2].c_str());
    
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetPlayerLevel(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetPlayerLevel  玩家名称  等级值】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetPlayerLevelReq req;
    req.mPlayerName = v[1];
    req.mLevel = lynxAtoi<UInt32>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);

}

void 
CommandSystem::execSetPlayerEnergy(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetPlayerEnergy  玩家名称  体力值】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetPlayerEnergyReq req;
    req.mPlayerName = v[1];
    req.mEnergy = lynxAtoi<UInt32>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);

}

void 
CommandSystem::execSetPlayerGold(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetPlayerGold  玩家名称  金币值】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetPlayerGoldReq req;
    req.mPlayerName = v[1];
    req.mGold = lynxAtoi<UInt32>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddItem(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.AddItem  玩家名称  模板ID  数量】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddItemReq req;
    req.mPlayerName = v[1];
    req.mItemTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddPetDebris(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.AddPetDebris  玩家名称  模板ID  数量】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddPetDebrisReq req;
    req.mPlayerName = v[1];
    req.mDebrisTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelItem(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.DelItem  玩家名称  道具ID 数量】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelItemReq req;
    req.mPlayerName = v[1];
    req.mItemGuid = lynxAtoi<Guid>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddEquip(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.AddEquip  玩家名称  模板ID  数量】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddEquipReq req;
    req.mPlayerName = v[1];
    req.mEquipTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelEquip(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.DelEquip  玩家名称  装备ID】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelEquipReq req;
    req.mPlayerName = v[1];
    req.mEquipGuid = lynxAtoi<Guid>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddRune(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.AddRune  玩家名称  模板ID  数量】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddRuneReq req;
    req.mPlayerName = v[1];
    req.mRuneTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelRune(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.DelRune  玩家名称  符文ID】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelRuneReq req;
    req.mPlayerName = v[1];
    req.mRuneGuid = lynxAtoi<Guid>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddHeroDebris(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.AddHeroDebris  玩家名称  模板ID  数量】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddHeroDebrisReq req;
    req.mPlayerName = v[1];
    req.mHeroDebrisTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelHeroDebris(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.DelHeroDebris  玩家名称  碎片ID 数量】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelHeroDebrisReq req;
    req.mPlayerName = v[1];
    req.mHeroDebrisGuid = lynxAtoi<Guid>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddHero(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.AddHero  玩家名称  模板ID  数量】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddHeroReq req;
    req.mPlayerName = v[1];
    req.mHeroTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelHero(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.DelHero  玩家名称  英雄ID】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelHeroReq req;
    req.mPlayerName = v[1];
    req.mHeroGuid = lynxAtoi<Guid>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execAddPet(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.AddPet  玩家名称  模板ID  数量】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMAddPetReq req;
    req.mPlayerName = v[1];
    req.mPetTemplateId = lynxAtoi<UInt32>(v[2].c_str());
    req.mCount = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execDelPet(const Vector<String>& v)
{
    if (v.size() != 3)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.DelPet  玩家名称  神兽ID】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMDelPetReq req;
    req.mPlayerName = v[1];
    req.mPetGuid = lynxAtoi<Guid>(v[2].c_str());
    mCurrentGMClient->sendMsg(req);
}


void 
CommandSystem::execSetHeroExp(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetHeroExp  玩家名称  英雄ID  经验值】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroExpReq req;
    req.mPlayerName = v[1];
    req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mLevelExp = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroAdvanced(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetHeroAdvanced  玩家名称  英雄ID  进阶值】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroAdvancedReq req;
    req.mPlayerName = v[1];
    req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mAdvanced = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroLevel(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetHeroLevel  玩家名称  英雄ID  等级值】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroLevelReq req;
    req.mPlayerName = v[1];
    req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mLevel = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroStarLevel(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetHeroStartLevel  玩家名称  英雄ID  星级值】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroStarLevelReq req;
    req.mPlayerName = v[1];
    req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mStarLevel = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroPresentPos(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetHeroPresentPos  玩家名称  英雄ID  位置(0不上阵，1，位置1（左），2，位置2（中），3， 位置3（右），4，替补1，5，替补2)】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroPresentPosReq req;
    req.mPlayerName = v[1];
    req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mPresentPos = lynxAtoi<UInt8>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroEquip(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetHeroEquip  玩家名称   装备ID  位置(0-25)】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroEquipReq req;
    req.mPlayerName = v[1];
    //req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mEquipId = lynxAtoi<Guid>(v[2].c_str());
    req.mPresentPos = lynxAtoi<UInt8>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetHeroRune(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetHeroRune  玩家名称  符文ID  位置(0-25)】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetHeroRuneReq req;
    req.mPlayerName = v[1];
    //req.mHeroId = lynxAtoi<Guid>(v[2].c_str());
    req.mRuneId = lynxAtoi<Guid>(v[2].c_str());
    req.mPresentPos = lynxAtoi<UInt8>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetEquipRefine(const Vector<String>& v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetEquipRefine  玩家名称  装备ID  等级值】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetEquipRefineReq req;
    req.mPlayerName = v[1];
    req.mEquipId = lynxAtoi<Guid>(v[2].c_str());
    req.mRefineLevel = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetRuneRefine(const Vector<String> & v)
{
    if (v.size() != 4)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetRuneRefine  玩家名称  符文ID  等级值】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetRuneRefineReq req;
    req.mPlayerName = v[1];
    req.mRuneId = lynxAtoi<Guid>(v[2].c_str());
    req.mRefineLevel = lynxAtoi<UInt32>(v[3].c_str());
    mCurrentGMClient->sendMsg(req);
}

void 
CommandSystem::execSetTollgate(const Vector<String> & v)
{
    if (v.size() != 6)
    {
        WindowLogPrintEvent printEvent;
        printEvent.mLogContents = "参数错误, 【.SetTollgate 玩家名称 关卡章节 关卡 星级 次数】";
        EventSystem::getSingleton().postEvent(printEvent);
        return;
    }

    GMSetTollgateReq req;
    req.mPlayerName = v[1];
    req.mChapterId = lynxAtoi<UInt8>(v[2].c_str());
    req.mTollgateNum = lynxAtoi<UInt8>(v[3].c_str());
    req.mStarCount = lynxAtoi<UInt8>(v[4].c_str());
    req.mCount = lynxAtoi<UInt8>(v[5].c_str());
    mCurrentGMClient->sendMsg(req);
}

