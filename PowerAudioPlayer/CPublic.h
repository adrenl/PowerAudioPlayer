#pragma once
#define SET					"Settings.json"
#define VER_VERSION			"0.1"
#define VER_NAME			"PowerAudioPlayer"
#define TIMER_ALAWAYS		1000
#define TIMER_PLAYING		1001
#define TIMER_SFX			1002
#define TASKBARBTN_PLAY		2000
#define TASKBARBTN_STOP		2001
#define TASKBARBTN_BACK		2002
#define TASKBARBTN_NEXT		2003
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include <strsafe.h>

struct Settings {
	int vol;
	int is_mute;
	int pl_location;
	int sfx_id;

	bool smain_rem_pl_location;
	bool smain_allow_drag;
	CString main_skin_path;
	int smain_sfx_render_elapse;
	//---
	CString spl_title_format;
	bool spl_show_snum;
	//---
	CString smidi_sf_path;
	//---
};

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
	static Settings set;
	static int split(const CString strLine, char split, CStringArray& strArray);
	static CString i2cs(QWORD var);
	static CString GetExeModuleDir();
	static CString GetInPathFileName(CString Path);
	static CString CharToCStr(char* CharStr);
	static const char* CStrToChar(CString CStr);
	static LPCWSTR CharToLPCWSTR(char* szStr);
};
