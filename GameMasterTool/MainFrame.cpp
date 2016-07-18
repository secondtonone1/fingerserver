#include "MainFrame.h"
#include "GameMasterTool.h"
#include "ServerPanel.h"
#include "LoginPanel.h"
#include "OutputPanel.h"
#include "ViewPanel.h"
#include "CommandPanel.h"

using namespace Lynx;

IMPLEMENT_CLASS(MainFrame, wxFrame)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)

END_EVENT_TABLE()

MainFrame::MainFrame(wxFrame* parent, wxWindowID id, const wxString& title,
                     const wxPoint& pos, const wxSize& size, long style, const wxString& name)
                     : wxFrame(parent, id, title, pos, size, style | wxTAB_TRAVERSAL, name)
{
    mPlayerInfoDialog = NULL;
    mGuildInfoDialog = NULL;
    mLoginPanel = NULL;
    mServerPanel = NULL;
    mViewPanel = NULL;
    mCommandPanel = NULL;
    mOutputPanel = NULL;
}

MainFrame::~MainFrame()
{
    release();
}

bool 
MainFrame::initial()
{
    Centre();
    Show(true);
    Update();

    mAuiManager.SetManagedWindow(this);
    mAuiManager.Update();

    mPlayerInfoDialog = new PlayerInfoDialog(this, wxID_ANY, wxT("玩家信息"), wxDefaultPosition,
        wxSize(1000, 700));
    assert(mPlayerInfoDialog);
    if (!mPlayerInfoDialog->initial())
    {
        LOG_WARN("Failed to initial player info dialog.");
        return false;
    }

    mGuildInfoDialog = new GuildInfoDialog(this, wxID_ANY, wxT("公会信息"), wxDefaultPosition,
        wxSize(1000, 700));
    assert(mGuildInfoDialog);
    if (!mGuildInfoDialog->initial())
    {
        LOG_WARN("Failed to initial guild info dialog.");
        return false;
    }

    registerEvents();

    showLoginWindow();

    return true;
}

void 
MainFrame::release()
{
    if (mPlayerInfoDialog)
    {
        delete(mPlayerInfoDialog);
        mPlayerInfoDialog = NULL;
    }

    if (mGuildInfoDialog)
    {
        delete(mGuildInfoDialog);
        mGuildInfoDialog = NULL;
    }


    mAuiManager.UnInit();
}

void 
MainFrame::registerEvents()
{
    REGISTER_EVENT(ServerSelectEvent, this, &MainFrame::onServerSelectEvent);
    REGISTER_EVENT(LoginSuccessEvent, this, &MainFrame::onLoginSuccessEvent);
    REGISTER_EVENT(LoginBrokenEvent, this, &MainFrame::onLoginBrokenEvent);
}

void
MainFrame::deregisterEvents()
{
    DEREGISTER_EVENT(ServerSelectEvent, this, &MainFrame::onServerSelectEvent);
    DEREGISTER_EVENT(LoginSuccessEvent, this, &MainFrame::onLoginSuccessEvent);
    DEREGISTER_EVENT(LoginBrokenEvent, this, &MainFrame::onLoginBrokenEvent);
}

void 
MainFrame::showLoginWindow()
{
    SetSize(340, 200);
    Centre();
    cleanAllPanels();

    if (!createLoginPanel())
    {
        LOG_WARN("Failed to create login panel.");
        if (wxMessageBox(wxT("创建登录窗口失败"), wxT("主界面"), wxOK) == wxOK)
        {
            Close(true);
        }
    }
    Show(true);
}

void 
MainFrame::showMainFrameWindow()
{
    SetSize(1280, 800);
    Centre();
    cleanAllPanels();

    if (!createMainFramePanels())
    {
        LOG_WARN("Failed to create main panel.");
        if (wxMessageBox(wxT("创建操作窗口失败"), wxT("主界面"), wxOK) == wxOK)
        {
            Close(true);
        }
    }
    Show(true);
}

bool 
MainFrame::createLoginPanel()
{
    mLoginPanel = new LoginPanel(this, myID_LOGIN_PANEL,
        wxDefaultPosition, wxSize(340, 200), wxTR_DEFAULT_STYLE | wxNO_BORDER | wxTAB_TRAVERSAL);
    assert(mLoginPanel);

    if (!mLoginPanel->initial())
    {
        LOG_WARN("Failed to initial login panel.");
        delete mLoginPanel;
        mLoginPanel = NULL;
        return false;
    }

    mAuiManager.AddPane(mLoginPanel, wxAuiPaneInfo().Window(mLoginPanel).Name(wxT("登录窗口")).CaptionVisible(false)
        .PaneBorder(false).Center().CloseButton(false).Floatable(false).MaximizeButton(false));
    mAuiManager.Update();
    return true;
}

bool 
MainFrame::createMainFramePanels()
{
    // 创建ServerPanel
    mServerPanel = new ServerPanel(this, myID_SERVER_PANEL,
        wxDefaultPosition, wxSize(250, 720), wxTR_DEFAULT_STYLE | wxNO_BORDER);
    assert(mServerPanel);
    
    if (!mServerPanel->initial())
    {
        LOG_WARN("Failed to initial serverPanel.");
        delete mServerPanel;
        return false;
    }
    mAuiManager.AddPane(mServerPanel, wxAuiPaneInfo().Name(wxT("服务器管理器")).
        Caption(wxT("服务器管理器")).Left().Layer(1).Position(1).CloseButton(false).
        Floatable(false).MaximizeButton(false));

    // 创建ViewPanel
    mViewPanel = new ViewPanel(this, myID_VIEW_PANEL,
        wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT |
        wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_MIDDLE_CLICK_CLOSE |
        wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);
    assert(mViewPanel);
    if (!mViewPanel->initial())
    {
        LOG_WARN("Failed to initial view panel.");
        delete mViewPanel;
        return false;
    }
    mAuiManager.AddPane(mViewPanel, wxAuiPaneInfo().Window(mViewPanel).Name(wxT("ViewWindow")).
        Caption(wxT("查看窗口")).Center().CloseButton(false).
        Floatable(false).MaximizeButton(false));

    // 创建OutputPanel
    mOutputPanel = new OutputPanel(this);
    assert(mOutputPanel);
    if (!mOutputPanel->initial())
    {
        LOG_WARN("Failed to initial output panel.");
        return false;
    }

    mAuiManager.AddPane(mOutputPanel, wxAuiPaneInfo().Window(mOutputPanel).Name(wxT("输出面板")).
        Caption(wxT("输出面板")).Bottom().Row(1).CloseButton(false).Floatable(false).
        MaximizeButton(false));

    // 创建命令面板
    mCommandPanel = new CommandPanel(this, myID_COMMAND_PANEL,
        wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE);
    assert(mCommandPanel);
    if (!mCommandPanel->initial())
    {
        LOG_WARN("Failed to intial command panel.");
        delete mCommandPanel;
        return false;
    }

    mAuiManager.AddPane(mCommandPanel, wxAuiPaneInfo().Window(mCommandPanel).Bottom().Row(0).CaptionVisible(false).
        CloseButton(false).Floatable(false).MaximizeButton(false));

    mAuiManager.Update();

    return true;
}

void 
MainFrame::cleanAllPanels()
{
    wxAuiPaneInfoArray& allPanes = mAuiManager.GetAllPanes();
    for (UInt32 i = 0, count = allPanes.GetCount(); i < count; ++i)
    {
        mAuiManager.ClosePane(allPanes.Item(i));
        wxWindow* window = allPanes.Item(i).window;
        if (window)
        {
            mAuiManager.DetachPane(window);
            RemoveChild(window);
            delete window;
            window = NULL;
        }
    }

    mAuiManager.Update();
}

PlayerInfoDialog& 
MainFrame::getPlayerInfoDialog()
{
    return *mPlayerInfoDialog;
}

GuildInfoDialog& 
MainFrame::getGuildInfoDialog()
{
    return *mGuildInfoDialog;
}

void 
MainFrame::onServerSelectEvent(const ServerSelectEvent& ev)
{
    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    if (gmClient->getServerName() != ev.mServerName)
    {
        return;
    }

    if (ev.mSelected)
    {
        std::string loginState;
        if (gmClient->getLoginState() == GMClient::LOGINED_STATE)
        {
            loginState = "登录";
        }
        else
        {
            loginState = "未登录";
        }

        std::string caption = "查看窗口【";
        caption += ev.mServerName.c_str();
        caption += "】";
        caption += "【";
        caption += loginState;
        caption += "】";

        mAuiManager.GetPane(mViewPanel).Caption(caption.c_str());

        caption = "输出窗口【";
        caption += ev.mServerName.c_str();
        caption += "】";
        caption += "【";
        caption += loginState;
        caption += "】";
        mAuiManager.GetPane(mOutputPanel).Caption(caption.c_str());
    }
    else
    {
        mAuiManager.GetPane(mViewPanel).Caption(wxT("查看窗口"));
        mAuiManager.GetPane(mOutputPanel).Caption(wxT("输出窗口"));
    }

    mAuiManager.Update();
}

void 
MainFrame::onLoginSuccessEvent(const LoginSuccessEvent& ev)
{
    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    if (gmClient->getServerName() != ev.mServerName)
    {
        return;
    }

    std::string loginState;
    if (gmClient->getLoginState() == GMClient::LOGINED_STATE)
    {
        loginState = "登录";
    }
    else
    {
        loginState = "未登录";
    }

    std::string caption = "查看窗口【";
    caption += ev.mServerName.c_str();
    caption += "】";
    caption += "【";
    caption += loginState;
    caption += "】";

    mAuiManager.GetPane(mViewPanel).Caption(caption.c_str());

    caption = "输出窗口【";
    caption += ev.mServerName.c_str();
    caption += "】";
    caption += "【";
    caption += loginState;
    caption += "】";
    mAuiManager.GetPane(mOutputPanel).Caption(caption.c_str());

    mAuiManager.Update();
}

void 
MainFrame::onLoginBrokenEvent(const LoginBrokenEvent& ev)
{
    if (mPlayerInfoDialog)
    {
        if (mPlayerInfoDialog->IsShown())
        {
            mPlayerInfoDialog->Close();
        }
    }

    if (mGuildInfoDialog)
    {
        if (mGuildInfoDialog->IsShown())
        {
            mGuildInfoDialog->Close();
        }
    }

    GMClient* gmClient = LogicSystem::getSingleton().getCurrentGMClient();
    if (!gmClient)
    {
        return;
    }

    if (gmClient->getServerName() != ev.mServerName)
    {
        return;
    }

    std::string loginState;
    if (gmClient->getLoginState() == GMClient::LOGINED_STATE)
    {
        loginState = "登录";
    }
    else
    {
        loginState = "未登录";
    }

    std::string caption = "查看窗口【";
    caption += ev.mServerName.c_str();
    caption += "】";
    caption += "【";
    caption += loginState;
    caption += "】";

    mAuiManager.GetPane(mViewPanel).Caption(caption.c_str());

    caption = "输出窗口【";
    caption += ev.mServerName.c_str();
    caption += "】";
    caption += "【";
    caption += loginState;
    caption += "】";
    mAuiManager.GetPane(mOutputPanel).Caption(caption.c_str());

    mAuiManager.Update();
}

