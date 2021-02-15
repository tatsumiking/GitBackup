using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace kden
{
    /// <summary>
    /// CalWin.xaml の相互作用ロジック
    /// </summary>
    public partial class CalWin : Window
    {
        private MainWindow m_wndMain;
        private MasterWin m_wndMaster;
        public LibCommon m_libCmn;
        public string m_sCrtDate;

        public CalWin()
        {
            InitializeComponent();
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wndMain = wnd;
            m_libCmn = m_wndMain.m_libCmn;
            m_wndMaster = null;
        }
        public void SetMasterWindow(MasterWin wnd)
        {
            m_wndMain = null;
            m_wndMaster = wnd;
            m_libCmn = m_wndMaster.m_libCmn;
        }
        private void calMain_SelectedDatesChanged(object sender, SelectionChangedEventArgs e)
        {
            string sDate;

            sDate = calMain.SelectedDates[0].ToShortDateString();
            if (m_wndMain != null)
            {
                m_wndMain.SetSlctDate(sDate);
            }
            if (m_wndMaster != null)
            {
                m_wndMaster.SetSlctDate(sDate);
            }
            this.Close();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string[] ary;
            int yy, mm, dd;

            if (m_sCrtDate == "")
            {
                return;
            }
            yy = 2000; mm = 1; dd = 1;
            ary = m_sCrtDate.Split('/');
            if (1 <= ary.Length)
            {
                yy = m_libCmn.StrToInt(ary[0]);
                if (2 <= ary.Length)
                {
                    mm = m_libCmn.StrToInt(ary[1]);
                    if (3 <= ary.Length)
                    {
                        dd = m_libCmn.StrToInt(ary[2]);
                    }
                }
            }
            calMain.DisplayDate = new DateTime(yy, mm, dd);
        }

        private void Window_Closed(object sender, EventArgs e)
        {
        }
    }
}
