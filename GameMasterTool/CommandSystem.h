#ifndef __LYNX_GAME_MASTER_TOOL_COMMAND_SYSTEM_H__
#define __LYNX_GAME_MASTER_TOOL_COMMAND_SYSTEM_H__

#include "CommonLib.h"
#include "GMClient.h"

namespace Lynx
{
    class CommandSystem : public Singleton<CommandSystem>
    {
    public:
        CommandSystem();
        virtual ~CommandSystem();

        bool initial();
        void release();

        void execCommand(const String& str);

    private:
        void execChat(const String& cmd);
        void execHelp(const Vector<String>& v);
        void execClear(const Vector<String>& v);
        void execSetPlayerExp(const Vector<String>& v);
        void execSetPlayerLevel(const Vector<String>& v);
        void execSetPlayerEnergy(const Vector<String>& v);
        void execSetPlayerGold(const Vector<String>& v);
        void execAddItem(const Vector<String>& v);
        void execDelItem(const Vector<String>& v);
        void execAddEquip(const Vector<String>& v);
        void execDelEquip(const Vector<String>& v);
        void execAddRune(const Vector<String>& v);
        void execDelRune(const Vector<String>& v);
        void execAddHeroDebris(const Vector<String>& v);
        void execDelHeroDebris(const Vector<String>& v);
        void execAddHero(const Vector<String>& v);
        void execDelHero(const Vector<String>& v);
        void execAddPet(const Vector<String>& v);
        void execDelPet(const Vector<String>& v);
        void execSetHeroExp(const Vector<String>& v);
        void execSetHeroAdvanced(const Vector<String>& v);
        void execSetHeroLevel(const Vector<String>& v);
        void execSetHeroStarLevel(const Vector<String>& v);
        void execSetHeroPresentPos(const Vector<String>& v);
        void execSetHeroEquip(const Vector<String>& v);
        void execSetHeroRune(const Vector<String>& v);
        void execSetEquipRefine(const Vector<String>& v);
        void execSetRuneRefine(const Vector<String> & v);
        void execSetTollgate(const Vector<String> & v);
        void execAddPetDebris(const Vector<String>& v);

    private:
        GMClient* mCurrentGMClient;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_COMMAND_SYSTEM_H__
