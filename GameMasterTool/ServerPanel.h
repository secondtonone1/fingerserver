#ifndef __LYNX_GAME_MASTER_TOOL_SERVER_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_SERVER_PANEL_H__

#include "WXLib.h"
#include "CommonLib.h"
#include "EventDefinition.h"

namespace Lynx
{
    class ServerPanel : public wxTreeCtrl
    {
        DECLARE_CLASS(ServerPanel)
    public:
        ServerPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
        virtual ~ServerPanel();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();

    private:
        void buildServerTree();
        void onTreeItemActivated(wxTreeEvent& ev);
        void onTreeItemRClicked(wxTreeEvent& ev);
        void onLoginAll(wxCommandEvent& ev);
        void onLogoutAll(wxCommandEvent& ev);
        void onLogin(wxCommandEvent& ev);
        void onLogout(wxCommandEvent& ev);

    private:
        void onUpdateServerPanelEvent(const UpdateServerPanelEvent& ev);

    private:
        DECLARE_EVENT_TABLE()
        wxTreeItemId mServerTreeItemId;
        wxTreeItemId mActiveTreeItemId;
        wxMenu* mServerRootMenu;
        wxMenu* mServerMenu;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_SERVER_PANEL_H__


