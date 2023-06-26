
// PowerAudioPlayerDlg.h: 头文件
//

#pragma once
#include "CGlobal.h"

// CPowerAudioPlayerDlg 对话框
class CPowerAudioPlayerDlg : public CDialogEx
{
// 构造
public:
	CPowerAudioPlayerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POWERAUDIOPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString LoadStr;
	CStatic m_infosta;
	CStatic m_ttimesta;
	CStatic m_ntimesta;
	CStatic m_volsta;
	CSliderCtrl m_timeside;
	CSliderCtrl m_volside;
	CButton m_volchk;
	CButton m_playbtn;
	CButton m_stopbtn;
	CButton m_backbtn;
	CButton m_nextbtn;	
	CListCtrl m_playlist;
	void ChangeVolumeSide();
	void Play(int Id);
	void AddToList(CString Path, CString Title = _T(""), int Time = 0, bool IsConvert = FALSE);
	void SaveList(CString Path = _T(""));
	void LoadList(CString Path = _T(""));
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnClose();
	afx_msg void On32776();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void On32783();
	afx_msg void On32782();
};
