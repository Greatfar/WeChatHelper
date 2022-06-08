#pragma once
#include <atlstr.h>

void CreateDir(const char *dir);        //创建目录
void CreateFileWithCurrentTime(char* filedir, char* filepostfix, DWORD filedata, DWORD filedatalen);    //用当前时间创建文件
std::wstring GetXmlContent(std::wstring fullxmlcontent, std::wstring str1, std::wstring str2);    //获取需要的XML内容
