#include "NoticePanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

#define STATIC_BOX_NOTICE_RESULT_LABEL(count) (wxString::Format("查询结果 公告(%u)", count))

IMPLEMENT_CLASS(NoticePanel, wxPanel)

BEGIN_EVENT_TABLE(NoticePanel, wxPanel)
EVT_BUTTON(myID_NOTICE_PANEL_BTN_PUBLISH, onPublishClicked)
END_EVENT_TABLE()

NoticePanel::NoticePanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    mTextCtrlContent = NULL;
    mTextCtrlStartTime = NULL;
    mTextCtrlCycleCount = NULL;
    mBtnPublish = NULL;
    mBtnSearch = NULL;
    mListSearchResult = NULL;
    mTextResultTips = NULL;
    mResultOperateMenu = NULL;

    memset(mSortFlag, 0, sizeof(mSortFlag));
}

NoticePanel::~NoticePanel()
{
    release();
}

bool
NoticePanel::initial()
{
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(topSizer);

    
    // 发布
    wxStaticBox* publishBox = new wxStaticBox(this, wxID_ANY, wxT("发布"));
    wxStaticBoxSizer* publishBoxSizer = new wxStaticBoxSizer(publishBox, wxVERTICAL);
    topSizer->Add(publishBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    wxBoxSizer* contentBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    publishBoxSizer->Add(contentBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    wxStaticText* contentStaticText = new wxStaticText(this, wxID_STATIC, wxString::Format("内容（%u字以内)", NOTICE_CONTENT_SIZE_LIMIT));
    mTextCtrlContent = new wxTextCtrl(this, myID_NOTICE_PANEL_TEXT_CTRL_CONTENT);
    mTextCtrlContent->SetMaxLength(NOTICE_CONTENT_SIZE_LIMIT);
    contentBoxSizer->Add(contentStaticText, 0, wxALL|wxALIGN_CENTRE, 5);
    contentBoxSizer->Add(mTextCtrlContent, 1, wxALL|wxALIGN_CENTRE, 5);

    wxBoxSizer* tmpSizer = new wxBoxSizer(wxHORIZONTAL);
    publishBoxSizer->Add(tmpSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    //wxBoxSizer* startTimeBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    //publishBoxSizer->Add(startTimeBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    wxStaticText* startTimeStaticText = new wxStaticText(this, wxID_STATIC, wxT("开始时间"));
    mTextCtrlStartTime = new wxTextCtrl(this, myID_NOTICE_PANEL_TEXT_CTRL_START_TIME);
    tmpSizer->Add(startTimeStaticText, 0, wxALL|wxALIGN_CENTRE, 5);
    tmpSizer->Add(mTextCtrlStartTime, 1, wxALL|wxALIGN_CENTRE, 5);

    //wxBoxSizer* cycleCountBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    //publishBoxSizer->Add(cycleCountBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    wxStaticText* cycleCountStaticText = new wxStaticText(this, wxID_STATIC, wxT("循环次数"));
    mTextCtrlCycleCount = new wxTextCtrl(this, myID_NOTICE_PANEL_TEXT_CTRL_CYCLE_COUNT);
    tmpSizer->Add(cycleCountStaticText, 0, wxALL|wxALIGN_CENTRE, 5);
    tmpSizer->Add(mTextCtrlCycleCount, 1, wxALL|wxALIGN_CENTRE, 5);

    // 操作
    wxStaticBox* uiOperatorBox = new wxStaticBox(this, wxID_ANY, wxT("操作"));
    wxStaticBoxSizer* uiOperatorBoxSizer = new wxStaticBoxSizer(uiOperatorBox, wxHORIZONTAL);
    topSizer->Add(uiOperatorBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    mBtnPublish = new wxButton(this, myID_NOTICE_PANEL_BTN_PUBLISH, wxT("发布"));
    mBtnSearch = new wxButton(this, myID_NOTICE_PANEL_BTN_SEARCH, wxT("查询"));

    uiOperatorBoxSizer->Add(mBtnPublish, 0, wxALL|wxALIGN_CENTRE, 5);
    uiOperatorBoxSizer->Add(mBtnSearch, 0, wxALL|wxALIGN_CENTRE, 5);


    // 查询结果
    wxStaticBox* searchBox = new wxStaticBox(this, wxID_ANY, STATIC_BOX_NOTICE_RESULT_LABEL(0));
    wxStaticBoxSizer* searchBoxSizer = new wxStaticBoxSizer(searchBox, wxVERTICAL);
    topSizer->Add(searchBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    wxBoxSizer* tipsSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* stateStaticText = new wxStaticText(this, wxID_STATIC, wxT("状态："));
    stateStaticText->SetOwnForegroundColour(wxColour(wxT("blue")));
    tipsSizer->Add(stateStaticText, 0, wxALIGN_LEFT, 0);
    mTextResultTips = new wxStaticText(this, wxID_STATIC, wxT("无效"));
    mTextResultTips->SetOwnForegroundColour(wxColour(wxT("grey")));
    tipsSizer->Add(mTextResultTips, 1, wxGROW|wxALL|wxALIGN_CENTRE, 0);

    mListSearchResult = createSearchResultListCtrl();

    searchBoxSizer->Add(tipsSizer, 0, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    searchBoxSizer->Add(mListSearchResult, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);

    // 重置UI
    resetLayout();

    topSizer->Fit(this);
    topSizer->SetSizeHints(this);

    registerEvents();
    return true;
}

void
NoticePanel::release()
{

    deregisterEvents();
}

void
NoticePanel::registerEvents()
{
    
}

void
NoticePanel::deregisterEvents()
{

}

wxListCtrl* 
NoticePanel::createSearchResultListCtrl()
{
    wxListCtrl* listCtrl = new wxListCtrl(this, myID_NOTICE_PANEL_LIST_CTRL_RESULT, 
        wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);

    // 生成布局
    for (int index = 0; index < RESULT_COL_TYPE_MAX_COUNT; index++)
    {
        int width = (index == RESULT_COL_TYPE_CONTENT ? 500 : 150);
        wxListItem itemCol;
        itemCol.SetText(getResultColTypeText(index)); 
        itemCol.SetWidth(width);
        listCtrl->InsertColumn(index, itemCol);
    }
    return listCtrl;
}

wxString
NoticePanel::getResultColTypeText(int resultColType)
{
    switch(resultColType)
    {
    case RESULT_COL_TYPE_ID:
        return wxString(wxT("ID"));
    case RESULT_COL_TYPE_START_TIME:
        return wxString(wxT("开始时间"));
    case RESULT_COL_TYPE_CYCLE_COUNT:
        return wxString(wxT("循环次数"));
    case RESULT_COL_TYPE_CONTENT:
        return wxString(wxT("公告内容"));
    }
    return wxString(wxT("未知"));
}

void 
NoticePanel::resetLayout()
{
    if (mTextCtrlContent)
    {
        mTextCtrlContent->Clear();
    }

    if (mTextCtrlStartTime)
    {
        mTextCtrlStartTime->SetValue(wxDateTime::Now().FormatISOCombined(' '));
    }

    if (mTextCtrlCycleCount)
    {
        mTextCtrlCycleCount->SetValue(wxT("1"));
    }

    if (mListSearchResult)
    {
        mListSearchResult->DeleteAllItems();
    }

    if (mTextResultTips)
    {
        mTextResultTips->SetLabelText(wxT("无效"));
        mTextResultTips->SetOwnForegroundColour(wxColour(wxT("grey")));
    }
}

void 
NoticePanel::onPublishClicked(wxCommandEvent& ev)
{
    wxString contents = mTextCtrlContent->GetValue();
    wxString cycleCount = mTextCtrlCycleCount->GetValue();
    wxString startTime = mTextCtrlStartTime->GetValue();

    GMSendSystemNoticeReq req;
    req.mContents = contents.c_str();
    req.mCycleCount = lynxAtoi<UInt32>(cycleCount.c_str());

    if (req.mCycleCount == 0 || req.mCycleCount > 3)
    {
        wxMessageBox(wxT("循环次数不能为0或者大于3"), wxT("发送公告"), wxOK);
        return;
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }
    gmClient->sendMsg(req);
}

