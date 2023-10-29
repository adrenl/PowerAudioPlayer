using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;

namespace PowerAudioPlayer
{
    /// <summary>
    /// 按照步骤 1a 或 1b 操作，然后执行步骤 2 以在 XAML 文件中使用此自定义控件。
    ///
    /// 步骤 1a) 在当前项目中存在的 XAML 文件中使用该自定义控件。
    /// 将此 XmlNamespace 特性添加到要使用该特性的标记文件的根
    /// 元素中:
    ///
    ///     xmlns:MyNamespace="clr-namespace:PowerAudioPlayer.Controls"
    ///
    ///
    /// 步骤 1b) 在其他项目中存在的 XAML 文件中使用该自定义控件。
    /// 将此 XmlNamespace 特性添加到要使用该特性的标记文件的根
    /// 元素中:
    ///
    ///     xmlns:MyNamespace="clr-namespace:PowerAudioPlayer.Controls;assembly=PowerAudioPlayer.Controls"
    ///
    /// 您还需要添加一个从 XAML 文件所在的项目到此项目的项目引用，
    /// 并重新生成以避免编译错误:
    ///
    ///     在解决方案资源管理器中右击目标项目，然后依次单击
    ///     “添加引用”->“项目”->[浏览查找并选择此项目]
    ///
    ///
    /// 步骤 2)
    /// 继续操作并在 XAML 文件中使用控件。
    ///
    ///     <MyNamespace:StatusBarPopupMessage/>
    ///
    /// </summary>
    public class StatusBarPopupMessage : Control
    {
        private static bool IsOpening = false;
        private static bool IsFirst = false;
        private static DispatcherTimer timer = new DispatcherTimer();

        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); }
        }
        public static readonly DependencyProperty TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(StatusBarPopupMessage), new PropertyMetadata("", null));

        public int Delay
        {
            get { return (int)GetValue(DelayProperty); }
            set { SetValue(DelayProperty, value); }
        }
        public static readonly DependencyProperty DelayProperty = DependencyProperty.Register("Delay", typeof(int), typeof(StatusBarPopupMessage), new PropertyMetadata(3000, OnDelayChanged));

        static StatusBarPopupMessage()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(StatusBarPopupMessage), new FrameworkPropertyMetadata(typeof(StatusBarPopupMessage)));
        }

        private static void OnDelayChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            timer.Interval = new TimeSpan(0, 0, 0, 0, (int)e.NewValue);
        }

        private void TimerTick(object sender, EventArgs e)
        {
            IsOpening = false;
            Visibility = Visibility.Collapsed;
            timer.Stop();
        }

        public void Open()
        {
            if (IsOpening)
                timer.Stop();
            Visibility = Visibility.Visible;
            timer.Interval = new TimeSpan(0, 0, 0, 0, Delay);
            timer.Tick += TimerTick;
            timer.Start();
            IsOpening = true;
        }
    }
}