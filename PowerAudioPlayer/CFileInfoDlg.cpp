// CFileInfoDlg.cpp: 实现文件
//

#include "pch.h"
#include "PowerAudioPlayer.h"
#include "afxdialogex.h"
#include "CFileInfoDlg.h"


// CFileInfoDlg 对话框

IMPLEMENT_DYNAMIC(CFileInfoDlg, CDialogEx)

CFileInfoDlg::CFileInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILEINFO, pParent)
{

}

CFileInfoDlg::~CFileInfoDlg()
{
}

void CFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileInfoDlg 消息处理程序
