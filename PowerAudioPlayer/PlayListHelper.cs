using Newtonsoft.Json;
using PowerAudioPlayer.Properties;
using System.Collections.Generic;
using System.IO;
using Un4seen.Bass;

namespace PowerAudioPlayer
{
    public enum DisplayTitleFormat
    {
        FileName,
        Title,
        ArtistTitle,
        TitleArtist
    };

    public class PlayListItem
    {
        public string File { get; set; } = "";

        public override string ToString()
        {
            return PlayListHelper.GetDisplayTitle(AudioInfoDataHelper.Get(File));
        }

        public PlayListItem(string file)
        {
            File = file;
        }
    }

    public class PlayList
    {
        public string Name { get; set; } = "";

        public bool IsDefault { get; set; } = false;

        public List<PlayListItem> Items = new List<PlayListItem>();

        public override string ToString()
        {
            return Name;
        }
    }

    internal static class PlayListHelper
    {
        public readonly static string PlayListDir = System.Windows.Forms.Application.StartupPath + "\\PlayList";
        public static List<PlayList> playLists = new List<PlayList>();
        private static int current = 0;

        public static int Current
        {
            get { return current; }
            set { current = Settings.Default.CurrentPlayList = value; }
        }

        public static void Load()
        {
            List<string> files = Utils.FindFile(PlayListDir, "*.json", false);
            foreach (string file in files)
            {
                StreamReader streamReader = new StreamReader(file);
                string? strLine, playListJson = "";
                while ((strLine = streamReader.ReadLine()) != null)
                {
                    playListJson += strLine;
                }
                streamReader.Close();
                List<PlayListItem> items = JsonConvert.DeserializeObject<List<PlayListItem>>(playListJson);
                string[] split = Path.GetFileNameWithoutExtension(file).Split('.');
                if (split[0] == "default")
                {
                    playLists.Add(new PlayList() { Name = Player.GetStr("TipPlayListDefault"), IsDefault = true, Items = items });
                }
                else
                {
                    playLists.Add(new PlayList() { Name = split[0], Items = items });
                }
            }
            if (ListCount() == 0)
            {
                playLists.Add(new PlayList() { Name = Player.GetStr("TipPlayListDefault"), IsDefault = true });
            }
            playLists = Utils.Swap(playLists, playLists.FindIndex(o => o.IsDefault == true), 0);
        }

        public static void Save()
        {
            string file;
            foreach (var list in playLists)
            {
                if (list.IsDefault)
                {
                    file = System.Windows.Forms.Application.StartupPath + "\\PlayList\\default.playlist.json";
                }
                else
                {
                    file = System.Windows.Forms.Application.StartupPath + $"\\PlayList\\{list.Name}.playlist.json";
                }
                StreamWriter streamWriter = new StreamWriter(file, false);
                streamWriter.Write(JsonConvert.SerializeObject(list.Items, Formatting.Indented));
                streamWriter.Close();
            }
        }

        public static List<PlayList> ListGetLists()
        {
            return playLists;
        }

        public static List<PlayListItem> ListGet()
        {
            return playLists[current].Items;
        }

        public static bool ListAdd(PlayList list)
        {
            if (ListCheckName(list.Name))
            {
                playLists.Add(list);
                try
                {
                    File.WriteAllText(Path.Combine(PlayListDir, $"{list.Name}.playlist.json"), string.Empty);
                }
                catch { }
                return true;
            }
            else
            {
                return false;
            }
        }

        public static void ListRemoveAt(int index)
        {
            try
            {
                File.Delete(Path.Combine(PlayListDir, $"{playLists[index].Name}.playlist.json"));
            }
            catch { }
            playLists.RemoveAt(index);
        }

        public static int ListCount()
        {
            return playLists.Count;
        }

        public static bool ListRename(string newName)
        {
            if (ListCheckName(newName) && current != 0)
            {
                try
                {
                    File.Move(Path.Combine(PlayListDir, $"{playLists[current].Name}.playlist.json"), Path.Combine(PlayListDir, $"{newName}.playlist.json"));
                }
                catch { }
                playLists[current].Name = newName;

                return true;
            }
            else
            {
                return false;
            }
        }

        public static void ItemAdd(string file)
        {
            playLists[current].Items.Add(new PlayListItem(file));

        }

        public static void ItemRemoveAt(int index, bool removeAudioInfoData = false)
        {
            string file = ItemGet(index);
            playLists[current].Items.RemoveAt(index);
            if (removeAudioInfoData)
                AudioInfoDataHelper.RemoveIfIsNotInAllLists(file);
        }

        public static int ItemCount()
        {
            return playLists[current].Items.Count;
        }

        public static string ItemGet(int index)
        {
            if (!ItemIsOutOfRange(index))
                return playLists[current].Items[index].File;
            else
                return "";
        }

        public static string ItemGetDisplayTitle(int index)
        {
            if (!ItemIsOutOfRange(index))
                return playLists[current].Items[index].ToString();
            else
                return "";
        }

        public static int ItemIndexOf(string file)
        {
            return playLists[current].Items.IndexOf(new PlayListItem(file));
        }

        public static int ItemIndexOf(PlayListItem item)
        {
            return playLists[current].Items.IndexOf(item);
        }

        public static void ItemClear(bool removeAudioInfoData = false)
        {
            if (removeAudioInfoData)
            {
                foreach (var item in playLists[current].Items)
                {
                    AudioInfoDataHelper.RemoveIfIsNotInAllLists(item.File);
                }
            }
            playLists[current].Items.Clear();
        }

        public static bool ItemIsOutOfRange(int index)
        {
            if (index < 0 || index >= ItemCount())
                return true;
            else
                return false;
        }

        public static bool ListIsOutOfRange(int index)
        {
            if (index >= 0 && index < ListCount())
                return false;
            else
                return true;
        }

        public static bool ListCheckName(string name)
        {
            if (!playLists.Exists(t => t.Name == name) && name != "default" && name.IndexOfAny(Path.GetInvalidFileNameChars()) == -1)
                return true;
            else
                return false;
        }

        public static bool IsInAllList(string file)
        {
            foreach (var list in playLists)
            {
                if (list.Items.Exists(o => o.File == file))
                {
                    return true;
                }
            }
            return false;
        }

        public static AudioInfo GetFileDataInfoByFile(string file)
        {
            AudioInfo info = new AudioInfo(file);
            try
            {
                TagLib.File tFile = TagLib.File.Create(file);
                info.Title = tFile.Tag.Title;
                info.Album = tFile.Tag.Album;
                info.Artist = string.Join(";", tFile.Tag.Performers);
                info.Genre = string.Join(";", tFile.Tag.Genres);
                info.Comment = tFile.Tag.Comment;
                info.Track = (int)tFile.Tag.Track;
                info.Year = (int)tFile.Tag.Year;
                info.BitRate = tFile.Properties.AudioBitrate;
                info.SampleRate = tFile.Properties.AudioSampleRate;
                info.Length = tFile.Properties.Duration.TotalSeconds;
            }
            catch
            {
                float bitrate = 0, freq = 0;
                BassCore core = new BassCore();
                core.Open(file);
                Bass.BASS_ChannelGetAttribute(core.Stream, BASSAttribute.BASS_ATTRIB_BITRATE, ref bitrate);
                Bass.BASS_ChannelGetAttribute(core.Stream, BASSAttribute.BASS_ATTRIB_FREQ, ref freq);
                info.BitRate = (int)bitrate;
                info.SampleRate = (int)freq;
                info.Length = core.GetLength(true);
                info.IsTagNull = true;
                core.Close();
            }
            info.IsInfoAcquired = true;
            return info;
        }

        public static string GetDisplayTitle(AudioInfo info, DisplayTitleFormat? format = null)
        {
            string emptyTitle = Player.GetStr("TipTitleUnknow");
            string emptyArtist = Player.GetStr("TipArtistUnknow");
            if (format == null)
                format = (DisplayTitleFormat)Settings.Default.DisplayTitleFormat;
            if (info.IsTagNull || info.IsInfoAcquired == false)
                return Path.GetFileName(info.File);
            switch (format)
            {
                case DisplayTitleFormat.FileName:
                    return Path.GetFileName(info.File);
                case DisplayTitleFormat.Title:
                    if (info.Title == "")
                        return Path.GetFileName(info.File);
                    else
                        return info.Title;
                case DisplayTitleFormat.ArtistTitle:
                    if (info.Title == "" && info.Artist == "")
                        return Path.GetFileName(info.File);
                    else
                        return info.Artist == "" ? emptyArtist : info.Artist + "-" + info.Title == "" ? emptyTitle : info.Title;
                case DisplayTitleFormat.TitleArtist:
                    if (info.Title == "" && info.Artist == "")
                        return Path.GetFileName(info.File);
                    else
                        return info.Title == "" ? emptyTitle : info.Title + "-" + info.Artist == "" ? emptyArtist : info.Artist;
                default:
                    return Path.GetFileName(info.File);
            }
        }
    }
    /*   public class MusicInfo
       {
           public string? Title { get; set; }
           public string? Album { get; set; }
           public string? Artist { get; set; }
           public string? Genre { get; set; }
           public string? Comment { get; set; }
           public uint? Track { get; set; }
           public uint? Year { get; set; }
           public int? SampleRate { get; set; }
           public int? BitRate { get; set; }
           public double? Length { get; set; }
           public bool TagIsNull { get; set; }

           public MusicInfo() { }
       }


       public class PlayListItem
       {
           public required string File { get; set; }
           public string? LrcFile { get; set; }
           public MusicInfo? Info { get; set; }
           public PlayListItem() { }

           public void MakeInfo()
           {
               if(Info == null)
               {
                   Info = new MusicInfo();
               }
               float bitrate = 0;
               float freq = 0;
               try
               {
                   TagLib.File tFile = TagLib.File.Create(File);
                   Info.Title = tFile.Tag.Title;
                   Info.Album = tFile.Tag.Album;
                   Info.Artist = string.Join(";", tFile.Tag.Performers);
                   Info.Genre = string.Join(";", tFile.Tag.Genres);
                   Info.Comment = tFile.Tag.Comment;
                   Info.Track = tFile.Tag.Track;
                   Info.Year = tFile.Tag.Year;
                   Info.BitRate = tFile.Properties.AudioBitrate;
                   Info.SampleRate = tFile.Properties.AudioSampleRate;
                   Info.Length = tFile.Properties.Duration.TotalSeconds;
               }
               catch
               {
                   BassCore core = new BassCore();
                   core.Open(File);
                   TAG_INFO tag = BassTags.BASS_TAG_GetFromFile(File);
                   Bass.BASS_ChannelGetAttribute(core.Stream, BASSAttribute.BASS_ATTRIB_BITRATE, ref bitrate);
                   Bass.BASS_ChannelGetAttribute(core.Stream, BASSAttribute.BASS_ATTRIB_FREQ, ref freq);
                   Info.BitRate = (int)bitrate;
                   Info.SampleRate = (int)freq;
                   Info.Length = core.GetLength(true);
                   Info.TagIsNull = true;
                   core.Close();
               }
           }

           public override string ToString()
           {
               string format = Settings.Default.TitleFormat;
               if (Info.TagIsNull || (Info.Title == null && Info.Artist == ""))
               {
                   if (Path.GetFileName(File) != string.Empty)
                   {
                       return Path.GetFileName(File);
                   }
                   else
                   {
                       return File;
                   }
               }
               format = format.Replace("{Title}", Info.Title == null ? Player.GetStr("TipTitleUnknow") : Info.Title);
               format = format.Replace("{Artist}", Info.Artist == "" ? Player.GetStr("TipArtistUnknow") : Info.Artist);
               format = format.Replace("{Album}", Info.Album == null ? Player.GetStr("TipAlbumUnknow") : Info.Album);
               format = format.Replace("{Genre}", Info.Genre == "" ? Player.GetStr("TipGenreUnknow") : Info.Genre);
               format = format.Replace("{Comm}", Info.Comment == null ? Player.GetStr("TipCommentUnknow") : Info.Comment);
               format = format.Replace("{Track}", Info.Track.ToString());
               format = format.Replace("{Year}", Info.Year.ToString());
               format = format.Replace("{File}", Path.GetFileName(File));
               format = format.Replace("\\{\\", "{");
               return format;
           }
       }

       public enum PlayListOrder
       {
           ORDER_FILE = 0,
           ORDER_FILE_DESCENDING = 1,
           ORDER_TITLE = 2,
           ORDER_TITLE_DESCENDING = 3,
           ORDER_ARTIST = 4,
           ORDER_ARTIST_DESCENDING = 5,
           ORDER_ALBUM = 6,
           ORDER_ALBUM_DESCENDING = 7,
           ORDER_LENGTH = 8,
           ORDER_LENGTH_DESCENDING = 9,
           ORDER_GENRE = 10,
           ORDER_GENRE_DESCENDING = 11,
           ORDER_SHOWTITLE = 12,
           ORDER_SHOWTITLE_DESCENDING = 13,
           ORDER_PATH = 14,
           ORDER_PATH_DESCENDING = 15,
           ORDER_TYPE = 16,
           ORDER_TYPE_DESCENDING = 17,
       }

       public enum PlayListSearch
       {
           SEARCH_FILE = 0,
           SEARCH_SHOWTITLE = 1,
           SEARCH_TITLE = 2,
           SEARCH_ARTIST = 3,
           SEARCH_ALBUM = 4,
           SEARCH_TYPE = 5
       }

       public class PlayListManager
       {
           public List<PlayListItem> list = new List<PlayListItem>();

           public int IndexOf(PlayListItem item)
           {
               return list.IndexOf(item);
           }

           public PlayListItem Item(int index)
           { 
               return list[index];
           }

           public void Load(string file = "")
           {
               if (file == "")
               {
                   file = Application.StartupPath + "\\DefaultList.json";
               }
               if (File.Exists(file))
               {
                   StreamReader streamReader = new StreamReader(file);
                   string strLine, playListJson = "";
                   while ((strLine = streamReader.ReadLine()) != null)
                   {
                       playListJson += strLine;
                   }
                   streamReader.Close();
                   list = JsonConvert.DeserializeObject<List<PlayListItem>>(playListJson);
               }
           }

           public void Save(string file = "")
           {
               if (file == "")
               {
                   file = Application.StartupPath + "\\DefaultList.json";
               }
               StreamWriter streamWriter = new StreamWriter(file);
               streamWriter.Write(JsonConvert.SerializeObject(list, Formatting.Indented));
               streamWriter.Close();
           }

           public void Add(PlayListItem playListItem, bool check = true)
           {
               if (check)
               {
                   if (!Player.supportedExtensions.Contains("*" + Path.GetExtension(playListItem.File).ToLower()) && !Utils.IsUrl(playListItem.File))
                   {
                       return;
                   }
               }
               list.Add(playListItem);
           }

           public void Remove(int index)
           {
               list.RemoveAt(index);
           }

           public void Remove(PlayListItem item)
           {
               list.Remove(item);
           }

           public string GetDisplayTitle(int index = -1, string format = "")
           {
               if(format == "")
               {
                   format = Settings.Default.TitleFormat;
               }
               if (index == -1)
               {
                   index = GetLastIndex();
               }
               PlayListItem playListItem = list[index];
               if (playListItem.Info.TagIsNull || (playListItem.Info.Title == null && playListItem.Info.Artist == ""))
               {
                   if (Path.GetFileName(playListItem.File) != string.Empty)
                   {
                       return Path.GetFileName(playListItem.File);
                   }
                   else
                   {
                       return playListItem.File;
                   }
               }
               format = format.Replace("{Title}", playListItem.Info.Title == null ? Player.GetStr("TipTitleUnknow") : playListItem.Info.Title);
               format = format.Replace("{Artist}", playListItem.Info.Artist == "" ? Player.GetStr("TipArtistUnknow") : playListItem.Info.Artist);
               format = format.Replace("{Album}", playListItem.Info.Album == null ? Player.GetStr("TipAlbumUnknow") : playListItem.Info.Album);
               format = format.Replace("{Genre}", playListItem.Info.Genre == "" ? Player.GetStr("TipGenreUnknow") : playListItem.Info.Genre);
               format = format.Replace("{Comm}", playListItem.Info.Comment == null ? Player.GetStr("TipCommentUnknow") : playListItem.Info.Comment);
               format = format.Replace("{Track}", playListItem.Info.Track.ToString());
               format = format.Replace("{Year}", playListItem.Info.Year.ToString());
               format = format.Replace("{File}", Path.GetFileName(playListItem.File));
               format = format.Replace("\\{\\", "{");
               return format;
           }

           public string GetDisplayTitle(PlayListItem playListItem, string format = "")
           {
               if (format == "")
               {
                   format = Settings.Default.TitleFormat;
               }
               if (playListItem.Info.TagIsNull || (playListItem.Info.Title == null && playListItem.Info.Artist == ""))
               {
                   if (Path.GetFileName(playListItem.File) != "")
                   {
                       return Path.GetFileName(playListItem.File);
                   }
                   else
                   {
                       return playListItem.File;
                   }
               }
               format = format.Replace("{Title}", playListItem.Info.Title == null ? Player.GetStr("TipTitleUnknow") : playListItem.Info.Title);
               format = format.Replace("{Artist}", playListItem.Info.Artist == "" ? Player.GetStr("TipArtistUnknow") : playListItem.Info.Artist);
               format = format.Replace("{Album}", playListItem.Info.Album == null ? Player.GetStr("TipAlbumUnknow") : playListItem.Info.Album);
               format = format.Replace("{Genre}", playListItem.Info.Genre == "" ? Player.GetStr("TipGenreUnknow") : playListItem.Info.Genre);
               format = format.Replace("{Comm}", playListItem.Info.Comment == null ? Player.GetStr("TipCommentUnknow") : playListItem.Info.Comment);
               format = format.Replace("{Track}", playListItem.Info.Track.ToString());
               format = format.Replace("{Year}", playListItem.Info.Year.ToString());
               format = format.Replace("{File}", Path.GetFileName(playListItem.File));
               format = format.Replace("\\{\\", "{");
               return format;
           }

           public int Count()
           {
               return list.Count;
           }

           public PlayListItem GetLast()
           {
               return list[list.Count - 1];
           }

           public int GetLastIndex()
           {
               return list.Count - 1;
           }

           public bool IsOutOfArray(int index)
           {
               return index < 0 || index >= list.Count;
           }

           public void Clear()
           {
               list.Clear();
           }

           public void SetLrc(string file, int index)
           {
               PlayListItem TempItem = list[index];
               TempItem.LrcFile = file;
               list[index] = TempItem;
           }

           public void OrderBy(PlayListOrder order)
           {
               switch (order)
               {
                   case PlayListOrder.ORDER_FILE:
                       list = list.OrderBy(o => o.File).ToList();
                       break;
                   case PlayListOrder.ORDER_FILE_DESCENDING:
                       list = list.OrderByDescending(o => o.File).ToList();
                       break;
                   case PlayListOrder.ORDER_ALBUM:
                       list = list.OrderBy(o => o.Info.Album).ToList();
                       break;
                   case PlayListOrder.ORDER_ALBUM_DESCENDING:
                       list = list.OrderByDescending(o => o.Info.Album).ToList();
                       break;
                   case PlayListOrder.ORDER_ARTIST:
                       list = list.OrderBy(o => o.Info.Artist).ToList();
                       break;
                   case PlayListOrder.ORDER_ARTIST_DESCENDING:
                       list = list.OrderByDescending(o => o.Info.Artist).ToList();
                       break;
                   case PlayListOrder.ORDER_LENGTH:
                       list = list.OrderBy(o => o.Info.Length).ToList();
                       break;
                   case PlayListOrder.ORDER_LENGTH_DESCENDING:
                       list = list.OrderByDescending(o => o.Info.Length).ToList();
                       break;
                   case PlayListOrder.ORDER_TITLE:
                       list = list.OrderBy(o => o.Info.Title).ToList();
                       break;
                   case PlayListOrder.ORDER_TITLE_DESCENDING:
                       list = list.OrderByDescending(o => o.Info.Title).ToList();
                       break;
                   case PlayListOrder.ORDER_GENRE:
                       list = list.OrderBy(o => o.Info.Genre).ToList();
                       break;
                   case PlayListOrder.ORDER_GENRE_DESCENDING:
                       list = list.OrderByDescending(o => o.Info.Genre).ToList();
                       break;
                   case PlayListOrder.ORDER_SHOWTITLE:
                       list = list.OrderBy(o => GetDisplayTitle(list.IndexOf(o), Settings.Default.TitleFormat)).ToList();
                       break;
                   case PlayListOrder.ORDER_SHOWTITLE_DESCENDING:
                       list = list.OrderByDescending(o => GetDisplayTitle(list.IndexOf(o), Settings.Default.TitleFormat)).ToList();
                       break;
                   case PlayListOrder.ORDER_PATH:
                       list = list.OrderBy(o => Path.GetDirectoryName(o.File)).ToList();
                       break;
                   case PlayListOrder.ORDER_PATH_DESCENDING:
                       list = list.OrderByDescending(o => Path.GetDirectoryName(o.File)).ToList();
                       break;
                   case PlayListOrder.ORDER_TYPE:
                       list = list.OrderBy(o => Path.GetExtension(o.File)).ToList();
                       break;
                   case PlayListOrder.ORDER_TYPE_DESCENDING:
                       list = list.OrderByDescending(o => Path.GetExtension(o.File)).ToList();
                       break;
               }
           }

           public List<PlayListItem> Search(PlayListSearch searchBy, string keyWord)
           {
               List<PlayListItem> l = new List<PlayListItem>();
               keyWord = keyWord.ToLower();
               foreach (PlayListItem item in list)
               {
                   switch (searchBy)
                   {
                       case PlayListSearch.SEARCH_ARTIST:
                           if (item.Info.Artist != null && item.Info.Artist.ToLower().IndexOf(keyWord) != -1)
                               l.Add(item);
                           break;
                       case PlayListSearch.SEARCH_TYPE:
                           if (Path.GetExtension(item.File).ToLower().IndexOf(keyWord) != -1)
                               l.Add(item);
                           break;
                       case PlayListSearch.SEARCH_FILE:
                           if (Path.GetFileNameWithoutExtension(item.File).ToLower().IndexOf(keyWord) != -1)
                               l.Add(item);
                           break;
                       case PlayListSearch.SEARCH_TITLE:
                           if (item.Info.Title != null && item.Info.Title.ToLower().IndexOf(keyWord) != -1)
                               l.Add(item);
                           break;
                       case PlayListSearch.SEARCH_SHOWTITLE:
                           if (GetDisplayTitle(item, Settings.Default.TitleFormat).ToLower().IndexOf(keyWord) != -1)
                               l.Add(item);
                           break;
                       case PlayListSearch.SEARCH_ALBUM:
                           if (item.Info.Album != null && item.Info.Album.ToLower().IndexOf(keyWord) != -1)
                               l.Add(item);
                           break;
                   }
               }
               return l;
           }
       }*/
}