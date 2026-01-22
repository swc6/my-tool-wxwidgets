#pragma once

#include "../HelperBase.h"

class CssHelper : public HelperBase {
public:
    CssHelper();
    ~CssHelper() override = default;

protected:
    wxString ProcessAction(const wxString& action, const wxString& input) override;

private:
    static std::string MinifyCss(const std::string& s);
    static std::string FormatCss(const std::string& s);
};