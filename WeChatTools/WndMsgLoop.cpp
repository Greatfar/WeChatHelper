#include "stdafx.h"
#include "WndMsgLoop.h"
#include "InitWeChat.h"
#include "MainWindow.h"
#include "ChatRecord.h"
#include "CAutoFunction.h"
#include "SelfInformation.h"
#include <stdio.h>
#include <string>

using std::string;
using std::wstring;

//************************************************************
// 函数名称: RegisterWindow
// 函数说明: 初始化窗口 
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: HMODULE hModule 句柄
// 返 回 值: void 
//************************************************************
void InitWindow(HMODULE hModule)
{
    string targetVersoin = "3.6.0.18";  // 目标微信版本
    string wxCurrentVersoin = getWxVersion();  // 当前微信版本

    // 检查当前微信版本
    if (wxCurrentVersoin == targetVersoin)
    {
        // 获取WeChatWin的基址
        DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
    
        // 检测微信是否登陆
        DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset;
        if (*(DWORD*)dwIsLogin == 0)    // 等于0说明微信未登录
        {
            // 开线程持续检测微信登陆状态
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckIsLogin, 0, 0, NULL);
            hThread != 0 && CloseHandle(hThread);
            
            // HOOK接收消息
            HookChatRecord();

            // 注册窗口
            RegisterWindow(hModule);
        }
        else
        {
            // 如果微信已经登陆 发送消息给客户端
            HWND hLogin = FindWindow(NULL, L"微信助手");
            if (hLogin == NULL)
            {
                OutputDebugStringA("未查找到微信助手窗口");
                return;
            }
            COPYDATASTRUCT login_msg;
            login_msg.dwData = WM_AlreadyLogin;
            login_msg.lpData = NULL;
            login_msg.cbData = 0;
            // 发送消息给控制端
            SendMessage(hLogin, WM_COPYDATA, (WPARAM)hLogin, (LPARAM)&login_msg);
        }
    }
    else
    {
        string versionErrorTips = "不支持当前微信版本（" + wxCurrentVersoin + "），请下载微信 for windows " + targetVersoin;
        MessageBoxA(NULL, versionErrorTips.c_str(), "微信版本不匹配", MB_OK | MB_SYSTEMMODAL);
        // 向控制端发送 版本不匹配 消息
        HWND hLogin = FindWindow(NULL, L"微信助手");
        if (hLogin == NULL)
        {
            OutputDebugStringA("未查找到微信助手窗口");
            return;
        }
        COPYDATASTRUCT login_msg;
        login_msg.dwData = WM_WechatVersionError;
        login_msg.lpData = NULL;
        login_msg.cbData = 0;
        SendMessage(hLogin, WM_COPYDATA, (WPARAM)hLogin, (LPARAM)&login_msg);
        // 退出微信进程
        ExitProcess(-1);
    }

}


//************************************************************
// 函数名称: RegisterWindow
// 函数说明: 注册窗口
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: HMODULE hModule 窗口句柄
// 返 回 值: void 
//************************************************************

void RegisterWindow(HMODULE hModule)
{
    //1  设计一个窗口类
    WNDCLASS wnd;
    wnd.style = CS_VREDRAW | CS_HREDRAW;//风格
    wnd.lpfnWndProc = WndProc;//窗口回调函数指针.
    wnd.cbClsExtra = NULL;
    wnd.cbWndExtra = NULL;
    wnd.hInstance = hModule;
    wnd.hIcon = NULL;
    wnd.hCursor = NULL;
    wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wnd.lpszMenuName = NULL;
    wnd.lpszClassName = TEXT("WeChatTools");
    //2  注册窗口类
    RegisterClass(&wnd);
    //3  创建窗口
    HWND hWnd = CreateWindow(
        TEXT("WeChatTools"),  //窗口类名
        TEXT("WeChatTools"),//窗口名
        WS_OVERLAPPEDWINDOW,//窗口风格
        10, 10, 500, 300, //窗口位置
        NULL,             //父窗口句柄
        NULL,             //菜单句柄
        hModule,        //实例句柄
        NULL              //传递WM_CREATE消息时的附加参数
    );
    //4  更新显示窗口
    ShowWindow(hWnd, SW_HIDE);
    UpdateWindow(hWnd);
    //5  消息循环（消息泵）
    MSG  msg = {};
    //   5.1获取消息
    while (GetMessage(&msg, 0, 0, 0))
    {
        //   5.2翻译消息
        TranslateMessage(&msg);
        //   5.3转发到消息回调函数
        DispatchMessage(&msg);
    }
}


//************************************************************
// 函数名称: WndProc
// 函数说明: 回调函数 用于和控制端通信 
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam
// 返 回 值: LRESULT 
//************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    try
    {
        if (Message == WM_COPYDATA)
        {
            COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lParam;
            switch (pCopyData->dwData)
            {
            // 退出微信
            case WM_Logout:
            {
                // 退出微信
                LogoutWeChat();
                // 开线程持续检测微信退出状态
                HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckIsLogout, 0, 0, NULL);
                hThread != 0 && CloseHandle(hThread);
            }
            break;
            // 获取个人信息
            case WM_GetInformation:
            {
                GetInformation();
            }
            break;
            default:
                break;
            }
        }
    }
    catch (...)
    {
        OutputDebugStringA("窗口消息循环出错了");
    }

    return DefWindowProc(hWnd, Message, wParam, lParam);
}
