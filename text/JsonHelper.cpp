#include "JsonHelper.h"
#include <string>

JsonHelper::JsonHelper()
    : HelperBase(wxString::FromUTF8("文本处理"), wxString::FromUTF8("JSON 格式化"))
{
    SetActions({ wxString::FromUTF8("Format"), wxString::FromUTF8("Minify"), wxString::FromUTF8("提取格式") });
}

wxString JsonHelper::ProcessAction(const wxString& action, const wxString& input)
{
    std::string act = std::string(action.ToUTF8());
    std::string in = std::string(input.ToUTF8());
    if (act == "Format" || act == "format") {
        return wxString::FromUTF8(PrettyPrintJson(in));
    } else if (act == "Minify" || act == "minify") {
        return wxString::FromUTF8(MinifyJson(in));
    } else if (act == "提取格式") {
        std::string result = ExtractFormat(in);
        return wxString::FromUTF8(result.c_str());
    }
    return input; // 默认返回原输入
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

std::string JsonHelper::ExtractFormat(const std::string& s)
{
    std::string minified = MinifyJson(s);
    std::string result;
    int braceCount = 0;
    int bracketCount = 0;
    bool inQuotes = false;
    bool escape = false;
    
    for (size_t i = 0; i < minified.size(); ++i) {
        char c = minified[i];
        
        if (escape) {
            result += c;
            escape = false;
            continue;
        }
        
        if (c == '\\') {
            result += c;
            escape = true;
            continue;
        }
        
        if (c == '"') {
            result += c;
            inQuotes = !inQuotes;
            continue;
        }
        
        if (inQuotes) {
            result += c;
            continue;
        }
        
        if (c == '{') {
            result += c;
            braceCount++;
            continue;
        }
        
        if (c == '}') {
            result += c;
            braceCount--;
            continue;
        }
        
        if (c == '[') {
            result += c;
            bracketCount++;
            
            // 找到数组的结束位置
            size_t endPos = i + 1;
            int tempBracketCount = 1;
            bool tempInQuotes = false;
            bool tempEscape = false;
            
            // 收集数组中的所有元素
            std::vector<std::string> elements;
            size_t elementStart = endPos;
            int elementDepth = 0;
            int objectDepth = 0;
            
            while (endPos < minified.size() && tempBracketCount > 0) {
                char tempC = minified[endPos];
                
                if (tempEscape) {
                    tempEscape = false;
                } else if (tempC == '\\') {
                    tempEscape = true;
                } else if (tempC == '"') {
                    tempInQuotes = !tempInQuotes;
                } else if (!tempInQuotes) {
                    if (tempC == '[') {
                        tempBracketCount++;
                        elementDepth++;
                    } else if (tempC == ']') {
                        tempBracketCount--;
                        elementDepth--;
                        if (tempBracketCount == 0 && elementStart < endPos) {
                            // 最后一个元素
                            std::string element = minified.substr(elementStart, endPos - elementStart);
                            elements.push_back(element);
                        }
                    } else if (tempC == '{') {
                        objectDepth++;
                    } else if (tempC == '}') {
                        objectDepth--;
                    } else if (tempC == ',' && elementDepth == 0 && objectDepth == 0) {
                        // 找到一个元素的结束位置
                        std::string element = minified.substr(elementStart, endPos - elementStart);
                        elements.push_back(element);
                        elementStart = endPos + 1;
                    }
                }
                
                endPos++;
            }
            
            if (tempBracketCount == 0) {
                // 如果是一个空数组，直接添加
                if (endPos == i + 1) {
                    result += ']';
                    bracketCount--;
                    i = endPos - 1;
                    continue;
                }
                
                // 找到最短的元素
                if (!elements.empty()) {
                    std::string shortestElement = elements[0];
                    size_t minLength = shortestElement.size();
                    
                    for (const std::string& element : elements) {
                        if (element.size() < minLength) {
                            shortestElement = element;
                            minLength = element.size();
                        }
                    }
                    
                    // 添加最短的元素
                    result += shortestElement;
                }
                
                // 添加数组结束符
                result += ']';
                bracketCount--;
                i = endPos - 1;
                continue;
            }
        }
        
        if (c == ']') {
            result += c;
            bracketCount--;
            continue;
        }
        
        result += c;
    }
    
    // 对结果进行格式化
    return PrettyPrintJson(result);
}