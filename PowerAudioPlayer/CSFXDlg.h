#pragma once
#include "afxdialogex.h"


// CSFXDlg 对话框

class CSFXDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSFXDlg)

public:
	CSFXDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSFXDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SFXDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
		virtual void PostNcDestroy();
	virtual void OnCancel();
};
