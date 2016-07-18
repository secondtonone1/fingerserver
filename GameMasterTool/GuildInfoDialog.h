#ifndef __LYNX_GAME_MASTER_TOOL_GUILD_INFO_DIALOG_H__
#define __LYNX_GAME_MASTER_TOOL_GUILD_INFO_DIALOG_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class GuildInfoNotebook;
    class GuildInfoDialog : public wxDialog
    {
        DECLARE_CLASS(GuildInfoDialog)
    public:
        GuildInfoDialog(wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxT("Íæ¼ÒÐÅÏ¢"),
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(1024, 768),
            long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX);
        virtual ~GuildInfoDialog();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

        GuildInfoNotebook& getGuildInfoNotebook();

    private:
        DECLARE_EVENT_TABLE()
        GuildInfoNotebook* mGuildInfoNotebook;
    };

}  // namespace Lynx

#endif  // __LYNX_GAME_MASTER_TOOL_GUILD_INFO_DIALOG_H__