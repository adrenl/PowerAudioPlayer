#include "pch.h"
#include "CPublic.h"
#include "pch.h"
int CPb::length = 0;
bool CPb::ToConvertList = FALSE;
int CPb::PlayId = -1;
CString CPb::SFF = _T("");
std::vector<CString> CPb::pl_path;
std::vector<CString> CPb::pl_title;
std::vector<int> CPb::pl_time;
std::vector<bool> CPb::pl_isconvert;
int CPb::pl_totaltime = 0;
std::vector<CString> CPb::SFXs;
std::vector<CString> CPb::DSPs;
std::vector<CString> CPb::support_exts = {_T("*.mp3"), _T("*.mp2"), _T("*.mp1"), _T("*.ogg"), _T("*.wav"), _T("*.mp3"), _T("*.aiff") };
Settings CPb::set;
CPb::CPb()
{
}

CPb::~CPb()
{
}

int CPb::split(const CString strLine, char split, CStringArray &strArray)
{
    strArray.RemoveAll();
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
        else
        {
            break;
        }
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
    if (IsUrl(Path))
    {
        return Path.Right(Path.GetLength() - Path.ReverseFind('/') - 1);
    }
    else
    {
        return Path.Right(Path.GetLength() - Path.ReverseFind('\\') - 1);
    }
}

CString CPb::CharToCStr(char *CharStr)
{
    CString Result;
    Result.Format(_T("%s"), CharStr);
    return Result;
}

const char *CPb::CStrToChar(CString CStr)
{
    const char *p = CStringA(CStr);
    return p;
}
LPCWSTR CPb::CharToLPCWSTR(char *szStr)
{
    CString str = CString(szStr);
    USES_CONVERSION;
    LPCWSTR wszClassName = new WCHAR[str.GetLength() + 1];
    wcscpy((LPTSTR)wszClassName, T2W((LPTSTR)str.GetBuffer(NULL)));
    str.ReleaseBuffer();
    return wszClassName;
}

char* CPb::replace_str(char* text, char sp_ch, char re_ch)
{
    int len = strlen(text);
    char* copy = (char*)malloc(len + 1);

    for (int i = 0; i < len; i++)
    {
        char ch = text[i];
        if (ch == sp_ch)
            copy[i] = re_ch;
        else
            copy[i] = ch;
    }
    copy[len] = 0;
    strcpy(text, copy);
    free(copy);
    return text;
}

bool CPb::IsUrl(CString str)
{
    if (str.Left(7) == _T("http://") || str.Left(8) == _T("https://") || str.Left(6) == _T("ftp://"))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CPb::ReadSettings()
{
    CString path = CPb::GetExeModuleDir() + _T("\\") + _T(SET);
    Json::Reader reader;
    Json::Value root;
    std::ifstream in(path, std::ios::binary);
    if (reader.parse(in, root))
    {
        set.vol = root["vol"].asInt();
        set.is_mute = root["is_mute"].asBool();
        set.pl_location = root["pl_location"].asInt();
        set.sfx_id = root["sfx_id"].asInt();
        set.dsp_id = root["dsp_id"].asInt();
        set.playmode = root["playmode"].asInt();
        set.smain_rem_pl_location = root["smain_rem_pl_location"].asBool();
        set.smain_sfx_render_elapse = root["smain_sfx_render_elapse"].asInt();
        set.spl_title_format = (CString)root["spl_title_format"].asCString();
        set.spl_show_snum = root["spl_show_snum"].asBool();
        set.smidi_sf_path = (CString)root["smidi_sf_path"].asCString();
    }
    in.close();
}

void CPb::WriteSettings()
{
    CString path = CPb::GetExeModuleDir() + _T("\\") + _T(SET);
    Json::Value root;
    Json::StyledWriter sw;
    root["vol"] = Json::Value(set.vol);
    root["is_mute"] = Json::Value(set.is_mute);
    root["pl_location"] = Json::Value(set.pl_location);
    root["sfx_id"] = Json::Value(set.sfx_id);
    root["dsp_id"] = Json::Value(set.dsp_id);
    root["playmode"] = Json::Value(set.playmode);
    root["smain_rem_pl_location"] = Json::Value(set.smain_rem_pl_location);
    root["smain_sfx_render_elapse"] = Json::Value(set.smain_sfx_render_elapse);
    root["spl_title_format"] = Json::Value(CPb::CStrToChar(set.spl_title_format));
    root["spl_show_snum"] = Json::Value(set.spl_show_snum);
    root["smidi_sf_path"] = Json::Value(CPb::CStrToChar(set.smidi_sf_path));
    std::ofstream os;
    os.open(path, std::ios::out);
    os << sw.write(root);
    os.close();
}

int CPb::GetRand(int min, int max)
{     
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

bool CPb::IsInVectorCString(std::vector<CString> vtr, CString Find)
{
    for (int l = 0; l < vtr.size(); ++l)
    {
        if (vtr[l] == Find)
        {
            return TRUE;
        }
    }
    return FALSE;
}
