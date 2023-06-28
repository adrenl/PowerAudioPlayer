
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
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CPowerAudioPlayerDlg::OnNMRClickList1)
	ON_COMMAND(ID_32779, &CPowerAudioPlayerDlg::On32779)
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

void CPowerAudioPlayerDlg::DelToList(int Id)
{
	//Id--;
	CPb::pl_path.erase(CPb::pl_path.begin() + Id);
	CPb::pl_title.erase(CPb::pl_title.begin() + Id);
	CPb::pl_time.erase(CPb::pl_time.begin() + Id);
	CPb::pl_isconvert.erase(CPb::pl_isconvert.begin() + Id);
}

void CPowerAudioPlayerDlg::CleanList()
{

}

void CPowerAudioPlayerDlg::SaveList(CString Path)
{
	if (Path == _T("")) Path = CPb::GetExeModuleDir() + _T("\\default.json");
	Json::Value root;
	Json::FastWriter fw;
	int total = CPb::pl_path.size();
	for (int i = 0; i < total; ++i){
		root[i]["tite"] = Json::Value(CPb::CStrToChar(CPb::pl_title[i]));
		root[i]["path"] = Json::Value(CPb::CStrToChar(CPb::pl_path[i]));
		root[i]["time"] = Json::Value(CPb::pl_time[i]);
		root[i]["isconvert"] = Json::Value(CPb::pl_isconvert[i]);
	}
	CString json = (CString)fw.write(root).c_str();
	CFile file(Path, CFile::modeCreate | CFile::modeWrite);
	file.SeekToEnd();
	wchar_t unicode = 0xFEFF;
	file.Write(&unicode, 2);
	file.Write(json, wcslen(json) * sizeof(wchar_t));
	file.Close();
}

void CPowerAudioPlayerDlg::LoadList(CString Path)
{
	if (Path == _T("")) Path = CPb::GetExeModuleDir() + _T("\\default.json");
	if (PathFileExists(Path) == FALSE) return;
	CStringArray St1;
	CFile file(Path, CFile::modeRead);
	char* pBuf;
	int dwFileLen = file.GetLength();
	pBuf = new char[dwFileLen + 1];
	pBuf[dwFileLen] = 0;
	file.Read(pBuf, dwFileLen);
	file.Close();
	CPb::split(CPb::CharToCStr(pBuf), *"\r\n", St1);
	Json::Reader reader;
	Json::Value root;
	CString temp;
	USES_CONVERSION;
	char* cJson = T2A(St1[0].GetBuffer(0));
	St1[0].ReleaseBuffer();
	if (reader.parse(cJson,root)){
		for (int i = 0; i < root.size();++i) {
			MessageBox(L"Success!!!!!!!!");
			AddToList((CString)root[i]["path"].asCString(), (CString)root[i]["title"].asCString(), root[i]["time"].asInt(), root[i]["isconvert"].asBool());
			m_playlist.InsertItem(i, (CString)root[i]["title"].asCString());
			m_playlist.SetItemText(i, 1, BASS::TimeToString(root[i]["path"].asInt()));
		}
	}

}

void CPowerAudioPlayerDlg::ConvertList()
{
	if (CPb::pl_path.size() == 0) return;
	int total = CPb::pl_path.size();
	for (int i = 0; i < total; i++) {
		//if (CPb::pl_isconvert[i] == TRUE) continue;
		HSTREAM STREAM = BASS_StreamCreateFile(FALSE, CPb::pl_path[i], NULL, NULL, NULL);
		CPb::pl_title[i] = CPb::CharToLPCWSTR((char*)TAGS_Read(STREAM, "%IFV2(%ARTI,%ICAP(%ARTI),无艺术家) - %IFV2(%TITL,%ICAP(%TITL) ,无标题 ) %IFV1(%ALBM,%IUPC(- %ALBM))"));
		CPb::pl_time[i] = BASS_ChannelBytes2Seconds(STREAM, BASS_ChannelGetLength(STREAM,0));
		m_playlist.SetItemText(i, 0, CPb::pl_title[i]);
		m_playlist.SetItemText(i, 1, BASS::TimeToString(CPb::pl_time[i]));
		CPb::pl_isconvert[i] = TRUE;
		BASS_StreamFree(STREAM);
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
	SkinH_Attach();
	SetTimer(TIMER_ALAWAYS, 400, NULL);
	m_playlist.InsertColumn(0, _T("标题"), LVCFMT_LEFT, 210);
	m_playlist.InsertColumn(1, _T("时间"), LVCFMT_LEFT, 50);
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
	SaveList(NULL);
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
	CFileDialog Dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, LoadStr, this);
	CString FilePath;
	DWORD max_file = 60000;
	TCHAR* lsf = new TCHAR[max_file];
	Dlg.m_ofn.nMaxFile = max_file;
	Dlg.m_ofn.lpstrFile = lsf;
	Dlg.m_ofn.lpstrFile[0] = NULL;
	if (Dlg.DoModal() == IDOK) {
		POSITION posFile = Dlg.GetStartPosition();
		while (posFile != NULL)
		{
			FilePath = Dlg.GetNextPathName(posFile);
			AddToList(FilePath);
			int index = m_playlist.GetItemCount();
			m_playlist.InsertItem(index, FilePath);
			m_playlist.SetItemText(index, 1, _T("00:00"));
		}
		delete lsf;
		CPb::ToConvertList = TRUE;
	}
}

void CPowerAudioPlayerDlg::On32783()
{
	LoadStr.LoadStringW(IDS_FILTER_PAPPL);
	CFileDialog Dlg(TRUE, NULL, NULL, 0, LoadStr, this);
	CString FilePath;
	if (Dlg.DoModal() == IDOK) {
		FilePath = Dlg.GetPathName();
		LoadList(FilePath);
	}
}


void CPowerAudioPlayerDlg::On32782()
{
	LoadStr.LoadStringW(IDS_FILTER_PAPPL);
	CFileDialog Dlg(FALSE, NULL, NULL, 0, LoadStr, this);
	CString FilePath;
	if (Dlg.DoModal() == IDOK) {
		FilePath = Dlg.GetPathName();
		SaveList(FilePath);
	}
}

void CPowerAudioPlayerDlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (m_playlist.GetSelectionMark() != -1) {
		CMenu menu;
		CMenu* pSubMenu = NULL;
		menu.LoadMenu(IDR_PLRMENU);
		CPoint posMouse;
		GetCursorPos(&posMouse);
		pSubMenu = menu.GetSubMenu(0);
		pSubMenu->TrackPopupMenu(0, posMouse.x, posMouse.y, this);
	}
	*pResult = 0;
}


void CPowerAudioPlayerDlg::On32779()
{
	for (int iItem = m_playlist.GetItemCount() - 1; iItem >= 0; iItem--)
	{
		if (m_playlist.GetItemState(iItem, LVIS_SELECTED) == LVIS_SELECTED){
			m_playlist.DeleteItem(iItem);
			DelToList(iItem);
		}
	}
}
