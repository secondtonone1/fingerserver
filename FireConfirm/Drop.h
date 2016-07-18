#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_D_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_D_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "Character.h"

namespace Lynx
{

	class Player;


	


	class DropManager
	{
		struct Drop
		{



// 			function T_Drop:ctor(id, dropType, value, pos, bFly, GScene, node)
// 			self.id = id
// 			self.dropType = dropType
// 			self.value = value
// 			self.pos = pos
// 			self.canFly = bFly
// 			self.server = GScene
// 			self.showNode = node
// 			self.flagEnd = false
// 			self.totalTime = 0
// 			self.haveRun = false
// 			self.targetPos = {x=0,y=0}

			UInt32 id;
			UInt32 dropTyp;
			UInt32 value;
			UInt32	 pos;			
			bool	canFly;
				//self.server = GScene
			UInt32	showNode ;
			bool	flagEnd ;
			UInt32	totalTime;
			bool	haveRun;
			Pos targetPos ;
			UInt64 waiteTime;
			UInt64 beforeDelayTime;
		};
	public:
		void moveEnd();
		Pos getPlayerPositon(Guid playerID);
		UInt32 runPath(Guid playerID);
	};








}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_D_MANAGER_H__
