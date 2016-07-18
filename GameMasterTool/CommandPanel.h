#ifndef __LYNX_GAME_MASTER_TOOL_COMMAND_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_COMMAND_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class CommandPanel : public wxPanel
    {
        DECLARE_CLASS(CommandPanel)
    public:
        CommandPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
        virtual ~CommandPanel();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

    private:
        void onServerSelectEvent(const ServerSelectEvent& ev);
        void onLoginSuccessEvent(const LoginSuccessEvent& ev);
        void onLoginBrokenEvent(const LoginBrokenEvent& ev);

    private:
        void onSendGameCommand();
        void onEnterButtonClicked(wxCommandEvent& ev);
        void onInputTextCtrlEntered(wxCommandEvent& ev);
        void onKeyUpEvent(wxKeyEvent& ev);
        void onKeyDownEvent(wxKeyEvent& ev);

    private:
        DECLARE_EVENT_TABLE()
        wxTextCtrl* mInputTextCtrl;
        wxButton* mEnterButton;
        List<String> mHistoryCmdList;
        UInt32 mIndex;
    };
} // namespace KGame

#endif // __LYNX_GAME_MASTER_TOOL_COMMAND_PANEL_H__