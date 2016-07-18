#ifndef __LYNX_GAME_MASTER_TOOL_LOGIN_PANEL__
#define __LYNX_GAME_MASTER_TOOL_LOGIN_PANEL__

#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class LoginPanel : public wxPanel
    {
        DECLARE_CLASS(LoginPanel)
    public:
        LoginPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
        virtual ~LoginPanel();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();

    private:
        void onLoginBtnClicked(wxCommandEvent& ev);
        void onQuitBtnClicked(wxCommandEvent& ev);

    private:
        wxTextCtrl* mAccountCtrl;
        wxTextCtrl* mPasswordCtrl;
        wxButton* mLoginBtn;
        wxButton* mQuitBtn;
    private:
        DECLARE_EVENT_TABLE();  
    };
}   // namespace KGame

#endif  // __LYNX_GAME_MASTER_TOOL_LOGIN_PANEL__