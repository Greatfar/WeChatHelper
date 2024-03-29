﻿#ifndef __HTTP_CURL_H__
#define __HTTP_CURL_H__

class CHttpClient
{
public:
    CHttpClient(void);
    ~CHttpClient(void);

public:
    bool Request(const std::string& strUrl, std::string& strResponse, const std::string& resBody = "");  // 发送请求
    bool caRequest(const std::string& strUrl, std::string& strResponse, const std::string& resBody = "", const char* pCaPath = NULL);  // 发送请求（带ca证书校验）
    int downloadFile(const char* url, const char outfilename[FILENAME_MAX]); // 下载文件

public:
    void SetDebug(bool flag);     // 设置调试开关
    std::string GetErrMsg();      // 获取cURL请求错误提示
    long GetResponseCode();       // 获取响应状态码
    
private:
    bool isDebug;                   // 调试标识
    std::string errorMsg = "";      // cURL请求错误提示
    long responseCode = 0;          // 响应状态码
};

#endif
