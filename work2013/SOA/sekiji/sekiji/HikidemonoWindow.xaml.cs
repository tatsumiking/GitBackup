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

namespace sekiji
{
    /// <summary>
    /// HikidemonoWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class HikidemonoWindow : Window
    {
        private MainWindow m_wndMain;
        private string m_sExecPath;
        private string m_sEnvPath;
        private LibCommon m_libCmn;
        private List<string> m_lstSyouhin;

        public HikidemonoWindow()
        {
            InitializeComponent();
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wndMain = wnd;
            m_libCmn = m_wndMain.m_libCmn;
            m_sEnvPath = m_wndMain.m_sEnvPath;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int idx, max;

            sFileName = m_sEnvPath + "\\Hikidemono.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    lstSource.Items.Add(sLine[idx]);
                }
            }
            max = m_lstSyouhin.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (m_lstSyouhin[idx] != "")
                {
                    lstSelect.Items.Add(m_lstSyouhin[idx]);
                }
            }
        }
        public void setSyouhinList(List<string> lstSyouhin)
        {
            m_lstSyouhin = lstSyouhin;
        }
        private void imgTuika_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            int idx, max;
            string str;

            max = lstSource.SelectedItems.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (5 <= lstSelect.Items.Count)
                {
                    return;
                }
                str = lstSource.SelectedItems[idx].ToString();
                lstSelect.Items.Add(str);
            }
        }
        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            int idx, max, i;
            Object obj;

            max = lstSelect.SelectedItems.Count;
            for (idx = max-1; idx >= 0; idx--)
            {
                obj = lstSelect.SelectedItems[idx];
                lstSelect.Items.Remove(obj);
            }
        }
        private void btnRet_Click(object sender, RoutedEventArgs e)
        {
            int idx, max, i;
            string str;

            m_lstSyouhin.Clear();
            max = lstSelect.Items.Count;
            for (idx = 0; idx < max; idx++)
            {
                str = lstSelect.Items[idx].ToString();
                m_lstSyouhin.Add(str);
            }
            for (; idx < Constants.HIKIDEMONOMAX; idx++)
            {
                m_lstSyouhin.Add("");
            }
            m_wndMain.setCrtHikimonoList(m_lstSyouhin);
            this.Close();
        }
        private void btnCanCel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void btnResetSelect_Click(object sender, RoutedEventArgs e)
        {
            lstSource.SelectedIndex = -1;
        }

    }
}
