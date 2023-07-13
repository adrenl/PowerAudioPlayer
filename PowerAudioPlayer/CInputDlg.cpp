// CInputDlg.cpp: 实现文件
//

#include "pch.h"
#include "PowerAudioPlayer.h"
#include "afxdialogex.h"
#include "CInputDlg.h"
CString CInputDlg::Describe;
CString CInputDlg::Default;
CString CInputDlg::IfIsNull;
bool CInputDlg::ResB;
CString CInputDlg::ResS;

// CInputDlg 对话框

IMPLEMENT_DYNAMIC(CInputDlg, CDialogEx)

CInputDlg::CInputDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_INPUTDLG, pParent)
{

}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_describe);
    DDX_Control(pDX, IDC_EDIT2, m_input);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CInputDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CInputDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CInputDlg 消息处理程序



BOOL CInputDlg::InputBox(CString Describe, CString Default, CString IfIsNull, CString &rString)
{
    CInputDlg Dlg;
    Dlg.Default = Default;
    Dlg.Describe = Describe;
    Dlg.IfIsNull = IfIsNull;
    Dlg.DoModal();
    rString = Dlg.ResS;
    return Dlg.ResB;
}

BOOL CInputDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_input.SetWindowTextW(Default);
    m_describe.SetWindowTextW(Describe);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CInputDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    ResB = TRUE;
    m_input.GetWindowTextW(ResS);
    if (ResS == _T(""))
    {
        ResS = IfIsNull;
    }
    CDialogEx::OnOK();
}


void CInputDlg::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    ResB = FALSE;
    CDialogEx::OnCancel();
}
