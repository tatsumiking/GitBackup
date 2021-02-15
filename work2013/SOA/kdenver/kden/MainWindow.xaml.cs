using System;
using System.Collections.Generic;
using System.Data;
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
using System.Printing;
using System.IO;
using System.Data.Odbc;

namespace kden
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        private string m_sMsg;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibCanvas m_libCanvas;
        public LibOdbc m_libOdbc;
        public LibPost m_libPost;
        public double m_dTaxPar;
        private Boolean m_bModify;
        private DataTable m_dt;
        private int m_nHanbaiSlctIdx;
        private string m_sColumnHead;
        private Boolean m_bInEdit;
        private List<string> m_lstSeikyuKubun;
        private List<string> m_lstSouke;
        private List<string> m_lstKokubetu;
        private int m_nSelectWinFlag; // 1:葬家
        private string m_sSyouhinElement;
        private string m_sSeikyuusakiElement;
        private string m_sCondition;
        private string m_sOder;

        public MainWindow()
        {
            InitializeComponent();
            m_dTaxPar = 8.0;
            m_sExecPath = InitExePath();
            m_sCondition = "";
            m_sOder = "";
            m_libCmn = new LibCommon();
            m_libCanvas = new LibCanvas();
            m_libOdbc = new LibOdbc();
            m_libOdbc.setLibCommonClass(m_libCmn);
            m_libOdbc.setExecPath(m_sExecPath);
            m_libPost = new LibPost();
            m_libPost.InitPostList(m_sExecPath+"\\kden");
            App.m_sExecPath = m_sExecPath;
            m_lstSouke = new List<string>();
            m_lstKokubetu = new List<string>();

            //initKdenDB();

        }
        private void initKdenDB()
        {
            string sFileName;
            string[] sLine;

            // データベース初期化
            m_libOdbc.createUseTable();
            sFileName = m_sExecPath + "\\kden\\syouhin.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initSyouhinRecords(sLine);
            /*
            sFileName = m_sExecPath + "\\kden\\seikyusaki.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initSeikyusakiRecords(sLine);
            sFileName = m_sExecPath + "\\kden\\sasidasinin.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initSasidasininRecords(sLine);
            sFileName = m_sExecPath + "\\kden\\furikomisaki.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initFurikomisakiRecords(sLine);
            */
            sFileName = m_sExecPath + "\\kden\\seikyukubun.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initSkkubunRecords(sLine);

            //m_libOdbc.testDataCreate();
            
        }
        private void deleteInstrCsvFile(string sFileName)
        {
           m_libCmn.DeleteFile(sFileName);
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.WindowState = WindowState.Maximized;
            SetModify(false);
            m_nHanbaiSlctIdx = -1;
            m_bInEdit = false;
            InitSearchElement();
            InitDGHanbai();
            loadCsvOut();
            chkSetSeikyuKubun.IsChecked = false;
            searchUpdateDataTable();
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (m_bModify == true)
            {
                updateHanbaiData();
                SetModify(false);
            }
            // e.Cancel = true;
            return;
        }
        private void SetModify(Boolean flag)
        {
            m_bModify = flag;
        }
        private string InitExePath()
        {
            string path;

            Assembly myAssembly = Assembly.GetEntryAssembly();
            path = myAssembly.Location;
            return (path.Substring(0, path.LastIndexOf("\\")));
        }
        public void InitSearchElement()
        {
            InitSougiElement();
            InitSeikyuKubunElement();
        }
        public void InitSougiElement()
        {
            List<string> listSougi;
            int max, idx;
            String[] sAry;
            string sSetStr;

            cmbSearchSouke.Items.Clear();
            listSougi = m_libOdbc.getSougiList("");
            cmbSearchSouke.Items.Add("全て");
            max = listSougi.Count;
            for (idx = 0; idx < max; idx++)
            {
                sAry = listSougi[idx].Split(',');
                sSetStr = sAry[3] + "," + sAry[10] + "," + sAry[4] + "," + sAry[7] + "," + sAry[0];
                m_lstSouke.Add(sSetStr);
                setKokubetuList(sAry[7]);
            }
            max = m_lstSouke.Count;
            for (idx = 0; idx < max; idx++)
            {
                cmbSearchSouke.Items.Add(m_lstSouke[idx]);
            }
            cmbSearchSouke.SelectedIndex = 0;
        }
        private void setKokubetuList(string sKokubetu)
        {
            int max, idx;

            max = m_lstKokubetu.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (m_lstKokubetu[idx] == sKokubetu)
                {
                    return;
                }
            }
            m_lstKokubetu.Add(sKokubetu);
        }
        private void InitSeikyuKubunElement()
        {
            int max, idx;
            String[] sAry;

            cmbSearchSeikyuKubun.Items.Clear();
            m_lstSeikyuKubun = m_libOdbc.getSkkubunList("");
            cmbSearchSeikyuKubun.Items.Add("全て");
            max = m_lstSeikyuKubun.Count;
            for (idx = 0; idx < max; idx++)
            {
                sAry = m_lstSeikyuKubun[idx].Split(',');
                if (4 <= sAry.Length)
                {
                    cmbSearchSeikyuKubun.Items.Add(sAry[1]);
                    cmbSetSeikyuKubun.Items.Add(sAry[1]);
                }
            }
            cmbSearchSeikyuKubun.SelectedIndex = 0;
            cmbSetSeikyuKubun.SelectedIndex = 0;
        }
        private void InitDGHanbai()
        {
            m_nHanbaiSlctIdx = -1;
            m_libCmn.CreateAddTextCol(dgHanbai, "col_no", "No", 30);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_change","変更", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_id", "id", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_regdate", "更新日", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_orderdate", "受注日", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_seikyuid", "請求書ID", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_sougiid", "葬儀ID", 0);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_souke", "葬家", 90);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_kokubetu", "告別式日", 90);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_name", "商品名", 200);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_tanka", "単価", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_tani", "単位", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_kosu", "個数", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_taxkind", "税区分", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_tax", "消費税", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_price", "金額", 0);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_nafuda1", "名札１", 100);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_nafuda2", "名札２", 100);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_nafuda3", "名札３", 100);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_seikyu", "請求先名", 200);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_busho", "請求先部署", 100);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_skskeisyou", "請求書敬称", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_skakeisyou", "宛名書敬称", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_seikyupost", "請求郵便番号", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_seikyuadrs1", "請求住所１", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_seikyuadrs2", "請求住所２", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_seikyutel", "請求電話", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_seikyufax", "請求FAX", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_seikyupay", "請求振込名", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_seikyukubun", "請求区分", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_receivedate", "入金日", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_receivekind", "入金種別", 0);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_vendername", "仕入先", 0);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_nafuda4", "名札４", 100);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_nafuda5", "名札５", 100);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_nafuda6", "名札６", 100);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_nafuda7", "名札７", 100);
            m_libCmn.CreateAddTextCol(dgHanbai, "col_nafuda8", "名札８", 100);
            m_libCmn.CreateAddTextColHidden(dgHanbai, "col_bikou", "備考", 0);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_change", typeof(int)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_regdate", typeof(string)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_orderdate", typeof(string)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_seikyuid", typeof(int)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_sougiid", typeof(int)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_souke", typeof(string)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_kokubetu", typeof(string)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_name", typeof(string)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_tanka", typeof(int)));         // 文字列
            m_dt.Columns.Add(new DataColumn("col_tani", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_kosu", typeof(int)));           // 文字列
            m_dt.Columns.Add(new DataColumn("col_taxkind", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_tax", typeof(int)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_price", typeof(int)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_nafuda1", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_nafuda2", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_nafuda3", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_seikyu", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_busho", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_skskeisyou", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_skakeisyou", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_seikyupost", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_seikyuadrs1", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_seikyuadrs2", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_seikyutel", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_seikyufax", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_seikyupay", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_seikyukubun", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_receivedate", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_receivekind", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_vendername", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_nafuda4", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_nafuda5", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_nafuda6", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_nafuda7", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_nafuda8", typeof(string)));          // 文字列
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));          // 文字列
            dgHanbai.DataContext = m_dt;
            // dgHanbai.CanUserAddRows = false;　// 最終空白行削除
            // dgHanbai.CommitEdit();
            // dgHanbai.CancelEdit();

        }
        private void loadCsvOut()
        {
            string sCrtymdhm, sCrtymd;
            string sFileName;
            string[] sLine;
            int max, idx;
            string[] sAry;
            string sData;
            int no;
            DataRow dataRow;

            DateTime dt = DateTime.Now;
            sCrtymdhm = dt.ToString("yyyy/MM/dd HH:mm");
            sCrtymd = dt.ToString("yyyy/MM/dd");

            sFileName = m_sExecPath + "\\instr\\instr.csv";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            no = m_dt.Rows.Count+1;
            max = sLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                sAry = sLine[idx].Split(',');
                if (10 <= sAry.Length)
                {
                    sData = "0," + sCrtymdhm + "," + sCrtymd + ",0,0," + sAry[1] + "," + sCrtymd + ",,0,," + sAry[2] + ",,0,0";
                    sData = sData + ",,,,,,,,,,,,,,," + sAry[3] + "," + sAry[4] + "," + sAry[5];
                    sData = sData + "," + sAry[6] + "," + sAry[7] + "," + sAry[8] + "," + sAry[9] + "," + sAry[10]+",,";
                    dataRow = m_dt.NewRow();
                    setCsvDataRow(dataRow, no, 1, sData);
                    m_dt.Rows.Add(dataRow);
                    no++;
                }
            }
            updateHanbaiData();
            deleteInstrCsvFile(sFileName);
        }
        private void setCsvDataRow(DataRow dataRow, int no, int flag, string sLine)
        {
            string[] aryCsv;
            aryCsv = sLine.Split(',');
            dataRow["col_no"] = no;
            dataRow["col_change"] = flag;
            dataRow["col_id"] = m_libCmn.StrToInt(aryCsv[0]);
            dataRow["col_regdate"] = aryCsv[1];
            dataRow["col_orderdate"] = aryCsv[2];
            dataRow["col_seikyuid"] = m_libCmn.StrToInt(aryCsv[3]);
            dataRow["col_sougiid"] = m_libCmn.StrToInt(aryCsv[4]);
            dataRow["col_souke"] = aryCsv[5];
            dataRow["col_kokubetu"] = aryCsv[6];
            dataRow["col_name"] = aryCsv[7];
            dataRow["col_tanka"] = m_libCmn.StrToInt(aryCsv[8]);
            dataRow["col_tani"] = aryCsv[9];
            dataRow["col_kosu"] = m_libCmn.StrToInt(aryCsv[10]);
            dataRow["col_taxkind"] = aryCsv[11];
            dataRow["col_tax"] = m_libCmn.StrToInt(aryCsv[12]);
            dataRow["col_price"] = m_libCmn.StrToInt(aryCsv[13]);
            dataRow["col_seikyu"] = aryCsv[14];
            dataRow["col_busho"] = aryCsv[15];
            dataRow["col_skskeisyou"] = aryCsv[16];
            dataRow["col_skakeisyou"] = aryCsv[17];
            dataRow["col_seikyupost"] = aryCsv[18];
            dataRow["col_seikyuadrs1"] = aryCsv[19];
            dataRow["col_seikyuadrs2"] = aryCsv[20];
            dataRow["col_seikyutel"] = aryCsv[21];
            dataRow["col_seikyufax"] = aryCsv[22];
            dataRow["col_seikyupay"] = aryCsv[23];
            dataRow["col_seikyukubun"] = aryCsv[24];
            dataRow["col_receivedate"] = aryCsv[25];
            dataRow["col_receivekind"] = aryCsv[26];
            dataRow["col_vendername"] = aryCsv[27];
            dataRow["col_nafuda1"] = aryCsv[28];
            dataRow["col_nafuda2"] = aryCsv[29];
            dataRow["col_nafuda3"] = aryCsv[30];
            dataRow["col_nafuda4"] = aryCsv[31];
            dataRow["col_nafuda5"] = aryCsv[32];
            dataRow["col_nafuda6"] = aryCsv[33];
            dataRow["col_nafuda7"] = aryCsv[34];
            dataRow["col_nafuda8"] = aryCsv[35];
            dataRow["col_bikou"] = aryCsv[36];
        }
        private void btnSearchKokubetu_Click(object sender, RoutedEventArgs e)
        {
            m_nSelectWinFlag = 0;
            showSelectWin(m_lstKokubetu, btnSearchKokubetu);
        }
        private void btnSelectDate_Click(object sender, RoutedEventArgs e)
        {
            CalWin calWin;

            calWin = new CalWin();
            calWin.SetMainWindow(this);
            calWin.m_sCrtDate = txtSearchKokubetu.Text;
            Point pt = txtSearchKokubetu.PointToScreen(new Point(0.0d, 0.0d));
            calWin.Left = pt.X;
            calWin.Top = pt.Y;
            calWin.Owner = this;
            //calWin.Topmost = true;
            //calWin.ShowDialog();
            calWin.ShowDialog();
        }
        private void btnSearch_Click(object sender, RoutedEventArgs e)
        {
            searchUpdateDataTable();
        }
        private void searchUpdateDataTable()
        {
            int idx;
            string sSougi;
            string[] sAry;
            string sKubun;
            int nSid, nEid;

            m_sCondition = "";
            if (txtSearchSeikyuSaki.Text != "")
            {
                m_sCondition = "(seikyusaki + seikyubusyo Like '%'+'";
                m_sCondition = m_sCondition + txtSearchSeikyuSaki.Text + "'+'%')";
            }
            if (txtSearchNafuda.Text != "")
            {
                if (m_sCondition != "")
                {
                    m_sCondition = m_sCondition + " AND ";
                }
                m_sCondition = m_sCondition + "(nafuda1+nafuda2+nafuda3+nafuda1+nafuda4";
                m_sCondition = m_sCondition + "+nafuda5+nafuda6+nafuda7+nafuda8 Like '%'+'";
                m_sCondition = m_sCondition + txtSearchNafuda.Text + "'+'%')";
            }
            if (cmbSearchSouke.SelectedIndex != -1 && cmbSearchSouke.SelectedIndex != 0)
            {

                if (m_sCondition != "")
                {
                    m_sCondition = m_sCondition + " AND ";
                }
                idx = cmbSearchSouke.SelectedIndex;
                sSougi = cmbSearchSouke.Items[idx].ToString();
                sAry = sSougi.Split(',');
                m_sCondition = m_sCondition + "(souke Like '%'+'" + sAry[0] + "'+'%')";
                m_sCondition = m_sCondition + " AND (kokubetu Like '%'+'" + sAry[3] + "'+'%')";
            }
            if (txtSearchKokubetu.Text != "")
            {
                if (m_sCondition != "")
                {
                    m_sCondition = m_sCondition + " AND ";
                }
                m_sCondition = m_sCondition + "(kokubetu Like '%'+'";
                m_sCondition = m_sCondition + txtSearchKokubetu.Text + "'+'%')";
            }
            if(cmbSearchSeikyuKubun.SelectedIndex != -1 && cmbSearchSeikyuKubun.SelectedIndex != 0)
            {
                if (m_sCondition != "")
                {
                    m_sCondition = m_sCondition + " AND ";
                }
                idx = cmbSearchSeikyuKubun.SelectedIndex;
                sKubun = cmbSearchSeikyuKubun.Items[idx].ToString();
                m_sCondition = m_sCondition + "(seikyukubun Like '%'+'" + sKubun + "'+'%')";
            }
            if (txtSearchSJyutyuNo.Text != "" || txtSearchEJyutyuNo.Text != "")
            {
                nSid = m_libCmn.StrToInt(txtSearchSJyutyuNo.Text);
                nEid = m_libCmn.StrToInt(txtSearchEJyutyuNo.Text);
                if (m_sCondition != "")
                {
                    m_sCondition = m_sCondition + " AND ";
                }
                if (txtSearchSJyutyuNo.Text != "" && txtSearchEJyutyuNo.Text != "")
                {
                    m_sCondition = m_sCondition + "( " + nSid + " <= id AND id <= " + nEid + " )";
                }
                else if (txtSearchSJyutyuNo.Text != "")
                {
                    m_sCondition = m_sCondition + "( " + nSid + " <= id )";
                }
                else if (txtSearchEJyutyuNo.Text != "")
                {
                    m_sCondition = m_sCondition + "( id <= " + nEid + " )";
                }
            }
            //string sOder = "regdate";
            m_sOder = "regdate DESC";

            updateDataTable(m_sCondition, m_sOder);
        }
        public void updateDataGrid()
        {
            int max, idx;
            DataRow dataRow;
            string sId;

            if (m_sCondition == "")
            {
                m_sOder = "regdate DESC";
                max = m_dt.Rows.Count;
                m_sCondition = "";
                for (idx = 0; idx < max; idx++)
                {
                    dataRow = m_dt.Rows[idx];
                    sId = dataRow["col_id"].ToString();
                    if (idx == 0)
                    {
                        m_sCondition = m_sCondition + "( id = " + sId + ")";
                    }
                    else
                    {
                        m_sCondition = m_sCondition + "OR ( id = " + sId + ")";
                    }
                }
            }
            updateDataTable(m_sCondition, m_sOder);
        }
        private void updateDataTable(string sCondition, string sOder)
        {
            List<string> list;
            int max, idx, no;
            DataRow dataRow;

            if (sOder == "")
            {
                list = m_libOdbc.getHanbaiList(sCondition);
            }
            else
            {
                list = m_libOdbc.getOderHanbaiList(sCondition, sOder);
            }
            max = list.Count;
            no = 1;
            m_dt.Rows.Clear();
            for (idx = 0; idx < max; idx++)
            {
                dataRow = m_dt.NewRow();
                setCsvDataRow(dataRow, no, 0, list[idx]);
                m_dt.Rows.Add(dataRow);
                no++;
            }
            dgHanbai.DataContext = m_dt;
        }
        private void btnSearchCls_Click(object sender, RoutedEventArgs e)
        {
            txtSearchSeikyuSaki.Text = "";
            txtSearchNafuda.Text = "";
            cmbSearchSouke.SelectedIndex = -1;
            txtSearchKokubetu.Text = "";
            cmbSearchSeikyuKubun.SelectedIndex = -1;
            txtSearchSJyutyuNo.Text = "";
            txtSearchEJyutyuNo.Text = "";
        }
        private void btnSetSouke_Click(object sender, RoutedEventArgs e)
        {
            showSougiSelectWin();
        }
        private void btnSetSyouhin_Click(object sender, RoutedEventArgs e)
        {
            showSyouhinSearchWin();
        }
        private void btnSetToday_Click(object sender, RoutedEventArgs e)
        {
            DateTime dt = DateTime.Now;
            txtSetKokubetu.Text = dt.ToString("yyyy/MM/dd");
            chkSetKokubetu.IsChecked = true;
        }

        private void btnSetTomorrow_Click(object sender, RoutedEventArgs e)
        {
            DateTime dt = DateTime.Now;
            dt = dt.AddDays(1);
            txtSetKokubetu.Text = dt.ToString("yyyy/MM/dd");
            chkSetKokubetu.IsChecked = true;
        }

        private void btnSetAfterTomorrow_Click(object sender, RoutedEventArgs e)
        {
            DateTime dt = DateTime.Now;
            dt = dt.AddDays(2);
            txtSetKokubetu.Text = dt.ToString("yyyy/MM/dd");
            chkSetKokubetu.IsChecked = true;
        }
        private void btnSetElement_Click(object sender, RoutedEventArgs e)
        {
            int max, idx, no, lineidx;
            DataRowView dataRowView;
            DataRow dataRow;
            string[] ary;
            int nSelectidx;
            string sStr;
            int nTanka, nTax, nCount, nPrice;

            SetModify(true);
            max = dgHanbai.SelectedItems.Count;
            for (idx = 0; idx < max; idx++)
            {
                dataRowView = (DataRowView)dgHanbai.SelectedItems[idx];
                no = (int)dataRowView.Row[0];
                lineidx = no - 1;
                dataRow = m_dt.Rows[lineidx];
                dataRow["col_change"] = 1;
                if (chkSetSouke.IsChecked == true)
                {
                    dataRow["col_souke"] = txtSetSouke.Text;
                }
                if (chkSetKokubetu.IsChecked == true)
                {
                    dataRow["col_kokubetu"] = txtSetKokubetu.Text;
                }
                if (chkSetSyouhin.IsChecked == true)
                {
                    ary = m_sSyouhinElement.Split(',');
                    nTanka = m_libCmn.StrToInt(ary[1]);
                    dataRow["col_name"] = ary[0];
                    dataRow["col_tanka"] = nTanka;
                    dataRow["col_tani"] = ary[2];
                    nCount = m_libCmn.StrToInt(dataRow["col_kosu"].ToString());
                    // dataRow["col_kosu"] = m_libCmn.StrToInt(ary[3]);
                    dataRow["col_taxkind"] = ary[3];
                    sStr = m_libCmn.CalcTaxPrice(ary[3], nTanka, nCount, m_dTaxPar);
                    ary = sStr.Split(',');
                    dataRow["col_tax"] = ary[0];
                    dataRow["col_price"] = ary[1];
                }
                if (chkSetSeikyuSaki.IsChecked == true)
                {
                    ary = m_sSeikyuusakiElement.Split(',');
                    dataRow["col_seikyu"] = ary[0];
                    dataRow["col_busho"] = ary[1];
                    dataRow["col_skskeisyou"] = ary[2];
                    dataRow["col_skakeisyou"] = ary[3];
                    dataRow["col_seikyupost"] = ary[4];
                    dataRow["col_seikyuadrs1"] = ary[5];
                    dataRow["col_seikyuadrs2"] = ary[6];
                    dataRow["col_seikyutel"] = ary[7];
                    dataRow["col_seikyufax"] = ary[8];
                    dataRow["col_seikyupay"] = ary[9];
                }
                if (chkSetSeikyuKubun.IsChecked == true)
                {
                    nSelectidx = cmbSetSeikyuKubun.SelectedIndex;
                    ary = m_lstSeikyuKubun[nSelectidx].Split(',');
                    dataRow["col_seikyukubun"] = ary[1];
                }
            }
            dgHanbai.DataContext = m_dt;
            updateHanbaiData();
            SetModify(false);
        }
        public void setSeikyusaki(int nId, string sData)
        {
            string[] ary;

            m_sSeikyuusakiElement = sData;
            ary = sData.Split(',');
            chkSetSeikyuSaki.IsChecked = true;
            txtSetSeikyuSaki.Text = ary[0];
            txtSetBusyo.Text = ary[1];
        }
        private void btnSetCls_Click(object sender, RoutedEventArgs e)
        {
            chkSetSouke.IsChecked = false;
            txtSetSouke.Text = "";
            chkSetSyouhin.IsChecked = false;
            txtSetSyouhin.Text = "";
            chkSetKokubetu.IsChecked = false;
            txtSetKokubetu.Text = "";
            chkSetSeikyuSaki.IsChecked = false;
            txtSetSeikyuSaki.Text = "";
            txtSetBusyo.Text = "";
            chkSetSeikyuKubun.IsChecked = false;
            cmbSetSeikyuKubun.SelectedIndex = -1;
        }
        private void btnSetSeikyuSaki_Click(object sender, RoutedEventArgs e)
        {
            showSeikyuSakiSelectWin();
        }
        private void btnAllSelect_Click(object sender, RoutedEventArgs e)
        {
            dgHanbai.SelectAll();
        }
        private void btnAllUnSelect_Click(object sender, RoutedEventArgs e)
        {
            dgHanbai.UpdateLayout();
            dgHanbai.Focus();
        }
        private void btnMeisaiWin_Click(object sender, RoutedEventArgs e)
        {
            int max, idx;
            DataRowView dataRowView;
            string sId, sNo;
            int nLine;
            string sRecs;
            SougiWin sougiWin;
            string sData, sDatas;
            string[] ary;
            string sSouke, sKokubetu;

            max = dgHanbai.SelectedItems.Count;
            if (max == 0)
            {
                MessageBox.Show("明細書印刷対象の行が選択されていません。", "確認", MessageBoxButton.OK);
                return;
            }
            dataRowView = (DataRowView)dgHanbai.SelectedItems[0];
            sId = dataRowView.Row[2].ToString();
            sNo = dataRowView.Row[0].ToString();
            nLine = m_libCmn.StrToInt(sNo) - 1;
            sData = getCsvElement(nLine);
            ary = sData.Split(',');
            sSouke = ary[5];
            sKokubetu = ary[6];

            sRecs = sId;
            sDatas = sData;
            for (idx = 1; idx < max; idx++)
            {
                dataRowView = (DataRowView)dgHanbai.SelectedItems[idx];
                sId = dataRowView.Row[2].ToString();
                sNo = dataRowView.Row[0].ToString();
                nLine = m_libCmn.StrToInt(sNo) - 1;
                sData = getCsvElement(nLine);
                ary = sData.Split(',');
                if (sSouke != ary[5] || sKokubetu != ary[6])
                {
                    MessageBox.Show("複数の葬儀が選択されています。", "確認", MessageBoxButton.OK);
                    return;
                }
                sRecs = sRecs + "," + sId;
                sDatas = sDatas + "\n" + sData;
            }

            sougiWin = new SougiWin();
            sougiWin.SetMainWindow(this);
            sougiWin.SetRecsCsv(sRecs);
            sougiWin.SetRecsElement(sDatas);
            Point pt = this.PointToScreen(new Point(0.0d, 0.0d));
            sougiWin.Left = pt.X;
            sougiWin.Top = pt.Y - 24;
            sougiWin.Owner = this;
            sougiWin.ShowDialog();

            updateDataGrid();
        }

        private void btnSeikyuWin_Click(object sender, RoutedEventArgs e)
        {
            int max, idx;
            DataRowView dataRowView;
            string sId, sNo;
            int nLine;
            string[] ary;
            string seikyu, busyo;
            string sRecs;
            SougiSeikyuWin sougiSeikyuWin;
            string sData, sDatas;

            sRecs = "";
            sDatas = "";
            max = dgHanbai.SelectedItems.Count;
            if (max == 0)
            {
                MessageBox.Show("請求書印刷対象の行が選択されていません。", "確認", MessageBoxButton.OK);
                return;
            }
            dataRowView = (DataRowView)dgHanbai.SelectedItems[0];
            sId = dataRowView.Row[2].ToString();
            sNo = dataRowView.Row[0].ToString();
            nLine = m_libCmn.StrToInt(sNo) - 1;
            sData = getCsvElement(nLine);
            ary = sData.Split(',');
            seikyu = ary[14];
            busyo = ary[15];
            if (ary[14] == "" && ary[15] == "")
            {
                MessageBox.Show("請求先が設定されていません。", "確認", MessageBoxButton.OK);
                return;
            }
            for (idx = 0; idx < max; idx++)
            {
                dataRowView = (DataRowView)dgHanbai.SelectedItems[idx];
                sId = dataRowView.Row[2].ToString();
                sNo = dataRowView.Row[0].ToString();
                nLine = m_libCmn.StrToInt(sNo) - 1;
                sData = getCsvElement(nLine);
                ary = sData.Split(',');
                if (ary[14] == "" && ary[15] == "")
                {
                    MessageBox.Show("請求先が設定されていません。", "確認", MessageBoxButton.OK);
                    return;
                }
                if (seikyu == ary[14] && busyo == ary[15])
                {
                    if (idx != 0)
                    {
                        sRecs = sRecs + ",";
                        sDatas = sDatas + "\n";
                    }
                    sRecs = sRecs + sId;
                    sDatas = sDatas + sData;
                }
                else
                {
                    MessageBox.Show("異なる請求先が設定されています。", "確認", MessageBoxButton.OK);
                    return;
                }
            }
            sougiSeikyuWin = new SougiSeikyuWin();
            sougiSeikyuWin.SetMainWindow(this);
            sougiSeikyuWin.SetRecsCsv(sRecs);
            sougiSeikyuWin.SetRecsElement(sDatas);
            Point pt = this.PointToScreen(new Point(0.0d, 0.0d));
            sougiSeikyuWin.Left = pt.X;
            sougiSeikyuWin.Top = pt.Y - 24;
            sougiSeikyuWin.Owner = this;
            sougiSeikyuWin.ShowDialog();

            updateDataGrid();
        }
        private void btnEditElement_Click(object sender, RoutedEventArgs e)
        {
            showHanbaiWin();
        }
        private void dgHanbai_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            showHanbaiWin();
        }
        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
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
        private void updateHanbaiData()
        {
            int max, idx;
            DataRow dataRow;
            int id, change;
            string sData;


            DateTime dt = DateTime.Now;
            max = m_dt.Rows.Count;
            for (idx = 0; idx < max; idx++)
            {
                dataRow = m_dt.Rows[idx];
                id = m_libCmn.StrToInt(dataRow["col_id"].ToString());
                change = m_libCmn.StrToInt(dataRow["col_change"].ToString());
                dataRow["col_change"] = 0; // 
                if (change == 1)
                {
                    dataRow["col_regdate"] = dt.ToString("yyyy/MM/dd hh:mm");
                    sData = getDataRowString(dataRow);
                    if (id == 0)
                    {
                        id = m_libOdbc.insertHanbai(sData);
                        dataRow["col_id"] = id;
                    }
                    else
                    {
                        m_libOdbc.updateHanbai(sData);
                    }
                }
            }
            dgHanbai.DataContext = m_dt;
        }
        private string getCsvElement(int nLine)
        {
            string sData;

            DataRow dataRow;
            dataRow = m_dt.Rows[nLine];
            sData = getDataRowString(dataRow);
            return (sData); 

        }
        private string getDataRowString(DataRow dataRow)
        {
            string sData;

            sData = dataRow["col_id"].ToString();
            sData = sData + "," + dataRow["col_regdate"].ToString();
            sData = sData + "," + dataRow["col_orderdate"].ToString();
            sData = sData + "," + dataRow["col_seikyuid"].ToString();
            sData = sData + "," + dataRow["col_sougiid"].ToString();
            sData = sData + "," + dataRow["col_souke"].ToString();
            sData = sData + "," + dataRow["col_kokubetu"].ToString();
            sData = sData + "," + dataRow["col_name"].ToString();
            sData = sData + "," + dataRow["col_tanka"].ToString();
            sData = sData + "," + dataRow["col_tani"].ToString();
            sData = sData + "," + dataRow["col_kosu"].ToString();
            sData = sData + "," + dataRow["col_taxkind"].ToString();
            sData = sData + "," + dataRow["col_tax"].ToString();
            sData = sData + "," + dataRow["col_price"].ToString();
            sData = sData + "," + dataRow["col_seikyu"].ToString();
            sData = sData + "," + dataRow["col_busho"].ToString();
            sData = sData + "," + dataRow["col_skskeisyou"].ToString();
            sData = sData + "," + dataRow["col_skakeisyou"].ToString();
            sData = sData + "," + dataRow["col_seikyupost"].ToString();
            sData = sData + "," + dataRow["col_seikyuadrs1"].ToString();
            sData = sData + "," + dataRow["col_seikyuadrs2"].ToString();
            sData = sData + "," + dataRow["col_seikyutel"].ToString();
            sData = sData + "," + dataRow["col_seikyufax"].ToString();
            sData = sData + "," + dataRow["col_seikyupay"].ToString();
            sData = sData + "," + dataRow["col_seikyukubun"].ToString();
            sData = sData + "," + dataRow["col_receivedate"].ToString();
            sData = sData + "," + dataRow["col_receivekind"].ToString();
            sData = sData + "," + dataRow["col_vendername"].ToString();
            sData = sData + "," + dataRow["col_nafuda1"].ToString();
            sData = sData + "," + dataRow["col_nafuda2"].ToString();
            sData = sData + "," + dataRow["col_nafuda3"].ToString();
            sData = sData + "," + dataRow["col_nafuda4"].ToString();
            sData = sData + "," + dataRow["col_nafuda5"].ToString();
            sData = sData + "," + dataRow["col_nafuda6"].ToString();
            sData = sData + "," + dataRow["col_nafuda7"].ToString();
            sData = sData + "," + dataRow["col_nafuda8"].ToString();
            sData = sData + "," + dataRow["col_bikou"].ToString();
            return (sData);
        }
        public void SetSlctDate(string sDate)
        {
            txtSearchKokubetu.Text = sDate;
        }
        public void settxtSetSyouhin(string sData)
        {
            string[] ary;
            m_sSyouhinElement = sData;
            ary = sData.Split(',');
            chkSetSyouhin.IsChecked = true;
            txtSetSyouhin.Text = ary[0];
        }
        public void setTextSelectWin(string sData)
        {
            string[] ary;
            if (m_nSelectWinFlag == 0)
            {
                txtSearchKokubetu.Text = sData;
            }
            else if (m_nSelectWinFlag == 1)
            {
                ary = sData.Split(',');
                chkSetSouke.IsChecked = true;
                txtSetSouke.Text = ary[0];
                chkSetKokubetu.IsChecked = true;
                txtSetKokubetu.Text = ary[3];
            }
        }
        private void cmbSetSeikyuKubun_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            chkSetSeikyuKubun.IsChecked = true;
        }
        private void dgHanbai_CurrentCellChanged(object sender, EventArgs e)
        {
            string sHead;
            int nIdx;

            if (dgHanbai.CurrentItem == null)
            {
                return;
            }
            if (dgHanbai.CurrentColumn == null)
            {
                return;
            }
            sHead = dgHanbai.CurrentColumn.Header.ToString();
            nIdx = dgHanbai.Items.IndexOf(dgHanbai.CurrentItem);
            if (m_bInEdit == true)
            {
                dgHanbai.CommitEdit();
                dgHanbai.CancelEdit();
            }
            m_sColumnHead = sHead;
            m_nHanbaiSlctIdx = nIdx;
            m_sMsg = "販売選択行CurrentCellChanged(" + m_nHanbaiSlctIdx + ")(" + m_sColumnHead + ")";
            System.Diagnostics.Trace.WriteLine(m_sMsg);
        }
        private void DoDispatch()
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
        private void dgHanbai_BeginningEdit(object sender, DataGridBeginningEditEventArgs e)
        {
            m_bInEdit = true;
        }

        private void dgHanbai_CellEditEnding(object sender, DataGridCellEditEndingEventArgs e)
        {
            m_sMsg = "編集終了CellEditEnding";
            System.Diagnostics.Trace.WriteLine(m_sMsg);
            SetModify(true);

            m_bInEdit = false;
        }
        private void showSyouhinSearchWin()
        {
            SyouhinSearchWin syouhinSearchWin;

            syouhinSearchWin = new SyouhinSearchWin();
            syouhinSearchWin.SetMainWindow(this);
            Point pt = btnSetSyouhin.PointToScreen(new Point(0.0d, 0.0d));
            syouhinSearchWin.Left = pt.X;
            syouhinSearchWin.Top = pt.Y;
            syouhinSearchWin.Owner = this;
            syouhinSearchWin.ShowDialog();
        }
        private void showSougiSelectWin()
        {
            m_nSelectWinFlag = 1;
            showSelectWin(m_lstSouke, btnSetSouke);
        }
        private void showSeikyuSakiSelectWin()
        {
            SeikyusakiSearchWin seikyusakiSearchWin;

            seikyusakiSearchWin = new SeikyusakiSearchWin();
            seikyusakiSearchWin.SetMainWindow(this);
            Point pt = btnSetSeikyuSaki.PointToScreen(new Point(0.0d, 0.0d));
            seikyusakiSearchWin.Left = pt.X;
            seikyusakiSearchWin.Top = pt.Y;
            seikyusakiSearchWin.Owner = this;
            seikyusakiSearchWin.ShowDialog();
        }
        private void showSelectWin(List<string> listSelect, Control control)
        {
            SelectWin selectWin;

            selectWin = new SelectWin();
            selectWin.SetMainWindow(this);
            selectWin.SetSelectList(listSelect);
            Point pt = control.PointToScreen(new Point(0.0d, 0.0d));
            selectWin.Left = pt.X;
            selectWin.Top = pt.Y;
            selectWin.Owner = this;
            selectWin.ShowDialog();
        }
        private void showHanbaiWin()
        {
            int max, idx;
            DataRow dataRow;
            HanbaiWin hanbaiWin;
            string sRecs;
            int nId;

            if (m_nHanbaiSlctIdx == -1)
            {
                MessageBox.Show("データが選択されていません。", "確認", MessageBoxButton.OK);
                return;
            }
            max = m_dt.Rows.Count;
            sRecs = "";
            for (idx = 0; idx < max; idx++)
            {
                if (idx != 0)
                {
                    sRecs = sRecs + ",";
                }
                dataRow = m_dt.Rows[idx];
                sRecs = sRecs + dataRow["col_id"].ToString();
            }
            dataRow = m_dt.Rows[m_nHanbaiSlctIdx];
            nId = m_libCmn.StrToInt(dataRow["col_id"].ToString());
            hanbaiWin = new HanbaiWin();
            hanbaiWin.SetMainWindow(this);
            hanbaiWin.SetRecsCsv(sRecs);
            hanbaiWin.SetSelectCrtId(nId);
            Point pt = this.PointToScreen(new Point(0.0d, 0.0d));
            hanbaiWin.Left = pt.X;
            hanbaiWin.Top = pt.Y;
            hanbaiWin.Owner = this;
            hanbaiWin.ShowDialog();
        }
        private void btnMasterWin_Click(object sender, RoutedEventArgs e)
        {
            MasterWin masterWin;

            masterWin = new MasterWin();
            masterWin.SetMainWindow(this);
            Point pt = this.PointToScreen(new Point(0.0d, 0.0d));
            masterWin.Left = pt.X;
            masterWin.Top = pt.Y;
            masterWin.Owner = this;
            masterWin.ShowDialog();
        }
    }
}
