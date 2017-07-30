#ifndef __LYNX_GAME_SERVER_PVP_SYSTEM_H__
#define __LYNX_GAME_SERVER_PVP_SYSTEM_H__

#include "../MainThread.h"
#include "PVPWorker.h"
#include "Scene.h"
#include "../GameServer.h"
	
#define PVP_THREAD_WORKER_COUNT 4//PVP战斗线程数

#define PVP_SCENE_MAX_COUNT 2000//PVP战斗每个线程关卡数

#define PVP_SCENE_CHARACTER_NUM 2//每个关卡总人数


namespace Lynx
{

	struct Robot
	{
		Guid playerID;
		std::string name;
		UInt32 level;
		UInt32 vipLevel;
		std::string title;
		UInt32 camp;
		UInt32 state;
		UInt32 modID;
		UInt32 power;
		UInt32 delay;
		UInt32 score;
		List<Goods> equipList;

	};

	class PVPSystem : public MainThread, public Singleton<PVPSystem>
	{
	public:
		PVPSystem();
		virtual ~PVPSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);

		SceneManager* initScene(UInt32 stageID);

		void removeScene(UInt32 sceneID);

		void getRoomList(const  ConnId& ,PVPRoomReq& );

		void createRoom(const  ConnId& ,PVPRoomReq& );
	
		 void joinRoom(const  ConnId& connId,PVPRoomReq& msg);

		 void leaveRoom(const  ConnId& connId,PVPRoomReq& msg);
	
		 void PVPStart(const  ConnId& connId,PVPRoomReq& msg);

		 void matchingRoom(const  ConnId& connId,PVPRoomReq& msg);

		 void matchingRoomResp(Guid playerID,Guid otherPlayerID);

		 Robot getSimilarPlayer(Guid playerID,UInt32 typeID);//匹配战力相近玩家

		static void onPVPRoomReq(const ConnId& connId,PVPRoomReq& msg);

		static void onPVPBattleReq(const ConnId& connId,PVPBattleReq& msg);

		void PlayerAction(PVPTreadReq& msg);

		UInt32 getRoomMaxNum();

		void removeRoom(UInt32 roomID);

		SceneManager* getScene(UInt32 id);

		void setScene(SceneManager* val);		

		template <typename MsgType >
		void postThreadMsg(const MsgType& msg, UInt16 channel)
		{
			UInt32 index = channel % PVP_THREAD_WORKER_COUNT;
			PVPWorker& worker = mPVPWorkerArray[index];
			if (!worker.isShutdown())
			{
				ThreadMsg* threadMsg = allocThreadMsg(MessageId<MsgType >::sMsgId, index);
				serialize(msg, threadMsg->mBody, 0);
				worker.postMsgToInputQueue(threadMsg);
			}
		}

	private:
		void dispatchThreadMsg(ThreadMsg& msg);
		
	private:
		ThreadMsgHandler mThreadMsgHandler;
		PVPWorker mPVPWorkerArray[PVP_THREAD_WORKER_COUNT];
	public:
// 		Map<UInt32,SceneTempelete*> SceneMap;


	};

	

	static Map<UInt32,SceneManager*> mSceneMap;

	static stack<SceneManager*> mScenePool;

	static List<RoomData> mRoomData ;

	static UInt32 mRoomID = 0;

	static List<Robot> mFriendData ;//临时用数据 好友数据

	static List<Robot> mSimilarPlayerData ;//临时用数据 好友数据

} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PVP_SYSTEM_H__

