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
// ��������: RegisterWindow
// ����˵��: ��ʼ������ 
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: HMODULE hModule ���
// �� �� ֵ: void 
//************************************************************
void InitWindow(HMODULE hModule)
{
    string targetVersoin = "3.6.0.18";  // Ŀ��΢�Ű汾
    string wxCurrentVersoin = getWxVersion();  // ��ǰ΢�Ű汾

    // ��鵱ǰ΢�Ű汾
    if (wxCurrentVersoin == targetVersoin)
    {
        // ��ȡWeChatWin�Ļ�ַ
        DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
    
        // ���΢���Ƿ��½
        DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset;
        if (*(DWORD*)dwIsLogin == 0)    // ����0˵��΢��δ��¼
        {
            // ���̳߳������΢�ŵ�½״̬
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckIsLogin, 0, 0, NULL);
            hThread != 0 && CloseHandle(hThread);
            
            // HOOK������Ϣ
            HookChatRecord();

            // ע�ᴰ��
            RegisterWindow(hModule);
        }
        else
        {
            // ���΢���Ѿ���½ ������Ϣ���ͻ���
            HWND hLogin = FindWindow(NULL, L"΢������");
            if (hLogin == NULL)
            {
                OutputDebugStringA("δ���ҵ�΢�����ִ���");
                return;
            }
            COPYDATASTRUCT login_msg;
            login_msg.dwData = WM_AlreadyLogin;
            login_msg.lpData = NULL;
            login_msg.cbData = 0;
            // ������Ϣ�����ƶ�
            SendMessage(hLogin, WM_COPYDATA, (WPARAM)hLogin, (LPARAM)&login_msg);
        }
    }
    else
    {
        string versionErrorTips = "��֧�ֵ�ǰ΢�Ű汾��" + wxCurrentVersoin + "����������΢�� for windows " + targetVersoin;
        MessageBoxA(NULL, versionErrorTips.c_str(), "΢�Ű汾��ƥ��", MB_OK | MB_SYSTEMMODAL);
        // ����ƶ˷��� �汾��ƥ�� ��Ϣ
        HWND hLogin = FindWindow(NULL, L"΢������");
        if (hLogin == NULL)
        {
            OutputDebugStringA("δ���ҵ�΢�����ִ���");
            return;
        }
        COPYDATASTRUCT login_msg;
        login_msg.dwData = WM_WechatVersionError;
        login_msg.lpData = NULL;
        login_msg.cbData = 0;
        SendMessage(hLogin, WM_COPYDATA, (WPARAM)hLogin, (LPARAM)&login_msg);
        // �˳�΢�Ž���
        ExitProcess(-1);
    }

}


//************************************************************
// ��������: RegisterWindow
// ����˵��: ע�ᴰ��
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: HMODULE hModule ���ھ��
// �� �� ֵ: void 
//************************************************************

void RegisterWindow(HMODULE hModule)
{
    //1  ���һ��������
    WNDCLASS wnd;
    wnd.style = CS_VREDRAW | CS_HREDRAW;//���
    wnd.lpfnWndProc = WndProc;//���ڻص�����ָ��.
    wnd.cbClsExtra = NULL;
    wnd.cbWndExtra = NULL;
    wnd.hInstance = hModule;
    wnd.hIcon = NULL;
    wnd.hCursor = NULL;
    wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wnd.lpszMenuName = NULL;
    wnd.lpszClassName = TEXT("WeChatTools");
    //2  ע�ᴰ����
    RegisterClass(&wnd);
    //3  ��������
    HWND hWnd = CreateWindow(
        TEXT("WeChatTools"),  //��������
        TEXT("WeChatTools"),//������
        WS_OVERLAPPEDWINDOW,//���ڷ��
        10, 10, 500, 300, //����λ��
        NULL,             //�����ھ��
        NULL,             //�˵����
        hModule,        //ʵ�����
        NULL              //����WM_CREATE��Ϣʱ�ĸ��Ӳ���
    );
    //4  ������ʾ����
    ShowWindow(hWnd, SW_HIDE);
    UpdateWindow(hWnd);
    //5  ��Ϣѭ������Ϣ�ã�
    MSG  msg = {};
    //   5.1��ȡ��Ϣ
    while (GetMessage(&msg, 0, 0, 0))
    {
        //   5.2������Ϣ
        TranslateMessage(&msg);
        //   5.3ת������Ϣ�ص�����
        DispatchMessage(&msg);
    }
}


//************************************************************
// ��������: WndProc
// ����˵��: �ص����� ���ںͿ��ƶ�ͨ�� 
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam
// �� �� ֵ: LRESULT 
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
            // �˳�΢��
            case WM_Logout:
            {
                // �˳�΢��
                LogoutWeChat();
                // ���̳߳������΢���˳�״̬
                HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckIsLogout, 0, 0, NULL);
                hThread != 0 && CloseHandle(hThread);
            }
            break;
            // ��ȡ������Ϣ
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
        OutputDebugStringA("������Ϣѭ��������");
    }

    return DefWindowProc(hWnd, Message, wParam, lParam);
}
