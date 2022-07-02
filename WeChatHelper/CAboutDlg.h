#pragma once
#include "stdafx.h"


// CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CAboutDlg)

public:
    CAboutDlg(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CAboutDlg();
    virtual BOOL OnInitDialog();
    void SetVersionTips();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_ABOUT };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
};
