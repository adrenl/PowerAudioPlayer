#include "pch.h"
#include "CBASS.h"
HSTREAM BASS::Stream = NULL;
HSOUNDFONT BASS::SoundFont = NULL;
HWADSP BASS::Wadsp = NULL;
std::vector<HPLUGIN> BASS::PLUGINS;

BASS::BASS()
{
}

BASS::~BASS()
{
}

//

DWORD BASS::GetConfig(DWORD option)
{
    return BASS_GetConfig(option);
}

const void *BASS::GetConfigPtr(DWORD option)
{
    return BASS_GetConfigPtr(option);
}

BOOL BASS::SetConfig(DWORD option, DWORD value)
{
    return BASS_SetConfig(option, value);
}

BOOL BASS::SetConfigPtr(DWORD option, void *value)
{
    return  BASS_SetConfigPtr(option, value);
}

void BASS::PluginLoad(const char *file, DWORD flags)
{
    BASS::PLUGINS.push_back(BASS_PluginLoad(file, flags));
}

const BASS_PLUGININFO *BASS::PluginGetInfo(int id)
{
    return BASS_PluginGetInfo(BASS::PLUGINS[id]);
}

void BASS::PluginFree(int id)
{
    BASS_PluginFree(BASS::PLUGINS[id]);
    BASS::PLUGINS.erase(BASS::PLUGINS.begin()  + id);
}

void BASS::PluginEnable(int id, BOOL enabl)
{
    BASS_PluginEnable(BASS::PLUGINS[id], enabl);
}

int BASS::ErrorGetCode()
{
    return BASS_ErrorGetCode();
}

BOOL BASS::Free()
{
    return BASS_Free();
}

float BASS::GetCPU()
{
    return BASS_GetCPU();
}

DWORD BASS::GetDevice()
{
    return BASS_GetDevice();
}

BOOL BASS::GetDeviceInfo(DWORD device, BASS_DEVICEINFO *info)
{
    return BASS_GetDeviceInfo(device, info);
}

BOOL BASS::GetInfo(BASS_INFO *info)
{
    return BASS_GetInfo(info);
}

DWORD BASS::GetVersion()
{
    return BASS_GetVersion();
}

float BASS::GetVolume()
{
    return BASS_GetVolume();
}

BOOL BASS::Init(int device, DWORD freq, DWORD flags, HWND win, void *clsid)
{
    return BASS_Init(device, freq, flags, win, clsid);
}

DWORD BASS::IsStarted()
{
    return BASS_IsStarted();
}

BOOL BASS::Pause()
{
    return BASS_Pause();
}

BOOL BASS::SetDevice(DWORD device)
{
    return BASS_SetDevice(device);
}

BOOL BASS::SetVolume(float volume)
{
    return BASS_SetVolume(volume);
}

BOOL BASS::Start()
{
    return BASS_Start();
}

BOOL BASS::Stop()
{
    return BASS_Stop();
}

BOOL BASS::Update(DWORD length)
{
    return BASS_Update(length);
}

void BASS::StreamCreate(DWORD freq, DWORD chans, DWORD flags, STREAMPROC *proc, void *user)
{
    BASS::Stream = BASS_StreamCreate(freq, chans, flags, proc, user);
}

void BASS::StreamCreateFile(BOOL mem, CString file, QWORD offset, QWORD length, DWORD flags)
{
    BASS::Stream = BASS_StreamCreateFile(mem, file, offset, length, flags);
}

void BASS::StreamCreateFileUser(DWORD system, DWORD flags, BASS_FILEPROCS *procs, void *user)
{
    BASS::Stream = BASS_StreamCreateFileUser(system, flags, procs, user);
}

void BASS::StreamCreateURL(char *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, void *user)
{
    BASS::Stream = BASS_StreamCreateURL(url, offset, flags, proc, user);
}

BOOL BASS::StreamFree()
{
    return BASS_StreamFree(BASS::Stream);
}

QWORD BASS::StreamGetFilePosition(DWORD mode)
{
    return BASS_StreamGetFilePosition(BASS::Stream, mode);
}

double BASS::ChannelBytes2Seconds(QWORD pos)
{
    return BASS_ChannelBytes2Seconds(BASS::Stream, pos);
}

BOOL BASS::ChannelFree()
{
    return BASS_ChannelFree(BASS::Stream);
}

QWORD BASS::ChannelGetLength(DWORD mode)
{
    return BASS_ChannelGetLength(BASS::Stream, mode);
}

DWORD BASS::ChannelGetLevel()
{
    return BASS_ChannelGetLevel(BASS::Stream);
}

BOOL BASS::ChannelGetLevelEx(float *levels, float length, DWORD flags)
{
    return BASS_ChannelGetLevelEx(BASS::Stream, levels, length, flags);
}

QWORD BASS::ChannelGetPosition(DWORD mode)
{
    return BASS_ChannelGetPosition(BASS::Stream, mode);
}

BOOL BASS::ChannelGetInfo(BASS_CHANNELINFO* info)
{
    return BASS_ChannelGetInfo(BASS::Stream, info);
}

DWORD BASS::ChannelIsActive()
{
    return BASS_ChannelIsActive(BASS::Stream);
}

BOOL BASS::ChannelPause()
{
    return BASS_ChannelPause(BASS::Stream);
}

BOOL BASS::ChannelPlay(BOOL restart)
{
    return BASS_ChannelPlay(BASS::Stream, restart);
}

QWORD BASS::ChannelSeconds2Bytes(double pos)
{
    return BASS_ChannelSeconds2Bytes(BASS::Stream, pos);
}

BOOL BASS::ChannelStart()
{
    return BASS_ChannelStart(BASS::Stream);
}

BOOL BASS::ChannelStop()
{
    return BASS_ChannelStop(BASS::Stream);
}

BOOL BASS::ChannelUpdate(DWORD length)
{
    return BASS_ChannelUpdate(BASS::Stream, length);
}

BOOL BASS::ChannelSetPosition(QWORD pos, DWORD mode)
{
    return BASS_ChannelSetPosition(BASS::Stream, pos, mode);
}

//
CString BASS::i2cs(QWORD val)
{
    CString Str;
    Str.Format(_T("%d"), val);
    return Str;
}

void BASS::SetMidiSoundFont(CString file)
{
    HSOUNDFONT newfont = BASS_MIDI_FontInit(file, 0);
    if (newfont)
    {
        BASS_MIDI_FONT sf{};
        sf.font = newfont;
        sf.preset = -1;
        sf.bank = 0;
        BASS_MIDI_StreamSetFonts(0, &sf, 1);
        BASS_MIDI_StreamSetFonts(BASS::Stream, &sf, 1);
        BASS_MIDI_FontFree(SoundFont);
        SoundFont = newfont;
    }
}

CString BASS::TimeToString(QWORD time) //Time:Need BASS_ChannelBytes2Seconds()
{
    CString Time1;
    CString Time2;
    if (time / 60 < 10)
    {
        Time1 = _T("0") + BASS::i2cs(time / 60);
    }
    else
    {
        Time1 = BASS::i2cs(time / 60);
    }
    if (time % 60 < 10)
    {
        Time2 = _T("0") + BASS::i2cs(time % 60);
    }
    else
    {
        Time2 = BASS::i2cs(time % 60);
    }
    return Time1 + ":" + Time2;
}

QWORD BASS::LengthFile(CString file, bool byte2sec)
{
    QWORD time = 0;
    HSTREAM STREAM = BASS_StreamCreateFile(FALSE, file, NULL, NULL, NULL);
    time = BASS_ChannelGetLength(STREAM, NULL);
    if (byte2sec) time = BASS_ChannelBytes2Seconds(Stream, time);
    BASS_StreamFree(STREAM);
    return time;
}