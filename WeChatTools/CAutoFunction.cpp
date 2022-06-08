#include "stdafx.h"
#include "CAutoFunction.h"
#include <atlconv.h>
#include <direct.h> //_mkdir函数的头文件
#include <io.h>     //_access函数的头文件
#include <iostream>
#include <cstdio>
#include <ctime>
#include <string>

using std::string;
using std::wstring;


//************************************************************
// 函数名称: CreateFileWithCurrentTime
// 函数说明: 根据当前时间创建文件
// 作    者: Greatfar
// 时    间: 2019/9/24
// 参    数: filedir 文件目录 filepostfix文件后缀 filedata 文件开始地址 filedatalen文件 长度
// 返 回 值: void 
//************************************************************
void CreateFileWithCurrentTime(char* filedir, char* filepostfix, DWORD filedata, DWORD filedatalen)
{
    //获取当前时间作为文件名
    time_t rawtime;
    struct tm *ptminfo = new struct tm;
    time(&rawtime);
    localtime_s(ptminfo, &rawtime);
    char currenttime[30] = { 0 };
    sprintf_s(currenttime, "%02d%02d%02d%02d%02d%02d", ptminfo->tm_year + 1900,
        ptminfo->tm_mon + 1, ptminfo->tm_mday, ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);

    //拼接完整的路径
    char filepath[MAX_PATH] = { 0 };
    sprintf_s(filepath, "%s%s%s", filedir, currenttime, filepostfix);
    //创建文件
    HANDLE hFile = CreateFileA(filepath, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBoxA(NULL, "创建文件失败", "错误", 0);
        return;
    }
    //写入文件
    DWORD dwWrite = 0;
    WriteFile(hFile, (LPCVOID)filedata, filedatalen, &dwWrite, 0);
    //关闭句柄
    CloseHandle(hFile);
}



//************************************************************
// 函数名称: CreateDir
// 函数说明: 创建目录
// 作    者: Greatfar
// 时    间: 2019/7/21
// 参    数: dir 目录
// 返 回 值: void
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
        n = _access(str2.c_str(), 0); //判断该目录是否存在
        if (n == -1)
        {
            _mkdir(str2.c_str());     //创建目录
        }
        str1 = str1.substr(m + 1, str1.size());
    }
}



//************************************************************
// 函数名称: GetXmlContent
// 函数说明: 获取需要的XML内容
// 作    者: Greatfar
// 时    间: 2019/11/15
// 参    数: fullxmlcontent 完整的XML内容  str1 需要获取的XML内容左边的字符串 str2 需要获取的XML内容右边的字符串
// 返 回 值: wstring 需要获取的XML内容
//************************************************************
wstring GetXmlContent(wstring fullxmlcontent, wstring str1, wstring str2)
{
    wstring targetstring;
    // 找到<title>字符串的位置
    int pos1 = fullxmlcontent.find(str1);
    //找到</title>字符串的位置
    int pos2 = fullxmlcontent.find(str2);

    if (pos1==wstring::npos|| pos2== wstring::npos)
    {
        return L"NULL";
    }

    //取出多余的字符串长度
    wstring noneedString1 = str1;
    int noneedLen1 = noneedString1.length();
    targetstring = fullxmlcontent.substr(pos1 + noneedLen1, pos2 - pos1 - noneedLen1);

    return targetstring;
}

