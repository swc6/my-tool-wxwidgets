#include "Md5Helper.h"
#include <string>
#include <cryptlib.h>
#include <md5.h>
#include <hex.h>
#include <sstream>
#include <iomanip>

Md5Helper::Md5Helper()
    : HelperBase(wxString::FromUTF8("加密解密"), wxString::FromUTF8("MD5"))
{
    SetActions({ wxString::FromUTF8("16位小写"), wxString::FromUTF8("16位大写"), 
                 wxString::FromUTF8("32位小写"), wxString::FromUTF8("32位大写") });
}

wxString Md5Helper::ProcessAction(const wxString& action, const wxString& input)
{
    std::string act = std::string(action.ToUTF8());
    std::string in = std::string(input.ToUTF8());
    
    // 使用Crypto++的MD5实现
    CryptoPP::MD5 hash;
    std::string digest;
    
    // 计算完整的32位MD5哈希值
    CryptoPP::StringSource(in, true, 
        new CryptoPP::HashFilter(hash, 
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest),
                false // 不使用大写
            )
        )
    );
    
    // 根据不同的action参数，返回不同格式的MD5哈希值
    if (act == "32位小写") {
        // 32位小写（已经是小写）
        return wxString::FromUTF8(digest.c_str());
    } else if (act == "32位大写") {
        // 32位大写
        std::string upperDigest;
        for (char c : digest) {
            upperDigest += std::toupper(c);
        }
        return wxString::FromUTF8(upperDigest.c_str());
    } else if (act == "16位小写") {
        // 16位小写（取中间8字节）
        std::string shortDigest = digest.substr(8, 16);
        return wxString::FromUTF8(shortDigest.c_str());
    } else if (act == "16位大写") {
        // 16位大写（取中间8字节并转换为大写）
        std::string shortDigest = digest.substr(8, 16);
        std::string upperShortDigest;
        for (char c : shortDigest) {
            upperShortDigest += std::toupper(c);
        }
        return wxString::FromUTF8(upperShortDigest.c_str());
    }
    
    return input;
}

std::string Md5Helper::HashToHex(size_t v)
{
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    ss << std::setw(sizeof(size_t)*2) << (v & (((size_t)1 << (sizeof(size_t)*8)) - 1));
    return ss.str();
}