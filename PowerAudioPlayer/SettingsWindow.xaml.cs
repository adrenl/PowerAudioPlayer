using PowerAudioPlayer.Properties;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using Un4seen.Bass;
using OpenFileDialog = Microsoft.Win32.OpenFileDialog;

namespace PowerAudioPlayer
{
    /// <summary>
    /// SettingsWindow.xaml 的交互逻辑
    /// </summary>
    public partial class SettingsWindow : Window
    {
        public SettingsWindow()
        {
            InitializeComponent();
            DataContext = Settings.Default;
            Utils.EnableDarkModeForWindow(new WindowInteropHelper(this).Handle.ToInt32(), Utils.IsDarkMode());
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = Player.GetStr("FilterSoundFont");
            if (openFileDialog.ShowDialog() == true)
            {
                Settings.Default.MIDISoundFont = openFileDialog.FileName;
                Player.bassCore.SetMIDISoundFont(openFileDialog.FileName);
            }
        }

        private void Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            Bass.BASS_SetConfig(BASSConfig.BASS_CONFIG_MIDI_VOICES, (int)((Slider)sender).Value);
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = Player.GetStr("FilterImage");
            if (openFileDialog.ShowDialog() == true)
            {
                Settings.Default.BgDefault = openFileDialog.FileName;
                CheckBox_Click(sender, e);
            }
        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            CheckBox_Click(sender, e);
        }

        private void CheckBox_Click(object sender, RoutedEventArgs e)
        {
            Utils.SendMessage(new WindowInteropHelper(Owner).Handle.ToInt32(), Player.WM_SETTINGSCHANGE, 0, 0);
        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Utils.SendMessage(new WindowInteropHelper(Owner).Handle.ToInt32(), Player.WM_SETTINGSCHANGE, 1, 0);
            Utils.EnableDarkModeForWindow(new WindowInteropHelper(this).Handle.ToInt32(), Utils.IsDarkMode());
        }

        private void CheckBox_Click_1(object sender, RoutedEventArgs e)
        {
            Utils.SendMessage(new WindowInteropHelper(Owner).Handle.ToInt32(), Player.WM_SETTINGSCHANGE, 1, 0);
        }

        private void ColorPicker_SelectedColorChanged(object sender, RoutedPropertyChangedEventArgs<System.Windows.Media.Color?> e)
        {
            Utils.SendMessage(new WindowInteropHelper(Owner).Handle.ToInt32(), Player.WM_SETTINGSCHANGE, 1, 0);
        }
    }
}
