#pragma once

#include "../HelperBase.h"

class Base64Helper : public HelperBase {
public:
    Base64Helper();
    ~Base64Helper() override = default;

protected:
    wxString ProcessAction(const wxString& action, const wxString& input) override;

private:
    static std::string EncodeBase64(const std::string& in);
    static std::string DecodeBase64(const std::string& in);
};