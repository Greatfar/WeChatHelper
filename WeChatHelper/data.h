#pragma once

#include <string>


// ͨ����Ϣ�ṹ��
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


// �����¼����Ϣ�ṹ��
struct ChatMessageData
{
    DWORD dwtype;               // ��Ϣ����
    wchar_t sztype[0x20];       // ��Ϣ����
    wchar_t source[0x400];      // ��Ϣ��Դ
    wchar_t wxid[0x40];         // ΢��ID/ȺID
    wchar_t wxname[0x200];      // ΢������/Ⱥ����
    wchar_t sender[0x100];      // ��Ϣ������
    wchar_t sendername[0x100];  // ��Ϣ�������ǳ�
    wchar_t content[0x8000];    // ��Ϣ����
};


// ������Ϣ�Ľṹ��
struct PersonalInformation
{
    wchar_t wxid[40];              // ΢��ID
    wchar_t wxcount[40];           // ΢���˺�
    wchar_t nickname[50];          // ΢���ǳ�
    wchar_t remark[50];            // ��ע
    wchar_t phonenumber[30];       // �ֻ���
    wchar_t device[20];            // ��½�豸
    wchar_t nation[20];            // ����
    wchar_t province[20];          // ʡ��
    wchar_t city[20];              // ����
    wchar_t area[20];              // ����
    wchar_t language[10];          // ����
    wchar_t bigheader[0x100];      // ��ͷ��
    wchar_t smallheader[0x100];    // Сͷ��
    wchar_t signature[50];         // ����ǩ��
};


// ��ǰ΢�ŵ�¼���û���Ϣ �ṹ��
struct UserInfo
{
    std::string wxid;             // ΢��ID
    std::string wxcount;          // ΢���˺�
    std::string nickname;         // ΢���ǳ�
    std::string phonenumber;      // �ֻ���
};
