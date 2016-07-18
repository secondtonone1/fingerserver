#ifndef __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_PET_PANEL_H__
#define __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_PET_PANEL_H__

#include "CommonLib.h"
#include "EventDefinition.h"
#include "WXLib.h"

namespace Lynx
{
    class PlayerInfoPetPanel : public wxPanel
    {
        DECLARE_CLASS(PlayerInfoPetPanel)
    public:
        PlayerInfoPetPanel(wxWindow *parent = NULL, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
        virtual ~PlayerInfoPetPanel();

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
            String mPetName;
            Guid mPetGuid;
            wxTreeItemId mTreeItemId;
        };

        bool initial();
        void release();
        void registerEvents();
        void deregisterEvents();
        void resetLayout();

    private:
        void onTreeItemActivated(wxTreeEvent& ev);
        void onCopySkillId(wxCommandEvent& ev);
        void onSkillListRightClicked(wxListEvent& ev);

    private:
        void showPetInfos(const Guid& guid);
        void showPetProperty(const Guid& guid);
        void showPetSkill(const Guid& guid);
        TreeItem* getTreeItem(wxTreeItemId id);
        wxString getSkillColTypeText(int colType);
        bool getSkillListTextContents(long index, UInt32 colType, std::string& res);

    private:
        DECLARE_EVENT_TABLE()
        wxStaticBox* mTreeCtrlBox;
        wxTreeCtrl* mTreeCtrl;
        wxStaticBox* mPropertyGridBox;
        wxPropertyGrid* mPropertyGrid;
        wxListCtrl* mSkillList;
        wxMenu* mSkillMenu;
        wxStaticBox* mSkillListBox;
        wxStaticBox* mPetInfoBox;
        wxTreeItemId mTreeCtrlRootItem;
        List<TreeItem> mTreeItemList;
    };
} // namespace Lynx

#endif // __LYNX_GAME_MASTER_TOOL_PLAYER_INFO_PET_PANEL_H__