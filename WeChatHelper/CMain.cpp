#include "stdafx.h"
#include "WeChatHelper.h"
#include "CMain.h"
#include "CInjectTools.h"
#include <string>
#include "CAboutDlg.h"
#include "CHttpClient.h"
#include "PublicFunction.h"


IMPLEMENT_DYNAMIC(CMain, CDialogEx)


// 全局变量，某些消息处理函数的执行上下文不在类中，读取不到类属性
HANDLE wxPid = NULL;      // 微信的进程id


CMain::CMain(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MAIN, pParent)
{

}


CMain::~CMain()
{
}


void CMain::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMain, CDialogEx)
    ON_COMMAND(ID_32776, &CMain::OnWxLogout)
    ON_WM_COPYDATA()
    ON_MESSAGE(WM_SHOWTASK, OnSoftTask)
    ON_COMMAND(ID_32818, &CMain::OnAppExit)
    ON_COMMAND(ID_32819, &CMain::OnAppExit)
    ON_COMMAND(ID_32778, &CMain::OnAbout)
    ON_COMMAND(ID_32825, &CMain::OnStartWeChat)
    ON_COMMAND(ID_32824, &CMain::OnSumFriendRequest)
    ON_COMMAND(ID_32826, &CMain::OnOpenLogFolder)
    ON_COMMAND(ID_32827, &CMain::OnOpenRuntimeLog)
    ON_COMMAND(ID_32828, &CMain::OnFriendRequestLog)
    ON_COMMAND(ID_32829, &CMain::OnOpenHelper)
    ON_WM_CLOSE()
    ON_WM_TIMER()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMain 对话框初始化
BOOL CMain::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 获取基础url
    GetBaseUrl();

    // 确保日志目录存在
    CreatePathIfNotExist(_T("./log"));

    // 启动并把dll注入微信
    Inject2WeChat();

    // 添加系统托盘图标
    RunInBackground();

    return TRUE;  // return TRUE unless you set the focus to a control
}



//************************************************************
// 函数名称: OnCopyData
// 函数说明: 响应CopyData消息
// 作    者: Greatfar
// 时    间: 2022/03/22
// 参    数: CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct
// 返 回 值: BOOL
//***********************************************************
BOOL CMain::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
    // 微信登录成功 消息
    if (pCopyDataStruct->dwData == WM_Login)
    {
        string logContent = "微信登录成功，微信助手正在上报登录状态";
        WriteLog(logContent.c_str());

        // 向被控端发送 获取个人信息 消息
        CWnd* pWnd = CWnd::FindWindow(NULL, L"WeChatTools");
        if (pWnd == NULL)
        {
            ShowTaskInfo(_T("正在获取微信个人信息，但无法查找到被控端窗口"));
            logContent = "正在获取微信个人信息，但无法查找到被控端窗口";
            WriteLog(logContent.c_str());
        }
        else {
            COPYDATASTRUCT GetInformation;
            GetInformation.dwData = WM_GetInformation;
            GetInformation.cbData = 0;
            GetInformation.lpData = NULL;
            pWnd->SendMessage(WM_COPYDATA, NULL, (LPARAM)&GetInformation);
        }
    }
    // 微信已经登录 消息
    else if (pCopyDataStruct->dwData == WM_AlreadyLogin)
    {
        showLog(_T("您已登录微信，请退出微信，再运行微信助手"));
        string logContent = "您已登录微信，请退出微信，再运行微信助手";
        WriteLog(logContent.c_str());
        MessageBoxA(NULL, "您已登录微信，请退出微信，再运行微信助手", "tips", 0);
        // 重新启动微信
        OnStartWeChat();
    }
    // 聊天记录 消息
    else if (pCopyDataStruct->dwData == WM_ShowChatRecord)
    {
        ChatMessageData* msg = (ChatMessageData*)pCopyDataStruct->lpData;
        HandleFriendRequest((WPARAM)msg);
    }
    // 个人信息 消息
    else if (pCopyDataStruct->dwData == WM_GetInformation)
    {
        isLoginWechat = TRUE;
        // 保存个人信息
        PersonalInformation* personInfo = (PersonalInformation*)pCopyDataStruct->lpData;
        userInfo.wxid = wchar_t_to_string(personInfo->wxid);            // 微信ID
        userInfo.wxcount = wchar_t_to_string(personInfo->wxcount);        // 微信账号
        if (userInfo.wxcount.length() <= 4) userInfo.wxcount = userInfo.wxid;   // 如果没有微信号就使用微信id
        userInfo.nickname = wchar_t_to_string(personInfo->nickname);        // 微信昵称
        userInfo.phonenumber = wchar_t_to_string(personInfo->phonenumber);    // 手机号

        // 调用HTTP接口，发送客服微信上线
        string postParam = "o_wx_id=" + userInfo.wxid + "&wx_id="
            + userInfo.wxcount + "&status=1&source=pc_wechat&device_number=undefined&wx_ic="
            + UrlEncode(unicode_to_utf8(userInfo.nickname));
        HttpRequest("fans_user_status", postParam);

        string logResult = "微信上线：" + userInfo.nickname + " -- " + userInfo.wxcount + " -- " + userInfo.wxid;
        CString csLog = CString(logResult.c_str());
        showLog(csLog);
    }
    // 显示窗口 消息
    else if (pCopyDataStruct->dwData == WM_DesktopShowWindow)
    {
        OnOpenHelper();
        ShowTaskInfo(_T("微信助手正在后台运行"));
        string logContent = "微信助手正在运行，用户双击了桌面图标";
        WriteLog(logContent.c_str());
    }
    // 微信版本不匹配 消息
    else if (pCopyDataStruct->dwData == WM_WechatVersionError)
    {
        string logContent = "微信版本不匹配，助手进程退出";
        WriteLog(logContent.c_str());
        // 退出当前进程
        ExitProcess(-1);
    }

    return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}



//************************************************************
// 函数名称: showLog
// 函数说明: 显示日志
// 作    者: Greatfar
// 时    间: 2022/03/23
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::showLog(CString log)
{
    // 格式化时间
    time_t t = time(0);
    char tflag[64];
    strftime(tflag, sizeof(tflag), "%Y-%m-%d  %H:%M:%S", localtime(&t));
    // 获得EDIT控件
    CEdit* pLogWin = (CEdit*)GetDlgItem(IDC_EDIT_LOG);
    // 取旧日志
    CString oldLog;
    pLogWin->GetWindowText(oldLog);
    // 组装新日志
    CString newLog = oldLog + tflag + "  --  " + log  + "\r\n";
    // 重新显示日志
    pLogWin->SetWindowText(newLog);
}



//************************************************************
// 函数名称: GetBaseUrl
// 函数说明: 获取基础URL
// 作    者: Greatfar
// 时    间: 2022/03/23
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::GetBaseUrl()
{
    this->baseUrl = GetValueFromeConfig(_T("./config.ini"), _T("api"), _T("baseurl"), _T(""));
}



//************************************************************
// 函数名称: OnAbout
// 函数说明: 响应关于菜单
// 作    者: Greatfar
// 时    间: 2022/02/23
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnAbout()
{
    CAboutDlg about;
    about.DoModal();
}



//************************************************************
// 函数名称: OnStartWeChat
// 函数说明: 响应启动菜单
// 作    者: Greatfar
// 时    间: 2022/02/23
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnStartWeChat()
{
    string logContent = "";
    // 查找被控端消息窗口
    CWnd* pToolsWnd = CWnd::FindWindow(NULL, L"WeChatTools");
    if (pToolsWnd == NULL)
    {
        // 终止微信进程
        DWORD dwPid = ProcessNameFindPID("WeChat.exe");
        if (dwPid != 0)
        {
            HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
            ::TerminateProcess(hProcess, 0);
        }
        // 重启微信
        this->isInjectWechat = false;
        this->isLoginWechat = FALSE;
        this->isSendWeChatNotFound = false;
        this->Inject2WeChat();
    }
    else
    {
        showLog(_T("微信正在运行，不能重复启动"));
        logContent = "微信正在运行，不能重复启动";
        WriteLog(logContent.c_str());
    }
}



//************************************************************
// 函数名称: Inject2WeChat
// 函数说明: 启动并把dll注入微信
// 作    者: Greatfar
// 时    间: 2022/02/23
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::Inject2WeChat()
{
    // 对微信进行远程线程注入
    if (!isInjectWechat)
    {
        // dll注入失败
        if (InjectDll(wxPid) == FALSE)
        {
            ShowTaskInfo(_T("微信助手初始化失败"));
            showLog(_T("微信助手初始化失败"));
            string logContent = "微信助手初始化失败";
            WriteLog(logContent.c_str());
        }
        // 注入成功
        else
        {
            // 打开已注入标识
            isInjectWechat = true;
            // 提示信息
            showLog(_T("欢迎使用微信助手，助手初始化成功"));
            string logContent = "欢迎使用微信助手，助手初始化成功";
            WriteLog(logContent.c_str());
            // 设置定时器，检查微信的健康状态
            SetTimer(TIMER_HEALTH_CHECK, 60000, 0);
        }
    }
    // 重复注入
    else
    {
        ShowTaskInfo(_T("微信助手被重复初始化"));
        showLog(_T("微信助手被重复初始化"));
        string logContent = "微信助手被重复初始化";
        WriteLog(logContent.c_str());
    }
}



//************************************************************
// 函数名称: OnOpenLogFolder
// 函数说明: 响应日志目录菜单
// 作    者: Greatfar
// 时    间: 2022/02/23
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnOpenLogFolder()
{
    // 获取当前程序目录
    CString currentPath =  GetCurrentPath();
    currentPath += _T("log\\");
    // 调用资源管理器打开目录
    ShellExecute(NULL, _T("explore"), currentPath, NULL, NULL, SW_SHOW);
}



//************************************************************
// 函数名称: OnOpenRuntimeLog
// 函数说明: 响应运行日志菜单
// 作    者: Greatfar
// 时    间: 2022/02/23
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnOpenRuntimeLog()
{
    // 获取当前程序目录
    CString currentPath = GetCurrentPath();
    // 组装日志文件全路径
    time_t timestamp = time(0);
    char tflag[64];
    strftime(tflag, sizeof(tflag), "%Y%m%d", localtime(&timestamp));
    std::string stflag = tflag;
    CString logFilePrefix = CString(stflag.c_str());
    currentPath = currentPath + _T("log\\") + logFilePrefix + _T("-runtime.log");
    // 调用记事本打开日志
    ShellExecute(NULL, _T("open"), currentPath, NULL, NULL, SW_SHOW);
}



//************************************************************
// 函数名称: OnFriendRequestLog
// 函数说明: 响应加粉日志菜单
// 作    者: Greatfar
// 时    间: 2022/02/23
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnFriendRequestLog()
{
    // 获取当前程序目录
    CString currentPath = GetCurrentPath();
    // 组装日志文件全路径
    time_t timestamp = time(0);
    char tflag[64];
    strftime(tflag, sizeof(tflag), "%Y%m%d", localtime(&timestamp));
    std::string stflag = tflag;
    CString logFilePrefix = CString(stflag.c_str());
    currentPath = currentPath + _T("log\\") + logFilePrefix + _T("-friend-request.log");
    // 调用记事本打开日志
    ShellExecute(NULL, _T("open"), currentPath, NULL, NULL, SW_SHOW);
}



//************************************************************
// 函数名称: GetCurrentPath
// 函数说明: 获取当前程序运行目录
// 作    者: Greatfar
// 时    间: 2022/02/23
// 参    数: void
// 返 回 值: void
//***********************************************************
CString CMain::GetCurrentPath()
{
    CString  strProgramPath;
    GetModuleFileName(NULL, strProgramPath.GetBuffer(MAX_PATH), MAX_PATH);
    strProgramPath.ReleaseBuffer(MAX_PATH);
    int nPathPos = strProgramPath.ReverseFind('\\');
    strProgramPath = strProgramPath.Left(nPathPos + 1);
    return strProgramPath;
}



//************************************************************
// 函数名称: OnOpenHelper
// 函数说明: 响应托盘图标：打开助手
// 作    者: Greatfar
// 时    间: 2022/04/16
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnOpenHelper()
{
    ModifyStyleEx(0, WS_EX_TOPMOST); // 窗口置顶
    ShowWindow(SW_SHOWNORMAL); // 显示窗口
}



//************************************************************
// 函数名称: OnSumFriendRequest
// 函数说明: 响应加粉统计菜单
// 作    者: Greatfar
// 时    间: 2019/03/23
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnSumFriendRequest()
{
    if (isLoginWechat) {
        string logResult = "当前微信号今天收到" + to_string(friendRequestCounter) + "个好友请求";
        CString csLog = CString(logResult.c_str());
        showLog(csLog);
        string logContent = "点击加粉统计菜单：" + logResult;
        WriteLog(logContent.c_str());
    }
    else {
        MessageBoxA(NULL, "请登录微信后统计加粉数量", "tips", 0);
    }
}


//************************************************************
// 函数名称: OnWxLogout
// 函数说明: 响应退出微信菜单
// 作    者: Greatfar
// 时    间: 2022/03/30
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnWxLogout()
{
    if (isLoginWechat) 
    {
        string logContent = "微信下线，微信助手正在上报登录状态";
        WriteLog(logContent.c_str());

        // 向被控端推送 微信退出 消息
        CWnd *pWnd = CWnd::FindWindow(NULL, L"WeChatTools");
        if (pWnd == NULL)
        {
            logContent = "正在退出微信，但无法查找到被控端窗口";
            WriteLog(logContent.c_str());
        }
        else {
            COPYDATASTRUCT logout;
            logout.dwData = WM_Logout;
            logout.cbData = 0;
            logout.lpData = NULL;
            pWnd->SendMessage(WM_COPYDATA, NULL, (LPARAM)&logout);
            isLoginWechat = FALSE;
            isInjectWechat = false;
        }
        
        // 调用HTTP接口，发送客服微信下线
        string postParam = "o_wx_id=" + userInfo.wxid + "&wx_id="
            + userInfo.wxcount + "&status=0&source=pc_wechat&device_number=undefined&wx_ic="
            + UrlEncode(unicode_to_utf8(userInfo.nickname));
        HttpRequest("fans_user_status", postParam);

        showLog(_T("微信下线，微信助手已上报登录状态"));
        logContent = "微信下线，微信助手已上报登录状态";
        WriteLog(logContent.c_str());
    }
    else {
        string logContent = "退出微信，未登录微信";
        WriteLog(logContent.c_str());
    }
}



//************************************************************
// 函数名称: OnAppExit
// 函数说明: 响应托盘菜单：退出微信助手
// 作    者: Greatfar
// 时    间: 2022/03/20
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnAppExit()
{
    if (MessageBoxA(NULL, "您要退出微信助手吗？退出后停止统计微信加粉数量", "tips", MB_YESNO) == IDYES)
    {
        // 退出微信
        OnWxLogout();
        // 终止微信进程
        DWORD dwPid = ProcessNameFindPID("WeChat.exe");
        if (dwPid != 0)
        {
            HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
            ::TerminateProcess(hProcess, 0);
        }
        // 销毁对话框
        EndDialog(0);
    }
}



//************************************************************
// 函数名称: OnClose
// 函数说明: 重载OnClose消息
// 作    者: Greatfar
// 时    间: 2022/03/26
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnClose()
{
    ShowTaskInfo(_T("助手已进入后台运行"));
    ShowWindow(SW_HIDE); // 隐藏主窗口
}



//************************************************************
// 函数名称: OnTimer
// 函数说明: 定时器消息处理器
// 作    者: Greatfar
// 时    间: 2022/03/26
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::OnTimer(UINT nIDEvent)
{
    if (!isLoginWechat) return;
    CWnd* pWnd = CWnd::FindWindow(NULL, L"WeChatTools");
    if (pWnd == NULL)
    {
        showLog(_T("无法与微信建立联系，请使用菜单（操作--启动微信）重新登录微信"));
        ShowTaskInfo(_T("无法与微信建立联系，请使用菜单（操作--启动微信）重新登录微信"));
        string logContent = "无法与微信建立联系，请使用菜单（操作--启动微信）重新登录微信";
        WriteLog(logContent.c_str());
        // 通知服务器1次，客服已下线
        if (!isSendWeChatNotFound) {
            isSendWeChatNotFound = true;
            // 调用HTTP接口，发送客服微信下线
            string postParam = "o_wx_id=" + userInfo.wxid + "&wx_id="
                + userInfo.wxcount + "&status=0&source=pc_wechat&device_number=wechat-client-not-found&wx_ic="
                + UrlEncode(unicode_to_utf8(userInfo.nickname));
            HttpRequest("fans_user_status", postParam);
            // 弹出重新启动微信对话框
            if (MessageBoxA(NULL, "微信似乎闪退了，是否重新登录微信", "tips", MB_SYSTEMMODAL | MB_YESNO) == IDYES)
            {
                OnStartWeChat();
            }
        }
    }
}



//************************************************************
// 函数名称: OnSoftTask
// 函数说明: 软件托盘菜单事件响应函数
// 作    者: Greatfar
// 时    间: 2019/03/24
// 参    数: void
// 返 回 值: void
//***********************************************************
LRESULT CMain::OnSoftTask(WPARAM wParam, LPARAM lParam)
{
    if (wParam != IDR_MAINFRAME)
        return 1;
    switch (lParam)
    {
    case WM_RBUTTONUP: // 右键起来时的处理
    {
        // 右击弹出托盘菜单
        CMenu menu;
        menu.LoadMenu(IDR_MENU_TASK); // 绑定菜单资源
        CMenu* pPopUp = menu.GetSubMenu(0);
        CPoint pt;
        GetCursorPos(&pt);
        SetForegroundWindow();
        pPopUp->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
        PostMessage(WM_NULL, 0, 0);
    }
    break;
    case WM_LBUTTONDBLCLK: // 双击左键的处理
    {
        OnOpenHelper();
    }
    case  WM_LBUTTONDOWN: // 左键单击的处理     
    {
        OnOpenHelper();
    }
    break;
    }
    return 0;
}



//************************************************************
// 函数名称: RunInBackground
// 函数说明: 添加系统托盘图标
// 作    者: Greatfar
// 时    间: 2022/03/20
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::RunInBackground()
{
    notifData.cbSize = sizeof(NOTIFYICONDATA);     // 结构大小
    notifData.hWnd = this->m_hWnd;                 // 接收托盘消息的窗口句柄
    notifData.uID = IDR_MAINFRAME;                 // 应用程序中定义的托盘图标ID
    notifData.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)); // 托盘图标句柄
    wcscpy(notifData.szTip, _T("微信助手 by Greatfar"));
    notifData.uCallbackMessage = WM_SHOWTASK;                  // 自定义消息,接收图标发送的消息
    notifData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;       // 设置属性
    Shell_NotifyIcon(NIM_ADD, &notifData);                     // 向任务栏的状态栏发送添加托盘图标的消息
}



//************************************************************
// 函数名称: ShowTaskInfo
// 函数说明: 系统托盘显示气泡提示
// 作    者: Greatfar
// 时    间: 2022/03/20
// 参    数: void
// 返 回 值: void
//***********************************************************
void CMain::ShowTaskInfo(LPCTSTR tips)
{
    notifData.cbSize = sizeof(NOTIFYICONDATA);
    notifData.hWnd = this->m_hWnd;
    notifData.uFlags = NIF_INFO;
    notifData.uID = IDR_MAINFRAME;
    notifData.dwInfoFlags = 1;
    notifData.uTimeout = 3000;
    wcscpy(notifData.szInfoTitle, _T("tips"));
    wcscpy(notifData.szInfo, tips);
    Shell_NotifyIcon(NIM_MODIFY, &notifData);
}



//************************************************************
// 函数名称: HttpRequest
// 函数说明: 发送HTTP请求
// 作    者: Greatfar
// 时    间: 2022/03/20
// 参    数: url 请求地址
// 参    数: postData 请求体(不传为GET请求，传入非空字符串为POST请求)
// 返 回 值: string 响应体
//***********************************************************
std::string CMain::HttpRequest(string url, string postData)
{
    // 默认提交的参数
    if (postData == "")
    {
        string joiner = url.find("?") == string::npos ? "?" : "&";
        url += joiner + "version=" + (string)APP_VERSION;
    }
    else
    {
        postData += "&version=" + (string)APP_VERSION;
    }
    // 发送HTTP请求
    CHttpClient httpClient;
    string response = "";  // 响应体
    url = this->baseUrl + url;
    bool status = httpClient.Request(url, response, postData);
    // 请求失败
    if (!status) {
        response = "网络请求失败：" + httpClient.GetErrMsg();
        CString csInfo = CString(response.c_str());
        showLog(csInfo);
        ShowTaskInfo(csInfo);
    }
    else {
        // 转码解决中文乱码问题 （本项目为Unicode工程（utf-16），网络请求返回的是utf-8编码）
        wstring respondW = utf8_to_unicode(response);
        // 转换成string
        response = wchar_t_to_string((wchar_t*)respondW.c_str());
    }
    // 记录日志
    string logContent = "发送网络请求：\r\n请求地址： " + url + "\r\n请求参数： " + postData + "\r\n请求结果： " + response;
    WriteLog(logContent.c_str());
    // 返回数据
    return response;
}



//************************************************************
// 函数名称: HandleFriendRequest
// 函数说明: 处理加好友请求
// 作    者: Greatfar
// 时    间: 2022/02/22
// 参    数: WPARAM wParam
// 返 回 值: BOOL
//***********************************************************
bool CMain::HandleFriendRequest(WPARAM wParam)
{
    // 取消息数据
    ChatMessageData* msg = (ChatMessageData*)wParam;
    // 消息内容
    std::string content = wchar_t_to_string(msg->content);
    // 如果不是好友请求，直接返回
    if (content.find("msg fromusername") == string::npos || content.find("country") == string::npos) {
        return false;
    }
    // 记录日志
    WriteLog(content.c_str(), "friend-request.log");
    // 提示信息
    ShowTaskInfo(_T("微信收到好友请求，请打开微信查看"));
    string logContent = "微信收到好友请求，助手正在统计数据";
    WriteLog(logContent.c_str());
    // 加粉统计
    friendRequestCounter++;
    // 读取好友请求信息
    string friendWxid = GetValueFromXml(content, "fromusername", "encryptusername");      // 微信id
    string friendWxac = GetValueFromXml(content, "alias", "weibo");                       // 微信号
    string friendNickname = GetValueFromXml(content, "fromnickname", "content");          // 微信昵称
    string friendRemark = GetValueFromXml(content, "content", "fullpy");                  // 添加备注
    string friendScene = GetValueFromXml(content, "scene", "country");                    // 添加场景值
    string bigHeadImgUrl = GetValueFromXml(content, "bigheadimgurl", "smallheadimgurl");  // 大头像
    string smallHeadImgUrl = GetValueFromXml(content, "smallheadimgurl", "ticket");       // 小头像
    // 如果没有微信号就用微信id
    if (friendWxac.length() <= 0) friendWxac = friendWxid;
    // 调用HTTP接口，发送加粉计数
    string postParam = "to_wx_id=" + friendWxac + "&from_wx_id=" + userInfo.wxcount
        + "&scene=" + friendScene + "&nickname=" + UrlEncode(unicode_to_utf8(friendNickname))
        + "&big_avatar=" + UrlEncode(bigHeadImgUrl)
        + "&small_avatar=" + UrlEncode(smallHeadImgUrl)
        + "&createtime=" + UrlEncode(FormatDateTime("%Y-%m-%d %H:%M:%S"));
    HttpRequest("fans_user", postParam);
    // 转换添加好友的场景值
    if (friendScene == "30") {
        friendScene = "扫二维码添加";
    }
    else if (friendScene == "15") {
        friendScene = "搜索手机号添加";
    }
    else if (friendScene == "14") {
        friendScene = "通过微信群添加";
    }
    else if (friendScene == "17") {
        friendScene = "通过名片添加";
    }
    else if (friendScene == "6") {
        friendScene = "通过非好友提示添加";
    }
    else if (friendScene == "3") {
        friendScene = "搜索微信号添加";
    }
    // 显示窗口日志
    string logResult = "第 " + to_string(friendRequestCounter) + " 个好友请求 -- "
        + friendScene + " -- " + friendWxac + " -- " + friendWxid + " -- "
        + friendNickname + " -- " + friendRemark;
    CString csLog = CString(logResult.c_str());
    showLog(csLog);
    // 返回
    return true;
}

