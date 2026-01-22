#pragma once

#include <wx/wx.h>
#include "../HelperBase.h"

// 右侧 tab 面板：每个 tab 持有一个 Helper 实例（由 caller 传入并由此类负责释放）
// 布局：左 输入（多行），中 操作按钮 列，右 结果（只读多行）
// helper == nullptr => 显示欢迎页
class TabPanel : public wxPanel {
public:
    explicit TabPanel(wxWindow* parent, HelperBase* helper);
    ~TabPanel() override;
    
    // 设置输入框文本
    void SetInputText(const wxString& text);

private:
    HelperBase* m_helper;
    wxTextCtrl* m_input = nullptr;
    wxTextCtrl* m_result = nullptr;
};