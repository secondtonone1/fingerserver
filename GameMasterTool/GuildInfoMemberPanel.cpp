#include "GuildInfoMemberPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(GuildInfoMemberPanel, wxPanel)

BEGIN_EVENT_TABLE(GuildInfoMemberPanel, wxPanel)

END_EVENT_TABLE()

GuildInfoMemberPanel::GuildInfoMemberPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos,
                         const wxSize& size, long style, const wxString& name)
                         : wxPanel(parent, winid, pos, size, style, name)
{

}

GuildInfoMemberPanel::~GuildInfoMemberPanel()
{

}

bool
GuildInfoMemberPanel::initial()
{
    resetLayout();
    registerEvents();
    return true;
}

void 
GuildInfoMemberPanel::release()
{
    deregisterEvents();
}

void
GuildInfoMemberPanel::registerEvents()
{

}

void
GuildInfoMemberPanel::deregisterEvents()
{

}

void
GuildInfoMemberPanel::resetLayout()
{

}
