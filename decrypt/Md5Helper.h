#pragma once

#include "../HelperBase.h"

class Md5Helper : public HelperBase {
public:
    Md5Helper();
    ~Md5Helper() override = default;

protected:
    wxString ProcessAction(const wxString& action, const wxString& input) override;

private:
    // 用于将哈希结果转换为十六进制字符串
    static std::string HashToHex(size_t v);
    // 新增：用于将字节数组转换为十六进制字符串
    static std::string HashToHex(const unsigned char* data, size_t length);
};