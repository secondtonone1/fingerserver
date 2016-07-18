#include "LoginPanel.h"
#include "GameMasterTool.h"

using namespace Lynx;

IMPLEMENT_CLASS(LoginPanel, wxPanel)

BEGIN_EVENT_TABLE(LoginPanel, wxPanel)
EVT_BUTTON(myID_LOGIN_BTN, onLoginBtnClicked)
EVT_BUTTON(myID_LOGIN_QUIT, onQuitBtnClicked)
END_EVENT_TABLE()

LoginPanel::LoginPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style)
                       : wxPanel(parent, id, pos, size, style)
{
    mAccountCtrl = NULL;
    mPasswordCtrl = NULL;
    mLoginBtn = NULL;
    mQuitBtn = NULL;
}

LoginPanel::~LoginPanel()
{
    release();
}

bool
LoginPanel::initial()
{
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(topSizer);

    wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
    boxSizer->Add(10, 10, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    wxBoxSizer* accountSizer = new wxBoxSizer(wxHORIZONTAL);
    boxSizer->Add(accountSizer, 0, wxGROW | wxALL, 5);

    wxStaticText* accountLabel = new wxStaticText(this, wxID_STATIC, wxT("账号:"),
        wxDefaultPosition, wxDefaultSize, 0);
    accountSizer->Add(accountLabel, 0, wxALIGN_LEFT | wxALL, 5);

    mAccountCtrl = new wxTextCtrl(this, myID_LOGIN_ACCOUNT_TEXT, wxT(""), wxDefaultPosition,
        wxSize(150, 20));
    mAccountCtrl->SetMaxLength(20);
    accountSizer->Add(mAccountCtrl, 0, wxALIGN_RIGHT | wxALL, 5);

    wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
    boxSizer->Add(passwordSizer, 0, wxGROW | wxALL, 5);

    wxStaticText* passwordLabel = new wxStaticText(this, wxID_STATIC, wxT("密码:"),
        wxDefaultPosition, wxDefaultSize, 0);
    passwordSizer->Add(passwordLabel, 0, wxALIGN_LEFT | wxALL, 5);
    mPasswordCtrl = new wxTextCtrl(this, myID_LOGIN_PASSWORD_TEXT, wxT(""), wxDefaultPosition,
        wxSize(150, 20), wxTE_PASSWORD);
    mPasswordCtrl->SetMaxLength(20);
    passwordSizer->Add(mPasswordCtrl, 0, wxALIGN_RIGHT | wxALL, 5);

    // 按钮Sizer
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    boxSizer->Add(btnSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
    // 登录按钮
    wxButton* mLoginBtn = new wxButton(this, myID_LOGIN_BTN, wxT("登录"),
        wxDefaultPosition, wxDefaultSize, 0);
    btnSizer->Add(mLoginBtn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    // 退出按钮
    wxButton* mQuitBtn = new wxButton(this, myID_LOGIN_QUIT, wxT("退出"),
        wxDefaultPosition, wxDefaultSize, 0);
    btnSizer->Add(mQuitBtn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mAccountCtrl->SetFocus();

    // 设置tab遍历顺序
    mAccountCtrl->MoveAfterInTabOrder(mQuitBtn);
    mPasswordCtrl->MoveAfterInTabOrder(mAccountCtrl);
    mLoginBtn->MoveAfterInTabOrder(mPasswordCtrl);
    mQuitBtn->MoveAfterInTabOrder(mLoginBtn);

    // 设置登录默认窗口
    mLoginBtn->SetDefault();

    topSizer->Fit(this);
    topSizer->SetSizeHints(this);

    return true;
}

void
LoginPanel::release()
{
    mAccountCtrl = NULL;
    mPasswordCtrl = NULL;
    mLoginBtn = NULL;
    mQuitBtn = NULL;
}

void
LoginPanel::onLoginBtnClicked(wxCommandEvent& ev)
{
    wxString account = mAccountCtrl->GetValue();
    wxString password = mPasswordCtrl->GetValue();

    if (account.empty())
    {
        LOG_WARN("Account is empty.");
        wxMessageBox(wxT("账户不能为空"), wxT("登录"), wxOK);
        return;
    }

    if (password.empty())
    {
        LOG_WARN("Password is empty.");
        wxMessageBox(wxT("密码不能为空"), wxT("登录"), wxOK);
        return;
    }

    LogicSystem::getSingleton().setAccountName(account.c_str());
    LogicSystem::getSingleton().setAccountPassword(password.c_str());

    MAIN_FRAME().showMainFrameWindow();
}

void
LoginPanel::onQuitBtnClicked(wxCommandEvent& ev)
{
    MAIN_FRAME().Close(true);
}


