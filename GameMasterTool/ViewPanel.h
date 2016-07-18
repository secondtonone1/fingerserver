#ifndef __LYNX_GAME_MASTER_VIEW_PANEL_H__
#define __LYNX_GAME_MASTER_VIEW_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"
#include "ChatPanel.h"
#include "PlayerPanel.h"
#include "GuildPanel.h"
#include "EmailPanel.h"
#include "MallPanel.h"
#include "NoticePanel.h"
#include "ForbiddenPanel.h"

namespace Lynx
{
    class ViewPanel : public wxAuiNotebook
    {
        DECLARE_CLASS(ViewPanel)
    public:
        ViewPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
        virtual ~ViewPanel();

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
        void onNotebookChanged(wxAuiNotebookEvent& ev);

    private:
        DECLARE_EVENT_TABLE()
        ChatPanel* mChatPanel;
        PlayerPanel* mPlayerPanel;
        GuildPanel* mGuildPanel;
        EmailPanel* mEmailPanel;
        MallPanel* mMallPanel;
        NoticePanel* mNoticePanel;
        ForbiddenPanel* mForbiddenPanel;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_VIEW_PANEL_H__


