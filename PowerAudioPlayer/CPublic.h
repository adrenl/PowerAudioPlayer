#pragma once
#define PATH_INI ".\PowerAudioPlayer.ini";
#define TIMER_ALAWAYS 100
#define TIMER_PLAYING 101
#include <vector>
#include <string>
#include <fstream>

/*struct Settings {
	CString midi_path;
	CString skin_path;
};*/

class CPb
{
public:
	CPb();
	virtual ~CPb();
	//static Settings set;
	static bool ToConvertList;
	static int PlayId;
	static std::vector<CString> pl_path;
	static std::vector<CString> pl_title;
	static std::vector<int> pl_time;
	static std::vector<bool> pl_isconvert;
	static int split(const CString strLine, char split, CStringArray& strArray);
	static CString i2cs(QWORD var);
	static CString GetExeModuleDir();
	static CString GetInPathFileName(CString Path);
	static CString CharToCStr(char* CharStr);
	static const char* CStrToChar(CString CStr);
	static LPCWSTR CharToLPCWSTR(char* szStr);
};
