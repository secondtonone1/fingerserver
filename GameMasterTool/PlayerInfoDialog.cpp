#include "PlayerInfoDialog.h"
#include "PlayerInfoNotebook.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(PlayerInfoDialog, wxDialog)

BEGIN_EVENT_TABLE(PlayerInfoDialog, wxDialog)

END_EVENT_TABLE()

PlayerInfoDialog::PlayerInfoDialog(wxWindow* parent, wxWindowID id, const wxString& title,
                                   const wxPoint& pos, const wxSize& size, long style)
                                   : wxDialog(parent, id, title, pos, size, style)
{
    mPlayerInfoNotebook = NULL;
}

PlayerInfoDialog::~PlayerInfoDialog()
{
    release();
}

bool 
PlayerInfoDialog::initial()
{
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* subSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(subSizer, wxSizerFlags(1).Expand().Border(wxALL, 2));
    SetSizer(topSizer);

    mPlayerInfoNotebook = new PlayerInfoNotebook(this);
    subSizer->Add(mPlayerInfoNotebook, wxSizerFlags(1).Expand());

    registerEvents();

    return mPlayerInfoNotebook->initial();
}

void 
PlayerInfoDialog::release()
{
    if (mPlayerInfoNotebook)
    {
        mPlayerInfoNotebook->release();
        delete mPlayerInfoNotebook;
    }

    deregisterEvents();
}

void
PlayerInfoDialog::resetLayout()
{
    SetSize(wxSize(1000, 700));

    if (mPlayerInfoNotebook)
    {
        mPlayerInfoNotebook->resetLayout();
    }
}

PlayerInfoNotebook& 
PlayerInfoDialog::getPlayerInfoNotebook()
{
    return *mPlayerInfoNotebook;
}

void 
PlayerInfoDialog::registerEvents()
{

}

void 
PlayerInfoDialog::deregisterEvents()
{

}
