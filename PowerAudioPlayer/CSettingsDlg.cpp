// CSettingsDlg.cpp: 实现文件
//

#include "pch.h"
#include "PowerAudioPlayer.h"
#include "afxdialogex.h"
#include "CSettingsDlg.h"


// CSettingsDlg 对话框

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS, pParent)
{

}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_pgctrl);
	DDX_Control(pDX, IDC_CANCELBTN, m_can);
	DDX_Control(pDX, IDC_OKBTN, m_ok);
	DDX_Control(pDX, IDC_STATIC23, m_static23);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSettingsDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_OKBTN, &CSettingsDlg::OnBnClickedOkbtn)
	ON_BN_CLICKED(IDC_CANCELBTN, &CSettingsDlg::OnBnClickedCancelbtn)
END_MESSAGE_MAP()


// CSettingsDlg 消息处理程序


BOOL CSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LOGFONT font = { NULL };
	CString SFFilter, SKINFilter, TITLEFMTDescribe;
	SFFilter.LoadStringW(IDS_FILTER_SF);
	SKINFilter.LoadStringW(IDS_FILTER_SKIN);
	TITLEFMTDescribe.LoadStringW(IDS_DESCRIBE_TITLE);

	/*
	CMFCPropertyGridProperty* pProp1 = new CMFCPropertyGridFileProperty(_T("MIDI音色库"),TRUE,_T(""),NULL,NULL,LoadStr);
	
	CMFCPropertyGridProperty* group0 = new CMFCPropertyGridProperty(_T("MIDI"));
	CMFCPropertyGridProperty* group1 = new CMFCPropertyGridProperty(_T("MIDI"));
	group0->AddSubItem(pProp1);
	m_pgctrl.AddProperty(group0);*/
	CMFCPropertyGridProperty* Groups[] = {
		new CMFCPropertyGridProperty(_T("首选项")), //0
		new CMFCPropertyGridProperty(_T("列表")),	//1
		new CMFCPropertyGridProperty(_T("MIDI")),	//2
		new CMFCPropertyGridProperty(_T("DSP")),	//3
		new CMFCPropertyGridProperty(_T("插件")),	//4
		new CMFCPropertyGridProperty(_T("关于"))	//5
	};
	/////////////
	CMFCPropertyGridProperty* g0Props[] = {
		new CMFCPropertyGridProperty(_T("记忆播放位置"),_T(""), _T("启动时列表是否跳到上次播放位置")),
		new CMFCPropertyGridProperty(_T("接收文件拖放"),_T(""), _T("是否接收文件拖放")),
		new CMFCPropertyGridFileProperty(_T("皮肤"),TRUE,NULL,NULL,NULL,SKINFilter,_T("留空则不加载")),
		new CMFCPropertyGridProperty(_T("可视化效果刷新周期"),(_variant_t)0, _T("设置可视化效果每次刷新时的间隔，单位为毫秒，越小的值会获得更流畅的效果，但也会占用更多资源"), NULL, NULL, NULL,_T("0123456789"))
	};
	g0Props[3]->EnableSpinControl(TRUE, 1, 1000);
	MFCPropertyGridPropertyMakeTrueOrFalse(g0Props[0]);
	MFCPropertyGridPropertyMakeTrueOrFalse(g0Props[1]);
	for (int i = 0; i < 4; ++i) {
		Groups[0]->AddSubItem(g0Props[i]);
	}
	/////////////
	CMFCPropertyGridProperty* g1Props[] = {
		new CMFCPropertyGridProperty(_T("标题显示格式"),_T(""), _T("可以自定义列表标题的显示格式，支持条件判断，当遇到不支持的文件时将以文件名作为标题。用法如下："+TITLEFMTDescribe)),
		new CMFCPropertyGridProperty(_T("显示序号"),_T(""), _T("是否在标题前显示项目在列表的序号")),
	};
	MFCPropertyGridPropertyMakeTrueOrFalse(g1Props[1]);
	for (int i = 0; i < 1; ++i) {
		Groups[1]->AddSubItem(g1Props[i]);
	}
	/////////////
	CMFCPropertyGridProperty* g2Props[] = {
		new CMFCPropertyGridFileProperty(_T("音色库"),TRUE,_T(""),NULL,NULL,LoadStr)
	};
	for (int i = 0; i < 1; ++i) {
		Groups[2]->AddSubItem(g2Props[i]);
	}
	/////////////
	std::vector<CMFCPropertyGridProperty*>g3Props;
	g3Props.push_back(new CMFCPropertyGridProperty(_T("名称"),_T("支持格式"), _T("")));
	g3Props[0]->AllowEdit(FALSE);
	CString Basic_name;
	CString Basic_exts;
	Basic_name.LoadStringW(IDS_BASIC_EXTS_NAME);
	Basic_exts.LoadStringW(IDS_BASIC_EXTS);
	g3Props.push_back(new CMFCPropertyGridProperty(Basic_name, Basic_exts, _T("")));
	g3Props[1]->AllowEdit(FALSE);
	CString Name = _T("");
	CString Exts = _T("");
	for (int i = 0; i < BASS::PLUGINS.size(); ++i) {
		const BASS_PLUGININFO* info = BASS::PluginGetInfo(i);
		if (info){
			if(info->formats->name) Name = CPb::CharToLPCWSTR((char*)info->formats->name);
			if (info->formats->exts) Exts = CPb::CharToLPCWSTR((char*)info->formats->exts);
			g3Props.push_back(new CMFCPropertyGridProperty(Name,Exts, _T("")));
			g3Props[g3Props.size()-1]->AllowEdit(FALSE);
		}
	}
	for (int i = 0; i < g3Props.size(); ++i) {
		Groups[4]->AddSubItem(g3Props[i]);
	}
	/////////////
	CMFCPropertyGridProperty* g5Props[] = {
		new CMFCPropertyGridProperty(_T("名称"),CPb::CharToLPCWSTR((char *)VER_NAME),_T("")),
		new CMFCPropertyGridProperty(_T("版本"),CPb::CharToLPCWSTR((char*)VER_VERSION),_T("")),
		new CMFCPropertyGridProperty(_T("BASS版本"),(LPCWSTR)CPb::i2cs(BASS::GetVersion()),_T("")),
	};
	g5Props[0]->AllowEdit(FALSE);
	g5Props[1]->AllowEdit(FALSE);
	g5Props[2]->AllowEdit(FALSE);
	for (int i = 0; i < 3; ++i) {
		Groups[5]->AddSubItem(g5Props[i]);
	}
	/////////////
	for (int i = 0; i < 6; ++i) {
		m_pgctrl.AddProperty(Groups[i]);
	}
	HDITEM item;
	item.cxy = 140;
	item.mask = HDI_WIDTH;
	m_pgctrl.GetHeaderCtrl().SetItem(0, new HDITEM(item));
	return TRUE;
}


void CSettingsDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CSettingsDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CSettingsDlg::MFCPropertyGridPropertyMakeTrueOrFalse(CMFCPropertyGridProperty *PropertyGridProperty)
{
	PropertyGridProperty->AddOption(_T("是"));
	PropertyGridProperty->AddOption(_T("否"));
	PropertyGridProperty->AllowEdit(FALSE);
}


void CSettingsDlg::OnSize(UINT nType, int cx, int cy)
{
	CRect Can;
	CRect OK;
	m_can.GetWindowRect(Can);
	m_ok.GetWindowRect(OK);
	m_pgctrl.SetWindowPos(NULL, 0, 0, cx, cy - 35, SWP_NOZORDER | SWP_NOMOVE);
	m_can.SetWindowPos(NULL, cx - 12 - Can.Width(), cy - 8 - Can.Height(), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_ok.SetWindowPos(NULL, cx - 12 - Can.Width() - 8 - OK.Width(), cy - 8 - OK.Height(), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	CDialogEx::OnSize(nType, cx, cy);
}


void CSettingsDlg::OnBnClickedOkbtn()
{
	CSettingsDlg::OnOK();
}


void CSettingsDlg::OnBnClickedCancelbtn()
{
	CSettingsDlg::OnCancel();
}
