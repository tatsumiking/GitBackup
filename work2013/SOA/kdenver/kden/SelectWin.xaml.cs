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
    /// SelectWin.xaml の相互作用ロジック
    /// </summary>
    public partial class SelectWin : Window
    {
        private MainWindow m_wndMain;
        private HanbaiWin m_wndHanbai;
        private List<string> m_listSelect;

        public SelectWin()
        {
            InitializeComponent();
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            initSelectList();
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wndMain = wnd;
            m_wndHanbai = null;
        }
        public void SetHanbaiWindow(HanbaiWin wnd)
        {
            m_wndMain = null;
            m_wndHanbai = wnd;
        }
        public void SetSelectList(List<string> list)
        {
            m_listSelect = list;
        }
        private void initSelectList()
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
            if (m_wndMain != null)
            {
                m_wndMain.setTextSelectWin(sStr);
            }
            if (m_wndHanbai != null)
            {
                m_wndHanbai.setTextSelectWin(sStr);
            }
            this.Close();
        }
    }
}
