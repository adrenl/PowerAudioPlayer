using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace PowerAudioPlayer
{
    /// <summary>
    /// CenterPage.xaml 的交互逻辑
    /// </summary>
    public partial class CenterPage : UserControl
    {
        public CenterPage()
        {
            InitializeComponent();
        }

        public SolidColorBrush MaskColor { get; set; } = new SolidColorBrush(Color.FromArgb(100, 128, 128, 128));

        public bool CloseButton { get; set; } = true;

        public double PageWidth { get; set; } = 150;

        public double PageHeight { get; set; } = 150;

        public required Uri Source { get; set; }

        public void Show()
        {
            FramePage.Source = Source;
            if (CloseButton)
                ButtonClose.Visibility = Visibility.Visible;
            else
                ButtonClose.Visibility = Visibility.Collapsed;
            Background = MaskColor;
            Visibility = Visibility.Visible;
            FramePage.Width = PageWidth;
            FramePage.Height = PageHeight;
        }

        public void Hide()
        {
            Visibility = Visibility.Collapsed;
        }

        private void ButtonClose_Click(object sender, RoutedEventArgs e)
        {
            Hide();
        }
    }
}
