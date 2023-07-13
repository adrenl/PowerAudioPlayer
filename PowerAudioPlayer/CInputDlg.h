#pragma once
#include "afxdialogex.h"


// CInputDlg 对话框

class CInputDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CInputDlg)

public:
    CInputDlg(CWnd *pParent = nullptr);   // 标准构造函数
    virtual ~CInputDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_INPUTDLG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    static BOOL InputBox(CString Describe, CString Default, CString IfIsNull, CString &rString);
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CEdit m_describe;
    CEdit m_input;
    static CString IfIsNull, Describe, Default;
    static bool ResB;
    static CString ResS;
};
