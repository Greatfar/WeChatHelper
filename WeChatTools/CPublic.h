#pragma once

void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress);        //HOOK�����ַ
DWORD GetWeChatWinBase();        //��ȡWeChatWin��ַ
wchar_t * UTF8ToUnicode(const char* str);
std::wstring UTF8ToUnicode2(const char* str);
void DebugCode(LPVOID pAddress);

