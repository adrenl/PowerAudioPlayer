using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;

namespace PowerAudioPlayer
{
    public class IndexConverter : IValueConverter
    {
        public object Convert(object value, Type TargetType, object parameter, CultureInfo culture)
        {
            ListViewItem item = (ListViewItem)value;
            ListView listView = ItemsControl.ItemsControlFromItemContainer(item) as ListView;
            if (listView != null)
            {
                int index = listView.ItemContainerGenerator.IndexFromContainer(item) + 1;
                return index.ToString();
            }
            else
                return "0";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class TimeConverter : IValueConverter
    {
        public object Convert(object value, Type TargetType, object parameter, CultureInfo culture)
        {
            return Utils.ParseTime(System.Convert.ToInt32(value));
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class SndVolPictrueConverter : IValueConverter
    {
        public object Convert(object value, Type TargetType, object parameter, CultureInfo culture)
        {
            int pos = System.Convert.ToInt32(value);
            if (pos == 0)
            {
                return Player.GetDrawingImg("Volume0Image");
            }
            else if (pos > 0 && pos < 33)
            {
                return Player.GetDrawingImg("Volume1Image");
            }
            else if (pos >= 33 && pos < 66)
            {
                return Player.GetDrawingImg("Volume2Image");
            }
            else if (pos >= 66)
            {
                return Player.GetDrawingImg("Volume3Image");
            }
            return Player.GetDrawingImg("Volume0Image");
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class SndVolTextConverter : IValueConverter
    {
        public object Convert(object value, Type TargetType, object parameter, CultureInfo culture)
        {
            return System.Convert.ToInt32(value).ToString() + "%";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class ListViewBackgroundConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            ListViewItem item = (ListViewItem)value;
            ListView? listView = ItemsControl.ItemsControlFromItemContainer(item) as ListView;
            int index = listView.ItemContainerGenerator.IndexFromContainer(item);
            Color bgcolor = ((SolidColorBrush)listView.Background).Color;
            if (index % 2 == 0)
            {
                return new SolidColorBrush(Utils.ChangeColorLight(bgcolor, -0.05f));
            }
            else
            {
                return new SolidColorBrush(bgcolor);
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class SystemThemeColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            parameter ??= 0f;
            if (targetType.FullName == "System.Windows.Media.Color")
            {
                return Utils.ChangeColorLight(Utils.GetSystemThemeColor(), (float)System.Convert.ToSingle(parameter));
            }
            else
            {
                return new SolidColorBrush(Utils.ChangeColorLight(Utils.GetSystemThemeColor(), (float)System.Convert.ToSingle(parameter)));
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class DrawColorToMediaColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return Utils.SystemColorToMediaColor((System.Drawing.Color)value);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return Utils.MediaColorToSystemColor((Color)value);
        }
    }

    public class ReverseBoolConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return !(bool)value;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class BoolToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            parameter ??= "0";
            return (bool)value ? Visibility.Visible : ((string)parameter == "0" ? Visibility.Hidden : Visibility.Collapsed);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class PlayListTimeConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string file = value.ToString();
            return Utils.ParseTime((int)AudioInfoDataHelper.Get(file).Length);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
