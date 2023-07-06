// CSFXDlg.cpp: 实现文件
//

#include "pch.h"
#include "PowerAudioPlayer.h"
#include "afxdialogex.h"
#include "CSFXDlg.h"


// CSFXDlg 对话框

IMPLEMENT_DYNAMIC(CSFXDlg, CDialogEx)

CSFXDlg::CSFXDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SFXDLG, pParent)
{
	hsfx = 0;
	m_pVisDC = 0;
}

CSFXDlg::~CSFXDlg()
{
}

void CSFXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIS_PANEL, m_vis_panel);
	DDX_Control(pDX, IDC_SFX_SELECT, m_sfx_select);
}


BEGIN_MESSAGE_MAP(CSFXDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_SFX_SELECT, &CSFXDlg::OnCbnSelchangeSfxSelect)
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSFXDlg 消息处理程序

BOOL CSFXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_sfx_select.InsertString(0,_T("空"));
	for(int i=0;i<CPb::SFXs.size();++i){
		m_sfx_select.InsertString(i+1,CPb::GetInPathFileName(CPb::SFXs[i]));
	}
	m_sfx_select.SetCurSel(CPb::SFXId);

	m_vis_panel.GetClientRect(&rect);
	m_pVisDC = m_vis_panel.GetDC();
	BASS_SFX_Init(AfxGetApp()->m_hInstance, m_hWnd);
	return TRUE;
}

void CSFXDlg::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	//delete this;
}


void CSFXDlg::OnCancel()
{
	KillTimer(TIMER_SFX);
	Sleep(20);
	ReleaseDC(m_pVisDC);
	BASS_SFX_Free();
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void CSFXDlg::OnCbnSelchangeSfxSelect()
{
	int nIndex = m_sfx_select.GetCurSel();
	if(nIndex != 0){
		char* file = (char *)CPb::CStrToChar(CPb::SFXs[nIndex - 1]);
		KillTimer(TIMER_SFX);
		Sleep(1);
		CString Title;
		if (hsfx)
			BASS_SFX_PluginFree(hsfx);
		if(hsfx = BASS_SFX_PluginCreate(file, m_vis_panel.m_hWnd, rect.Width(), rect.Height(), 0)){
			BASS_SFX_PluginSetStream(hsfx, BASS::Stream);
			BASS_SFX_PluginStart(hsfx);
			SetTimer(TIMER_SFX, 1, NULL);
			Title.Format(_T("可视化效果 - %s"), CPb::CharToLPCWSTR(BASS_SFX_PluginGetName(hsfx)));
		}else {
			Title.Format(_T("可视化效果 - 加载失败 - 错误码为%d"),BASS_SFX_ErrorGetCode());
		}
		SetWindowText(Title);
	} else {
		if(hsfx){
			SetWindowText(_T("可视化效果"));
			BASS_SFX_PluginFree(hsfx);
			KillTimer(TIMER_SFX);
		}
	}
}


void CSFXDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(hsfx)
	BASS_SFX_PluginRender(hsfx, BASS::Stream, m_pVisDC->GetSafeHdc());
	CDialogEx::OnTimer(nIDEvent);
}


void CSFXDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	m_vis_panel.SetWindowPos(NULL, 0, 0, cx, cy - 20, SWP_NOZORDER | SWP_NOMOVE);
	m_sfx_select.SetWindowPos(NULL, 0, cy-20, cx, 12, SWP_NOZORDER);
	m_vis_panel.GetClientRect(&rect);
	CSFXDlg::OnCbnSelchangeSfxSelect();
}
