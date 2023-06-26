#include "pch.h"
#include "CGlobal.h"
HSTREAM CGlobal::BASS = NULL;
int CGlobal::PlayId = -1;
int CGlobal::Length;
std::vector<CString> CGlobal::pl_path;
std::vector<CString> CGlobal::pl_title;
std::vector<int> CGlobal::pl_time;
std::vector<bool> CGlobal::pl_isconvert;
CGlobal::CGlobal()
{
}

CGlobal::~CGlobal()
{
}

int CGlobal::split(const CString strLine, char split, CStringArray& strArray)
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

CString CGlobal::to_string(int var)
{
	CString Str;
	Str.Format(_T("%d"), var);
	return Str;
}

CString CGlobal::GetExeModuleDir()
{
	CString  strProgramPath;
	GetModuleFileName(NULL, strProgramPath.GetBuffer(MAX_PATH), MAX_PATH);
	strProgramPath.ReleaseBuffer(MAX_PATH);
	int nPathPos = strProgramPath.ReverseFind('\\');
	strProgramPath = strProgramPath.Left(nPathPos + 0);
	return strProgramPath;
}

CString CGlobal::CharToCStr(char *CharStr)
{
	CString Result;
	Result.Format(_T("%s"), CharStr);
	return Result;
}

const char* CGlobal::CStrToChar(CString CStr)
{
	const char* p = CStringA(CStr);
	return p;
}
double CGlobal::GetLengthByBassStream(HSTREAM Stream)
{
	return BASS_ChannelGetLength(Stream, BASS_POS_BYTE);
}
double CGlobal::GetLengthByBassFile(CString File)
{
	HSTREAM Stream = BASS_StreamCreateFile(FALSE, File, 0, 0, BASS_SAMPLE_FLOAT);
	double Time = CGlobal::GetLengthByBassStream(Stream);
	BASS_StreamFree(Stream);
	return Time;
}
CString CGlobal::BassGetTimeToString(int Time)
{
	CString Time1;
	CString Time2;
	if (Time / 60 < 10) {
		Time1 = _T("0") + CGlobal::to_string(Time / 60);
	}else {
		Time1= CGlobal::to_string(Time / 60);
	}
	if (Time % 60 < 10) {
		Time2=_T("0")+ CGlobal::to_string(Time % 60);
	}else {
		Time2=CGlobal::to_string(Time % 60);
	}
	return Time1 + ":" + Time2;
}
