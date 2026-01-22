#pragma once

#include "../HelperBase.h"

class ShaHelper : public HelperBase {
public:
    ShaHelper();
    ~ShaHelper() override = default;

protected:
    wxString ProcessAction(const wxString& action, const wxString& input) override;
};