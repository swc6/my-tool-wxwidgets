#include "TabPanel.h"
#include <wx/font.h>
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/statline.h>


TabPanel::TabPanel(wxWindow* parent, HelperBase* helper)
    : wxPanel(parent, wxID_ANY)
    , m_helper(helper)
{
    // 欢迎 / 占位页
    if (!m_helper) {
        wxBoxSizer* welcomeSizer = new wxBoxSizer(wxVERTICAL);
        wxStaticText* title = new wxStaticText(this, wxID_ANY, wxString::FromUTF8("欢迎使用 我的工具箱"),
                                               wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
        title->SetFont(wxFont(wxFontInfo(16).Family(wxFONTFAMILY_DEFAULT).Weight(wxFONTWEIGHT_BOLD)));
        welcomeSizer->Add(title, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 20);

        wxStaticText* info = new wxStaticText(this, wxID_ANY,
            wxString::FromUTF8("从左侧选择一个工具开始。\n每次点击会打开一个新的工作页，工作页包含输入、操作按钮与结果区。"),
            wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
        welcomeSizer->Add(info, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 12);

        wxButton* demo = new wxButton(this, wxID_ANY, wxString::FromUTF8("示例：打开 HTML 处理"));
        demo->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
            wxMessageBox(wxString::FromUTF8("请从左侧选择工具以打开新工作页"), wxString::FromUTF8("提示"), wxOK | wxICON_INFORMATION, this);
        });
        welcomeSizer->Add(demo, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 20);

        SetSizer(welcomeSizer);
        Layout();
        return;
    }

    // 设置helper的tabPanel指针
    m_helper->SetTabPanel(this);

    // 三列布局：左 输入 / 中 操作按钮 / 右 结果
    wxBoxSizer* main = new wxBoxSizer(wxHORIZONTAL);

    // 左：输入
    m_input = new wxTextCtrl(this, wxID_ANY, m_helper->GetDoText(),
                             wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    main->Add(m_input, 1, wxEXPAND | wxALL, 6);

    // 中：action 按钮列（先放 helper 自带按钮，之后放重置/复制）
    wxBoxSizer* midSizer = new wxBoxSizer(wxVERTICAL);

    // Helper 自带的 action 按钮（若有）
    if (m_helper->HasActions()) {
        const auto& acts = m_helper->GetActions();
        for (size_t i = 0; i < acts.size(); ++i) {
            wxButton* btn = new wxButton(this, wxID_ANY, acts[i]);
            btn->Bind(wxEVT_BUTTON, [this, i](wxCommandEvent&) {
                if (!m_helper) return;
                m_helper->SetDoText(m_input->GetValue());
                m_helper->RunActionAt(static_cast<int>(i));
                if (m_result) m_result->SetValue(m_helper->GetResultText());
            });
            midSizer->Add(btn, 0, wxEXPAND | wxALL, 4);
        }
    } else {
        // 无 action 时显示占位
        midSizer->Add(new wxStaticText(this, wxID_ANY, wxString::FromUTF8("无操作")), 0, wxALL, 6);
    }

    // 分隔线
    midSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 4);

    // Reset 按钮：清空输入与输出，并重置 helper 内部输入
    wxButton* resetBtn = new wxButton(this, wxID_ANY, wxString::FromUTF8("重置"));
    resetBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        if (m_input) m_input->Clear();
        if (m_result) m_result->Clear();
        if (m_helper) m_helper->SetDoText(wxEmptyString);
    });
    midSizer->Add(resetBtn, 0, wxEXPAND | wxALL, 4);

    // Copy 按钮：复制输出内容到系统剪贴板
    wxButton* copyBtn = new wxButton(this, wxID_ANY, wxString::FromUTF8("复制"));
    copyBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        if (!m_result) return;
        wxString text = m_result->GetValue();
        if (text.IsEmpty()) return;
        if (wxTheClipboard->Open()) {
            wxTextDataObject* data = new wxTextDataObject(text);
            wxTheClipboard->SetData(data); // clipboard takes ownership
            wxTheClipboard->Close();
        } else {
            wxMessageBox(wxString::FromUTF8("无法打开剪贴板"), wxString::FromUTF8("错误"), wxOK | wxICON_ERROR, this);
        }
    });
    midSizer->Add(copyBtn, 0, wxEXPAND | wxALL, 4);

    main->Add(midSizer, 0, wxEXPAND | wxALL, 6);

    // 右：结果（只读）
    m_result = new wxTextCtrl(this, wxID_ANY, m_helper->GetResultText(),
                              wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    main->Add(m_result, 1, wxEXPAND | wxALL, 6);

    SetSizer(main);
    Layout();
}

TabPanel::~TabPanel()
{
    delete m_helper;
    m_helper = nullptr;
}

void TabPanel::SetInputText(const wxString& text)
{
    if (m_input) {
        m_input->SetValue(text);
    }
}