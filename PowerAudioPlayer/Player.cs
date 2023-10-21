using PowerAudioPlayer.Properties;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Media;
using Un4seen.Bass;

namespace PowerAudioPlayer
{
    public enum PlayMode
    {
        OrderPlay,
        TrackLoop,
        ListLoop,
        RamdonPlay
    }

    public class AudioInfo
    {
        public AudioInfo(string file = "")
        {
            File = file;
        }

        public string File { get; set; } = "";

        public string Title { get; set; } = "";

        public string Album { get; set; } = "";

        public string Artist { get; set; } = "";

        public string Genre { get; set; } = "";

        public string Comment { get; set; } = "";

        public int Track { get; set; } = 0;

        public int Year { get; set; } = 0;

        public int SampleRate { get; set; } = 0;

        public int BitRate { get; set; } = 0;

        public int Channel { get; set; } = 0;

        public double Length { get; set; } = 0;

        public bool IsInfoAcquired { get; set; } = false;

        public bool IsTagNull { get; set; } = false;
    }


    static class Player
    {
        public const int WM_SETTINGSCHANGE = 0x400 + 1;

        public static BassCore bassCore = new BassCore();
        public static List<string> supportedExtensions = new List<string>();
        public static string playFile = "";
        public static int playIndex = -1;
        public static PlayMode playMode = PlayMode.OrderPlay;

        public static ResourceDictionary stringDictionary = new ResourceDictionary();
        public static ResourceDictionary imageDictionary = new ResourceDictionary();

        public static void Init()
        {
            AudioInfoDataHelper.LoadAudioInfoData();
            PlayListHelper.Load();
            bassCore.Init();
        }

        public static void UnInit()
        {
            AudioInfoDataHelper.CleanUp();
            AudioInfoDataHelper.SaveAudioInfoData();
            PlayListHelper.Save();
            bassCore.UnInit();
        }

        public static string GetStr(string key)
        {
            return (string)Application.Current.FindResource(key);
        }

        public static ImageSource GetImg(string key)
        {
            return (ImageSource)Application.Current.FindResource(key);
        }        
        
        public static DrawingImage GetDrawingImg(string key)
        {
            return (DrawingImage)Application.Current.FindResource(key);
        }

        public static string GetFileFilter()
        {
            string SupportedFileFilterAll = GetStr("FilterSupportedFile") + "|" + string.Join(';', supportedExtensions);
            string SupportedFiltFilter = Bass.SupportedStreamName + "|" + Bass.SupportedStreamExtensions + "|Module Music|" + Bass.SupportedMusicExtensions + "|";
            foreach (int plugin in bassCore.BassPlugins)
            {
                BASS_PLUGININFO info = Bass.BASS_PluginGetInfo(plugin);
                foreach (BASS_PLUGINFORM form in info.formats)
                {
                    if (info.formatc >= 16)
                        continue;
                    SupportedFiltFilter += form.ToString() + "|";
                }
            }
            return SupportedFileFilterAll + "|" + SupportedFiltFilter + GetStr("FilterAllFile") + "|*.*";
        }

        public static void SetPlayMode(PlayMode mode)
        {
            Settings.Default.PlayMode = (int)mode;
            playMode = mode;
        }
    }
}