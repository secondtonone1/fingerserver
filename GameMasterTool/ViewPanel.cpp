#include "ViewPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

static const std::string sPanelPageChatName("聊天");
static const std::string sPanelPageGuildName("公会");
static const std::string sPanelPagePlayerName("玩家");
static const std::string sPanelPageEmailName("邮件");
static const std::string sPanelPageNoticeName("公告");
static const std::string sPanelPageMallName("商城");
static const std::string sPanelPageForbiddenName("禁止");

IMPLEMENT_CLASS(ViewPanel, wxAuiNotebook)

BEGIN_EVENT_TABLE(ViewPanel, wxAuiNotebook)
EVT_AUINOTEBOOK_PAGE_CHANGED(myID_VIEW_PANEL, onNotebookChanged)
END_EVENT_TABLE()


ViewPanel::ViewPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style)
    : wxAuiNotebook(parent, id, pos, size, style)
{
    mChatPanel = NULL;
    mPlayerPanel = NULL;
    mGuildPanel = NULL;
    mEmailPanel = NULL;
    mMallPanel = NULL;
    mNoticePanel = NULL;
    mForbiddenPanel = NULL;
}

ViewPanel::~ViewPanel()
{
    release();
}

bool
ViewPanel::initial()
{
    wxBitmap pageBmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));

    // 创建聊天面板
    mChatPanel = new ChatPanel(this, myID_CHAT_PANEL, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxNO_BORDER | wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
    assert(mChatPanel);
    if (!mChatPanel->initial())
    {
        LOG_WARN("Failed to initial chat panel.");
        return false;
    }

    mChatPanel->SetName(sPanelPageChatName.c_str());
    AddPage(mChatPanel, mChatPanel->GetName(), true, pageBmp);

    // 创建公告面板
    mNoticePanel = new NoticePanel(this, myID_NOTICE_PANEL, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxNO_BORDER);
    assert(mNoticePanel);
    if (!mNoticePanel->initial())
    {
        LOG_WARN("Failed to initial notice panel.");
        return false;
    }

    mNoticePanel->SetName(sPanelPageNoticeName.c_str());
    AddPage(mNoticePanel, mNoticePanel->GetName(), false, pageBmp);


    // 创建玩家面板
    mPlayerPanel = new PlayerPanel(this, myID_PLAYER_PANEL, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxNO_BORDER);
    assert(mPlayerPanel);
    if (!mPlayerPanel->initial())
    {
        LOG_WARN("Failed to initial player panel.");
        return false;
    }

    mPlayerPanel->SetName(sPanelPagePlayerName.c_str());
    AddPage(mPlayerPanel, mPlayerPanel->GetName(), false, pageBmp);

    
    // 创建公会面板
    mGuildPanel = new GuildPanel(this, myID_GUILD_PANEL, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxNO_BORDER);
    assert(mGuildPanel);
    if (!mGuildPanel->initial())
    {
        LOG_WARN("Failed to initial guild panel.");
        return false;
    }

    mGuildPanel->SetName(sPanelPageGuildName.c_str());
    AddPage(mGuildPanel, mGuildPanel->GetName(), false, pageBmp);

    // 创建邮件面板
    mEmailPanel = new EmailPanel(this, myID_EMAIL_PANEL, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxNO_BORDER);
    assert(mEmailPanel);
    if (!mEmailPanel->initial())
    {
        LOG_WARN("Failed to initial email panel.");
        return false;
    }

    mEmailPanel->SetName(sPanelPageEmailName.c_str());
    AddPage(mEmailPanel, mEmailPanel->GetName(), false, pageBmp);

    // 创建商城面板
    mMallPanel = new MallPanel(this, myID_MALL_PANEL, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxNO_BORDER);
    assert(mMallPanel);
    if (!mMallPanel->initial())
    {
        LOG_WARN("Failed to initial mall panel.");
        return false;
    }

    mMallPanel->SetName(sPanelPageMallName.c_str());
    AddPage(mMallPanel, mMallPanel->GetName(), false, pageBmp);

    // 创建禁止面板
    mForbiddenPanel = new ForbiddenPanel(this, myID_FORBIDDEN_PANEL, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxNO_BORDER);
    assert(mForbiddenPanel);
    if (!mForbiddenPanel->initial())
    {
        LOG_WARN("Failed to initial forbidden panel.");
        return false;
    }

    mForbiddenPanel->SetName(sPanelPageForbiddenName.c_str());
    AddPage(mForbiddenPanel, mForbiddenPanel->GetName(), false, pageBmp);
    
    registerEvents();

    Disable();

    return true;
}

void
ViewPanel::release()
{
    mChatPanel = NULL;
    mPlayerPanel = NULL;
    mGuildPanel = NULL;
    mEmailPanel = NULL;
    mMallPanel = NULL;
    mNoticePanel = NULL;

    deregisterEvents();
}

void
ViewPanel::registerEvents()
{
    REGISTER_EVENT(ServerSelectEvent, this, &ViewPanel::onServerSelectEvent);
    REGISTER_EVENT(LoginSuccessEvent, this, &ViewPanel::onLoginSuccessEvent);
    REGISTER_EVENT(LoginBrokenEvent, this, &ViewPanel::onLoginBrokenEvent);
}

void
ViewPanel::deregisterEvents()
{
    DEREGISTER_EVENT(ServerSelectEvent, this, &ViewPanel::onServerSelectEvent);
    DEREGISTER_EVENT(LoginSuccessEvent, this, &ViewPanel::onLoginSuccessEvent);
    DEREGISTER_EVENT(LoginBrokenEvent, this, &ViewPanel::onLoginBrokenEvent);
}

void 
ViewPanel::resetLayout()
{
    SetSelection(0);

    if (mChatPanel)
    {
        mChatPanel->resetLayout();
    }

    if (mPlayerPanel)
    {
        mPlayerPanel->resetLayout();
    }

    if (mGuildPanel)
    {
        mGuildPanel->resetLayout();
    }

    if (mEmailPanel)
    {
        mEmailPanel->resetLayout();
    }

    if (mMallPanel)
    {
        mMallPanel->resetLayout();
    }

    if (mNoticePanel)
    {
        mNoticePanel->resetLayout();
    }

    if (mForbiddenPanel)
    {
        mForbiddenPanel->resetLayout();
    }
}

void 
ViewPanel::onNotebookChanged(wxAuiNotebookEvent& ev)
{
    //if (mChatPanel)
    //{
    //    mChatPanel->resetLayout();
    //}

    //if (mPlayerPanel)
    //{
    //    mPlayerPanel->resetLayout();
    //}

    //if (mGuildPanel)
    //{
    //    mGuildPanel->resetLayout();
    //}

    //if (mEmailPanel)
    //{
    //    mEmailPanel->resetLayout();
    //}

    //if (mMallPanel)
    //{
    //    mMallPanel->resetLayout();
    //}

    if (mNoticePanel)
    {
        mNoticePanel->resetLayout();
    }

    //if (mForbiddenPanel)
    //{
    //    mForbiddenPanel->resetLayout();
    //}
}

void
ViewPanel::onServerSelectEvent(const ServerSelectEvent& ev)
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
        if (gmClient->getLoginState() == GMClient::LOGINED_STATE)
        {
            Enable();
            resetLayout();
        }
        else
        {
            Disable();
        }
    }
    else
    {
        Disable();
    }
}

void 
ViewPanel::onLoginSuccessEvent(const LoginSuccessEvent& ev)
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

    if (gmClient->getLoginState() == GMClient::LOGINED_STATE)
    {
        Enable();
        resetLayout();
    }
    else
    {
        resetLayout();
        Disable();
    }
}

void 
ViewPanel::onLoginBrokenEvent(const LoginBrokenEvent& ev)
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

    if (gmClient->getLoginState() != GMClient::LOGINED_STATE)
    {
        resetLayout();
        Disable();
    }
}


