#include "ShaHelper.h"
#include <string>
#include <cryptlib.h>
#include <sha.h>
#include <hex.h>

ShaHelper::ShaHelper()
    : HelperBase(wxString::FromUTF8("加密解密"), wxString::FromUTF8("SHA"))
{
    SetActions({ wxString::FromUTF8("SHA1"), wxString::FromUTF8("SHA224"), 
                 wxString::FromUTF8("SHA256"), wxString::FromUTF8("SHA384"), 
                 wxString::FromUTF8("SHA512") });
}

wxString ShaHelper::ProcessAction(const wxString& action, const wxString& input)
{
    std::string act = std::string(action.ToUTF8());
    std::string in = std::string(input.ToUTF8());
    std::string digest;
    
    if (act == "SHA1") {
        // 使用Crypto++的SHA1实现
        CryptoPP::SHA1 hash;
        CryptoPP::StringSource(in, true, 
            new CryptoPP::HashFilter(hash, 
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest),
                    false // 不使用大写
                )
            )
        );
    } else if (act == "SHA224") {
        // 使用Crypto++的SHA224实现
        CryptoPP::SHA224 hash;
        CryptoPP::StringSource(in, true, 
            new CryptoPP::HashFilter(hash, 
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest),
                    false // 不使用大写
                )
            )
        );
    } else if (act == "SHA256") {
        // 使用Crypto++的SHA256实现
        CryptoPP::SHA256 hash;
        CryptoPP::StringSource(in, true, 
            new CryptoPP::HashFilter(hash, 
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest),
                    false // 不使用大写
                )
            )
        );
    } else if (act == "SHA384") {
        // 使用Crypto++的SHA384实现
        CryptoPP::SHA384 hash;
        CryptoPP::StringSource(in, true, 
            new CryptoPP::HashFilter(hash, 
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest),
                    false // 不使用大写
                )
            )
        );
    } else if (act == "SHA512") {
        // 使用Crypto++的SHA512实现
        CryptoPP::SHA512 hash;
        CryptoPP::StringSource(in, true, 
            new CryptoPP::HashFilter(hash, 
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest),
                    false // 不使用大写
                )
            )
        );
    }
    
    return wxString::FromUTF8(digest.c_str());
}