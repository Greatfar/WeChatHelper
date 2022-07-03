#include "stdafx.h"
#include "CHttpClient.h"
#include "curl/curl.h"
#include <string>

using std::string;


CHttpClient::CHttpClient(void) : isDebug(false)
{

}

CHttpClient::~CHttpClient(void)
{

}


/**
 * @brief ���Է���
 * @link https://curl.se/libcurl/
 * @return int
 */
static int OnDebug(CURL*, curl_infotype itype, char* pData, size_t size, void*)
{
    if (itype == CURLINFO_TEXT)
    {
        // printf("[TEXT]%s\n", pData);
    }
    else if (itype == CURLINFO_HEADER_IN)
    {
        printf("[HEADER_IN]%s\n", pData);
    }
    else if (itype == CURLINFO_HEADER_OUT)
    {
        printf("[HEADER_OUT]%s\n", pData);
    }
    else if (itype == CURLINFO_DATA_IN)
    {
        printf("[DATA_IN]%s\n", pData);
    }
    else if (itype == CURLINFO_DATA_OUT)
    {
        printf("[DATA_OUT]%s\n", pData);
    }
    return 0;
}


/**
 * @brief ��ȡHTTP��Ӧ��
 * @link https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 * @return size_t
 */
static size_t GetResponseBody(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    string* str = dynamic_cast<string*>((string*)lpVoid);
    if (NULL == str || NULL == buffer)
    {
        return -1;
    }

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}



/**
 * @brief ��Ӧ����д���ļ�
 * @return size_t
 */
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}



/**
 * @brief �����ļ�
 * @param strUrl ���ص�ַ
 * @param outfilename �ļ���
 * @return int
 */
int CHttpClient::downloadFile(const char* url, const char outfilename[FILENAME_MAX]) {
    CURL* curl;
    FILE* fp;
    CURLcode res;
    // ����curl_global_init()��ʼ��libcurl
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != res)
    {
        printf("init libcurl failed.");
        curl_global_cleanup();
        return -1;
    }
    // ����curl_easy_init()�����õ� easy interface��ָ��
    curl = curl_easy_init();
    if (curl) {
        // ���ļ� д��ģʽ wb����ļ����ھ�ɾ���󴴽�
        fopen_s(&fp, outfilename, "wb");
        // ����curl_easy_setopt() ���ô���ѡ��
        res = curl_easy_setopt(curl, CURLOPT_URL, url);
        if (res != CURLE_OK)
        {
            fp != 0 && fclose(fp);
            curl_easy_cleanup(curl);
            return -1;
        }
        // ����curl_easy_setopt()���õĴ���ѡ�ʵ�ֻص�����������û��ض�����
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        if (res != CURLE_OK)
        {
            fp != 0 && fclose(fp);
            curl_easy_cleanup(curl);
            return -1;
        }
        // ����curl_easy_setopt()���õĴ���ѡ�ʵ�ֻص�����������û��ض�����
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        if (res != CURLE_OK)
        {
            fp != 0 && fclose(fp);
            curl_easy_cleanup(curl);
            return -1;
        }// ����curl_easy_perform()������ɴ�������
        res = curl_easy_perform(curl);
        fp != 0 && fclose(fp);
        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return -1;
        }
        // always cleanup
        curl_easy_cleanup(curl);  // ����curl_easy_cleanup()�ͷ��ڴ�
    }
    curl_global_cleanup();
    return 0;
}



/**
 * @brief ��������
 * @param strUrl �����url
 * @param strResponse ��Ӧ����
 * @param resBody �����壨����������ΪGET���󣬴�������ΪPOST��
 * @return �����Ƿ�����ɹ�
 */
bool CHttpClient::Request(const string& strUrl, string& strResponse, const string& resBody)
{
    // ��ʼ��cURL
    CURL* curl = curl_easy_init();
    if (NULL == curl)
    {
        string errMsg(curl_easy_strerror(CURLE_FAILED_INIT));
        this->errorMsg = errMsg;
        return false;
    }
    // ����ģʽ
    if (isDebug)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
    }
    // ����URL
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    // ����body ��POST����
    if (resBody != "") {
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, resBody.c_str());
    }
    // ����HTTP��Ӧ��
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetResponseBody);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResponse);
    // ����ʱ
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    // ����SSL֤��У��
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    // ��������
    CURLcode curlCode = curl_easy_perform(curl);
    // ��������״̬
    if (curlCode != CURLE_OK) {
        string errMsg(curl_easy_strerror(curlCode));
        this->errorMsg = errMsg;
        curl_easy_cleanup(curl);
        // ��������ʧ��
        return false;
    }
    else
    {
        // ��Ӧ״̬��
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &this->responseCode);
    }
    curl_easy_cleanup(curl);
    // ��������ɹ�
    return true;
}


/**
 * @brief �������󣨴�ca֤��У�飩
 * @param strUrl �����url
 * @param strResponse ��Ӧ����
 * @param resBody �����壨����������ΪGET���󣬴�������ΪPOST��
 * @param pCaPath ca֤��·��
 * @return �����Ƿ�����ɹ�
 */
bool CHttpClient::caRequest(const string& strUrl, string& strResponse, const string& resBody, const char* pCaPath)
{
    // ��ʼ��cURL
    CURL* curl = curl_easy_init();
    if (NULL == curl)
    {
        string errMsg(curl_easy_strerror(CURLE_FAILED_INIT));
        this->errorMsg = errMsg;
        return false;
    }
    // ����ģʽ
    if (isDebug)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
    }
    // ����URL
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    // ����body ��POST����
    if (resBody != "") {
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, resBody.c_str());
    }
    // ����HTTP��Ӧ��
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetResponseBody);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResponse);
    // ����ʱ
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    // ����SSL֤��У��
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    // ca֤��У��
    if (NULL == pCaPath)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    }
    else
    {
        // ȱʡ��PEM��ʽ���������ã�����֧��DER
        // curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
    }
    // ��������
    CURLcode curlCode = curl_easy_perform(curl);
    // ��������״̬
    if (curlCode != CURLE_OK) {
        string errMsg(curl_easy_strerror(curlCode));
        this->errorMsg = errMsg;
        curl_easy_cleanup(curl);
        // ��������ʧ��
        return false;
    }
    else
    {
        // ��Ӧ״̬��
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &this->responseCode);
    }
    curl_easy_cleanup(curl);
    // ��������ɹ�
    return true;
}


/**
 * @brief ���õ��Կ���
 * @param flag ���Ա�ʶ
 * @return string ������Ϣ
 */
void CHttpClient::SetDebug(bool flag)
{
    isDebug = flag;
}


/**
 * @brief ��ȡcURL���������ʾ
 * @return string ������Ϣ
 */
string CHttpClient::GetErrMsg()
{
    return this->errorMsg;
}


/**
 * @brief ��ȡhttp��Ӧ״̬��
 * @return int http״̬��
 */
long CHttpClient::GetResponseCode()
{
    return this->responseCode;
}

