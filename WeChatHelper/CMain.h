#pragma once
#include <string>

// CMain 对话框

class CMain : public CDialogEx
{
    DECLARE_DYNAMIC(CMain)

public:
    CMain(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CMain();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MAIN };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    DECLARE_MESSAGE_MAP()

public:
    NOTIFYICONDATA notifData;           // 软件托盘气泡提示数据 结构体
    struct UserInfo userInfo;           // 当前登录微信的用户信息
    int friendRequestCounter = 0;       // 好友请求数量
    bool isInjectWechat = false;        // dll是否已经远程注入微信
    BOOL isLoginWechat = FALSE;         // 是否登录微信
    bool isSendWeChatNotFound = false;  // 微信客户端不正常退出时，是否已发送客服下线
    std::string baseUrl = "";                // API基础URL
    int readInfoCounter = 0;             // 获取个人信息的次数

    virtual BOOL OnInitDialog();

    afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
    afx_msg LRESULT OnSoftTask(WPARAM wParam, LPARAM lParam);
    afx_msg void OnAppExit();
    afx_msg void OnWxLogout();
    afx_msg void OnClose();

    void OnTimer(UINT nIDEvent);
    void showLog(CString log);
    void RunInBackground();
    void ShowTaskInfo(LPCTSTR tips);
    void OnAbout();
    void OnSumFriendRequest();
    void OnStartWeChat();
    void Inject2WeChat();
    void OnOpenLogFolder();
    void OnOpenRuntimeLog();
    void OnFriendRequestLog();
    void OnOpenHelper();
    void GetBaseUrl();
    void SendGetInformation();
    CString GetCurrentPath();
    std::string HttpRequest(std::string uri, std::string postData = "");
    bool HandleFriendRequest(WPARAM wParam);
};

