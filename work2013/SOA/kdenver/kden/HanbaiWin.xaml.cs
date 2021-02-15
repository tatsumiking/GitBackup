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
    public static class UIElementBehavior
    {
        // 添付プロパティの初期値は false。
        // コールバックを指定しておく。
        public static readonly DependencyProperty EnterCommand = DependencyProperty.RegisterAttached("EnterCommand",
            typeof(bool),
            typeof(UIElementBehavior),
            new UIPropertyMetadata(false, EnterCommandChanged));

        public static bool GetEnterCommand(DependencyObject obj)
        {
            return (bool)obj.GetValue(EnterCommand);
        }

        public static void SetEnterCommand(DependencyObject obj, bool value)
        {
            obj.SetValue(EnterCommand, value);
        }

        // EnterCommand の値が変更されたときに呼び出される。
        // KeyDown イベントハンドラの登録＆解除を行う。
        public static void EnterCommandChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            UIElement element = sender as UIElement;
            if (element == null)
            {
                return;
            }

            if (GetEnterCommand(element))
            {
                element.KeyDown += textBox_KeyDown;
            }
            else
            {
                element.KeyDown -= textBox_KeyDown;
            }
        }

        // Enter キーが押されたら、次のコントロールにフォーカスを移動する
        private static void textBox_KeyDown(object sender, KeyEventArgs e)
        {
            if ((Keyboard.Modifiers == ModifierKeys.None) && (e.Key == Key.Enter))
            {
                UIElement element = sender as UIElement;
                element.MoveFocus(new TraversalRequest(FocusNavigationDirection.Next));
            }
        }
    }
    /// <summary>
    /// HanbaiWin.xaml の相互作用ロジック
    /// </summary>
    public partial class HanbaiWin : Window
    {
        private MainWindow m_wnd;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibOdbc m_libOdbc;
        public LibPost m_libPost;
        private Boolean m_bModify;
        private string m_sMsg;
        private int m_nSelectWinFlag;
        private int m_nCrtId;
        private int m_nPrevId;
        private int m_nNextId;
        private string m_sRecsCsv;
        private string m_sSeikyuid;
        private string m_sSougiid;
        private string m_sReceivedat;
        private string m_sReceivekind;
        private string m_sVendername;
        private List<string> m_lstSouke;
        private string[] m_sTaxKubunLine;
        private string[] m_sKeisyouLine;
        private List<string> m_lstSeikyuKubun;
        private int m_nSeikyusakiId;

        public HanbaiWin()
        {
            InitializeComponent();
            m_nSelectWinFlag = 0;
            m_nSeikyusakiId = 0;
            m_bModify = false;
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wnd = wnd;
            m_sExecPath = m_wnd.m_sExecPath;
            m_libCmn = m_wnd.m_libCmn;
            m_libOdbc = m_wnd.m_libOdbc;
            m_libPost = m_wnd.m_libPost;
        }
        public void SetRecsCsv(string sRecs)
        {
            m_sRecsCsv = sRecs;
        }
        public void SetSelectCrtId(int nId)
        {
            m_nCrtId = nId;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            initCmbTaxKubun();
            initCmbKeisyou();
            initCmbSeikyuKubun();
            setPrevNextButton();
            setElementText();
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (m_bModify == true)
            {
                MessageBoxResult ret = MessageBox.Show("データが更新されています、保存しますか？", "確認", MessageBoxButton.OKCancel);
                if (ret == MessageBoxResult.OK)
                {
                    updateHanbaiRecord();
                }
            }
            m_wnd.updateDataGrid();
        }
        public void setTextSelectWin(string str)
        {
            string[] ary;

            ary = str.Split(',');
            if (m_nSelectWinFlag == 1)
            {
                txtSouke.Text = ary[0];
                txtKokubetu.Text = ary[3];
            }
        }
        public void setSeikyusaki(int nId, string sData)
        {
            string[] ary;

            m_nSeikyusakiId = nId;
            ary = sData.Split(',');
            txtSeikyuSaki.Text = ary[0];
            txtSeikyuBusyo.Text = ary[1];
            cmbSKeisyou.Text = ary[2];
            cmbAKeisyou.Text = ary[3];
            txtSeikyuPost.Text = ary[4];
            txtSeikyuAdrs1.Text = ary[5];
            txtSeikyuAdrs2.Text = ary[6];
            txtSeikyuTel.Text = ary[7];
            txtSeikyuFax.Text = ary[8];
            txtSeikyuPay.Text = ary[9];
        }
        private void setElementText()
        {
            string sFilds;
            string[] aryFild;

            sFilds = m_libOdbc.getHanbaiRecord(m_nCrtId);
            aryFild = sFilds.Split(',');
            txtJyutyuNo.Text = aryFild[0];
            txtOrderDate.Text = aryFild[2];
            m_sSeikyuid = aryFild[3];
            m_sSougiid = aryFild[4];
            txtSouke.Text = aryFild[5];
            txtKokubetu.Text = aryFild[6];

            txtSyouhin.Text = aryFild[7];
            txtSrcTanka.Text = aryFild[8];
            txtTanka.Text = aryFild[8];
            txtSrcTani.Text = aryFild[9];
            txtTani.Text = aryFild[9];
            txtKosu.Text = aryFild[10];
            cmbSrcTaxKubun.Text = aryFild[11];
            cmbTaxKubun.Text = aryFild[11];
            txtTax.Text = aryFild[12];
            txtPrice.Text = aryFild[13];

            txtSeikyuSaki.Text = aryFild[14];
            txtSeikyuBusyo.Text = aryFild[15];
            cmbSKeisyou.Text = aryFild[16];
            if (cmbSKeisyou.Text == "")
            {
                cmbSKeisyou.Text = "様";
            }
            cmbAKeisyou.Text = aryFild[17];
            if (cmbAKeisyou.Text == "")
            {
                cmbAKeisyou.Text = "様";
            }
            txtSeikyuPost.Text = aryFild[18];
            txtSeikyuAdrs1.Text = aryFild[19];
            txtSeikyuAdrs2.Text = aryFild[20];
            txtSeikyuTel.Text = aryFild[21];
            txtSeikyuFax.Text = aryFild[22];
            txtSeikyuPay.Text = aryFild[23];
            cmbSeikyuKubun.Text = aryFild[24];

            m_sReceivedat = aryFild[25];
            m_sReceivekind = aryFild[26];
            m_sVendername = aryFild[27];

            txtNafuda1.Text = aryFild[28];
            txtNafuda2.Text = aryFild[29];
            txtNafuda3.Text = aryFild[30];
            txtNafuda4.Text = aryFild[31];
            txtNafuda5.Text = aryFild[32];
            txtNafuda6.Text = aryFild[33];
            txtNafuda7.Text = aryFild[34];
            txtNafuda8.Text = aryFild[35];
            txtBikou.Text = aryFild[36]; 
        }
        private string getElementText()
        {
            string sCrt;
            string sData;

            DateTime dt = DateTime.Now;
            sCrt = dt.ToString("yyyy/MM/dd");

            sData = txtJyutyuNo.Text; // 0
            sData = sData + "," + sCrt; // 1
            sData = sData + "," + txtOrderDate.Text; // 2
            sData = sData + "," + m_sSeikyuid; // 3
            sData = sData + "," + m_sSougiid; // 4
            sData = sData + "," + txtSouke.Text; // 5
            sData = sData + "," + txtKokubetu.Text; // 6
            sData = sData + "," + txtSyouhin.Text; // 7
            sData = sData + "," + txtTanka.Text; // 8
            sData = sData + "," + txtTani.Text; // 9
            sData = sData + "," + txtKosu.Text; // 10
            sData = sData + "," + cmbTaxKubun.Text; // 11
            sData = sData + "," + txtTax.Text; // 12
            sData = sData + "," + txtPrice.Text; // 13
            sData = sData + "," + txtSeikyuSaki.Text; // 14
            sData = sData + "," + txtSeikyuBusyo.Text; // 15
            sData = sData + "," + cmbSKeisyou.Text; // 16
            sData = sData + "," + cmbAKeisyou.Text; // 17
            sData = sData + "," + txtSeikyuPost.Text; // 18
            sData = sData + "," + txtSeikyuAdrs1.Text; // 19
            sData = sData + "," + txtSeikyuAdrs2.Text; // 20
            sData = sData + "," + txtSeikyuTel.Text; // 21
            sData = sData + "," + txtSeikyuFax.Text; // 22
            sData = sData + "," + txtSeikyuPay.Text; // 23
            sData = sData + "," + cmbSeikyuKubun.Text; // 24
            sData = sData + "," + m_sReceivedat; // 25
            sData = sData + "," + m_sReceivekind; // 26
            sData = sData + "," + m_sVendername; // 27
            sData = sData + "," + txtNafuda1.Text; // 28
            sData = sData + "," + txtNafuda2.Text; // 29
            sData = sData + "," + txtNafuda3.Text; // 30
            sData = sData + "," + txtNafuda4.Text; // 31
            sData = sData + "," + txtNafuda5.Text; // 32
            sData = sData + "," + txtNafuda6.Text; // 33
            sData = sData + "," + txtNafuda7.Text; // 34
            sData = sData + "," + txtNafuda8.Text; // 35
            sData = sData + "," + txtBikou.Text; // 36
            return (sData);
        }
        private void initCmbTaxKubun()
        {
            string sFileName;
            int max, idx;
            String[] sAry;

            cmbSrcTaxKubun.Items.Clear();
            sFileName = m_sExecPath + "\\kden\\taxkubun.txt";
            m_sTaxKubunLine = m_libCmn.LoadFileLineSJIS(sFileName);
            sAry = m_sTaxKubunLine[0].Split(',');
            txtSrcTaxPar.Text = sAry[1];
            max = m_sTaxKubunLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                sAry = m_sTaxKubunLine[idx].Split(',');
                if (sAry[0] != "")
                {
                    cmbSrcTaxKubun.Items.Add(sAry[0]);
                    cmbTaxKubun.Items.Add(sAry[0]);
                }
            }
        }
        private void initCmbKeisyou()
        {
            string sFileName;
            int max, idx;
            String[] sAry;

            cmbSKeisyou.Items.Clear();
            cmbAKeisyou.Items.Clear();
            sFileName = m_sExecPath + "\\kden\\keisyou.txt";
            m_sKeisyouLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = m_sKeisyouLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                sAry = m_sKeisyouLine[idx].Split(',');
                if (2 <= sAry.Length)
                {
                    cmbSKeisyou.Items.Add(sAry[0]);
                    cmbAKeisyou.Items.Add(sAry[0]);
                }
            }
            cmbSKeisyou.SelectedIndex = 0;
            cmbAKeisyou.SelectedIndex = 0;
        }
        private void initCmbSeikyuKubun()
        {
            int max, idx;
            String[] sAry;

            cmbSeikyuKubun.Items.Clear();
            m_lstSeikyuKubun = m_libOdbc.getSkkubunList("");
            max = m_lstSeikyuKubun.Count;
            for (idx = 1; idx < max; idx++)
            {
                sAry = m_lstSeikyuKubun[idx].Split(',');
                if (4 <= sAry.Length)
                {
                    cmbSeikyuKubun.Items.Add(sAry[1]);
                }
            }
            cmbSeikyuKubun.SelectedIndex = 0;
        }

        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key.ToString() == "F5")
            {
                showSyouhinSearchWin();
            }
            else if (e.Key.ToString() == "F7")
            {
                showSeikyuSakiSelectWin();
            }
        }
        private void btnSouke_Click(object sender, RoutedEventArgs e)
        {
            showSoukeSearchWin();
        }
        private void btnSelectSyouhin_Click(object sender, RoutedEventArgs e)
        {
            showSyouhinSearchWin();
        }
        private void btnSelectSeikyuSaki_Click(object sender, RoutedEventArgs e)
        {
            showSeikyuSakiSelectWin();
        }

        private void btnClsSeikyuSaki_Click(object sender, RoutedEventArgs e)
        {
            m_nSeikyusakiId = 0;
            txtSeikyuSaki.Text = "";
            txtSeikyuBusyo.Text = "";
            txtSeikyuPost.Text = "";
            txtSeikyuAdrs1.Text = "";
            txtSeikyuAdrs2.Text = "";
            txtSeikyuTel.Text = "";
            txtSeikyuFax.Text = "";
            txtSeikyuPay.Text = "";
        }
        private void cmbTaxKubun_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int idx;

            idx = cmbTaxKubun.SelectedIndex;
            cmbTaxKubun.Text = m_sTaxKubunLine[idx + 1];
            CalcTaxPriceText();
        }
        private void btnSetToday_Click(object sender, RoutedEventArgs e)
        {
            DateTime dt = DateTime.Now;
            txtKokubetu.Text = dt.ToString("yyyy/MM/dd");
        }

        private void btnSetTomorrow_Click(object sender, RoutedEventArgs e)
        {
            DateTime dt = DateTime.Now;
            dt = dt.AddDays(1);
            txtKokubetu.Text = dt.ToString("yyyy/MM/dd");
        }

        private void btnSetAfterTomorrow_Click(object sender, RoutedEventArgs e)
        {
            DateTime dt = DateTime.Now;
            dt = dt.AddDays(2);
            txtKokubetu.Text = dt.ToString("yyyy/MM/dd");
        }

        private void btnPrevRecord_Click(object sender, RoutedEventArgs e)
        {
            if (checkCancelModify() == true)
            {
                return;
            }
            m_nCrtId = m_nPrevId;
            setElementText();
            setPrevNextButton();
        }

        private void btnNextRecord_Click(object sender, RoutedEventArgs e)
        {
            if (checkCancelModify() == true)
            {
                return;
            }
            m_nCrtId = m_nNextId;
            setElementText();
            setPrevNextButton();
        }

        private void btnCopyRecord_Click(object sender, RoutedEventArgs e)
        {
            string sData;
            int nId;

            if (checkCancelModify() == true)
            {
                return;
            }
            sData = getElementText();
            nId = m_libOdbc.insertHanbai(sData);
            m_sRecsCsv = m_sRecsCsv + "," + nId;
            m_nCrtId = nId;
            setElementText();
            setPrevNextButton();
        }
        private void btnDeleteRecord_Click(object sender, RoutedEventArgs e)
        {
            string[] aryRecs;
            int max, idx, setidx;
            int id;

            if (checkCancelModify() == true)
            {
                return;
            }
            m_libOdbc.deleteHanbai(m_nCrtId);
            aryRecs = m_sRecsCsv.Split(',');
            max = aryRecs.Length;
            m_sRecsCsv = "";
            setidx = 0;
            for (idx = 0; idx < max; idx++)
            {
                id = m_libCmn.StrToInt(aryRecs[idx]);
                if (m_nCrtId != id)
                {
                    if (setidx == 0)
                    {
                        m_sRecsCsv = m_sRecsCsv + aryRecs[idx];
                    }
                    else
                    {
                        m_sRecsCsv = m_sRecsCsv + "," + aryRecs[idx];
                    }
                }
            }

            if (m_nNextId != -1)
            {
                m_nCrtId = m_nNextId;
            }
            else if (m_nPrevId != -1)
            {
                m_nCrtId = m_nPrevId;
            }
            else
            {
                m_sMsg = "操作可能なレコードがないため終了します";
                System.Diagnostics.Trace.WriteLine(m_sMsg);
                this.Close();
            }
            setElementText();
            setPrevNextButton();
        }

        private void btnUpdateRecord_Click(object sender, RoutedEventArgs e)
        {
            updateHanbaiRecord();
        }
        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private Boolean checkCancelModify()
        {
            if (m_bModify == true)
            {
                MessageBoxResult ret = MessageBox.Show("データが更新されています、保存しますか？", "確認", MessageBoxButton.OKCancel);
                if (ret == MessageBoxResult.OK)
                {
                    updateHanbaiRecord();
                }
                else
                {
                    return (true);
                }
            }
            return (false);
        }
        private void updateHanbaiRecord()
        {
            string sData;
            string sCsv;

            DateTime dt = DateTime.Now;
            string sCrtymdhm = dt.ToString("yyyy/MM/dd HH:mm");

            sData = getElementText();
            m_libOdbc.updateHanbai(sData);
            if (txtSeikyuSaki.Text != "")
            {
                m_nSeikyusakiId = searchSeikyuusakiId(txtSeikyuSaki.Text, txtSeikyuBusyo.Text);
                sCsv = m_nSeikyusakiId+"," + sCrtymdhm + "," + txtSeikyuSaki.Text + "," + txtSeikyuBusyo.Text;
                sCsv = sCsv + "," + cmbSKeisyou.Text + "," + cmbAKeisyou.Text;
                sCsv = sCsv + "," + txtSeikyuPost.Text;
                sCsv = sCsv + "," + txtSeikyuAdrs1.Text + "," + txtSeikyuAdrs2.Text;
                sCsv = sCsv + "," + txtSeikyuTel.Text + "," + txtSeikyuFax.Text;
                sCsv = sCsv + "," + txtSeikyuPay.Text + ",,";
                if (m_nSeikyusakiId == 0)
                {
                    m_nSeikyusakiId = m_libOdbc.insertSeikyusaki(sCsv);
                }
                else
                {
                    m_libOdbc.updateSeikyusaki(sCsv);
                }
            }
            m_bModify = false;
        }
        private int searchSeikyuusakiId(string sSeikyusaki, string sBusyo)
        {
            string sCondision;
            List<string> lstLine;
            string[] ary;
            int nId;

            sCondision = "(name) = '" + sSeikyusaki + "' AND (busyo)='" + sBusyo + "'";
            lstLine = m_libOdbc.getSeikyusakiList(sCondision);
            if (lstLine.Count != 0)
            {
                ary = lstLine[0].Split(',');
                nId = m_libCmn.StrToInt(ary[0]);
                return(nId);
            }
            return (0);
        }
        private void setPrevNextButton()
        {
            string[] aryRecs;
            int max, idx;
            int id;

            aryRecs = m_sRecsCsv.Split(',');
            max = aryRecs.Length;
            for (idx = 0; idx < max; idx++)
            {
                id = m_libCmn.StrToInt(aryRecs[idx]);
                if (m_nCrtId == id)
                {
                    break;
                }
            }
            if (idx == max)
            {
                m_nPrevId = -1;
                m_nNextId = -1;
                btnPrevRecord.IsEnabled = false;
                btnPrevRecord.Content = "前データ";
                btnNextRecord.IsEnabled = false;
                btnNextRecord.Content = "次データ";
            }else{
                if(idx == 0){
                    m_nPrevId = -1;
                    btnPrevRecord.IsEnabled = false;
                    btnPrevRecord.Content="前データ";
                }else{
                    m_nPrevId = m_libCmn.StrToInt(aryRecs[idx - 1]);
                    btnPrevRecord.IsEnabled = true;
                    btnPrevRecord.Content="["+ aryRecs[idx-1]+"]←前データ";
                }
                if(max <= idx+1){
                    m_nNextId = -1;
                    btnNextRecord.IsEnabled = false;
                    btnNextRecord.Content = "次データ";
                }else{
                    m_nNextId = m_libCmn.StrToInt(aryRecs[idx + 1]);
                    btnNextRecord.IsEnabled = true;
                    btnNextRecord.Content = "次データ→[" + aryRecs[idx + 1] + "]";
                }
            }
        }
        private void showSyouhinSearchWin()
        {
            SyouhinSearchWin syouhinSearchWin;

            syouhinSearchWin = new SyouhinSearchWin();
            syouhinSearchWin.SetHanbaiWindow(this);
            Point pt = btnSelectSyouhin.PointToScreen(new Point(0.0d, 0.0d));
            syouhinSearchWin.Left = pt.X;
            syouhinSearchWin.Top = pt.Y;
            syouhinSearchWin.Owner = this;
            syouhinSearchWin.ShowDialog();
        }
        public void settxtSetSyouhin(string sData)
        {
            string[] ary;

            ary = sData.Split(',');
            txtSyouhin.Text = ary[0];
            txtSrcTanka.Text = ary[1];
            cmbSrcTaxKubun.Text = ary[3];
            txtSrcTani.Text = ary[2];
            txtTanka.Text = ary[1];
            cmbTaxKubun.Text = ary[3];
            txtTani.Text = ary[2];
            txtKosu.Text = "1";
            CalcTaxPriceText();
        }
        private void CalcTaxPriceText()
        {
            string sKubun;
            int nTanka;
            int nCount;
            double dTaxPar;
            string[] ary;
            string sStr;

            sKubun = cmbTaxKubun.Text;
            nTanka = m_libCmn.StrToInt(txtTanka.Text);
            nCount = m_libCmn.StrToInt(txtKosu.Text);
            dTaxPar = m_libCmn.StrToDouble(txtSrcTaxPar.Text);
            sStr = m_libCmn.CalcTaxPrice(sKubun, nTanka, nCount, dTaxPar);
            ary = sStr.Split(',');
            txtTax.Text = ary[0];
            txtPrice.Text = ary[1];
        }
        private void showSoukeSearchWin()
        {
            List<string> listSougi;
            int max, idx;
            String[] sAry;
            string sSetStr;

            m_lstSouke = new List<string>();
            listSougi = m_libOdbc.getSougiList("");
            max = listSougi.Count;
            for (idx = 0; idx < max; idx++)
            {
                sAry = listSougi[idx].Split(',');
                sSetStr = sAry[3] + "," + sAry[10] + "," + sAry[4] + "," + sAry[7] + "," + sAry[0];
                m_lstSouke.Add(sSetStr);
            }
            m_nSelectWinFlag = 1;
            showSelectWin(m_lstSouke, btnSouke);
        }
        private void showSeikyuSakiSelectWin()
        {
            SeikyusakiSearchWin seikyusakiSearchWin;

            seikyusakiSearchWin = new SeikyusakiSearchWin();
            seikyusakiSearchWin.SetHanbaiWindow(this);
            Point pt = gridTopPos.PointToScreen(new Point(0.0d, 0.0d));
            seikyusakiSearchWin.Left = pt.X;
            seikyusakiSearchWin.Top = pt.Y;
            seikyusakiSearchWin.Owner = this;
            seikyusakiSearchWin.ShowDialog();
        }
        private void showSelectWin(List<string> listSelect, Control control)
        {
            SelectWin selectWin;

            selectWin = new SelectWin();
            selectWin.SetHanbaiWindow(this);
            selectWin.SetSelectList(listSelect);
            Point pt = control.PointToScreen(new Point(0.0d, 0.0d));
            selectWin.Left = pt.X;
            selectWin.Top = pt.Y;
            selectWin.Owner = this;
            selectWin.ShowDialog();
        }
        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            m_bModify = true;
        }

        private void btnAdrsSearch_Click(object sender, RoutedEventArgs e)
        {
            int nPost;
            string sAdrs;

            nPost = m_libCmn.StrToInt(txtSeikyuPost.Text);
            Point pt = btnAdrsSearch.PointToScreen(new Point(0.0d, 0.0d));
            sAdrs = m_libPost.SearchPostAddress(nPost, this, pt.X, pt.Y);
            txtSeikyuAdrs1.Text = sAdrs; 
        }

    }
}
