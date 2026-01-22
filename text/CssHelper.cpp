#include "CssHelper.h"
#include <string>

CssHelper::CssHelper()
    : HelperBase(wxString::FromUTF8("文本处理"), wxString::FromUTF8("CSS 处理"))
{
    SetActions({ wxString::FromUTF8("Minify"), wxString::FromUTF8("Format") });
}

wxString CssHelper::ProcessAction(const wxString& action, const wxString& input)
{
    std::string act = std::string(action.ToUTF8());
    std::string in = std::string(input.ToUTF8());

    if (act == "Minify" || act == "minify") {
        return wxString::FromUTF8(MinifyCss(in));
    } else if (act == "Format" || act == "format") {
        return wxString::FromUTF8(FormatCss(in));
    }

    return input;
}

// 简单 CSS 压缩：移除注释与多余空白（非完整实现，仅适合常规场景）
std::string CssHelper::MinifyCss(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    bool inComment = false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (!inComment && i + 1 < s.size() && s[i] == '/' && s[i+1] == '*') {
            inComment = true; i++;
            continue;
        }
        if (inComment) {
            if (i + 1 < s.size() && s[i] == '*' && s[i+1] == '/') { inComment = false; i++; }
            continue;
        }
        char c = s[i];
        if (isspace(static_cast<unsigned char>(c))) {
            // 将连续空白缩为单个空格，且去掉在特殊符号前后的空格
            if (!out.empty() && !isspace(static_cast<unsigned char>(out.back()))) {
                out += ' ';
            }
        } else {
            // 删除空格周围的不必要空格
            if (!out.empty() && out.back() == ' ') {
                char prev = out.size() >= 2 ? out[out.size()-2] : '\0';
                if (c == '{' || c == '}' || c == ':' || c == ';' || c == ',' || c == '>') {
                    out.pop_back();
                }
            }
            out += c;
        }
    }
    // 去掉多余空格
    std::string trimmed;
    for (size_t i = 0; i < out.size(); ++i) {
        if (i == 0 || !(out[i] == ' ' && out[i-1] == ' ')) trimmed += out[i];
    }
    return trimmed;
}

// CSS 格式化：添加适当的缩进和换行（非完整实现，仅适合常规场景）
std::string CssHelper::FormatCss(const std::string& s)
{
    std::string out;
    out.reserve(s.size() * 2); // 预留足够空间
    int indentLevel = 0;
    const std::string indent = "  "; // 2个空格缩进
    bool inComment = false;
    bool inString = false;
    char stringDelimiter = '\0';
    
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        
        // 处理注释
        if (!inString) {
            if (!inComment && i + 1 < s.size() && c == '/' && s[i+1] == '*') {
                inComment = true;
                out += "/*";
                i++;
                continue;
            }
            if (inComment) {
                out += c;
                if (i + 1 < s.size() && c == '*' && s[i+1] == '/') {
                    out += '/';
                    inComment = false;
                    i++;
                }
                continue;
            }
        }
        
        // 处理字符串
        if (inComment) {
            out += c;
            continue;
        }
        
        if ((c == '"' || c == '\'') && (i == 0 || s[i-1] != '\\')) {
            if (!inString) {
                inString = true;
                stringDelimiter = c;
            } else if (c == stringDelimiter) {
                inString = false;
                stringDelimiter = '\0';
            }
            out += c;
            continue;
        }
        
        if (inString) {
            out += c;
            continue;
        }
        
        // 处理 CSS 结构
        switch (c) {
            case '{':
                out += " {\n";
                indentLevel++;
                for (int j = 0; j < indentLevel; j++) {
                    out += indent;
                }
                break;
            case '}':
                out += "\n";
                if (indentLevel > 0) {
                    indentLevel--;
                }
                for (int j = 0; j < indentLevel; j++) {
                    out += indent;
                }
                out += '}';
                if (i + 1 < s.size() && s[i+1] != ';' && s[i+1] != '}') {
                    out += '\n';
                    for (int j = 0; j < indentLevel; j++) {
                        out += indent;
                    }
                }
                break;
            case ';':
                out += ';';
                if (i + 1 < s.size() && s[i+1] != '}') {
                    out += '\n';
                    for (int j = 0; j < indentLevel; j++) {
                        out += indent;
                    }
                }
                break;
            case ',':
                out += ",\n";
                for (int j = 0; j < indentLevel; j++) {
                    out += indent;
                }
                break;
            case '\n':
            case '\r':
            case '\t':
            case ' ':
                // 忽略多余的空白字符
                if (!out.empty() && !isspace(static_cast<unsigned char>(out.back()))) {
                    out += ' ';
                }
                break;
            default:
                out += c;
                break;
        }
    }
    
    // 清理末尾多余的空白
    while (!out.empty() && isspace(static_cast<unsigned char>(out.back()))) {
        out.pop_back();
    }
    
    out += '\n';
    return out;
}