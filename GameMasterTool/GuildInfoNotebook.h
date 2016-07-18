#ifndef __LYNX_GAME_MASTER_TOOL_GUILD_INFO_NOTEBOOK_H__
#define __LYNX_GAME_MASTER_TOOL_GUILD_INFO_NOTEBOOK_H__

#include "GuildInfoBasePanel.h"
#include "GuildInfoMemberPanel.h"

namespace Lynx
{
    class GuildInfoNotebook : public wxAuiNotebook
    {
        DECLARE_CLASS(GuildInfoNotebook);
    public:
        GuildInfoNotebook(wxWindow *parent = NULL, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = 0);
        virtual ~GuildInfoNotebook();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();
        int getPageIndex(wxWindow* win);

    private:
        DECLARE_EVENT_TABLE()
        GuildInfoBasePanel* mGuildInfoBasePanel;
        GuildInfoMemberPanel* mGuildInfoMemberPanel;
    };
} // namespace KGame

#endif  // __LYNX_GAME_MASTER_TOOL_GUILD_INFO_NOTEBOOK_H__