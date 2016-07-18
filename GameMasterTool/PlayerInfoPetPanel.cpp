#include "PlayerInfoPetPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

#define STATIC_BOX_PET_LABEL(currCount) (wxString::Format("神兽列表(%u)", currCount))

IMPLEMENT_CLASS(PlayerInfoPetPanel, wxPanel)

BEGIN_EVENT_TABLE(PlayerInfoPetPanel, wxPanel)
EVT_TREE_ITEM_ACTIVATED(myID_PLAYER_INFO_PET_PANEL_TREE, onTreeItemActivated)
EVT_MENU(myID_PLAYER_INFO_PET_PANEL_SKILL_MENU, onCopySkillId)
EVT_LIST_ITEM_RIGHT_CLICK(myID_PLAYER_INFO_PET_PANEL_SKILL, onSkillListRightClicked)
END_EVENT_TABLE()

PlayerInfoPetPanel::PlayerInfoPetPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos,
                         const wxSize& size, long style, const wxString& name)
                         : wxPanel(parent, winid, pos, size, style, name)
{
    mTreeCtrlBox = NULL;
    mTreeCtrl = NULL;
    mPropertyGridBox = NULL;
    mPropertyGrid = NULL;
    mSkillList = NULL;
    mSkillListBox = NULL;
    mPetInfoBox = NULL;
    mSkillMenu = NULL;
}

PlayerInfoPetPanel::~PlayerInfoPetPanel()
{

}

bool
PlayerInfoPetPanel::initial()
{
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(topSizer);

    mTreeCtrlBox = new wxStaticBox(this, wxID_ANY, STATIC_BOX_PET_LABEL(0));
    wxStaticBoxSizer* treeCtrlBoxSizer = new wxStaticBoxSizer(mTreeCtrlBox, wxHORIZONTAL);
    topSizer->Add(treeCtrlBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);
    mTreeCtrl = new wxTreeCtrl(this, myID_PLAYER_INFO_PET_PANEL_TREE, wxDefaultPosition, wxDefaultSize);
    treeCtrlBoxSizer->Add(mTreeCtrl, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mPetInfoBox = new wxStaticBox(this, wxID_ANY, wxT("神兽信息"));
    wxStaticBoxSizer* heroInfoBoxSizer = new wxStaticBoxSizer(mPetInfoBox, wxVERTICAL);
    topSizer->Add(heroInfoBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mPropertyGridBox = new wxStaticBox(this, wxID_ANY, wxT("神兽属性"));
    wxStaticBoxSizer* propertyGridBoxSizer = new wxStaticBoxSizer(mPropertyGridBox, wxHORIZONTAL);
    heroInfoBoxSizer->Add(propertyGridBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mPropertyGrid = new wxPropertyGrid(this, myID_PLAYER_INFO_PET_PANEL_PROPERTY, wxDefaultPosition, wxDefaultSize, 
        wxPG_BOLD_MODIFIED | wxPG_TOOLBAR | wxPG_SPLITTER_AUTO_CENTER | wxPG_STATIC_SPLITTER);
    mPropertyGrid->SetVerticalSpacing(2);
    propertyGridBoxSizer->Add(mPropertyGrid, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);
    
    mSkillListBox = new wxStaticBox(this, wxID_ANY, wxT("神兽技能"));
    wxStaticBoxSizer* skillListBoxSizer = new wxStaticBoxSizer(mSkillListBox, wxHORIZONTAL);
    heroInfoBoxSizer->Add(skillListBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mSkillList = new wxListCtrl(this, myID_PLAYER_INFO_PET_PANEL_SKILL, 
        wxDefaultPosition,  wxSize(700, 100), wxLC_REPORT | wxLC_SINGLE_SEL);
    skillListBoxSizer->Add(mSkillList, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mSkillMenu = new wxMenu(wxT(""));
    mSkillMenu->Append(myID_PLAYER_INFO_PET_PANEL_SKILL_MENU, wxT("拷贝ID"));

    resetLayout();
    registerEvents();
    return true;
}

void 
PlayerInfoPetPanel::release()
{
    if (mSkillMenu)
    {
        delete mSkillMenu;
        mSkillMenu = NULL;
    }

    deregisterEvents();
}

void
PlayerInfoPetPanel::registerEvents()
{

}

void
PlayerInfoPetPanel::deregisterEvents()
{

}

void
PlayerInfoPetPanel::resetLayout()
{
    if (mTreeCtrl)
    {
        mTreeCtrl->DeleteAllItems();
    }

    if (mPropertyGrid)
    {
        mPropertyGrid->Clear();
    }

    if (mSkillList)
    {
        mSkillList->DeleteAllItems();
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    const PlayerPetData& petData = gmClient->getPlayerData().mPetData;
    mTreeCtrlRootItem = mTreeCtrl->AddRoot("神兽列表", 0);
    mTreeItemList.clear();

    for (List<PetData>::ConstIter* iter = petData.mPetDataList.begin();
        iter != NULL; iter = petData.mPetDataList.next(iter))
    {
        PetTemplate* petTemplate = PET_TABLE().get(iter->mValue.mBaseData.mTemplateId);
        if (!petTemplate)
        {
            continue;
        }
        TreeItem treeItem;
        treeItem.mPetName = petTemplate->mName.c_str();
        treeItem.mPetGuid = iter->mValue.mBaseData.mPetId;
        treeItem.mTreeItemId = mTreeCtrl->AppendItem(mTreeCtrlRootItem, petTemplate->mName.c_str(), 1, -1);
        mTreeItemList.insertTail(treeItem);
    }

    mTreeCtrl->Expand(mTreeCtrlRootItem);
    mTreeCtrlBox->SetLabel(STATIC_BOX_PET_LABEL(petData.mPetDataList.size()));
}

PlayerInfoPetPanel::TreeItem* 
PlayerInfoPetPanel::getTreeItem(wxTreeItemId id)
{
    for (List<TreeItem>::Iter* iter = mTreeItemList.begin();
        iter != NULL; iter = mTreeItemList.next(iter))
    {
        if (id == iter->mValue.mTreeItemId)
        {
            return &iter->mValue;
        }
    }
    return NULL;
}

wxString 
PlayerInfoPetPanel::getSkillColTypeText(int colType)
{
    switch (colType)
    {
    case COL_TYPE_SKILL_TEMPLATE_ID:
        return wxString(wxT("模板ID"));
    case COL_TYPE_SKILL_NAME:
        return wxString(wxT("技能名称"));
    case COL_TYPE_SKILL_TYPE:
        return wxString(wxT("技能类型"));
    case COL_TYPE_SKILL_COOLDOWN:
        return wxString(wxT("技能回合数"));
    case COL_TYPE_SKILL_INC_ANGER:
        return wxString(wxT("回怒气值"));
    case COL_TYPE_SKILL_TRIGGER_ANGER:
        return wxString(wxT("释放怒气值"));
    case COL_TYPE_SKILL_DESC:
        return wxString(wxT("技能描述"));
    }
    return wxString(wxT("未知"));
}

void 
PlayerInfoPetPanel::showPetInfos(const Guid& guid)
{
    showPetProperty(guid);
    showPetSkill(guid);
}

void 
PlayerInfoPetPanel::showPetProperty(const Guid& guid)
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

    const List<PetData>& petDataList = gmClient->getPlayerData().mPetData.mPetDataList;
    const PetData* petData = NULL;

    for (List<PetData>::ConstIter* iter = petDataList.begin();
        iter != NULL; iter = petDataList.next(iter))
    {
        if (guid == iter->mValue.mBaseData.mPetId)
        {
            petData = &iter->mValue;
            break;
        }
    }

    if (!petData)
    {
        return;
    }

    PetTemplate* petTemplate = PET_TABLE().get(petData->mBaseData.mTemplateId);
    if (!petTemplate)
    {
        return;
    }

    wxPGProperty* p = NULL;
    char buf[256] = { 0 };
    sprintf(buf, "%llu", petData->mBaseData.mPetId);
    p = mPropertyGrid->Append(new wxStringProperty("ID", wxPG_LABEL, buf));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxStringProperty("名称", wxPG_LABEL, petTemplate->mName.c_str()));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("星级", wxPG_LABEL, petData->mBaseData.mStarLevel));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("等级", wxPG_LABEL, petData->mBaseData.mLevel));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("经验", wxPG_LABEL, petData->mBaseData.mLevelExp));
    mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("种族", wxPG_LABEL, petData->mBaseData.mCarrer));
    //mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("元素", wxPG_LABEL, petData->mBaseData.mElementType));
    //mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("怒气", wxPG_LABEL, petData->mBaseData.mAnger));
    //mPropertyGrid->SetPropertyReadOnly(p);
    
    //p = mPropertyGrid->Append(new wxUIntProperty("血量", wxPG_LABEL, petData->mBaseData.mHp));
    //mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("物理攻击", wxPG_LABEL, petData->mExtendData.mAttack));
    //mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("物理防御", wxPG_LABEL, petData->mExtendData.mPhysicsDefense));
    //mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("魔法防御", wxPG_LABEL, petData->mExtendData.mMagicDefense));
    //mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("暴击率", wxPG_LABEL, petData->mExtendData.mCritical));
    //mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("命中率", wxPG_LABEL, petData->mExtendData.mHit));
    //mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("闪避率", wxPG_LABEL, petData->mExtendData.mDodge));
    //mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("血量上限", wxPG_LABEL, petData->mExtendData.mAngerUpper));
    //mPropertyGrid->SetPropertyReadOnly(p);
    //p = mPropertyGrid->Append(new wxUIntProperty("怒气上限", wxPG_LABEL, petData->mExtendData.mHpUpper));
    //mPropertyGrid->SetPropertyReadOnly(p);
}

void
PlayerInfoPetPanel::showPetSkill(const Guid& guid)
{
    if (!mSkillList)
    {
        return;
    }

    mSkillList->ClearAll();

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    const List<PetData>& petDataList = gmClient->getPlayerData().mPetData.mPetDataList;
    const PetData* petData = NULL;

    for (List<PetData>::ConstIter* iter = petDataList.begin();
        iter != NULL; iter = petDataList.next(iter))
    {
        if (guid == iter->mValue.mBaseData.mPetId)
        {
            petData = &iter->mValue;
            break;
        }
    }

    if (!petData)
    {
        return;
    }

    PetTemplate* petTemplate = PET_TABLE().get(petData->mBaseData.mTemplateId);
    if (!petTemplate)
    {
        return;
    }

    for (int i = 0; i < COL_TYPE_SKILL_MAX_COUNT; ++i)
    {
        wxListItem itemCol;
        itemCol.SetText(getSkillColTypeText(i));
        itemCol.SetWidth(120);
        mSkillList->InsertColumn(i, itemCol);
    }

    const PetSkillData& skillData = petData->mSkillData;
    
    SkillTemplate* skillTemplate = SKILL_TABLE().get(skillData.mCommonSkill.mTemplateId);
    if (!skillTemplate)
    {
        return;
    }

    long index = mSkillList->InsertItem(mSkillList->GetItemCount(), wxT(""));
    mSkillList->SetItem(index, COL_TYPE_SKILL_TEMPLATE_ID, wxString::Format("%u", skillData.mCommonSkill.mTemplateId));
    mSkillList->SetItem(index, COL_TYPE_SKILL_NAME, skillTemplate->mSkillName.c_str());
    mSkillList->SetItem(index, COL_TYPE_SKILL_TYPE, wxString::Format("%u", skillData.mCommonSkill.mSkillAttackType));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_COOLDOWN, wxString::Format("%u", skillData.mCommonSkill.mCoolDownRound));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_INC_ANGER, wxString::Format("%u", skillData.mCommonSkill.mIncAnger));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_TRIGGER_ANGER, wxString::Format("%u", skillData.mCommonSkill.mTriggerAnger));
    mSkillList->SetItem(index, COL_TYPE_SKILL_DESC, skillTemplate->mSkillName.c_str());
}

void 
PlayerInfoPetPanel::onTreeItemActivated(wxTreeEvent& ev)
{
    wxTreeItemId itemId = ev.GetItem();

    if (!itemId.IsOk())
    {
        return;
    }

    if (itemId == mTreeCtrlRootItem)
    {
        return;
    }

    TreeItem* treeItem = getTreeItem(itemId);
    if (!treeItem)
    {
        return;
    }

    showPetInfos(treeItem->mPetGuid);
}

void 
PlayerInfoPetPanel::onCopySkillId(wxCommandEvent& ev)
{
    if (!mSkillList)
    {
        return;
    }

    std::string value;
    if (!getSkillListTextContents(mSkillList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED),
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

void 
PlayerInfoPetPanel::onSkillListRightClicked(wxListEvent& ev)
{
    if (!mSkillMenu)
    {
        return;
    }

    if (!mSkillList)
    {
        return;
    }

    if ((mSkillList->GetWindowStyle() & wxLC_REPORT) && mSkillList->GetSelectedItemCount() == 1)
    {
        wxPoint pt = ev.GetPoint();
        pt.y += mSkillList->GetPosition().y;
        PopupMenu(mSkillMenu, pt);
    }
}

bool 
PlayerInfoPetPanel::getSkillListTextContents(long index, UInt32 colType, std::string& res)
{
    res.clear();

    if (!mSkillList)
    {
        return false;
    }

    wxListItem info;
    info.m_itemId = index;
    info.m_col = colType;
    info.m_mask = wxLIST_MASK_TEXT;

    if (!mSkillList->GetItem(info))
    {
        return false;
    }

    res = info.GetText().c_str();
    return true;
}
