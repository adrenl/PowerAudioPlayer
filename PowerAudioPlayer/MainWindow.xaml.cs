using Microsoft.VisualBasic;
using Microsoft.Win32;
using PowerAudioPlayer.Properties;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shell;
using System.Windows.Threading;
using Un4seen.Bass;
using MessageBox = System.Windows.MessageBox;
using OpenFileDialog = Microsoft.Win32.OpenFileDialog;
namespace PowerAudioPlayer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private SYNCPROC? endSync = null;
        private DispatcherTimer playTimer = new DispatcherTimer();
        private DispatcherTimer lyricsTimer = new DispatcherTimer();
        private TagLib.File? tFile;

        public MainWindow()
        {
            InitializeComponent();
            Init();

            DataContext = Settings.Default;
            SystemEvents_UserPreferenceChanged(new object(), new UserPreferenceChangedEventArgs(new UserPreferenceCategory()));
            endSync = new SYNCPROC(EndSync);

            playTimer.Interval = TimeSpan.FromMilliseconds(25);
            playTimer.Tick += PlayTimer_Tick;

            lyricsTimer.Interval = TimeSpan.FromMilliseconds(250);
            lyricsTimer.Tick += LyricsTimer_Tick;

            SystemEvents.UserPreferenceChanged += SystemEvents_UserPreferenceChanged;
            cbCurrentPlayList.ItemsSource = PlayListHelper.playLists;
            lvPlayList.ItemsSource = PlayListHelper.playLists[PlayListHelper.Current].Items;
            SetBackground();
        }

        private void Init()
        {
            if (Settings.Default.PlayMode == (int)PlayMode.OrderPlay)
            {
                rbOrderPlay.IsChecked = true;
            }
            else if (Settings.Default.PlayMode == (int)PlayMode.TrackLoop)
            {
                rbTrackLoop.IsChecked = true;
            }
            else if (Settings.Default.PlayMode == (int)PlayMode.ListLoop)
            {
                rbPlayListLoop.IsChecked = true;
            }
            else
            {
                rbRandomPlay.IsChecked = true;
            }
            lvPlayList.Items.Refresh();
        }

        private void SetBackground()
        {
            if (!Settings.Default.BgEnable)
            {
                imgBackground.Source = null;
                return;
            }
            try
            {
                if (tFile.Tag.Pictures.Length != 0 && Settings.Default.BgUseAlbumPicture)
                {
                    imgBackground.Source = Utils.ByteArrayToBitmapImage(tFile.Tag.Pictures[0].Data.ToArray());
                }
                else
                {
                    if (Settings.Default.BgDefaultUseDesktopBg)
                    {
                        imgBackground.Source = new BitmapImage(new Uri(Utils.GetWallpaper()));
                    }
                    else
                    {
                        imgBackground.Source = new BitmapImage(new Uri(Settings.Default.BgDefault));
                    }
                }
            }
            catch
            {
                try
                {
                    if (Settings.Default.BgDefaultUseDesktopBg)
                    {
                        imgBackground.Source = new BitmapImage(new Uri(Utils.GetWallpaper()));
                    }
                    else
                    {
                        imgBackground.Source = new BitmapImage(new Uri(Settings.Default.BgDefault));
                    }
                }
                catch
                {
                    imgBackground.Source = null;
                }
            }
        }

        private void LoadLyrics()
        {
            lyricsViewer.ClearLrc();
            int index = Player.playIndex;
            if (index == -1)
                return;
            string file, lrc = "";
            //if (Player.playList.list[index].LrcFile == string.Empty || Player.playList.list[index].LrcFile == "")
            //{
            //    file = Path.GetDirectoryName(Player.playList.list[index].File) + "\\" + Path.GetFileNameWithoutExtension(Player.playList.list[index].File) + ".lrc";
            //}
            //else
            //{
            //    //file = Player.playList.list[index].LrcFile;
            //   file = Path.GetDirectoryName(Player.playList.list[index].File) + "\\" + Path.GetFileNameWithoutExtension(Player.playList.list[index].File) + ".lrc";
            //}
            file = Path.GetDirectoryName(PlayListHelper.ItemGet(Player.playIndex)) + "\\" + Path.GetFileNameWithoutExtension(PlayListHelper.ItemGet(Player.playIndex)) + ".lrc";
            if (!File.Exists(file))
            {
                return;
            }
            try
            {
                using (FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read))
                {
                    string? line;
                    Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
                    using (StreamReader sr = new StreamReader(fs))
                    {
                        while ((line = sr.ReadLine()) != null)
                        {
                            if (line == "")
                                continue;
                            if (line.StartsWith("[ti:")) { }
                            else if (line.StartsWith("[ar:")) { }
                            else if (line.StartsWith("[al:")) { }
                            else if (line.StartsWith("[by:")) { }
                            else if (line.StartsWith("[offset:")) { }
                            else if (line.StartsWith("[total:")) { }
                            else if (line.StartsWith("[sign:")) { }
                            else if (line.StartsWith("[ver:")) { }
                            else if (line.StartsWith("[t_time:")) { }
                            else if (line.StartsWith("[id:")) { }
                            else if (line.StartsWith("[la:")) { }
                            else
                            {
                                lrc += line + "\n\n\n\n";
                            }
                        }
                    }
                }
                lyricsViewer.LoadLrc(lrc);
                lyricsTimer.Start();
            }
            catch { }
        }

        private void CloseFile()
        {
            playTimer.Stop();
            Player.bassCore.Close();
            Player.playIndex = -1;
            Player.playFile = "";
            RestUI();
            StatusLabel();
        }

        private void StatusLabel()
        {
            if (Player.bassCore.IsActive() == BASSActive.BASS_ACTIVE_PLAYING)
            {
                tbStatus.Text = ThumbButtonPlayPause.Description = Player.GetStr("TipPlay");
                imgPlay.Source = Player.GetDrawingImg("PauseImage");
                ThumbButtonPlayPause.ImageSource = taskBarItem.Overlay = Player.GetImg("PauseSmall");
                taskBarItem.ProgressState = TaskbarItemProgressState.Normal;
            }
            else if (Player.bassCore.IsActive() == BASSActive.BASS_ACTIVE_PAUSED)
            {
                tbStatus.Text = ThumbButtonPlayPause.Description = Player.GetStr("TipPause");
                imgPlay.Source = Player.GetDrawingImg("PlayImage");
                ThumbButtonPlayPause.ImageSource = taskBarItem.Overlay = Player.GetImg("PlaySmall");
                taskBarItem.ProgressState = TaskbarItemProgressState.Paused;
            }
            else
            {
                tbStatus.Text = Player.GetStr("TipStop");
                imgPlay.Source = Player.GetImg("PlayImage");
                ThumbButtonPlayPause.ImageSource = taskBarItem.Overlay = Player.GetImg("PlaySmall");
                taskBarItem.ProgressState = TaskbarItemProgressState.None;
            }
            try
            {
                if (Player.playIndex != -1)
                    Title = PlayListHelper.ItemGetDisplayTitle(Player.playIndex) + " - PowerAudioPlayer";
                else
                    Title = "PowerAudioPlayer";
            }
            catch
            {
                Title = "PowerAudioPlayer";
            }
        }

        private void RestUI()
        {
            tbTitle.Text = tbAlbum.Text = tbArtist.Text = tbFileName.Text = string.Empty;
            tbCurrent.Text = tbLength.Text = Player.GetStr("TipTimeZero");
            tbStatus.Text = Player.GetStr("TipStop");
            tbSN.Text = "0";
            sliderCurrent.Value = 0;
            sliderCurrent.IsEnabled = false;
            imgSpectrum.Source = null;
            imgAlbumPicture.Source = Player.GetImg("ApplicationImage");
            imgAlbumPicture.Stretch = Stretch.None;
            imgPlay.Source = Player.GetDrawingImg("PlayImage");
            taskBarItem.Overlay = ThumbButtonPlayPause.ImageSource = Player.GetImg("PlaySmall");
            taskBarItem.ProgressState = TaskbarItemProgressState.None;
            lyricsViewer.ClearLrc();
        }

        private void ShowAlburmPicture(string file)
        {
            try
            {
                tFile = TagLib.File.Create(file);
                if (!Settings.Default.ShowAlbumPicture)
                    throw new Exception();
                imgAlbumPicture.Stretch = Stretch.Uniform;
                imgAlbumPicture.Source = Utils.ByteArrayToBitmapImage(tFile.Tag.Pictures[0].Data.ToArray());
            }
            catch
            {
                tFile = null;
                imgAlbumPicture.Stretch = Stretch.None;
                imgAlbumPicture.Source = Player.GetImg("ApplicationImage");
            }
        }

        private void EndSync(int handle, int channel, int data, nint user)
        {
            playTimer.Stop();
            if (Settings.Default.DisbaleAutoPlay)
                return;
            Dispatcher.Invoke(new Action(delegate ()
            {
                switch (Player.playMode)
                {
                    case PlayMode.OrderPlay:
                        if (Player.playIndex >= PlayListHelper.ItemCount() - 1)
                        {
                            Player.bassCore.Close();
                            RestUI();
                        }
                        else
                        {
                            Button_Click_1(new object(), new RoutedEventArgs());
                        }
                        break;
                    case PlayMode.TrackLoop:
                        playTimer.Start();
                        Player.bassCore.Play();
                        break;
                    case PlayMode.ListLoop:
                        if (Player.playIndex >= PlayListHelper.ItemCount() - 1)
                            Play(0);
                        else
                            Button_Click_1(new object(), new RoutedEventArgs());
                        break;
                    case PlayMode.RamdonPlay:
                        Play(new Random().Next(0, PlayListHelper.ItemCount()));
                        break;
                }
            }));

        }

        private void Play(int index)
        {
            if (PlayListHelper.ItemIsOutOfRange(index))
                return;
            RestUI();
            Player.bassCore.Open(PlayListHelper.ItemGet(index));
            if (Player.bassCore.Stream == 0)
            {
                if (!Settings.Default.DisableAutoNextWhenError)
                {
                    if (Utils.DelayWithFlag(500))
                    {
                        ErrorMessage.MessageText = string.Format(Player.GetStr("MsgErrorPlay"), PlayListHelper.ItemGetDisplayTitle(index));
                        ErrorMessage.Show();
                        Play(index + 1);
                    }
                }
                return;
            }
            if (Utils.IsDelaing)
                Utils.DelayFunctionInterrupt = true;
            tbLength.Text = Utils.ParseTime((int)Player.bassCore.GetLength(true));
            AudioInfo info = AudioInfoDataHelper.Get(PlayListHelper.ItemGet(index));
            tbSN.Text = (index + 1).ToString();
            tbTitle.Text = info.Title;
            tbArtist.Text = info.Artist;
            tbAlbum.Text = info.Album;
            tbFileName.Text = Path.GetFileName(PlayListHelper.ItemGet(index));

            sliderCurrent.Maximum = (int)Player.bassCore.GetLength();
            sliderCurrent.Value = 0;
            sliderCurrent.IsEnabled = true;

            playTimer.Start();

            Player.playIndex = index;
            Player.bassCore.SetEndSync(endSync);
            Player.bassCore.Play();

            ShowAlburmPicture(PlayListHelper.ItemGet(index));
            SetBackground();
            StatusLabel();
            LoadLyrics();
        }

        private void Play(PlayListItem item)
        {
            Play(PlayListHelper.ItemIndexOf(item));
        }

        private void SystemEvents_UserPreferenceChanged(object sender, UserPreferenceChangedEventArgs e)
        {
            Utils.EnableDarkModeForWindow(new WindowInteropHelper(this).Handle.ToInt32(), Utils.IsDarkMode());
            /*ResourceDictionary colorResource = new ResourceDictionary();
            ResourceDictionary imagesResource = new ResourceDictionary();
            if (Utils.IsDarkMode())
            {
                colorResource.Source = new Uri("pack://application:,,,/PowerAudioPlayer;component/Themes/Color.Dark.xaml");
                imagesResource.Source = new Uri("pack://application:,,,/PowerAudioPlayer;component/Resources/Images.Dark.xaml");
            }
            else
            {
                colorResource.Source = new Uri("pack://application:,,,/PowerAudioPlayer;component/Themes/Color.Light.xaml");
                imagesResource.Source = new Uri("pack://application:,,,/PowerAudioPlayer;component/Resources/Images.Light.xaml");
            }
            System.Windows.Application.Current.Resources.MergedDictionaries[1] = colorResource;
            System.Windows.Application.Current.Resources.MergedDictionaries[2] = imagesResource;
            System.Windows.Application.Current.Resources.MergedDictionaries[3] = new ResourceDictionary() { Source = new Uri("pack://application:,,,/PowerAudioPlayer;component/Themes/Styles.xaml") };

            var lvStyle = lvPlayList.Style;
            var lvItemContainerStyle = lvPlayList.ItemContainerStyle;
            lvPlayList.Style = null;
            lvPlayList.ItemContainerStyle = null;
            lvPlayList.Style = lvStyle;
            lvPlayList.ItemContainerStyle = lvItemContainerStyle;*/
        }

        private void LyricsTimer_Tick(object? sender, EventArgs e)
        {
            try
            {
                if (Player.playIndex != -1 && Player.bassCore.IsActive() == BASSActive.BASS_ACTIVE_PLAYING)
                    lyricsViewer.LrcRoll(Player.bassCore.GetCurrentPosition(true) * 1000);
            }
            catch { }
        }

        private void PlayTimer_Tick(object? sender, EventArgs e)
        {
            if (Settings.Default.ShowSpectrum || !(WindowState == WindowState.Minimized) || !Settings.Default.IsMiniMode)
            {
                try
                {
                    System.Drawing.Color color = Utils.MediaColorToSystemColor(Utils.GetSystemThemeColor());
                    IntPtr hBitmap = Player.bassCore.visModified.CreateSpectrumLinePeak(Player.bassCore.Stream,
                        (int)imgSpectrum.Width,
                        (int)imgSpectrum.Height,
                        color,
                        color,
                        color,
                        System.Drawing.Color.Transparent,
                        Settings.Default.SpectrumLineWidth,
                        Settings.Default.SpectrumPeakWidth,
                        Settings.Default.SpectrumDistance,
                        Settings.Default.SpectrumPeakDelay,
                        Settings.Default.SpectrumLinear,
                        Settings.Default.SpectrumFull,
                        Settings.Default.SpectrumHighQuality).GetHbitmap();
                    imgSpectrum.Source = Imaging.CreateBitmapSourceFromHBitmap(hBitmap, IntPtr.Zero, Int32Rect.Empty, BitmapSizeOptions.FromEmptyOptions());
                    Utils.DeleteObject(hBitmap);
                }
                catch
                {
                    imgSpectrum.Source = null;
                }
            }
            tbCurrent.Text = Utils.ParseTime((int)Player.bassCore.GetCurrentPosition(true));
            int pos = (int)Player.bassCore.GetCurrentPosition();
            if (pos <= sliderCurrent.Maximum && pos >= sliderCurrent.Minimum)
            {
                sliderCurrent.Value = pos;
                try
                {
                    taskBarItem.ProgressValue = pos / sliderCurrent.Maximum;
                }
                catch { }
            }
        }

        private void Item_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            Play((PlayListItem)lvPlayList.SelectedItem);
        }

        private void MenuItem_Click_1(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog();
            if (folderBrowserDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                busyIndicator.IsBusy = true;
                busyIndicator.BusyContent = Player.GetStr("MsgAddingTitle");
                Thread thread = new Thread(() =>
                {
                    foreach (string Ext in Player.supportedExtensions)
                    {
                        List<string> Files = Utils.FindFile(folderBrowserDialog.SelectedPath, Ext, true, false, (string file) =>
                        {
                            PlayListHelper.ItemAdd(file);
                            AudioInfoDataHelper.Add(PlayListHelper.GetFileDataInfoByFile(file));
                            Dispatcher.BeginInvoke(new Action(() =>
                            {
                                busyIndicator.BusyContent = string.Format(Player.GetStr("MsgAdding"), file);
                            }));
                        });
                    }
                    Dispatcher.BeginInvoke(new Action(() =>
                    {
                        lvPlayList.Items.Refresh();
                        busyIndicator.IsBusy = false;
                    }));
                });
                thread.IsBackground = true;
                thread.Start();
            }
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = Player.GetFileFilter();
            openFileDialog.Multiselect = true;
            if (openFileDialog.ShowDialog() == true)
            {
                busyIndicator.IsBusy = true;
                busyIndicator.BusyContent = Player.GetStr("MsgAddingTitle");

                Thread thread = new Thread(() =>
                {
                    foreach (string file in openFileDialog.FileNames)
                    {
                        PlayListHelper.ItemAdd(file);
                        AudioInfoDataHelper.Add(PlayListHelper.GetFileDataInfoByFile(file));
                        Dispatcher.BeginInvoke(new Action(() =>
                        {
                            busyIndicator.BusyContent = string.Format(Player.GetStr("MsgAdding"), file);
                        }));
                    }
                    Dispatcher.BeginInvoke(new Action(() =>
                    {
                        lvPlayList.Items.Refresh();
                        busyIndicator.IsBusy = false;
                    }));
                });
                thread.IsBackground = true;
                thread.Start();
            }
        }

        private void MenuItem_Click_2(object sender, RoutedEventArgs e)
        {
            Interaction.InputBox(Player.GetStr("MsgInputURL"));
        }

        private void sliderCurrent_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                if (Player.bassCore.IsActive() == BASSActive.BASS_ACTIVE_PLAYING)
                    Player.bassCore.SetPosition((long)sliderCurrent.Value);
            }
        }

        private void sliderVolume_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (Player.playIndex != -1 && Player.bassCore.IsActive() == BASSActive.BASS_ACTIVE_PLAYING)
            {
                int pos = (int)sliderVolume.Value;
                Player.bassCore.SetVolume(pos);
                lyricsViewer.LrcRoll(Player.bassCore.GetCurrentPosition(true) * 1000);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Player.bassCore.Stop();
            sliderCurrent.Value = 0;
            sliderCurrent.IsEnabled = false;
            StatusLabel();
            playTimer.Stop();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            if (Player.playMode == PlayMode.RamdonPlay)
            {
                Play(new Random().Next(0, PlayListHelper.ItemCount()));
            }
            else
            {
                Play(Player.playIndex + 1);
            }
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            if (Player.playIndex == -1)
            {
                Play(0);
                return;
            }
            if (Player.bassCore.IsActive() == BASSActive.BASS_ACTIVE_PLAYING)
            {
                playTimer.Stop();
                sliderCurrent.IsEnabled = false;
                Player.bassCore.Pause();
            }
            else
            {
                playTimer.Start();
                sliderCurrent.IsEnabled = true;
                Player.bassCore.Play();
            }
            StatusLabel();
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            if (Player.playMode == PlayMode.RamdonPlay)
            {
                Button_Click_1(sender, e);
            }
            else
            {
                Play(Player.playIndex - 1);
            }
        }

        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            Player.SetPlayMode(PlayMode.OrderPlay);
        }

        private void RadioButton_Checked_1(object sender, RoutedEventArgs e)
        {
            Player.SetPlayMode(PlayMode.TrackLoop);
        }

        private void RadioButton_Checked_2(object sender, RoutedEventArgs e)
        {
            Player.SetPlayMode(PlayMode.ListLoop);
        }

        private void RadioButton_Checked_3(object sender, RoutedEventArgs e)
        {
            Player.SetPlayMode(PlayMode.RamdonPlay);
        }

        private void MenuItem_Click_3(object sender, RoutedEventArgs e)
        {
            if (lvPlayList.SelectedItems.Count > 0)
            {
                List<PlayListItem> listItems = lvPlayList.SelectedItems.Cast<PlayListItem>().ToList();
                foreach (var item in listItems)
                {
                    if (Player.playIndex == PlayListHelper.ItemIndexOf(item))
                        CloseFile();
                    PlayListHelper.ItemRemoveAt(PlayListHelper.ItemIndexOf(item));
                }
                lvPlayList.Items.Refresh();
            }
            RemoveButton.IsOpen = false;
        }

        private void MenuItem_Click_4(object sender, RoutedEventArgs e)
        {
            PlayListHelper.ItemClear();
            CloseFile();
            lvPlayList.Items.Refresh();
            RemoveButton.IsOpen = false;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Player.UnInit();
            Settings.Default.Save();
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            if (Player.playIndex != -1)
            {
                lvPlayList.SelectedIndex = Player.playIndex;
            }
        }

        private void Button_Click_5(object sender, RoutedEventArgs e)
        {
            SettingsWindow settingsWindow = new SettingsWindow();
            settingsWindow.Owner = this;
            settingsWindow.ShowDialog();
        }

        private void ThumbButton_Click_3(object sender, EventArgs e)
        {
            Button_Click_3(new object(), new RoutedEventArgs());
        }

        private void ThumbButton_Click(object sender, EventArgs e)
        {
            Button_Click(new object(), new RoutedEventArgs());
        }

        private void ThumbButton_Click_2(object sender, EventArgs e)
        {
            Button_Click_2(new object(), new RoutedEventArgs());
        }

        private void ThumbButton_Click_1(object sender, EventArgs e)
        {
            Button_Click_1(new object(), new RoutedEventArgs());
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);
            HwndSource? hwndSource = PresentationSource.FromVisual(this) as HwndSource;
            if (hwndSource != null)
                hwndSource.AddHook(new HwndSourceHook(this.WndProc));
        }

        private nint WndProc(nint hwnd, int msg, nint wParam, nint lParam, ref bool handled)
        {
            switch (msg)
            {
                case Player.WM_SETTINGSCHANGE:
                    switch (wParam)
                    {
                        case 0:
                            SetBackground();
                            break;
                        case 1:
                            SystemEvents_UserPreferenceChanged(new object(), new UserPreferenceChangedEventArgs(new UserPreferenceCategory()));
                            break;
                    }
                    break;
            }
            return IntPtr.Zero;
        }

        private void Button_Click_6(object sender, RoutedEventArgs e)
        {
            EffectCenterFrame.Show();
        }

        private void cbCurrentPlayList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                CloseFile();
                PlayListHelper.Current = cbCurrentPlayList.SelectedIndex;
                lvPlayList.ItemsSource = PlayListHelper.playLists[PlayListHelper.Current].Items;
                lvPlayList.Items.Refresh();

            }
            catch { }
        }

        private void MenuItem_Click_5(object sender, RoutedEventArgs e)
        {
            string name = Interaction.InputBox(Player.GetStr("MsgListName"), Player.GetStr("MsgNewList"));
            if (name != "")
            {
                if (PlayListHelper.ListAdd(new PlayList() { Name = name }))
                    cbCurrentPlayList.Items.Refresh();
                else
                    MessageBox.Show(Player.GetStr("MsgListNameError"), null, 0, MessageBoxImage.Error);
            }
        }

        private void MenuItem_Click_6(object sender, RoutedEventArgs e)
        {
            if (PlayListHelper.Current == 0)
                return;
            PlayListHelper.ListRemoveAt(PlayListHelper.Current);
            PlayListHelper.Current = Settings.Default.CurrentPlayList = 0;
            cbCurrentPlayList.Items.Refresh();
        }

        private void MenuItem_Click_7(object sender, RoutedEventArgs e)
        {
            if (PlayListHelper.Current == 0)
                return;
            string name = Interaction.InputBox(Player.GetStr("MsgListName"), Player.GetStr("MsgRenameList"));
            if (name != "")
            {
                if (PlayListHelper.ListRename(name))
                {
                    cbCurrentPlayList.Items.Refresh();
                    cbCurrentPlayList.Text = PlayListHelper.ListGetLists()[PlayListHelper.Current].Name;
                }
                else
                    MessageBox.Show(Player.GetStr("MsgListNameError"), null, 0, MessageBoxImage.Error);
            }
        }

        private void Button_Click_7(object sender, RoutedEventArgs e)
        {
            CloseFile();
        }
    }
}