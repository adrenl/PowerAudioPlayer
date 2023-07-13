#pragma once
class BASS
{
public:
    BASS();
    virtual ~BASS();
    static HSTREAM Stream;
    static HSOUNDFONT SoundFont;
    static HWADSP Wadsp;
    static std::vector<HPLUGIN> PLUGINS;
    static DWORD GetConfig(DWORD option);
    static const void *GetConfigPtr(DWORD option);
    static BOOL SetConfig(DWORD option, DWORD value);
    static BOOL SetConfigPtr(DWORD option, void *value);
    static void PluginLoad(const char *file, DWORD flags);
    static const BASS_PLUGININFO *PluginGetInfo(int id);
    static void PluginFree(int id);
    static void PluginEnable(int id, BOOL enable);
    static int ErrorGetCode();
    static BOOL Free();
    static float GetCPU();
    static DWORD GetDevice();
    static BOOL GetDeviceInfo(DWORD device, BASS_DEVICEINFO *info);
    static BOOL GetInfo(BASS_INFO *info);
    static DWORD GetVersion();
    static float GetVolume();
    static BOOL Init(int device, DWORD freq, DWORD flags, HWND win, void *clsid);
    static DWORD IsStarted();
    static BOOL Pause();
    static BOOL SetDevice(DWORD device);
    static BOOL SetVolume(float volume);
    static BOOL Start();
    static BOOL Stop();
    static BOOL Update(DWORD length);
    static void StreamCreate(DWORD freq, DWORD chans, DWORD flags, STREAMPROC *proc, void *user);
    static void StreamCreateFile(BOOL mem, CString file, QWORD offset, QWORD length, DWORD flags);
    static void StreamCreateFileUser(DWORD system, DWORD flags, BASS_FILEPROCS *procs, void *user);
    static void StreamCreateURL(char *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, void *user);
    static BOOL StreamFree();
    static QWORD StreamGetFilePosition(DWORD mode);
    static double ChannelBytes2Seconds(QWORD pos);
    static BOOL ChannelFree();
    static QWORD ChannelGetLength(DWORD mode);
    static DWORD ChannelGetLevel();
    static BOOL ChannelGetLevelEx(float *levels, float length, DWORD flags);
    static QWORD ChannelGetPosition(DWORD mode);
    static BOOL ChannelGetInfo(BASS_CHANNELINFO* info);
    static DWORD ChannelIsActive();
    static BOOL ChannelPause();
    static BOOL ChannelPlay(BOOL restart);
    static QWORD ChannelSeconds2Bytes(double pos);
    static BOOL ChannelStart();
    static BOOL ChannelStop();
    static BOOL ChannelUpdate(DWORD length);
    static BOOL ChannelSetPosition(QWORD pos, DWORD mode);
    //
    static void SetMidiSoundFont(CString file);
    static CString TimeToString(QWORD time);
    static QWORD LengthFile(CString file, bool byte2sec = TRUE);
private:
    static CString i2cs(QWORD val);

};