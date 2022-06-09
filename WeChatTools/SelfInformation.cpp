#include "stdafx.h"
#include "SelfInformation.h"
#include "CPublic.h"



//************************************************************
// 函数名称: GetInformation
// 函数说明: 获取微信个人信息
// 作    者: Greatfar
// 时    间: 2022/06/09
// 参    数: void
// 返 回 值: void 
//************************************************************
void GetInformation()
{
    std::unique_ptr<PersonalInformation> info(new PersonalInformation);

    DWORD dwWeChatWin = GetWeChatWinBase();

    char tempcontent[0x300];
    // 微信号
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxCount));

    // 判断是否为0 是0说明微信号为空
    if (tempcontent[0] == 0)
    {
        wcscpy_s(info->wxcount, wcslen(L"NULL") + 1, L"NULL");
    }
    else
    {
        wcscpy_s(info->wxcount, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());
    }

    // 微信ID 这里需要兼容新老微信号  新微信号是指针 老微信号是字符串
    memset(tempcontent, 0, 0x300);
    DWORD pWxid = dwWeChatWin + WxID;
    // 如果长度小于6或者大于20 说明这个地方是个指针
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxID));
    if (strlen(tempcontent) < 0x6 || strlen(tempcontent) > 0x14)
    {
        // 说明是新的微信号 微信ID用地址保存
        sprintf_s(tempcontent, "%s", (char*)(*(DWORD*)(dwWeChatWin + WxID)));
    }
    else
    {
        sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxID));
    }
    wcscpy_s(info->wxid, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // 小头像
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(*(DWORD*)(dwWeChatWin + WxSmallHeader)));
    wcscpy_s(info->smallheader, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // 大头像
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(*(DWORD*)(dwWeChatWin + WxBigHeader)));
    wcscpy_s(info->bigheader, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());


    // 国家
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxNation));
    wcscpy_s(info->nation, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // 省份
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxProvince));
    wcscpy_s(info->province, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // 城市
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxCity));
    wcscpy_s(info->city, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // 手机号
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxPhoneNumber));
    wcscpy_s(info->phonenumber, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // 昵称
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

    // 登陆设备
    memset(tempcontent, 0, 0x300);
    sprintf_s(tempcontent, "%s", (char*)(dwWeChatWin + WxDevice));
    wcscpy_s(info->device, wcslen(UTF8ToWstring(tempcontent).c_str()) + 1, UTF8ToWstring(tempcontent).c_str());

    // 发送消息给控制端
    HWND hInformation = FindWindow(NULL, L"微信助手");
    if (hInformation == NULL)
    {
        OutputDebugStringA("未查找到微信助手窗口");
        return;
    }
    COPYDATASTRUCT information_msg;
    information_msg.dwData = WM_GetInformation;
    information_msg.lpData = info.get();
    information_msg.cbData = sizeof(PersonalInformation);
    SendMessage(hInformation, WM_COPYDATA, (WPARAM)hInformation, (LPARAM)&information_msg);
}

