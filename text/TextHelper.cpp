#include "TextHelper.h"
#include <sstream>
#include <set>
#include <string>

TextHelper::TextHelper() {
    SetName(wxString::FromUTF8("文字处理"));
    SetCategory(wxString::FromUTF8("文本处理"));
    // 添加操作：去除重复行
    SetActions({ wxString::FromUTF8("去除重复行") });
}

wxString TextHelper::ProcessAction(const wxString& action, const wxString& input) {
    if (action == wxString::FromUTF8("去除重复行")) {
        std::string stdInput = input.ToStdString();
        std::string result = RemoveDuplicateLines(stdInput);
        return wxString::FromUTF8(result.c_str());
    }
    return input; // 默认返回原输入
}

std::string TextHelper::RemoveDuplicateLines(const std::string& input) {
    std::istringstream iss(input);
    std::ostringstream oss;
    std::set<std::string> seenLines;
    std::string line;

    while (std::getline(iss, line)) {
        // 如果该行之前没有出现过，则添加到结果中
        if (seenLines.find(line) == seenLines.end()) {
            seenLines.insert(line);
            oss << line << '\n';
        }
    }

    return oss.str();
}
