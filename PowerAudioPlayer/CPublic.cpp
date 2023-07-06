#include "pch.h"
#include "CPublic.h"
#include "pch.h"
bool CPb::ToConvertList = FALSE;
int CPb::PlayId = -1;
CString CPb::SFF = _T("");
std::vector<CString> CPb::pl_path;
std::vector<CString> CPb::pl_title;
std::vector<int> CPb::pl_time;
std::vector<bool> CPb::pl_isconvert;
std::vector<CString> CPb::SFXs;
int CPb::SFXId = 0;
CPb::CPb()
{
}

CPb::~CPb()
{
}

int CPb::split(const CString strLine, char split, CStringArray& strArray)
{
	strArray.RemoveAll();//自带清空属性
	CString temp = strLine;
	int tag = 0;
	while (1)
	{
		tag = temp.Find(split);
		if (tag >= 0)
		{
			strArray.Add(temp.Left(tag));
			temp = temp.Right(temp.GetLength() - tag - 1);
		}
		else { break; }
	}
	strArray.Add(temp);
	return strArray.GetSize();
}

CString CPb::i2cs(QWORD var)
{
	CString Str;
	Str.Format(_T("%d"), var);
	return Str;
}

CString CPb::GetExeModuleDir()
{
	CString  strProgramPath;
	GetModuleFileName(NULL, strProgramPath.GetBuffer(MAX_PATH), MAX_PATH);
	strProgramPath.ReleaseBuffer(MAX_PATH);
	int nPathPos = strProgramPath.ReverseFind('\\');
	strProgramPath = strProgramPath.Left(nPathPos + 0);
	return strProgramPath;
}

CString CPb::GetInPathFileName(CString Path)
{
	return Path.Right(Path.GetLength() - Path.ReverseFind('\\') - 1);
}

CString CPb::CharToCStr(char* CharStr)
{
	CString Result;
	Result.Format(_T("%s"), CharStr);
	return Result;
}

const char* CPb::CStrToChar(CString CStr)
{
	const char* p = CStringA(CStr);
	return p;
}
LPCWSTR CPb::CharToLPCWSTR(char* szStr)
{
	CString str = CString(szStr);
	USES_CONVERSION;
	LPCWSTR wszClassName = new WCHAR[str.GetLength() + 1];
	wcscpy((LPTSTR)wszClassName, T2W((LPTSTR)str.GetBuffer(NULL)));
	str.ReleaseBuffer();
	return wszClassName;
}
