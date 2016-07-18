#ifndef __LYNX_GAME_SERVER_PET_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_PET_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
    struct PetMsgHandler
    {
        static void onRefinePetReq(const ConnId&, RefinePetReq&);
        static void onAwakenPetReq(const ConnId&, AwakenPetReq&);
        static void onGetTalentReq(const ConnId&, GetTalentReq&);
        static void onSaveTalentReq(const ConnId&, SaveTalentReq&);
        static void onCombinePetReq(const ConnId&, CombinePetReq&);
        static void onSetPetPresentPosReq(const ConnId&, SetPetPresentPosReq&);

        static void onAdvancePetReq(const ConnId&, AdvancePetReq&);
    };
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_PET_MSG_HANDLER_H__

