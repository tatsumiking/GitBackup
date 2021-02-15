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

namespace sden
{
    /// <summary>
    /// ProgressWindows.xaml の相互作用ロジック
    /// </summary>
    public partial class ProgressWindows : Window
    {
        public ProgressWindows()
        {
            InitializeComponent();
            pgStep.Minimum = 0;
            pgStep.Maximum = 500;
            pgStep.Value = 0;
        }

        public void SetTitle(string sTitle)
        {
            string sSetTitle;

            sSetTitle = sTitle + "作成中．．．";
            this.Title = sSetTitle;
        }

        public void SetReng(int mini, int max)
        {
            pgStep.Minimum = mini;
            pgStep.Maximum = max;
            pgStep.Value = 0;
        }

        public void IncrementBar()
        {
            pgStep.Value++;
        }
    }
}
