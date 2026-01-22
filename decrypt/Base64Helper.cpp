#include "Base64Helper.h"
#include <string>
#include <vector>

Base64Helper::Base64Helper()
    : HelperBase(wxString::FromUTF8("加密解密"), wxString::FromUTF8("Base64"))
{
    SetActions({ wxString::FromUTF8("Encode"), wxString::FromUTF8("Decode") });
}

wxString Base64Helper::ProcessAction(const wxString& action, const wxString& input)
{
    std::string act = std::string(action.ToUTF8());
    std::string in = std::string(input.ToUTF8());
    if (act == "Encode" || act == "encode") {
        return wxString::FromUTF8(EncodeBase64(in));
    } else if (act == "Decode" || act == "decode") {
        return wxString::FromUTF8(DecodeBase64(in));
    }
    return input;
}

// 简单 Base64 实现（RFC 4648）
static const char* b64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string Base64Helper::EncodeBase64(const std::string& in)
{
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(b64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(b64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

static inline int b64_val(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

std::string Base64Helper::DecodeBase64(const std::string& in)
{
    std::vector<unsigned char> out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; ++i) T[(unsigned char)b64_chars[i]] = i;
    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) {
            if (c == '=') break;
            else continue;
        }
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return std::string(out.begin(), out.end());
}