#pragma once

#include <string>

std::string unicode_to_utf8(const std::string& str); // unicode 转 utf8
std::wstring utf8_to_unicode(const std::string& utf8_str);     // utf8 转 unicode
std::string wchar_t_to_string(wchar_t* wchar);       // wchat_t* 转 string
wchar_t* string_to_wchar_t(const std::string& str);  // string 转 wchat_t*
std::string GetValueFromXml(std::string& xmlStr, std::string nodeName, std::string nextNodeName);  // 从XML字符串中获取属性值
std::string FormatDateTime(std::string fmt);                                             // 格式化日期时间 YYYY-MM-DD HH:II:SS
void WriteLog(const std::string& content, std::string filePath = "runtime.log");    // 写日志
unsigned char ToHex(unsigned char x);                                          // 转十六进制
unsigned char FromHex(unsigned char x);                                        // 十六进制转十进制
std::string UrlEncode(const std::string& str);                                 // URL编码
std::string UrlDecode(const std::string& str);                                 // URL解码
std::string curlRequest(std::string url, std::string postData = "");                     // 使用cURL发送请求
DWORD ProcessNameFindPID(const char* ProcessName);                             // 通过进程名获取进程ID
std::string cstring_to_string(const CString& cstr);                                 // 将 CString 转为 string
CString string_to_cstring(const std::string& str);                                  // 将 string 转为 CString
void CreatePathIfNotExist(CString path);                                       // 检查日志目录是否存在，不存在则创建
std::string GetValueFromeConfig(CString path, CString field, CString key, CString defaultValue = _T(""));  // 从配置文件取值

