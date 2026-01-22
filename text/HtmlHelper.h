#pragma once

#include "../HelperBase.h"

class HtmlHelper : public HelperBase {
public:
    HtmlHelper();
    ~HtmlHelper() override = default;

protected:
    // 实现具体 action 处理
    wxString ProcessAction(const wxString& action, const wxString& input) override;

private:
    // 简单实体编码/解码辅助
    static std::string EncodeEntities(const std::string& s);
    static std::string DecodeEntities(const std::string& s);
    // 过滤HTML标签，仅保留文本内容
    static std::string FilterTags(const std::string& s);
};