#include "stdafx.h"
#include "CPublic.h"

using std::wstring;


//************************************************************
// 函数名称: HookAnyAddress
// 函数说明: Hook任意地址
// 作    者: Greatfar
// 时    间: 2022/06/09
// 参    数: dwHookAddr 需要HOOK的地址 dwJmpAddress跳转的地址 dwBackAddress 返回的地址
// 返 回 值: void 
//************************************************************
void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress)
{
    // 组装跳转数据
    BYTE jmpCode[5] = { 0 };
    jmpCode[0] = 0xE9;  // jmp对应的机器码的十六进制是E9

    // 计算jmp指令的偏移  公式：要跳转的地址 - HOOK地址 - 5
    *(DWORD*)& jmpCode[1] = (DWORD)dwJmpAddress - dwHookAddr - 5;

    // 保存以前的属性用于还原
    DWORD OldProtext = 0;

    // 因为要往代码段写入数据，又因为代码段是不可写的，所以需要修改属性
    VirtualProtect((LPVOID)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);

    // 写入自己的代码
    memcpy((void*)dwHookAddr, jmpCode, 5);

    // 执行完了操作之后需要进行还原
    VirtualProtect((LPVOID)dwHookAddr, 5, OldProtext, &OldProtext);
}



//************************************************************
// 函数名称: GetWeChatWinBase
// 函数说明: 获取WeChatWin基址
// 作    者: Greatfar
// 时    间: 2022/06/09
// 参    数: void
// 返 回 值: void 
//************************************************************
DWORD GetWeChatWinBase()
{
    return (DWORD)GetModuleHandle(TEXT("WeChatWin.dll"));
}



//************************************************************
// 函数名称: UTF8ToUnicode
// 函数说明: 将UTF8编码转为Unicode(微信默认编码为UTF8)
// 作    者: Greatfar
// 时    间: 2022/06/09
// 参    数: str 需要转换的字符串
// 返 回 值: wchar_t 返回的字符串 
//************************************************************
wchar_t * UTF8ToUnicode(const char* str)
{
    int textlen = 0;
    wchar_t * result;
    textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    result = (wchar_t *)malloc((textlen + 1) * sizeof(wchar_t));
    result != 0 && memset(result, 0, (textlen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
    return result;
}



//************************************************************
// 函数名称: UTF8ToWstring
// 函数说明: utf-8 转 unicode (utf-16)
// 作    者: Greatfar
// 时    间: 2022/06/09
// 参    数: void
// 返 回 值: void 
//************************************************************
wstring UTF8ToWstring(const char* str)
{
    int textlen = 0;
    wchar_t* result;
    textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    result = (wchar_t *)malloc((textlen + 1) * sizeof(wchar_t));
    memset(result, 0, (textlen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
    wstring tempstr(result);
    free(result);
    result = NULL;
    return tempstr;
}



//************************************************************
// 函数名称: DebugCode
// 函数说明: 调试指针地址
// 作    者: Greatfar
// 时    间: 2022/06/09
// 参    数: void
// 返 回 值: void 
//************************************************************
void DebugCode(LPVOID pAddress)
{
    char buff[40] = { 0 };
    sprintf_s(buff, "%p", pAddress);
    MessageBoxA(0, buff, "地址", 0);
}

