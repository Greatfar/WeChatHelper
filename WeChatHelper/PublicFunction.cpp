#include "stdafx.h"
#include "PublicFunction.h"
#include <locale>
#include <string>
#include "CHttpClient.h"
#include <TlHelp32.h>
#include <atlstr.h>

using namespace std;


//************************************************************
// ��������: unicode_to_utf8
// ����˵��: unicode��utf-16��ת UTF8
// ��    ��: Greatfar
// ʱ    ��: 2022/03/30
// ��    ��: str ��Ҫת�����ַ���
// �� �� ֵ: string ת������ַ���
//***********************************************************
std::string unicode_to_utf8(const std::string& str)
{
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    wchar_t* pwBuf = new wchar_t[nwLen + 1];  // һ��Ҫ��1����Ȼ�����β�� 
    ZeroMemory(pwBuf, nwLen * 2 + 2);

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);

    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr(pBuf);

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
std::string wchar_t_to_string(wchar_t* wchar)
{
    std::string szDst;
    wchar_t* wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);  // WideCharToMultiByte������
    char* psText;   // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
    psText = new char[dwNum];
    WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);  // WideCharToMultiByte���ٴ�����
    szDst = psText;  // std::string��ֵ
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
wchar_t* string_to_wchar_t(const std::string& str)
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
    std::string stflag = tflag;
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
void WriteLog(const std::string& content, string filePath)
{
    setlocale(LC_ALL, "zh_CN.UTF-8");
    std::string log;
    // ���ݸ�ʽ�� unicode��utf-16��תutf8
    log = unicode_to_utf8(
        FormatDateTime("%Y-%m-%d %H:%M:%S") + "    -    " + content +
        "\r\n" + "---------------------------------------\r\n\r\n\r\n"
    );
    // ��־�ļ���
    time_t timestamp = time(0);
    char tflag[64];
    strftime(tflag, sizeof(tflag), "./log/%Y%m%d-", localtime(&timestamp));
    std::string stflag = tflag;
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
std::string UrlEncode(const std::string& str)
{
    std::string strTemp = "";
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
std::string UrlDecode(const std::string& str)
{
    std::string strTemp = "";
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
// ��������: curlRequest
// ����˵��: ʹ��cURL����HTTP����
// ��    ��: Greatfar
// ʱ    ��: 2022/06/04
// ��    ��: url �����ַ
// ��    ��: postData ������(����ΪGET���󣬴���ǿ��ַ���ΪPOST����)
// �� �� ֵ: string ��Ӧ��
//***********************************************************
std::string curlRequest(string url, string postData)
{
    // Ĭ���ύ�Ĳ���
    if (postData == "")
    {
        string joiner = url.find("?") == string::npos ? "?" : "&";
        url += joiner + "version=" + (string)APP_VERSION;
    }
    else
    {
        postData += "&version=" + (string)APP_VERSION;
    }
    // ����HTTP����
    CHttpClient httpClient;
    string response = "";  // ��Ӧ��
    bool status = httpClient.Request(url, response, postData);
    // ����ʧ��
    if (!status) {
        response = "��������ʧ�ܣ�" + httpClient.GetErrMsg();
    }
    else {
        // ת���������������� ������ĿΪUnicode���̣�utf-16�����������󷵻ص���utf-8���룩
        wstring respondW = utf8_to_unicode(response);
        // wchar_t* ת���� string
        response = wchar_t_to_string((wchar_t*)respondW.c_str());
    }
    // ��¼��־
    string logContent = "������������\r\n�����ַ�� " + url + "\r\n��������� " + postData + "\r\n�������� " + response;
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
// ����˵��: ��CStringתΪstd::string
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
// ����˵��: ��std::stringתΪCString
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
    CString strName;
    GetPrivateProfileString(field, key, defaultValue, strName.GetBuffer(MAX_PATH), MAX_PATH, path);
    strName.ReleaseBuffer();
    return cstring_to_string(strName);
}

