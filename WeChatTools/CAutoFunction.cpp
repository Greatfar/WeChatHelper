#include "stdafx.h"
#include "CAutoFunction.h"
#include <atlconv.h>
#include <direct.h> //_mkdir������ͷ�ļ�
#include <io.h>     //_access������ͷ�ļ�
#include <iostream>
#include <cstdio>
#include <ctime>
#include <string>

using std::string;
using std::wstring;


//************************************************************
// ��������: CreateFileWithCurrentTime
// ����˵��: ���ݵ�ǰʱ�䴴���ļ�
// ��    ��: Greatfar
// ʱ    ��: 2019/9/24
// ��    ��: filedir �ļ�Ŀ¼ filepostfix�ļ���׺ filedata �ļ���ʼ��ַ filedatalen�ļ� ����
// �� �� ֵ: void 
//************************************************************
void CreateFileWithCurrentTime(char* filedir, char* filepostfix, DWORD filedata, DWORD filedatalen)
{
    //��ȡ��ǰʱ����Ϊ�ļ���
    time_t rawtime;
    struct tm *ptminfo = new struct tm;
    time(&rawtime);
    localtime_s(ptminfo, &rawtime);
    char currenttime[30] = { 0 };
    sprintf_s(currenttime, "%02d%02d%02d%02d%02d%02d", ptminfo->tm_year + 1900,
        ptminfo->tm_mon + 1, ptminfo->tm_mday, ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);

    //ƴ��������·��
    char filepath[MAX_PATH] = { 0 };
    sprintf_s(filepath, "%s%s%s", filedir, currenttime, filepostfix);
    //�����ļ�
    HANDLE hFile = CreateFileA(filepath, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBoxA(NULL, "�����ļ�ʧ��", "����", 0);
        return;
    }
    //д���ļ�
    DWORD dwWrite = 0;
    WriteFile(hFile, (LPCVOID)filedata, filedatalen, &dwWrite, 0);
    //�رվ��
    CloseHandle(hFile);
}



//************************************************************
// ��������: CreateDir
// ����˵��: ����Ŀ¼
// ��    ��: Greatfar
// ʱ    ��: 2019/7/21
// ��    ��: dir Ŀ¼
// �� �� ֵ: void
//************************************************************
void CreateDir(const char *dir)
{
    int m = 0, n;
    string str1, str2;

    str1 = dir;
    str2 = str1.substr(0, 2);
    str1 = str1.substr(3, str1.size());

    while (m >= 0)
    {
        m = str1.find('\\');
        str2 += '\\' + str1.substr(0, m);
        n = _access(str2.c_str(), 0); //�жϸ�Ŀ¼�Ƿ����
        if (n == -1)
        {
            _mkdir(str2.c_str());     //����Ŀ¼
        }
        str1 = str1.substr(m + 1, str1.size());
    }
}



//************************************************************
// ��������: GetXmlContent
// ����˵��: ��ȡ��Ҫ��XML����
// ��    ��: Greatfar
// ʱ    ��: 2019/11/15
// ��    ��: fullxmlcontent ������XML����  str1 ��Ҫ��ȡ��XML������ߵ��ַ��� str2 ��Ҫ��ȡ��XML�����ұߵ��ַ���
// �� �� ֵ: wstring ��Ҫ��ȡ��XML����
//************************************************************
wstring GetXmlContent(wstring fullxmlcontent, wstring str1, wstring str2)
{
    wstring targetstring;
    // �ҵ�<title>�ַ�����λ��
    int pos1 = fullxmlcontent.find(str1);
    //�ҵ�</title>�ַ�����λ��
    int pos2 = fullxmlcontent.find(str2);

    if (pos1==wstring::npos|| pos2== wstring::npos)
    {
        return L"NULL";
    }

    //ȡ��������ַ�������
    wstring noneedString1 = str1;
    int noneedLen1 = noneedString1.length();
    targetstring = fullxmlcontent.substr(pos1 + noneedLen1, pos2 - pos1 - noneedLen1);

    return targetstring;
}

