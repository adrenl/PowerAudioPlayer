#pragma once
#include "afxdialogex.h"


// CSettingsDlg 对话框

class CSettingsDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CSettingsDlg)

public:
    CSettingsDlg(CWnd *pParent = nullptr);   // 标准构造函数
    virtual ~CSettingsDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SETTINGS };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CString LoadStr;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CMFCPropertyGridCtrl m_pgctrl;
    void MFCPropertyGridPropertyMakeTrueOrFalse(CMFCPropertyGridProperty *PropertyGridProperty);
    LPCWSTR PropertyGridPropertyOutputTrueOrFalse(bool val);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    CButton m_can;
    CButton m_ok;
    afx_msg void OnBnClickedOkbtn();
    afx_msg void OnBnClickedCancelbtn();
};
