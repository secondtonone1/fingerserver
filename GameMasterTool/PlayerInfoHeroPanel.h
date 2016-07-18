#ifndef __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_HERO_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_HERO_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class PlayerInfoHeroPanel : public wxPanel
    {
        DECLARE_CLASS(PlayerInfoBagPanel)
    public:
        PlayerInfoHeroPanel(wxWindow *parent = NULL, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
        virtual ~PlayerInfoHeroPanel();

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

        enum SkillColType
        {
            COL_TYPE_SKILL_TEMPLATE_ID = 0,
            COL_TYPE_SKILL_NAME,
            COL_TYPE_SKILL_TYPE,
            COL_TYPE_SKILL_COOLDOWN,
            COL_TYPE_SKILL_INC_ANGER,
            COL_TYPE_SKILL_TRIGGER_ANGER,
            COL_TYPE_SKILL_DESC,
            COL_TYPE_SKILL_MAX_COUNT,
        };

        struct TreeItem
        {
            String mHeroName;
            Guid mHeroGuid;
            wxTreeItemId mTreeItemId;
        };

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

    private:
        void onTreeItemActivated(wxTreeEvent& ev);
        void onCopyEquipId(wxCommandEvent& ev);
        void onCopyRuneId(wxCommandEvent& ev);
        void onCopySkillId(wxCommandEvent& ev);
        void onEquipListRightClicked(wxListEvent& ev);
        void onRuneListRightClicked(wxListEvent& ev);
        void onSkillListRightClicked(wxListEvent& ev);

    private:
        void showHeroInfos(const Guid& guid);
        void showHeroProperty(const Guid& guid);
        void showHeroEquip(const Guid& guid);
        void showHeroRune(const Guid& guid);
        void showHeroSkill(const Guid& guid);

        TreeItem* getTreeItem(wxTreeItemId id);
        wxString getEquipColTypeText(int colType);
        wxString getRuneColTypeText(int colType);
        wxString getSkillColTypeText(int colType);

        bool getEquipListTextContents(long index, UInt32 colType, std::string& res);
        bool getRuneListTextContents(long index, UInt32 colType, std::string& res);
        bool getSkillListTextContents(long index, UInt32 colType, std::string& res);

    private:
        DECLARE_EVENT_TABLE()
        wxStaticBox* mTreeCtrlBox;
        wxTreeCtrl* mTreeCtrl;
        wxStaticBox* mPropertyGridBox;
        wxPropertyGrid* mPropertyGrid;
        wxStaticBox* mEquipListBox;
        wxListCtrl* mEquipList;
        wxMenu* mEquipMenu;
        wxStaticBox* mRuneListBox;
        wxListCtrl* mRuneList;
        wxMenu* mRuneMenu;
        wxListCtrl* mSkillList;
        wxMenu* mSkillMenu;
        wxStaticBox* mSkillListBox;
        wxStaticBox* mHeroInfoBox;
        wxTreeItemId mTreeCtrlRootItem;
        wxTreeItemId mPresentHeroRootItem;
        wxTreeItemId mBackupHeroRootItem;
        List<TreeItem> mTreeItemList;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_HERO_PANEL_H__