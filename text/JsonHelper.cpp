#include "JsonHelper.h"
#include <string>

JsonHelper::JsonHelper()
    : HelperBase(wxString::FromUTF8("文本处理"), wxString::FromUTF8("JSON 格式化"))
{
    SetActions({ wxString::FromUTF8("Format"), wxString::FromUTF8("Minify") });
}

wxString JsonHelper::ProcessAction(const wxString& action, const wxString& input)
{
    std::string act = std::string(action.ToUTF8());
    std::string in = std::string(input.ToUTF8());
    if (act == "Format" || act == "format") {
        return wxString::FromUTF8(PrettyPrintJson(in));
    } else if (act == "Minify" || act == "minify") {
        return wxString::FromUTF8(MinifyJson(in));
    }
    return input;
}

// 非严格 JSON 格式化：按括号/逗号/冒号做缩进，简单处理字符串与转义
std::string JsonHelper::PrettyPrintJson(const std::string& s)
{
    std::string out;
    int indent = 0;
    bool inQuotes = false;
    bool escape = false;
    const std::string indentStr = "  ";
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (escape) { out += c; escape = false; continue; }
        if (c == '\\') { out += c; escape = true; continue; }
        if (c == '"') { inQuotes = !inQuotes; out += c; continue; }
        if (inQuotes) { out += c; continue; }
        if (c == '{' || c == '[') {
            out += c;
            out += '\n';
            indent++;
            out.append(indent * indentStr.size(), ' ');
            continue;
        }
        if (c == '}' || c == ']') {
            out += '\n';
            indent = std::max(0, indent - 1);
            out.append(indent * indentStr.size(), ' ');
            out += c;
            continue;
        }
        if (c == ',') {
            out += c;
            out += '\n';
            out.append(indent * indentStr.size(), ' ');
            continue;
        }
        if (c == ':') {
            out += c;
            out += ' ';
            continue;
        }
        if (isspace(static_cast<unsigned char>(c))) continue;
        out += c;
    }
    return out;
}

std::string JsonHelper::MinifyJson(const std::string& s)
{
    std::string out;
    bool inQuotes = false;
    bool escape = false;
    for (char c : s) {
        if (escape) { out += c; escape = false; continue; }
        if (c == '\\') { out += c; escape = true; continue; }
        if (c == '"') { out += c; inQuotes = !inQuotes; continue; }
        if (inQuotes) { out += c; continue; }
        if (!isspace(static_cast<unsigned char>(c))) out += c;
    }
    return out;
}