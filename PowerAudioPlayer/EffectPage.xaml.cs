using PowerAudioPlayer.Properties;
using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Un4seen.Bass;
using Un4seen.Bass.AddOn.Fx;

namespace PowerAudioPlayer
{
    /// <summary>
    /// EffectPage.xaml 的交互逻辑
    /// </summary>

    public class EQSliderData
    {
        public string Text { get; set; } = string.Empty;

        public int Tag { get; set; } = 0;

        public int Maximun { get; set; } = 130;

        public int Minimun { get; set; } = -130;
    }

    public partial class EffectPage : Page
    {
        List<EQSliderData> EQSliders = new List<EQSliderData>()
        {
            new EQSliderData(){Text = "80", Tag = 0},
            new EQSliderData(){Text = "125", Tag = 1},
            new EQSliderData(){Text = "250", Tag = 2},
            new EQSliderData(){Text = "550", Tag = 3},
            new EQSliderData(){Text = "1K", Tag = 4},
            new EQSliderData(){Text = "1.5K", Tag = 5},
            new EQSliderData(){Text = "2K", Tag = 6},
            new EQSliderData(){Text = "4K", Tag = 7},
            new EQSliderData(){Text = "8K", Tag = 8},
            new EQSliderData(){Text = "16K", Tag = 9},
            new EQSliderData(){Text = Player.GetStr("TipGain"), Tag = -1, Maximun = 100000, Minimun = -100000},
        };

        public EffectPage()
        {
            InitializeComponent();
            DataContext = Settings.Default;
            EQItemControl.ItemsSource = EQSliders;
        }

        private void sliderCurrent_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                Slider? slider = sender as Slider;
                if (slider.Name == "SliderTempo")
                {
                    Bass.BASS_ChannelSetAttribute(Player.bassCore.Stream, BASSAttribute.BASS_ATTRIB_TEMPO, Convert.ToInt16(slider.Value));
                }
                else if (slider.Name == "SliderPitch")
                {
                    Bass.BASS_ChannelSetAttribute(Player.bassCore.Stream, BASSAttribute.BASS_ATTRIB_TEMPO_PITCH, Convert.ToInt16(slider.Value));
                }
                else if ((int)slider.Tag == -1)
                {
                    Settings.Default.EQGain = slider.Value;
                    Player.bassCore.SetGain(slider.Value / 1000d);
                }
                else
                {
                    Player.bassCore.UpdateEQ((int)slider.Tag, (float)(slider.Value / 10f));
                    Utils.SetPropertyValue(Settings.Default, "EQ" + slider.Tag.ToString(), slider.Value.ToString());
                }
            }
        }

        private void CheckBoxReverse_Click(object sender, RoutedEventArgs e)
        {
            int srcChan = BassFx.BASS_FX_TempoGetSource(Player.bassCore.Stream);
            if (CheckBoxReverse.IsChecked == true)
            {
                Bass.BASS_ChannelSetAttribute(srcChan, BASSAttribute.BASS_ATTRIB_REVERSE_DIR, -1);
            }
            else
            {
                Bass.BASS_ChannelSetAttribute(srcChan, BASSAttribute.BASS_ATTRIB_REVERSE_DIR, 1);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < 10; i++)
            {
                Player.bassCore.UpdateEQ(i, 0);
                Utils.SetPropertyValue(Settings.Default, "EQ" + i.ToString(), "0");
            }
            Player.bassCore.SetGain(0);
            Settings.Default.EQGain = 0;

            List<Slider> sliderList = FindChirldHelper.FindVisualChild<Slider>(EQItemControl);
            foreach (var slider in sliderList)
            {
                if (slider.Tag != null && (int)slider.Tag < 10)
                {
                    slider.Value = 0;
                }
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            Settings.Default.OtherEffectTempo = 0;
            Bass.BASS_ChannelSetAttribute(Player.bassCore.Stream, BASSAttribute.BASS_ATTRIB_TEMPO, 0);
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            Settings.Default.OtherEffectPitch = 0;
            Bass.BASS_ChannelSetAttribute(Player.bassCore.Stream, BASSAttribute.BASS_ATTRIB_TEMPO_PITCH, 0);
        }

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            List<Slider> sliderList = FindChirldHelper.FindVisualChild<Slider>(EQItemControl);
            foreach (var slider in sliderList)
            {
                if (slider.Tag != null && (int)slider.Tag < 10 && (int)slider.Tag > -1)
                {
                    slider.Value = Convert.ToDouble(Settings.Default.GetType().GetProperty("EQ" + slider.Tag.ToString()).GetValue(new Settings(), null));
                }
                else if (slider.Tag != null && (int)slider.Tag == -1)
                {
                    slider.Value = Settings.Default.EQGain * 1000d;
                }
            }
        }

        private void CheckBoxEnableEQ_Click(object sender, RoutedEventArgs e)
        {
            if (CheckBoxEnableEQ.IsChecked == true)
            {
                Player.bassCore.SetGain(Settings.Default.EQGain / 1000d);
                for (int i = 0; i < 10; i++)
                {
                    Player.bassCore.UpdateEQ(i, Convert.ToInt16(Settings.Default.GetType().GetProperty("EQ" + i.ToString()).GetValue(new Settings(), null)));
                }
            }
            else
            {
                for (int i = 0; i < 10; i++)
                {
                    Player.bassCore.UpdateEQ(i, 0);
                }
                Player.bassCore.SetGain(0);
            }
        }
    }
}
