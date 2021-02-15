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
    /// MinSlctWin.xaml の相互作用ロジック
    /// </summary>
    public partial class MinSlctWin : Window
    {
        private SougiWin m_wndSougi;
        private SougiSeikyuWin m_wndSougiSeikyu;
        private List<string> m_listSelect;

        public MinSlctWin()
        {
            InitializeComponent();
        }
        public void SetSougiWin(SougiWin wnd)
        {
            m_wndSougi = wnd;
            m_wndSougiSeikyu = null;
        }
        public void SetSougiSeikyuWin(SougiSeikyuWin wnd)
        {
            m_wndSougi = null;
            m_wndSougiSeikyu = wnd;
        }
        public void SetSelectList(List<string> list)
        {
            m_listSelect = list;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            int max, idx;
            max = m_listSelect.Count;
            for (idx = 0; idx < max; idx++)
            {
                listSelect.Items.Add(m_listSelect[idx]);
            }
        }
        private void listSelect_PreviewMouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            this.Close();
        }

        private void listSelect_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int idx = listSelect.SelectedIndex;
            string sStr = listSelect.Items[idx].ToString();
            if (m_wndSougi != null)
            {
                m_wndSougi.setTextSelectWin(sStr);
            }
            if (m_wndSougiSeikyu != null)
            {
                m_wndSougiSeikyu.setTextSelectWin(sStr);
            }
            this.Close();
        }

    }
}
