#include "pch.h"
#include "CBASS.h"
HSTREAM BASS::Stream = NULL;

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

const void* BASS::GetConfigPtr(DWORD option)
{
	return BASS_GetConfigPtr(option);
}

BOOL BASS::SetConfig(DWORD option, DWORD value)
{
	return BASS_SetConfig(option,value);
}

BOOL BASS::SetConfigPtr(DWORD option, void* value)
{
	return  BASS_SetConfigPtr(option,value);
}

HPLUGIN BASS::PluginLoad(char* file, DWORD flags)
{
	return BASS_PluginLoad(file, flags);
}

const BASS_PLUGININFO* BASS::PluginGetInfo(HPLUGIN handle)
{
	return BASS_PluginGetInfo(handle);
}

BOOL BASS::PluginFree(HPLUGIN handle)
{
	return BASS_PluginFree(handle);
}

BOOL BASS::PluginEnable(HPLUGIN handle, BOOL enabl)
{
	return BASS_PluginEnable(handle,enabl);
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

BOOL BASS::GetDeviceInfo(DWORD device, BASS_DEVICEINFO* info)
{
	return BASS_GetDeviceInfo(device,info);
}

BOOL BASS::GetInfo(BASS_INFO* info)
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

BOOL BASS::Init(int device, DWORD freq, DWORD flags, HWND win, void* clsid)
{
	return BASS_Init(device,freq,flags,win,clsid);
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

HSTREAM BASS::StreamCreate(DWORD freq, DWORD chans, DWORD flags, STREAMPROC* proc, void* user)
{
	return BASS::Stream = BASS_StreamCreate(freq,chans,flags,proc,user);
}

HSTREAM BASS::StreamCreateFile(BOOL mem, CString file, QWORD offset, QWORD length, DWORD flags)
{
	return BASS::Stream = BASS_StreamCreateFile(mem,file,offset,length,flags);
}

HSTREAM BASS::StreamCreateFileUser(DWORD system, DWORD flags, BASS_FILEPROCS* procs, void* user)
{
	return BASS::Stream = BASS_StreamCreateFileUser(system,flags,procs,user);
}

HSTREAM BASS::StreamCreateURL(char* url, DWORD offset, DWORD flags, DOWNLOADPROC* proc, void* user)
{
	return BASS::Stream = BASS_StreamCreateURL(url,offset,flags,proc,user);
}

BOOL BASS::StreamFree()
{
	return BASS_StreamFree(BASS::Stream);
}

QWORD BASS::StreamGetFilePosition(DWORD mode)
{
	return BASS_StreamGetFilePosition(BASS::Stream,mode);
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
	return BASS_ChannelGetLength(BASS::Stream,mode);
}

DWORD BASS::ChannelGetLevel()
{
	return BASS_ChannelGetLevel(BASS::Stream);
}

BOOL BASS::ChannelGetLevelEx(float* levels, float length, DWORD flags)
{
	return BASS_ChannelGetLevelEx(BASS::Stream,levels,length,flags);
}

QWORD BASS::ChannelGetPosition(DWORD mode)
{
	return BASS_ChannelGetPosition(BASS::Stream, mode);
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
	return BASS_ChannelPlay(BASS::Stream,restart);
}

QWORD BASS::ChannelSeconds2Bytes(double pos)
{
	return BASS_ChannelSeconds2Bytes(BASS::Stream,pos);
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
	return BASS_ChannelUpdate(BASS::Stream,length);
}

BOOL BASS::ChannelSetPosition(QWORD pos, DWORD mode) {
	return BASS_ChannelSetPosition(BASS::Stream, pos, mode);
}
//
CString BASS::i2cs(QWORD val)
{
	CString Str;
	Str.Format(_T("%d"), val);
	return Str;
}

CString BASS::TimeToString(QWORD time) //Time:Need BASS_ChannelBytes2Seconds()
{
	CString Time1;
	CString Time2;
	if (time / 60 < 10) {
		Time1 = _T("0") + BASS::i2cs(time / 60);
	}
	else {
		Time1 = BASS::i2cs(time / 60);
	}
	if (time % 60 < 10) {
		Time2 = _T("0") + BASS::i2cs(time % 60);
	}
	else {
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
