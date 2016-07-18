#ifndef __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_DIALOG_H__
#define __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_DIALOG_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class PlayerInfoNotebook;
    class PlayerInfoDialog : public wxDialog
    {
        DECLARE_CLASS(PlayerInfoDialog)
    public:
        PlayerInfoDialog(wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxT("Íæ¼ÒÐÅÏ¢"),
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(1024, 768),
            long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX);
        virtual ~PlayerInfoDialog();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

        PlayerInfoNotebook& getPlayerInfoNotebook();

    private:
        DECLARE_EVENT_TABLE()
        PlayerInfoNotebook* mPlayerInfoNotebook;
    };

}  // namespace Lynx

#endif  // __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_DIALOG_H__