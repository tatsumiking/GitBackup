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

namespace Observe
{
    /// <summary>
    /// CardWin.xaml の相互作用ロジック
    /// </summary>
    public partial class CardWin : Window
    {
        MainWindow m_wndMain;
        ClsCard m_clsCard;
        Boolean m_bCancelFlag;
        List<string> m_lstGlobalIp;
        string m_sSyoName;
        string m_sSyoIP;

        public CardWin()
        {
            m_lstGlobalIp = new List<string>();
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
        public void setCardLatLnd(double dLat, double dLnd)
        {
            m_clsCard.m_dLat = dLat;
            m_clsCard.m_dLnd = dLnd;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            initCmbSyoName();
            if (m_clsCard == null)
            {
                txtSetNo.Text = "";
                txtIP.Text = "";
                txtSyoNo.Text = "";
                cmbSyoName.Text = "";
                txtAddress1.Text = "";
                txtAddress2.Text = "";
                txtTel1.Text = "";
                txtTel2.Text = "";
                txtName.Text = "";
                txtBikou.Text = "";
            }
            else
            {
                m_sSyoIP = m_clsCard.m_sIP; 
                m_sSyoName = m_clsCard.m_sSyoName;
                txtSetNo.Text = m_clsCard.m_sSetNo;
                txtSyoNo.Text = m_clsCard.m_sSyoNo;
                cmbSyoName.Text = m_sSyoName;
                txtAddress1.Text = m_clsCard.m_sAddress1;
                txtAddress2.Text = m_clsCard.m_sAddress2;
                txtTel1.Text = m_clsCard.m_sTel1;
                txtTel2.Text = m_clsCard.m_sTel1;
                txtName.Text = m_clsCard.m_sName;
                txtBikou.Text = m_clsCard.m_sBikou;
            }
            m_bCancelFlag = true;
        }
        private void m_cardWin_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (m_bCancelFlag == true)
            {
                m_wndMain.RemoveNewCard();
            }
            m_wndMain.m_cardWin = null;
        }
        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            MessageBoxResult result;

            result = MessageBox.Show("削除してよろしいですか?", "確認", MessageBoxButton.OKCancel);
            if (result == MessageBoxResult.Cancel)
            {
                return;
            }
            m_wndMain.DeleteCrtCard();
            m_bCancelFlag = false;
            this.Close();
        }
        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void btnSetting_Click(object sender, RoutedEventArgs e)
        {
            String sSetNo;
            String sAddress;
            Boolean ret;

            if (txtSetNo.Text == "")
            {
                MessageBox.Show("通報装置番号が入力されていません", "確認", MessageBoxButton.OK);
                return;
            }
            sSetNo = txtSetNo.Text;
            if (m_wndMain.checkSamebSetNo(sSetNo) == true)
            {
                MessageBox.Show("同じ通報装置番号があります", "確認", MessageBoxButton.OK);
                return;
            }
            if (txtSyoNo.Text == "")
            {
                MessageBox.Show("通報装置署番号が入力されていません", "確認", MessageBoxButton.OK);
                return;
            }
            if (txtAddress1.Text == "")
            {
                MessageBox.Show("住所が入力されていません", "確認", MessageBoxButton.OK);
                return;
            }
            if (m_clsCard.m_dLat == 0 || m_clsCard.m_dLnd == 0)
            {
                sAddress = txtAddress1.Text;
                ret = moveAdrsLatLnd(sAddress);
                if (ret == false)
                {
                    return;
                }
            }
            m_clsCard.m_sSetNo = txtSetNo.Text;
            m_clsCard.m_sIP = m_sSyoIP;
            m_clsCard.m_sSyoNo = txtSyoNo.Text;
            m_clsCard.m_sSyoName = m_sSyoName;
            m_clsCard.m_sAddress1 = txtAddress1.Text;
            m_clsCard.m_sAddress2 = txtAddress2.Text;
            m_clsCard.m_sTel1 = txtTel1.Text;
            m_clsCard.m_sTel2 = txtTel2.Text;
            m_clsCard.m_sName = txtName.Text;
            m_clsCard.m_sBikou = txtBikou.Text;
            m_wndMain.SetCrtCardElement(m_clsCard);
            m_bCancelFlag = false;
            this.Close();
        }
        private void btnMovePos_Click(object sender, RoutedEventArgs e)
        {
            String sAddress;

            sAddress = txtAddress1.Text;
            if (sAddress == "")
            {
                MessageBox.Show("住所が入力されていません", "確認", MessageBoxButton.OK);
                return;
            }
            moveAdrsLatLnd(sAddress);
        }
        private void cmbSyoName_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int idx;

            idx = cmbSyoName.SelectedIndex;
            m_sSyoName = cmbSyoName.Items[idx].ToString();
            m_sSyoIP = m_lstGlobalIp[idx];
        }
        private void initCmbSyoName()
        {
            string sSyoToGlobalIPFile;
            string[] sLine;
            int max, idx;
            string[] ary;
            string sIP;

            sSyoToGlobalIPFile = m_wndMain.m_sExecPath + "\\SyoToGlobalIP.csv";
            sLine = m_wndMain.m_libCmn.LoadFileLineSJIS(sSyoToGlobalIPFile);

            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                ary = sLine[idx].Split(',');
                if (8 <= ary.Length && ary[1] != "" && ary[4] == "172")
                {
                    cmbSyoName.Items.Add(ary[1]);
                    sIP = ary[4] + "." + ary[5] + "." + ary[6] + "." + ary[7];
                    m_lstGlobalIp.Add(sIP);
                }
            }
        }
        private Boolean moveAdrsLatLnd(String sAddress)
        {
            ClsLatLnd clsLatLnd;
            clsLatLnd = m_wndMain.getAddressToLatLnd(sAddress);
            m_clsCard.m_dLat = clsLatLnd.m_dLat;
            m_clsCard.m_dLnd = clsLatLnd.m_dLnd;
            if (m_clsCard.m_dLat == 0 || m_clsCard.m_dLnd == 0)
            {
                MessageBox.Show("該当する住所が見つかりません", "確認", MessageBoxButton.OK);
                return(false);
            }
            m_wndMain.SetCrtCardLatLnd(clsLatLnd.m_dLat, clsLatLnd.m_dLnd);
            m_wndMain.moveLatLnd(clsLatLnd);
            return (true);
        }
    }
}
