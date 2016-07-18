#ifndef __LYNX_GAME_SERVER_PERSIST_EXT_WORKER_H__
#define __LYNX_GAME_SERVER_PERSIST_EXT_WORKER_H__

#include "CommonLib.h"
#include "PersistThreadMsg.h"
#include "DBInterface.h"

namespace Lynx
{

	class PersistWorkerExt : public ThreadWorker
	{
	public:
		PersistWorkerExt();
		virtual ~PersistWorkerExt();



	private:
// 		ThreadMsgHandler mThreadMsgHandler;
		DBInterface mDBInterface;
	};

     
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PERSIST_EXT_WORKER_H__

