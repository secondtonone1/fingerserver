#include "EmailPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

#define STATIC_BOX_RECV_NAME_LABEL(currCount, maxCount) (wxString::Format("收件人(%u/%u)", currCount, maxCount))
#define STATIC_BOX_CONTENTS_LABEL(currCount, maxCount) (wxString::Format("内容(%u/%u)", currCount, maxCount))
#define STATIC_BOX_ITEM_LABEL(currCount, maxCount) (wxString::Format("道具(%u/%u)", currCount, maxCount))

IMPLEMENT_CLASS(EmailPanel, wxPanel)

BEGIN_EVENT_TABLE(EmailPanel, wxPanel)
EVT_LIST_ITEM_RIGHT_CLICK(myID_EMAIL_PANEL_LIST_CTRL_ITEM, onRightClicked)
END_EVENT_TABLE()

EmailPanel::EmailPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
     mStaticBoxRecvName = NULL;
     mTextCtrlRecvName = NULL;
     mStaticBoxContents = NULL;
     mTextCtrlContents = NULL;
     mStaticBoxItem = NULL;
     mBtnAddItem = NULL;
     mBtnDelAllItem = NULL;
     mListCtrlItem = NULL;
     mTextCtrlItemTemplateId = NULL;
     mTextCtrlItemCount = NULL;
     mTextCtrlRecord = NULL;
     mBtnSendEmail = NULL;
     mBtnClearRecord = NULL;
     mBtnReset = NULL;
     mOperateMenu = NULL;
}

EmailPanel::~EmailPanel()
{
    release();
}

bool
EmailPanel::initial()
{
    mOperateMenu = new wxMenu(wxT(""));
    mOperateMenu->Append(myID_EMAIL_PANEL_MENU_DEL_ITEM, wxT("删除道具"));

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(topSizer);

    // 收件人
    mStaticBoxRecvName = new wxStaticBox(this, wxID_ANY, STATIC_BOX_RECV_NAME_LABEL(0, EMAIL_MULTICAST_MAX_COUNT));
    wxStaticBoxSizer* recvNameBoxSizer = new wxStaticBoxSizer(mStaticBoxRecvName, wxHORIZONTAL);
    topSizer->Add(recvNameBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    mTextCtrlRecvName = new wxTextCtrl(this, myID_EMAIL_PANEL_TEXT_CTRL_RECV_NAME, wxEmptyString,
        wxDefaultPosition, wxSize(600, 70), wxTE_MULTILINE | wxTE_WORDWRAP);
    recvNameBoxSizer->Add(mTextCtrlRecvName, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);

    // 内容
    mStaticBoxContents = new wxStaticBox(this, wxID_ANY, STATIC_BOX_CONTENTS_LABEL(0, EMAIL_CONTENT_MAX_LENGTH));
    wxStaticBoxSizer* contentsSizer = new wxStaticBoxSizer(mStaticBoxContents, wxHORIZONTAL);
    topSizer->Add(contentsSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    mTextCtrlContents = new wxTextCtrl(this, myID_EMAIL_PANEL_TEXT_CTRL_CONTENTS, wxEmptyString,
        wxDefaultPosition, wxSize(600, 70), wxTE_MULTILINE | wxTE_WORDWRAP);
    mTextCtrlContents->SetMaxLength(EMAIL_CONTENT_MAX_LENGTH);
    contentsSizer->Add(mTextCtrlContents, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);

    // 道具
    mStaticBoxItem = new wxStaticBox(this, wxID_ANY, STATIC_BOX_ITEM_LABEL(0, EMAIL_ITEM_MAX_COUNT));
    wxStaticBoxSizer* itemSizer = new wxStaticBoxSizer(mStaticBoxItem, wxVERTICAL);
    topSizer->Add(itemSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    wxBoxSizer* itemListCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
    itemSizer->Add(itemListCtrlSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    mListCtrlItem = createItemListCtrl();
    itemListCtrlSizer->Add(mListCtrlItem, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    
    wxBoxSizer* itemBtnSizer = new wxBoxSizer(wxHORIZONTAL);
    itemSizer->Add(itemBtnSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 0);
    wxStaticText* itemTemplateIdText = new wxStaticText(this, wxID_STATIC, wxT("道具模板ID"));
    mTextCtrlItemTemplateId = new wxTextCtrl(this, myID_EMAIL_DIALOG_TEXT_CTRL_ITEM_TEMPLATE_ID);
    wxStaticText* itemCountText = new wxStaticText(this, wxID_STATIC, wxT("道具数量"));
    mTextCtrlItemCount = new wxTextCtrl(this, myID_EMIAL_DIALOG_TEXT_CTRL_ITEM_COUNT);
    mBtnAddItem = new wxButton(this, myID_EMAIL_PANEL_ADD_ITEM, wxT("添加道具"));
    mBtnDelAllItem = new wxButton(this, myID_EMAIL_PANEL_DEL_ALL_ITEM, wxT("删除所有"));

    mTextCtrlItemTemplateId->SetMaxLength(20);
    mTextCtrlItemCount->SetMaxLength(3);
    mBtnAddItem->Disable();
    mBtnDelAllItem->Disable();

    itemBtnSizer->Add(itemTemplateIdText, 0, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    itemBtnSizer->Add(mTextCtrlItemTemplateId, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    itemBtnSizer->Add(itemCountText, 0, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    itemBtnSizer->Add(mTextCtrlItemCount, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    itemBtnSizer->Add(mBtnAddItem, 0, wxGROW|wxALL|wxALIGN_CENTRE, 5);
    itemBtnSizer->Add(mBtnDelAllItem, 0, wxGROW|wxALL|wxALIGN_CENTRE, 5);

    // 记录
    wxStaticBox* recordStaticBox = new wxStaticBox(this, wxID_ANY, wxT("记录"));
    wxStaticBoxSizer* recordStaticBoxSizer = new wxStaticBoxSizer(recordStaticBox, wxHORIZONTAL);
    topSizer->Add(recordStaticBoxSizer, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    mTextCtrlRecord = new wxTextCtrl(this, myID_EMAIL_PANEL_TEXT_CTRL_RECORD, wxEmptyString,
        wxDefaultPosition, wxSize(600, 70), wxTR_DEFAULT_STYLE | wxNO_BORDER | wxTE_MULTILINE |
        wxTE_READONLY | wxTE_RICH);
    //mTextCtrlRecord->SetBackgroundColour(*wxBLACK);
    //mTextCtrlRecord->SetForegroundColour(*wxWHITE);
    recordStaticBoxSizer->Add(mTextCtrlRecord, 1, wxGROW|wxALL|wxALIGN_CENTRE, 5);

    // 按钮
    wxBoxSizer* btnBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(btnBoxSizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5);

    // 发送按钮
    mBtnSendEmail = new wxButton(this, myID_EMAIL_PANEL_SEND_EMAIL, wxT("发送邮件"));
    btnBoxSizer->Add(mBtnSendEmail, 0, wxALL|wxALIGN_CENTRE, 5);
    // 重置UI按钮
    mBtnReset = new wxButton(this, myID_EMAIL_PANEL_RESET, wxT("重置"));
    btnBoxSizer->Add(mBtnReset, 0, wxALL|wxALIGN_CENTRE, 5);
    // 清空记录按钮
    mBtnClearRecord = new wxButton(this, myID_EMAIL_PANEL_CLEAR_RECORD, wxT("清空记录"));
    btnBoxSizer->Add(mBtnClearRecord, 0, wxALL|wxALIGN_CENTRE, 5);

    topSizer->Fit(this);
    topSizer->SetSizeHints(this);

    // 临时插入测试数据
    long index = mListCtrlItem->InsertItem(mListCtrlItem->GetItemCount(), wxT(""));
    mListCtrlItem->SetItemPtrData(index, 123456789);
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_TEMPLATE_ID, wxString::Format("%u", 123456789));
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_ITEM_COUNT, wxString::Format("%u", 1));

    index = mListCtrlItem->InsertItem(mListCtrlItem->GetItemCount(), wxT(""));
    mListCtrlItem->SetItemPtrData(index, 123456789);
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_TEMPLATE_ID, wxString::Format("%u", 123456789));
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_ITEM_COUNT, wxString::Format("%u", 1));

    index = mListCtrlItem->InsertItem(mListCtrlItem->GetItemCount(), wxT(""));
    mListCtrlItem->SetItemPtrData(index, 123456789);
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_TEMPLATE_ID, wxString::Format("%u", 123456789));
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_ITEM_COUNT, wxString::Format("%u", 1));

    index = mListCtrlItem->InsertItem(mListCtrlItem->GetItemCount(), wxT(""));
    mListCtrlItem->SetItemPtrData(index, 123456789);
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_TEMPLATE_ID, wxString::Format("%u", 123456789));
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_ITEM_COUNT, wxString::Format("%u", 1));

    index = mListCtrlItem->InsertItem(mListCtrlItem->GetItemCount(), wxT(""));
    mListCtrlItem->SetItemPtrData(index, 123456789);
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_TEMPLATE_ID, wxString::Format("%u", 123456789));
    mListCtrlItem->SetItem(index, ITEM_COL_TYPE_ITEM_COUNT, wxString::Format("%u", 1));


    registerEvents();
    return true;
}

void
EmailPanel::release()
{
    if (mOperateMenu)
    {
        delete mOperateMenu;
        mOperateMenu = NULL;
    }
    
    deregisterEvents();
}

void
EmailPanel::registerEvents()
{

}

void
EmailPanel::deregisterEvents()
{

}

wxListCtrl*
EmailPanel::createItemListCtrl()
{
    wxListCtrl* listCtrl = new wxListCtrl(this, myID_EMAIL_PANEL_LIST_CTRL_ITEM, wxDefaultPosition, wxSize(600, 130), wxLC_REPORT);

    // 生成布局
    for (int index = 0; index < ITEM_COL_TYPE_MAX_COUNT; index++)
    {
        wxListItem itemCol;
        itemCol.SetText(getItemColTypeText(index));
        itemCol.SetWidth(300);
        listCtrl->InsertColumn(index, itemCol);
    }
    return listCtrl;
}

wxString 
EmailPanel::getItemColTypeText(int itemColType)
{
    switch (itemColType)
    {
    case ITEM_COL_TYPE_TEMPLATE_ID:
        return wxString(wxT("模板ID"));
    case ITEM_COL_TYPE_ITEM_COUNT:
        return wxString(wxT("数量"));
    }

    return wxString(wxT("未知"));
}

void 
EmailPanel::onRightClicked(wxListEvent& ev)
{
    if (!mOperateMenu)
    {
        return;
    }

    if (!mListCtrlItem)
    {
        return;
    }

    if ((mListCtrlItem->GetWindowStyle() & wxLC_REPORT) && mListCtrlItem->GetSelectedItemCount() == 1)
    {
        wxPoint pt = ev.GetPoint();
        pt.y += mListCtrlItem->GetPosition().y;
        PopupMenu(mOperateMenu, pt);
    }
}

void 
EmailPanel::resetLayout()
{
    if (mTextCtrlRecvName)
    {
        mTextCtrlRecvName->Clear();
    }

    if (mTextCtrlContents)
    {
        mTextCtrlContents->Clear();
    }

    if (mListCtrlItem)
    {
        mListCtrlItem->DeleteAllItems();
    }

    if (mTextCtrlItemTemplateId)
    {
        mTextCtrlItemTemplateId->Clear();
    }

    if (mTextCtrlItemCount)
    {
        mTextCtrlItemCount->Clear();
    }

    if (mTextCtrlRecord)
    {
        mTextCtrlRecord->Clear();
    }

}

