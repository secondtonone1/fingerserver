#include "PlayerInfoHeroPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

#define STATIC_BOX_HERO_LABEL(currCount) (wxString::Format("英雄列表(%u)", currCount))

IMPLEMENT_CLASS(PlayerInfoHeroPanel, wxPanel)

BEGIN_EVENT_TABLE(PlayerInfoHeroPanel, wxPanel)
EVT_TREE_ITEM_ACTIVATED(myID_PLAYER_INFO_HERO_PANEL_TREE, onTreeItemActivated)
EVT_MENU(myID_PLAYER_INFO_HERO_PANEL_EQUIP_MENU, onCopyEquipId)
EVT_MENU(myID_PLAYER_INFO_HERO_PANEL_RUNE_MENU, onCopyRuneId)
EVT_MENU(myID_PLAYER_INFO_HERO_PANEL_SKILL_MENU, onCopySkillId)
EVT_LIST_ITEM_RIGHT_CLICK(myID_PLAYER_INFO_HERO_PANEL_EQUIP, onEquipListRightClicked)
EVT_LIST_ITEM_RIGHT_CLICK(myID_PLAYER_INFO_HERO_PANEL_RUNE, onRuneListRightClicked)
EVT_LIST_ITEM_RIGHT_CLICK(myID_PLAYER_INFO_HERO_PANEL_SKILL, onSkillListRightClicked)

END_EVENT_TABLE()

PlayerInfoHeroPanel::PlayerInfoHeroPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos,
                         const wxSize& size, long style, const wxString& name)
                         : wxPanel(parent, winid, pos, size, style, name)
{
    mTreeCtrlBox = NULL;
    mTreeCtrl = NULL;
    mPropertyGridBox = NULL;
    mPropertyGrid = NULL;
    mEquipListBox = NULL;
    mEquipList = NULL;
    mRuneListBox = NULL;
    mRuneList = NULL;
    mHeroInfoBox = NULL;
    mSkillList = NULL;
    mSkillListBox = NULL;
    mEquipMenu = NULL;
    mRuneMenu = NULL;
    mSkillMenu = NULL;
}

PlayerInfoHeroPanel::~PlayerInfoHeroPanel()
{

}

bool
PlayerInfoHeroPanel::initial()
{
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(topSizer);

    mTreeCtrlBox = new wxStaticBox(this, wxID_ANY, STATIC_BOX_HERO_LABEL(0));
    wxStaticBoxSizer* treeCtrlBoxSizer = new wxStaticBoxSizer(mTreeCtrlBox, wxHORIZONTAL);
    topSizer->Add(treeCtrlBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);
    mTreeCtrl = new wxTreeCtrl(this, myID_PLAYER_INFO_HERO_PANEL_TREE, wxDefaultPosition, wxDefaultSize);
    treeCtrlBoxSizer->Add(mTreeCtrl, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mHeroInfoBox = new wxStaticBox(this, wxID_ANY, wxT("英雄信息"));
    wxStaticBoxSizer* heroInfoBoxSizer = new wxStaticBoxSizer(mHeroInfoBox, wxVERTICAL);
    topSizer->Add(heroInfoBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mPropertyGridBox = new wxStaticBox(this, wxID_ANY, wxT("英雄属性"));
    wxStaticBoxSizer* propertyGridBoxSizer = new wxStaticBoxSizer(mPropertyGridBox, wxHORIZONTAL);
    heroInfoBoxSizer->Add(propertyGridBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mPropertyGrid = new wxPropertyGrid(this, myID_PLAYER_INFO_HERO_PANEL_PROPERTY, wxDefaultPosition, wxDefaultSize, 
        wxPG_BOLD_MODIFIED | wxPG_TOOLBAR | wxPG_SPLITTER_AUTO_CENTER | wxPG_STATIC_SPLITTER);
    mPropertyGrid->SetVerticalSpacing(2);
    propertyGridBoxSizer->Add(mPropertyGrid, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);
    
    mEquipListBox = new wxStaticBox(this, wxID_ANY, wxT("英雄装备"));
    wxStaticBoxSizer* equipListBoxSizer = new wxStaticBoxSizer(mEquipListBox, wxHORIZONTAL);
    heroInfoBoxSizer->Add(equipListBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mEquipList = new wxListCtrl(this, myID_PLAYER_INFO_HERO_PANEL_EQUIP, 
        wxDefaultPosition,  wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    equipListBoxSizer->Add(mEquipList, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mRuneListBox = new wxStaticBox(this, wxID_ANY, wxT("英雄符文"));
    wxStaticBoxSizer* runeListBoxSizer = new wxStaticBoxSizer(mRuneListBox, wxHORIZONTAL);
    heroInfoBoxSizer->Add(runeListBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mRuneList = new wxListCtrl(this, myID_PLAYER_INFO_HERO_PANEL_RUNE, 
        wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    runeListBoxSizer->Add(mRuneList, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mSkillListBox = new wxStaticBox(this, wxID_ANY, wxT("英雄技能"));
    wxStaticBoxSizer* skillListBoxSizer = new wxStaticBoxSizer(mSkillListBox, wxHORIZONTAL);
    heroInfoBoxSizer->Add(skillListBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mSkillList = new wxListCtrl(this, myID_PLAYER_INFO_HERO_PANEL_SKILL, 
        wxDefaultPosition, wxSize(700, 100), wxLC_REPORT | wxLC_SINGLE_SEL);
    skillListBoxSizer->Add(mSkillList, 1, wxGROW|wxALIGN_CENTRE|wxALL, 2);

    mEquipMenu = new wxMenu(wxT(""));
    mEquipMenu->Append(myID_PLAYER_INFO_HERO_PANEL_EQUIP_MENU, wxT("拷贝ID"));

    mRuneMenu = new wxMenu(wxT(""));
    mRuneMenu->Append(myID_PLAYER_INFO_HERO_PANEL_RUNE_MENU, wxT("拷贝ID"));

    mSkillMenu = new wxMenu(wxT(""));
    mSkillMenu->Append(myID_PLAYER_INFO_HERO_PANEL_SKILL_MENU, wxT("拷贝ID"));

    resetLayout();
    registerEvents();
    return true;
}

void 
PlayerInfoHeroPanel::release()
{
    if (mEquipMenu)
    {
        delete (mEquipMenu);
        mEquipMenu = NULL;
    }

    if (mRuneMenu)
    {
        delete mRuneMenu;
        mEquipMenu = NULL;
    }

    if (mSkillMenu)
    {
        delete mSkillMenu;
        mEquipMenu = NULL;
    }

    deregisterEvents();
}

void
PlayerInfoHeroPanel::registerEvents()
{

}

void
PlayerInfoHeroPanel::deregisterEvents()
{

}

void
PlayerInfoHeroPanel::resetLayout()
{
    if (mTreeCtrl)
    {
        mTreeCtrl->DeleteAllItems();
    }

    if (mPropertyGrid)
    {
        mPropertyGrid->Clear();
    }

    if (mEquipList)
    {
        mEquipList->DeleteAllItems();
    }

    if (mRuneList)
    {
        mRuneList->DeleteAllItems();
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

    const PlayerHeroData& heroData = gmClient->getPlayerData().mHeroData;
    mTreeCtrlRootItem = mTreeCtrl->AddRoot("英雄列表", 0);
    mPresentHeroRootItem = mTreeCtrl->AppendItem(mTreeCtrlRootItem, "上阵英雄", 1, -1);
    mBackupHeroRootItem = mTreeCtrl->AppendItem(mTreeCtrlRootItem, "备选英雄", 1, -1);
    mTreeItemList.clear();

    for (List<HeroData>::ConstIter* iter = heroData.mHeroDataList.begin();
        iter != NULL; iter = heroData.mHeroDataList.next(iter))
    {
        HeroTemplate* heroTemplate = HERO_TABLE().get(iter->mValue.mBaseData.mTemplateId);
        if (!heroTemplate)
        {
            continue;
        }

        if (iter->mValue.mBaseData.mPresentPos != 0)
        {
            TreeItem treeItem;
            treeItem.mHeroName = heroTemplate->mHeroName.c_str();
            treeItem.mHeroGuid = iter->mValue.mBaseData.mHeroId;
            treeItem.mTreeItemId = mTreeCtrl->AppendItem(mPresentHeroRootItem, heroTemplate->mHeroName.c_str(), 1, -1);
            mTreeItemList.insertTail(treeItem);
        }
        else
        {
            TreeItem treeItem;
            treeItem.mHeroName = heroTemplate->mHeroName.c_str();
            treeItem.mHeroGuid = iter->mValue.mBaseData.mHeroId;
            treeItem.mTreeItemId = mTreeCtrl->AppendItem(mBackupHeroRootItem, heroTemplate->mHeroName.c_str(), 1, -1);
            mTreeItemList.insertTail(treeItem);
        }
    }

    mTreeCtrl->Expand(mTreeCtrlRootItem);
    mTreeCtrlBox->SetLabel(STATIC_BOX_HERO_LABEL(heroData.mHeroDataList.size()));
}

void 
PlayerInfoHeroPanel::onTreeItemActivated(wxTreeEvent& ev)
{
    wxTreeItemId itemId = ev.GetItem();

    if (!itemId.IsOk())
    {
        return;
    }

    if (itemId == mTreeCtrlRootItem ||
        itemId == mPresentHeroRootItem ||
        itemId == mBackupHeroRootItem)
    {
        return;
    }

    TreeItem* treeItem = getTreeItem(itemId);
    if (!treeItem)
    {
        return;
    }

    showHeroInfos(treeItem->mHeroGuid);

}

void 
PlayerInfoHeroPanel::showHeroInfos(const Guid& guid)
{
    showHeroProperty(guid);
    showHeroEquip(guid);
    showHeroRune(guid);
    showHeroSkill(guid);
}

void 
PlayerInfoHeroPanel::showHeroProperty(const Guid& guid)
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

    const List<HeroData>& heroDataList = gmClient->getPlayerData().mHeroData.mHeroDataList;
    const HeroData* heroData = NULL;

    for (List<HeroData>::ConstIter* iter = heroDataList.begin();
        iter != NULL; iter = heroDataList.next(iter))
    {
        if (guid == iter->mValue.mBaseData.mHeroId)
        {
            heroData = &iter->mValue;
            break;
        }
    }

    if (!heroData)
    {
        return;
    }

    HeroTemplate* heroTemplate = HERO_TABLE().get(heroData->mBaseData.mTemplateId);
    if (!heroTemplate)
    {
        return;
    }

    wxPGProperty* p = NULL;
    char buf[256] = { 0 };
    sprintf(buf, "%llu", heroData->mBaseData.mHeroId);
    p = mPropertyGrid->Append(new wxStringProperty("ID", wxPG_LABEL, buf));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxStringProperty("名称", wxPG_LABEL, heroTemplate->mHeroName.c_str()));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("星级", wxPG_LABEL, heroData->mBaseData.mStarLevel));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("等级", wxPG_LABEL, heroData->mBaseData.mLevel));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("经验", wxPG_LABEL, heroData->mBaseData.mLevelExp));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("怒气", wxPG_LABEL, heroData->mBaseData.mAnger));
    mPropertyGrid->SetPropertyReadOnly(p);
    
    p = mPropertyGrid->Append(new wxUIntProperty("血量", wxPG_LABEL, heroData->mBaseData.mHp));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("攻击力", wxPG_LABEL, heroData->mExtendData.mAttack));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("物理防御", wxPG_LABEL, heroData->mExtendData.mPhysicsDefense));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("魔法防御", wxPG_LABEL, heroData->mExtendData.mMagicDefense));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("暴击率", wxPG_LABEL, heroData->mExtendData.mCritical));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("命中率", wxPG_LABEL, heroData->mExtendData.mHit));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("闪避率", wxPG_LABEL, heroData->mExtendData.mDodge));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("血量上限", wxPG_LABEL, heroData->mExtendData.mAngerUpper));
    mPropertyGrid->SetPropertyReadOnly(p);
    p = mPropertyGrid->Append(new wxUIntProperty("怒气上限", wxPG_LABEL, heroData->mExtendData.mHpUpper));
    mPropertyGrid->SetPropertyReadOnly(p);
}

void
PlayerInfoHeroPanel::showHeroEquip(const Guid& guid)
{
    if (!mEquipList)
    {
        return;
    }

    mEquipList->ClearAll();

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    const List<HeroData>& heroDataList = gmClient->getPlayerData().mHeroData.mHeroDataList;
    const HeroData* heroData = NULL;

    for (List<HeroData>::ConstIter* iter = heroDataList.begin();
        iter != NULL; iter = heroDataList.next(iter))
    {
        if (guid == iter->mValue.mBaseData.mHeroId)
        {
            heroData = &iter->mValue;
            break;
        }
    }

    if (!heroData)
    {
        return;
    }

    HeroTemplate* heroTemplate = HERO_TABLE().get(heroData->mBaseData.mTemplateId);
    if (!heroTemplate)
    {
        return;
    }

    for (int i = 0; i < COL_TYPE_EQUIP_MAX_COUNT; ++i)
    {
        wxListItem itemCol;
        itemCol.SetText(getEquipColTypeText(i));
        itemCol.SetWidth(120);
        mEquipList->InsertColumn(i, itemCol);
    }
#if 0
    const List<EquipData>& equipDataList = heroData->mEquipData.mHeroEquipList;
    
    for (List<EquipData>::ConstIter* iter = equipDataList.begin();
        iter != NULL; iter = equipDataList.next(iter))
    {
        EquipTemplate* equipTemplate = EQUIP_TABLE().get(iter->mValue.mTemplateId);
        if (!equipTemplate)
        {
            continue;
        }
        long index = mEquipList->InsertItem(mEquipList->GetItemCount(), wxT(""));
        mEquipList->SetItem(index, COL_TYPE_EQUIP_ID, wxString::Format("%llu", iter->mValue.mItemId));
        mEquipList->SetItem(index, COL_TYPE_EQUIP_TEMPLATE_ID, wxString::Format("%u", iter->mValue.mTemplateId));
        mEquipList->SetItem(index, COL_TYPE_EQUIP_NAME, equipTemplate->mName.c_str());
        mEquipList->SetItem(index, COL_TYPE_EQUIP_DESC, equipTemplate->mDescripe.c_str());
    }
#endif
}

void 
PlayerInfoHeroPanel::showHeroRune(const Guid& guid)
{
    if (!mRuneList)
    {
        return;
    }

    mRuneList->ClearAll();

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    const List<HeroData>& heroDataList = gmClient->getPlayerData().mHeroData.mHeroDataList;
    const HeroData* heroData = NULL;

    for (List<HeroData>::ConstIter* iter = heroDataList.begin();
        iter != NULL; iter = heroDataList.next(iter))
    {
        if (guid == iter->mValue.mBaseData.mHeroId)
        {
            heroData = &iter->mValue;
            break;
        }
    }

    if (!heroData)
    {
        return;
    }

    HeroTemplate* heroTemplate = HERO_TABLE().get(heroData->mBaseData.mTemplateId);
    if (!heroTemplate)
    {
        return;
    }

    for (int i = 0; i < COL_TYPE_RUNE_MAX_COUNT; ++i)
    {
        wxListItem itemCol;
        itemCol.SetText(getRuneColTypeText(i));
        itemCol.SetWidth(120);
        mRuneList->InsertColumn(i, itemCol);
    }

    /*const List<RuneData>& runeDataList = heroData->mRuneData.mHeroRuneList;
    
    for (List<RuneData>::ConstIter* iter = runeDataList.begin();
        iter != NULL; iter = runeDataList.next(iter))
    {
        RuneTemplate* runeTemplate = RUNE_TABLE().get(iter->mValue.mTemplateId);
        if (!runeTemplate)
        {
            continue;
        }
        long index = mRuneList->InsertItem(mEquipList->GetItemCount(), wxT(""));
        mRuneList->SetItem(index, COL_TYPE_RUNE_ID, wxString::Format("%llu", iter->mValue.mItemId));
        mRuneList->SetItem(index, COL_TYPE_RUNE_TEMPLATE_ID, wxString::Format("%u", iter->mValue.mTemplateId));
        mRuneList->SetItem(index, COL_TYPE_RUNE_NAME, runeTemplate->mName.c_str());
        mRuneList->SetItem(index, COL_TYPE_RUNE_DESC, runeTemplate->mDesc.c_str());
    }*/
}

void 
PlayerInfoHeroPanel::showHeroSkill(const Guid& guid)
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

    const List<HeroData>& heroDataList = gmClient->getPlayerData().mHeroData.mHeroDataList;
    const HeroData* heroData = NULL;

    for (List<HeroData>::ConstIter* iter = heroDataList.begin();
        iter != NULL; iter = heroDataList.next(iter))
    {
        if (guid == iter->mValue.mBaseData.mHeroId)
        {
            heroData = &iter->mValue;
            break;
        }
    }

    if (!heroData)
    {
        return;
    }

    HeroTemplate* heroTemplate = HERO_TABLE().get(heroData->mBaseData.mTemplateId);
    if (!heroTemplate)
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

    const SkillData& skillData = heroData->mSkillData;
    
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

    skillTemplate = SKILL_TABLE().get(skillData.mAutoSkill.mTemplateId);
    if (!skillTemplate)
    {
        return;
    }

    index = mSkillList->InsertItem(mSkillList->GetItemCount(), wxT(""));
    mSkillList->SetItem(index, COL_TYPE_SKILL_TEMPLATE_ID, wxString::Format("%u", skillData.mAutoSkill.mTemplateId));
    mSkillList->SetItem(index, COL_TYPE_SKILL_NAME, skillTemplate->mSkillName.c_str());
    mSkillList->SetItem(index, COL_TYPE_SKILL_TYPE, wxString::Format("%u", skillData.mAutoSkill.mSkillAttackType));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_COOLDOWN, wxString::Format("%u", skillData.mAutoSkill.mCoolDownRound));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_INC_ANGER, wxString::Format("%u", skillData.mAutoSkill.mIncAnger));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_TRIGGER_ANGER, wxString::Format("%u", skillData.mAutoSkill.mTriggerAnger));
    mSkillList->SetItem(index, COL_TYPE_SKILL_DESC, skillTemplate->mSkillName.c_str());

    skillTemplate = SKILL_TABLE().get(skillData.mSuperSkill.mTemplateId);
    if (!skillTemplate)
    {
        return;
    }

    index = mSkillList->InsertItem(mSkillList->GetItemCount(), wxT(""));
    mSkillList->SetItem(index, COL_TYPE_SKILL_TEMPLATE_ID, wxString::Format("%u", skillData.mSuperSkill.mTemplateId));
    mSkillList->SetItem(index, COL_TYPE_SKILL_NAME, skillTemplate->mSkillName.c_str());
    mSkillList->SetItem(index, COL_TYPE_SKILL_TYPE, wxString::Format("%u", skillData.mSuperSkill.mSkillAttackType));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_COOLDOWN, wxString::Format("%u", skillData.mSuperSkill.mCoolDownRound));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_INC_ANGER, wxString::Format("%u", skillData.mSuperSkill.mIncAnger));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_TRIGGER_ANGER, wxString::Format("%u", skillData.mSuperSkill.mTriggerAnger));
    mSkillList->SetItem(index, COL_TYPE_SKILL_DESC, skillTemplate->mSkillName.c_str());

    skillTemplate = SKILL_TABLE().get(skillData.mEnterSkill.mTemplateId);
    if (!skillTemplate)
    {
        return;
    }

    index = mSkillList->InsertItem(mSkillList->GetItemCount(), wxT(""));
    mSkillList->SetItem(index, COL_TYPE_SKILL_TEMPLATE_ID, wxString::Format("%u", skillData.mEnterSkill.mTemplateId));
    mSkillList->SetItem(index, COL_TYPE_SKILL_NAME, skillTemplate->mSkillName.c_str());
    mSkillList->SetItem(index, COL_TYPE_SKILL_TYPE, wxString::Format("%u", skillData.mEnterSkill.mSkillAttackType));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_COOLDOWN, wxString::Format("%u", skillData.mEnterSkill.mCoolDownRound));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_INC_ANGER, wxString::Format("%u", skillData.mEnterSkill.mIncAnger));
    //mSkillList->SetItem(index, COL_TYPE_SKILL_TRIGGER_ANGER, wxString::Format("%u", skillData.mEnterSkill.mTriggerAnger));
    mSkillList->SetItem(index, COL_TYPE_SKILL_DESC, skillTemplate->mSkillName.c_str());
}

PlayerInfoHeroPanel::TreeItem* 
PlayerInfoHeroPanel::getTreeItem(wxTreeItemId id)
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
PlayerInfoHeroPanel::getEquipColTypeText(int colType)
{
    switch (colType)
    {
    case COL_TYPE_EQUIP_ID:
        return wxString(wxT("ID"));
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
PlayerInfoHeroPanel::getRuneColTypeText(int colType)
{
    switch (colType)
    {
    case COL_TYPE_EQUIP_ID:
        return wxString(wxT("ID"));
    case COL_TYPE_EQUIP_TEMPLATE_ID:
        return wxString(wxT("模板ID"));
    case COL_TYPE_EQUIP_NAME:
        return wxString(wxT("符文名称"));
    case COL_TYPE_EQUIP_DESC:
        return wxString(wxT("符文描述"));
    }
    return wxString(wxT("未知"));
}

wxString
PlayerInfoHeroPanel::getSkillColTypeText(int colType)
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
PlayerInfoHeroPanel::onCopyEquipId(wxCommandEvent& ev)
{
    if (!mEquipList)
    {
        return;
    }

    std::string value;
    if (!getEquipListTextContents(mEquipList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED),
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
PlayerInfoHeroPanel::onCopyRuneId(wxCommandEvent& ev)
{
    if (!mRuneList)
    {
        return;
    }

    std::string value;
    if (!getRuneListTextContents(mRuneList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED),
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
PlayerInfoHeroPanel::onCopySkillId(wxCommandEvent& ev)
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
PlayerInfoHeroPanel::onEquipListRightClicked(wxListEvent& ev)
{
    if (!mEquipMenu)
    {
        return;
    }

    if (!mEquipList)
    {
        return;
    }

    if ((mEquipList->GetWindowStyle() & wxLC_REPORT) && mEquipList->GetSelectedItemCount() == 1)
    {
        wxPoint pt = ev.GetPoint();
        pt.y += mEquipList->GetPosition().y;
        PopupMenu(mEquipMenu, pt);
    }
}

void 
PlayerInfoHeroPanel::onRuneListRightClicked(wxListEvent& ev)
{
    if (!mRuneMenu)
    {
        return;
    }

    if (!mRuneList)
    {
        return;
    }

    if ((mRuneList->GetWindowStyle() & wxLC_REPORT) && mRuneList->GetSelectedItemCount() == 1)
    {
        wxPoint pt = ev.GetPoint();
        pt.y += mRuneList->GetPosition().y;
        PopupMenu(mRuneMenu, pt);
    }
}

void 
PlayerInfoHeroPanel::onSkillListRightClicked(wxListEvent& ev)
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
PlayerInfoHeroPanel::getEquipListTextContents(long index, UInt32 colType, std::string& res)
{
    res.clear();

    if (!mEquipList)
    {
        return false;
    }

    wxListItem info;
    info.m_itemId = index;
    info.m_col = colType;
    info.m_mask = wxLIST_MASK_TEXT;

    if (!mEquipList->GetItem(info))
    {
        return false;
    }

    res = info.GetText().c_str();
    return true;
}

bool 
PlayerInfoHeroPanel::getRuneListTextContents(long index, UInt32 colType, std::string& res)
{
    res.clear();

    if (!mRuneList)
    {
        return false;
    }

    wxListItem info;
    info.m_itemId = index;
    info.m_col = colType;
    info.m_mask = wxLIST_MASK_TEXT;

    if (!mRuneList->GetItem(info))
    {
        return false;
    }

    res = info.GetText().c_str();
    return true;
}

bool 
PlayerInfoHeroPanel::getSkillListTextContents(long index, UInt32 colType, std::string& res)
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
