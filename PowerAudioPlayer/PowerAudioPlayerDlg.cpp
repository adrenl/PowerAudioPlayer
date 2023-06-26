
// PowerAudioPlayerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PowerAudioPlayer.h"
#include "PowerAudioPlayerDlg.h"
#include "afxdialogex.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPowerAudioPlayerDlg 对话框

CPowerAudioPlayerDlg::CPowerAudioPlayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POWERAUDIOPLAYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPowerAudioPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO, m_infosta);
	DDX_Control(pDX, IDC_TTIME, m_ttimesta);
	DDX_Control(pDX, IDC_NTIME, m_ntimesta);
	DDX_Control(pDX, IDC_VOL, m_volsta);
	DDX_Control(pDX, IDC_SLIDER2, m_timeside);
	DDX_Control(pDX, IDC_SLIDER1, m_volside);
	DDX_Control(pDX, IDC_CHECK1, m_volchk);
	DDX_Control(pDX, IDC_BUTTON1, m_playbtn);
	DDX_Control(pDX, IDC_BUTTON2, m_stopbtn);
	DDX_Control(pDX, IDC_BUTTON3, m_backbtn);
	DDX_Control(pDX, IDC_BUTTON4, m_nextbtn);
	DDX_Control(pDX, IDC_LIST1, m_playlist);
}

BEGIN_MESSAGE_MAP(CPowerAudioPlayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_BN_CLICKED(IDC_CHECK1, &CPowerAudioPlayerDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON5, &CPowerAudioPlayerDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &CPowerAudioPlayerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPowerAudioPlayerDlg::OnBnClickedButton2)	
	ON_COMMAND(ID_32776, &CPowerAudioPlayerDlg::On32776)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CPowerAudioPlayerDlg::OnNMDblclkList1)
	ON_COMMAND(ID_32783, &CPowerAudioPlayerDlg::On32783)
	ON_COMMAND(ID_32782, &CPowerAudioPlayerDlg::On32782)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
END_MESSAGE_MAP()

void CPowerAudioPlayerDlg::ChangeVolumeSide() {
	int pos = m_volside.GetPos();
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, pos * 100);
	CString StaticText;
	StaticText.Format(_T("%d%%"), pos);
	m_volsta.SetWindowText(StaticText);
}

void CPowerAudioPlayerDlg::Play(int Id)
{
	BASS::StreamFree();
	if (Id >= 0 && Id < CPb::pl_path.size()) {
		BASS::StreamCreateFile(FALSE, CPb::pl_path[Id], 0, 0, BASS_SAMPLE_FLOAT);
		if (BASS::Stream== 0) {
			Play(Id + 1);
			return;
		}
		int Length = BASS::ChannelGetLength(0);
		BASS::ChannelPlay(FALSE);
		SetTimer(TIMER_PLAYING, 500, NULL);
		CPowerAudioPlayerDlg::ChangeVolumeSide();
		m_timeside.SetRange(0, Length);
		m_timeside.SetPos(0);
		CPb::PlayId = Id;
		m_playbtn.SetWindowTextW(_T("暂停"));
		m_infosta.SetWindowTextW(CPb::pl_title[Id]);
		m_ttimesta.SetWindowTextW(BASS::TimeToString(BASS::ChannelBytes2Seconds(Length)));
		//MessageBox(CPb::CharToLPCWSTR((char*)TAGS_Read(BASS::Stream, "%IFV1(%TRCK,%TRCK. )%IFV2(%ARTI,%ICAP(%ARTI),no artist) - %IFV2(%TITL,%ICAP(%TITL) -,no title -)%IFV1(%ALBM, %IUPC(%ALBM))%IFV1(%YEAR, %(%YEAR%))")));
	}else {
		Play(0);
	}
}

void CPowerAudioPlayerDlg::AddToList(CString Path, CString Title, int Time, bool IsConvert)
{
	if (Title == _T("")) Title = Path;
	CPb::pl_path.push_back(Path);
	CPb::pl_title.push_back(Title);
	CPb::pl_time.push_back(Time);
	CPb::pl_isconvert.push_back(IsConvert);
}

void CPowerAudioPlayerDlg::SaveList(CString Path)
{
	if (Path == _T("")) Path = CPb::GetExeModuleDir() + _T("\\default.pappl");
	CFile file;
	CString Content;
	file.Open(Path, CFile::modeCreate | CFile::modeReadWrite);
	file.SeekToEnd();
	int total = CPb::pl_path.size();
	for (int i = 0; i < total; i++) {
		Content += CPb::pl_title[i] + _T("|") + CPb::pl_path[i] + _T("|") + CPb::i2cs(CPb::pl_time[i]) + _T("|") + CPb::i2cs(CPb::pl_isconvert[i])+_T("\r\n");
	}
	wchar_t unicode = 0xFEFF;
	file.Write(&unicode, 2);
	file.Write(Content, wcslen(Content) * sizeof(wchar_t));
	file.Close();
}

void CPowerAudioPlayerDlg::LoadList(CString Path)
{
	if (Path == _T("")) Path = CPb::GetExeModuleDir() + _T("\\default.pappl");
	if (PathFileExists(Path) == FALSE) return;
	CFile file;
	CStringArray St1, St2;
	char* pBuf;
	file.Open(Path, CFile::modeRead);
	int dwFileLen = file.GetLength();
	pBuf = new char[dwFileLen + 1];
	pBuf[dwFileLen] = 0;
	file.Read(pBuf, dwFileLen);
	file.Close();
	CPb::split(CPb::CharToCStr(pBuf), *"\r\n", St1);
	St1.RemoveAt(St1.GetSize() - 1);
	for (int i = 0; i < St1.GetSize(); ++i) {
		CPb::split(St1[i], *"|", St2);
		AddToList(St2[1], St2[0], _ttoi(St2[2]), _ttoi(St2[3]));
		m_playlist.InsertItem(i, St2[0]);
		m_playlist.SetItemText(i, 1, BASS::TimeToString(_ttoi(St2[2])));
	}
}

void CPowerAudioPlayerDlg::ConvertList()
{
	if (CPb::pl_path.size() == 0) return;
	//ZPlay* file = CreateZPlay();
	//TID3InfoEx id3_info{};
	//LPCWSTR Title = nullptr;
	int total = CPb::pl_path.size();
	for (int i = 0; i < total; i++) {
		//char *location = (char*)CPb::CStrToChar(CPb::pl_path[i]);
		//CPb::pl_time[i]=BASS::LengthFile(CPb::pl_path[i]);
		//m_playlist.SetItemText(i, 1,BASS::TimeToString(CPb::pl_time[i]));
		//file->LoadFileID3Ex(location, sfAutodetect, &id3_info, 1);
		//m_playlist.SetItemText(i, 0, CPb::CharToLPCWSTR(id3_info.Title));
	}
	CPb::ToConvertList = FALSE;
}

// CPowerAudioPlayerDlg 消息处理程序

BOOL CPowerAudioPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
		MessageBox(_T("初始化Bass失败"),NULL, MB_ICONERROR);
		exit(-1);
	}
	//SkinH_Attach();
	SetTimer(TIMER_ALAWAYS, 400, NULL);
	m_playlist.InsertColumn(1, _T("标题"), LVCFMT_LEFT, 210);
	m_playlist.InsertColumn(2, _T("时间"), LVCFMT_LEFT, 50);
	m_playlist.SetExtendedStyle(m_playlist.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//加载Plugins
	CString filepath = CPb::GetExeModuleDir();
	CString filename = _T("");
	CFileFind find;
	BOOL IsFind = find.FindFile(filepath + _T("\\bass_*.dll"));
	while (IsFind)
	{
		IsFind = find.FindNextFile();
		if (find.IsDots()) {
			continue;
		} else {
			filename = filepath+"\\"+find.GetFileName();
			BASS_PluginLoad(CPb::CStrToChar(filename), 0);
		}
	}
	//
	LoadList();
	return TRUE;
}

LRESULT CPowerAudioPlayerDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	if (CPb::ToConvertList == TRUE) {
		ConvertList();
	}
	return 0;
}

void CPowerAudioPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CPowerAudioPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPowerAudioPlayerDlg::OnBnClickedButton5()
{
	CMenu menu;
	CMenu* pSubMenu = NULL;
	menu.LoadMenu(IDR_PLMENU);  
	CPoint posMouse;
	GetCursorPos(&posMouse);
	pSubMenu = menu.GetSubMenu(0);
	pSubMenu->TrackPopupMenu(0, posMouse.x, posMouse.y, this);
}

void CPowerAudioPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ALAWAYS) {

	}else if (nIDEvent == TIMER_PLAYING) {
		int Position = BASS::ChannelGetPosition(0);
		m_timeside.SetPos(Position);
		m_ntimesta.SetWindowTextW(BASS::TimeToString(BASS::ChannelBytes2Seconds(Position)));
		if (m_timeside.GetRangeMax() == m_timeside.GetPos()) {
			//Next One.
		}

	}
	CDialogEx::OnTimer(nIDEvent);
}

void CPowerAudioPlayerDlg::OnBnClickedButton1()
{
	int Active=BASS::ChannelIsActive();
	if (BASS::Stream == 0) {
		Play(0);
		return;
	}
	if (Active == 0) {
		
	}
	if (Active == 1) {
		BASS::ChannelPause();
		m_playbtn.SetWindowTextW(_T("播放"));
		SetTimer(TIMER_PLAYING, 0, NULL);
	} else {
		BASS::ChannelPlay(FALSE);
		m_playbtn.SetWindowTextW(_T("暂停"));
		SetTimer(TIMER_PLAYING, 500, NULL);
	}
}


void CPowerAudioPlayerDlg::OnBnClickedButton2()
{
	BASS::ChannelStop();
	m_playbtn.SetWindowTextW(_T("播放"));
}

void CPowerAudioPlayerDlg::OnBnClickedCheck1()
{
	if (m_volchk.GetCheck()) {
		BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, 0);
		m_volside.EnableWindow(FALSE);
		m_volsta.SetWindowText(_T("静音"));
	}else {
		m_volside.EnableWindow(TRUE);
		CPowerAudioPlayerDlg::ChangeVolumeSide();
	}
}

void CPowerAudioPlayerDlg::OnClose()
{
	BASS_Free();
	SaveList(CPb::GetExeModuleDir() + _T("\\default.pappl"));
	CDialogEx::OnClose();
}

void CPowerAudioPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWnd* pVolumeSider = GetDlgItem(IDC_SLIDER1);
	CWnd* pTimeSider = GetDlgItem(IDC_SLIDER2);
	if(pScrollBar == pVolumeSider){
		CPowerAudioPlayerDlg::ChangeVolumeSide();
	}else if (pScrollBar == pTimeSider) {
		if (BASS::ChannelIsActive() == 0) {
			return;
		}
			BASS::ChannelSetPosition(m_timeside.GetPos(), BASS_POS_BYTE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPowerAudioPlayerDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int iSel=m_playlist.GetSelectionMark();
	if (iSel != -1) {
		Play(iSel);
	}
	*pResult = 0;
}

void CPowerAudioPlayerDlg::On32776()
{
	LoadStr.LoadStringW(IDS_FILTER_AUDIO);
	CFileDialog Dlg(TRUE, NULL, NULL, 0, LoadStr, this);
	CString FilePath;
	if (Dlg.DoModal()) {
		FilePath = Dlg.GetPathName();
		AddToList(FilePath);
		int index = m_playlist.GetItemCount();
		m_playlist.InsertItem(index, FilePath);
		m_playlist.SetItemText(index, 1, _T("00:00"));
		CPb::ToConvertList = TRUE;
	}
}

void CPowerAudioPlayerDlg::On32783()
{
	LoadStr.LoadStringW(IDS_FILTER_PAPPL);
	CFileDialog Dlg(TRUE, NULL, NULL, 0, LoadStr, this);
	CString FilePath;
	if (Dlg.DoModal()) {
		FilePath = Dlg.GetPathName();
		LoadList(FilePath);
	}
}


void CPowerAudioPlayerDlg::On32782()
{
	LoadStr.LoadStringW(IDS_FILTER_PAPPL);
	CFileDialog Dlg(FALSE, NULL, NULL, 0, LoadStr, this);
	CString FilePath;
	if (Dlg.DoModal()) {
		FilePath = Dlg.GetPathName();
		SaveList(FilePath);
	}
}
