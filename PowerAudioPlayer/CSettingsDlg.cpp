// CSettingsDlg.cpp: 实现文件
//

#include "pch.h"
#include "PowerAudioPlayer.h"
#include "afxdialogex.h"
#include "CSettingsDlg.h"


// CSettingsDlg 对话框

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_SETTINGS, pParent)
{

}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_pgctrl);
    DDX_Control(pDX, IDC_CANCELBTN, m_can);
    DDX_Control(pDX, IDC_OKBTN, m_ok);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_OKBTN, &CSettingsDlg::OnBnClickedOkbtn)
    ON_BN_CLICKED(IDC_CANCELBTN, &CSettingsDlg::OnBnClickedCancelbtn)
    ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()


// CSettingsDlg 消息处理程序


BOOL CSettingsDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    CString SFFilter, SKINFilter, TITLEFMTDescribe;
    SFFilter.LoadStringW(IDS_FILTER_SF);
    TITLEFMTDescribe.LoadStringW(IDS_DESCRIBE_TITLE);
    m_pgctrl.MarkModifiedProperties(TRUE);
    CMFCPropertyGridProperty *Groups[] =
    {
        new CMFCPropertyGridProperty(_T("首选项")), //0
        new CMFCPropertyGridProperty(_T("列表")),	//1
        new CMFCPropertyGridProperty(_T("MIDI")),	//2
        new CMFCPropertyGridProperty(_T("DSP")),	//3
        new CMFCPropertyGridProperty(_T("插件")),	//4
        new CMFCPropertyGridProperty(_T("关于"))	//5
    };
    /////////////0
    CMFCPropertyGridProperty *g0Props[] =
    {
        new CMFCPropertyGridProperty(_T("记忆播放位置"), PropertyGridPropertyOutputTrueOrFalse(CPb::set.smain_rem_pl_location), _T("启动时列表是否跳到上次播放位置")),
        new CMFCPropertyGridProperty(_T("可视化效果刷新周期"), (_variant_t)CPb::set.smain_sfx_render_elapse, _T("设置可视化效果每次刷新时的间隔，单位为毫秒，越小的值会获得更流畅的效果，但也会占用更多资源。你必须重新打开可视化效果窗口才能看到所做更改"), NULL, NULL, NULL, _T("0123456789")),
    };
    g0Props[3]->EnableSpinControl(TRUE, 1, 1000);
    MFCPropertyGridPropertyMakeTrueOrFalse(g0Props[0]);
    MFCPropertyGridPropertyMakeTrueOrFalse(g0Props[1]);
    for (int i = 0; i < 2; ++i)
    {
        Groups[0]->AddSubItem(g0Props[i]);
    }
    /////////////1
    CMFCPropertyGridProperty *g1Props[] =
    {
        new CMFCPropertyGridProperty(_T("标题显示格式"), CPb::set.spl_title_format, TITLEFMTDescribe),
        new CMFCPropertyGridProperty(_T("显示序号"), PropertyGridPropertyOutputTrueOrFalse(CPb::set.spl_show_snum), _T("是否在标题前显示项目在列表的序号")),
        new CMFCPropertyGridProperty(_T("注意"), _T("你必须重新转换列表才能看到所做更改"), _T("")),
    };
    MFCPropertyGridPropertyMakeTrueOrFalse(g1Props[1]);
    g1Props[2]->Enable(FALSE);
    for (int i = 0; i < 3; ++i)
    {
        Groups[1]->AddSubItem(g1Props[i]);
    }
    /////////////2
    CMFCPropertyGridProperty *g2Props[] =
    {
        new CMFCPropertyGridFileProperty(_T("音色库"), TRUE, CPb::set.smidi_sf_path, NULL, NULL, SFFilter, _T("选择播放MIDI时使用的音色库，更大的音色库将占用更多内存"))
    };
    for (int i = 0; i < 1; ++i)
    {
        Groups[2]->AddSubItem(g2Props[i]);
    }
    /////////////3
    CString p0Value;
    if (CPb::set.dsp_id != -1)
    {
        p0Value =CPb::GetInPathFileName(CPb::DSPs[CPb::set.dsp_id]);
    }
    else
    {
        p0Value = _T("空");
    }
    OldDSPId = CPb::set.dsp_id;
    CMFCPropertyGridProperty* g3Props[] =
    {
        new CMFCPropertyGridProperty(_T("加载的DSP"), p0Value, _T("选择需要加载的DSP")),
        new CMFCPropertyGridProperty(_T("配置DSP"), _T(""), _T("当DSP已加载时，选中下拉列表以配置DSP")),
    };
    g3Props[0]->AddOption(_T("空"));
    g3Props[1]->AddOption(_T("<配置>"));
    g3Props[1]->AllowEdit(FALSE);
    for (int i = 0; i < CPb::DSPs.size(); ++i) {
        g3Props[0]->AddOption(CPb::GetInPathFileName(CPb::DSPs[i]));
    }
    for (int i = 0; i < 2; ++i)
    {
        Groups[3]->AddSubItem(g3Props[i]);
    }
    /////////////4
    std::vector<CMFCPropertyGridProperty *>g4Props;
    g4Props.push_back(new CMFCPropertyGridProperty(_T("名称"), _T("支持格式"), _T("")));
    g4Props[0]->AllowEdit(FALSE);
    CString Basic_name;
    CString Basic_exts;
    Basic_name.LoadStringW(IDS_BASIC_EXTS_NAME);
    Basic_exts.LoadStringW(IDS_BASIC_EXTS);
    g4Props.push_back(new CMFCPropertyGridProperty(Basic_name, Basic_exts, _T("")));
    g4Props[1]->AllowEdit(FALSE);
    CString Name = _T("");
    CString Exts = _T("");
    for (int i = 0; i < BASS::PLUGINS.size(); ++i)
    {
        const BASS_PLUGININFO *info = BASS::PluginGetInfo(i);
        if (info)
        {
            if(info->formats->name) Name = CPb::CharToLPCWSTR((char *)info->formats->name);
            if (info->formats->exts) Exts = CPb::CharToLPCWSTR((char *)info->formats->exts);
            g4Props.push_back(new CMFCPropertyGridProperty(Name, Exts, _T("")));
            g4Props[g4Props.size() - 1]->AllowEdit(FALSE);
        }
    }
    for (int i = 0; i < g4Props.size(); ++i)
    {
        Groups[4]->AddSubItem(g4Props[i]);
    }    
    /////////////5
    CMFCPropertyGridProperty *g5Props[] =
    {
        new CMFCPropertyGridProperty(_T("名称"), CPb::CharToLPCWSTR((char *)VER_NAME), _T("")),
        new CMFCPropertyGridProperty(_T("版本"), CPb::CharToLPCWSTR((char *)VER_VERSION), _T("")),
        new CMFCPropertyGridProperty(_T("BASS版本"), (LPCWSTR)CPb::i2cs(BASS::GetVersion()), _T("")),
    };
    g5Props[0]->AllowEdit(FALSE);
    g5Props[1]->AllowEdit(FALSE);
    g5Props[2]->AllowEdit(FALSE);
    for (int i = 0; i < 3; ++i)
    {
        Groups[5]->AddSubItem(g5Props[i]);
    }
    /////////////
    for (int i = 0; i < 6; ++i)
    {
        m_pgctrl.AddProperty(Groups[i]);
    }
    HDITEM item;
    item.cxy = 140;
    item.mask = HDI_WIDTH;
    m_pgctrl.GetHeaderCtrl().SetItem(0, new HDITEM(item));
    return TRUE;
}

LRESULT CSettingsDlg::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
    CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
    COleVariant name = pProp->GetName();
    COleVariant value = pProp->GetValue();
    if ((CString)value == _T("<配置>") && (CString)name == _T("配置DSP"))
    {
        pProp->SetValue(static_cast <_variant_t>(""));
        if(!BASS_WADSP_Config(BASS::Wadsp)) {
            AfxMessageBox(_T("未加载DSP或没有可配置项目"));
        }
    }
    return 0;
}

void CSettingsDlg::MFCPropertyGridPropertyMakeTrueOrFalse(CMFCPropertyGridProperty *PropertyGridProperty)
{
    PropertyGridProperty->AddOption(_T("是"));
    PropertyGridProperty->AddOption(_T("否"));
    PropertyGridProperty->AllowEdit(FALSE);
}

LPCWSTR CSettingsDlg::PropertyGridPropertyOutputTrueOrFalse(bool val)
{
    if (val)
    {
        return _T("是");
    }
    else
    {
        return _T("否");
    }
}

bool CSettingsDlg::TXTTrueOrFalseToBool(CString val)
{
    if (val == _T("是"))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
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
    int count = m_pgctrl.GetPropertyCount();
    for (int i = 0; i < count; i++)
    {
        CMFCPropertyGridProperty* pProperty = m_pgctrl.GetProperty(i);
        if (pProperty == nullptr)
            continue;
        switch (i) {
            case 0:
            {
                int subItemCount = pProperty->GetSubItemsCount();
                for (int j = 0; j < subItemCount; j++)
                {
                    CMFCPropertyGridProperty* pSubProperty = pProperty->GetSubItem(j);
                    if (pSubProperty == nullptr)
                        continue;
                    COleVariant value = pSubProperty->GetValue();
                    switch (j) {
                        case 0:
                        {
                            CPb::set.smain_rem_pl_location =TXTTrueOrFalseToBool(value);
                            break;
                        }
                        case 1:
                        {
                            CPb::set.smain_sfx_render_elapse = (_variant_t)value;
                            break;
                        }
                    }
                }
                break;
            }
            case 1:
            {
                int subItemCount = pProperty->GetSubItemsCount();
                for (int j = 0; j < subItemCount; j++)
                {
                    CMFCPropertyGridProperty* pSubProperty = pProperty->GetSubItem(j);
                    if (pSubProperty == nullptr)
                        continue;
                    COleVariant value = pSubProperty->GetValue();
                    switch (j) {
                        case 0:
                        {
                            CPb::set.spl_title_format = value;
                            break;
                        }
                        case 1:
                        {
                            CPb::set.spl_show_snum = TXTTrueOrFalseToBool(value);
                            break;
                        }
                    }
                }
                break;
            }
            case 2:
            {
                CMFCPropertyGridProperty* pSubProperty = pProperty->GetSubItem(0);
                CPb::set.smidi_sf_path = pSubProperty->GetValue();
                break;
            }
            case 3:
            {
                CMFCPropertyGridProperty* pSubProperty = pProperty->GetSubItem(0);
                CString value = pSubProperty->GetValue();
                if (value == _T("空"))
                {
                    CPb::set.dsp_id = -1;
                }
                else
                {
                    for (int k = 0; k < CPb::DSPs.size(); ++k)
                    {
                        if (CPb::GetInPathFileName(CPb::DSPs[k]) == value)
                        {
                            CPb::set.dsp_id = k;
                            break; 
                        }
                    }
                }
                break;
            }
        }
    }
    if (CPb::set.dsp_id != -1) 
    {
        if(CPb::set.dsp_id != OldDSPId){
            if (BASS::Wadsp)
            {
                BASS_WADSP_Stop(BASS::Wadsp);
                BASS_WADSP_FreeDSP(BASS::Wadsp);
            }
            BASS::Wadsp = BASS_WADSP_Load(CPb::CStrToChar(CPb::DSPs[CPb::set.dsp_id]), NULL, NULL, NULL, NULL, NULL);
            BASS_WADSP_Start(BASS::Wadsp, NULL, 0);
            BASS_WADSP_ChannelSetDSP(BASS::Wadsp, BASS::Stream, NULL);
        }
    }
    else
    {
        BASS_WADSP_Stop(BASS::Wadsp);
        BASS_WADSP_FreeDSP(BASS::Wadsp);
    }
    BASS::SetMidiSoundFont(CPb::set.smidi_sf_path);
    CPb::WriteSettings();
    CSettingsDlg::OnOK();
}

void CSettingsDlg::OnBnClickedCancelbtn()
{
    CSettingsDlg::OnCancel();
}

