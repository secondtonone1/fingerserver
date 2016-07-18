#ifndef __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_BASE_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_BASE_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class PlayerInfoBasePanel : public wxPanel
    {
        DECLARE_CLASS(PlayerInfoBasePanel)
    public:
        PlayerInfoBasePanel(wxWindow *parent = NULL, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
        virtual ~PlayerInfoBasePanel();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

    private:
        DECLARE_EVENT_TABLE()
        wxPropertyGrid* mPropertyGrid;
        wxBoxSizer* mPropertyTopSizer;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_BASE_PANEL_H__