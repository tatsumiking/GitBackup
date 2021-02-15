using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace sden
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        public String m_sExePath;
        public String m_sEnvPath;
        public LibCommon m_libCmn;
        public LibExcel m_libExcel;
        public String m_sExcelSavePath;
        public int m_nWaitSecond;

        public MainWindow()
        {
            InitializeComponent();
            m_nWaitSecond = 3;
            m_sExePath = InitExePath();
            m_sEnvPath = InitEnvPath();
            m_libCmn = new LibCommon();
            m_libExcel = new LibExcel();
            LoadEnv();
            this.WindowState = WindowState.Minimized;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            String sDstXlsFile;
            int idx, max;

            CreateExcelFile csCreateExcelFile = new CreateExcelFile();
            sDstXlsFile = csCreateExcelFile.SetWndClass(this);
            if (csCreateExcelFile.Create() == false)
            {
                MessageBox.Show("Excelファイル作成に失敗しました。", "確認", MessageBoxButton.OK);
                this.Close();
            }

            Process.Start(sDstXlsFile);

            max = m_nWaitSecond;
            for (idx = 0; idx < max; idx++)
            {
                m_libCmn.DoDispatch();
                System.Threading.Thread.Sleep(1000);
            }

            this.Close();

        }
        private string InitExePath()
        {
            string path;

            Assembly myAssembly = Assembly.GetEntryAssembly();
            path = myAssembly.Location;

            return System.IO.Path.GetDirectoryName(path);
        }
        private string InitEnvPath()
        {
            return (m_sExePath);
        }
        public void DoDispatch()
        {
            DispatcherFrame frame = new DispatcherFrame();
            Dispatcher.CurrentDispatcher.BeginInvoke(DispatcherPriority.Background, new DispatcherOperationCallback(ExitFrames), frame);
            Dispatcher.PushFrame(frame);
        }
        public object ExitFrames(object frames)
        {
            ((DispatcherFrame)frames).Continue = false;
            return null;
        }
        private void LoadEnv()
        {
            string envfile;
            string[] aryLine;

            m_sExcelSavePath = "c:\\sms";
            envfile = m_sExePath + "\\sden.env";
            if (System.IO.File.Exists(envfile) == true)
            {
                aryLine = m_libCmn.LoadFileLineSJIS(envfile);
                m_sExcelSavePath = aryLine[1];
            }
            envfile = m_sExePath + "\\sdenwait.txt";
            if (System.IO.File.Exists(envfile) == true)
            {
                aryLine = m_libCmn.LoadFileLineSJIS(envfile);
                m_nWaitSecond = m_libCmn.StrToInt(aryLine[1]);
            }
        }
    }
}
