#include "stdafx.h"
#include "PublicFunction.h"
#include <locale>
#include <string>
#include "CHttpClient.h"
#include <TlHelp32.h>
#include <atlstr.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


using std::string;
using std::wstring;
using namespace rapidjson;  // 使用 rapidjson 的命名空间


//************************************************************
// 函数名称: unicode_to_utf8
// 函数说明: unicode（utf-16）转 UTF8
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: str 需要转换的字符串
// 返 回 值: string 转换后的字符串
//***********************************************************
string unicode_to_utf8(const string& str)
{
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    wchar_t* pwBuf = new wchar_t[nwLen + 1];  // 一定要加1，不然会出现尾巴 
    ZeroMemory(pwBuf, nwLen * 2 + 2);

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);

    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    string retStr(pBuf);

    delete[]pwBuf;
    delete[]pBuf;

    pwBuf = NULL;
    pBuf = NULL;

    return retStr;
}



//************************************************************
// 函数名称: utf8_to_unicode
// 函数说明: utf8 转 unicode（utf-16）
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: string 需要转换的字符串
// 返 回 值: wstring 转换后的字符串
//***********************************************************
wstring utf8_to_unicode(const string& utf8_str)
{
    int len;
    len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8_str.c_str(), -1, NULL, 0);
    WCHAR* wszUnicode = new WCHAR[len + 1];
    memset(wszUnicode, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8_str.c_str(), -1, wszUnicode, len);
    wstring unicodeWstring(wszUnicode);
    delete wszUnicode;
    return unicodeWstring;
}



//************************************************************
// 函数名称: wchar_t_to_string
// 函数说明: wchar_t 转 string
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: wchar 需要转换的字符串
// 返 回 值: string 转换后的字符串
//***********************************************************
string wchar_t_to_string(wchar_t* wchar)
{
    string szDst;
    wchar_t* wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);  // WideCharToMultiByte的运用
    char* psText;   // psText为char*的临时数组，作为赋值给string的中间变量
    psText = new char[dwNum];
    WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);  // WideCharToMultiByte的再次运用
    szDst = psText;  // string赋值
    delete[]psText;  // psText的清除
    return szDst;
}



//************************************************************
// 函数名称: string_to_wchar_t
// 函数说明: string 转 wchar_t
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: str 需要转换的字符串
// 返 回 值: wchar_t* 转换后的字符串
//***********************************************************
wchar_t* string_to_wchar_t(const string& str)
{
    wchar_t* m_chatroommmsg = new wchar_t[str.size() * 2];  //
    memset(m_chatroommmsg, 0, str.size() * 2);
    setlocale(LC_ALL, "zh_CN.UTF-8");
    swprintf(m_chatroommmsg, str.size() * 2, L"%S", str.c_str());
    return m_chatroommmsg;
}



//************************************************************
// 函数名称: GetValueFromXml
// 函数说明: 从XML截取特定值
// 作    者: Greatfar
// 时    间: 2022/06/02
// 参    数: xmlStr xml字符串
// 参    数: nodeName 需要读取的xml属性名称
// 参    数: nextNodeName 下一个属性的名称
// 返 回 值: string 属性的值
//***********************************************************
string GetValueFromXml(string& xmlStr, string nodeName, string nextNodeName)
{
    int posStrat = xmlStr.find(nodeName) + nodeName.length() + 2;
    int posEnd = xmlStr.find(nextNodeName) - 2;
    string nodeValue = "";
    if (posStrat > 0 && posEnd > 0 && posEnd > posStrat) {
        nodeValue = xmlStr.substr(posStrat, posEnd - posStrat);
    }
    return nodeValue;
}



//************************************************************
// 函数名称: FormatDateTime
// 函数说明: 格式化当前时间
// 作    者: Greatfar
// 时    间: 2022/06/02
// 参    数: string 时间格式
// 返 回 值: string 
//***********************************************************
string FormatDateTime(string fmt)
{
    time_t t = time(0);
    char tflag[64];
    strftime(tflag, sizeof(tflag), fmt.c_str(), localtime(&t));
    string stflag = tflag;
    return stflag;
}



//************************************************************
// 函数名称: WriteLog
// 函数说明: 写日志到文件
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: content 日志内容 filePath 日志文件名
// 返 回 值: void
//***********************************************************
void WriteLog(const string& content, string filePath)
{
    setlocale(LC_ALL, "zh_CN.UTF-8");
    string log;
    // 内容格式化 unicode（utf-16）转utf8
    log = unicode_to_utf8(
        FormatDateTime("%Y-%m-%d %H:%M:%S") + "    -    " + content +
        "\r\n" + "---------------------------------------\r\n\r\n\r\n"
    );
    // 日志文件名
    time_t timestamp = time(0);
    char tflag[64];
    strftime(tflag, sizeof(tflag), "./log/%Y%m%d-", localtime(&timestamp));
    string stflag = tflag;
    string path = stflag + filePath;
    // 写入日志
    FILE* fp = fopen(path.c_str(), "ab+");
    fwrite(log.c_str(), log.length(), 1, fp);
    fclose(fp);
}



//************************************************************
// 函数名称: ToHex
// 函数说明: 字符转16进制
// 作    者: Greatfar
// 时    间: 2022/04/08
// 参    数: char
// 返 回 值: char
//***********************************************************
unsigned char ToHex(unsigned char x)
{
    return  x > 9 ? x + 55 : x + 48;
}



//************************************************************
// 函数名称: FromHex
// 函数说明: 16进制转字符
// 作    者: Greatfar
// 时    间: 2022/04/08
// 参    数: char
// 返 回 值: char
//***********************************************************
unsigned char FromHex(unsigned char x)
{
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else ASSERT(0);
    return y;
}



//************************************************************
// 函数名称: UrlEncode
// 函数说明: URL编码
// 作    者: Greatfar
// 时    间: 2022/04/08
// 参    数: string
// 返 回 值: string
//***********************************************************
string UrlEncode(const string& str)
{
    string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}



//************************************************************
// 函数名称: UrlDecode
// 函数说明: URL解码
// 作    者: Greatfar
// 时    间: 2022/04/08
// 参    数: string
// 返 回 值: string
//***********************************************************
string UrlDecode(const string& str)
{
    string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            ASSERT(i + 2 < length);
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            strTemp += high * 16 + low;
        }
        else strTemp += str[i];
    }
    return strTemp;
}



//************************************************************
// 函数名称: CurlRequest
// 函数说明: 使用cURL发送HTTP请求
// 作    者: Greatfar
// 时    间: 2022/06/04
// 参    数: url 请求地址
// 参    数: postData 请求体(不传为GET请求，传入非空字符串为POST请求)
// 返 回 值: string 响应体
//***********************************************************
string CurlRequest(string url, string postData)
{
    // 发送HTTP请求
    CHttpClient httpClient;
    string response = "";  // 响应体
    bool status = httpClient.Request(url, response, postData);
    // 请求失败
    if (!status)
    {
        response = "网络请求失败：" + httpClient.GetErrMsg();
    }
    else
    {
        // 转码解决中文乱码问题 （本项目为Unicode工程（utf-16），网络请求返回的是utf-8编码）
        wstring respondW = utf8_to_unicode(response);
        // wchar_t* 转换成 string
        response = wchar_t_to_string((wchar_t*)respondW.c_str());
    }
    // 记录日志
    string logContent = "发送网络请求：\r\n请求地址： " + url;
    if (postData != "") logContent += "\r\n请求参数： " + postData;
    logContent += "\r\n请求结果： " + response;
    WriteLog(logContent.c_str());
    // 返回数据
    return response;
}



//************************************************************
// 函数名称: ProcessNameFindPID
// 函数说明: 通过进程名找到进程ID
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: ProcessName 进程名
// 返 回 值: DWORD 进程PID
//************************************************************
DWORD ProcessNameFindPID(const char* ProcessName)
{
    PROCESSENTRY32 pe32 = { 0 };
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(hProcess, &pe32) == TRUE)
    {
        do
        {
            USES_CONVERSION;
            if (strcmp(ProcessName, W2A(pe32.szExeFile)) == 0)
            {
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hProcess, &pe32));
    }
    return 0;
}



//************************************************************
// 函数名称: cstring_to_string
// 函数说明: 将CString转为string
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: cstr CString
// 返 回 值: string
//************************************************************
string cstring_to_string(const CString& cstr)
{
    CStringA stra((TCHAR*)(LPCTSTR)(cstr));
    string str((char*)(LPCSTR)(stra));
    return str;
}



//************************************************************
// 函数名称: string_to_cstring
// 函数说明: 将string转为CString
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: str string
// 返 回 值: CString
//************************************************************
CString string_to_cstring(const string& str)
{
    CString cstr(str.c_str(), str.length());
    return  cstr;
}



//************************************************************
// 函数名称: CreatePathIfNotExist
// 函数说明: 检查日志目录是否存在，不存在则创建
// 作    者: Greatfar
// 时    间: 2022/03/23
// 参    数: void
// 返 回 值: void
//***********************************************************
void CreatePathIfNotExist(CString path)
{
    //  检查日志目录是否存在
    if (!PathIsDirectory(path))
    {
        // 创建目录
        CreateDirectory(path, NULL);
    }
}



//************************************************************
// 函数名称: GetValueFromeConfig
// 函数说明: 从配置文件中获取配置
// 作    者: Greatfar
// 时    间: 2022/03/23
// 参    数: path 配置文件路径
// 参    数: field 配置字段
// 参    数: key 配置键
// 参    数: defaultValue 默认值
// 返 回 值: string 配置的值
//***********************************************************
string GetValueFromeConfig(CString path, CString field, CString key, CString defaultValue)
{
    CString cfgVaule;
    GetPrivateProfileString(field, key, defaultValue, cfgVaule.GetBuffer(MAX_PATH), MAX_PATH, path);
    cfgVaule.ReleaseBuffer();
    return cstring_to_string(cfgVaule);
}



//************************************************************
// 函数名称: AppUpdateThread
// 函数说明: 应用更新（线程函数）
// 作    者: Greatfar
// 时    间: 2022/07/03
// 参    数: LPVOID
// 返 回 值: UINT
//***********************************************************
UINT AppUpdateThread(LPVOID lParam)
{
    // 请求检查更新接口
    string updateApi = GetValueFromeConfig(_T("./config.ini"), _T("api"), _T("update"), _T(""));
    string appVersion = GetValueFromeConfig(_T("./config.ini"), _T("base"), _T("app_version"), _T(""));
    string updateResponse = CurlRequest(updateApi + "?version=" + appVersion);
    // 如果响应数据不是json
    if (updateResponse.find("code") == string::npos) {
        return 0;
    }
    // 使用 rapidjson 解析 json 数据
    Document document;
    document.Parse(updateResponse.c_str());
    // 判断接口状态
    if (document.HasMember("code") && document["code"].IsInt())
    {
        int resCode = document["code"].GetInt();
        // 接口错误
        if (resCode != 200)
        {
            return 0;
        }
    }
    // 获取更新状态
    if (document.HasMember("data") && document["data"].IsObject())
    {
        // 读取更新标识
        bool isUpdate = false;
        if (document["data"].HasMember("is_update") && document["data"]["is_update"].IsBool())
        {
            isUpdate = document["data"]["is_update"].GetBool();
        }
        // 如果无需更新
        if (!isUpdate) return 0;
        // 读取更新链接
        Value::MemberIterator link = document["data"].FindMember("down_link");
        string downLink = link->value.GetString();
        // 读取版本号
        Value::MemberIterator downAppVersionValue = document["data"].FindMember("app_version");
        string downAppVersion = downAppVersionValue->value.GetString();
        string lastDownVersion = GetValueFromeConfig(_T("./config.ini"), _T("update"), _T("last_download"), _T(""));
        // 如果上次成功下载的版本与当前要下载的版本不一致 或 update.exe 不存在  减少重复下载，节省带宽
        if (downAppVersion != lastDownVersion || !FileExist(L"update.exe"))
        {
            // 下载更新包
            CHttpClient httpClient;
            httpClient.downloadFile(downLink.c_str(), "update.exe");
        }
        // 保存当前已下载版本号到配置文件
        WritePrivateProfileString(_T("update"), _T("last_download"), (LPCWSTR)string_to_wchar_t(downAppVersion), _T("./config.ini"));
        // 提示安装更新
        if (MessageBoxA(NULL, "发现新版本的微信助手，是否立即升级？", "软件升级", MB_SYSTEMMODAL | MB_YESNO) == IDYES)
        {
            // 调用CMD执行更新
            string cmd = "taskkill /f /im WeChat.exe & taskkill /f /im WeChatHelper.exe & start " + cstring_to_string(GetCurrentPath()) + "update.exe";
            system(cmd.c_str());
        }
    }
    // 线程函数必须返回一个INT值
    return 0;
}



//************************************************************
// 函数名称: GetCurrentPath
// 函数说明: 获取当前程序运行目录
// 作    者: Greatfar
// 时    间: 2022/02/23
// 参    数: void
// 返 回 值: CString
//***********************************************************
CString GetCurrentPath()
{
    CString  strProgramPath;
    GetModuleFileName(NULL, strProgramPath.GetBuffer(MAX_PATH), MAX_PATH);
    strProgramPath.ReleaseBuffer(MAX_PATH);
    int nPathPos = strProgramPath.ReverseFind('\\');
    strProgramPath = strProgramPath.Left(nPathPos + 1);
    return strProgramPath;
}



//************************************************************
// 函数名称: FileExist
// 函数说明: 检查指定的文件是否存在（文件存在则返回 TRUE, 不存在则返回 FALSE）
// 作    者: Greatfar
// 时    间: 2022/02/23
// 参    数: lpszFile 文件的绝对路径, 例: C:\\a.txt
// 返 回 值: BOOL
//***********************************************************
BOOL FileExist(LPCTSTR lpszFile)
{
    HANDLE hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    CloseHandle(hFile);
    return hFile != INVALID_HANDLE_VALUE;
}



