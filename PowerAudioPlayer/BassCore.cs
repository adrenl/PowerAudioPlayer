using PowerAudioPlayer.Properties;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using Un4seen.Bass;
using Un4seen.Bass.AddOn.Fx;
using Un4seen.Bass.AddOn.Midi;
using Un4seen.Bass.Misc;

namespace PowerAudioPlayer
{
    internal class BassCore
    {
        public float[] FREQ_TABLE = { 80f, 125f, 250f, 500f, 1000f, 1500f, 2000f, 4000f, 8000f, 1600f };

        public int Stream = 0;
        public int hSF = 0;
        public int hSync = 0;
        public List<int> BassPlugins = new List<int>();
        public BASS_CHANNELINFO ChannelInfo = new BASS_CHANNELINFO();
        public static int[] _fxEQ = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        public static DSP_Gain _gain = new DSP_Gain();
        public Visuals visModified = new Visuals();

        public void Init()
        {
            BassNet.Registration("adrenl1234@163.com", "2X39331812622");
            Bass.BASS_Init(Settings.Default.OutputDevice, Settings.Default.OutputFreq, 0, IntPtr.Zero);

            List<string> PluginsFile = Utils.FindFile(System.Windows.Forms.Application.StartupPath + "\\Plugins", "*.dll", false);
            Player.supportedExtensions.AddRange(Bass.SupportedStreamExtensions.Split(new char[] { ';' }));
            Player.supportedExtensions.AddRange(Bass.SupportedMusicExtensions.Split(new char[] { ';' }));
            foreach (string plugin in PluginsFile)
            {
                int HPlugin = Bass.BASS_PluginLoad(plugin);
                BassPlugins.Add(HPlugin);
                BASS_PLUGININFO info = Bass.BASS_PluginGetInfo(HPlugin);
                foreach (BASS_PLUGINFORM form in info.formats)
                {
                    Player.supportedExtensions.AddRange(form.exts.Split(new char[] { ';' }));
                }
            }
            Player.supportedExtensions = Player.supportedExtensions.Distinct().ToList();

            visModified.MaxFFT = BASSData.BASS_DATA_FFT1024;
            visModified.MaxFrequencySpectrum = Un4seen.Bass.Utils.FFTFrequency2Index(16000, 1024, 44100);

            SetMIDISoundFont(Settings.Default.MIDISoundFont);
        }

        public void UnInit()
        {
            Close();
            foreach (int Plugin in BassPlugins)
            {
                Bass.BASS_PluginFree(Plugin);
            }
            Bass.BASS_Stop();
            Bass.BASS_Free();
        }

        public void Open(string File)
        {
            if (Stream != 0)
            {
                Close();
            }
            try
            {
                if (Utils.IsUrl(File))
                {
                    Stream = Bass.BASS_StreamCreateURL(File, 0, Settings.Default.OtherEffectEnable ? BASSFlag.BASS_STREAM_DECODE : 0, null, 0);
                }
                else
                {
                    Stream = Bass.BASS_StreamCreateFile(File, 0, 0, Settings.Default.OtherEffectEnable ? BASSFlag.BASS_STREAM_DECODE : 0);
                }
                if (Stream == 0)
                {
                    Stream = Bass.BASS_MusicLoad(File, 0, 0, Settings.Default.OtherEffectEnable ? BASSFlag.BASS_STREAM_DECODE : 0 | BASSFlag.BASS_MUSIC_PRESCAN, Settings.Default.OutputFreq);
                }
                Bass.BASS_ChannelGetInfo(Stream, ChannelInfo);
                if (Settings.Default.OtherEffectEnable)
                {
                    Stream = BassFx.BASS_FX_ReverseCreate(Stream, 2, BASSFlag.BASS_STREAM_DECODE | BASSFlag.BASS_FX_FREESOURCE);
                    Stream = BassFx.BASS_FX_TempoCreate(Stream, BASSFlag.BASS_FX_FREESOURCE);
                    Bass.BASS_ChannelSetAttribute(Stream, BASSAttribute.BASS_ATTRIB_TEMPO, Settings.Default.OtherEffectTempo);
                    Bass.BASS_ChannelSetAttribute(Stream, BASSAttribute.BASS_ATTRIB_TEMPO_PITCH, Settings.Default.OtherEffectPitch);
                    if (Settings.Default.OtherEffectReverse)
                    {
                        Bass.BASS_ChannelSetAttribute(BassFx.BASS_FX_TempoGetSource(Stream), BASSAttribute.BASS_ATTRIB_REVERSE_DIR, -1);
                    }
                    else
                    {
                        Bass.BASS_ChannelSetAttribute(BassFx.BASS_FX_TempoGetSource(Stream), BASSAttribute.BASS_ATTRIB_REVERSE_DIR, 1);
                    }
                }
                BASS_DX8_PARAMEQ eq = new BASS_DX8_PARAMEQ();
                for (int i = 0; i < _fxEQ.Length; i++)
                {
                    _fxEQ[i] = Bass.BASS_ChannelSetFX(Stream, BASSFXType.BASS_FX_DX8_PARAMEQ, 0);
                }
                _gain = new DSP_Gain(Stream, 0);
                SetGain(Settings.Default.EQEnable ? Settings.Default.EQGain : 0);
                eq.fBandwidth = 18f;
                for (int i = 0; i < 10; i++)
                {
                    eq.fCenter = FREQ_TABLE[i];
                    eq.fGain = Convert.ToInt16(Settings.Default.EQEnable ? Settings.Default.GetType().GetProperty("EQ" + i.ToString()).GetValue(new Settings(), null) : 0);
                    Bass.BASS_FXSetParameters(_fxEQ[i], eq);
                }
            }
            catch (SEHException) { }
        }

        public void Close()
        {
            if (Stream != 0)
            {
                Bass.BASS_ChannelStop(Stream);
                Bass.BASS_ChannelFree(Stream);
                Bass.BASS_StreamFree(Stream);
                Bass.BASS_MusicFree(Stream);
                ChannelInfo = new BASS_CHANNELINFO();
                Stream = 0;
            }
        }

        public void Play()
        {
            Bass.BASS_ChannelPlay(Stream, false);
        }

        public void Pause()
        {
            Bass.BASS_ChannelPause(Stream);
        }

        public void Stop()
        {
            Bass.BASS_ChannelStop(Stream);
            Bass.BASS_ChannelSetPosition(Stream, 0);
        }

        public BASSActive IsActive()
        {
            return Bass.BASS_ChannelIsActive(Stream);
        }

        public void SetVolume(int Vol)
        {
            Bass.BASS_SetConfig(BASSConfig.BASS_CONFIG_GVOL_STREAM, Vol * 100);
            Bass.BASS_SetConfig(BASSConfig.BASS_CONFIG_GVOL_MUSIC, Vol * 100);
        }

        public int GetVolume()
        {
            return Bass.BASS_GetConfig(BASSConfig.BASS_CONFIG_GVOL_STREAM) / 100;
        }

        public long GetCurrentPosition()
        {
            return Bass.BASS_ChannelGetPosition(Stream);
        }

        public long GetLength()
        {
            return Bass.BASS_ChannelGetLength(Stream);
        }

        public double GetCurrentPosition(bool Byte2Seconds = true)
        {
            return Bass.BASS_ChannelBytes2Seconds(Stream, Bass.BASS_ChannelGetPosition(Stream));
        }

        public double GetLength(bool Byte2Seconds = true)
        {
            return Bass.BASS_ChannelBytes2Seconds(Stream, Bass.BASS_ChannelGetLength(Stream));
        }

        public double Bytes2Seconds(long pos)
        {
            return Bass.BASS_ChannelBytes2Seconds(Stream, pos);
        }

        public long Seconds2Bytes(double pos)
        {
            return Bass.BASS_ChannelSeconds2Bytes(Stream, pos);
        }

        public bool SetPosition(long pos)
        {
            return Bass.BASS_ChannelSetPosition(Stream, pos);
        }

        internal void SetMIDISoundFont(string fileName)
        {
            if (!File.Exists(fileName))
                return;
            int newfont = BassMidi.BASS_MIDI_FontInit(fileName, 0);
            if (newfont != 0)
            {
                BASS_MIDI_FONT[] sf = { new BASS_MIDI_FONT(newfont, -1, 0) };
                BassMidi.BASS_MIDI_StreamSetFonts(0, sf, 1);
                BassMidi.BASS_MIDI_StreamSetFonts(Stream, sf, 1);
                BassMidi.BASS_MIDI_FontFree(hSF);
                hSF = newfont;
            }
        }

        public void UpdateEQ(int band, float gain)
        {
            BASS_DX8_PARAMEQ eq = new BASS_DX8_PARAMEQ();
            if (Bass.BASS_FXGetParameters(_fxEQ[band], eq))
            {
                eq.fGain = gain;
                Bass.BASS_FXSetParameters(_fxEQ[band], eq);
            }
        }

        public void SetGain(double v)
        {
            if (_gain != null)
            {
                try
                {
                    double gainDB = v;
                    if (gainDB == 0.0)
                        _gain.SetBypass(true);
                    else
                    {
                        _gain.SetBypass(false);
                        _gain.Gain_dBV = gainDB;
                    }
                }
                catch { }
            }
        }

        public void SetEndSync(SYNCPROC sYNCPROC)
        {
            hSync = Bass.BASS_ChannelSetSync(Stream, BASSSync.BASS_SYNC_END, 0, sYNCPROC, IntPtr.Zero);
        }

        public void RemoveSync()
        {
            Bass.BASS_ChannelRemoveSync(Stream, hSync);
        }
    }
}
