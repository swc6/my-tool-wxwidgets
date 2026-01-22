#pragma once

#include <wx/wx.h>
#include <wx/collpane.h>
#include <wx/scrolwin.h>
#include <wx/aui/auibook.h>
#include "HelperBase.h"

class MainFrame : public wxFrame {
public:
    MainFrame();
    ~MainFrame() override = default;

    // 当左侧工具被选择时由按钮回调调用（每次会新增一个 tab）
    void OnToolSelected(const wxString& toolName);

private:
    // 右侧工作区的 notebook（每次新增由左侧触发）
    wxAuiNotebook* m_notebook;
    // 用于生成新页的计数器（标题后缀）
    int m_newTabCounter;

    // 左侧/右侧占位控件
    wxPanel* m_rightPanel;
    wxStaticText* m_rightLabel;

    // 左侧选中按钮（用于选中样式）
    wxButton* m_selectedButton = nullptr;

    // 关闭时隐藏窗口（遵循项目约定）
    void OnClose(wxCloseEvent& event);

    // 创建新 tab（追加到末尾并选中），传入已实例化的 Helper 对象，TabPanel 将接管其生命周期
    wxWindow* CreateNewTab(HelperBase* helper);
};
