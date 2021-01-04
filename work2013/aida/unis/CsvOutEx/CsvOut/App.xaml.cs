using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;
using System.Windows;

namespace CsvOut
{
    /// <summary>
    /// App.xaml の相互作用ロジック
    /// </summary>
    public partial class App : Application
    {
        public static string m_sArgv = "";
        public static string m_sProgName = "csvout";
        private System.Threading.Mutex mutex = new System.Threading.Mutex(false, "ApplicationName");

        private void Application_Startup(object sender, StartupEventArgs e)
        {
            string[] aryLine;

            Assembly asm = System.Reflection.Assembly.GetExecutingAssembly();
            aryLine = asm.FullName.Split(',');
            if (!mutex.WaitOne(0, false))
            {
                // 既に起動しているため終了させる
                MessageBox.Show(aryLine[0] + "は既に起動しています。", "二重起動防止", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                mutex.Close();
                mutex = null;
                this.Shutdown();
            }
            try
            {
                if (e.Args.Length == 0)
                    return;
                m_sArgv = e.Args[0];
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }
        public static void LogOut(string msg)
        {
            try
            {
                if (m_sArgv != "")
                {
                    string sFileName = "c:\\UsesProgram\\" + m_sProgName + "\\" + m_sProgName + "log.txt";
                    System.IO.File.AppendAllText(sFileName, msg+"\r\n");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }
        // 正常に出力された勤怠データの件数ログ
        public static void OutLogAppend(string msg)
        {
            DateTime dt;
            string sYYYY;
            string sHead;
            string sFileName;

            try
            {
                dt = DateTime.Now;
                sYYYY = dt.ToString("yyyy");
                sFileName = "c:\\UsesProgram\\" + m_sProgName + "\\log\\" + sYYYY + "OutDataCount.log";
                if (!File.Exists(sFileName))
                {
                    System.IO.FileStream fs = System.IO.File.Create(sFileName);
                    fs.Close();
                    fs.Dispose();
                }
                sHead = "NORMAL," + dt.ToString("yyyy/MM/dd hh:mm:ss") + ",";
                System.IO.File.AppendAllText(sFileName, sHead + msg + "\r\n");
            }
            catch (Exception ex)
            {
                App.LogOut(ex.ToString());
            }
        }
        // 通常のExceptionを伴うエラーログ
        public static void ErrorLogAppend(string msg)
        {
            DateTime dt;
            string sYYYY;
            string sHead;
            string sFileName;

            try
            {
                dt = DateTime.Now;
                sYYYY = dt.ToString("yyyy");
                sFileName = "c:\\UsesProgram\\" + m_sProgName + "\\log\\" + sYYYY + "ExceptionError.log";
                if (!File.Exists(sFileName))
                {
                    System.IO.FileStream fs = System.IO.File.Create(sFileName);
                    fs.Close();
                    fs.Dispose();
                }
                sHead = "ERROR," + dt.ToString("yyyy/MM/dd hh:mm:ss") + ",";
                System.IO.File.AppendAllText(sFileName, sHead+msg + "\r\n");
            }
            catch (Exception ex)
            {
                App.LogOut(ex.ToString());
            }
        }
        // 勤怠エラーデータ保存ログファイル
        public static void ErrorDataAppend(string msg)
        {
            DateTime dt;
            string sYYYY;
            string sHead;
            string sFileName;

            try
            {
                dt = DateTime.Now;
                sYYYY = dt.ToString("yyyy");
                sFileName = "c:\\UsesProgram\\" + m_sProgName + "\\log\\" + sYYYY + "ErrorData.log";
                if (!File.Exists(sFileName))
                {
                    System.IO.FileStream fs = System.IO.File.Create(sFileName);
                    fs.Close();
                    fs.Dispose();
                }
                sHead = "ERROR," + dt.ToString("yyyy/MM/dd hh:mm:ss") + ",";
                System.IO.File.AppendAllText(sFileName, sHead+msg + "\r\n");
            }
            catch (Exception ex)
            {
                App.LogOut(ex.ToString());
            }
        }

        private void Application_Exit(object sender, ExitEventArgs e)
        {
            if (mutex != null)
            {
                mutex.ReleaseMutex();
                mutex.Close();
            }
        }
    }
}
