#ifndef __LYNX_SERVER_DEBUG_TOOL_MSG_DEFINE_H__
#define __LYNX_SERVER_DEBUG_TOOL_MSG_DEFINE_H__

#include "PlatformLib.h"
#include "NetworkLib.h"

namespace Lynx
{
    enum
    {
        START_SERVER_NOTIFY           = 1,
        REBUILD_MAIN_PANEL_NOTIFY,
    };

    struct ReBuildMainPanelNotify
    {
        ReBuildMainPanelNotify() {}
        LYNX_MESSAGE_0(REBUILD_MAIN_PANEL_NOTIFY, ReBuildMainPanelNotify);
    };

} // namespace Lynx


#endif // __LYNX_SERVER_DEBUG_TOOL_MSG_DEFINE_H__