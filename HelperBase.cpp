#include "HelperBase.h"
#include "ui/TabPanel.h"

HelperBase::HelperBase(const wxString& category, const wxString& name)
    : m_category(category),
    m_name(name),
    m_doText(),
    m_resultText(),
    m_actions(),
    m_currentActionIndex(-1),
    m_tabPanel(nullptr)
{
}

wxString HelperBase::GetCategory() const noexcept { return m_category; }
wxString HelperBase::GetName() const noexcept { return m_name; }
wxString HelperBase::GetDoText() const noexcept { return m_doText; }
wxString HelperBase::GetResultText() const noexcept { return m_resultText; }
const std::vector<wxString>& HelperBase::GetActions() const noexcept { return m_actions; }
std::optional<int> HelperBase::GetCurrentActionIndex() const noexcept {
    return (m_currentActionIndex >= 0 && m_currentActionIndex < static_cast<int>(m_actions.size()))
        ? std::optional<int>(m_currentActionIndex) : std::optional<int>();
}
TabPanel* HelperBase::GetTabPanel() const noexcept { return m_tabPanel; }

void HelperBase::SetCategory(const wxString& category) noexcept { m_category = category; }
void HelperBase::SetName(const wxString& name) noexcept { m_name = name; }
void HelperBase::SetDoText(const wxString& text) noexcept { m_doText = text; }
void HelperBase::SetActions(const std::vector<wxString>& actions) noexcept {
    m_actions = actions;
    // 重置当前索引
    m_currentActionIndex = m_actions.empty() ? -1 : 0;
}
void HelperBase::ClearActions() noexcept {
    m_actions.clear();
    m_currentActionIndex = -1;
}
void HelperBase::SetTabPanel(TabPanel* tabPanel) noexcept { m_tabPanel = tabPanel; }

bool HelperBase::SetCurrentActionIndex(int idx) noexcept {
    if (idx < 0 || idx >= static_cast<int>(m_actions.size())) return false;
    m_currentActionIndex = idx;
    return true;
}

bool HelperBase::HasActions() const noexcept { return !m_actions.empty(); }

bool HelperBase::RunActionAt(int index)
{
    if (index < 0 || index >= static_cast<int>(m_actions.size())) return false;
    wxString action = m_actions[index];
    wxString input = m_doText;
    // 调用派生类实现
    wxString out = ProcessAction(action, input);
    m_resultText = out;
    m_currentActionIndex = index;
    return true;
}

bool HelperBase::RunCurrentAction()
{
    if (m_currentActionIndex < 0 || m_currentActionIndex >= static_cast<int>(m_actions.size())) return false;
    return RunActionAt(m_currentActionIndex);
}

wxString HelperBase::RunAllActions()
{
    if (m_actions.empty()) {
        m_resultText.clear();
        return m_resultText;
    }
    wxString input = m_doText;
    for (size_t i = 0; i < m_actions.size(); ++i) {
        input = ProcessAction(m_actions[i], input);
    }
    m_resultText = input;
    // 最后一个 action 索引
    m_currentActionIndex = static_cast<int>(m_actions.size()) - 1;
    return m_resultText;
}