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
    /// SyouhinSearchWin.xaml の相互作用ロジック
    /// </summary>
    public partial class SyouhinSearchWin : Window
    {
        private MainWindow m_wndMain;
        private HanbaiWin m_wndHanbai;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibOdbc m_libOdbc;
        private DataTable m_dt;
        private int m_nSelectIdx;

        public SyouhinSearchWin()
        {
            InitializeComponent();
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wndMain = wnd;
            m_wndHanbai = null;
            m_sExecPath = m_wndMain.m_sExecPath;
            m_libCmn = m_wndMain.m_libCmn;
            m_libOdbc = m_wndMain.m_libOdbc;
        }
        public void SetHanbaiWindow(HanbaiWin wnd)
        {
            m_wndMain = null;
            m_wndHanbai = wnd;
            m_sExecPath = m_wndHanbai.m_sExecPath;
            m_libCmn = m_wndHanbai.m_libCmn;
            m_libOdbc = m_wndHanbai.m_libOdbc;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            List<string> listLine;

            m_nSelectIdx = -1;
            initDGSyouhin();
            listLine = m_libOdbc.getSyouhinList("");
            setSyouhinList(listLine);
            initCmbSearchKind();
        }
        private void initDGSyouhin()
        {
            m_libCmn.CreateAddTextColHidden(dgSyouhin, "col_no", "No", 30);
            m_libCmn.CreateAddTextColHidden(dgSyouhin, "col_id", "id", 30);
            m_libCmn.CreateAddTextCol(dgSyouhin, "col_name", "商品名", 200);
            m_libCmn.CreateAddPriceCol(dgSyouhin, "col_tanka", "単価", 60);
            m_libCmn.CreateAddTextCol(dgSyouhin, "col_tani", "単位", 40);
            m_libCmn.CreateAddTextCol(dgSyouhin, "col_kubun", "分類", 50);
            m_libCmn.CreateAddTextCol(dgSyouhin, "col_tax", "税区分", 50);
            m_libCmn.CreateAddRightTextCol(dgSyouhin, "col_sort", "並順", 40);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_name", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tanka", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_tani", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_kubun", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tax", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_sort", typeof(int)));

        }
        private void setSyouhinList(List<string> listLine)
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
            dgSyouhin.DataContext = m_dt;
        }
        private void initCmbSearchKind()
        {
            string sFileName;
            string[] sLine;
            int max, idx;

            sFileName = m_sExecPath + "\\kden\\shyouhinkind.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            cmbSearchKind.Items.Add("全て");
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    cmbSearchKind.Items.Add(sLine[idx]);
                }
            }
        }
        private DataRow setCsvElement(int no, string[] ary)
        {
            DataRow dataRow;

            dataRow = m_dt.NewRow();
            dataRow["col_no"] = no;
            dataRow["col_id"] = ary[0];
            dataRow["col_name"] = ary[2];
            dataRow["col_tanka"] = ary[3];
            dataRow["col_tani"] = ary[4];
            dataRow["col_kubun"] = ary[5];
            dataRow["col_tax"] = ary[6];
            dataRow["col_sort"] = ary[7];
            return (dataRow);
        }
        private void dgSyouhin_CurrentCellChanged(object sender, EventArgs e)
        {
            string sHead;
            int nIdx;

            if (dgSyouhin.CurrentItem == null)
            {
                return;
            }
            if (dgSyouhin.CurrentColumn == null)
            {
                return;
            }
            sHead = dgSyouhin.CurrentColumn.Header.ToString();
            nIdx = dgSyouhin.Items.IndexOf(dgSyouhin.CurrentItem);
            dgSyouhin.CommitEdit();
            m_nSelectIdx = nIdx;

        }
        private void btnSearch_Click(object sender, RoutedEventArgs e)
        {
            string sConditions;
            int idx;
            string str;
            List<string> listLine;

            sConditions = "";
            if (txtSearchName.Text != "")
            {
                sConditions = "name LIKE '%" + txtSearchName.Text+"%'";
            }
            idx = cmbSearchKind.SelectedIndex;
            if (idx != -1 && idx != 0)
            {

                str = cmbSearchKind.Items[idx].ToString();
                if (sConditions != "")
                {
                    sConditions = sConditions + " AND ";
                }
                sConditions = sConditions + "bunrui LIKE '" + str + "'";
            }
            listLine = m_libOdbc.getSyouhinList(sConditions);
            setSyouhinList(listLine);

        }
        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            selectedSyouhin();
        }
        private void dgSyouhin_PreviewMouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            selectedSyouhin();
        }
        private void selectedSyouhin()
        {
            DataRow dataRow;
            string sData;


            if (m_nSelectIdx == -1)
            {
                MessageBox.Show("商品が選択されていません。", "確認", MessageBoxButton.OK);
                return;
            }
            if (m_dt.Rows.Count <= m_nSelectIdx)
            {
                MessageBox.Show("この行は削除できません。", "確認", MessageBoxButton.OK);
                return;
            }
            // 名前単価単位税区分
            dataRow = m_dt.Rows[m_nSelectIdx];
            sData = dataRow[2].ToString() + "," + dataRow[3].ToString();
            sData = sData + "," + dataRow[4].ToString() + "," + dataRow[6].ToString();
            if (m_wndMain != null)
            {
                m_wndMain.settxtSetSyouhin(sData);
            }
            if (m_wndHanbai != null)
            {
                m_wndHanbai.settxtSetSyouhin(sData);
            }
            this.Close();
        }
        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
