#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_SCENE_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_SCENE_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "./SkillBaseTable.h"
#include "Character.h"
#include "PVPThreadMsg.h"

namespace Lynx
{

	class Player;

	struct Parent
	{
		UInt32 selectFashionId;
		bool allowTouch;
		UInt32 treasureState;
		UInt32 useGemCount;
		UInt32 widget;
		UInt32 curStoneIndex;
		UInt32 UiNode;
		UInt32 MapNode;
		UInt32 actNode;
		
		Guid objUUID;
		Guid materailUUID;
		
		List<UInt32> equipResData;
	};
	struct DropContent
	{
		UInt32 drop_id;
		UInt32 dropType;
		UInt32 value;
		
		bool bFly;
		//SceneTempelete self;
		UInt32 par;
		Pos pos;
	};


	struct SceneCharacter
	{
		Map<Guid,Character*> CharacterMap;
	};

	struct SceneTempelete
	{
		UInt32 stageID;//add

		UInt32 DropIdBase;
		UInt32 activeNode;
		UInt32 heroSpeed;
		UInt32 rhymePower;

		UInt32 watchdog_hitRecordTime;
		UInt32 watchdog_hitChangeTime;

		UInt32 timer;
		UInt32 num ;
		UInt32 x1;
		UInt32 x2;

		bool isFire;
// 		bool canSkillControl;
// 		bool canRealMove;//应放在角色里
		bool initFlag;
		bool isQTE;

		Guid soldierUUID;
		Parent parent;		
		Pos sceneEdge;

		List<UInt32> bgHeight;
		List<Guid> playerPool;
		List<Guid> monsterPool;
		List<Guid> deadedPool;
		List<Guid> SceneObjectPool;
		List<DropContent> dropList;		

		Map<UInt32,Guid> triggers;//存放当前场景的事件触发
		Map<UInt32,Guid> bgActorLayers;
		Map<UInt32,Guid> sceneAi;

		SceneCharacter sceneCharacter;
	};

	struct SceneData
	{
		UInt32 coin;
		Pos pos;

	};

	struct NameVal
	{
		UInt32 name;
		UInt32 value;
	};

	class SceneManager
	{

	public:
		void addPlayer(Guid playerID,UInt32 sceneID);
		void update();
		
		void removeScenePlayer(UInt32 playerID);

		void RemoveDrop(UInt32 drop_id );

		void AddDrop(String dropType,UInt32 value,Pos pos,bool bFly);

		void addMonsterDrop(SceneData sceneData);

		void addHit();
		void updateHit();

		void unlockScene();

		void setChapterLabel(UInt32 x);

		void changeAttackBtnScale();

		void leftMoveMapLayer(UInt32 playerID,UInt32 speed) ;

		void rightMoveMapLayer(UInt32 playerID,UInt32 speed) ;

		UInt32 getScreenEdgePosX(bool bRight);

		void onEvent(PVPTreadReq& msg);

		List<Guid> findAttackTargets( Guid playerID,bool isPlayer,UInt32 skillId,UInt32 Multiidx);

		void switchJoystickSkillContent();


		Character *  getPlayerFromSceneByUUID(Guid playerID);

		Character *  getMonsterFromSceneByUUID(Guid playerID);

		void lockScene(UInt32 x1,UInt32 x2);

		void  playAnimation(UInt32 flag);

		void senderSceneAll(UInt16 packetID,std::string jsonStr);

		UInt32 mSceneID;

		UInt32 mRoomID;

		UInt64 lastUpdateTime;

		SceneTempelete *m_SceneTempelete;

		stack<Character*> CharacterPool;
	};


	// 	self.MsgQueue = Queue.new()
	// 		self.triggers = {} --存放当前场景的事件触发

	// 		self.bgActorLayers={}

	// 	self.DropIdBase = 1
	// 		self.sceneAi = {}
	// 	self.parent = parent

	// 		self.updateHandler = nil
	// 		self.recvAwardList = {}
	// 		self.activeNode = nil

	// self:init(sceneId, sceneData)
	// 	 end

}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_BOX_MANAGER_H__
