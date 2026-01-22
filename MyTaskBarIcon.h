#pragma once

#include <wx/taskbar.h>
#include <wx/string.h>

class MainFrame; // 前向声明，避免包含 MainFrame 的实现

class MyTaskBarIcon : public wxTaskBarIcon {
public:
    enum {
        ID_Show = wxID_HIGHEST + 1,
        ID_Settings,
        ID_Exit
    };

    explicit MyTaskBarIcon(MainFrame* frame, const wxString& iconPath);
    ~MyTaskBarIcon() override;

    wxMenu* CreatePopupMenu() override;

private:
    MainFrame* m_frame;

    void ToggleFrameVisibility();
    void OnActivate(wxTaskBarIconEvent& event);
    void OnShowMenu(wxCommandEvent& event);
    void OnSettingsMenu(wxCommandEvent& event);
    void OnExitMenu(wxCommandEvent& event);
};
