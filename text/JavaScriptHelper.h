#pragma once

#include "../HelperBase.h"

class JavaScriptHelper : public HelperBase {
public:
    JavaScriptHelper();
    ~JavaScriptHelper() override = default;

protected:
    wxString ProcessAction(const wxString& action, const wxString& input) override;

private:
    static std::string MinifyJs(const std::string& s);
    static std::string ObfuscateJs(const std::string& s);
};