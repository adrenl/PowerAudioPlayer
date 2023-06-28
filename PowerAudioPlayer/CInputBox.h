#pragma once
#include "afxdialogex.h"


// CInputBox 对话框

class CInputBox : public CDialogEx
{
	DECLARE_DYNAMIC(CInputBox)

public:
	CInputBox(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInputBox();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_descp;
	CEdit m_input;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
