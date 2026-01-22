#pragma once

#include "../HelperBase.h"

class JsonHelper : public HelperBase {
public:
    JsonHelper();
    ~JsonHelper() override = default;

protected:
    wxString ProcessAction(const wxString& action, const wxString& input) override;

private:
    // 简单 JSON 格式化（非严格解析，仅做友好缩进）
    static std::string PrettyPrintJson(const std::string& s);
    static std::string MinifyJson(const std::string& s);
};