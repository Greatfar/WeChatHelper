#include "stdafx.h"
#include <strstream>
#include <iostream>
#include <string>
#pragma comment(lib,"Version.lib")

using std::string;
using std::wstring;

//************************************************************
// 函数名称: getWxVersion
// 函数说明: 获取微信版本
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: void
// 返 回 值: string
//************************************************************
string getWxVersion()
{
    DWORD wxBaseAddress = (DWORD)GetModuleHandle(TEXT("WeChatWin.dll"));

    WCHAR VersionFilePath[MAX_PATH];
    if (GetModuleFileName((HMODULE)wxBaseAddress, VersionFilePath, MAX_PATH) == 0)
    {
        return "0.0.0.0";
    }

    string asVer = "";
    VS_FIXEDFILEINFO* pVsInfo;
    unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
    int iVerInfoSize = GetFileVersionInfoSize(VersionFilePath, NULL);
    if (iVerInfoSize != 0) {
        char* pBuf = new char[iVerInfoSize];
        if (GetFileVersionInfo(VersionFilePath, 0, iVerInfoSize, pBuf)) {
            if (VerQueryValue(pBuf, TEXT("\\"), (void**)& pVsInfo, &iFileInfoSize)) {

                int s_major_ver = (pVsInfo->dwFileVersionMS >> 16) & 0x0000FFFF;

                int s_minor_ver = pVsInfo->dwFileVersionMS & 0x0000FFFF;

                int s_build_num = (pVsInfo->dwFileVersionLS >> 16) & 0x0000FFFF;

                int s_revision_num = pVsInfo->dwFileVersionLS & 0x0000FFFF;

                // 把版本变成字符串
                std::strstream wxVer;
                wxVer << s_major_ver << "." << s_minor_ver << "." << s_build_num << "." << s_revision_num;
                wxVer >> asVer;
            }
        }
        delete[] pBuf;
    }

    return asVer;
}


//************************************************************
// 函数名称: CheckIsLogin
// 函数说明: 检查微信是否登陆
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: void
// 返 回 值: void
//************************************************************
void CheckIsLogin()
{
    // 获取WeChatWin的基址
    DWORD  dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

    while (true)
    {
        DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset;
        if (*(DWORD*)dwIsLogin != 0)
        {
            // 查找登陆窗口句柄
            HWND hLogin = FindWindow(NULL, L"微信助手");
            if (hLogin == NULL)
            {
                OutputDebugStringA("未查找到Login窗口");
                return;
            }
            COPYDATASTRUCT login_msg;
            login_msg.dwData = WM_Login;
            login_msg.lpData = NULL;
            login_msg.cbData = 0;
            // 发送消息给控制端
            SendMessage(hLogin, WM_COPYDATA, (WPARAM)hLogin, (LPARAM)&login_msg);
            break;
        }

        Sleep(500);
    }
}

//************************************************************
// 函数名称: CheckIsLogout
// 函数说明: 检查微信是否已退出
// 作    者: Greatfar
// 时    间: 2022/06/04
// 参    数: void
// 返 回 值: void
//************************************************************
void CheckIsLogout()
{
    // 获取WeChatWin的基址
    DWORD  dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

    while (true)
    {
        DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset;
        if (*(DWORD*)dwIsLogin == 0)
        {
            // 开线程持续检测微信登陆状态
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckIsLogin, 0, 0, NULL);
            hThread != 0 && CloseHandle(hThread);
            // 退出循环（退出CheckIsLogout线程）
            break;
        }

        Sleep(500);
    }
}

