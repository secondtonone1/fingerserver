#include "GuildInfoNotebook.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(GuildInfoNotebook, wxAuiNotebook)

BEGIN_EVENT_TABLE(GuildInfoNotebook, wxAuiNotebook)
END_EVENT_TABLE()

GuildInfoNotebook::GuildInfoNotebook(wxWindow *parent, wxWindowID id, const wxPoint& pos,
                     const wxSize& size, long style)
                     : wxAuiNotebook(parent, id, pos, size, style)
{
    mGuildInfoBasePanel = NULL;
    mGuildInfoMemberPanel = NULL;
}

GuildInfoNotebook::~GuildInfoNotebook()
{
    release();
}

bool
GuildInfoNotebook::initial()
{
    mGuildInfoBasePanel = new GuildInfoBasePanel(this, myID_GUILD_INFO_BASE_PANEL);
    assert(mGuildInfoBasePanel);
    if (!mGuildInfoBasePanel->initial())
    {
        LOG_WARN("Failed to create guild info base panel.");
        return false;
    }
    AddPage(mGuildInfoBasePanel, wxT("基础属性"));

    mGuildInfoMemberPanel = new GuildInfoMemberPanel(this, myID_GUILD_INFO_MEMBER_PANEL);
    assert(mGuildInfoMemberPanel);
    if (!mGuildInfoMemberPanel->initial())
    {
        LOG_WARN("Failed to create guild info member panel.");
        return false;
    }
    AddPage(mGuildInfoMemberPanel, wxT("成员列表"));

    resetLayout();
    registerEvents();
    return true;
}

void 
GuildInfoNotebook::release()
{
    deregisterEvents();
}


void 
GuildInfoNotebook::resetLayout()
{
   SetSelection(0);

   if (mGuildInfoBasePanel)
   {
       mGuildInfoBasePanel->resetLayout();
   }

   if (mGuildInfoMemberPanel)
   {
       mGuildInfoMemberPanel->resetLayout();
   }
}

int 
GuildInfoNotebook::getPageIndex(wxWindow* win)
{
    if (!win)
    {
        return wxNOT_FOUND;
    }

    for (int index = 0; index < GetPageCount(); ++index)
    {
        if (win == GetPage(index))
        {
            return index;
        }
    }
    return wxNOT_FOUND;
}

void 
GuildInfoNotebook::registerEvents()
{

}

void 
GuildInfoNotebook::deregisterEvents()
{

}

