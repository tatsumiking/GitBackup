using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace kden
{
    /// <summary>
    /// App.xaml の相互作用ロジック
    /// </summary>
    public partial class App : Application
    {
        public static string m_sArgv = "";
        public static string m_sExecPath = "";

        private void Application_Startup(object sender, StartupEventArgs e)
        {
            if (e.Args.Length == 0)
                return;
            m_sArgv = e.Args[0];
        }
        public static void LogOut(string msg)
        {
            if (m_sArgv != "")
            {

                string sFileName = m_sExecPath+"\\kden\\log.txt";
                System.IO.File.AppendAllText(sFileName, msg + "\r\n");
            }
        }
    }
}
