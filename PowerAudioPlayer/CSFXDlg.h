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
	HSFX hsfx;
	CRect rect;
	CDC* m_pVisDC;
	DECLARE_MESSAGE_MAP()
		virtual void PostNcDestroy();
	virtual void OnCancel();
public:
	CStatic m_vis_panel;
	CComboBox m_sfx_select;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeSfxSelect();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
