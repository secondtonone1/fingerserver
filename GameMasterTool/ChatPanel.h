#ifndef __LYNX_GAME_MASTER_TOOL_CHAT_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_CHAT_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class ChatPanel : public wxTextCtrl
    {
        DECLARE_CLASS(ChatPanel)
    public:
        ChatPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
        virtual ~ChatPanel();

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

        void onSystemNoticeNotifyEvent(const SystemNoticeNotifyEvent& ev);


    private:
        DECLARE_EVENT_TABLE()
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_CHAT_PANEL_H__


