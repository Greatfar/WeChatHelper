#include "stdafx.h"
#include "CHttpClient.h"
#include "curl/curl.h"
#include <string>

CHttpClient::CHttpClient(void) : isDebug(false)
{

}

CHttpClient::~CHttpClient(void)
{

}


/**
 * @brief 调试方法
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
 * @brief 获取HTTP响应体
 * @link https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 * @return size_t
 */
static size_t GetResponseBody(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    std::string* str = dynamic_cast<std::string*>((std::string*)lpVoid);
    if (NULL == str || NULL == buffer)
    {
        return -1;
    }

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}


/**
 * @brief 发送请求
 * @param strUrl 请求的url
 * @param strResponse 响应内容
 * @param resBody 请求体（不传请求体为GET请求，传请求体为POST）
 * @return 返回是否请求成功
 */
bool CHttpClient::Request(const std::string& strUrl, std::string& strResponse, const std::string& resBody)
{
    // 初始化cURL
    CURL* curl = curl_easy_init();
    if (NULL == curl)
    {
        string errMsg(curl_easy_strerror(CURLE_FAILED_INIT));
        this->errorMsg = errMsg;
        return false;
    }
    // 调试模式
    if (isDebug)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
    }
    // 请求URL
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    // 请求body （POST请求）
    if (resBody != "") {
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, resBody.c_str());
    }
    // 接收HTTP响应体
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetResponseBody);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResponse);
    // 请求超时
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    // 忽略SSL证书校验
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    // 发送请求
    CURLcode curlCode = curl_easy_perform(curl);
    // 处理请求状态
    if (curlCode != CURLE_OK) {
        string errMsg(curl_easy_strerror(curlCode));
        this->errorMsg = errMsg;
        curl_easy_cleanup(curl);
        // 返回请求失败
        return false;
    }
    else
    {
        // 响应状态码
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &this->responseCode);
    }
    curl_easy_cleanup(curl);
    // 返回请求成功
    return true;
}


/**
 * @brief 发送请求（带ca证书校验）
 * @param strUrl 请求的url
 * @param strResponse 响应内容
 * @param resBody 请求体（不传请求体为GET请求，传请求体为POST）
 * @param pCaPath ca证书路径
 * @return 返回是否请求成功
 */
bool CHttpClient::caRequest(const std::string& strUrl, std::string& strResponse, const std::string& resBody, const char* pCaPath)
{
    // 初始化cURL
    CURL* curl = curl_easy_init();
    if (NULL == curl)
    {
        string errMsg(curl_easy_strerror(CURLE_FAILED_INIT));
        this->errorMsg = errMsg;
        return false;
    }
    // 调试模式
    if (isDebug)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
    }
    // 请求URL
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    // 请求body （POST请求）
    if (resBody != "") {
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, resBody.c_str());
    }
    // 接收HTTP响应体
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetResponseBody);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResponse);
    // 请求超时
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    // 忽略SSL证书校验
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    // ca证书校验
    if (NULL == pCaPath)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    }
    else
    {
        // 缺省是PEM格式，无需设置，另外支持DER
        // curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
    }
    // 发送请求
    CURLcode curlCode = curl_easy_perform(curl);
    // 处理请求状态
    if (curlCode != CURLE_OK) {
        string errMsg(curl_easy_strerror(curlCode));
        this->errorMsg = errMsg;
        curl_easy_cleanup(curl);
        // 返回请求失败
        return false;
    }
    else
    {
        // 响应状态码
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &this->responseCode);
    }
    curl_easy_cleanup(curl);
    // 返回请求成功
    return true;
}


/**
 * @brief 设置调试开关
 * @param flag 调试标识
 * @return string 错误信息
 */
void CHttpClient::SetDebug(bool flag)
{
    isDebug = flag;
}


/**
 * @brief 获取cURL请求错误提示
 * @return string 错误信息
 */
std::string CHttpClient::GetErrMsg()
{
    return this->errorMsg;
}


/**
 * @brief 获取http响应状态码
 * @return int http状态码
 */
long CHttpClient::GetResponseCode()
{
    return this->responseCode;
}

