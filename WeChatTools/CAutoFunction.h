#pragma once
#include <atlstr.h>

void CreateDir(const char *dir);        //����Ŀ¼
void CreateFileWithCurrentTime(char* filedir, char* filepostfix, DWORD filedata, DWORD filedatalen);    //�õ�ǰʱ�䴴���ļ�
wstring GetXmlContent(wstring fullxmlcontent, wstring str1, wstring str2);    //��ȡ��Ҫ��XML����
