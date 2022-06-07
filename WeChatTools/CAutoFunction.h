#pragma once
#include <atlstr.h>

void CreateDir(const char *dir);        //创建目录
void CreateFileWithCurrentTime(char* filedir, char* filepostfix, DWORD filedata, DWORD filedatalen);    //用当前时间创建文件
wstring GetXmlContent(wstring fullxmlcontent, wstring str1, wstring str2);    //获取需要的XML内容
