#pragma once

#include "../HelperBase.h"

class TimeStampHelper : public HelperBase {
public:
    TimeStampHelper();
    ~TimeStampHelper() override = default;

protected:
    // 实现具体 action 处理
    wxString ProcessAction(const wxString& action, const wxString& input) override;

private:
    // 将时间戳转换为时间格式
    static std::string TimestampToDateTime(const std::string& timestamp);
    // 将时间格式转换为时间戳
    static std::string DateTimeToTimestamp(const std::string& datetime);
};