#include "PlayerInfoBagPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

#define STATIC_BOX_ITEM_LABEL(currCount, maxCount) (wxString::Format("物品数量(%u/%u)", currCount, maxCount))

IMPLEMENT_CLASS(PlayerInfoBagPanel, wxPanel)

BEGIN_EVENT_TABLE(PlayerInfoBagPanel, wxPanel)
EVT_TREE_ITEM_ACTIVATED(myID_PLAYER_INFO_BAG_PANEL_TREE, onTreeItemActivated)
EVT_MENU(myID_PLAYER_INFO_BAG_PANEL_MENU, onCopyId)
EVT_LIST_ITEM_RIGHT_CLICK(myID_PLAYER_INFO_BAG_PANEL_LIST, onRightClicked)
END_EVENT_TABLE()

PlayerInfoBagPanel::PlayerInfoBagPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos,
                         const wxSize& size, long style, const wxString& name)
                         : wxPanel(parent, winid, pos, size, style, name)
{
    mTreeCtrl = NULL;
    mListCtrl = NULL;
    mTreeBox = NULL;
    mListBox = NULL;
    mOperateMenu = NULL;
}

PlayerInfoBagPanel::~PlayerInfoBagPanel()
{

}

bool
PlayerInfoBagPanel::initial()
{
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(topSizer);

    mTreeBox = new wxStaticBox(this, wxID_ANY, wxT("物品类型"));
    wxStaticBoxSizer* treeBoxSizer = new wxStaticBoxSizer(mTreeBox, wxHORIZONTAL);
    topSizer->Add(treeBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);
    mTreeCtrl = new wxTreeCtrl(this, myID_PLAYER_INFO_BAG_PANEL_TREE, wxDefaultPosition, wxDefaultSize);
    treeBoxSizer->Add(mTreeCtrl, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);
    
    mListBox = new wxStaticBox(this, wxID_ANY, STATIC_BOX_ITEM_LABEL(0,0));
    wxStaticBoxSizer* listBoxSizer = new wxStaticBoxSizer(mListBox, wxHORIZONTAL);
    topSizer->Add(listBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);
    mListCtrl = new wxListCtrl(this, myID_PLAYER_INFO_BAG_PANEL_LIST, 
        wxDefaultPosition, wxSize(800, 800), wxLC_REPORT | wxLC_SINGLE_SEL);
    listBoxSizer->Add(mListCtrl, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mTreeCtrlRootItem = mTreeCtrl->AddRoot("背包类型", 0);
    mTreeCtrlItemId = mTreeCtrl->AppendItem(mTreeCtrlRootItem, "道具背包", 1, -1);
    mTreeCtrlEquipId = mTreeCtrl->AppendItem(mTreeCtrlRootItem, "装备背包", 1, -1);
    mTreeCtrlRuneId = mTreeCtrl->AppendItem(mTreeCtrlRootItem, "符文背包", 1, -1);
    mTreeCtrlHeroDebrisId = mTreeCtrl->AppendItem(mTreeCtrlRootItem, "英雄碎片", 1, -1);

    mOperateMenu = new wxMenu(wxT(""));
    mOperateMenu->Append(myID_PLAYER_INFO_BAG_PANEL_MENU, wxT("拷贝ID"));

    resetLayout();
    registerEvents();
    return true;
}

void 
PlayerInfoBagPanel::release()
{
    if (mOperateMenu)
    {
        delete (mOperateMenu);
        mOperateMenu = NULL;
    }
    deregisterEvents();
}

void
PlayerInfoBagPanel::registerEvents()
{

}

void
PlayerInfoBagPanel::deregisterEvents()
{

}

void
PlayerInfoBagPanel::resetLayout()
{
    if (mListCtrl)
    {
        mListCtrl->DeleteAllItems();
    }

    if (mTreeCtrl)
    {
        mTreeCtrl->Expand(mTreeCtrlRootItem);
    }
}

void 
PlayerInfoBagPanel::onTreeItemActivated(wxTreeEvent& ev)
{
    wxTreeItemId itemId = ev.GetItem();

    if (!itemId.IsOk())
    {
        return;
    }

    if (itemId == mTreeCtrlItemId)
    {
        showItemList();
    }
    else if (itemId == mTreeCtrlEquipId)
    {
        showEquipList();
    }
    else if (itemId == mTreeCtrlRuneId)
    {
        showRuneList();
    }
    else if (itemId == mTreeCtrlHeroDebrisId)
    {
        showHeroDebrisList();
    }
    else
    {
        if (mListCtrl)
        {
            mListCtrl->DeleteAllItems();
        }
    }
}

void 
PlayerInfoBagPanel::showItemList()
{
    if (mListCtrl)
    {
        mListCtrl->ClearAll();
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }


    for (int i = 0; i < COL_TYPE_ITEM_MAX_COUNT; ++i)
    {
        wxListItem itemCol;
        itemCol.SetText(getItemColTypeText(i));
        itemCol.SetWidth(120);
        mListCtrl->InsertColumn(i, itemCol);
    }

    const PlayerItemData& playerItemData = gmClient->getPlayerData().mItemData;

    for (List<ItemData>::ConstIter* iter = playerItemData.mItemDataList.begin();
        iter != NULL; iter = playerItemData.mItemDataList.next(iter))
    {
        ItemTemplate* itemTemplate = ITEM_TABLE().get(iter->mValue.mTemplateId);
        if (!itemTemplate)
        {
            continue;
        }
        long index = mListCtrl->InsertItem(mListCtrl->GetItemCount(), wxT(""));
        mListCtrl->SetItem(index, COL_TYPE_ITEM_ID, wxString::Format("%llu", iter->mValue.mItemId));
        mListCtrl->SetItem(index, COL_TYPE_ITEM_TEMPLATE_ID, wxString::Format("%u", iter->mValue.mTemplateId));
        mListCtrl->SetItem(index, COL_TYPE_ITEM_NAME, itemTemplate->mName.c_str());
        mListCtrl->SetItem(index, COL_TYPE_ITEM_COUNT, wxString::Format("%u", iter->mValue.mCount));
        mListCtrl->SetItem(index, COL_TYPE_ITEM_DESC, itemTemplate->mDesc.c_str());
    }

    mListBox->SetLabel(STATIC_BOX_ITEM_LABEL(playerItemData.mItemDataList.size(), 100));
}

void 
PlayerInfoBagPanel::showEquipList()
{
    if (mListCtrl)
    {
        mListCtrl->ClearAll();
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    for (int i = 0; i < COL_TYPE_EQUIP_MAX_COUNT; ++i)
    {
        wxListItem itemCol;
        itemCol.SetText(getEquipColTypeText(i));
        itemCol.SetWidth(120);
        mListCtrl->InsertColumn(i, itemCol);
    }

    const PlayerEquipData& playerEquipData = gmClient->getPlayerData().mEquipData;

    for (List<EquipData>::ConstIter* iter = playerEquipData.mEquipDataList.begin();
        iter != NULL; iter = playerEquipData.mEquipDataList.next(iter))
    {
        EquipTemplate* equipTemplate = EQUIP_TABLE().get(iter->mValue.mTemplateId);
        if (!equipTemplate)
        {
            continue;
        }
        long index = mListCtrl->InsertItem(mListCtrl->GetItemCount(), wxT(""));
        mListCtrl->SetItem(index, COL_TYPE_EQUIP_ID, wxString::Format("%llu", iter->mValue.mEquipId));
        mListCtrl->SetItem(index, COL_TYPE_EQUIP_TEMPLATE_ID, wxString::Format("%u", iter->mValue.mTemplateId));
        mListCtrl->SetItem(index, COL_TYPE_EQUIP_NAME, equipTemplate->mName.c_str());
        mListCtrl->SetItem(index, COL_TYPE_EQUIP_DESC, equipTemplate->mDescripe.c_str());
    }

    mListBox->SetLabel(STATIC_BOX_ITEM_LABEL(playerEquipData.mEquipDataList.size(), 100));
}

void 
PlayerInfoBagPanel::showRuneList()
{
    if (mListCtrl)
    {
        mListCtrl->ClearAll();
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    for (int i = 0; i < COL_TYPE_RUNE_MAX_COUNT; ++i)
    {
        wxListItem itemCol;
        itemCol.SetText(getRuneColTypeText(i));
        itemCol.SetWidth(120);
        mListCtrl->InsertColumn(i, itemCol);
    }

    const PlayerRuneData& playerRuneData = gmClient->getPlayerData().mRuneData;

    for (List<RuneData>::ConstIter* iter = playerRuneData.mRuneDataList.begin();
        iter != NULL; iter = playerRuneData.mRuneDataList.next(iter))
    {
        RuneTemplate* runeTemplate = RUNE_TABLE().get(iter->mValue.mTemplateId);
        if (!runeTemplate)
        {
            continue;
        }
        long index = mListCtrl->InsertItem(mListCtrl->GetItemCount(), wxT(""));
        mListCtrl->SetItem(index, COL_TYPE_RUNE_ID, wxString::Format("%llu", iter->mValue.mRuneId));
        mListCtrl->SetItem(index, COL_TYPE_RUNE_TEMPLATE_ID, wxString::Format("%u", iter->mValue.mTemplateId));
        mListCtrl->SetItem(index, COL_TYPE_RUNE_NAME, runeTemplate->mName.c_str());
        mListCtrl->SetItem(index, COL_TYPE_RUNE_DESC, runeTemplate->mDescripe.c_str());
    }

    mListBox->SetLabel(STATIC_BOX_ITEM_LABEL(playerRuneData.mRuneDataList.size(), 100));
}

void 
PlayerInfoBagPanel::showHeroDebrisList()
{
    if (mListCtrl)
    {
        mListCtrl->ClearAll();
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    for (int i = 0; i < COL_TYPE_HERO_DEBRIS_MAX_COUNT; ++i)
    {
        wxListItem itemCol;
        itemCol.SetText(getHeroDebrisColTypeText(i));
        itemCol.SetWidth(120);
        mListCtrl->InsertColumn(i, itemCol);
    }

    const PlayerHeroDebrisData& playerHeroDebrisData = gmClient->getPlayerData().mHeroDebrisData;

    for (List<HeroDebrisData>::ConstIter* iter = playerHeroDebrisData.mHeroDebrisList.begin();
        iter != NULL; iter = playerHeroDebrisData.mHeroDebrisList.next(iter))
    {
        HeroDebrisTemplate* heroDebrisTemplate = HERO_DEBRIS_TABLE().get(iter->mValue.mTemplateId);
        if (!heroDebrisTemplate)
        {
            continue;
        }
        long index = mListCtrl->InsertItem(mListCtrl->GetItemCount(), wxT(""));
        mListCtrl->SetItem(index, COL_TYPE_HERO_DEBRIS_ID, wxString::Format("%llu", iter->mValue.mHeroDebrisId));
        mListCtrl->SetItem(index, COL_TYPE_HERO_DEBRIS_TEMPLATE_ID, wxString::Format("%u", iter->mValue.mTemplateId));
        mListCtrl->SetItem(index, COL_TYPE_HERO_DEBRIS_NAME, heroDebrisTemplate->mName.c_str());
        mListCtrl->SetItem(index, COL_TYPE_HERO_DEBRIS_DESC, heroDebrisTemplate->mDesc.c_str());
    }

    mListBox->SetLabel(STATIC_BOX_ITEM_LABEL(playerHeroDebrisData.mHeroDebrisList.size(), 100));
}

wxString
PlayerInfoBagPanel::getItemColTypeText(int colType)
{
    switch (colType)
    {
    case COL_TYPE_ITEM_ID:
        return wxString(wxT("道具ID"));
    case COL_TYPE_ITEM_TEMPLATE_ID:
        return wxString(wxT("模板ID"));
    case COL_TYPE_ITEM_NAME:
        return wxString(wxT("道具名称"));
    case COL_TYPE_ITEM_COUNT:
        return wxString(wxT("道具数量"));
    case COL_TYPE_ITEM_DESC:
        return wxString(wxT("道具描述"));
    }

    return wxString(wxT("未知"));
}

wxString
PlayerInfoBagPanel::getEquipColTypeText(int colType)
{
    switch (colType)
    {
    case COL_TYPE_EQUIP_ID:
        return wxString(wxT("装备ID"));
    case COL_TYPE_EQUIP_TEMPLATE_ID:
        return wxString(wxT("模板ID"));
    case COL_TYPE_EQUIP_NAME:
        return wxString(wxT("装备名称"));
    case COL_TYPE_EQUIP_DESC:
        return wxString(wxT("装备描述"));
    }

    return wxString(wxT("未知"));
}

wxString
PlayerInfoBagPanel::getRuneColTypeText(int colType)
{
    switch (colType)
    {
    case COL_TYPE_RUNE_ID:
        return wxString(wxT("符文ID"));
    case COL_TYPE_RUNE_TEMPLATE_ID:
        return wxString(wxT("模板ID"));
    case COL_TYPE_RUNE_NAME:
        return wxString(wxT("符文名称"));
    case COL_TYPE_RUNE_DESC:
        return wxString(wxT("符文描述"));
    }

    return wxString(wxT("未知"));
}

wxString 
PlayerInfoBagPanel::getHeroDebrisColTypeText(int colType)
{
    switch (colType)
    {
    case COL_TYPE_HERO_DEBRIS_ID:
        return wxString(wxT("碎片ID"));
    case COL_TYPE_HERO_DEBRIS_TEMPLATE_ID:
        return wxString(wxT("模板ID"));
    case COL_TYPE_HERO_DEBRIS_NAME:
        return wxString(wxT("碎片名称"));
    case COL_TYPE_HERO_DEBRIS_DESC:
        return wxString(wxT("碎片描述"));
    }

    return wxString(wxT("未知"));
}

bool 
PlayerInfoBagPanel::getListCtrlTextContents(long index, UInt32 colType, std::string& res)
{
    res.clear();

    if (!mListCtrl)
    {
        return false;
    }

    wxListItem info;
    info.m_itemId = index;
    info.m_col = colType;
    info.m_mask = wxLIST_MASK_TEXT;

    if (!mListCtrl->GetItem(info))
    {
        return false;
    }

    res = info.GetText().c_str();
    return true;
}

void 
PlayerInfoBagPanel::onRightClicked(wxListEvent& ev)
{
    if (!mOperateMenu)
    {
        return;
    }

    if (!mListCtrl)
    {
        return;
    }

    if ((mListCtrl->GetWindowStyle() & wxLC_REPORT) && mListCtrl->GetSelectedItemCount() == 1)
    {
        wxPoint pt = ev.GetPoint();
        pt.y += mListCtrl->GetPosition().y;
        PopupMenu(mOperateMenu, pt);
    }
}

void 
PlayerInfoBagPanel::onCopyId(wxCommandEvent& ev)
{
    if (!mListCtrl)
    {
        return;
    }

    std::string value;
    if (!getListCtrlTextContents(mListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED),
        0, value))
    {
        return;
    }

    wxClipboardLocker locker;
    if (!locker)
    {
        LOG_WARN("Failed to lock clipboard locker.");
    }
    else
    {
        if (wxTheClipboard->Open())
        {
            wxTheClipboard->SetData(new wxTextDataObject(value.c_str()));
            wxTheClipboard->Close();
        }
    }
}

