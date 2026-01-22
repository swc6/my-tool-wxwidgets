#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/taskbar.h> // 提供 wxTaskBarIcon 的定义
#include <wx/image.h>   // 提供 PNG 加载/缩放支持
#include "MyTaskBarIcon.h" // 新引入的托盘类
#include "MainFrame.h"     // 分离后的主窗体

class App : public wxApp {
public:
    App() : m_taskbarIcon(nullptr), m_frame(nullptr) {}

    virtual bool OnInit() override {
        // 初始化图片处理器，确保能加载 PNG
        wxInitAllImageHandlers();

        m_frame = new MainFrame();
        if (!m_frame) {
            return false;
        }

        SetTopWindow(m_frame);

        // 将图标路径作为参数传入 MyTaskBarIcon（相对于可执行文件的相对路径或绝对路径）
        wxString iconPath = wxString::FromUTF8("img/s_s.png");
        m_taskbarIcon = new MyTaskBarIcon(m_frame, iconPath);

        m_frame->Show();

        return true;
    }

    virtual int OnExit() override {
        if (m_taskbarIcon) {
            m_taskbarIcon->RemoveIcon();
            delete m_taskbarIcon;
            m_taskbarIcon = nullptr;
        }
        if (m_frame) {
            m_frame->Destroy();
            m_frame = nullptr;
        }
        return 0;
    }

private:
    MyTaskBarIcon* m_taskbarIcon;
    MainFrame* m_frame;
};

wxIMPLEMENT_APP(App);