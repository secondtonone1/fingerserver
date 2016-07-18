#ifndef __LYNX_GAME_MASTER_TOOL_WX_LIB_H__
#define __LYNX_GAME_MASTER_TOOL_WX_LIB_H__

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/aui/aui.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/window.h>
#include <wx/statusbr.h>
#include <wx/button.h>
#include <wx/image.h>
#include <wx/imaglist.h>
#include <wx/scrolwin.h>
#include <wx/filedlg.h>
#include <wx/generic/textdlgg.h>
#include <wx/dirdlg.h>
#include <wx/scrolwin.h>
#include <wx/dcclient.h>
#include <wx/dnd.h>
#include <wx/treebase.h>
#include <wx/event.h>
#include <wx/msgdlg.h>
#include <wx/notebook.h>
#include <wx/aui/auibook.h>
#include <wx/generic/panelg.h>
#include <wx/dnd.h>
#include <wx/stc/stc.h>
#include <wx/button.h>
#include <wx/msw/slider.h>
#include <wx/spinctrl.h>
#include <wx/timer.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/listctrl.h>
#include <wx/generic/propdlg.h>
#include <wx/clipbrd.h>
#include <wx/statline.h>
#include <wx/grid.h>
#include <wx/headerctrl.h>
#include <wx/generic/gridctrl.h>
#include <wx/generic/grideditors.h>
#include <wx/url.h>
#include <wx/protocol/http.h>
#include <wx/sstream.h>

enum
{
    myID_ANY = wxID_HIGHEST+ 1000,
    myID_TIMER,
    myID_SERVER_PANEL,
    myID_MAIN_FRAME,
    myID_LOGIN_ACCOUNT_TEXT,
    myID_LOGIN_PASSWORD_TEXT,
    myID_LOGIN_BTN,
    myID_LOGIN_QUIT,
    myID_LOGIN_PANEL,
    myID_SERVER_PANEL_ROOT_MENU_LOGIN_ALL,
    myID_SERVER_PANEL_ROOT_MENU_LOGOUT_ALL,
    myID_SERVER_PANEL_MENU_LOGIN,
    myID_SERVER_PANEL_MENU_LOGOUT,
    myID_VIEW_PANEL,
    myID_CHAT_PANEL,
    myID_PLAYER_PANEL,
    myID_COMMAND_INPUT_TEXT_CTRL,
    myID_COMMAND_ENTER_BUTTON,
    myID_COMMAND_PANEL,
    myID_NOTICE_PANEL,
    myID_GUILD_PANEL,
    myID_MALL_PANEL,
    myID_EMAIL_PANEL,
    myID_FORBIDDEN_PANEL,
    myID_NOTICE_PANEL_BTN_PUBLISH,
    myID_NOTICE_PANEL_BTN_SEARCH,
    myID_NOTICE_PANEL_TEXT_CTRL_CONTENT,
    myID_NOTICE_PANEL_TEXT_CTRL_START_TIME,
    myID_NOTICE_PANEL_TEXT_CTRL_CYCLE_COUNT,
    myID_NOTICE_PANEL_LIST_CTRL_RESULT,
    myID_PLAYER_MENU_COPY_ID,
    myID_PLAYER_MENU_COPY_NAME,
    myID_PLAYER_MENU_COPY_ACCOUNT_ID,
    myID_PLAYER_MENU_COPY_ACCOUNT_NAME,
    myID_PLAYER_MENU_SHOW,
    myID_PLAYER_MENU_FORBIDDEN_CHAT,
    myID_PLAYER_MENU_FORBIDDEN_LOGIN,
    myID_PLAYER_MENU_CHAT,
    myID_PLAYER_MENU_SEND_EMAIL,
    myID_EMAIL_PANEL_CLEAR_RECORD,
    myID_EMAIL_PANEL_SEND_EMAIL,
    myID_EMAIL_PANEL_TEXT_CTRL_RECORD,
    myID_EMAIL_PANEL_DEL_ALL_ITEM,
    myID_EMIAL_DIALOG_TEXT_CTRL_ITEM_COUNT,
    myID_EMAIL_DIALOG_TEXT_CTRL_ITEM_TEMPLATE_ID,
    myID_EMAIL_PANEL_TEXT_CTRL_CONTENTS,
    myID_EMAIL_PANEL_TEXT_CTRL_RECV_NAME,
    myID_EMAIL_PANEL_LIST_CTRL_ITEM,
    myID_EMAIL_PANEL_ADD_ITEM,
    myID_EMAIL_PANEL_RESET,
    myID_EMAIL_PANEL_MENU_DEL_ITEM,
    myID_PLAYER_PANEL_RESET_LAYOUT,
    myID_PLAYER_PANEL_SHOW_ONLINE,
    myID_PLAYER_PANEL_CLEAR_RESULT,
    myID_PLAYER_PANEL_PLAYER_GUID,
    myID_PLAYER_PANEL_PLAYER_GUID_SEARCH,
    myID_PLAYER_PANEL_PLAYER_GUID_CLEAR,
    myID_PLAYER_PANEL_PLAYER_NAME,
    myID_PLAYER_PANEL_PLAYER_NAME_SEARCH,
    myID_PLAYER_PANEL_PLAYER_NAME_CLEAR,
    myID_PLAYER_PANEL_ACCOUNT_GUID,
    myID_PLAYER_PANEL_ACCOUNT_GUID_SEARCH,
    myID_PLAYER_PANEL_ACCOUNT_GUID_CLEAR,
    myID_PLAYER_PANEL_ACCOUNT_NAME,
    myID_PLAYER_PANEL_ACCOUNT_NAME_SEARCH,
    myID_PLAYER_PANEL_ACCOUNT_NAME_CLEAR,
    myID_PLAYER_PANEL_SEARCH_RESULT,
    myID_GUILD_PANEL_SEARCH_RESULT,
    myID_GUILD_PANEL_GUILD_GUID,
    myID_GUILD_PANEL_GUILD_GUID_SEARCH,
    myID_GUILD_PANEL_GUILD_GUID_CLEAR,
    myID_GUILD_PANEL_GUILD_NAME,
    myID_GUILD_PANEL_GUILD_NAME_SEARCH,
    myID_GUILD_PANEL_GUILD_NAME_CLEAR,
    myID_GUILD_PANEL_SHOW_ONLINE,
    myID_GUILD_PANEL_RESET_LAYOUT,
    myID_GUILD_PANEL_CLEAR_RESULT,
    myID_GUILD_MENU_COPY_ID,
    myID_GUILD_MENU_COPY_NAME,
    myID_GUILD_MENU_SHOW,
    myID_PLAYER_INFO_BASE_PANEL,
    myID_PLAYER_INFO_BAG_PANEL,
    myID_PLAYER_INFO_HERO_PANEL,
    myID_PLAYER_INFO_PET_PANEL,
    myID_GUILD_INFO_BASE_PANEL,
    myID_GUILD_INFO_MEMBER_PANEL,
    myID_PLAYER_INFO_BASE_PANEL_PROPERTY_GRID,
    myID_PLAYER_INFO_BAG_PANEL_TREE,
    myID_PLAYER_INFO_BAG_PANEL_LIST,
    myID_PLAYER_INFO_HERO_PANEL_TREE,
    myID_PLAYER_INFO_HERO_PANEL_EQUIP,
    myID_PLAYER_INFO_HERO_PANEL_RUNE,
    myID_PLAYER_INFO_HERO_PANEL_SKILL,
    myID_PLAYER_INFO_HERO_PANEL_PROPERTY,
    myID_PLAYER_INFO_PET_PANEL_PROPERTY,
    myID_PLAYER_INFO_PET_PANEL_TREE,
    myID_PLAYER_INFO_PET_PANEL_SKILL,
    myID_PLAYER_INFO_BAG_PANEL_MENU,
    myID_PLAYER_INFO_HERO_PANEL_EQUIP_MENU,
    myID_PLAYER_INFO_HERO_PANEL_RUNE_MENU,
    myID_PLAYER_INFO_HERO_PANEL_SKILL_MENU,
    myID_PLAYER_INFO_PET_PANEL_SKILL_MENU,

};

#endif // __LYNX_GAME_MASTER_TOOL_WX_LIB_H__
