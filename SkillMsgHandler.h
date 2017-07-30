#ifndef __LYNX_GAME_SERVER_SKILL_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_SKILL_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct SkillMsgHandler
	{
		static void onSkillLevelUpReq(const ConnId&, CGSkillLevelUpReq&);
		static void onSkillPositionSetReq(const ConnId&, CGSkillEquipSetReq&);
		static void onSkillLvUpOnceReq(const ConnId&, CGSkillLvUpOnceReq&);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_RHYME_MSG_HANDLER_H__