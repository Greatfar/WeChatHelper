#pragma once

#include <string>

std::string unicode_to_utf8(const std::string& str); // unicode ת utf8
std::wstring utf8_to_unicode(const std::string& utf8_str);     // utf8 ת unicode
std::string wchar_t_to_string(wchar_t* wchar);       // wchat_t* ת string
wchar_t* string_to_wchar_t(const std::string& str);  // string ת wchat_t*
std::string GetValueFromXml(std::string& xmlStr, std::string nodeName, std::string nextNodeName);  // ��XML�ַ����л�ȡ����ֵ
std::string FormatDateTime(std::string fmt);                                             // ��ʽ������ʱ�� YYYY-MM-DD HH:II:SS
void WriteLog(const std::string& content, std::string filePath = "runtime.log");    // д��־
unsigned char ToHex(unsigned char x);                                          // תʮ������
unsigned char FromHex(unsigned char x);                                        // ʮ������תʮ����
std::string UrlEncode(const std::string& str);                                 // URL����
std::string UrlDecode(const std::string& str);                                 // URL����
std::string curlRequest(std::string url, std::string postData = "");                     // ʹ��cURL��������
DWORD ProcessNameFindPID(const char* ProcessName);                             // ͨ����������ȡ����ID
std::string cstring_to_string(const CString& cstr);                                 // �� CString תΪ string
CString string_to_cstring(const std::string& str);                                  // �� string תΪ CString
void CreatePathIfNotExist(CString path);                                       // �����־Ŀ¼�Ƿ���ڣ��������򴴽�
std::string GetValueFromeConfig(CString path, CString field, CString key, CString defaultValue = _T(""));  // �������ļ�ȡֵ

