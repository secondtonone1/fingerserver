#include "ServerPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(ServerPanel, wxTreeCtrl)

BEGIN_EVENT_TABLE(ServerPanel, wxTreeCtrl)
EVT_TREE_ITEM_ACTIVATED(myID_SERVER_PANEL, onTreeItemActivated)
EVT_TREE_ITEM_RIGHT_CLICK(myID_SERVER_PANEL, onTreeItemRClicked)
EVT_MENU(myID_SERVER_PANEL_ROOT_MENU_LOGIN_ALL, onLoginAll)
EVT_MENU(myID_SERVER_PANEL_ROOT_MENU_LOGOUT_ALL, onLogoutAll)
EVT_MENU(myID_SERVER_PANEL_MENU_LOGIN, onLogin)
EVT_MENU(myID_SERVER_PANEL_MENU_LOGOUT, onLogout)
END_EVENT_TABLE()

ServerPanel::ServerPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                         const wxSize& size, long style)
                         : wxTreeCtrl(parent, id, pos, size, style)
{

}

ServerPanel::~ServerPanel()
{
    release();
}

bool
ServerPanel::initial()
{
    wxImageList* imageList = new wxImageList(16, 16, true, 2);
    imageList->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16, 16)));
    imageList->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16)));

    AssignImageList(imageList);

    mServerRootMenu = new wxMenu(wxT(""));
    mServerRootMenu->Append(myID_SERVER_PANEL_ROOT_MENU_LOGIN_ALL, wxT("登录所有"));
    mServerRootMenu->Append(myID_SERVER_PANEL_ROOT_MENU_LOGOUT_ALL, wxT("登出所有"));

    mServerMenu = new wxMenu(wxT(""));
    mServerMenu->Append(myID_SERVER_PANEL_MENU_LOGIN, wxT("登录"));
    mServerMenu->Append(myID_SERVER_PANEL_MENU_LOGOUT, wxT("登出"));

    registerEvents();
    buildServerTree();

    return true;
}

void
ServerPanel::release()
{
    if (mServerRootMenu)
    {
        delete mServerRootMenu;
        mServerRootMenu = NULL;
    }

    if (mServerMenu)
    {
        delete mServerMenu;
        mServerMenu = NULL;
    }

    deregisterEvents();
}

void
ServerPanel::registerEvents()
{
    REGISTER_EVENT(UpdateServerPanelEvent, this, &ServerPanel::onUpdateServerPanelEvent);
}

void
ServerPanel::deregisterEvents()
{
    DEREGISTER_EVENT(UpdateServerPanelEvent, this, &ServerPanel::onUpdateServerPanelEvent);
}

void
ServerPanel::buildServerTree()
{
    DeleteAllItems();

    char buffer[1024] = { 0 };
    sprintf(buffer, "服务器列表[%u]", 0);

    mServerTreeItemId = AddRoot(buffer, 0);

    DeleteChildren(mServerTreeItemId);
    mActiveTreeItemId = NULL;

    GMClient* currentGMCilent = LogicSystem::getSingleton().getCurrentGMClient();

    const Map<String, GMClient*>& gmClientMap = LogicSystem::getSingleton().getGMClientMap();
    for (Map<String, GMClient*>::Iter* iter = gmClientMap.begin();
        iter != gmClientMap.end(); iter = gmClientMap.next(iter))
    {
        char buffer[1024] = { 0 };
        GMClient* gmClient = iter->mValue;
        if (gmClient)
        {
            if (gmClient->getLoginState() == GMClient::LOGINED_STATE)
            {
                sprintf(buffer, "%s[%d]", gmClient->getServerName().c_str(), gmClient->getPlayerCount());
            }
            else
            {
                sprintf(buffer, "%s[未登录]", gmClient->getServerName().c_str());
            }

            wxTreeItemId itemId = AppendItem(mServerTreeItemId, buffer, 1, -1);
            if (gmClient == currentGMCilent)
            {
                if (gmClient->getLoginState() == GMClient::LOGINED_STATE)
                {
                    mActiveTreeItemId = itemId;
                }
            }
        }
    }
    
    SortChildren(mServerTreeItemId);
    Expand(mServerTreeItemId);

    if (mActiveTreeItemId.IsOk())
    {
        SetItemTextColour(mActiveTreeItemId, *wxRED);
    }
}

void
ServerPanel::onTreeItemActivated(wxTreeEvent& ev)
{

}

void
ServerPanel::onTreeItemRClicked(wxTreeEvent &ev)
{
    wxTreeItemId itemId = ev.GetItem();

    if (!itemId.IsOk())
    {
        return;
    }

    SelectItem(itemId);
    wxPoint pt = ev.GetPoint();
    if (itemId == mServerTreeItemId)
    {
        if (mServerRootMenu)
        {
            PopupMenu(mServerRootMenu, pt);
        }
    }
    else
    {
        if (mServerMenu)
        {
            PopupMenu(mServerMenu, pt);
        }
    }
}

void 
ServerPanel::onUpdateServerPanelEvent(const UpdateServerPanelEvent& ev)
{
    buildServerTree();
}

void 
ServerPanel::onLoginAll(wxCommandEvent& ev)
{
    LogicSystem::getSingleton().loginAll();
}

void 
ServerPanel::onLogoutAll(wxCommandEvent& ev)
{
    LogicSystem::getSingleton().logoutAll();
}

void 
ServerPanel::onLogin(wxCommandEvent& ev)
{
    wxTreeItemId item = GetSelection();
    if (!item.IsOk())
    {
        return;
    }

    std::string value = GetItemText(item).c_str();
    String lynxValue = value.c_str();
    Vector<String> v;
    lynxStrSplit(lynxValue, "[", v, true);

    String serverName = v[0];

    LogicSystem::getSingleton().login(serverName.c_str());

    wxColor defaultColour = GetItemTextColour(mServerTreeItemId);
    if (mActiveTreeItemId.IsOk())
    {
        SetItemTextColour(mActiveTreeItemId, defaultColour);
    }
    SetItemTextColour(item, *wxRED);
    mActiveTreeItemId = item;

    LogicSystem::getSingleton().setCurrentGMClient(serverName);
}

void 
ServerPanel::onLogout(wxCommandEvent& ev)
{
    wxTreeItemId item = GetSelection();
    if (!item.IsOk())
    {
        return;
    }

    std::string value = GetItemText(item).c_str();
    String lynxValue = value.c_str();
    Vector<String> v;
    lynxStrSplit(lynxValue, "[", v, true);

    String serverName = v[0];

    LogicSystem::getSingleton().logout(serverName.c_str());

    wxColor defaultColour = GetItemTextColour(mServerTreeItemId);
    if (mActiveTreeItemId.IsOk())
    {
        SetItemTextColour(item, defaultColour);
    }

    //LogicSystem::getSingleton().setCurrentGMClient(serverName);
}

