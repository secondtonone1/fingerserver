#include "GuildPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

#define STATIC_BOX_GUILD_RESULT_LABEL(count) (wxString::Format("查询结果 公会(%u)", count))

IMPLEMENT_CLASS(GuildPanel, wxPanel)

BEGIN_EVENT_TABLE(GuildPanel, wxPanel)
EVT_LIST_ITEM_SELECTED(myID_GUILD_PANEL_SEARCH_RESULT, onSelected)
EVT_LIST_ITEM_ACTIVATED(myID_GUILD_PANEL_SEARCH_RESULT, onActived)
EVT_LIST_ITEM_RIGHT_CLICK(myID_GUILD_PANEL_SEARCH_RESULT, onRightClicked)
EVT_LIST_COL_CLICK(myID_GUILD_PANEL_SEARCH_RESULT, onColClicked)
EVT_MENU(myID_GUILD_MENU_SHOW, onShowGuildInfoDialog)
END_EVENT_TABLE()

GuildPanel::GuildPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    mBtnResetLayout = NULL;
    mBtnShowOnline = NULL;
    mBtnClearResult = NULL;
    mTextCtrlGuildGuid = NULL;
    mBtnSearchGuildGuid = NULL;
    mBtnClearGuildGuid = NULL;
    mTextCtrlGuildName = NULL;
    mBtnSearchGuildName = NULL;
    mBtnClearGuildName = NULL;
    mTextResultTips = NULL;
    mListResultSet = NULL;
    mOperateMenu = NULL;
    memset(mSortFlag, 0, sizeof(mSortFlag));
}

GuildPanel::~GuildPanel()
{
    release();
}

bool
GuildPanel::initial()
{
    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(topSizer);

    // 方式
    wxStaticBox* searchPatternBox = new wxStaticBox(this, wxID_ANY, wxT("方式"));
    wxStaticBoxSizer* patternSizer = new wxStaticBoxSizer(searchPatternBox, wxHORIZONTAL);
    topSizer->Add(patternSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);
    wxBoxSizer* searchPattern1BoxSizer = new wxBoxSizer(wxVERTICAL);
    patternSizer->Add(searchPattern1BoxSizer, wxGROW|wxALIGN_CENTRE|wxALL, 5);
    wxBoxSizer* searchPattern2BoxSizer = new wxBoxSizer(wxVERTICAL);
    patternSizer->Add(searchPattern2BoxSizer, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    wxBoxSizer* guildGuidSizer = new wxBoxSizer(wxHORIZONTAL);
    searchPattern1BoxSizer->Add(guildGuidSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    
    wxStaticText* guildGuidStaticText = new wxStaticText(this, wxID_STATIC, wxT("公会ID:"));
    mTextCtrlGuildGuid = new wxTextCtrl(this, myID_GUILD_PANEL_GUILD_GUID, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mBtnSearchGuildGuid = new wxButton(this, myID_GUILD_PANEL_GUILD_GUID_SEARCH, wxT("查询"));
    mBtnClearGuildGuid = new wxButton(this, myID_GUILD_PANEL_GUILD_GUID_CLEAR, wxT("清空"));

    guildGuidSizer->Add(guildGuidStaticText, 0, wxALL|wxALIGN_CENTRE, 5);
    guildGuidSizer->Add(mTextCtrlGuildGuid, 1, wxALL|wxALIGN_CENTRE, 5);
    guildGuidSizer->Add(mBtnSearchGuildGuid, 0, wxALL|wxALIGN_CENTRE, 5);
    guildGuidSizer->Add(mBtnClearGuildGuid, 0, wxALL|wxALIGN_CENTRE, 5);

    wxBoxSizer* guildNameSizer = new wxBoxSizer(wxHORIZONTAL);
    searchPattern2BoxSizer->Add(guildNameSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);

    wxStaticText* guildNameStaticText = new wxStaticText(this, wxID_STATIC, wxT("公会名:"));
    mTextCtrlGuildName = new wxTextCtrl(this, myID_GUILD_PANEL_GUILD_NAME, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mBtnSearchGuildName = new wxButton(this, myID_GUILD_PANEL_GUILD_NAME_SEARCH, wxT("查询"));
    mBtnClearGuildName = new wxButton(this, myID_GUILD_PANEL_GUILD_NAME_CLEAR, wxT("清空"));

    guildNameSizer->Add(guildNameStaticText, 0, wxALL|wxALIGN_CENTRE, 5);
    guildNameSizer->Add(mTextCtrlGuildName, 1, wxALL|wxALIGN_CENTRE, 5);
    guildNameSizer->Add(mBtnSearchGuildName, 0, wxALL|wxALIGN_CENTRE, 5);
    guildNameSizer->Add(mBtnClearGuildName, 0, wxALL|wxALIGN_CENTRE, 5);

    // 操作
    wxStaticBox* uiOperatorBox = new wxStaticBox(this, wxID_ANY, wxT("操作"));
    wxStaticBoxSizer* uiOperatorBoxSizer = new wxStaticBoxSizer(uiOperatorBox, wxHORIZONTAL);
    topSizer->Add(uiOperatorBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    mBtnShowOnline = new wxButton(this, myID_GUILD_PANEL_SHOW_ONLINE, wxT("查询所有"));
    mBtnResetLayout = new wxButton(this, myID_GUILD_PANEL_RESET_LAYOUT, wxT("重置界面"));
    mBtnClearResult = new wxButton(this, myID_GUILD_PANEL_CLEAR_RESULT, wxT("清空结果"));

    uiOperatorBoxSizer->Add(mBtnShowOnline, 0, wxALL|wxALIGN_CENTRE, 5);
    uiOperatorBoxSizer->Add(mBtnResetLayout, 0, wxALL|wxALIGN_CENTRE, 5);
    uiOperatorBoxSizer->Add(mBtnClearResult, 0, wxALL|wxALIGN_CENTRE, 5);

    // 分割线
    //wxStaticLine* staticLine = new wxStaticLine(this);
    //topSizer->Add(staticLine, 0, wxGROW|wxALIGN_CENTRE|wxALL, 10);

    // 查询结果
    wxStaticBox* resultBox = new wxStaticBox(this, wxID_ANY, STATIC_BOX_GUILD_RESULT_LABEL(0));
    wxStaticBoxSizer* resultBoxSizer = new wxStaticBoxSizer(resultBox, wxVERTICAL);
    topSizer->Add(resultBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    wxBoxSizer* tipsSizer = new wxBoxSizer(wxHORIZONTAL);
    mListResultSet = createSearchResultListCtrl();

    resultBoxSizer->Add(tipsSizer, 0, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    resultBoxSizer->Add(mListResultSet, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);

    wxStaticText* stateStaticText = new wxStaticText(this, wxID_STATIC, wxT("状态："));
    stateStaticText->SetOwnForegroundColour(wxColour(wxT("blue")));
    tipsSizer->Add(stateStaticText, 0, wxALIGN_LEFT, 0);
    mTextResultTips = new wxStaticText(this, wxID_STATIC, wxT("无效"));
    mTextResultTips->SetOwnForegroundColour(wxColour(wxT("grey")));
    tipsSizer->Add(mTextResultTips, 1, wxGROW|wxALL|wxALIGN_CENTRE, 0);

    // 重置UI
    resetLayout();

    topSizer->Fit(this);
    topSizer->SetSizeHints(this);


    mOperateMenu = new wxMenu(wxT(""));
    mOperateMenu->Append(myID_GUILD_MENU_SHOW, wxT("查看公会"));
    mOperateMenu->AppendSeparator();
    mOperateMenu->Append(myID_GUILD_MENU_COPY_ID, wxT("拷贝公会ID"));
    mOperateMenu->Append(myID_GUILD_MENU_COPY_NAME, wxT("拷贝公会名称"));

    // 临时插入测试数据
    long index = mListResultSet->InsertItem(mListResultSet->GetItemCount(), wxT(""));
    mListResultSet->SetItemPtrData(index, 123456789);
    mListResultSet->SetItem(index, COL_TYPE_GUILD_ID, wxString::Format("%u", 123456789));
    mListResultSet->SetItem(index, COL_TYPE_GUILD_NAME, "abcd");
    mListResultSet->SetItem(index, COL_TYPE_GUILD_LEVEL, wxString::Format("%u", 99));
    mListResultSet->SetItem(index, COL_TYPE_GUILD_MEMBER_COUNT, wxString::Format("%u", 100));
    mListResultSet->SetItem(index, COL_TYPE_GUILD_ONLINE_MEMBER_COUNT, wxString::Format("%u", 75));

    registerEvents();
    return true;
}

void 
GuildPanel::release()
{
    if (mOperateMenu)
    {
        delete mOperateMenu;
        mOperateMenu = NULL;
    }

    deregisterEvents();
}

void
GuildPanel::registerEvents()
{

}

void
GuildPanel::deregisterEvents()
{

}

wxString
GuildPanel::getColTypeText(int colType)
{
    switch (colType)
    {
    case COL_TYPE_GUILD_ID:
        return wxString(wxT("公会ID"));
    case COL_TYPE_GUILD_NAME:
        return wxString(wxT("公会名称"));
    case COL_TYPE_GUILD_LEVEL:
        return wxString(wxT("公会等级"));
    case COL_TYPE_GUILD_MEMBER_COUNT:
        return wxString(wxT("公会成员数"));
    case COL_TYPE_GUILD_ONLINE_MEMBER_COUNT:
        return wxString(wxT("在线成员数"));
    }

    return wxString(wxT("未知"));
}

void 
GuildPanel::onSelected(wxListEvent& ev)
{
    if (!mListResultSet)
    {
        return;
    }

}

void 
GuildPanel::onActived(wxListEvent& ev)
{
    if (!mListResultSet)
    {
        return;
    }

}

void 
GuildPanel::onRightClicked(wxListEvent& ev)
{
    if (!mOperateMenu)
    {
        return;
    }

    if (!mListResultSet)
    {
        return;
    }

    if ((mListResultSet->GetWindowStyle() & wxLC_REPORT) && mListResultSet->GetSelectedItemCount() == 1)
    {
        wxPoint pt = ev.GetPoint();
        pt.y += mListResultSet->GetPosition().y;
        PopupMenu(mOperateMenu, pt);
    }
}

void 
GuildPanel::onColClicked(wxListEvent& ev)
{
    if (!mListResultSet)
    {
        return;
    }

    int col = ev.GetColumn();

    if (col < 0 || col >= COL_TYPE_MAX_COUNT)
    {
        return;
    }
}

wxListCtrl*
GuildPanel::createSearchResultListCtrl()
{
    wxListCtrl* listCtrl = new wxListCtrl(this, myID_GUILD_PANEL_SEARCH_RESULT, 
        wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);

    for (int index = 0; index < COL_TYPE_MAX_COUNT; ++index)
    {
        wxListItem itemCol;
        itemCol.SetText(getColTypeText(index));
        itemCol.SetWidth(160);
        listCtrl->InsertColumn(index, itemCol);
    }

    return listCtrl;
}

void 
GuildPanel::resetLayout()
{
    clearGuildGuid();
    clearGuildName();
    clearResultSet();
}

void 
GuildPanel::clearGuildGuid()
{
    if (mTextCtrlGuildGuid)
    {
        mTextCtrlGuildGuid->Clear();
    }
}

void 
GuildPanel::clearGuildName()
{
    if (mTextCtrlGuildName)
    {
        mTextCtrlGuildName->Clear();
    }
}

void 
GuildPanel::clearResultSet(bool isClearTips)
{
    if (isClearTips && mTextResultTips)
    {
        mTextResultTips->SetLabelText("无效");
        mTextResultTips->SetOwnForegroundColour(wxT("grey"));
    }

    if (mListResultSet)
    {
        mListResultSet->DeleteAllItems();
    }
}

void 
GuildPanel::onShowGuildInfoDialog(wxCommandEvent& ev)
{
    MAIN_FRAME().getGuildInfoDialog().ShowModal();
}