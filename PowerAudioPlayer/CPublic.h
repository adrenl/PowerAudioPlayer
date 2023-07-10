#pragma once
#define SET					"settings.json"
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
#include <random>

struct Settings
{
    int vol = 50;
    bool is_mute = FALSE;
    int pl_location = -1;
    int sfx_id = -1;
    int dsp_id = -1;
    int playmode = 0;

    bool smain_rem_pl_location = TRUE;
    bool smain_allow_drag = TRUE;
    int smain_sfx_render_elapse = 20;
    //---
    CString spl_title_format =_T("%IFV2(%TITL,%IFV2(%TITL,%TITL,无标题) - %IFV2(%ARTI,%ARTI,无艺术家)%IFV1(%ALBM, - %ALBM),%FILE)");
    bool spl_show_snum = TRUE;
    //---
    CString smidi_sf_path = _T("");
    //---
};

class CPb
{
public:
    CPb();
    virtual ~CPb();
    static int length;
    static bool ToConvertList;
    static int PlayId;
    static CString SFF;
    static std::vector<CString> pl_path;
    static std::vector<CString> pl_title;
    static std::vector<int> pl_time;
    static std::vector<bool> pl_isconvert;
    static std::vector<CString> SFXs;
    static std::vector<CString> DSPs;
    static Settings set;
    static int split(const CString strLine, char split, CStringArray &strArray);
    static CString i2cs(QWORD var);
    static CString GetExeModuleDir();
    static CString GetInPathFileName(CString Path);
    static CString CharToCStr(char *CharStr);
    static const char *CStrToChar(CString CStr);
    static LPCWSTR CharToLPCWSTR(char *szStr);
    static char* replace_str(char* text, char sp_ch, char re_ch);
    static bool IsUrl(CString str);
    static void ReadSettings();
    static void WriteSettings();
    static int GetRand(int MIN, int MAX);
};
