#pragma once


//΢��ͨ�ýṹ��
struct GeneralStruct
{
    wchar_t* pstr;
    int iLen;
    int iMaxLen;
    int full1;
    int full2;
    GeneralStruct(wchar_t* pString)
    {
        pstr = pString;
        iLen = wcslen(pString);
        iMaxLen = iLen * 2;
        full1 = 0;
        full2 = 0;
    }
};


// �����¼����Ϣ�ṹ��
struct ChatMessageData
{
    DWORD dwtype;                      // ��Ϣ����
    wchar_t sztype[0x20];              // ��Ϣ����
    wchar_t source[0x400];             // ��Ϣ��Դ
    wchar_t wxid[0x40];                // ΢��ID/ȺID
    wchar_t wxname[0x200];             // ΢������/Ⱥ����
    wchar_t sender[0x100];             // ��Ϣ������
    wchar_t sendername[0x100];         // ��Ϣ�������ǳ�
    wchar_t content[0x8000];           // ��Ϣ����
};


//������Ϣ�Ľṹ��
struct PersonalInformation
{
    wchar_t wxid[40];               // ΢��ID
    wchar_t wxcount[40];            // ΢���˺�
    wchar_t nickname[50];           // ΢���ǳ�
    wchar_t remark[50];             // ��ע
    wchar_t phonenumber[30];        // �ֻ���
    wchar_t device[20];             // ��½�豸
    wchar_t nation[20];             // ����
    wchar_t province[20];           // ʡ��
    wchar_t city[20];               // ����
    wchar_t area[20];               // ����
    wchar_t language[10];           // ����
    wchar_t bigheader[0x100];       // ��ͷ��
    wchar_t smallheader[0x100];     // Сͷ��
    wchar_t signature[50];          // ����ǩ��
};

