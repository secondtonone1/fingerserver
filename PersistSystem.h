#ifndef __LYNX_GAME_SERVER_PERSIST_SYSTEM_H__
#define __LYNX_GAME_SERVER_PERSIST_SYSTEM_H__

#include "MainThread.h"
#include "PersistWorker.h"

#define PERSIST_THREAD_WORKER_COUNT 4

namespace Lynx
{
	//存盘状态信息
	struct PersistInfo
	{
		PersistInfo():mPlayerUid(0),mDirtyBit(0),mProcessState(0),mCheckTimes(0){}
		UInt64 mPlayerUid;
		UInt32 mDirtyBit;
		UInt32 mProcessState;
		UInt32 mCheckTimes;

		
		LYNX_S11N_4(PersistInfo, UInt64, mPlayerUid, UInt32, mDirtyBit,
			UInt32, mProcessState,UInt32, mCheckTimes);
	};
	
	
	class PersistSystem : public MainThread, public Singleton<PersistSystem>
	{
	public:
		PersistSystem();
		virtual ~PersistSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);
		
		template <typename MsgType >
		void postThreadMsg(const MsgType& msg, UInt16 channel)
		{
			UInt32 index = channel % PERSIST_THREAD_WORKER_COUNT;
			PersistWorker& worker = mWorkerArray[index];
			if (!worker.isShutdown())
			{
				ThreadMsg* threadMsg = allocThreadMsg(MessageId<MsgType >::sMsgId, channel);
				serialize(msg, threadMsg->mBody, 0);
				worker.postMsgToInputQueue(threadMsg);
			}
		}

		void SaveAllDirty();

    private:
        bool loadDatabaseData();
		 
		bool loadSqlTime(DBInterface& dbInterface);
        bool loadGuidData(DBInterface& dbInterface);
	
        bool loadGuildData(DBInterface& dbInterface);
        bool loadEmailData(DBInterface& dbInterface);
        bool loadPlayerInfo(DBInterface& dbInterface);
		

		//检测map中的dirty状态，然后通知worker进行存盘操作
		void checkDirtyToSave();


	private:
		void dispatchThreadMsg(ThreadMsg& msg);

        void onPersistLoadPlayerDataResp(PersistLoadPlayerDataResp& msg);

		void onPersistFindSimilarPowerResp(PersistFindSimilarPowerResp& msg);
       
		void onPersistSetDirtyResp(PersistManagerSetDirtyResp & msg);

		void onPersistNotifyWorkerSaveDbResp(PersistNotifyWorkerSaveDbResp & msg);

		void onPersistCreatePlayerResp(PersistCreatePlayerResp & msg);

		void onPersistLoadOfflineDataResp(PersistLoadOfflineDataResp& msg);

		void onPersistLoadPlayerByAccountResp(PersistLoadPlayerByAccountResp &msg);

		void onPersistInsertRobotResp(PersistInsertRobotResp & msg);

	private:
		ThreadMsgHandler mThreadMsgHandler;
		PersistWorker mWorkerArray[PERSIST_THREAD_WORKER_COUNT];
	
		//玩家uid到persistinfo的map
		Map<UInt64,PersistInfo> m_mapUid2PersistInfo;

		UInt64 m_nLastSyncTime;

	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PERSIST_SYSTEM_H__

