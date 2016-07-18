#include "GuildInfoDialog.h"
#include "GuildInfoNotebook.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(GuildInfoDialog, wxDialog)

BEGIN_EVENT_TABLE(GuildInfoDialog, wxDialog)

END_EVENT_TABLE()

GuildInfoDialog::GuildInfoDialog(wxWindow* parent, wxWindowID id, const wxString& title,
                                   const wxPoint& pos, const wxSize& size, long style)
                                   : wxDialog(parent, id, title, pos, size, style)
{
    mGuildInfoNotebook = NULL;
}

GuildInfoDialog::~GuildInfoDialog()
{
    release();
}

bool 
GuildInfoDialog::initial()
{
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* subSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(subSizer, wxSizerFlags(1).Expand().Border(wxALL, 2));
    SetSizer(topSizer);

    mGuildInfoNotebook = new GuildInfoNotebook(this);
    subSizer->Add(mGuildInfoNotebook, wxSizerFlags(1).Expand());

    registerEvents();

    return mGuildInfoNotebook->initial();
}

void 
GuildInfoDialog::release()
{
    if (mGuildInfoNotebook)
    {
        mGuildInfoNotebook->release();
        delete mGuildInfoNotebook;
    }

    deregisterEvents();
}

void
GuildInfoDialog::resetLayout()
{
    SetSize(wxSize(1000, 700));

    if (mGuildInfoNotebook)
    {
        mGuildInfoNotebook->resetLayout();
    }
}

GuildInfoNotebook& 
GuildInfoDialog::getGuildInfoNotebook()
{
    return *mGuildInfoNotebook;
}

void 
GuildInfoDialog::registerEvents()
{

}

void 
GuildInfoDialog::deregisterEvents()
{

}
