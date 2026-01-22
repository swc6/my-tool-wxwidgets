#include "HtmlHelper.h"
#include <string>
#include <unordered_map>

HtmlHelper::HtmlHelper()
    : HelperBase(wxString::FromUTF8("文本处理"), wxString::FromUTF8("HTML 处理"))
{
    // 默认支持的 actions：encode / decode / filter tags
    SetActions({ wxString::FromUTF8("EncodeEntities"), wxString::FromUTF8("DecodeEntities"), wxString::FromUTF8("FilterTags") });
}

wxString HtmlHelper::ProcessAction(const wxString& action, const wxString& input)
{
    std::string act = std::string(action.ToUTF8());
    std::string in = std::string(input.ToUTF8());

    if (act == "EncodeEntities" || act == "encode" || act == "Encode") {
        return wxString::FromUTF8(EncodeEntities(in));
    } else if (act == "DecodeEntities" || act == "decode" || act == "Decode") {
        return wxString::FromUTF8(DecodeEntities(in));
    } else if (act == "FilterTags" || act == "filter" || act == "Filter") {
        return wxString::FromUTF8(FilterTags(in));
    }
    // 未知 action 原样返回
    return input;
}

std::string HtmlHelper::EncodeEntities(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
        case '&': out += "&amp;"; break;
        case '<': out += "&lt;"; break;
        case '>': out += "&gt;"; break;
        case '"': out += "&quot;"; break;
        case '\'': out += "&#39;"; break;
        default: out += c; break;
        }
    }
    return out;
}

std::string HtmlHelper::DecodeEntities(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '&') {
            if (s.compare(i, 5, "&amp;") == 0) { out += '&'; i += 4; continue; }
            if (s.compare(i, 4, "&lt;") == 0) { out += '<'; i += 3; continue; }
            if (s.compare(i, 4, "&gt;") == 0) { out += '>'; i += 3; continue; }
            if (s.compare(i, 6, "&quot;") == 0) { out += '"'; i += 5; continue; }
            if (s.compare(i, 5, "&#39;") == 0) { out += '\''; i += 4; continue; }
        }
        out += s[i];
    }
    return out;
}

std::string HtmlHelper::FilterTags(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    bool inTag = false;
    
    for (char c : s) {
        if (c == '<') {
            inTag = true;
        } else if (c == '>') {
            inTag = false;
        } else if (!inTag) {
            out += c;
        }
    }
    
    // 解码HTML实体，确保最终显示的是可读文本
    return DecodeEntities(out);
}