#include "DebugMsgHandler.h"
#include "GameServer.h"

using namespace Lynx;
#ifdef DEBUG
void
DebugMsgHandler::onDebugRemoteLogNotify(const ConnId& connId, DebugRemoteLogNotify& msg)
{
    LOG_DEBUG(msg.mLogString.c_str());
}
#endif
