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

}

CSFXDlg::~CSFXDlg()
{
}

void CSFXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSFXDlg, CDialogEx)
END_MESSAGE_MAP()


// CSFXDlg 消息处理程序


void CSFXDlg::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	//delete this;
}


void CSFXDlg::OnCancel()
{
	DestroyWindow();
	//CDialogEx::OnCancel();
}
