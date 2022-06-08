#include "stdafx.h"
#include <strstream>
#include <iostream>
#include <string>
#pragma comment(lib,"Version.lib")

using std::string;
using std::wstring;

//************************************************************
// ��������: getWxVersion
// ����˵��: ��ȡ΢�Ű汾
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: void
// �� �� ֵ: string
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

                // �Ѱ汾����ַ���
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
// ��������: CheckIsLogin
// ����˵��: ���΢���Ƿ��½
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: void
// �� �� ֵ: void
//************************************************************
void CheckIsLogin()
{
    // ��ȡWeChatWin�Ļ�ַ
    DWORD  dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

    while (true)
    {
        DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset;
        if (*(DWORD*)dwIsLogin != 0)
        {
            // ���ҵ�½���ھ��
            HWND hLogin = FindWindow(NULL, L"΢������");
            if (hLogin == NULL)
            {
                OutputDebugStringA("δ���ҵ�Login����");
                return;
            }
            COPYDATASTRUCT login_msg;
            login_msg.dwData = WM_Login;
            login_msg.lpData = NULL;
            login_msg.cbData = 0;
            // ������Ϣ�����ƶ�
            SendMessage(hLogin, WM_COPYDATA, (WPARAM)hLogin, (LPARAM)&login_msg);
            break;
        }

        Sleep(500);
    }
}

//************************************************************
// ��������: CheckIsLogout
// ����˵��: ���΢���Ƿ����˳�
// ��    ��: Greatfar
// ʱ    ��: 2022/06/04
// ��    ��: void
// �� �� ֵ: void
//************************************************************
void CheckIsLogout()
{
    // ��ȡWeChatWin�Ļ�ַ
    DWORD  dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

    while (true)
    {
        DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset;
        if (*(DWORD*)dwIsLogin == 0)
        {
            // ���̳߳������΢�ŵ�½״̬
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckIsLogin, 0, 0, NULL);
            hThread != 0 && CloseHandle(hThread);
            // �˳�ѭ�����˳�CheckIsLogout�̣߳�
            break;
        }

        Sleep(500);
    }
}

