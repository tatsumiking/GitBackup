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

namespace CenterList
{
    /// <summary>
    /// CardWin.xaml の相互作用ロジック
    /// </summary>
    public partial class CardWin : Window
    {
        MainWindow m_wndMain;
        ClsCard m_clsCard;

        public CardWin()
        {
            InitializeComponent();
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wndMain = wnd;
        }
        public void SetClsCard(ClsCard clsCard)
        {
            m_clsCard = clsCard;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_clsCard == null)
            {
                txtSetNo.Text = "";
                txtIP.Text = "";
                txtSyoNo.Text = "";
                txtSyoName.Text = "";
                txtAddress1.Text = "";
                txtAddress2.Text = "";
                txtTel1.Text = "";
                txtTel2.Text = "";
                txtName.Text = "";
                txtBikou.Text = "";
            }
            else
            {
                txtSetNo.Text = m_clsCard.m_sSetNo;
                txtIP.Text = "";
                txtSyoNo.Text = m_clsCard.m_sSyoNo;
                txtSyoName.Text = m_clsCard.m_sSyoName;
                txtAddress1.Text = m_clsCard.m_sAddress1;
                txtAddress2.Text = m_clsCard.m_sAddress2;
                txtTel1.Text = m_clsCard.m_sTel1;
                txtTel2.Text = m_clsCard.m_sTel1;
                txtName.Text = m_clsCard.m_sName;
                txtBikou.Text = m_clsCard.m_sBikou;
            }
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            m_wndMain.m_cardWin = null;
            m_wndMain.m_nCardIdx = -1;
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            string sSetNo;

            sSetNo = txtSetNo.Text.ToString();
            m_wndMain.deleteCard(sSetNo);
            this.Close();
        }
    }
}
