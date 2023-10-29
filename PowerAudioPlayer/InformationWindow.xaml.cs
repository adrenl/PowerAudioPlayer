using System.IO;
using System.Windows;
using Un4seen.Bass;
using Un4seen.Bass.AddOn.Tags;
using Un4seen.Bass.AddOn.Midi;
using System.Linq;

namespace PowerAudioPlayer
{
    /// <summary>
    /// InformationWindow.xaml 的交互逻辑
    /// </summary>
    public partial class InformationWindow : Window
    {
        private TagLib.File tag;

        public string File {  get; set; }

        public InformationWindow()
        {
            InitializeComponent();
            cbGenre.ItemsSource = BassTags.ID3v1Genre;
        }

        public void Read()
        {
            BassCore bass = new BassCore();
            BASS_CHANNELINFO info = new BASS_CHANNELINFO();
            AudioInfo audioInfo = new AudioInfo();
            bass.Open(File);
            if (bass.Stream == 0)
            {
                Visibility = Visibility.Hidden;
                MessageBox.Show(Player.GetStr("MsgOpenError"), null, 0, MessageBoxImage.Error);
                Close();
            }
            tag = TagLib.File.Create(File);
            Bass.BASS_ChannelGetInfo(bass.Stream, info);
            audioInfo = PlayListHelper.GetFileDataInfoByFile(File);
            BasicInformationGrid.DataContext = audioInfo;
            TagInformationGrid.DataContext = audioInfo;

            tbFileName.Text = Path.GetFileName(File);
            tbPath.Text = Path.GetDirectoryName(File);
            tbType.Text = info.ctype.ToString();
            tbSize.Text = Utils.CountSize(Utils.GetFileSize(File));

            if(info.ctype == BASSChannelType.BASS_CTYPE_STREAM_MIDI)
            {
                MIDITab.Visibility = Visibility.Visible;
                
                MIDIMarkText.ItemsSource = BassMidi.BASS_MIDI_StreamGetMarks(bass.Stream, -1, BASSMIDIMarker.BASS_MIDI_MARK_TEXT);
                MIDIMarkCopy.ItemsSource = BassMidi.BASS_MIDI_StreamGetMarks(bass.Stream, -1, BASSMIDIMarker.BASS_MIDI_MARK_COPY);
                MIDIMarkInst.ItemsSource = BassMidi.BASS_MIDI_StreamGetMarks(bass.Stream, -1, BASSMIDIMarker.BASS_MIDI_MARK_INST);
                MIDIMarkMarker.ItemsSource = BassMidi.BASS_MIDI_StreamGetMarks(bass.Stream, -1, BASSMIDIMarker.BASS_MIDI_MARK_MARKER);
                MIDIMarkTimeSign.ItemsSource = BassMidi.BASS_MIDI_StreamGetMarks(bass.Stream, -1, BASSMIDIMarker.BASS_MIDI_MARK_TIMESIG);
                MIDIMarkKeySig.ItemsSource = BassMidi.BASS_MIDI_StreamGetMarks(bass.Stream, -1, BASSMIDIMarker.BASS_MIDI_MARK_KEYSIG);
                MIDIMarkCue.ItemsSource = BassMidi.BASS_MIDI_StreamGetMarks(bass.Stream, -1, BASSMIDIMarker.BASS_MIDI_MARK_CUE);
            }
            else if(info.ctype == BASSChannelType.BASS_CTYPE_STREAM_WAV || info.ctype == BASSChannelType.BASS_CTYPE_STREAM_WAV_FLOAT || info.ctype == BASSChannelType.BASS_CTYPE_STREAM_WAV_PCM) { }
            else
            {
                TagTab.Visibility = Visibility.Visible;
                imgAlbumPicture.Source = Utils.ByteArrayToBitmapImage(tag.Tag.Pictures[0].Data.ToArray());
            }
            bass.Close();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Read();
        }
    }
}
