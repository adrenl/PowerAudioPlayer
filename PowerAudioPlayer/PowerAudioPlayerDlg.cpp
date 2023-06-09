﻿
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

static UINT indicators[] =    //指定状态栏上三个元素的ID
{
    IDS_SBAR_COUNT,
    IDS_SBAR_TOTALTIME
};
// CPowerAudioPlayerDlg 对话框

CPowerAudioPlayerDlg::CPowerAudioPlayerDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_POWERAUDIOPLAYER_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hBigIcon = AfxGetApp()->LoadIcon(IDI_BIG);
    m_hSmallIcon = AfxGetApp()->LoadIcon(IDI_SMALL);

    m_hPauseIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_PAUSE), IMAGE_ICON, 16, 16, NULL);
    m_hPlayIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_PLAY), IMAGE_ICON, 16, 16, NULL);
    m_hBackIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_BACK), IMAGE_ICON, 16, 16, NULL);
    m_hNextIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_NEXT), IMAGE_ICON, 16, 16, NULL);
    m_hStopIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_STOP), IMAGE_ICON, 16, 16, NULL);
    m_hVolIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_VOL), IMAGE_ICON, 16, 16, NULL);
    m_hMuteIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_MUTE), IMAGE_ICON, 16, 16, NULL);
    m_hListIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_LIST), IMAGE_ICON, 16, 16, NULL);

}

void CPowerAudioPlayerDlg::DoDataExchange(CDataExchange *pDX)
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
    DDX_Control(pDX, IDC_BUTTON5, m_listbtn);
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
    ON_COMMAND(ID_32784, &CPowerAudioPlayerDlg::On32784)
    ON_COMMAND(ID_32794, &CPowerAudioPlayerDlg::On32794)
    ON_COMMAND(ID_MENU_32786, &CPowerAudioPlayerDlg::OnMenu32786)
    ON_COMMAND(ID_MENU_32795, &CPowerAudioPlayerDlg::OnMenu32795)
    ON_COMMAND(ID_32797, &CPowerAudioPlayerDlg::On32797)
    ON_COMMAND(ID_32798, &CPowerAudioPlayerDlg::On32798)
    ON_WM_MOUSEWHEEL()
    ON_COMMAND(ID_32800, &CPowerAudioPlayerDlg::On32800)
    ON_COMMAND(ID_32801, &CPowerAudioPlayerDlg::On32801)
    ON_COMMAND(ID_32802, &CPowerAudioPlayerDlg::On32802)
    ON_COMMAND(ID_32803, &CPowerAudioPlayerDlg::On32803)
    ON_BN_CLICKED(IDC_BUTTON3, &CPowerAudioPlayerDlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CPowerAudioPlayerDlg::OnBnClickedButton4)
    ON_COMMAND(ID_MENU_32785, &CPowerAudioPlayerDlg::OnMenu32785)
    ON_COMMAND(ID_32778, &CPowerAudioPlayerDlg::On32778)
    ON_COMMAND(ID_32781, &CPowerAudioPlayerDlg::On32781)
    ON_WM_DROPFILES()
    ON_MESSAGE(WM_MSG_CMD, &CPowerAudioPlayerDlg::OnMsg)
    ON_COMMAND(ID_32805, &CPowerAudioPlayerDlg::On32805)
    ON_COMMAND(ID_MENU_32791, &CPowerAudioPlayerDlg::OnMenu32791)
    ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, &CPowerAudioPlayerDlg::OnLvnKeydownList1)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CPowerAudioPlayerDlg::LoadSettings(bool isReload)
{
    CPb::ReadSettings();
    if (CPb::set.smain_rem_pl_location)
        m_playlist.EnsureVisible(CPb::set.pl_location, TRUE);
    m_volside.SetPos(CPb::set.vol);
    m_volchk.SetCheck(CPb::set.is_mute);
    CPowerAudioPlayerDlg::CPowerAudioPlayerDlg::OnBnClickedCheck1();
    ChangePlayMode(CPb::set.playmode, TRUE);
    BASS::SetMidiSoundFont(CPb::set.smidi_sf_path);
    if (CPb::set.dsp_id != -1 && CPb::set.dsp_id < CPb::DSPs.size())
    {
        if(BASS::Wadsp = BASS_WADSP_Load(CPb::CStrToChar(CPb::DSPs[CPb::set.dsp_id]), NULL, NULL, NULL, NULL, NULL))
        {
            BASS_WADSP_Start(BASS::Wadsp, 0, NULL);
            BASS_WADSP_ChannelSetDSP(BASS::Wadsp, BASS::Stream, NULL);
        }
    }
}

void CPowerAudioPlayerDlg::ChangeVolumeSide()
{
    int pos = m_volside.GetPos();
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, pos * 100);
    CString StaticText;
    StaticText.Format(_T("%d%%"), pos);
    m_volsta.SetWindowText(StaticText);
}

void CPowerAudioPlayerDlg::ChangePlayMode(int Mode, bool ChangeMenu_)
{
    CPb::set.playmode = Mode;
    if (ChangeMenu_)
    {
        CMenu *menu = GetMenu()->GetSubMenu(0);
        switch (Mode)
        {
        case 0:
        {
            menu->CheckMenuRadioItem(ID_32800, ID_32803, ID_32800, MF_BYCOMMAND);
            break;
        }
        case 1:
        {
            menu->CheckMenuRadioItem(ID_32800, ID_32803, ID_32801, MF_BYCOMMAND);
            break;
        }
        case 2:
        {
            menu->CheckMenuRadioItem(ID_32800, ID_32803, ID_32802, MF_BYCOMMAND);
            break;
        }
        case 3:
        {
            menu->CheckMenuRadioItem(ID_32800, ID_32803, ID_32803, MF_BYCOMMAND);
            break;
        }
        }
    }
}

void CPowerAudioPlayerDlg::Play(int Id)
{
    if (CPb::pl_path.size() == 0)
        return;
    BASS::StreamFree();
    if (Id >= 0 && Id < CPb::pl_path.size())
    {
        if (CPb::IsUrl(CPb::pl_path[Id]))
        {
            BASS::StreamCreateURL((char *)CPb::CStrToChar(CPb::pl_path[Id]), 0, BASS_SAMPLE_FLOAT, NULL, NULL);
        }
        else
        {
            BASS::StreamCreateFile(FALSE, CPb::pl_path[Id], 0, 0, BASS_SAMPLE_FLOAT);
        }
        if (BASS::Stream == 0)
        {
            m_playlist.SetItemText(Id, 0, _T("!!错误!!") + CPb::pl_title[Id]);
            if (Id + 1 < CPb::pl_path.size())
            {
                Play(Id + 1);
            }
            else
            {
                RestUI();
            }
            return;
        }
        m_playlist.SetItemText(Id, 0, CPb::pl_title[Id]);
        CPb::length = BASS::ChannelGetLength(0);
        BASS::ChannelPlay(FALSE);
        BASS_WADSP_ChannelSetDSP(BASS::Wadsp, BASS::Stream, NULL);
        CPowerAudioPlayerDlg::CPowerAudioPlayerDlg::OnBnClickedCheck1();
        SetTimer(TIMER_PLAYING, 200, NULL);
        CPowerAudioPlayerDlg::ChangeVolumeSide();
        CPb::PlayId = Id;
        CPb::PlayPath == CPb::pl_path[Id];
        m_timeside.SetRange(0, CPb::length);
        m_timeside.SetPos(0);
        m_playbtn.SetWindowTextW(_T("暂停"));
        m_playbtn.SetIcon(m_hPlayIcon);
        m_infosta.SetWindowTextW(CPb::pl_title[Id]);
        m_ttimesta.SetWindowTextW(BASS::TimeToString(BASS::ChannelBytes2Seconds(CPb::length)));
        m_playlist.EnsureVisible(Id, TRUE);
    }
    else
    {
        Play(0);
    }
}

void CPowerAudioPlayerDlg::AddToList(CString Path, CString Title, int Time, bool IsConvert)
{
    if (Title == _T("")) Title = CPb::GetInPathFileName(Path);
    CPb::pl_path.push_back(Path);
    CPb::pl_title.push_back(Title);
    CPb::pl_time.push_back(Time);
    CPb::pl_isconvert.push_back(IsConvert);
    int index = m_playlist.GetItemCount();
    m_playlist.InsertItem(index, CPb::GetInPathFileName(Title));
    m_playlist.SetItemText(index, 1, BASS::TimeToString(Time));
}

void CPowerAudioPlayerDlg::DelToList(int Id)
{
    CPb::pl_totaltime = CPb::pl_totaltime - CPb::pl_time[Id];
    CPb::pl_path.erase(CPb::pl_path.begin() + Id);
    CPb::pl_title.erase(CPb::pl_title.begin() + Id);
    CPb::pl_time.erase(CPb::pl_time.begin() + Id);
    CPb::pl_isconvert.erase(CPb::pl_isconvert.begin() + Id);
    m_Statusbar.SetPaneText(0, CPb::i2cs(CPb::pl_path.size()));
    m_Statusbar.SetPaneText(1, BASS::TimeToString(CPb::pl_totaltime));
}

void CPowerAudioPlayerDlg::CleanList()
{
    m_playlist.DeleteAllItems();
    CPb::pl_title.clear();
    CPb::pl_path.clear();
    CPb::pl_time.clear();
    CPb::pl_isconvert.clear();
    CPb::pl_totaltime = 0;
    m_Statusbar.SetPaneText(0, _T("0"));
    m_Statusbar.SetPaneText(1, _T("00:00"));
}

void CPowerAudioPlayerDlg::SaveList(CString Path)
{
    if (Path == _T("")) Path = CPb::GetExeModuleDir() + _T("\\default.json");
    Json::Value root;
    Json::StyledWriter sw;
    int total = CPb::pl_path.size();
    for (int i = 0; i < total; ++i)
    {
        root[i]["title"] = Json::Value(CPb::CStrToChar(CPb::pl_title[i]));
        root[i]["path"] = Json::Value(CPb::CStrToChar(CPb::pl_path[i]));
        root[i]["time"] = Json::Value(CPb::pl_time[i]);
        root[i]["isconvert"] = Json::Value(CPb::pl_isconvert[i]);
    }
    std::ofstream os;
    os.open(Path, std::ios::out);
    os << sw.write(root);
    os.close();
}

void CPowerAudioPlayerDlg::LoadList(CString Path)
{
    if (Path == _T("")) Path = CPb::GetExeModuleDir() + _T("\\default.json");
    if (PathFileExists(Path) == FALSE) return;
    CleanList();
    Json::Reader reader;
    Json::Value root;
    std::ifstream in(Path, std::ios::binary);
    if (reader.parse(in, root))
    {
        for (int i = 0; i < root.size(); ++i)
        {
            CString nTitle = (CString)root[i]["title"].asCString();
            CString nPath = (CString)root[i]["path"].asCString();
            int nTime = root[i]["time"].asInt();
            bool nIsConvert = root[i]["isconvert"].asBool();
            AddToList(nPath, nTitle, nTime, nIsConvert);
            CPb::pl_totaltime += nTime;
        }
        m_Statusbar.SetPaneText(0, CPb::i2cs(CPb::pl_path.size()));
        m_Statusbar.SetPaneText(1, BASS::TimeToString(CPb::pl_totaltime));
    }
    in.close();
}

void CPowerAudioPlayerDlg::ConvertList(bool ReConvert)
{
    if(CPb::pl_path.size() == 0)
        return;
    if (ReConvert)
        CPb::pl_totaltime = 0;
    int total = CPb::pl_path.size();
    for (int i = 0; i < total; i++)
    {
        if (CPb::pl_isconvert[i] == TRUE && ReConvert == FALSE)
            continue;
        HSTREAM STREAM = (!CPb::IsUrl(CPb::pl_path[i]) ? BASS_StreamCreateFile(FALSE, CPb::pl_path[i], NULL, NULL, 256) : BASS_StreamCreateURL((char *)CPb::CStrToChar(CPb::pl_path[i]), 0, BASS_SAMPLE_FLOAT, NULL, NULL));
        CString FMT = CPb::set.spl_title_format;
        FMT.Replace(_T("%FILE"), CPb::GetInPathFileName(CPb::pl_path[i]));
        CPb::pl_title[i] = CPb::CharToLPCWSTR((char *)TAGS_Read(STREAM, CPb::CStrToChar(FMT)));
        if (CPb::pl_title[i] == _T(""))
            CPb::pl_title[i] = CPb::GetInPathFileName(CPb::pl_path[i]);
        CPb::pl_time[i] = BASS_ChannelBytes2Seconds(STREAM, BASS_ChannelGetLength(STREAM, 0));
        CPb::pl_totaltime += CPb::pl_time[i];
        m_playlist.SetItemText(i, 0, CPb::pl_title[i]);
        m_playlist.SetItemText(i, 1, BASS::TimeToString(CPb::pl_time[i]));
        CPb::pl_isconvert[i] = TRUE;
        if (i == CPb::PlayId)
            m_infosta.SetWindowText(CPb::pl_title[i]);
        BASS_StreamFree(STREAM);
        m_Statusbar.SetPaneText(1, BASS::TimeToString(CPb::pl_totaltime));
        AfxPumpMessage();
    }
    m_Statusbar.SetPaneText(0, CPb::i2cs(CPb::pl_path.size()));
    CPb::ToConvertList = FALSE;
}

void CPowerAudioPlayerDlg::LoadPlugins()
{
    CString filepath = CPb::GetExeModuleDir();
    CString filename = _T("");
    CString AllFile, OtherFile;
    CFileFind find;
    BOOL IsFind = find.FindFile(filepath + _T("\\plugins\\*.dll"));
    while (IsFind)
    {
        IsFind = find.FindNextFile();
        if (find.IsDots())
        {
            continue;
        }
        else
        {
            filename = filepath + "\\plugins\\" + find.GetFileName();
            BASS::PluginLoad(CPb::CStrToChar(filename), 0);
        }
    }
    CString Basic_name;
    CString Basic_exts;
    Basic_name.LoadStringW(IDS_BASIC_EXTS_NAME);
    Basic_exts.LoadStringW(IDS_BASIC_EXTS);
    AllFile.Format(_T("所有支持的文件|%s"), Basic_exts);
    OtherFile.Format(_T("%s(%s)|%s"), Basic_name, Basic_exts, Basic_exts);
    CString Name = _T("");
    CString Exts = _T("");
    CString Add = _T("");
    std::vector<CString> _temp;
    for (int i = 0; i < BASS::PLUGINS.size(); ++i)
    {
        const BASS_PLUGININFO *info = BASS::PluginGetInfo(i);
        if (info)
        {
            if (info->formats->name) Name = CPb::CharToLPCWSTR((char *)info->formats->name);
            if (info->formats->exts) Exts = CPb::CharToLPCWSTR((char *)info->formats->exts);
            _temp.push_back(Exts);
            Add.Format(_T("|%s(%s)|%s"), Name, Exts, Exts);
            OtherFile += Add;
            AllFile += _T(";") + Exts;
        }
    }
    LoadStr.LoadStringW(IDS_FILTER_ALL);
    CPb::SFF = AllFile + _T("|") + OtherFile + _T("|") + LoadStr;

    CStringArray StrArray;
    for(int i = 0; i < _temp.size(); ++i)
    {
        CPb::split(_temp[i], *";", StrArray);
        for (int j = 0; j < StrArray.GetCount(); ++j)
        {
            
            CPb::support_exts.push_back(StrArray[j]);
        }
    }
}

void CPowerAudioPlayerDlg::BuildSFXList()
{
    CString filepath = CPb::GetExeModuleDir();
    CFileFind find;
    CString FindFiles[2] = { _T("\\sfx\\*.dll"), _T("\\sfx\\*.svp") };
    for(int j = 0; j < 2; ++j)
    {
        bool IsFind = find.FindFile(filepath + FindFiles[j]);
        while (IsFind)
        {
            IsFind = find.FindNextFile();
            if (find.IsDots())
            {
                continue;
            }
            else
            {
                CPb::SFXs.push_back(_T(".\\sfx\\") + find.GetFileName());
            }
        }
    }
}

void CPowerAudioPlayerDlg::BuildDSPList()
{
    CString filepath = CPb::GetExeModuleDir();
    CFileFind find;
    bool IsFind = find.FindFile(filepath + "\\dsp\\*.dll");
    while (IsFind)
    {
        IsFind = find.FindNextFile();
        if (find.IsDots())
        {
            continue;
        }
        else
        {
            CPb::DSPs.push_back(_T(".\\dsp\\") + find.GetFileName());
        }
    }
}

void CPowerAudioPlayerDlg::RestUI()
{
    m_infosta.SetWindowText(_T(VER_NAME));
    m_ttimesta.SetWindowText(_T("00:00"));
    m_ntimesta.SetWindowText(_T("00:00"));
    m_timeside.SetPos(0);
    m_playbtn.SetWindowTextW(_T("播放"));
    m_playbtn.SetIcon(m_hPauseIcon);
}

void CPowerAudioPlayerDlg::CmdLineHandle(LPTSTR CmdLine)
{
    LPTSTR cmd;
    if (CmdLine == NULL)
        cmd = ::GetCommandLine();
    else
        cmd = CmdLine;
    int argc = 0;
    WCHAR *const *argv;
    argv = ::CommandLineToArgvW(cmd, &argc);
    for(int i = 0; i < argc; ++i)
    {
        if (i == 0)
            continue;
        CString FileName = CPb::GetInPathFileName(argv[i]);
        CString ext = FileName.Right(FileName.GetLength() - FileName.ReverseFind('.') - 1);
        if (CPb::IsInVectorCString(CPb::support_exts, _T("*.") + ext.MakeLower()))
        {
            AddToList(argv[i]);
        }
    }
}

void CPowerAudioPlayerDlg::FindFiles(CString strPath, CString ext)
{
    //AfxMessageBox(ext);
    CFileFind find;
    bool IsFind = find.FindFile(strPath + _T("\\") + ext);
    while (IsFind)
    {
        IsFind = find.FindNextFile();
        if (find.IsDots())
        {
            continue;
        }
        else
        {
            m_FindFileStatic->SetWindowText(_T("寻找文件...\r\n") + find.GetFilePath());
            AddToList(strPath + _T("\\") + find.GetFileName());
        }
    }
}
// CPowerAudioPlayerDlg 消息处理程序

BOOL CPowerAudioPlayerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hBigIcon, TRUE);			// 设置大图标
    SetIcon(m_hSmallIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    if (!BASS_Init(-1, 44100, NULL, NULL, NULL))
    {
        AfxMessageBox(_T("Initializing BASS failed with error code ") + CPb::i2cs(BASS::ErrorGetCode()));
        exit(-1);
    }
    BASS_WADSP_Init(this->m_hWnd);
    SetTimer(TIMER_ALAWAYS, 400, NULL);
    m_playlist.InsertColumn(0, _T("标题"), LVCFMT_LEFT, 210);
    m_playlist.InsertColumn(1, _T("时间"), LVCFMT_LEFT, 50);
    m_playlist.SetExtendedStyle(m_playlist.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_Statusbar.Create(this);
    m_Statusbar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
    CRect rect;
    GetWindowRect(&rect);
    m_Statusbar.SetPaneInfo(0, IDS_SBAR_COUNT, SBPS_NORMAL, rect.Width() / 2);
    m_Statusbar.SetPaneInfo(1, IDS_SBAR_TOTALTIME, SBPS_NORMAL, rect.Width() / 2);
    m_Statusbar.SetPaneText(0, _T("0"));
    m_Statusbar.SetPaneText(1, _T("00:00"));
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

    m_playbtn.SetIcon(m_hPauseIcon);
    m_stopbtn.SetIcon(m_hStopIcon);
    m_backbtn.SetIcon(m_hBackIcon);
    m_nextbtn.SetIcon(m_hNextIcon);
    m_listbtn.SetIcon(m_hListIcon);
    m_volchk.SetIcon(m_hVolIcon);

    LoadPlugins();
    BuildSFXList();
    BuildDSPList();
    LoadList();
    LoadSettings();
    CmdLineHandle();
    return TRUE;
}

LRESULT CPowerAudioPlayerDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
    int Active = BASS::ChannelIsActive();
    if (Active == BASS_ACTIVE_PLAYING)
    {
        UpdateThumbarButton(this->m_hWnd, FALSE);
    }
    else if (Active == BASS_ACTIVE_PAUSED || Active == BASS_ACTIVE_STOPPED)
    {
        UpdateThumbarButton(this->m_hWnd, TRUE);
    }
    if (CPb::ToConvertList == TRUE)
    {
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
    CMenu *pSubMenu = NULL;
    menu.LoadMenu(IDR_PLMENU);
    CPoint posMouse;
    GetCursorPos(&posMouse);
    pSubMenu = menu.GetSubMenu(0);
    pSubMenu->TrackPopupMenu(0, posMouse.x, posMouse.y, this);
}

void CPowerAudioPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == TIMER_ALAWAYS)
    {

    }
    else if (nIDEvent == TIMER_PLAYING)
    {
        int Position = BASS::ChannelGetPosition(0);
        m_timeside.SetPos(Position);
        m_ntimesta.SetWindowTextW(BASS::TimeToString(BASS::ChannelBytes2Seconds(Position)));
        if (BASS::ChannelIsActive() == BASS_ACTIVE_STOPPED)    //播放完了
        {
            KillTimer(TIMER_PLAYING);
            switch (CPb::set.playmode)
            {
                case 0:         //顺序播放
                {
                    if (CPb::PlayId == CPb::pl_path.size())
                    {
                        BASS::ChannelFree();
                        RestUI();
                    }
                    else
                    {
                        CPowerAudioPlayerDlg::OnBnClickedButton4();
                    }
                    break;
                }
                case 1:         //单曲循环
                {
                    Play(CPb::PlayId);
                    break;
                }
                case 2:         //列表循环
                {
                    CPowerAudioPlayerDlg::OnBnClickedButton4();
                    break;
                }
                case 3:         //随机播放
                {
                    Play(CPb::GetRand(0, CPb::pl_path.size()));
                    break;
                }
            }
        }
    }
    CDialogEx::OnTimer(nIDEvent);
}

void CPowerAudioPlayerDlg::OnBnClickedButton1()
{
    int Active = BASS::ChannelIsActive();
    if (BASS::Stream == 0)
    {
        Play(0);
        return;
    }
    if (Active == BASS_ACTIVE_STOPPED)
    {
        if (CPb::IsInVectorCString(CPb::pl_path, CPb::PlayPath))
        {
            BASS::ChannelPlay(TRUE);
        }
        else
        {
            Play(CPb::PlayId);
        }
    }
    if (Active == BASS_ACTIVE_PLAYING)
    {
        BASS::ChannelPause();
        m_playbtn.SetWindowTextW(_T("播放"));
        m_playbtn.SetIcon(m_hPauseIcon);
        KillTimer(TIMER_PLAYING);
    }
    else if(Active == BASS_ACTIVE_PAUSED)
    {
        BASS::ChannelPlay(FALSE);
        m_playbtn.SetWindowTextW(_T("暂停"));
        m_playbtn.SetIcon(m_hPlayIcon);
        SetTimer(TIMER_PLAYING, 200, NULL);
    }
}


void CPowerAudioPlayerDlg::OnBnClickedButton2()
{
    BASS::ChannelStop();
    KillTimer(TIMER_PLAYING);
    BASS::ChannelSetPosition(0, 0);
    m_timeside.SetPos(0);
    m_playbtn.SetWindowTextW(_T("播放"));
    m_playbtn.SetIcon(m_hPauseIcon);
    RestUI();
}

void CPowerAudioPlayerDlg::OnBnClickedButton3()
{
    Play(CPb::PlayId - 1);
}


void CPowerAudioPlayerDlg::OnBnClickedButton4()
{
    Play(CPb::PlayId + 1);
}


void CPowerAudioPlayerDlg::OnBnClickedCheck1()
{
    if (m_volchk.GetCheck())
    {
        BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, 0);
        m_volside.EnableWindow(FALSE);
        m_volsta.SetWindowText(_T("静音"));
        m_volchk.SetIcon(m_hMuteIcon);
    }
    else
    {
        m_volside.EnableWindow(TRUE);
        m_volchk.SetIcon(m_hVolIcon);
        CPowerAudioPlayerDlg::ChangeVolumeSide();
    }
    CPb::set.is_mute = m_volchk.GetCheck();
}

void CPowerAudioPlayerDlg::OnClose()
{
    On32798();
    SaveList(NULL);
    CPb::WriteSettings();
    if(BASS::Wadsp)
    {
        BASS_WADSP_Stop(BASS::Wadsp);
        BASS_WADSP_FreeDSP(BASS::Wadsp);
    }
    BASS_WADSP_Free();
    BASS_Free();
    HANDLE hself = GetCurrentProcess();
    TerminateProcess(hself, 0);
    CDialogEx::OnClose();
}

void CPowerAudioPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
    CWnd *pVolumeSider = GetDlgItem(IDC_SLIDER1);
    CWnd *pTimeSider = GetDlgItem(IDC_SLIDER2);
    if(pScrollBar == pVolumeSider)
    {
        CPb::set.vol = m_volside.GetPos();
        ChangeVolumeSide();
    }
    else if (pScrollBar == pTimeSider)
    {
        if (BASS::ChannelIsActive() == 0)
        {
            return;
        }
        BASS::ChannelSetPosition(m_timeside.GetPos(), BASS_POS_BYTE);
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPowerAudioPlayerDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int nIndex = m_playlist.GetSelectionMark();
    if (nIndex != -1)
    {
        CPb::set.pl_location = nIndex;
        Play(nIndex);
    }
    *pResult = 0;
}

void CPowerAudioPlayerDlg::On32776()
{
    CFileDialog Dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, CPb::SFF, this);
    CString FilePath;
    DWORD max_file = 60000;
    TCHAR *lsf = new TCHAR[max_file];
    Dlg.m_ofn.nMaxFile = max_file;
    Dlg.m_ofn.lpstrFile = lsf;
    Dlg.m_ofn.lpstrFile[0] = NULL;
    if (Dlg.DoModal() == IDOK)
    {
        POSITION posFile = Dlg.GetStartPosition();
        while (posFile != NULL)
        {
            FilePath = Dlg.GetNextPathName(posFile);
            AddToList(FilePath);
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
    if (Dlg.DoModal() == IDOK)
    {
        FilePath = Dlg.GetPathName();
        LoadList(FilePath);
    }
}


void CPowerAudioPlayerDlg::On32782()
{
    LoadStr.LoadStringW(IDS_FILTER_PAPPL);
    CFileDialog Dlg(FALSE, _T("json"), NULL, 0, LoadStr, this);
    CString FilePath;
    if (Dlg.DoModal() == IDOK)
    {
        FilePath = Dlg.GetPathName();
        SaveList(FilePath);
    }
}

void CPowerAudioPlayerDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    if (m_playlist.GetSelectionMark() != -1)
    {
        CMenu menu;
        CMenu *pSubMenu = NULL;
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
        if (m_playlist.GetItemState(iItem, LVIS_SELECTED) == LVIS_SELECTED)
        {
            m_playlist.DeleteItem(iItem);
            DelToList(iItem);
        }
    }
}


void CPowerAudioPlayerDlg::On32784()
{
    CleanList();
}


void CPowerAudioPlayerDlg::On32794()
{
    ConvertList(TRUE);
}


void CPowerAudioPlayerDlg::OnMenu32786()
{
    On32779();
}


void CPowerAudioPlayerDlg::OnMenu32795()
{
    CSettingsDlg Modal;
    Modal.DoModal();
}


void CPowerAudioPlayerDlg::On32797()
{
    if (m_SFXDlg == NULL || !m_SFXDlg->GetSafeHwnd())
    {
        m_SFXDlg = new CSFXDlg(this);
        m_SFXDlg->Create(IDD_SFXDLG, this);
    }
    m_SFXDlg->ShowWindow(TRUE);
}


void CPowerAudioPlayerDlg::On32798()
{
    if (m_SFXDlg->GetSafeHwnd())
    {
        m_SFXDlg->DestroyWindow();
    }
}


BOOL CPowerAudioPlayerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    int pos = m_volside.GetPos();
    if (zDelta > 0)
    {
        m_volside.SetPos(pos - 2);
    }
    else if (zDelta < 0)
    {
        m_volside.SetPos(pos + 2);
    }
    ChangeVolumeSide();
    return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

LRESULT CPowerAudioPlayerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    static UINT s_uTBBC = WM_NULL;
    if (s_uTBBC == WM_NULL)
    {
        s_uTBBC = RegisterWindowMessage(L"TaskbarButtonCreated");
        ChangeWindowMessageFilter(s_uTBBC, MSGFLT_ADD);
        ChangeWindowMessageFilter(WM_COMMAND, MSGFLT_ADD);
    }
    if (message == s_uTBBC)
    {
        CreateThumbnailToolbar(this->m_hWnd);
    }
    else
    {
        switch (message)
        {
        case WM_COMMAND:
        {
            int const wmId = LOWORD(wParam);
            switch (wmId)
            {
            case TASKBARBTN_PLAY:
            {
                CPowerAudioPlayerDlg::OnBnClickedButton1();
                break;
            }
            case TASKBARBTN_STOP:
            {
                CPowerAudioPlayerDlg::OnBnClickedButton2();
                break;
            }
            case TASKBARBTN_BACK:
            {
                CPowerAudioPlayerDlg::OnBnClickedButton3();
                break;
            }
            case TASKBARBTN_NEXT:
            {
                CPowerAudioPlayerDlg::OnBnClickedButton4();
                break;
            }
            }
        }
        }
    }
    return CDialogEx::WindowProc(message, wParam, lParam);
}


HRESULT CPowerAudioPlayerDlg::CreateThumbnailToolbar(HWND hWnd)
{
    ITaskbarList3 *pTaskbarList;
    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pTaskbarList));
    if (SUCCEEDED(hr))
    {
        hr = pTaskbarList->HrInit();
        if (SUCCEEDED(hr))
        {
            struct
            {
                PCWSTR pbmp;
                int cx;
            }
            const bitmaps[4] =
            {
                { MAKEINTRESOURCE(IDB_IMGGROUP),  16 },
                { MAKEINTRESOURCE(IDB_IMGGROUP), 20 },
                { MAKEINTRESOURCE(IDB_IMGGROUP), 24 },
                { MAKEINTRESOURCE(IDB_IMGGROUP), 28 }
            };
            int const cxButton = GetSystemMetrics(SM_CXSMICON);
            int iButtons = 0;
            for (int i = 0; i < ARRAYSIZE(bitmaps); i++)
            {
                if (bitmaps[i].cx <= cxButton)
                {
                    iButtons = i;
                }
            }
            HIMAGELIST himl = ImageList_LoadImage(GetModuleHandle(NULL), bitmaps[iButtons].pbmp, bitmaps[iButtons].cx, 0, RGB(255, 0, 255), IMAGE_BITMAP, LR_CREATEDIBSECTION);
            if (himl)
            {
                hr = pTaskbarList->ThumbBarSetImageList(hWnd, himl);
                if (SUCCEEDED(hr))
                {
                    THUMBBUTTON buttons[4] = {};
                    buttons[0].dwMask = THB_BITMAP | THB_TOOLTIP | THB_FLAGS;
                    buttons[0].dwFlags = THBF_ENABLED | THBF_DISMISSONCLICK;
                    buttons[0].iId = TASKBARBTN_BACK;
                    buttons[0].iBitmap = 3;
                    StringCchCopy(buttons[0].szTip, ARRAYSIZE(buttons[0].szTip), L"上一首");

                    buttons[1].dwMask = THB_BITMAP | THB_TOOLTIP | THB_FLAGS;
                    buttons[1].dwFlags = THBF_ENABLED | THBF_DISMISSONCLICK;
                    buttons[1].iId = TASKBARBTN_PLAY;
                    buttons[1].iBitmap = 1;
                    StringCchCopy(buttons[1].szTip, ARRAYSIZE(buttons[1].szTip), L"播放");

                    buttons[2].dwMask = THB_BITMAP | THB_TOOLTIP | THB_FLAGS;
                    buttons[2].dwFlags = THBF_ENABLED | THBF_DISMISSONCLICK;
                    buttons[2].iId = TASKBARBTN_STOP;
                    buttons[2].iBitmap = 2;
                    StringCchCopy(buttons[2].szTip, ARRAYSIZE(buttons[2].szTip), L"停止");

                    buttons[3].dwMask = THB_BITMAP | THB_TOOLTIP | THB_FLAGS;
                    buttons[3].dwFlags = THBF_ENABLED | THBF_DISMISSONCLICK;
                    buttons[3].iId = TASKBARBTN_NEXT;
                    buttons[3].iBitmap = 4;
                    StringCchCopy(buttons[3].szTip, ARRAYSIZE(buttons[2].szTip), L"下一首");

                    hr = pTaskbarList->ThumbBarAddButtons(hWnd, ARRAYSIZE(buttons), buttons);
                }
                ImageList_Destroy(himl);
            }
        }
        pTaskbarList->Release();
    }

    return hr;
}

HRESULT CPowerAudioPlayerDlg::UpdateThumbarButton(HWND hwnd, bool isPause)
{
    THUMBBUTTON thbButton;
    thbButton.dwMask = THB_BITMAP | THB_TOOLTIP | THB_FLAGS;
    thbButton.dwFlags = THBF_ENABLED | THBF_DISMISSONCLICK;
    thbButton.iId = TASKBARBTN_PLAY;
    thbButton.iBitmap = isPause ? 1 : 0;
    StringCchCopy(thbButton.szTip, ARRAYSIZE(thbButton.szTip), isPause ? L"播放" : L"暂停");
    ITaskbarList3 *ptbl;
    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ptbl));
    if (SUCCEEDED(hr))
    {
        hr = ptbl->ThumbBarUpdateButtons(hwnd, 1, &thbButton);
        ptbl->Release();
    }
    return hr;
}

BOOL CPowerAudioPlayerDlg::PreTranslateMessage(MSG *pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        switch (pMsg->wParam)
        {
        case VK_RETURN:
            return TRUE;
        case VK_ESCAPE:
            return TRUE;
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

void CPowerAudioPlayerDlg::On32800()
{
    ChangePlayMode(0);
    GetMenu()->GetSubMenu(0)->CheckMenuRadioItem(ID_32800, ID_32803, ID_32800, MF_BYCOMMAND);
}


void CPowerAudioPlayerDlg::On32801()
{
    ChangePlayMode(1);
    GetMenu()->GetSubMenu(0)->CheckMenuRadioItem(ID_32800, ID_32803, ID_32801, MF_BYCOMMAND);
}


void CPowerAudioPlayerDlg::On32802()
{
    ChangePlayMode(2);
    GetMenu()->GetSubMenu(0)->CheckMenuRadioItem(ID_32800, ID_32803, ID_32802, MF_BYCOMMAND);
}



void CPowerAudioPlayerDlg::On32803()
{
    ChangePlayMode(3);
    GetMenu()->GetSubMenu(0)->CheckMenuRadioItem(ID_32800, ID_32803, ID_32803, MF_BYCOMMAND);
}

void CPowerAudioPlayerDlg::OnMenu32785()
{
    int iSel = m_playlist.GetSelectionMark();
    if (iSel != -1)
    {
        Play(iSel);
    }
}


void CPowerAudioPlayerDlg::On32778()
{
    CString url;
    if (CInputDlg::InputBox(_T("输入文件的URL\r\n例子：https://example.com/test.mp3\r\nftp://example.com/test.mp3"), NULL, NULL, url))
    {
        if (url != _T(""))
        {
            AddToList(url);
            CPb::ToConvertList = TRUE;
        }
    }
}


void CPowerAudioPlayerDlg::On32781()
{
    On32784();
}


void CPowerAudioPlayerDlg::OnDropFiles(HDROP hDropInfo)
{
    int count_droppedfile = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
    for (int i = 0; i < count_droppedfile; ++i)
    {
        wchar_t filepath[MAX_PATH] = { 0 };
        if (DragQueryFile(hDropInfo, i, filepath, MAX_PATH) > 0)
        {
            CString FileName = CPb::GetInPathFileName(filepath);
            CString ext = FileName.Right(FileName.GetLength() - FileName.ReverseFind('.') - 1);
            if (CPb::IsInVectorCString(CPb::support_exts, _T("*.") + ext.MakeLower()))
            {
                AddToList(filepath);
            }
        }
    }
    CPb::ToConvertList = TRUE;
    CDialogEx::OnDropFiles(hDropInfo);
}

LRESULT CPowerAudioPlayerDlg::OnMsg(WPARAM wParam, LPARAM lParam)
{
    if (wParam == 1000)
    {
        LPTSTR Command = (LPTSTR)(LPCTSTR)theApp.GetProfileStringW(_T("MsgPass"), _T("Command"));
        CmdLineHandle(Command);
    }
    return LRESULT();
}


void CPowerAudioPlayerDlg::On32805()
{
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(BROWSEINFO));
    bi.hwndOwner = m_hWnd;
    bi.ulFlags = BIF_RETURNONLYFSDIRS;
    bi.lpszTitle = _T("选择欲添加的文件夹");
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    BOOL bRet = FALSE;
    TCHAR szFolder[MAX_PATH * 2];
    szFolder[0] = _T('/0');
    if (pidl)
    {
        if (SHGetPathFromIDList(pidl, szFolder))
            bRet = TRUE;
        IMalloc *pMalloc = NULL;
        if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
        {
            pMalloc->Free(pidl);
            pMalloc->Release();
        }
    }
    if (szFolder)
    {
        CRect rect;
        GetClientRect(rect);
        m_FindFileStatic = new CStatic();
        m_FindFileStatic->ModifyStyle(0, SS_LEFTNOWORDWRAP);
        m_FindFileStatic->Create(_T("寻找文件..."), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, rect.Width(), rect.Height()), this);
        for (int i = 0; i < CPb::support_exts.size(); ++i)
        {
            FindFiles(szFolder, CPb::support_exts[i]);
        }
        CPb::ToConvertList = TRUE;
        delete m_FindFileStatic;
    }
}


void CPowerAudioPlayerDlg::OnMenu32791()
{
    CFileInfoDlg Modal;
    Modal.DoModal();
}


void CPowerAudioPlayerDlg::OnLvnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
    if (VK_DELETE)
        On32779();
    *pResult = 0;
}


HBRUSH CPowerAudioPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    
     //TODO:  在此更改 DC 的任何特性
    if (pWnd->GetDlgCtrlID() == IDC_INFO)
    {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(pDC->GetBkColor());
        return HBRUSH(GetStockObject(HOLLOW_BRUSH));
    }
     //TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}
