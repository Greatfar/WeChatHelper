#pragma once

#include <string>

std::string unicode_to_utf8(const std::string& str); // unicode ת utf8
wstring utf8_to_unicode(const string& utf8_str);     // utf8 ת unicode
std::string wchar_t_to_string(wchar_t* wchar);       // wchat_t* ת string
wchar_t* string_to_wchar_t(const std::string& str);  // string ת wchat_t*
string GetValueFromXml(string& xmlStr, string nodeName, string nextNodeName);  // ��XML�ַ����л�ȡ����ֵ
string FormatDateTime(string fmt);                                             // ��ʽ������ʱ�� YYYY-MM-DD HH:II:SS
void WriteLog(const std::string& content, string filePath = "runtime.log");    // д��־
unsigned char ToHex(unsigned char x);                                          // תʮ������
unsigned char FromHex(unsigned char x);                                        // ʮ������תʮ����
std::string UrlEncode(const std::string& str);                                 // URL����
std::string UrlDecode(const std::string& str);                                 // URL����
std::string curlRequest(string url, string postData = "");                     // ʹ��cURL��������
DWORD ProcessNameFindPID(const char* ProcessName);                             // ͨ����������ȡ����ID
string cstring_to_string(const CString& cstr);                                 // �� CString תΪ string
CString string_to_cstring(const string& str);                                  // �� string תΪ CString
void CreatePathIfNotExist(CString path);                                       // �����־Ŀ¼�Ƿ���ڣ��������򴴽�
string GetValueFromeConfig(CString path, CString field, CString key, CString defaultValue = _T(""));  // �������ļ�ȡֵ

