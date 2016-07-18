#ifndef __LYNX_GAME_MASTER_TOOL_MAIN_FRAME_H__
#define __LYNX_GAME_MASTER_TOOL_MAIN_FRAME_H__

#include "WXLib.h"
#include "CommonLib.h"
#include "EventDefinition.h"
#include "PlayerInfoDialog.h"
#include "GuildInfoDialog.h"
#include "ViewPanel.h"
#include "OutputPanel.h"
#include "CommandPanel.h"
#include "ServerPanel.h"
#include "LoginPanel.h"

namespace Lynx
{
    class MainFrame : public wxFrame
    {
        DECLARE_CLASS(MainFrame)
    public:
        MainFrame(wxFrame* parent, wxWindowID id, const wxString& title,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr);
        virtual ~MainFrame();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();

        void showLoginWindow();
        void showMainFrameWindow();

        PlayerInfoDialog& getPlayerInfoDialog();
        GuildInfoDialog& getGuildInfoDialog();

    private:
        void onServerSelectEvent(const ServerSelectEvent& ev);
        void onLoginSuccessEvent(const LoginSuccessEvent& ev);
        void onLoginBrokenEvent(const LoginBrokenEvent& ev);

    private:
        bool createLoginPanel();
        bool createMainFramePanels();
        void cleanAllPanels();

    private:
        DECLARE_EVENT_TABLE()
        wxAuiManager mAuiManager;
        PlayerInfoDialog* mPlayerInfoDialog;
        GuildInfoDialog* mGuildInfoDialog;
        LoginPanel* mLoginPanel;
        ServerPanel* mServerPanel;
        ViewPanel* mViewPanel;
        CommandPanel* mCommandPanel;
        OutputPanel* mOutputPanel;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_MAIN_FRAME_H__

