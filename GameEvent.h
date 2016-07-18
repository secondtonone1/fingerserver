#ifndef __LYNX_GAME_SERVER_GAME_EVENT_H__
#define __LYNX_GAME_SERVER_GAME_EVENT_H__

#include "CommonLib.h"

namespace Lynx
{
	enum EventId
	{
		EVENT_SERVER_STARTUP,                     // 服务器启动事件
		EVENT_SERVER_SHUTDOWN,                    // 服务器关闭事件
		EVENT_CLIENT_CONNECTION_NOT_EXIST,        // 客户端连接对象不存在
		EVENT_CLIENT_CONNECTION_CLOSE,            // 逻辑层关闭客户端连接事件
	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_GAME_EVENT_H__

