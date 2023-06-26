#pragma once
#include <vector>
#define PATH_INI ".\PowerAudioPlayer.ini";
#define TIMER_ALAWAYS 100
#define TIMER_PLAYING 101
class CGlobal
{
	public:
		CGlobal();
		virtual ~CGlobal();
	public:
		static HSTREAM BASS;
		static int PlayId;
		static int Length;
		static std::vector<CString> pl_path;
		static std::vector<CString> pl_title;
		static std::vector<int> pl_time;
		static std::vector<bool> pl_isconvert;
		static int split(const CString strLine, char split, CStringArray& strArray);
		static CString to_string(int var);
		static CString GetExeModuleDir();
		static CString CharToCStr(char *CharStr);
		static const char* CStrToChar(CString CStr);
		static double GetLengthByBassStream(HSTREAM Stream);
		static double GetLengthByBassFile(CString File);
		static CString BassGetTimeToString(int Time);
};