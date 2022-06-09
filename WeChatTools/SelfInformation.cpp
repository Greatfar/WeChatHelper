#include "stdafx.h"
#include "SelfInformation.h"
#include "CPublic.h"



//************************************************************
// ��������: GetInformation
// ����˵��: ��ȡ΢�Ÿ�����Ϣ
// ��    ��: Greatfar
// ʱ    ��: 2022/06/09
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
void GetInformation()
{
    std::unique_ptr<PersonalInformation> info(new PersonalInformation);

    DWORD dwWeChatWin = GetWeChatWinBase();

    char tempcontent[0x300];
    // ΢�ź�
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxCount));

    // �ж��Ƿ�Ϊ0 ��0˵��΢�ź�Ϊ��
    if (tempcontent[0] == 0)
    {
        wcscpy_s(info->wxcount, wcslen(L"NULL") + 1, L"NULL");
    }
    else
    {
        wcscpy_s(info->wxcount, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());
    }

    // ΢��ID ������Ҫ��������΢�ź�  ��΢�ź���ָ�� ��΢�ź����ַ���
    memset(tempcontent, 0, 0x300);
    DWORD pWxid = dwWeChatWin + WxID;
    // �������С��6���ߴ���20 ˵������ط��Ǹ�ָ��
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxID));
    if (strlen(tempcontent) < 0x6 || strlen(tempcontent) > 0x14)
    {
        // ˵�����µ�΢�ź� ΢��ID�õ�ַ����
        sprintf_s(tempcontent, "%s", (char*)(*(DWORD*)(dwWeChatWin + WxID)));
    }
    else
    {
        sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxID));
    }
    wcscpy_s(info->wxid, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // Сͷ��
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(*(DWORD*)(dwWeChatWin + WxSmallHeader)));
    wcscpy_s(info->smallheader, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // ��ͷ��
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(*(DWORD*)(dwWeChatWin + WxBigHeader)));
    wcscpy_s(info->bigheader, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());


    // ����
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxNation));
    wcscpy_s(info->nation, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // ʡ��
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxProvince));
    wcscpy_s(info->province, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // ����
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxCity));
    wcscpy_s(info->city, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // �ֻ���
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxPhoneNumber));
    wcscpy_s(info->phonenumber, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // �ǳ�
    memset(tempcontent, 0, 0x300);
    if (*(DWORD*)(dwWeChatWin + WxNickName + 0x14) == 0xF)
    {
        sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxNickName));
    }
    else
    {
        sprintf_s(tempcontent, "%s", (char*)(*(DWORD*)(dwWeChatWin + WxNickName)));
    }
    
    wcscpy_s(info->nickname, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // ��½�豸
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxDevice));
    wcscpy_s(info->device, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // ������Ϣ�����ƶ�
    HWND hInformation = FindWindow(NULL, L"΢������");
    if (hInformation == NULL)
    {
        OutputDebugStringA("δ���ҵ�΢�����ִ���");
        return;
    }
    COPYDATASTRUCT information_msg;
    information_msg.dwData = WM_GetInformation;
    information_msg.lpData = info.get();
    information_msg.cbData = sizeof(PersonalInformation);
    SendMessage(hInformation, WM_COPYDATA, (WPARAM)hInformation, (LPARAM)&information_msg);
}

