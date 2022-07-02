#pragma once

#include <string>


// 通用消息结构体
struct MessageStruct
{
    wchar_t msgdata1[MAX_PATH];
    wchar_t msgdata2[MAX_PATH];

    MessageStruct(wchar_t* pString1, wchar_t* pString2)
    {
        wcscpy_s(msgdata1, wcslen(pString1) + 1, pString1);
        wcscpy_s(msgdata2, wcslen(pString2) + 1, pString2);
    }

    MessageStruct(CString pString1, CString pString2)
    {
        wcscpy_s(msgdata1, wcslen(pString1) + 1, pString1);
        wcscpy_s(msgdata2, wcslen(pString2) + 1, pString2);
    }
};


// 聊天记录的消息结构体
struct ChatMessageData
{
    DWORD dwtype;               // 消息类型
    wchar_t sztype[0x20];       // 消息类型
    wchar_t source[0x400];      // 消息来源
    wchar_t wxid[0x40];         // 微信ID/群ID
    wchar_t wxname[0x200];      // 微信名称/群名称
    wchar_t sender[0x100];      // 消息发送者
    wchar_t sendername[0x100];  // 消息发送者昵称
    wchar_t content[0x8000];    // 消息内容
};


// 个人信息的结构体
struct PersonalInformation
{
    wchar_t wxid[40];              // 微信ID
    wchar_t wxcount[40];           // 微信账号
    wchar_t nickname[50];          // 微信昵称
    wchar_t remark[50];            // 备注
    wchar_t phonenumber[30];       // 手机号
    wchar_t device[20];            // 登陆设备
    wchar_t nation[20];            // 国籍
    wchar_t province[20];          // 省份
    wchar_t city[20];              // 城市
    wchar_t area[20];              // 地区
    wchar_t language[10];          // 语言
    wchar_t bigheader[0x100];      // 大头像
    wchar_t smallheader[0x100];    // 小头像
    wchar_t signature[50];         // 个性签名
};


// 当前微信登录的用户信息 结构体
struct UserInfo
{
    std::string wxid;             // 微信ID
    std::string wxcount;          // 微信账号
    std::string nickname;         // 微信昵称
    std::string phonenumber;      // 手机号
};
