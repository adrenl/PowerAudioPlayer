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
    CMFCPropertyGridCtrl m_pgctrl;
    void MFCPropertyGridPropertyMakeTrueOrFalse(CMFCPropertyGridProperty *PropertyGridProperty);
    LPCWSTR PropertyGridPropertyOutputTrueOrFalse(bool val);
    bool  TXTTrueOrFalseToBool(CString val);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    CButton m_can;
    CButton m_ok;
    afx_msg void OnBnClickedOkbtn();
    afx_msg void OnBnClickedCancelbtn();
    afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
    int OldDSPId;
};
