using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace WpfUdpTest
{
    /// <summary>
    /// App.xaml の相互作用ロジック
    /// </summary>
    public partial class App : Application
    {
        private static string m_sLogFileName = "";

        public static void setLogFileName(string sFileName)
        {
            m_sLogFileName = sFileName;
        }
        public static void outLog(string msg)
        {
            string sDispMsg;
            DateTime dt;

            dt = DateTime.Now;
            sDispMsg = dt.ToString("yyyy/mm/dd hh:MM:ss\t") + msg + "\r\n";
            System.IO.File.AppendAllText(m_sLogFileName, sDispMsg);
        }
    }
}
