#ifndef __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_BAG_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_BAG_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class PlayerInfoBagPanel : public wxPanel
    {
        DECLARE_CLASS(PlayerInfoBagPanel)
    public:
        PlayerInfoBagPanel(wxWindow *parent = NULL, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
        virtual ~PlayerInfoBagPanel();

        enum ItemColType
        {
            COL_TYPE_ITEM_ID = 0,
            COL_TYPE_ITEM_TEMPLATE_ID,
            COL_TYPE_ITEM_NAME,
            COL_TYPE_ITEM_COUNT,
            COL_TYPE_ITEM_DESC,
            COL_TYPE_ITEM_MAX_COUNT,
        };

        enum EquipColType
        {
            COL_TYPE_EQUIP_ID = 0,
            COL_TYPE_EQUIP_TEMPLATE_ID,
            COL_TYPE_EQUIP_NAME,
            COL_TYPE_EQUIP_DESC,
            COL_TYPE_EQUIP_MAX_COUNT,
        };

        enum RuneColType
        {
            COL_TYPE_RUNE_ID = 0,
            COL_TYPE_RUNE_TEMPLATE_ID,
            COL_TYPE_RUNE_NAME,
            COL_TYPE_RUNE_DESC,
            COL_TYPE_RUNE_MAX_COUNT,
        };

        enum HeroDebrisColType
        {
            COL_TYPE_HERO_DEBRIS_ID = 0,
            COL_TYPE_HERO_DEBRIS_TEMPLATE_ID,
            COL_TYPE_HERO_DEBRIS_NAME,
            COL_TYPE_HERO_DEBRIS_DESC,
            COL_TYPE_HERO_DEBRIS_MAX_COUNT,
        };

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

    private:
        wxString getItemColTypeText(int colType);
        wxString getEquipColTypeText(int colType);
        wxString getRuneColTypeText(int colType);
        wxString getHeroDebrisColTypeText(int colType);
        bool getListCtrlTextContents(long index, UInt32 colType, std::string& res);

        void showItemList();
        void showEquipList();
        void showRuneList();
        void showHeroDebrisList();

    private:
        void onTreeItemActivated(wxTreeEvent& ev);
        void onCopyId(wxCommandEvent& ev);
        void onRightClicked(wxListEvent& ev);

    private:
        DECLARE_EVENT_TABLE()
        wxTreeCtrl* mTreeCtrl;
        wxListCtrl* mListCtrl;
        wxStaticBox* mTreeBox;
        wxStaticBox* mListBox;
        wxTreeItemId mTreeCtrlRootItem;
        wxTreeItemId mTreeCtrlItemId;
        wxTreeItemId mTreeCtrlEquipId;
        wxTreeItemId mTreeCtrlRuneId;
        wxTreeItemId mTreeCtrlHeroDebrisId;
        wxMenu* mOperateMenu;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_BAG_PANEL_H__