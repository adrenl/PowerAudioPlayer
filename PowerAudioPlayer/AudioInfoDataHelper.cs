using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SQLite;
using System.IO;
using System.Linq;

namespace PowerAudioPlayer
{
    internal static class AudioInfoDataHelper
    {
        private const string TABLE_NAME = "AudioData";
        private static SQLiteConnection dbSQL = new SQLiteConnection();
        private static SQLiteCommand cmd = new SQLiteCommand();

        public static void LoadAudioInfoData()
        {
            string file = Path.Combine(System.Windows.Forms.Application.StartupPath, "FileData.db");
            dbSQL = new SQLiteConnection($"Data Source=\"{file}\";Version=3;");
            dbSQL.Open();
            cmd.Connection = dbSQL;
        }

        public static void SaveAudioInfoData()
        {
            cmd.CommandText = "VACUUM";
            cmd.ExecuteNonQuery();
            dbSQL.Close();
        }

        public static void Add(AudioInfo info)
        {
            try
            {
                cmd.CommandText = $"REPLACE INTO {TABLE_NAME} VALUES ('{Espace(info.File)}', '{Espace(info.Title)}', '{Espace(info.Album)}', '{Espace(info.Artist)}', '{Espace(info.Genre)}', '{Espace(info.Comment)}', {info.Track}, {info.Year}, {info.SampleRate}, {info.BitRate}, {info.Channel}, {info.Length}, {info.IsInfoAcquired}, {info.IsTagNull})";
                cmd.ExecuteNonQuery();
            }
            catch { }
        }

        public static void Remove(string file)
        {
            cmd.CommandText = $"DELETE FROM {TABLE_NAME} WHERE File = '{Espace(file)}'";
            cmd.ExecuteNonQuery();
        }

        public static AudioInfo Get(string file)
        {
            try
            {
                cmd.CommandText = $"SELECT * FROM {TABLE_NAME} WHERE File = '{Espace(file)}' LIMIT 1";
                cmd.ExecuteNonQuery();
                SQLiteDataReader reader = cmd.ExecuteReader();
                reader.Read();
                AudioInfo info = new AudioInfo(file);
                if (reader.HasRows)
                {
                    info.File = file;
                    info.Album = reader.GetValue("Album").ToString();
                    info.Artist = reader.GetValue("Artist").ToString();
                    info.Genre = reader.GetValue("Genre").ToString();
                    info.Comment = reader.GetValue("Comment").ToString();
                    info.Track = Convert.ToInt32(reader.GetValue("Track"));
                    info.Year = Convert.ToInt32(reader.GetValue("Year"));
                    info.BitRate = Convert.ToInt32(reader.GetValue("BitRate"));
                    info.SampleRate = Convert.ToInt32(reader.GetValue("SampleRate"));
                    info.Channel = Convert.ToInt32(reader.GetValue("Channel"));
                    info.Length = Convert.ToDouble(reader.GetValue("Length"));
                    info.IsTagNull = reader.GetBoolean("IsTagNull").ToString().Equals("0");
                    info.IsInfoAcquired = reader.GetBoolean("IsInfoAcquired").ToString().Equals("0");
                }
                reader.Close();
                return info;
            }
            catch
            {
                return new AudioInfo(file);
            }
        }

        public static bool IsExists(string file)
        {
            cmd.CommandText = $"SELECT * FROM {TABLE_NAME} WHERE File = '{Espace(file)}' LIMIT 1";
            cmd.ExecuteNonQuery();
            SQLiteDataReader reader = cmd.ExecuteReader();
            bool hasRows = reader.HasRows;
            reader.Close();
            return hasRows;
        }

        public static void RemoveIfIsNotInAllLists(string file)
        {
            if (!PlayListHelper.IsInAllList(file))
                Remove(file);
        }

        public static void Clear()
        {
            cmd.CommandText = $"TRUNCATE TABLE {TABLE_NAME}";
            cmd.ExecuteNonQuery();
        }

        public static void CleanUp()
        {
            List<string> list = new List<string>();
            cmd.CommandText = $"SELECT * FROM {TABLE_NAME}";
            cmd.ExecuteNonQuery();
            SQLiteDataReader reader = cmd.ExecuteReader();
            while (reader.Read())
            {
                string? file = reader.GetValue("File").ToString();
                if (file != null && !PlayListHelper.IsInAllList(file))
                    list.Add(file);
            }
            reader.Close();
            cmd.CommandText = $"DELETE FROM {TABLE_NAME} WHERE File = '" + string.Join("' OR File = '", list.Select(Espace)) + "'";
            cmd.ExecuteNonQuery();
        }

        private static string Espace(string str)
        {
            if (str == null)
                return "";
            else
                return str.Replace("'", "''");
        }
    }
}