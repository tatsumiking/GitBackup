using System;
using System.Collections.Generic;
using System.Data;
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
    /// SelectRecWin.xaml の相互作用ロジック
    /// </summary>
    public partial class SelectRecWin : Window
    {
        public SougiSeikyuWin m_wnd;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibCanvas m_libCanvas;
        public LibOdbc m_libOdbc;
        public int m_nSelectTable;
        private DataTable m_dt;
        private int m_nSelectIdx;
        private List<string> m_listRecLine;

        public SelectRecWin()
        {
            InitializeComponent();
        }
        public void SetSougiSeikyuWin(SougiSeikyuWin wnd)
        {
            m_wnd = wnd;
            m_sExecPath = m_wnd.m_sExecPath;
            m_libCmn = m_wnd.m_libCmn;
            m_libCanvas = m_wnd.m_libCanvas;
            m_libOdbc = m_wnd.m_libOdbc;
        }
        public void SetSelectTable(int nSelectTable)
        {
            m_nSelectTable = nSelectTable;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            m_nSelectIdx = -1;
            if (m_nSelectTable == 1)
            {
                initSeikyusakiDGTable();
                m_listRecLine = m_libOdbc.getSeikyusakiRegOderList("");
                setSeikyusakiList(m_listRecLine);
            }
            else if (m_nSelectTable == 2)
            {
                initSasidasininDGTable();
                m_listRecLine = m_libOdbc.getSasidasininList("");
                setSasidasininList(m_listRecLine);
            }
            else if (m_nSelectTable == 3)
            {
                initFurikomisakiDGTable();
                m_listRecLine = m_libOdbc.getFurikomisakiList("");
                setFurikomisakiList(m_listRecLine);
            }
        }
        private void initSeikyusakiDGTable()
        {
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 0);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_id", "id", 0);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_regdate", "更新日時", 0);
            m_libCmn.CreateAddTextCol(dgTable, "col_name", "請求先名", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_busyo", "部署名", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_skeisyou", "請求敬称", 40);
            m_libCmn.CreateAddTextCol(dgTable, "col_akeisyou", "宛名敬称", 40);
            m_libCmn.CreateAddTextCol(dgTable, "col_postno", "郵便番号", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_adrs1", "住所１", 200);
            m_libCmn.CreateAddTextCol(dgTable, "col_adrs2", "住所２", 200);
            m_libCmn.CreateAddTextCol(dgTable, "col_tel", "電話番号", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_fax", "FAX番号", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_payname", "振込名", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 50);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_regdate", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_name", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_busyo", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_skeisyou", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_akeisyou", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_postno", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_adrs1", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_adrs2", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tel", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_fax", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_payname", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));
        }
        private void setSeikyusakiList(List<string> listLine)
        {
            int max, idx, no;
            string[] ary;
            DataRow dataRow;

            m_dt.Rows.Clear();
            max = listLine.Count;
            no = 1;
            for (idx = 0; idx < max; idx++)
            {
                ary = listLine[idx].Split(',');
                if (13 <= ary.Length)
                {
                    dataRow = setSeikyusakiCsvElement(no, ary);
                    m_dt.Rows.Add(dataRow);
                    no++;
                }
            }
            dgTable.DataContext = m_dt;
        }
        private DataRow setSeikyusakiCsvElement(int no, string[] ary)
        {
            DataRow dataRow;

            dataRow = m_dt.NewRow();
            dataRow["col_no"] = no;
            dataRow["col_id"] = ary[0];
            dataRow["col_regdate"] = ary[1];
            dataRow["col_name"] = ary[2];
            dataRow["col_busyo"] = ary[3];
            dataRow["col_skeisyou"] = ary[4];
            dataRow["col_akeisyou"] = ary[5];
            dataRow["col_postno"] = ary[6];
            dataRow["col_adrs1"] = ary[7];
            dataRow["col_adrs2"] = ary[8];
            dataRow["col_tel"] = ary[9];
            dataRow["col_fax"] = ary[10];
            dataRow["col_payname"] = ary[11];
            dataRow["col_bikou"] = ary[12];
            return (dataRow);
        }
        private void initSasidasininDGTable()
        {
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 0);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_id", "id", 0);
            m_libCmn.CreateAddTextCol(dgTable, "col_name", "差出人名", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_sisetu", "施設名", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_postno", "郵便番号", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_adrs1", "住所１", 200);
            m_libCmn.CreateAddTextCol(dgTable, "col_adrs2", "住所２", 200);
            m_libCmn.CreateAddTextCol(dgTable, "col_tel", "電話番号", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_fax", "FAX番号", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 50);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_name", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_sisetu", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_postno", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_adrs1", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_adrs2", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tel", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_fax", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));
        }
        private void setSasidasininList(List<string> listLine)
        {
            int max, idx, no;
            string[] ary;
            DataRow dataRow;

            m_dt.Rows.Clear();
            max = listLine.Count;
            no = 1;
            for (idx = 0; idx < max; idx++)
            {
                ary = listLine[idx].Split(',');
                if (9 <= ary.Length)
                {
                    dataRow = setSasidasininCsvElement(no, ary);
                    m_dt.Rows.Add(dataRow);
                    no++;
                }
            }
            dgTable.DataContext = m_dt;
        }
        private DataRow setSasidasininCsvElement(int no, string[] ary)
        {
            DataRow dataRow;

            dataRow = m_dt.NewRow();
            dataRow["col_no"] = no;
            dataRow["col_id"] = ary[0];
            dataRow["col_name"] = ary[1];
            dataRow["col_sisetu"] = ary[2];
            dataRow["col_postno"] = ary[3];
            dataRow["col_adrs1"] = ary[4];
            dataRow["col_adrs2"] = ary[5];
            dataRow["col_tel"] = ary[6];
            dataRow["col_fax"] = ary[7];
            dataRow["col_bikou"] = ary[8];
            return (dataRow);
        }
        private void initFurikomisakiDGTable()
        {
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 0);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_id", "id", 0);
            m_libCmn.CreateAddTextCol(dgTable, "col_name", "銀行名", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_siten", "支店名", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_kind", "預金種別", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_bankno", "預金番号", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_kana", "登録名", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_hojyo", "漢字名", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 50);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_name", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_siten", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_kind", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bankno", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_kana", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_hojyo", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));
        }
        private void setFurikomisakiList(List<string> listLine)
        {
            int max, idx, no;
            string[] ary;
            DataRow dataRow;

            m_dt.Rows.Clear();
            max = listLine.Count;
            no = 1;
            for (idx = 0; idx < max; idx++)
            {
                ary = listLine[idx].Split(',');
                if (8 <= ary.Length)
                {
                    dataRow = setFurikomisakiCsvElement(no, ary);
                    m_dt.Rows.Add(dataRow);
                    no++;
                }
            }
            dgTable.DataContext = m_dt;
        }
        private DataRow setFurikomisakiCsvElement(int no, string[] ary)
        {
            DataRow dataRow;

            dataRow = m_dt.NewRow();
            dataRow["col_no"] = no;
            dataRow["col_id"] = ary[0];
            dataRow["col_name"] = ary[1];
            dataRow["col_siten"] = ary[2];
            dataRow["col_kind"] = ary[3];
            dataRow["col_bankno"] = ary[4];
            dataRow["col_kana"] = ary[5];
            dataRow["col_hojyo"] = ary[6];
            dataRow["col_bikou"] = ary[7];
            return (dataRow);
        }

        private void dgTable_CurrentCellChanged(object sender, EventArgs e)
        {
            string sHead;
            int nIdx;

            if (dgTable.CurrentItem == null)
            {
                return;
            }
            if (dgTable.CurrentColumn == null)
            {
                return;
            }
            sHead = dgTable.CurrentColumn.Header.ToString();
            nIdx = dgTable.Items.IndexOf(dgTable.CurrentItem);
            dgTable.CommitEdit();
            m_nSelectIdx = nIdx;
        }

        private void dgTable_PreviewMouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            setSelectRec();
            this.Close();
        }
        private void btnSearch_Click(object sender, RoutedEventArgs e)
        {

        }
        private void btnSelect_Click(object sender, RoutedEventArgs e)
        {
            setSelectRec();
            this.Close();
        }
        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void setSelectRec()
        {
            if (m_nSelectIdx == -1)
            {
                return;
            }
            if (m_nSelectTable == 1)
            {
                m_wnd.setSeikyusaki(m_listRecLine[m_nSelectIdx]);
            }
            else if (m_nSelectTable == 2)
            {
                m_wnd.setSasidasinin(m_listRecLine[m_nSelectIdx]);
            }
            else if (m_nSelectTable == 3)
            {
                m_wnd.setFurikomisaki(m_listRecLine[m_nSelectIdx]);
            }
        }
    }
}
