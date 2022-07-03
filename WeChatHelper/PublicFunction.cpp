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
using namespace rapidjson;  // ʹ�� rapidjson �������ռ�


//************************************************************
// ��������: unicode_to_utf8
// ����˵��: unicode��utf-16��ת UTF8
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: str ��Ҫת�����ַ���
// �� �� ֵ: string ת������ַ���
//***********************************************************
string unicode_to_utf8(const string& str)
{
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    wchar_t* pwBuf = new wchar_t[nwLen + 1];  // һ��Ҫ��1����Ȼ�����β�� 
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
// ��������: utf8_to_unicode
// ����˵��: utf8 ת unicode��utf-16��
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: string ��Ҫת�����ַ���
// �� �� ֵ: wstring ת������ַ���
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
// ��������: wchar_t_to_string
// ����˵��: wchar_t ת string
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: wchar ��Ҫת�����ַ���
// �� �� ֵ: string ת������ַ���
//***********************************************************
string wchar_t_to_string(wchar_t* wchar)
{
    string szDst;
    wchar_t* wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);  // WideCharToMultiByte������
    char* psText;   // psTextΪchar*����ʱ���飬��Ϊ��ֵ��string���м����
    psText = new char[dwNum];
    WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);  // WideCharToMultiByte���ٴ�����
    szDst = psText;  // string��ֵ
    delete[]psText;  // psText�����
    return szDst;
}



//************************************************************
// ��������: string_to_wchar_t
// ����˵��: string ת wchar_t
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: str ��Ҫת�����ַ���
// �� �� ֵ: wchar_t* ת������ַ���
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
// ��������: GetValueFromXml
// ����˵��: ��XML��ȡ�ض�ֵ
// ��    ��: Greatfar
// ʱ    ��: 2022/06/02
// ��    ��: xmlStr xml�ַ���
// ��    ��: nodeName ��Ҫ��ȡ��xml��������
// ��    ��: nextNodeName ��һ�����Ե�����
// �� �� ֵ: string ���Ե�ֵ
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
// ��������: FormatDateTime
// ����˵��: ��ʽ����ǰʱ��
// ��    ��: Greatfar
// ʱ    ��: 2022/06/02
// ��    ��: string ʱ���ʽ
// �� �� ֵ: string 
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
// ��������: WriteLog
// ����˵��: д��־���ļ�
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: content ��־���� filePath ��־�ļ���
// �� �� ֵ: void
//***********************************************************
void WriteLog(const string& content, string filePath)
{
    setlocale(LC_ALL, "zh_CN.UTF-8");
    string log;
    // ���ݸ�ʽ�� unicode��utf-16��תutf8
    log = unicode_to_utf8(
        FormatDateTime("%Y-%m-%d %H:%M:%S") + "    -    " + content +
        "\r\n" + "---------------------------------------\r\n\r\n\r\n"
    );
    // ��־�ļ���
    time_t timestamp = time(0);
    char tflag[64];
    strftime(tflag, sizeof(tflag), "./log/%Y%m%d-", localtime(&timestamp));
    string stflag = tflag;
    string path = stflag + filePath;
    // д����־
    FILE* fp = fopen(path.c_str(), "ab+");
    fwrite(log.c_str(), log.length(), 1, fp);
    fclose(fp);
}



//************************************************************
// ��������: ToHex
// ����˵��: �ַ�ת16����
// ��    ��: Greatfar
// ʱ    ��: 2022/04/08
// ��    ��: char
// �� �� ֵ: char
//***********************************************************
unsigned char ToHex(unsigned char x)
{
    return  x > 9 ? x + 55 : x + 48;
}



//************************************************************
// ��������: FromHex
// ����˵��: 16����ת�ַ�
// ��    ��: Greatfar
// ʱ    ��: 2022/04/08
// ��    ��: char
// �� �� ֵ: char
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
// ��������: UrlEncode
// ����˵��: URL����
// ��    ��: Greatfar
// ʱ    ��: 2022/04/08
// ��    ��: string
// �� �� ֵ: string
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
// ��������: UrlDecode
// ����˵��: URL����
// ��    ��: Greatfar
// ʱ    ��: 2022/04/08
// ��    ��: string
// �� �� ֵ: string
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
// ��������: CurlRequest
// ����˵��: ʹ��cURL����HTTP����
// ��    ��: Greatfar
// ʱ    ��: 2022/06/04
// ��    ��: url �����ַ
// ��    ��: postData ������(����ΪGET���󣬴���ǿ��ַ���ΪPOST����)
// �� �� ֵ: string ��Ӧ��
//***********************************************************
string CurlRequest(string url, string postData)
{
    // ����HTTP����
    CHttpClient httpClient;
    string response = "";  // ��Ӧ��
    bool status = httpClient.Request(url, response, postData);
    // ����ʧ��
    if (!status)
    {
        response = "��������ʧ�ܣ�" + httpClient.GetErrMsg();
    }
    else
    {
        // ת���������������� ������ĿΪUnicode���̣�utf-16�����������󷵻ص���utf-8���룩
        wstring respondW = utf8_to_unicode(response);
        // wchar_t* ת���� string
        response = wchar_t_to_string((wchar_t*)respondW.c_str());
    }
    // ��¼��־
    string logContent = "������������\r\n�����ַ�� " + url;
    if (postData != "") logContent += "\r\n��������� " + postData;
    logContent += "\r\n�������� " + response;
    WriteLog(logContent.c_str());
    // ��������
    return response;
}



//************************************************************
// ��������: ProcessNameFindPID
// ����˵��: ͨ���������ҵ�����ID
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: ProcessName ������
// �� �� ֵ: DWORD ����PID
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
// ��������: cstring_to_string
// ����˵��: ��CStringתΪstring
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: cstr CString
// �� �� ֵ: string
//************************************************************
string cstring_to_string(const CString& cstr)
{
    CStringA stra((TCHAR*)(LPCTSTR)(cstr));
    string str((char*)(LPCSTR)(stra));
    return str;
}



//************************************************************
// ��������: string_to_cstring
// ����˵��: ��stringתΪCString
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: str string
// �� �� ֵ: CString
//************************************************************
CString string_to_cstring(const string& str)
{
    CString cstr(str.c_str(), str.length());
    return  cstr;
}



//************************************************************
// ��������: CreatePathIfNotExist
// ����˵��: �����־Ŀ¼�Ƿ���ڣ��������򴴽�
// ��    ��: Greatfar
// ʱ    ��: 2022/03/23
// ��    ��: void
// �� �� ֵ: void
//***********************************************************
void CreatePathIfNotExist(CString path)
{
    //  �����־Ŀ¼�Ƿ����
    if (!PathIsDirectory(path))
    {
        // ����Ŀ¼
        CreateDirectory(path, NULL);
    }
}



//************************************************************
// ��������: GetValueFromeConfig
// ����˵��: �������ļ��л�ȡ����
// ��    ��: Greatfar
// ʱ    ��: 2022/03/23
// ��    ��: path �����ļ�·��
// ��    ��: field �����ֶ�
// ��    ��: key ���ü�
// ��    ��: defaultValue Ĭ��ֵ
// �� �� ֵ: string ���õ�ֵ
//***********************************************************
string GetValueFromeConfig(CString path, CString field, CString key, CString defaultValue)
{
    CString cfgVaule;
    GetPrivateProfileString(field, key, defaultValue, cfgVaule.GetBuffer(MAX_PATH), MAX_PATH, path);
    cfgVaule.ReleaseBuffer();
    return cstring_to_string(cfgVaule);
}



//************************************************************
// ��������: AppUpdateThread
// ����˵��: Ӧ�ø��£��̺߳�����
// ��    ��: Greatfar
// ʱ    ��: 2022/07/03
// ��    ��: LPVOID
// �� �� ֵ: UINT
//***********************************************************
UINT AppUpdateThread(LPVOID lParam)
{
    // ��������½ӿ�
    string updateApi = GetValueFromeConfig(_T("./config.ini"), _T("api"), _T("update"), _T(""));
    string appVersion = GetValueFromeConfig(_T("./config.ini"), _T("base"), _T("app_version"), _T(""));
    string updateResponse = CurlRequest(updateApi + "?version=" + appVersion);
    // �����Ӧ���ݲ���json
    if (updateResponse.find("code") == string::npos) {
        return 0;
    }
    // ʹ�� rapidjson ���� json ����
    Document document;
    document.Parse(updateResponse.c_str());
    // �жϽӿ�״̬
    if (document.HasMember("code") && document["code"].IsInt())
    {
        int resCode = document["code"].GetInt();
        // �ӿڴ���
        if (resCode != 200)
        {
            return 0;
        }
    }
    // ��ȡ����״̬
    if (document.HasMember("data") && document["data"].IsObject())
    {
        // ��ȡ���±�ʶ
        bool isUpdate = false;
        if (document["data"].HasMember("is_update") && document["data"]["is_update"].IsBool())
        {
            isUpdate = document["data"]["is_update"].GetBool();
        }
        // ����������
        if (!isUpdate) return 0;
        // ��ȡ��������
        Value::MemberIterator link = document["data"].FindMember("down_link");
        string downLink = link->value.GetString();
        // ��ȡ�汾��
        Value::MemberIterator downAppVersionValue = document["data"].FindMember("app_version");
        string downAppVersion = downAppVersionValue->value.GetString();
        string lastDownVersion = GetValueFromeConfig(_T("./config.ini"), _T("update"), _T("last_download"), _T(""));
        // ����ϴγɹ����صİ汾�뵱ǰҪ���صİ汾��һ�� �� update.exe ������  �����ظ����أ���ʡ����
        if (downAppVersion != lastDownVersion || !FileExist(L"update.exe"))
        {
            // ���ظ��°�
            CHttpClient httpClient;
            httpClient.downloadFile(downLink.c_str(), "update.exe");
        }
        // ���浱ǰ�����ذ汾�ŵ������ļ�
        WritePrivateProfileString(_T("update"), _T("last_download"), (LPCWSTR)string_to_wchar_t(downAppVersion), _T("./config.ini"));
        // ��ʾ��װ����
        if (MessageBoxA(NULL, "�����°汾��΢�����֣��Ƿ�����������", "�������", MB_SYSTEMMODAL | MB_YESNO) == IDYES)
        {
            // ����CMDִ�и���
            string cmd = "taskkill /f /im WeChat.exe & taskkill /f /im WeChatHelper.exe & start " + cstring_to_string(GetCurrentPath()) + "update.exe";
            system(cmd.c_str());
        }
    }
    // �̺߳������뷵��һ��INTֵ
    return 0;
}



//************************************************************
// ��������: GetCurrentPath
// ����˵��: ��ȡ��ǰ��������Ŀ¼
// ��    ��: Greatfar
// ʱ    ��: 2022/02/23
// ��    ��: void
// �� �� ֵ: CString
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
// ��������: FileExist
// ����˵��: ���ָ�����ļ��Ƿ���ڣ��ļ������򷵻� TRUE, �������򷵻� FALSE��
// ��    ��: Greatfar
// ʱ    ��: 2022/02/23
// ��    ��: lpszFile �ļ��ľ���·��, ��: C:\\a.txt
// �� �� ֵ: BOOL
//***********************************************************
BOOL FileExist(LPCTSTR lpszFile)
{
    HANDLE hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    CloseHandle(hFile);
    return hFile != INVALID_HANDLE_VALUE;
}



