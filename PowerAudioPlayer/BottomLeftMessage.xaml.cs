using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;

namespace PowerAudioPlayer
{
    /// <summary>
    /// BottomLeftMessage.xaml 的交互逻辑
    /// </summary>
    public partial class BottomLeftMessage : UserControl
    {
        DispatcherTimer timer = new DispatcherTimer();

        public string MessageText { get; set; } = "";

        public int Delay { get; set; } = 2500;

        public BottomLeftMessage()
        {
            InitializeComponent();
        }

        public void Show()
        {
            TextBoxMessage.Text = MessageText;
            timer.Interval = new TimeSpan(0, 0, 0, 0, Delay, 0);
            timer.Tick += (object? sender, EventArgs e) =>
            {
                Visibility = Visibility.Collapsed;
            };
            timer.Start();
            Visibility = Visibility.Visible;
        }

        private void UserControl_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            Visibility = Visibility.Collapsed;
        }
    }
}
