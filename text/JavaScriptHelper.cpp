#include "JavaScriptHelper.h"
#include <string>
#include <unordered_map>
#include <unordered_set>  // 添加这个头文件
#include <regex>
#include <random>

JavaScriptHelper::JavaScriptHelper()
    : HelperBase(wxString::FromUTF8("文本处理"), wxString::FromUTF8("JavaScript 处理"))
{
    SetActions({ wxString::FromUTF8("Minify"), wxString::FromUTF8("混淆") });
}

wxString JavaScriptHelper::ProcessAction(const wxString& action, const wxString& input)
{
    std::string act = std::string(action.ToUTF8());
    std::string in = std::string(input.ToUTF8());
    if (act == "Minify" || act == "minify") {
        return wxString::FromUTF8(MinifyJs(in));
    } else if (act == "混淆" || act == "Obfuscate" || act == "obfuscate") {
        return wxString::FromUTF8(ObfuscateJs(in));
    }
    return input;
}

// 非严格的 JS 压缩：移除 // 注释、/* */ 注释 和多余空白（仅作简单示例）
std::string JavaScriptHelper::MinifyJs(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    bool inString = false;
    char stringDelim = 0;
    bool escape = false;
    bool inLineComment = false;
    bool inBlockComment = false;

    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (inLineComment) {
            if (c == '\n') inLineComment = false;
            else continue;
        }
        if (inBlockComment) {
            if (c == '*' && i + 1 < s.size() && s[i+1] == '/') { inBlockComment = false; ++i; }
            continue;
        }
        if (escape) { out += c; escape = false; continue; }
        if (c == '\\') { out += c; escape = true; continue; }
        if (!inString) {
            if (c == '/' && i + 1 < s.size() && s[i+1] == '/') { inLineComment = true; ++i; continue; }
            if (c == '/' && i + 1 < s.size() && s[i+1] == '*') { inBlockComment = true; ++i; continue; }
        }
        if ((c == '"' || c == '\'' || c == '`') && !inString) {
            inString = true; stringDelim = c; out += c; continue;
        } else if (inString && c == stringDelim) {
            inString = false; out += c; continue;
        }
        if (!inString && isspace(static_cast<unsigned char>(c))) {
            // collapse whitespace to single space where appropriate
            if (!out.empty() && !isspace(static_cast<unsigned char>(out.back()))) out += ' ';
            continue;
        }
        out += c;
    }

    // 最后删除多余空格（简单）
    std::string trimmed;
    for (size_t i = 0; i < out.size(); ++i) {
        if (i == 0 || !(out[i] == ' ' && out[i-1] == ' ')) trimmed += out[i];
    }
    return trimmed;
}

// JavaScript 简单混淆实现
std::string JavaScriptHelper::ObfuscateJs(const std::string& s)
{
    // 首先进行压缩
    std::string compressed = MinifyJs(s);
    std::string out;
    out.reserve(compressed.size());
    
    // JavaScript 关键字和内置函数名，这些不应该被重命名
    static const std::unordered_set<std::string> keywords = {
        "break", "case", "catch", "class", "const", "continue", "debugger", "default", 
        "delete", "do", "else", "enum", "export", "extends", "false", "finally", 
        "for", "function", "if", "import", "in", "instanceof", "new", "null", 
        "return", "super", "switch", "this", "throw", "true", "try", "typeof", 
        "var", "void", "while", "with", "yield", "async", "await", "static", 
        "let", "of", "from", "as", "implements", "interface", "package", "private", 
        "protected", "public", "static", "let", "async", "await", "of",
        // 常用内置函数和对象
        "Object", "Array", "Function", "String", "Number", "Boolean", "Symbol", 
        "Date", "RegExp", "Promise", "Map", "Set", "WeakMap", "WeakSet", 
        "ArrayBuffer", "DataView", "TypedArray", "Int8Array", "Uint8Array", 
        "Uint8ClampedArray", "Int16Array", "Uint16Array", "Int32Array", "Uint32Array", 
        "Float32Array", "Float64Array", "Math", "JSON", "globalThis", "console", 
        "alert", "confirm", "prompt", "setTimeout", "setInterval", "clearTimeout", 
        "clearInterval", "document", "window", "navigator", "location", "history"
    };
    
    // 变量名映射表
    std::unordered_map<std::string, std::string> varMap;
    
    // 生成随机变量名的函数
    auto generateVarName = []() -> std::string {
        static const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> lenDist(2, 6);
        static std::uniform_int_distribution<> charDist(0, chars.size() - 1);
        
        int len = lenDist(gen);
        std::string name;
        name.reserve(len);
        
        // 确保变量名以字母开头
        name += chars[charDist(gen)];
        
        for (int i = 1; i < len; ++i) {
            name += chars[charDist(gen)];
        }
        
        return name;
    };
    
    // 处理字符串，避免替换字符串中的内容
    bool inString = false;
    char stringDelim = 0;
    bool escape = false;
    size_t pos = 0;
    
    while (pos < compressed.size()) {
        char c = compressed[pos];
        
        if (escape) {
            out += c;
            escape = false;
            pos++;
            continue;
        }
        
        if (c == '\\') {
            out += c;
            escape = true;
            pos++;
            continue;
        }
        
        if (!inString && (c == '"' || c == '\'' || c == '`')) {
            inString = true;
            stringDelim = c;
            out += c;
            pos++;
            continue;
        }
        
        if (inString && c == stringDelim) {
            inString = false;
            out += c;
            pos++;
            continue;
        }
        
        if (inString) {
            out += c;
            pos++;
            continue;
        }
        
        // 不在字符串中，尝试匹配变量名
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '$') {
            // 开始匹配变量名
            size_t varStart = pos;
            while (pos < compressed.size() && 
                   (std::isalnum(static_cast<unsigned char>(compressed[pos])) || 
                    compressed[pos] == '_' || 
                    compressed[pos] == '$')) {
                pos++;
            }
            
            std::string varName(compressed.substr(varStart, pos - varStart));
            
            // 检查是否是关键字或已经映射过的变量名
            if (keywords.find(varName) == keywords.end()) {
                if (varMap.find(varName) == varMap.end()) {
                    // 生成新的变量名
                    std::string newName;
                    do {
                        newName = generateVarName();
                    } while (keywords.find(newName) != keywords.end() || varMap.find(newName) != varMap.end());
                    
                    varMap[varName] = newName;
                }
                
                out += varMap[varName];
            } else {
                // 是关键字，保持不变
                out += varName;
            }
        } else {
            // 不是变量名，直接添加
            out += c;
            pos++;
        }
    }
    
    return out;
}