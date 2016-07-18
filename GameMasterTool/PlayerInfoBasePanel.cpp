#include "PlayerInfoBasePanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(PlayerInfoBasePanel, wxPanel)

BEGIN_EVENT_TABLE(PlayerInfoBasePanel, wxPanel)

END_EVENT_TABLE()

PlayerInfoBasePanel::PlayerInfoBasePanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos,
                         const wxSize& size, long style, const wxString& name)
                         : wxPanel(parent, winid, pos, size, style, name)
{
    mPropertyGrid = NULL;
    mPropertyTopSizer = NULL;
}

PlayerInfoBasePanel::~PlayerInfoBasePanel()
{

}

bool
PlayerInfoBasePanel::initial()
{
    mPropertyTopSizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(mPropertyTopSizer);

    mPropertyGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
        wxPG_BOLD_MODIFIED | wxPG_TOOLBAR | wxPG_SPLITTER_AUTO_CENTER | wxPG_STATIC_SPLITTER);
    mPropertyGrid->SetVerticalSpacing(2);

    mPropertyTopSizer->Add(mPropertyGrid, 1, wxEXPAND);
    mPropertyTopSizer->SetSizeHints(mPropertyGrid);

    resetLayout();
    registerEvents();
    return true;
}

void 
PlayerInfoBasePanel::release()
{
    deregisterEvents();
}

void
PlayerInfoBasePanel::registerEvents()
{

}

void
PlayerInfoBasePanel::deregisterEvents()
{

}

void
PlayerInfoBasePanel::resetLayout()
{
    if (!mPropertyGrid)
    {
        return;
    }

    mPropertyGrid->Clear();

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    const PlayerData& playerData = gmClient->getPlayerData();

    wxPGProperty* p = NULL;
    char buf[256] = { 0 };
    sprintf(buf, "%llu", playerData.mBaseData.mPlayerId);
    p = mPropertyGrid->Append(new wxStringProperty("ID", wxPG_LABEL, buf));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxStringProperty("名称", wxPG_LABEL, playerData.mBaseData.mPlayerName.c_str()));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("等级", wxPG_LABEL, playerData.mBaseData.mLevel));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("经验", wxPG_LABEL, playerData.mBaseData.mLevelExp));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("体力", wxPG_LABEL, playerData.mBaseData.mEnergy));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("战力", wxPG_LABEL, playerData.mBaseData.mBattleValue));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("金币", wxPG_LABEL, playerData.mBaseData.mGold));
    mPropertyGrid->SetPropertyReadOnly(p);
}
