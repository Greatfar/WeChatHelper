#pragma once
#include <atlstr.h>

void CreateDir(const char *dir);        //����Ŀ¼
void CreateFileWithCurrentTime(char* filedir, char* filepostfix, DWORD filedata, DWORD filedatalen);    //�õ�ǰʱ�䴴���ļ�
std::wstring GetXmlContent(std::wstring fullxmlcontent, std::wstring str1, std::wstring str2);    //��ȡ��Ҫ��XML����
