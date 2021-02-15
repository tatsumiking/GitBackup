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
    /// SeikyusakiSearchWin.xaml の相互作用ロジック
    /// </summary>
    public partial class SeikyusakiSearchWin : Window
    {
        private MainWindow m_wndMain;
        private HanbaiWin m_wndHanbai;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibOdbc m_libOdbc;
        private DataTable m_dt;
        private int m_nSelectIdx;

        public SeikyusakiSearchWin()
        {
            InitializeComponent();
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wndMain = wnd;
            m_wndHanbai = null;
            m_sExecPath = wnd.m_sExecPath;
            m_libCmn = wnd.m_libCmn;
            m_libOdbc = wnd.m_libOdbc;
        }
        public void SetHanbaiWindow(HanbaiWin wnd)
        {
            m_wndMain = null;
            m_wndHanbai = wnd;
            m_sExecPath = wnd.m_sExecPath;
            m_libCmn = wnd.m_libCmn;
            m_libOdbc = wnd.m_libOdbc;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            List<string> listLine;

            m_nSelectIdx = -1;
            initDGSeikyusaki();
            listLine = m_libOdbc.getSeikyusakiRegOderList("");
            setSeikyusakiList(listLine);
            //initCmbSearchKind();
        }
        private void initDGSeikyusaki()
        {
            m_libCmn.CreateAddTextColHidden(dgSeikyusaki, "col_no", "No", 0);
            m_libCmn.CreateAddTextColHidden(dgSeikyusaki, "col_id", "id", 0);
            m_libCmn.CreateAddTextColHidden(dgSeikyusaki, "col_regdate", "更新日時", 0);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_name", "請求先名", 150);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_busyo", "部署名", 80);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_skeisyou", "請求敬称", 40);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_akeisyou", "宛名敬称", 40);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_postno", "郵便番号", 80);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_adrs1", "住所１", 200);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_adrs2", "住所２", 200);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_tel", "電話番号", 100);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_fax", "FAX番号", 100);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_payname", "振込名", 100);
            m_libCmn.CreateAddTextCol(dgSeikyusaki, "col_bikou", "備考", 50);
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
                if (9 <= ary.Length)
                {
                    dataRow = setCsvElement(no, ary);
                    m_dt.Rows.Add(dataRow);
                    no++;
                }
            }
            dgSeikyusaki.DataContext = m_dt;
        }
        private DataRow setCsvElement(int no, string[] ary)
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
        private void dgSeikyusaki_CurrentCellChanged(object sender, EventArgs e)
        {
            string sHead;
            int nIdx;

            if (dgSeikyusaki.CurrentItem == null)
            {
                return;
            }
            if (dgSeikyusaki.CurrentColumn == null)
            {
                return;
            }
            sHead = dgSeikyusaki.CurrentColumn.Header.ToString();
            nIdx = dgSeikyusaki.Items.IndexOf(dgSeikyusaki.CurrentItem);
            dgSeikyusaki.CommitEdit();
            m_nSelectIdx = nIdx;

        }
        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            selectedSeikyusaki();
        }
        private void dgSeikyusaki_PreviewMouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            selectedSeikyusaki();
        }
        private void selectedSeikyusaki()
        {
            DataRow dataRow;
            string sData;
            int nId;

            if (m_nSelectIdx == -1)
            {
                MessageBox.Show("請求先が選択されていません。", "確認", MessageBoxButton.OK);
                return;
            }
            if (m_dt.Rows.Count <= m_nSelectIdx)
            {
                MessageBox.Show("この行は削除できません。", "確認", MessageBoxButton.OK);
                return;
            }
            // 名前単価単位税区分
            dataRow = m_dt.Rows[m_nSelectIdx];
            sData = dataRow["col_name"].ToString();
            sData = sData + "," + dataRow["col_busyo"].ToString();
            sData = sData + "," + dataRow["col_skeisyou"].ToString();
            sData = sData + "," + dataRow["col_akeisyou"].ToString();
            sData = sData + "," + dataRow["col_postno"].ToString();
            sData = sData + "," + dataRow["col_adrs1"].ToString();
            sData = sData + "," + dataRow["col_adrs2"].ToString();
            sData = sData + "," + dataRow["col_tel"].ToString();
            sData = sData + "," + dataRow["col_fax"].ToString();
            sData = sData + "," + dataRow["col_payname"].ToString();
            sData = sData + "," + dataRow["col_bikou"].ToString();
            nId = m_libCmn.StrToInt(dataRow["col_id"].ToString());
            m_libOdbc.updateSeikyusakiRegdate(nId);
            if (m_wndMain != null)
            {
                m_wndMain.setSeikyusaki(nId, sData);
            }
            if (m_wndHanbai != null)
            {
                m_wndHanbai.setSeikyusaki(nId, sData);
            }
            this.Close();
        }
        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
