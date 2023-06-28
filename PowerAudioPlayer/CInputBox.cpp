// CInputBox.cpp: 实现文件
//

#include "pch.h"
#include "PowerAudioPlayer.h"
#include "afxdialogex.h"
#include "CInputBox.h"


// CInputBox 对话框

IMPLEMENT_DYNAMIC(CInputBox, CDialogEx)

CInputBox::CInputBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUTBOX, pParent)
{

}

CInputBox::~CInputBox()
{
}

void CInputBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_descp);
	DDX_Control(pDX, IDC_EDIT2, m_input);
}


BEGIN_MESSAGE_MAP(CInputBox, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputBox::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInputBox::OnBnClickedCancel)
END_MESSAGE_MAP()


// CInputBox 消息处理程序


BOOL CInputBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CInputBox::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CInputBox::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}