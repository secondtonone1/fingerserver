#include "ForbiddenPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(ForbiddenPanel, wxListCtrl)

BEGIN_EVENT_TABLE(ForbiddenPanel, wxListCtrl)
EVT_LIST_ITEM_SELECTED(myID_FORBIDDEN_PANEL, onSelected)
EVT_LIST_ITEM_ACTIVATED(myID_FORBIDDEN_PANEL, onActived)
EVT_LIST_ITEM_RIGHT_CLICK(myID_FORBIDDEN_PANEL, onRightClicked)
EVT_LIST_COL_CLICK(myID_FORBIDDEN_PANEL, onColClicked)
END_EVENT_TABLE()

ForbiddenPanel::ForbiddenPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxListCtrl(parent, id, pos, size, wxLC_REPORT | wxLC_SINGLE_SEL)
{
    mOperateMenu = NULL;
    memset(mSortFlag, 0, sizeof(mSortFlag));
}

ForbiddenPanel::~ForbiddenPanel()
{
    release();
}

bool
ForbiddenPanel::initial()
{
    for (int index = 0; index < COL_TYPE_MAX_COUNT; ++index)
    {
        wxListItem itemCol;
        itemCol.SetText(getColTypeText(index));
        itemCol.SetWidth(140);
        InsertColumn(index, itemCol);
    }

    mOperateMenu = new wxMenu(wxT(""));
    mOperateMenu->Append(myID_PLAYER_MENU_COPY_ID, wxT("拷贝玩家ID"));
    mOperateMenu->Append(myID_PLAYER_MENU_COPY_NAME, wxT("拷贝玩家名称"));
    mOperateMenu->Append(myID_PLAYER_MENU_COPY_ACCOUNT_ID, wxT("拷贝账户ID"));
    mOperateMenu->Append(myID_PLAYER_MENU_COPY_ACCOUNT_NAME, wxT("拷贝账户名称"));

    mOperateMenu->AppendSeparator();
    mOperateMenu->Append(myID_PLAYER_MENU_FORBIDDEN_CHAT, wxT("解除禁止"));

    // 临时插入测试数据
    long index = InsertItem(GetItemCount(), wxT(""));
    SetItemPtrData(index, 123456789);
    SetItem(index, COL_TYPE_FORBIDDEN_TYPE, "禁止发言");
    SetItem(index, COL_TYPE_FORBIDDEN_START_TIME, "2014-11-23 20:29:25");
    SetItem(index, COL_TYPE_FORBIDDEN_END_TIME, "2014-11-24 00:00:00");
    SetItem(index, COL_TYPE_FORBIDDEN_ID, wxString::Format("%u", 2233344));
    SetItem(index, COL_TYPE_FORBIDDEN_NAME, "abcdefg");
    SetItem(index, COL_TYPE_FORBIDDEN_ACCOUNT_ID, wxString::Format("%u", 2233344));
    SetItem(index, COL_TYPE_FORBIDDEN_ACCOUNT_NAME, "abcdefg");

    index = InsertItem(GetItemCount(), wxT(""));
    SetItemPtrData(index, 123456789);
    SetItem(index, COL_TYPE_FORBIDDEN_TYPE, "禁止登录");
    SetItem(index, COL_TYPE_FORBIDDEN_START_TIME, "2014-11-23 20:29:25");
    SetItem(index, COL_TYPE_FORBIDDEN_END_TIME, "2014-11-24 00:00:00");
    SetItem(index, COL_TYPE_FORBIDDEN_ID, wxString::Format("%u", 1234567));
    SetItem(index, COL_TYPE_FORBIDDEN_NAME, "abcdefg");
    SetItem(index, COL_TYPE_FORBIDDEN_ACCOUNT_ID, wxString::Format("%u", 341244));
    SetItem(index, COL_TYPE_FORBIDDEN_ACCOUNT_NAME, "abcdefg");

    registerEvents();
    return true;
}

void 
ForbiddenPanel::release()
{
    if (mOperateMenu)
    {
        delete mOperateMenu;
        mOperateMenu = NULL;
    }

    deregisterEvents();
}

void
ForbiddenPanel::registerEvents()
{

}

void
ForbiddenPanel::deregisterEvents()
{

}

void
ForbiddenPanel::resetLayout()
{
    DeleteAllItems();
}

wxString
ForbiddenPanel::getColTypeText(int colType)
{
    switch (colType)
    {
    case COL_TYPE_FORBIDDEN_TYPE:
        return wxString(wxT("禁止类型"));
    case COL_TYPE_FORBIDDEN_START_TIME:
        return wxString(wxT("禁止开始时间"));
    case COL_TYPE_FORBIDDEN_END_TIME:
        return wxString(wxT("禁止结束时间"));
    case COL_TYPE_FORBIDDEN_ID:
        return wxString(wxT("玩家ID"));
    case COL_TYPE_FORBIDDEN_NAME:
        return wxString(wxT("玩家名称"));
    case COL_TYPE_FORBIDDEN_ACCOUNT_ID:
        return wxString(wxT("账户ID"));
    case COL_TYPE_FORBIDDEN_ACCOUNT_NAME:
        return wxString(wxT("账户名称"));
    }

    return wxString(wxT("未知"));
}

void 
ForbiddenPanel::onSelected(wxListEvent& ev)
{

}

void 
ForbiddenPanel::onActived(wxListEvent& ev)
{

}

void 
ForbiddenPanel::onRightClicked(wxListEvent& ev)
{
    if (!mOperateMenu)
    {
        return;
    }

    if ((GetWindowStyle() & wxLC_REPORT) && GetSelectedItemCount() == 1)
    {
        wxPoint pt = ev.GetPoint();
        PopupMenu(mOperateMenu, pt);
    }
}

void 
ForbiddenPanel::onColClicked(wxListEvent& ev)
{
    int col = ev.GetColumn();

    if (col < 0 || col >= COL_TYPE_MAX_COUNT)
    {
        return;
    }
}
