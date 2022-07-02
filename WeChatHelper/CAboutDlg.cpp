// CAboutDlg.cpp: 实现文件
#include "stdafx.h"
#include "CAboutDlg.h"
#include "resource.h"
#include <string>
#include "PublicFunction.h"


using std::string;


// CAboutDlg 对话框

IMPLEMENT_DYNAMIC(CAboutDlg, CDialogEx)

CAboutDlg::CAboutDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_ABOUT, pParent)
{

}


CAboutDlg::~CAboutDlg()
{
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAboutDlg 对话框初始化
BOOL CAboutDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置提示
    SetVersionTips();

    return TRUE;  // return TRUE unless you set the focus to a control
}



//************************************************************
// 函数名称: SetVersionTips
// 函数说明: 设置提示
// 作    者: Greatfar
// 时    间: 2022/07/02
// 参    数: void
// 返 回 值: void
//***********************************************************
void CAboutDlg::SetVersionTips()
{
    // 设置app版本
    string appVersion = GetValueFromeConfig(_T("./config.ini"), _T("base"), _T("app_version"), _T(""));
    CString aboutText = _T("微信助手 ");
    aboutText += string_to_cstring(appVersion);
    GetDlgItem(IDC_ABOUT_VERSION)->SetWindowText(aboutText);  // 向 static text 控件设置文本
    // 设置目标微信版本
    string wxVersion = GetValueFromeConfig(_T("./config.ini"), _T("base"), _T("target_wechat_version"), _T(""));
    CString targetWechatText = _T("适用于 微信 for windows ");
    targetWechatText += string_to_cstring(wxVersion);
    GetDlgItem(IDC_TARGET_WECHAT_VERSION)->SetWindowText(targetWechatText);  // 向 static text 控件设置文本
    // 设置版权信息
    string author = GetValueFromeConfig(_T("./config.ini"), _T("base"), _T("author"), _T(""));
    CString copyRightText = string_to_cstring(author);
    copyRightText += _T(" All Rights Reserved.");
    GetDlgItem(IDC_ABOUT_COPY_RIGHT)->SetWindowText(copyRightText);  // 向 static text 控件设置文本
}

