#pragma once

void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress);        // HOOK�����ַ
DWORD GetWeChatWinBase();                                          // ��ȡWeChatWin��ַ
wchar_t * UTF8ToUnicode(const char* str);                          // UTF8 ת Unicode ����wchar_t *
std::wstring UTF8ToWstring(const char* str);                       // UTF8 ת Unicode ����wstring
void DebugCode(LPVOID pAddress);                                   // ���Դ���

