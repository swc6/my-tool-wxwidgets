#pragma once

#include <wx/string.h>
#include <vector>
#include <optional>

// 前向声明
class TabPanel;

class HelperBase {
public:
    HelperBase(const wxString& category = wxString(),
        const wxString& name = wxString());
    virtual ~HelperBase() = default;

    // 属性访问
    wxString GetCategory() const noexcept;
    wxString GetName() const noexcept;
    wxString GetDoText() const noexcept;
    wxString GetResultText() const noexcept;
    const std::vector<wxString>& GetActions() const noexcept;
    std::optional<int> GetCurrentActionIndex() const noexcept;
    TabPanel* GetTabPanel() const noexcept;

    void SetCategory(const wxString& category) noexcept;
    void SetName(const wxString& name) noexcept;
    void SetDoText(const wxString& text) noexcept;
    void SetActions(const std::vector<wxString>& actions) noexcept;
    void ClearActions() noexcept;
    // 设置当前操作索引（越界则无效）
    bool SetCurrentActionIndex(int idx) noexcept;
    void SetTabPanel(TabPanel* tabPanel) noexcept;

    bool HasActions() const noexcept;

    // 执行指定索引的 action（index 必须有效），并把结果写入 resultText
    // 返回 true 表示 action 执行成功（注意：具体行为由派生类实现）
    bool RunActionAt(int index);

    // 执行当前索引的 action（需要先 SetCurrentActionIndex）
    bool RunCurrentAction();

    // 顺序执行所有 actions，输入为 doText，第一个 action 以 doText 为输入，
    // 随后的 action 使用前一个 action 的结果作为输入；最终结果写入 resultText。
    // 返回最终结果（空字符串也可能是合法结果）；若没有 actions，返回空字符串。
    wxString RunAllActions();

protected:
    // 派生类必须实现具体 action 的处理逻辑：给定 action 名和输入文本，返回处理后的文本
    virtual wxString ProcessAction(const wxString& action, const wxString& input) = 0;

private:
    wxString m_category;
    wxString m_name;
    wxString m_doText;
    wxString m_resultText;
    std::vector<wxString> m_actions;
    // 当前操作在 actions 中的位置，-1 表示未设置
    int m_currentActionIndex;
    TabPanel* m_tabPanel = nullptr;
};