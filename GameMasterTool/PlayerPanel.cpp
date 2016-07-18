#include "PlayerPanel.h"
#include "PlayerInfoDialog.h"
#include "GameMasterTool.h"

using namespace Lynx;

#define STATIC_BOX_PLAYER_RESULT_LABEL(count) (wxString::Format("查询结果 玩家(%u)", count))

IMPLEMENT_CLASS(PlayerPanel, wxPanel)

BEGIN_EVENT_TABLE(PlayerPanel, wxPanel)
EVT_LIST_ITEM_SELECTED(myID_PLAYER_PANEL_SEARCH_RESULT, onSelected)
EVT_LIST_ITEM_ACTIVATED(myID_PLAYER_PANEL_SEARCH_RESULT, onActived)
EVT_LIST_ITEM_RIGHT_CLICK(myID_PLAYER_PANEL_SEARCH_RESULT, onRightClicked)
EVT_LIST_COL_CLICK(myID_PLAYER_PANEL_SEARCH_RESULT, onColClicked)
EVT_MENU(myID_PLAYER_MENU_SHOW, onShowPlayerInfoDialog)
EVT_BUTTON(myID_PLAYER_PANEL_SHOW_ONLINE, onShowOnlinePlayerList)
EVT_BUTTON(myID_PLAYER_PANEL_RESET_LAYOUT, onResetLayout)
EVT_BUTTON(myID_PLAYER_PANEL_CLEAR_RESULT, onClearResult)
EVT_BUTTON(myID_PLAYER_PANEL_PLAYER_GUID_SEARCH, onFindByPlayerId)
EVT_BUTTON(myID_PLAYER_PANEL_PLAYER_GUID_CLEAR, onClearByPlayerId)
EVT_BUTTON(myID_PLAYER_PANEL_PLAYER_NAME_SEARCH, onFindByPlayerName)
EVT_BUTTON(myID_PLAYER_PANEL_PLAYER_NAME_CLEAR, onClearByPlayerName)
EVT_BUTTON(myID_PLAYER_PANEL_ACCOUNT_GUID_SEARCH, onFindByAccountId)
EVT_BUTTON(myID_PLAYER_PANEL_ACCOUNT_GUID_CLEAR, onClearByAccountId)
EVT_BUTTON(myID_PLAYER_PANEL_ACCOUNT_NAME_SEARCH, onFindByAccountName)
EVT_BUTTON(myID_PLAYER_PANEL_ACCOUNT_NAME_CLEAR, onClearByAccountName)
EVT_MENU(myID_PLAYER_MENU_COPY_ID, onCopyPlayerId)
EVT_MENU(myID_PLAYER_MENU_COPY_NAME, onCopyPlayerName)
EVT_MENU(myID_PLAYER_MENU_COPY_ACCOUNT_ID, onCopyAccountId)
EVT_MENU(myID_PLAYER_MENU_COPY_ACCOUNT_NAME, onCopyAccountName)
END_EVENT_TABLE()

PlayerPanel::PlayerPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    mBtnResetLayout = NULL;
    mBtnShowOnline = NULL;
    mBtnClearResult = NULL;
    mTextCtrlPlayerGuid = NULL;
    mBtnSearchPlayerGuid = NULL;
    mBtnClearPlayerGuid = NULL;
    mTextCtrlPlayerName = NULL;
    mBtnSearchPlayerName = NULL;
    mBtnClearPlayerName = NULL;
    mTextCtrlAccountGuid = NULL;
    mBtnSearchAccountGuid = NULL;
    mBtnClearAccountGuid = NULL;
    mTextCtrlAccountName = NULL;
    mBtnSearchAccountName = NULL;
    mBtnClearAccountName = NULL;
    mTextResultTips = NULL;
    mListResultSet = NULL;
    mOperateMenu = NULL;
    mResultBox = NULL;
    memset(mSortFlag, 0, sizeof(mSortFlag));
}

PlayerPanel::~PlayerPanel()
{
    release();
}

bool
PlayerPanel::initial()
{
    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(topSizer);

    // 方式
    wxStaticBox* searchPatternBox = new wxStaticBox(this, wxID_ANY, wxT("查询"));
    wxStaticBoxSizer* patternSizer = new wxStaticBoxSizer(searchPatternBox, wxHORIZONTAL);
    topSizer->Add(patternSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);
    wxBoxSizer* searchPattern1BoxSizer = new wxBoxSizer(wxVERTICAL);
    patternSizer->Add(searchPattern1BoxSizer, wxGROW|wxALIGN_CENTRE|wxALL, 5);
    wxBoxSizer* searchPattern2BoxSizer = new wxBoxSizer(wxVERTICAL);
    patternSizer->Add(searchPattern2BoxSizer, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    wxBoxSizer* playerGuidSizer = new wxBoxSizer(wxHORIZONTAL);
    searchPattern1BoxSizer->Add(playerGuidSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    
    wxStaticText* playerGuidStaticText = new wxStaticText(this, wxID_STATIC, wxT("玩家ID:"));
    mTextCtrlPlayerGuid = new wxTextCtrl(this, myID_PLAYER_PANEL_PLAYER_GUID, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mBtnSearchPlayerGuid = new wxButton(this, myID_PLAYER_PANEL_PLAYER_GUID_SEARCH, wxT("查询"));
    mBtnClearPlayerGuid = new wxButton(this, myID_PLAYER_PANEL_PLAYER_GUID_CLEAR, wxT("清空"));

    playerGuidSizer->Add(playerGuidStaticText, 0, wxALL|wxALIGN_CENTRE, 5);
    playerGuidSizer->Add(mTextCtrlPlayerGuid, 1, wxALL|wxALIGN_CENTRE, 5);
    playerGuidSizer->Add(mBtnSearchPlayerGuid, 0, wxALL|wxALIGN_CENTRE, 5);
    playerGuidSizer->Add(mBtnClearPlayerGuid, 0, wxALL|wxALIGN_CENTRE, 5);

    wxBoxSizer* playerNameSizer = new wxBoxSizer(wxHORIZONTAL);
    searchPattern1BoxSizer->Add(playerNameSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);

    wxStaticText* playerNameStaticText = new wxStaticText(this, wxID_STATIC, wxT("玩家名:"));
    mTextCtrlPlayerName = new wxTextCtrl(this, myID_PLAYER_PANEL_PLAYER_NAME, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mBtnSearchPlayerName = new wxButton(this, myID_PLAYER_PANEL_PLAYER_NAME_SEARCH, wxT("查询"));
    mBtnClearPlayerName = new wxButton(this, myID_PLAYER_PANEL_PLAYER_NAME_CLEAR, wxT("清空"));

    playerNameSizer->Add(playerNameStaticText, 0, wxALL|wxALIGN_CENTRE, 5);
    playerNameSizer->Add(mTextCtrlPlayerName, 1, wxALL|wxALIGN_CENTRE, 5);
    playerNameSizer->Add(mBtnSearchPlayerName, 0, wxALL|wxALIGN_CENTRE, 5);
    playerNameSizer->Add(mBtnClearPlayerName, 0, wxALL|wxALIGN_CENTRE, 5);

    wxBoxSizer* accountGuidSizer = new wxBoxSizer(wxHORIZONTAL);
    searchPattern2BoxSizer->Add(accountGuidSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    
    wxStaticText* accountGuidStaticText = new wxStaticText(this, wxID_STATIC, wxT("账户ID:"));
    mTextCtrlAccountGuid = new wxTextCtrl(this, myID_PLAYER_PANEL_ACCOUNT_GUID, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mBtnSearchAccountGuid = new wxButton(this, myID_PLAYER_PANEL_ACCOUNT_GUID_SEARCH, wxT("查询"));
    mBtnClearAccountGuid = new wxButton(this, myID_PLAYER_PANEL_ACCOUNT_GUID_CLEAR, wxT("清空"));

    accountGuidSizer->Add(accountGuidStaticText, 0, wxALL|wxALIGN_CENTRE, 5);
    accountGuidSizer->Add(mTextCtrlAccountGuid, 1, wxALL|wxALIGN_CENTRE, 5);
    accountGuidSizer->Add(mBtnSearchAccountGuid, 0, wxALL|wxALIGN_CENTRE, 5);
    accountGuidSizer->Add(mBtnClearAccountGuid, 0, wxALL|wxALIGN_CENTRE, 5);

    wxBoxSizer* accountNameSizer = new wxBoxSizer(wxHORIZONTAL);
    searchPattern2BoxSizer->Add(accountNameSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);

    wxStaticText* accountNameStaticText = new wxStaticText(this, wxID_STATIC, wxT("账户名:"));
    mTextCtrlAccountName = new wxTextCtrl(this, myID_PLAYER_PANEL_ACCOUNT_NAME, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mBtnSearchAccountName = new wxButton(this, myID_PLAYER_PANEL_ACCOUNT_NAME_SEARCH, wxT("查询"));
    mBtnClearAccountName = new wxButton(this, myID_PLAYER_PANEL_ACCOUNT_NAME_CLEAR, wxT("清空"));

    accountNameSizer->Add(accountNameStaticText, 0, wxALL|wxALIGN_CENTRE, 5);
    accountNameSizer->Add(mTextCtrlAccountName, 1, wxALL|wxALIGN_CENTRE, 5);
    accountNameSizer->Add(mBtnSearchAccountName, 0, wxALL|wxALIGN_CENTRE, 5);
    accountNameSizer->Add(mBtnClearAccountName, 0, wxALL|wxALIGN_CENTRE, 5);

    // 操作
    wxStaticBox* uiOperatorBox = new wxStaticBox(this, wxID_ANY, wxT("操作"));
    wxStaticBoxSizer* uiOperatorBoxSizer = new wxStaticBoxSizer(uiOperatorBox, wxHORIZONTAL);
    topSizer->Add(uiOperatorBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    mBtnShowOnline = new wxButton(this, myID_PLAYER_PANEL_SHOW_ONLINE, wxT("查询在线"));
    mBtnResetLayout = new wxButton(this, myID_PLAYER_PANEL_RESET_LAYOUT, wxT("重置界面"));
    mBtnClearResult = new wxButton(this, myID_PLAYER_PANEL_CLEAR_RESULT, wxT("清空结果"));

    uiOperatorBoxSizer->Add(mBtnShowOnline, 0, wxALL|wxALIGN_CENTRE, 5);
    uiOperatorBoxSizer->Add(mBtnResetLayout, 0, wxALL|wxALIGN_CENTRE, 5);
    uiOperatorBoxSizer->Add(mBtnClearResult, 0, wxALL|wxALIGN_CENTRE, 5);

    // 分割线
    //wxStaticLine* staticLine = new wxStaticLine(this);
    //topSizer->Add(staticLine, 0, wxGROW|wxALIGN_CENTRE|wxALL, 10);

    // 查询结果
    mResultBox= new wxStaticBox(this, wxID_ANY, STATIC_BOX_PLAYER_RESULT_LABEL(0));
    wxStaticBoxSizer* resultBoxSizer = new wxStaticBoxSizer(mResultBox, wxVERTICAL);
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

    mOperateMenu->Append(myID_PLAYER_MENU_SHOW, wxT("查看玩家"));
    mOperateMenu->AppendSeparator();

    mOperateMenu->Append(myID_PLAYER_MENU_COPY_ID, wxT("拷贝玩家ID"));
    mOperateMenu->Append(myID_PLAYER_MENU_COPY_NAME, wxT("拷贝玩家名称"));
    mOperateMenu->Append(myID_PLAYER_MENU_COPY_ACCOUNT_ID, wxT("拷贝账户ID"));
    mOperateMenu->Append(myID_PLAYER_MENU_COPY_ACCOUNT_NAME, wxT("拷贝账户名称"));

    mOperateMenu->AppendSeparator();
    mOperateMenu->Append(myID_PLAYER_MENU_FORBIDDEN_CHAT, wxT("禁止发言"));
    mOperateMenu->Append(myID_PLAYER_MENU_FORBIDDEN_LOGIN, wxT("禁止登录"));
    mOperateMenu->Append(myID_PLAYER_MENU_CHAT, wxT("发起私聊"));
    mOperateMenu->Append(myID_PLAYER_MENU_SEND_EMAIL, wxT("发送邮件"));

    registerEvents();
    return true;
}

void 
PlayerPanel::release()
{
    if (mOperateMenu)
    {
        delete mOperateMenu;
        mOperateMenu = NULL;
    }

    deregisterEvents();
}

void
PlayerPanel::registerEvents()
{
    REGISTER_EVENT(GetPlayerByOnlineEvent, this, &PlayerPanel::onGetPlayerByOnlineEvent);
    REGISTER_EVENT(GetPlayerByPlayerGuidEvent, this, &PlayerPanel::onGetPlayerByPlayerGuidEvent);
    REGISTER_EVENT(GetPlayerByPlayerNameEvent, this, &PlayerPanel::onGetPlayerByPlayerNameEvent);
    REGISTER_EVENT(GetPlayerByAccountGuidEvent, this, &PlayerPanel::onGetPlayerByAccountGuidEvent);
    REGISTER_EVENT(GetPlayerByAccountNameEvent, this, &PlayerPanel::onGetPlayerByAccountNameEvent);
    REGISTER_EVENT(GetPlayerDataEvent, this, &PlayerPanel::onGetPlayerDataEvent);
}

void
PlayerPanel::deregisterEvents()
{
    DEREGISTER_EVENT(GetPlayerByOnlineEvent, this, &PlayerPanel::onGetPlayerByOnlineEvent);
    DEREGISTER_EVENT(GetPlayerByPlayerGuidEvent, this, &PlayerPanel::onGetPlayerByPlayerGuidEvent);
    DEREGISTER_EVENT(GetPlayerByPlayerNameEvent, this, &PlayerPanel::onGetPlayerByPlayerNameEvent);
    DEREGISTER_EVENT(GetPlayerByAccountGuidEvent, this, &PlayerPanel::onGetPlayerByAccountGuidEvent);
    DEREGISTER_EVENT(GetPlayerByAccountNameEvent, this, &PlayerPanel::onGetPlayerByAccountNameEvent);
    DEREGISTER_EVENT(GetPlayerDataEvent, this, &PlayerPanel::onGetPlayerDataEvent);
}

wxString
PlayerPanel::getColTypeText(int colType)
{
    switch (colType)
    {
    case COL_TYPE_PLAYER_ID:
        return wxString(wxT("玩家ID"));
    case COL_TYPE_PLAYER_NAME:
        return wxString(wxT("玩家名称"));
    case COL_TYPE_PLAYER_LEVEL:
        return wxString(wxT("玩家等级"));
    case COL_TYPE_PLAYER_ACCOUNT_ID:
        return wxString(wxT("账户ID"));
    case COL_TYPE_PLAYER_ACCOUNT_NAME:
        return wxString(wxT("账户名称"));
    case COL_TYPE_PLAYER_ONLINE:
        return wxString(wxT("是否在线"));
    }

    return wxString(wxT("未知"));
}

void 
PlayerPanel::onSelected(wxListEvent& ev)
{
    if (!mListResultSet)
    {
        return;
    }

}

void 
PlayerPanel::onActived(wxListEvent& ev)
{
    if (!mListResultSet)
    {
        return;
    }

}

void 
PlayerPanel::onRightClicked(wxListEvent& ev)
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
PlayerPanel::onColClicked(wxListEvent& ev)
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

void 
PlayerPanel::onShowPlayerInfoDialog(wxCommandEvent& ev)
{
    if (!mListResultSet)
    {
        return;
    }

    std::string value;
    if (!getResultSetTextContents(mListResultSet->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED),
        COL_TYPE_PLAYER_ID, value))
    {
        return;
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    GMGetPlayerDataReq req;
    req.mPlayerGuid = lynxAtoi<Guid>(value.c_str());
    gmClient->sendMsg(req);
}

void 
PlayerPanel::onClearResult(wxCommandEvent& ev)
{
    clearResultSet(true);
}

void 
PlayerPanel::onResetLayout(wxCommandEvent& ev)
{
    resetLayout();
}

void 
PlayerPanel::onCopyPlayerId(wxCommandEvent& ev)
{
    if (!mListResultSet)
    {
        return;
    }

    std::string value;
    if (!getResultSetTextContents(mListResultSet->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED),
        COL_TYPE_PLAYER_ID, value))
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
PlayerPanel::onCopyPlayerName(wxCommandEvent& ev)
{
    if (!mListResultSet)
    {
        return;
    }

    std::string value;
    if (!getResultSetTextContents(mListResultSet->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED),
        COL_TYPE_PLAYER_NAME, value))
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
PlayerPanel::onCopyAccountId(wxCommandEvent& ev)
{
    if (!mListResultSet)
    {
        return;
    }

    std::string value;
    if (!getResultSetTextContents(mListResultSet->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED),
        COL_TYPE_PLAYER_ACCOUNT_ID, value))
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
PlayerPanel::onCopyAccountName(wxCommandEvent& ev)
{
    if (!mListResultSet)
    {
        return;
    }

    std::string value;
    if (!getResultSetTextContents(mListResultSet->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED),
        COL_TYPE_PLAYER_ACCOUNT_NAME, value))
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
PlayerPanel::onFindByPlayerId(wxCommandEvent& ev)
{
    if (!mTextCtrlPlayerGuid)
    {
        return;
    }

    wxString guidText = mTextCtrlPlayerGuid->GetValue();
    if (guidText.IsEmpty())
    {
        wxMessageBox(wxT("玩家ID不能为空"), wxT(""));
        return;
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (gmClient)
    {
        GMGetPlayerByPlayerGuidReq req;
        req.mPlayerGuid = lynxAtoi<Guid>(guidText.c_str());
        gmClient->sendMsg(req);
    }
}

void 
PlayerPanel::onClearByPlayerId(wxCommandEvent& ev)
{
    mTextCtrlPlayerGuid->Clear();
}

void 
PlayerPanel::onFindByPlayerName(wxCommandEvent& ev)
{
    if (!mTextCtrlPlayerName)
    {
        return;
    }

    wxString nameText = mTextCtrlPlayerName->GetValue();
    if (nameText.IsEmpty())
    {
        wxMessageBox(wxT("玩家名字不能为空"), wxT(""));
        return;
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (gmClient)
    {
        GMGetPlayerByPlayerNameReq req;
        req.mPlayerName = nameText.c_str();
        gmClient->sendMsg(req);
    }
}

void 
PlayerPanel::onClearByPlayerName(wxCommandEvent& ev)
{
    mTextCtrlPlayerName->Clear();
}

void 
PlayerPanel::onFindByAccountId(wxCommandEvent& ev)
{
    if (!mTextCtrlAccountGuid)
    {
        return;
    }

    wxString guidText = mTextCtrlAccountGuid->GetValue();
    if (guidText.IsEmpty())
    {
        wxMessageBox(wxT("账户ID不能为空"), wxT(""));
        return;
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (gmClient)
    {
        GMGetPlayerByAccountGuidReq req;
        req.mAccountGuid = lynxAtoi<Guid>(guidText.c_str());
        gmClient->sendMsg(req);
    }
}

void 
PlayerPanel::onClearByAccountId(wxCommandEvent& ev)
{
    mTextCtrlAccountGuid->Clear();
}

void 
PlayerPanel::onFindByAccountName(wxCommandEvent& ev)
{
    if (!mTextCtrlAccountName)
    {
        return;
    }

    wxString nameText = mTextCtrlAccountName->GetValue();
    if (nameText.IsEmpty())
    {
        wxMessageBox(wxT("账户名字不能为空"), wxT(""));
        return;
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (gmClient)
    {
        GMGetPlayerByAccountNameReq req;
        req.mAccountName = nameText.c_str();
        gmClient->sendMsg(req);
    }
}

void 
PlayerPanel::onClearByAccountName(wxCommandEvent& ev)
{
    mTextCtrlAccountName->Clear();
}

void 
PlayerPanel::onShowOnlinePlayerList(wxCommandEvent& ev)
{
    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (gmClient)
    {
        GMGetPlayerByOnlineReq req;
        gmClient->sendMsg(req);
    }
}

wxListCtrl*
PlayerPanel::createSearchResultListCtrl()
{
    wxListCtrl* listCtrl = new wxListCtrl(this, myID_PLAYER_PANEL_SEARCH_RESULT, 
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
PlayerPanel::resetLayout()
{
    clearPlayerGuid();
    clearPlayerName();
    clearAccountGuid();
    clearAccountName();
    clearResultSet();
}

void 
PlayerPanel::clearPlayerGuid()
{
    if (mTextCtrlPlayerGuid)
    {
        mTextCtrlPlayerGuid->Clear();
    }
}

void 
PlayerPanel::clearPlayerName()
{
    if (mTextCtrlPlayerName)
    {
        mTextCtrlPlayerName->Clear();
    }
}

void 
PlayerPanel::clearAccountGuid()
{
    if (mTextCtrlAccountGuid)
    {
        mTextCtrlAccountGuid->Clear();
    }
}

void 
PlayerPanel::clearAccountName()
{
    if (mTextCtrlAccountName)
    {
        mTextCtrlAccountName->Clear();
    }
}

void 
PlayerPanel::clearResultSet(bool isClearTips)
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

    if (mResultBox)
    {
        mResultBox->SetLabel(STATIC_BOX_PLAYER_RESULT_LABEL(0));
    }
}

void 
PlayerPanel::onGetPlayerByOnlineEvent(const GetPlayerByOnlineEvent& ev)
{
    if (mListResultSet)
    {
        mListResultSet->DeleteAllItems();
    }

    if (ev.mErrno != LynxErrno::None)
    {
        if (mTextResultTips)
        {
            mTextResultTips->SetLabelText("失败");
            mTextResultTips->SetOwnForegroundColour(wxT("red"));
        }
    }
    else
    {
        if (mTextResultTips)
        {
            mTextResultTips->SetLabelText("成功");
            mTextResultTips->SetOwnForegroundColour(wxT("green"));
        }
    }

    

    const List<PlayerBaseData>& playerList = ev.mPlayerList;

    if (mResultBox)
    {
        mResultBox->SetLabel(STATIC_BOX_PLAYER_RESULT_LABEL(playerList.size()));
    }

    for (List<PlayerBaseData>::ConstIter* iter = playerList.begin();
        iter != playerList.end(); iter = playerList.next(iter))
    {
        const PlayerBaseData& data = iter->mValue;
        long index = mListResultSet->InsertItem(mListResultSet->GetItemCount(), wxT(""));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ID, wxString::Format("%llu", data.mPlayerId));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_NAME, data.mPlayerName.c_str());
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_LEVEL, wxString::Format("%u", data.mLevel));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ACCOUNT_ID, wxString::Format("%llu", data.mAccountGuid));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ACCOUNT_NAME, data.mAccountName.c_str());
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ONLINE, "Yes");
    }
}

void 
PlayerPanel::onGetPlayerByPlayerGuidEvent(const GetPlayerByPlayerGuidEvent& ev)
{
    if (mListResultSet)
    {
        mListResultSet->DeleteAllItems();
    }

    if (ev.mErrno != LynxErrno::None)
    {
        if (mTextResultTips)
        {
            mTextResultTips->SetLabelText("失败");
            mTextResultTips->SetOwnForegroundColour(wxT("red"));
        }
    }
    else
    {
        if (mTextResultTips)
        {
            mTextResultTips->SetLabelText("成功");
            mTextResultTips->SetOwnForegroundColour(wxT("green"));
        }

        GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
        if (!gmClient)
        {
            return;
        }


        const PlayerBaseData& data = gmClient->getPlayerData().mBaseData;

        if (mResultBox)
        {
            mResultBox->SetLabel(STATIC_BOX_PLAYER_RESULT_LABEL(1));
        }

        long index = mListResultSet->InsertItem(mListResultSet->GetItemCount(), wxT(""));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ID, wxString::Format("%llu", data.mPlayerId));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_NAME, data.mPlayerName.c_str());
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_LEVEL, wxString::Format("%u", data.mLevel));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ACCOUNT_ID, wxString::Format("%llu", data.mAccountGuid));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ACCOUNT_NAME, data.mAccountName.c_str());
        if (ev.mOffline)
        {
            mListResultSet->SetItem(index, COL_TYPE_PLAYER_ONLINE, "No");
        }
        else
        {
            mListResultSet->SetItem(index, COL_TYPE_PLAYER_ONLINE, "Yes");
        }
    }
}

void 
PlayerPanel::onGetPlayerByPlayerNameEvent(const GetPlayerByPlayerNameEvent& ev)
{
    if (mListResultSet)
    {
        mListResultSet->DeleteAllItems();
    }

    if (ev.mErrno != LynxErrno::None)
    {
        if (mTextResultTips)
        {
            mTextResultTips->SetLabelText("失败");
            mTextResultTips->SetOwnForegroundColour(wxT("red"));
        }
    }
    else
    {
        if (mTextResultTips)
        {
            mTextResultTips->SetLabelText("成功");
            mTextResultTips->SetOwnForegroundColour(wxT("green"));
        }

        GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
        if (!gmClient)
        {
            return;
        }


        const PlayerBaseData& data = gmClient->getPlayerData().mBaseData;

        if (mResultBox)
        {
            mResultBox->SetLabel(STATIC_BOX_PLAYER_RESULT_LABEL(1));
        }

        long index = mListResultSet->InsertItem(mListResultSet->GetItemCount(), wxT(""));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ID, wxString::Format("%llu", data.mPlayerId));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_NAME, data.mPlayerName.c_str());
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_LEVEL, wxString::Format("%u", data.mLevel));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ACCOUNT_ID, wxString::Format("%llu", data.mAccountGuid));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ACCOUNT_NAME, data.mAccountName.c_str());
        if (ev.mOffline)
        {
            mListResultSet->SetItem(index, COL_TYPE_PLAYER_ONLINE, "No");
        }
        else
        {
            mListResultSet->SetItem(index, COL_TYPE_PLAYER_ONLINE, "Yes");
        }
    }
}

void 
PlayerPanel::onGetPlayerByAccountGuidEvent(const GetPlayerByAccountGuidEvent& ev)
{
    if (mListResultSet)
    {
        mListResultSet->DeleteAllItems();
    }

    if (ev.mErrno != LynxErrno::None)
    {
        if (mTextResultTips)
        {
            mTextResultTips->SetLabelText("失败");
            mTextResultTips->SetOwnForegroundColour(wxT("red"));
        }
    }
    else
    {
        if (mTextResultTips)
        {
            mTextResultTips->SetLabelText("成功");
            mTextResultTips->SetOwnForegroundColour(wxT("green"));
        }

        GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
        if (!gmClient)
        {
            return;
        }


        const PlayerBaseData& data = gmClient->getPlayerData().mBaseData;

        if (mResultBox)
        {
            mResultBox->SetLabel(STATIC_BOX_PLAYER_RESULT_LABEL(1));
        }

        long index = mListResultSet->InsertItem(mListResultSet->GetItemCount(), wxT(""));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ID, wxString::Format("%llu", data.mPlayerId));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_NAME, data.mPlayerName.c_str());
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_LEVEL, wxString::Format("%u", data.mLevel));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ACCOUNT_ID, wxString::Format("%llu", data.mAccountGuid));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ACCOUNT_NAME, data.mAccountName.c_str());
        if (ev.mOffline)
        {
            mListResultSet->SetItem(index, COL_TYPE_PLAYER_ONLINE, "No");
        }
        else
        {
            mListResultSet->SetItem(index, COL_TYPE_PLAYER_ONLINE, "Yes");
        }
    }
}

void 
PlayerPanel::onGetPlayerByAccountNameEvent(const GetPlayerByAccountNameEvent& ev)
{
    if (mListResultSet)
    {
        mListResultSet->DeleteAllItems();
    }

    if (ev.mErrno != LynxErrno::None)
    {
        if (mTextResultTips)
        {
            mTextResultTips->SetLabelText("失败");
            mTextResultTips->SetOwnForegroundColour(wxT("red"));
        }
    }
    else
    {
        if (mTextResultTips)
        {
            mTextResultTips->SetLabelText("成功");
            mTextResultTips->SetOwnForegroundColour(wxT("green"));
        }

        GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
        if (!gmClient)
        {
            return;
        }


        const PlayerBaseData& data = gmClient->getPlayerData().mBaseData;

        if (mResultBox)
        {
            mResultBox->SetLabel(STATIC_BOX_PLAYER_RESULT_LABEL(1));
        }

        long index = mListResultSet->InsertItem(mListResultSet->GetItemCount(), wxT(""));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ID, wxString::Format("%llu", data.mPlayerId));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_NAME, data.mPlayerName.c_str());
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_LEVEL, wxString::Format("%u", data.mLevel));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ACCOUNT_ID, wxString::Format("%llu", data.mAccountGuid));
        mListResultSet->SetItem(index, COL_TYPE_PLAYER_ACCOUNT_NAME, data.mAccountName.c_str());
        if (ev.mOffline)
        {
            mListResultSet->SetItem(index, COL_TYPE_PLAYER_ONLINE, "No");
        }
        else
        {
            mListResultSet->SetItem(index, COL_TYPE_PLAYER_ONLINE, "Yes");
        }
    }
}

void 
PlayerPanel::onGetPlayerDataEvent(const GetPlayerDataEvent& ev)
{
    if (ev.mErrno == LynxErrno::None)
    {
        MAIN_FRAME().getPlayerInfoDialog().resetLayout();
        MAIN_FRAME().getPlayerInfoDialog().ShowModal();
    }
    else
    {
        wxMessageBox(wxT("获得玩家信息失败"), wxT(""));
    }
}

bool 
PlayerPanel::getResultSetTextContents(long index, ColType colType, std::string& res)
{
    res.clear();

    if (!mListResultSet)
    {
        return false;
    }

    if (COL_TYPE_MAX_COUNT <= colType || index < 0)
    {
        return false;
    }

    wxListItem info;
    info.m_itemId = index;
    info.m_col = colType;
    info.m_mask = wxLIST_MASK_TEXT;

    if (!mListResultSet->GetItem(info))
    {
        return false;
    }

    res = info.GetText().c_str();
    return true;
}

