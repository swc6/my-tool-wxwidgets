#pragma once

#include "../HelperBase.h"

class TextHelper : public HelperBase {
public:
    TextHelper();
    ~TextHelper() override = default;

protected:
    wxString ProcessAction(const wxString& action, const wxString& input) override;

private:
    static std::string RemoveDuplicateLines(const std::string& input);
};
