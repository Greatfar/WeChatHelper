#include "stdafx.h"
#include "CInjectTools.h"
#include <direct.h>
#include <stdlib.h>
#include <TlHelp32.h>
#include <stdio.h>
#include "PublicFunction.h"
#pragma comment(lib,"advapi32")


#define WECHAT_PROCESS_NAME "WeChat.exe"
#define DLLNAME "WeChatTools.dll"


CString GetAppRegeditPath(CString strAppName)
{
    //������ر���
    HKEY hKey;
    CString strAppRegeditPath("");
    TCHAR szProductType[MAX_PATH];
    memset(szProductType, 0, sizeof(szProductType));

    DWORD dwBuflen = MAX_PATH;
    LONG lRet = 0;

    //�����Ǵ�ע���,ֻ�д򿪺��������������
    lRet = RegOpenKeyEx(HKEY_CURRENT_USER, //Ҫ�򿪵ĸ���
        LPCTSTR(strAppName), //Ҫ�򿪵����Ӽ�
        0, //���һ��Ϊ0
        KEY_QUERY_VALUE, //ָ���򿪷�ʽ,��Ϊ��
        &hKey); //�������ؾ��

    if (lRet != ERROR_SUCCESS) //�ж��Ƿ�򿪳ɹ�
    {
        return strAppRegeditPath;
    }
    else
    {
        //���濪ʼ��ѯ
        lRet = RegQueryValueEx(hKey, //��ע���ʱ���صľ��
            TEXT("Wechat"), //Ҫ��ѯ������,��ѯ�������װĿ¼������
            NULL, //һ��ΪNULL����0
            NULL,
            (LPBYTE)szProductType, //����Ҫ�Ķ�����������
            &dwBuflen);

        if (lRet != ERROR_SUCCESS) //�ж��Ƿ��ѯ�ɹ�
        {
            return strAppRegeditPath;
        }
        else
        {
            RegCloseKey(hKey);
            
            strAppRegeditPath = szProductType;

            int nPos = strAppRegeditPath.Find('-');

            if (nPos >= 0)
            {
                CString sSubStr = strAppRegeditPath.Left(nPos - 1);//����$,�������ʱnPos+1
                strAppRegeditPath = sSubStr;
            }
        }
    }
    return strAppRegeditPath;
}


CString GetAppRegeditPath2(CString strAppName)
{
    //������ر���
    HKEY hKey;
    CString strAppRegeditPath("");
    TCHAR szProductType[MAX_PATH];
    memset(szProductType, 0, sizeof(szProductType));

    DWORD dwBuflen = MAX_PATH;
    LONG lRet = 0;

    //�����Ǵ�ע���,ֻ�д򿪺��������������
    lRet = RegOpenKeyEx(HKEY_CURRENT_USER, //Ҫ�򿪵ĸ���
        LPCTSTR(strAppName), //Ҫ�򿪵����Ӽ�
        0, //���һ��Ϊ0
        KEY_QUERY_VALUE, //ָ���򿪷�ʽ,��Ϊ��
        &hKey); //�������ؾ��

    if (lRet != ERROR_SUCCESS) //�ж��Ƿ�򿪳ɹ�
    {
        return strAppRegeditPath;
    }
    else
    {
        //���濪ʼ��ѯ
        lRet = RegQueryValueEx(hKey, //��ע���ʱ���صľ��
            TEXT("InstallPath"), //Ҫ��ѯ������,��ѯ�������װĿ¼������
            NULL, //һ��ΪNULL����0
            NULL,
            (LPBYTE)szProductType, //����Ҫ�Ķ�����������
            &dwBuflen);

        if (lRet != ERROR_SUCCESS) //�ж��Ƿ��ѯ�ɹ�
        {
            return strAppRegeditPath;
        }
        else
        {
            RegCloseKey(hKey);
            strAppRegeditPath = szProductType;

        }
    }
    return strAppRegeditPath;
}


//************************************************************
// ��������: InjectDll
// ����˵��: ע��DLL
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: void
// �� �� ֵ: void
//************************************************************
BOOL InjectDll(HANDLE& wxPid)
{
    // ��ȡ��ǰ����Ŀ¼�µ�dll
    char szPath[MAX_PATH] = { 0 };
    char* buffer = NULL;
    if ((buffer = _getcwd(NULL, 0)) == NULL)
    {
        MessageBoxA(NULL, "��ȡ��ǰ����Ŀ¼ʧ��", "����", 0);
        return FALSE;
    }
    sprintf_s(szPath, "%s\\%s", buffer, DLLNAME);
    // ��ȡ΢��Pid
    DWORD dwPid = ProcessNameFindPID(WECHAT_PROCESS_NAME);
    if (dwPid == 0)
    {
        CString wxStrAppName = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
        CString szProductType = GetAppRegeditPath(wxStrAppName);

        if (szProductType.GetLength()<5)
        {
            wxStrAppName = TEXT("Software\\Tencent\\WeChat");
            szProductType = GetAppRegeditPath2(wxStrAppName);
            szProductType.Append(L"\\WeChat.exe");
        }
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        si.dwFlags = STARTF_USESHOWWINDOW;// ָ��wShowWindow��Ա��Ч
        si.wShowWindow = TRUE;          // �˳�Ա��ΪTRUE�Ļ�����ʾ�½����̵������ڣ�
                                       // ΪFALSE�Ļ�����ʾ
        // ��΢�ŵ�¼����
        CreateProcess(szProductType, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

        HWND  hWechatMainForm = NULL; // ΢�ŵ�¼���ھ��
        int checkWechatLoginFormTimes = 0; // ����΢�ŵ�¼���ڵĴ���
        // ѭ�� ����΢�ŵ�¼���ڵľ��
        while (NULL == hWechatMainForm)
        {
            // ����һ��������ֹͣ����
            checkWechatLoginFormTimes++;
            if (checkWechatLoginFormTimes > 120) {
                MessageBoxA(NULL, "΢������ʧ�ܣ���ȷ���Ƿ��Ѱ�װ΢�ŵ��԰�", "tips", MB_SYSTEMMODAL);
                break;
            }
            // ����΢�ŵ�¼����
            hWechatMainForm = FindWindow(TEXT("WeChatLoginWndForPC"), NULL);
            Sleep(500);
        }
        if (NULL == hWechatMainForm)
        {
            return FALSE;
        }
        dwPid = pi.dwProcessId;
        wxPid = pi.hProcess;
    }
    // ���dll�Ƿ��Ѿ�ע��
    if (CheckIsInject(dwPid))
    {
        // �򿪽���
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
        if (hProcess == NULL)
        {
            MessageBoxA(NULL, "���̴�ʧ��", "����", 0);
            return FALSE;
        }
        // ��΢�Ž����������ڴ�
        LPVOID pAddress = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (pAddress == NULL)
        {
            MessageBoxA(NULL, "�ڴ����ʧ��", "����", 0);
            return FALSE;
        }
        // д��dll·����΢�Ž���
        if (WriteProcessMemory(hProcess, pAddress, szPath, MAX_PATH, NULL) == 0)
        {
            MessageBoxA(NULL, "·��д��ʧ��", "����", 0);
            return FALSE;
        }

        // ��ȡLoadLibraryA������ַ
        FARPROC pLoadLibraryAddress = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
        if (pLoadLibraryAddress == NULL)
        {
            MessageBoxA(NULL, "��ȡLoadLibraryA������ַʧ��", "����", 0);
            return FALSE;
        }
        // Զ���߳�ע��dll
        HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryAddress, pAddress, 0, NULL);
        if (hRemoteThread == NULL)
        {
            MessageBoxA(NULL, "Զ���߳�ע��ʧ��", "����", 0);
            return FALSE;
        }

        CloseHandle(hRemoteThread);
        CloseHandle(hProcess);
    }
    else
    {
        MessageBoxA(NULL, "���΢�Źرպ��ٴ�΢������", "tips", 0);
        // �˳���ǰ����
        ExitProcess(-1);
        // return FALSE;
    }
    return TRUE;
}


//************************************************************
// ��������: CheckIsInject
// ����˵��: ����Ƿ��Ѿ�ע��dll
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: void
// �� �� ֵ: BOOL 
//************************************************************
BOOL CheckIsInject(DWORD dwProcessid)
{
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    //��ʼ��ģ����Ϣ�ṹ��
    MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
    //����ģ����� 1 �������� 2 ����ID
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessid);
    //��������Ч�ͷ���false
    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
        MessageBoxA(NULL, "����ģ�����ʧ��", "����", MB_OK);
        return FALSE;
    }
    //ͨ��ģ����վ����ȡ��һ��ģ�����Ϣ
    if (!Module32First(hModuleSnap, &me32))
    {
        MessageBoxA(NULL, "��ȡ��һ��ģ�����Ϣʧ��", "����", MB_OK);
        //��ȡʧ����رվ��
        CloseHandle(hModuleSnap);
        return FALSE;
    }
    do
    {
        if (StrCmpW(me32.szModule, L"WeChatTools.dll") == 0)
        {
            return FALSE;
        }

    } while (Module32Next(hModuleSnap, &me32));
    return TRUE;
}

