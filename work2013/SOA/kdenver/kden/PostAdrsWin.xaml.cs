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
    /// PostAdrsWin.xaml の相互作用ロジック
    /// </summary>
    public partial class PostAdrsWin : Window
    {
        public List<String> m_lstAdrs;
        public string m_sSlctAdrs;
        public PostAdrsWin()
        {
            InitializeComponent();
        }
        public void setList(List<string> lst)
        {
            m_lstAdrs = lst;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            lstAdrs.ItemsSource = m_lstAdrs;
        }
        private void lstAdrs_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int idx = lstAdrs.SelectedIndex;
            m_sSlctAdrs = m_lstAdrs[idx];
            this.Close();
        }

    }
}
