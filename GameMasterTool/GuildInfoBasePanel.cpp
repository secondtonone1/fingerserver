#include "GuildInfoBasePanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(GuildInfoBasePanel, wxPanel)

BEGIN_EVENT_TABLE(GuildInfoBasePanel, wxPanel)

END_EVENT_TABLE()

GuildInfoBasePanel::GuildInfoBasePanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos,
                         const wxSize& size, long style, const wxString& name)
                         : wxPanel(parent, winid, pos, size, style, name)
{

}

GuildInfoBasePanel::~GuildInfoBasePanel()
{

}

bool
GuildInfoBasePanel::initial()
{
    resetLayout();
    registerEvents();
    return true;
}

void 
GuildInfoBasePanel::release()
{
    deregisterEvents();
}

void
GuildInfoBasePanel::registerEvents()
{

}

void
GuildInfoBasePanel::deregisterEvents()
{

}

void
GuildInfoBasePanel::resetLayout()
{

}
