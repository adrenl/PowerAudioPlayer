using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace PowerAudioPlayer
{
    /// <summary>
    /// LyricsViewer.xaml 的交互逻辑
    /// </summary>
    public partial class LyricsViewer : UserControl
    {
        public static readonly DependencyProperty ItemsMarginProperty = DependencyProperty.Register("ItemsMargin", typeof(int), typeof(LyricsViewer), new PropertyMetadata(10, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
        {
            LyricsViewer lyricsViewer = (LyricsViewer)d;
            if (d != null)
            {
                lyricsViewer.IMargin = (int)e.NewValue;
                if (lyricsViewer.Lrcs.Any())
                {
                    foreach (var item in lyricsViewer.Lrcs)
                    {
                        item.Value.c_LrcTb.Margin = new Thickness(0, lyricsViewer.IMargin, 0, 0);
                    }
                }
            }
        })));

        public static readonly DependencyProperty NormalColorProperty = DependencyProperty.Register("NormalColor", typeof(SolidColorBrush), typeof(LyricsViewer), new PropertyMetadata(new SolidColorBrush(Color.FromRgb(0, 0, 0)), new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
        {
            LyricsViewer lyricsViewer = (LyricsViewer)d;
            if (d != null)
            {
                lyricsViewer.NoramlLrcColor = (SolidColorBrush)e.NewValue;
                if (lyricsViewer.Lrcs.Any())
                {
                    foreach (var item in lyricsViewer.Lrcs)
                    {
                        if (!item.Value.Equals(lyricsViewer.foucslrc))
                            item.Value.c_LrcTb.Foreground = lyricsViewer.NoramlLrcColor;
                    }
                }
            }
        })));

        public static readonly DependencyProperty HighLightColorProperty = DependencyProperty.Register("HighLightColor", typeof(SolidColorBrush), typeof(LyricsViewer), new PropertyMetadata(new SolidColorBrush(Color.FromRgb(255, 0, 0)), new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
        {
            LyricsViewer lyricsViewer = (LyricsViewer)d;
            if (d != null)
            {
                lyricsViewer.HighLightColor = (SolidColorBrush)e.NewValue;
                if (lyricsViewer.Lrcs.Any())
                {
                    lyricsViewer.foucslrc.c_LrcTb.Foreground = lyricsViewer.HighLightColor;
                }
            }
        })));

        [Bindable(true)]
        [Category("Appearance")]
        public SolidColorBrush NormalColor
        {
            get { return NoramlLrcColor; }
            set { NoramlLrcColor = value; }
        }

        [Bindable(true)]
        [Category("Appearance")]
        public SolidColorBrush HighLightColor
        {
            get { return FoucsLrcColor; }
            set { FoucsLrcColor = value; }
        }

        [Bindable(true)]
        [Category("Appearance")]
        public int ItemsMargin
        {
            get { return (int)GetValue(ItemsMarginProperty); }
            set { SetValue(ItemsMarginProperty, value); }
        }

        #region 歌词模型
        public class LrcModel
        {
            /// <summary>
            /// 歌词所在控件
            /// </summary>
            public TextBlock c_LrcTb { get; set; }

            /// <summary>
            /// 歌词字符串
            /// </summary>
            public string LrcText { get; set; }

            /// <summary>
            /// 时间（读取格式参照网易云音乐歌词格式：xx:xx.xx，即分:秒.毫秒，秒小数点保留2位。如：00:28.64）
            /// </summary>
            public string Time { get; set; }
        }
        #endregion

        #region 变量
        //歌词集合
        public Dictionary<double, LrcModel> Lrcs = new Dictionary<double, LrcModel>();

        //添加当前焦点歌词变量
        public LrcModel foucslrc { get; set; }
        //当前焦点所在歌词集合位置
        public int FoucsLrcLocation { get; set; } = -1;
        //非焦点歌词颜色
        public SolidColorBrush NoramlLrcColor = new SolidColorBrush(Colors.Black);
        //焦点歌词颜色
        public SolidColorBrush FoucsLrcColor = new SolidColorBrush(Colors.Red);

        public int IMargin = 10;
        #endregion
        public LyricsViewer()
        {
            InitializeComponent();
        }

        #region 加载歌词

        public void ClearLrc()
        {
            Lrcs.Clear();
            lrcItems.Children.Clear();
        }

        public void LoadLrc(string lrcstr)
        {
            ClearLrc();
            //循环以换行\n切割出歌词
            foreach (string str in lrcstr.Split('\n'))
            {
                //过滤空行，判断是否存在时间
                if (str.Length > 0 && str.IndexOf(":") != -1)
                {
                    //歌词时间
                    TimeSpan time = GetTime(str);
                    //歌词取]后面的就行了
                    string lrc = str.Split(']')[1];



                    //歌词显示textblock控件
                    TextBlock c_lrcbk = new TextBlock();
                    //赋值
                    c_lrcbk.Text = lrc;
                    c_lrcbk.HorizontalAlignment = HorizontalAlignment.Center;
                    c_lrcbk.Foreground = NoramlLrcColor;
                    if (lrcItems.Children.Count > 0)
                    {
                        //增加一些行间距，see起来不那么拥挤~
                        c_lrcbk.Margin = new Thickness(0, IMargin, 0, 0);
                    }

                    //添加到集合，方便日后操作
                    Lrcs.Add(time.TotalMilliseconds, new LrcModel()
                    {
                        c_LrcTb = c_lrcbk,
                        LrcText = lrc,
                        Time = time.TotalMilliseconds.ToString()
                    });

                    //将歌词显示textblock控件添加到界面中显示
                    lrcItems.Children.Add(c_lrcbk);

                }
            }
        }

        //正则表达式提取时间

        public TimeSpan GetTime(string str)
        {
            Regex reg = new Regex(@"\[(?<time>.*)\]", RegexOptions.IgnoreCase);
            string timestr = reg.Match(str).Groups["time"].Value;

            //获得分
            int m = Convert.ToInt32(timestr.Split(':')[0]);
            //判断是否有小数点
            int s = 0, f = 0;
            if (timestr.Split(':')[1].IndexOf(".") != -1)
            {
                //有
                s = Convert.ToInt32(timestr.Split(':')[1].Split('.')[0]);
                //获得毫秒位
                f = Convert.ToInt32(timestr.Split(':')[1].Split('.')[1]);

            }
            else
            {
                //没有
                s = Convert.ToInt32(timestr.Split(':')[1]);

            }
            return new TimeSpan(0, 0, m, s, f);

        }

        #endregion

        #region 歌词滚动
        /// <summary>
        /// 歌词滚动、定位焦点
        /// </summary>
        /// <param name="nowtime"></param>
        public void LrcRoll(double nowtime)
        {
            if (!Lrcs.Any())
                return;
            if (foucslrc == null)
            {
                foucslrc = Lrcs.Values.First();
                foucslrc.c_LrcTb.Foreground = FoucsLrcColor;
            }
            else
            {
                if (nowtime < Convert.ToDouble(Lrcs.First().Value.Time))
                {
                    scrollViewer.ScrollToVerticalOffset(0);
                    foucslrc.c_LrcTb.Foreground = NoramlLrcColor;
                }
                //查找焦点歌词
                IEnumerable<KeyValuePair<double, LrcModel>> s = Lrcs.Where(m => nowtime >= m.Key);
                if (s.Any())
                {
                    LrcModel lm = s.Last().Value;
                    foucslrc.c_LrcTb.Foreground = NoramlLrcColor;
                    foucslrc = lm;
                    foucslrc.c_LrcTb.Foreground = FoucsLrcColor;
                    //定位歌词在控件中间区域
                    ResetLrcviewScroll();
                }
            }
        }
        #endregion
        #region 调整歌词控件滚动条位置
        public void ResetLrcviewScroll()
        {
            //获得焦点歌词位置
            GeneralTransform gf = foucslrc.c_LrcTb.TransformToVisual(lrcItems);
            Point p = gf.Transform(new Point(0, 0));

            //计算滚动位置（p.Y是焦点歌词控件(c_LrcTb)相对于父级控件lrcItems(StackPanel)的位置）
            //拿焦点歌词位置减去滚动区域控件高度除以2的值得到的【大概】就是歌词焦点在滚动区域控件的位置
            double os = p.Y - (scrollViewer.ActualHeight / 2) + 10;
            //滚动
            scrollViewer.ScrollToVerticalOffset(os);

        }
        #endregion
    }
}
