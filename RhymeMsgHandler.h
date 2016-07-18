#ifndef __LYNX_GAME_SERVER_RHYME_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_RHYME_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct RhymeMsgHandler
	{
		
		static void onRhymeEnhanceReq(const ConnId&, CGRhymeEnhanceReq&);
		static void onRhymeAcupointActiveReq(const ConnId&, CGRhymeAcupointActiveReq&);
		static void onRhymeSkillSetReq(const ConnId&, CGRhymeSkillSetReq&);
		static void onRhymeStepReq(const ConnId&, CGRhymeStepReq&);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_RHYME_MSG_HANDLER_H__