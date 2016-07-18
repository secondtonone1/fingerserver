#include "PlayerInfoNotebook.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(PlayerInfoNotebook, wxAuiNotebook)

BEGIN_EVENT_TABLE(PlayerInfoNotebook, wxAuiNotebook)
END_EVENT_TABLE()

PlayerInfoNotebook::PlayerInfoNotebook(wxWindow *parent, wxWindowID id, const wxPoint& pos,
                     const wxSize& size, long style)
                     : wxAuiNotebook(parent, id, pos, size, style)
{
    mPlayerInfoBagPanel = NULL;
    mPlayerInfoBasePanel = NULL;
    mPlayerInfoHeroPanel = NULL;
    mPlayerInfoPetPanel = NULL;
}

PlayerInfoNotebook::~PlayerInfoNotebook()
{
    release();
}

bool
PlayerInfoNotebook::initial()
{
    wxBitmap pageBmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));

    mPlayerInfoBasePanel = new PlayerInfoBasePanel(this, myID_PLAYER_INFO_BASE_PANEL);
    assert(mPlayerInfoBasePanel);
    if (!mPlayerInfoBasePanel->initial())
    {
        LOG_WARN("Failed to create player info base panel.");
        return false;
    }
    AddPage(mPlayerInfoBasePanel, wxT("基础属性"), true, pageBmp);

    mPlayerInfoBagPanel = new PlayerInfoBagPanel(this, myID_PLAYER_INFO_BAG_PANEL);
    assert(mPlayerInfoBagPanel);
    if (!mPlayerInfoBagPanel->initial())
    {
        LOG_WARN("Failed to create player info bag panel.");
        return false;
    }
    AddPage(mPlayerInfoBagPanel, wxT("背包信息"), false, pageBmp);

    mPlayerInfoHeroPanel = new PlayerInfoHeroPanel(this, myID_PLAYER_INFO_HERO_PANEL);
    assert(mPlayerInfoHeroPanel);
    if (!mPlayerInfoHeroPanel->initial())
    {
        LOG_WARN("Failed to create player info hero panel.");
        return false;
    }
    AddPage(mPlayerInfoHeroPanel, wxT("英雄信息"), false, pageBmp);

    mPlayerInfoPetPanel = new PlayerInfoPetPanel(this, myID_PLAYER_INFO_PET_PANEL);
    assert(mPlayerInfoPetPanel);
    if (!mPlayerInfoPetPanel->initial())
    {
        LOG_WARN("Failed to create player info pet panel.");
        return false;
    }
    AddPage(mPlayerInfoPetPanel, wxT("神兽信息"), false, pageBmp);

    resetLayout();
    registerEvents();
    return true;
}

void 
PlayerInfoNotebook::release()
{
    deregisterEvents();
}


void 
PlayerInfoNotebook::resetLayout()
{
   if (mPlayerInfoBasePanel)
   {
       mPlayerInfoBasePanel->resetLayout();
   }

   if (mPlayerInfoBagPanel)
   {
       mPlayerInfoBagPanel->resetLayout();
   }

   if (mPlayerInfoHeroPanel)
   {
       mPlayerInfoHeroPanel->resetLayout();
   }

   if (mPlayerInfoPetPanel)
   {
       mPlayerInfoPetPanel->resetLayout();
   }

   SetSelection(GetPageIndex(mPlayerInfoBasePanel));
}

int 
PlayerInfoNotebook::getPageIndex(wxWindow* win)
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
PlayerInfoNotebook::registerEvents()
{

}

void 
PlayerInfoNotebook::deregisterEvents()
{

}

