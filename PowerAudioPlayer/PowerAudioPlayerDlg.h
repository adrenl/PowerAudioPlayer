
// PowerAudioPlayerDlg.h: 头文件
//

#pragma once
#include "CSettingsDlg.h"
#include "CSFXDlg.h"

// CPowerAudioPlayerDlg 对话框
class CPowerAudioPlayerDlg : public CDialogEx
{
    // 构造
public:
    CPowerAudioPlayerDlg(CWnd *pParent = nullptr);	// 标准构造函数

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_POWERAUDIOPLAYER_DIALOG };
#endif
protected:
    virtual void DoDataExchange(CDataExchange *pDX);	// DDX/DDV 支持


    // 实现
protected:
    HICON m_hIcon;
    HICON m_hBigIcon;
    HICON m_hSmallIcon;

    HICON m_hPlayIcon;
    HICON m_hPauseIcon;
    HICON m_hStopIcon;
    HICON m_hBackIcon;
    HICON m_hNextIcon;
    HICON m_hVolIcon;
    HICON m_hMuteIcon;
    HICON m_hListIcon;
    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
public:
    CSFXDlg *m_SFXDlg;
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
    Settings Sets;
    void LoadSettings(bool isReload = FALSE);
    void ChangeVolumeSide();
    void ChangePlayMode(int Mode,bool ChangeMenu_ = FALSE);
    void Play(int Id);
    void AddToList(CString Path, CString Title = _T(""), int Time = 0, bool IsConvert = FALSE);
    void DelToList(int Id);
    void CleanList();
    void SaveList(CString Path = _T(""));
    void LoadList(CString Path = _T(""));
    void ConvertList(bool ReConvert = FALSE);
    void LoadPlugins();
    void BuildSFXList();
    afx_msg void OnBnClickedButton5();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnClose();
    afx_msg void On32776();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void On32783();
    afx_msg void On32782();
    afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void On32779();
    afx_msg void On32784();
    afx_msg void On32794();
    afx_msg void On32777();
    afx_msg void OnMenu32786();
    afx_msg void OnMenu32795();
    afx_msg void On32797();
    afx_msg void On32798();
    CButton m_listbtn;
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    HRESULT CreateThumbnailToolbar(HWND hWnd);
    HRESULT UpdateThumbarButton(HWND hwnd, bool isPause);
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    afx_msg void On32800();
    afx_msg void On32801();
    afx_msg void On32802();
    afx_msg void On32803();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
};
