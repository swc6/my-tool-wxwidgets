#include "MyTaskBarIcon.h"
#include "MainFrame.h" // 添加此行，确保 MainFrame 类型完整
#include <wx/artprov.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bitmap.h>
#include <wx/msgdlg.h>
#include <wx/wx.h>

// 前向声明与定义分离：这里只需要 MainFrame 的指针操作
// class MainFrame; // 这行可以保留或删除，已包含完整声明

MyTaskBarIcon::MyTaskBarIcon(MainFrame* frame, const wxString& iconPath)
    : wxTaskBarIcon()
    , m_frame(frame)
{
    // 尝试从指定路径加载 PNG 并缩放到托盘常用大小（16x16）
    wxBitmap bmp;
    if (!iconPath.IsEmpty()) {
        wxImage img;
        if (img.LoadFile(iconPath, wxBITMAP_TYPE_PNG)) {
            if (img.IsOk()) {
                img = img.Scale(16, 16, wxIMAGE_QUALITY_HIGH);
                bmp = wxBitmap(img);
            }
        }
    }

    // 若加载失败，回退到系统艺术图标
    if (!bmp.IsOk()) {
        bmp = wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16, 16));
    }

    if (bmp.IsOk()) {
        wxIcon icon;
        icon.CopyFromBitmap(bmp);
        SetIcon(icon, wxString::FromUTF8("wxDemo"));
    }

    // 绑定托盘的单击/双击事件以便快速显示/隐藏主窗口
    Bind(wxEVT_TASKBAR_LEFT_DOWN, &MyTaskBarIcon::OnActivate, this);
    Bind(wxEVT_TASKBAR_LEFT_DCLICK, &MyTaskBarIcon::OnActivate, this);
}

MyTaskBarIcon::~MyTaskBarIcon()
{
    RemoveIcon();
}

wxMenu* MyTaskBarIcon::CreatePopupMenu()
{
    wxMenu* menu = new wxMenu;
    menu->Append(ID_Show, wxString::FromUTF8("显示主界面"));
    menu->Append(ID_Settings, wxString::FromUTF8("系统设置"));
    menu->AppendSeparator();
    menu->Append(ID_Exit, wxString::FromUTF8("退出"));

    menu->Bind(wxEVT_MENU, &MyTaskBarIcon::OnShowMenu, this, ID_Show);
    menu->Bind(wxEVT_MENU, &MyTaskBarIcon::OnSettingsMenu, this, ID_Settings);
    menu->Bind(wxEVT_MENU, &MyTaskBarIcon::OnExitMenu, this, ID_Exit);

    return menu;
}

void MyTaskBarIcon::ToggleFrameVisibility()
{
    if (!m_frame) return;
    if (m_frame->IsShown()) {
        m_frame->Hide();
    } else {
        m_frame->Show();
        m_frame->Raise();
    }
}

void MyTaskBarIcon::OnActivate(wxTaskBarIconEvent&)
{
    ToggleFrameVisibility();
}

void MyTaskBarIcon::OnShowMenu(wxCommandEvent&)
{
    if (!m_frame) return;
    if (!m_frame->IsShown()) {
        m_frame->Show();
    }
    m_frame->Raise();
}

void MyTaskBarIcon::OnSettingsMenu(wxCommandEvent&)
{
    if (m_frame) {
        wxMessageBox(wxString::FromUTF8("系统设置对话（在此处实现具体设置界面）"),
                     wxString::FromUTF8("系统设置"), wxOK | wxICON_INFORMATION, m_frame);
    }
}

void MyTaskBarIcon::OnExitMenu(wxCommandEvent&)
{
    // 退出时显式销毁主窗口并退出主循环（避免被隐藏逻辑拦截）
    if (m_frame) {
        m_frame->Destroy();
        m_frame = nullptr;
    }
    RemoveIcon();
    wxTheApp->ExitMainLoop();
}