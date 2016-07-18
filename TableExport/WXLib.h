#ifndef __LYNX_AVATAR_EDITOR_WX_LIB_H__
#define __LYNX_AVATAR_EDITOR_WX_LIB_H__

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
#include <wx/grid.h>

enum
{
    myID_ANY = wxID_HIGHEST + 1000,
    myID_TIMER,
    myID_MAIN_FRAME,
    myID_MAIN_PANEL,
    myID_MAIN_FRAME_EDIT_EXPORT_SELECTED_BUTTON,
    myID_MAIN_FRAME_EDIT_SELECT_ALL_BUTTON,
    myID_MAIN_FRAME_EDIT_UNSELECT_ALL_BUTTON,
    myID_MAIN_PANEL_PROPERTY_BAR,
};

#define CENTER_SERVER "CenterServer"
#define GAME_SERVER "GameServer"
#define LOGIN_SERVER "LoginServer"
#define PERSISTENCE_SERVER "PersistenceServer"
#define PASSPORT_SERVER "PassportServer"

#endif // __LYNX_AVATAR_EDITOR_WX_LIB_H__


