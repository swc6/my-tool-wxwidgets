#include "TimeStampHelper.h"
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include "../ui/TabPanel.h"

TimeStampHelper::TimeStampHelper()
    : HelperBase(wxString::FromUTF8("文本处理"), wxString::FromUTF8("时间戳处理"))
{
    // 默认支持的 actions：当前时间、时间戳转时间格式、时间格式转时间戳
    SetActions({ wxString::FromUTF8("当前时间"), wxString::FromUTF8("时间戳转时间"), wxString::FromUTF8("时间转时间戳") });
}

wxString TimeStampHelper::ProcessAction(const wxString& action, const wxString& input)
{
    std::string act = std::string(action.ToUTF8());
    std::string in = std::string(input.ToUTF8());

    if (act == "当前时间" || act == "current_time") {
        // 获取当前系统时间
        std::time_t now = std::time(nullptr);
        std::tm* local_time = std::localtime(&now);
        
        // 将当前时间设置为输入文本
        std::ostringstream time_stream;
        time_stream << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
        wxString time_str = wxString::FromUTF8(time_stream.str());
        
        // 修改输入文本
        SetDoText(time_str);
        
        // 使用tabPanel指针更新左侧面板显示
        if (GetTabPanel()) {
            GetTabPanel()->SetInputText(time_str);
        }
        
        // 返回当前时间的时间戳
        return wxString::FromUTF8(std::to_string(now));
    } else if (act == "时间戳转时间" || act == "timestamp2datetime" || act == "to_datetime") {
        return wxString::FromUTF8(TimestampToDateTime(in));
    } else if (act == "时间转时间戳" || act == "datetime2timestamp" || act == "to_timestamp") {
        return wxString::FromUTF8(DateTimeToTimestamp(in));
    }
    // 未知 action 原样返回
    return input;
}

std::string TimeStampHelper::TimestampToDateTime(const std::string& timestamp)
{
    try {
        // 将字符串转换为时间戳
        std::istringstream iss(timestamp);
        std::time_t ts;
        iss >> ts;
        
        // 转换为本地时间
        std::tm* local_time = std::localtime(&ts);
        
        // 格式化为字符串
        std::ostringstream oss;
        oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
        
        return oss.str();
    } catch (...) {
        return "无效的时间戳格式";
    }
}

std::string TimeStampHelper::DateTimeToTimestamp(const std::string& datetime)
{
    try {
        // 解析时间字符串
        std::tm tm = {};
        std::istringstream iss(datetime);
        
        // 尝试解析常见的时间格式
        if (iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S")) {
            // 转换为时间戳
            std::time_t ts = std::mktime(&tm);
            return std::to_string(ts);
        } else if (iss.str().find('/') != std::string::npos) {
            // 尝试解析 MM/DD/YYYY 格式
            std::istringstream iss2(datetime);
            if (iss2 >> std::get_time(&tm, "%m/%d/%Y %H:%M:%S")) {
                std::time_t ts = std::mktime(&tm);
                return std::to_string(ts);
            }
        }
        
        return "无效的时间格式，请使用 YYYY-MM-DD HH:MM:SS";
    } catch (...) {
        return "无效的时间格式，请使用 YYYY-MM-DD HH:MM:SS";
    }
}