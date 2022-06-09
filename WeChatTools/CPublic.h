#pragma once

void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress);        // HOOK任意地址
DWORD GetWeChatWinBase();                                          // 获取WeChatWin基址
wchar_t * UTF8ToUnicode(const char* str);                          // UTF8 转 Unicode 返回wchar_t *
std::wstring UTF8ToWstring(const char* str);                       // UTF8 转 Unicode 返回wstring
void DebugCode(LPVOID pAddress);                                   // 调试代码

