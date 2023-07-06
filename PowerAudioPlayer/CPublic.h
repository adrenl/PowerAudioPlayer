#pragma once
#define PATH_INI ".\PowerAudioPlayer.ini";
#define TIMER_ALAWAYS 1000
#define TIMER_PLAYING 1001
#define TIMER_SFX	  1002
#include <vector>
#include <string>
#include <fstream>
#include <exception>

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
	static CString SFF;
	static std::vector<CString> pl_path;
	static std::vector<CString> pl_title;
	static std::vector<int> pl_time;
	static std::vector<bool> pl_isconvert;
	static std::vector<CString> SFXs;
	static int SFXId;
	static int split(const CString strLine, char split, CStringArray& strArray);
	static CString i2cs(QWORD var);
	static CString GetExeModuleDir();
	static CString GetInPathFileName(CString Path);
	static CString CharToCStr(char* CharStr);
	static const char* CStrToChar(CString CStr);
	static LPCWSTR CharToLPCWSTR(char* szStr);
};
