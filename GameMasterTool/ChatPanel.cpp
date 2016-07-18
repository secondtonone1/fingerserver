#include "ChatPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(ChatPanel, wxTextCtrl)

BEGIN_EVENT_TABLE(ChatPanel, wxTextCtrl)
END_EVENT_TABLE()

ChatPanel::ChatPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxTextCtrl(parent, id, wxEmptyString, pos, size, style)
{
    
}

ChatPanel::~ChatPanel()
{
    release();
}

bool
ChatPanel::initial()
{
    //SetBackgroundColour(*wxBLACK);
    //SetForegroundColour(*wxWHITE);

    registerEvents();
    return true;
}

void 
ChatPanel::release()
{
    deregisterEvents();
}

void
ChatPanel::registerEvents()
{
    REGISTER_EVENT(SystemNoticeNotifyEvent, this, &ChatPanel::onSystemNoticeNotifyEvent);
}

void
ChatPanel::deregisterEvents()
{
    DEREGISTER_EVENT(SystemNoticeNotifyEvent, this, &ChatPanel::onSystemNoticeNotifyEvent);
}

void
ChatPanel::resetLayout()
{
    Clear();
}

void 
ChatPanel::onSystemNoticeNotifyEvent(const SystemNoticeNotifyEvent& ev)
{
    //wxTextAttr oldStyle = GetDefaultStyle();
    //SetDefalutStyle(style);

    AppendText(wxString(ev.mContents.c_str()));
    AppendText(wxString("\r\n"));
    //SetDefaultStyle(oldStyle);
}

