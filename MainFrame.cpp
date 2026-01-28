#include "MainFrame.h"
#include "ui/TabPanel.h"
#include <wx/statline.h>
#include <wx/wrapsizer.h>
#include <vector>
#include <memory>

// helper 实现类头
#include "text/HtmlHelper.h"
#include "text/CssHelper.h"
#include "text/JsonHelper.h"
#include "text/JavaScriptHelper.h"
#include "text/TimeStampHelper.h"  // 添加这一行
#include "text/TextHelper.h"        // 添加这一行
#include "decrypt/Base64Helper.h"
#include "decrypt/Md5Helper.h"
#include "decrypt/ShaHelper.h"

namespace {
wxStaticText* FindFirstStaticText(wxWindow* parent) {
    if (!parent) return nullptr;
    wxStaticText* st = dynamic_cast<wxStaticText*>(parent);
    if (st) return st;
    const wxWindowList& children = parent->GetChildren();
    for (wxWindowList::const_iterator it = children.begin(); it != children.end(); ++it) {
        wxStaticText* found = FindFirstStaticText(*it);
        if (found) return found;
    }
    return nullptr;
}
// 根据左侧按钮文本创建对应的 Helper 实例（caller 负责释放）
HelperBase* CreateHelperForTool(const wxString& toolName) {
    if (toolName == wxString::FromUTF8("HTML 处理")) return new HtmlHelper();
    if (toolName == wxString::FromUTF8("CSS 处理")) return new CssHelper();
    if (toolName == wxString::FromUTF8("JSON 格式化")) return new JsonHelper();
    if (toolName == wxString::FromUTF8("JavaScript 处理")) return new JavaScriptHelper();
    if (toolName == wxString::FromUTF8("时间戳处理")) return new TimeStampHelper();  // 添加这一行
    if (toolName == wxString::FromUTF8("文字处理")) return new TextHelper();          // 添加这一行
    if (toolName == wxString::FromUTF8("MD5")) return new Md5Helper();
    if (toolName == wxString::FromUTF8("Base64")) return new Base64Helper();
    if (toolName == wxString::FromUTF8("SHA")) return new ShaHelper();
    return nullptr;
}
} // namespace

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, wxString::FromUTF8("我的工具箱"),
              wxDefaultPosition, wxSize(900, 600)),
      m_notebook(nullptr),
      m_newTabCounter(1),
      m_rightPanel(nullptr),
      m_rightLabel(nullptr),
      m_selectedButton(nullptr)
{
    // 设置主窗体图标
    wxIcon icon;
    wxString iconPath = wxString::FromUTF8("img/s_s.png");
    if (icon.LoadFile(iconPath, wxBITMAP_TYPE_PNG)) {
        SetIcon(icon);
    }

    // 主水平布局：左侧工具区（固定宽，滚动 + 自定义折叠分类），右侧工作区（占剩余空间）
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // 左侧：可滚动窗口
    wxScrolledWindow* leftScroll = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    leftScroll->SetScrollRate(5, 5);
    leftScroll->SetMinSize(wxSize(260, -1)); // 左栏宽度

    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    // Helper: 创建分类（去掉默认折叠箭头，使用中间横线与居中分类名；点击标题切换显示/隐藏内容）
    auto createCategory = [&](const wxString& title, const std::vector<wxString>& items) {
        wxPanel* header = new wxPanel(leftScroll, wxID_ANY);
        wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticLine* lineLeft = new wxStaticLine(header, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        wxStaticText* lbl = new wxStaticText(header, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
        wxStaticLine* lineRight = new wxStaticLine(header, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);

        headerSizer->Add(lineLeft, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 8);
        headerSizer->Add(lbl, 0, wxALIGN_CENTER_VERTICAL);
        headerSizer->Add(lineRight, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 8);
        header->SetSizer(headerSizer);

        wxPanel* content = new wxPanel(leftScroll, wxID_ANY);
        wxWrapSizer* contentSizer = new wxWrapSizer(wxHORIZONTAL);

        for (const auto& it : items) {
            wxButton* btn = new wxButton(content, wxID_ANY, it);
            btn->Bind(wxEVT_BUTTON, [this, btn, it](wxCommandEvent&) {
                // 恢复上一个选中按钮的默认颜色
                if (m_selectedButton && m_selectedButton != btn) {
                    m_selectedButton->SetBackgroundColour(wxNullColour);
                    m_selectedButton->SetForegroundColour(wxNullColour);
                    m_selectedButton->Refresh();
                }
                // 设置当前选中按钮的颜色标记
                m_selectedButton = btn;
                m_selectedButton->SetBackgroundColour(wxColour(200, 230, 255));
                m_selectedButton->SetForegroundColour(wxColour(0, 0, 0));
                m_selectedButton->Refresh();

                // 新逻辑：为每次点击创建对应 helper，并传入右侧新建 tab
                HelperBase* helper = CreateHelperForTool(it);
                CreateNewTab(helper);
            });
            contentSizer->Add(btn, wxSizerFlags().Border(wxALL, 4));
        }

        content->SetSizer(contentSizer);
        content->Layout();

        header->Bind(wxEVT_LEFT_UP, [content, leftScroll](wxMouseEvent&) {
            bool vis = content->IsShown();
            content->Show(!vis);
            leftScroll->Layout();
            leftScroll->FitInside();
        });
        lbl->Bind(wxEVT_LEFT_UP, [header](wxMouseEvent& ev){ header->GetEventHandler()->ProcessEvent(ev); });
        lineLeft->Bind(wxEVT_LEFT_UP, [header](wxMouseEvent& ev){ header->GetEventHandler()->ProcessEvent(ev); });
        lineRight->Bind(wxEVT_LEFT_UP, [header](wxMouseEvent& ev){ header->GetEventHandler()->ProcessEvent(ev); });

        leftSizer->Add(header, 0, wxEXPAND | wxALL, 6);
        leftSizer->Add(content, 0, wxEXPAND | wxLEFT | wxRIGHT, 6);
    };

    // 两个分类
    createCategory(wxString::FromUTF8("文本处理"),
                   { wxString::FromUTF8("文字处理"),
                     wxString::FromUTF8("HTML 处理"),
                     wxString::FromUTF8("CSS 处理"),
                     wxString::FromUTF8("JSON 格式化"),
                     wxString::FromUTF8("JavaScript 处理"),
                     wxString::FromUTF8("时间戳处理") });  // 添加这一行

    createCategory(wxString::FromUTF8("加密解密"),
                   { wxString::FromUTF8("MD5"), wxString::FromUTF8("Base64"), wxString::FromUTF8("SHA") });

    leftSizer->AddStretchSpacer(1);
    leftScroll->SetSizer(leftSizer);
    leftScroll->Layout();
    leftScroll->FitInside();

    // 右侧：使用 wxAuiNotebook，启用标签移动与所有标签的关闭按钮
    m_notebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                   wxAUI_NB_TAB_MOVE | wxAUI_NB_CLOSE_ON_ALL_TABS);

    // 默认创建一个初始页（欢迎页）
    CreateNewTab(nullptr);

    mainSizer->Add(leftScroll, 0, wxEXPAND);
    mainSizer->Add(m_notebook, 1, wxEXPAND);

    SetSizer(mainSizer);
    Layout();

    // 最小尺寸限制
    SetMinSize(wxSize(700, 600));
    SetSizeHints(700, 600, -1, -1);
    wxSize cur = GetSize();
    int w = wxMax(cur.GetWidth(), 900);
    int h = wxMax(cur.GetHeight(), 600);
    SetSize(wxSize(w, h));

    Centre();

    // 关闭改为隐藏
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    if (event.CanVeto()) {
        Hide();
        event.Veto();
    } else {
        event.Skip();
    }
}

void MainFrame::OnToolSelected(const wxString& toolName)
{
    // 兼容接口：直接创建 helper 并生成 tab
    HelperBase* helper = CreateHelperForTool(toolName);
    CreateNewTab(helper);
}


wxWindow* MainFrame::CreateNewTab(HelperBase* helper)
{
    wxString tabTitle;
    if (helper && !helper->GetName().IsEmpty()) tabTitle = helper->GetName();
    else if (!helper) tabTitle = wxString::FromUTF8("欢迎");
    else tabTitle = wxString::Format(wxString::FromUTF8("工作页 %d"), m_newTabCounter++);

    TabPanel* page = new TabPanel(m_notebook, helper);
    m_notebook->AddPage(page, tabTitle, true);
    return page;
}