using Microsoft.Win32;
using PowerAudioPlayer.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace PowerAudioPlayer
{
    public static class Utils
    {
        public const uint SPI_GETDESKWALLPAPER = 0x0073;

        public static bool DelayFunctionInterrupt = false;
        public static bool IsDelaing = false;

        public enum DwmWindowAttribute : uint
        {
            NCRenderingEnabled = 1,
            NCRenderingPolicy,
            TransitionsForceDisabled,
            AllowNCPaint,
            CaptionButtonBounds,
            NonClientRtlLayout,
            ForceIconicRepresentation,
            Flip3DPolicy,
            ExtendedFrameBounds,
            HasIconicBitmap,
            DisallowPeek,
            ExcludedFromPeek,
            Cloak,
            Cloaked,
            FreezeRepresentation,
            PassiveUpdateMode,
            UseHostBackdropBrush,
            UseImmersiveDarkMode = 20,
            WindowCornerPreference = 33,
            BorderColor,
            CaptionColor,
            TextColor,
            VisibleFrameBorderThickness,
            SystemBackdropType,
            Last
        }

        [DllImport("user32.dll", EntryPoint = "SendMessageA")]
        public static extern int SendMessage(IntPtr hwnd, int wMsg, IntPtr wParam, IntPtr lParam);

        [DllImport("dwmapi.dll", PreserveSig = true)]
        public static extern int DwmSetWindowAttribute(IntPtr hwnd, DwmWindowAttribute attr, ref int attrValue, int attrSize);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool SystemParametersInfo(uint uAction, uint uParam, StringBuilder lpvParam, uint init);

        [DllImport("gdi32.dll", EntryPoint = "DeleteObject")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool DeleteObject([In] IntPtr hObject);

        public delegate void FindFileCallback(string file);
        public static List<string> FindFile(string path, string type = "*.*", bool searchSubDir = true, bool doEvents = false, FindFileCallback? callback = null)
        {
            if (!Directory.Exists(path))
            {
                return new List<string>();
            }
            List<string> Files = new List<string>();
            IEnumerable<string> files = Directory.EnumerateFiles(path, type, new EnumerationOptions
            {
                IgnoreInaccessible = true,
                RecurseSubdirectories = searchSubDir
            });
            IEnumerator<string> filesEnum = files.GetEnumerator();
            while (filesEnum.MoveNext())
            {
                if (doEvents)
                    System.Windows.Forms.Application.DoEvents();
                Files.Add(filesEnum.Current);
                callback?.Invoke(filesEnum.Current);
            }
            return Files;
        }

        public static bool IsUrl(string url)
        {
            if (Regex.IsMatch(url, @"^[A-Za-z]+://"))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public static string ParseTime(int sec, bool isMs = false)
        {
            if (isMs)
            {
                sec = sec / 1000;
            }
            if (sec < 0)
            {
                return "--:--";
            }
            string time1;
            string time2;
            if (sec / 60 < 10)
            {
                time1 = "0" + (sec / 60).ToString();
            }
            else
            {
                time1 = (sec / 60).ToString();
            }
            if (sec % 60 < 10)
            {
                time2 = "0" + (sec % 60).ToString();
            }
            else
            {
                time2 = (sec % 60).ToString();
            }
            return time1 + ":" + time2;
        }

        public static byte[] ImageToByte(System.Drawing.Image image)
        {
            MemoryStream ms = new MemoryStream();
            image.Save(ms, System.Drawing.Imaging.ImageFormat.Jpeg);
            return ms.ToArray();
        }

        public static Color GetSystemThemeColor()
        {
            if (!Settings.Default.ThemeColorFollowSystem)
            {
                return SystemColorToMediaColor(Settings.Default.ThemeColor);
            }
            int value = Convert.ToInt32(Registry.GetValue("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\DWM", "ColorizationColor", 0));
            return Color.FromArgb((byte)(value >> 24), (byte)(value >> 16), (byte)(value >> 8), (byte)value);
        }

        public static Color ChangeColorLight(Color color, float correctionFactor)
        {
            float red = color.R;
            float green = color.G;
            float blue = color.B;
            if (correctionFactor < 0)
            {
                correctionFactor = 1 + correctionFactor;
                red *= correctionFactor;
                green *= correctionFactor;
                blue *= correctionFactor;
            }
            else
            {
                red = (255 - red) * correctionFactor + red;
                green = (255 - green) * correctionFactor + green;
                blue = (255 - blue) * correctionFactor + blue;
            }
            if (red < 0) red = 0;
            if (red > 255) red = 255;
            if (green < 0) green = 0;
            if (green > 255) green = 255;
            if (blue < 0) blue = 0;
            if (blue > 255) blue = 255;
            //return Color.FromArgb(color.A, (byte)(int)red, (byte)(int)green, (byte)(int)blue);
            return Color.FromRgb((byte)(int)red, (byte)(int)green, (byte)(int)blue);
        }

        public static Color SystemColorToMediaColor(System.Drawing.Color color)
        {
            return Color.FromArgb(color.A, color.R, color.G, color.B);
        }

        public static System.Drawing.Color MediaColorToSystemColor(Color color)
        {
            return System.Drawing.Color.FromArgb(color.A, color.R, color.G, color.B);
        }

        public static bool IsDarkMode()
        {
            if (Settings.Default.ColorMode != 0)
            {
                if (Settings.Default.ColorMode == 1)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            const string registryKeyPath = @"Software\Microsoft\Windows\CurrentVersion\Themes\Personalize";
            const string registryValueName = "AppsUseLightTheme";
            object? registryValueObject = Registry.CurrentUser.OpenSubKey(registryKeyPath)?.GetValue(registryValueName);
            if (registryValueObject is null) return false;
            return (int)registryValueObject > 0 ? false : true;
        }

        public static long GetFileSize(string sFullName)
        {
            long lSize = 0;
            if (File.Exists(sFullName))
            {
                lSize = new FileInfo(sFullName).Length;
            }
            return lSize;
        }

        public static string CountSize(long factSize)
        {
            string strSize = "";
            if (factSize < 1024.00)
            {
                strSize = factSize.ToString("F2") + " Byte";
            }
            else if (factSize >= 1024.00 && factSize < 1048576)
            {
                strSize = (factSize / 1024.00).ToString("F2") + " KB";
            }
            else if (factSize >= 1048576 && factSize < 1073741824)
            {
                strSize = (factSize / 1024.00 / 1024.00).ToString("F2") + " MB";
            }
            else if (factSize >= 1073741824)
            {
                strSize = (factSize / 1024.00 / 1024.00 / 1024.00).ToString("F2") + " GB";
            }
            return strSize;
        }

        public static BitmapImage ByteArrayToBitmapImage(byte[] array)
        {
            using (var ms = new System.IO.MemoryStream(array))
            {
                var image = new BitmapImage();
                image.BeginInit();
                image.CacheOption = BitmapCacheOption.OnLoad; // here
                image.StreamSource = ms;
                image.EndInit();
                image.Freeze();
                return image;
            }
        }

        public static bool EnableDarkModeForWindow(IntPtr hWnd, bool enable)
        {
            int darkMode = enable ? 1 : 0;
            int hr = DwmSetWindowAttribute(hWnd, DwmWindowAttribute.UseImmersiveDarkMode, ref darkMode, sizeof(int));
            return hr >= 0;
        }

        public static string GetWallpaper()
        {
            StringBuilder wallPaperPath = new StringBuilder(200);
            SystemParametersInfo(SPI_GETDESKWALLPAPER, 200, wallPaperPath, 0);
            return wallPaperPath.ToString();
        }

        public static bool DelayWithFlag(int mm)
        {
            DateTime current = DateTime.Now;
            IsDelaing = true;
            while (current.AddMilliseconds(mm) > DateTime.Now)
            {
                if (DelayFunctionInterrupt)
                {
                    DelayFunctionInterrupt = false;
                    IsDelaing = false;
                    return false;
                }
                System.Windows.Forms.Application.DoEvents();
            }
            IsDelaing = false;
            return true;
        }

        public static bool Delay(int mm)
        {
            DateTime current = DateTime.Now;
            while (current.AddMilliseconds(mm) > DateTime.Now)
            {
                System.Windows.Forms.Application.DoEvents();
            }
            return true;
        }

        public static List<T> Swap<T>(List<T> list, int index1, int index2)
        {
            var temp = list[index1];
            list[index1] = list[index2];
            list[index2] = temp;
            return list;
        }

        public static void SetPropertyValue(object obj, string propertyName, string sValue)
        {
            PropertyInfo? p = obj.GetType().GetProperty(propertyName, BindingFlags.Public | BindingFlags.Instance | BindingFlags.IgnoreCase);
            if (p != null)
            {
                object? dynmicValue;

                if (p.PropertyType.IsArray)//数组类型,单独处理
                {
                    p.SetValue(obj, sValue, null);
                }
                else
                {
                    //根据属性类型动态转换属性的值
                    if (string.IsNullOrEmpty(sValue.ToString()))//空值
                        dynmicValue = p.PropertyType.IsValueType ? Activator.CreateInstance(p.PropertyType) : null;//值类型
                    else
                        dynmicValue = System.ComponentModel.TypeDescriptor.GetConverter(p.PropertyType).ConvertFromString(sValue.ToString());//创建对象

                    //调用属性的SetValue方法赋值
                    p.SetValue(obj, dynmicValue, null);
                }
            }
        }
    }

    public static class FindChirldHelper
    {
        public static List<T> FindVisualChild<T>(DependencyObject obj) where T : DependencyObject
        {
            try
            {
                List<T> TList = new List<T> { };
                for (int i = 0; i < VisualTreeHelper.GetChildrenCount(obj); i++)
                {
                    DependencyObject child = VisualTreeHelper.GetChild(obj, i);
                    if (child != null && child is T)
                    {
                        TList.Add((T)child);
                        List<T> childOfChildren = FindVisualChild<T>(child);
                        if (childOfChildren != null)
                        {
                            TList.AddRange(childOfChildren);
                        }
                    }
                    else
                    {
                        List<T> childOfChildren = FindVisualChild<T>(child);
                        if (childOfChildren != null)
                        {
                            TList.AddRange(childOfChildren);
                        }
                    }
                }
                return TList;
            }
            catch (Exception)
            {
                return null;
            }
        }
    }

    public class PNotifyPropertyChanged : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        public void RaisePropertyChanged(string p)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(p));
            }
        }
        public void RaiseAndSetIfChanged<T>(ref T a, T v, [CallerMemberName] string propertyName = null)
        {
            a = v;
            if (propertyName != null)
            {
                RaisePropertyChanged(propertyName);
            }
        }
    }
}