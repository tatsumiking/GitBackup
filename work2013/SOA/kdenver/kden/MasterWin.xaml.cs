using Microsoft.Win32;
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
    /// MasterWin.xaml の相互作用ロジック
    /// </summary>
    public partial class MasterWin : Window
    {
        private MainWindow m_wnd;
        private string m_sMsg;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibCanvas m_libCanvas;
        public LibOdbc m_libOdbc;
        private Boolean m_bModify;
        private DataTable m_dt;
        private Boolean m_bInEdit;
        private string m_sBaindString;
        private int m_nSlctIdx;
        private int m_nTableKind; // 0:non 1:Syouhin
        private TextBox m_txtboxDate;
        private Brush m_brushDefButton;
        private string m_sCsvFolder;
        private string m_sCsvFileName;

        public MasterWin()
        {
            InitializeComponent();
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wnd = wnd;
            m_sExecPath = m_wnd.m_sExecPath;
            m_libCmn = m_wnd.m_libCmn;
            m_libCanvas = m_wnd.m_libCanvas;
            m_libOdbc = m_wnd.m_libOdbc;
            m_nTableKind = 0;
            m_sCsvFolder = "";
            m_sCsvFileName = "";
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            
            m_brushDefButton = btnSyouhin.Background;
            m_bModify = false;
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            checkModifySave();
        }
        private Boolean checkModifySave()
        {
            if (m_bModify == false)
            {
                return (true);
            }
            if (MessageBox.Show("テーブルの内容が変更されています、保存しますか。", "更新確認", MessageBoxButton.YesNo,
                     MessageBoxImage.Information) == MessageBoxResult.Yes)
            {
                updateCrtTable();
            }
            return (true);
        }
        private void btnSyouhin_Click(object sender, RoutedEventArgs e)
        {
            if (checkModifySave() == false)
            {
                return;
            }
            m_bModify = false;
            m_nTableKind = 1;
            setButtonColor();
            dgTable.Columns.Clear();
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 30);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_chk", "check", 30);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_id", "id", 50);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_regdate", "更新日時", 120);
            m_libCmn.CreateAddTextCol(dgTable, "col_name", "商品名", 240);
            m_libCmn.CreateAddPriceCol(dgTable, "col_tanka", "単価", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_tani", "単位", 40);
            m_libCmn.CreateAddTextCol(dgTable, "col_bunrui", "分類", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_taxkind", "税区分", 50);
            m_libCmn.CreateAddRightTextCol(dgTable, "col_sortno", "並順", 40);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 200);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_chk", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_regdate", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_name", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tanka", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_tani", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bunrui", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_taxkind", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_sortno", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));

            txtSearchStartDate.Text = "";
            txtSearchEndDate.Text = "";
            setCrtRecordTables("");
        }
        private void btnSeikyusaki_Click(object sender, RoutedEventArgs e)
        {
            if (checkModifySave() == false)
            {
                return;
            }
            m_bModify = false;
            m_nTableKind = 2;
            setButtonColor();
            dgTable.Columns.Clear();
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 30);
            m_libCmn.CreateAddTextCol(dgTable, "col_chk", "check", 30);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_id", "id", 50);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_regdate", "更新日時", 120);
            m_libCmn.CreateAddTextCol(dgTable, "col_name", "請求先名", 250);
            m_libCmn.CreateAddTextCol(dgTable, "col_busyo", "請求先部署", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_skeisyou", "請求書敬称", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_akeisyou", "宛名敬称", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_postno", "郵便番号", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_adrs1", "住所１", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_adrs2", "住所２", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_tel", "電話", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_fax", "FAX", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_payname", "振込名", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 200);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_chk", typeof(int)));
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

            txtSearchStartDate.Text = "";
            txtSearchEndDate.Text = "";
            setCrtRecordTables("");
        }
        private void btnSeikyukubun_Click(object sender, RoutedEventArgs e)
        {
            if (checkModifySave() == false)
            {
                return;
            }
            m_bModify = false;
            m_nTableKind = 3;
            setButtonColor();
            dgTable.Columns.Clear();
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 30);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_chk", "check", 30);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_id", "id", 50);
            m_libCmn.CreateAddTextCol(dgTable, "col_name", "区分名", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_mark", "マーク", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 200);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_chk", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_name", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_mark", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));

            txtSearchStartDate.Text = "";
            txtSearchEndDate.Text = "";
            setCrtRecordTables("");
        }

        private void btnSasidasinin_Click(object sender, RoutedEventArgs e)
        {
            if (checkModifySave() == false)
            {
                return;
            }
            m_bModify = false;
            m_nTableKind = 4;
            setButtonColor();
            dgTable.Columns.Clear();
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 30);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_chk", "check", 30);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_id", "id", 50);
            m_libCmn.CreateAddTextCol(dgTable, "col_name", "会社名", 250);
            m_libCmn.CreateAddTextCol(dgTable, "col_sisetu", "施設名", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_postno", "郵便番号", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_adrs1", "住所１", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_adrs2", "住所２", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_tel", "電話", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_fax", "FAX", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 200);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_chk", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_name", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_sisetu", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_postno", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_adrs1", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_adrs2", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tel", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_fax", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));

            txtSearchStartDate.Text = "";
            txtSearchEndDate.Text = "";
            setCrtRecordTables("");
        }

        private void btnFurikomisaki_Click(object sender, RoutedEventArgs e)
        {
            if (checkModifySave() == false)
            {
                return;
            }
            m_bModify = false;
            m_nTableKind = 5;
            setButtonColor();
            dgTable.Columns.Clear();
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 30);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_chk", "check", 30);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_id", "id", 50);
            m_libCmn.CreateAddTextCol(dgTable, "col_name", "銀行名", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_siten", "支店名", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_kind", "預金種別", 60);
            m_libCmn.CreateAddTextCol(dgTable, "col_bankno", "口座番号", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_kana", "カナ名義", 200);
            m_libCmn.CreateAddTextCol(dgTable, "col_hojyo", "漢字名義", 200);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 200);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_chk", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_name", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_siten", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_kind", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bankno", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_kana", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_hojyo", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));

            txtSearchStartDate.Text = "";
            txtSearchEndDate.Text = "";
            setCrtRecordTables("");
        }
        private void btnSougi_Click(object sender, RoutedEventArgs e)
        {
            if (checkModifySave() == false)
            {
                return;
            }
            m_bModify = false;
            m_nTableKind = 11;
            setButtonColor();
            dgTable.Columns.Clear();
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 30);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_chk", "check", 30);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_id", "id", 50);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_regdate", "更新日時", 120);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyuno", "識別番号", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_souke", "葬家", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_tuya_date", "通夜日", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_tuya_stime", "通夜開始", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_tuya_etime", "通夜終了", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_kokubetu_date", "告別式日", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_kokubetu_stime", "告別式開始", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_kokubetu_etime", "告別式終了", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_sekoubasyo", "施行場所", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_tantou", "担当者", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 200);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_chk", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_regdate", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyuno", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_souke", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tuya_date", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tuya_stime", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tuya_etime", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_kokubetu_date", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_kokubetu_stime", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_kokubetu_etime", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_sekoubasyo", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tantou", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));

            txtSearchStartDate.Text = "";
            txtSearchEndDate.Text = "";
            setCrtRecordTables("");
        }
        private void btnSeikyu_Click(object sender, RoutedEventArgs e)
        {
            if (checkModifySave() == false)
            {
                return;
            }
            m_bModify = false;
            m_nTableKind = 12;
            setButtonColor();
            dgTable.Columns.Clear();
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 30);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_chk", "check", 30);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_id", "id", 50);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_regdate", "更新日時", 120);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyuno", "識別番号", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyusakiid", "請求先ID", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_sasidasininid", "差出人ID", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_furikomisakiid", "振込先ID", 100);
            m_libCmn.CreateAddPriceCol(dgTable, "col_nebiki", "値引額", 200);
            m_libCmn.CreateAddPriceCol(dgTable, "col_tax", "税金", 200);
            m_libCmn.CreateAddPriceCol(dgTable, "col_sumprice", "請求合計", 200);
            m_libCmn.CreateAddTextCol(dgTable, "col_nouhin", "請求書発効日", 200);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyu", "納品書発効日", 200);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 200);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_chk", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_regdate", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyuno", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyusakiid", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_sasidasininid", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_furikomisakiid", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_nebiki", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_tax", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_sumprice", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_nouhin", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyu", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));

            txtSearchStartDate.Text = "";
            txtSearchEndDate.Text = "";
            setCrtRecordTables("");
        }
        private void btnHanbai_Click(object sender, RoutedEventArgs e)
        {
            if (checkModifySave() == false)
            {
                return;
            }
            m_bModify = false;
            m_nTableKind = 13;
            setButtonColor();
            dgTable.Columns.Clear();
            m_libCmn.CreateAddTextColHidden(dgTable, "col_no", "No", 30);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_chk", "check", 30);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_id", "id", 50);
            m_libCmn.CreateAddTextColNonEdit(dgTable, "col_regdate", "更新日時", 120);
            m_libCmn.CreateAddTextCol(dgTable, "col_orderdate", "受注日", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyuid", "請求ID", 50);
            m_libCmn.CreateAddTextCol(dgTable, "col_sougiid", "葬儀ID", 50);
            m_libCmn.CreateAddTextCol(dgTable, "col_souke", "葬家名", 250);
            m_libCmn.CreateAddTextCol(dgTable, "col_kokubetu", "告別式", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_name", "商品名", 250);
            m_libCmn.CreateAddPriceCol(dgTable, "col_tanka", "単価", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_tani", "単位", 40);
            m_libCmn.CreateAddRightTextCol(dgTable, "col_kosu", "個数", 40);
            m_libCmn.CreateAddTextCol(dgTable, "col_taxkind", "税区分", 60);
            m_libCmn.CreateAddPriceCol(dgTable, "col_tax", "消費税", 100);
            m_libCmn.CreateAddPriceCol(dgTable, "col_price", "総額", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyusaki", "請求先名", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyubusyo", "請求部署", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_skskeisyou", "請求書敬称", 40);
            m_libCmn.CreateAddTextCol(dgTable, "col_skakeisyou", "宛名書敬称", 40);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyupost", "請求先郵便番号", 80);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyuadrs1", "請求先住所１", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyuadrs2", "請求先住所２", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyutel", "請求先電話", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyufax", "請求先FAX", 100);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyupay", "振込名", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_seikyukubun", "請求区分", 150);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_receivedate", "入金日", 100);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_receivekind", "入金種別", 60);
            m_libCmn.CreateAddTextColHidden(dgTable, "col_vendername", "仕入先", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_nafuda1", "名札１", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_nafuda2", "名札２", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_nafuda3", "名札３", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_nafuda4", "名札４", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_nafuda5", "名札５", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_nafuda6", "名札６", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_nafuda7", "名札７", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_nafuda8", "名札８", 150);
            m_libCmn.CreateAddTextCol(dgTable, "col_bikou", "備考", 200);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_chk", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_id", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_regdate", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_orderdate", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyuid", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_sougiid", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_souke", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_kokubetu", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_name", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tanka", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_tani", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_kosu", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_taxkind", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_tax", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_price", typeof(int)));
            m_dt.Columns.Add(new DataColumn("col_seikyusaki", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyubusyo", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_skskeisyou", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_skakeisyou", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyupost", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyuadrs1", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyuadrs2", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyutel", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyufax", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyupay", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_seikyukubun", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_receivedate", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_receivekind", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_vendername", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_nafuda1", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_nafuda2", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_nafuda3", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_nafuda4", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_nafuda5", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_nafuda6", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_nafuda7", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_nafuda8", typeof(string)));
            m_dt.Columns.Add(new DataColumn("col_bikou", typeof(string)));

            txtSearchStartDate.Text = "";
            txtSearchEndDate.Text = "";
            setCrtRecordTables("");
        }
        private void btnCsvLoad_Click(object sender, RoutedEventArgs e)
        {
            string[] sAryLine;
            int max, idx, no;
            string[] ary;
            DataRow dataRow;

            if (m_nTableKind == 0)
            {
                return;
            }
            if (m_sCsvFolder == "")
            {
                m_sCsvFolder = m_sExecPath + "\\kden";
            }
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = m_sCsvFolder;
            ofd.Filter = "CSVファイル(*.csv)|*.csv||";
            ofd.Title = "CSVファイルを選択してください";
            var result = ofd.ShowDialog();
            if (result == false)
            {
                return;
            }
            m_sCsvFolder = System.IO.Path.GetDirectoryName(ofd.FileName);
            m_sCsvFileName = System.IO.Path.GetFileName(ofd.FileName);
            sAryLine = m_libCmn.LoadFileLineSJIS(ofd.FileName);
            m_dt.Rows.Clear();
            no = 1;
            max = sAryLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                ary = sAryLine[idx].Split(',');
                if (m_nTableKind == 1)
                {
                    if (9 <= ary.Length)
                    {
                        dataRow = m_dt.NewRow();
                        dataRow["col_no"] = no;
                        dataRow["col_chk"] = 0;
                        setCsvToDataRow(sAryLine[idx], dataRow);
                        m_dt.Rows.Add(dataRow);
                        no++;
                    }
                }
                else if (m_nTableKind == 2)
                {
                    if (13 <= ary.Length)
                    {
                        dataRow = m_dt.NewRow();
                        dataRow["col_no"] = no;
                        dataRow["col_chk"] = 0;
                        setCsvToDataRow(sAryLine[idx], dataRow);
                        m_dt.Rows.Add(dataRow);
                        no++;
                    }
                }
                else if (m_nTableKind == 3)
                {
                    if (3 <= ary.Length)
                    {
                        dataRow = m_dt.NewRow();
                        dataRow["col_no"] = no;
                        dataRow["col_chk"] = 0;
                        setCsvToDataRow(sAryLine[idx], dataRow);
                        m_dt.Rows.Add(dataRow);
                        no++;
                    }
                }
                else if (m_nTableKind == 4)
                {
                    if (9 <= ary.Length)
                    {
                        dataRow = m_dt.NewRow();
                        dataRow["col_no"] = no;
                        dataRow["col_chk"] = 0;
                        setCsvToDataRow(sAryLine[idx], dataRow);
                        m_dt.Rows.Add(dataRow);
                        no++;
                    }
                }
                else if (m_nTableKind == 5)
                {
                    if (8 <= ary.Length)
                    {
                        dataRow = m_dt.NewRow();
                        dataRow["col_no"] = no;
                        dataRow["col_chk"] = 0;
                        setCsvToDataRow(sAryLine[idx], dataRow);
                        m_dt.Rows.Add(dataRow);
                        no++;
                    }
                }
                else if (m_nTableKind == 11)
                {
                    if (13 <= ary.Length)
                    {
                        dataRow = m_dt.NewRow();
                        dataRow["col_no"] = no;
                        dataRow["col_chk"] = 0;
                        setCsvToDataRow(sAryLine[idx], dataRow);
                        m_dt.Rows.Add(dataRow);
                        no++;
                    }
                }
                else if (m_nTableKind == 12)
                {
                    if (12 <= ary.Length)
                    {
                        dataRow = m_dt.NewRow();
                        dataRow["col_no"] = no;
                        dataRow["col_chk"] = 0;
                        setCsvToDataRow(sAryLine[idx], dataRow);
                        m_dt.Rows.Add(dataRow);
                        no++;
                    }
                }
                else if (m_nTableKind == 13)
                {
                    if (37 <= ary.Length)
                    {
                        dataRow = m_dt.NewRow();
                        dataRow["col_no"] = no;
                        dataRow["col_chk"] = 0;
                        setCsvToDataRow(sAryLine[idx], dataRow);
                        m_dt.Rows.Add(dataRow);
                        no++;
                    }
                }

            }
        }
        private void btnCsvSave_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            int max, idx;
            string sData;
            DataRow dataRow;

            if (m_nTableKind == 0)
            {
                return;
            }
            if (m_sCsvFolder == "")
            {
                m_sCsvFolder = m_sExecPath + "\\kden";
            }
            if (m_sCsvFileName == "")
            {
                if (m_nTableKind == 1)
                {
                    m_sCsvFileName = "Syouhin.csv";
                }
                else if (m_nTableKind == 2)
                {
                    m_sCsvFileName = "Seikyusaki.csv";
                }
                else if (m_nTableKind == 3)
                {
                    m_sCsvFileName = "Skkubun.csv";
                }
                else if (m_nTableKind == 4)
                {
                    m_sCsvFileName = "Sasidasinin.csv";
                }
                else if (m_nTableKind == 5)
                {
                    m_sCsvFileName = "Furikomisaki.csv";
                }
                else if (m_nTableKind == 11)
                {
                    m_sCsvFileName = "Sougi.csv";
                }
                else if (m_nTableKind == 12)
                {
                    m_sCsvFileName = "Seikyu.csv";
                }
                else if (m_nTableKind == 13)
                {
                    m_sCsvFileName = "Hanbai.csv";
                }
            }
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.InitialDirectory = m_sCsvFolder;
            sfd.FileName = m_sCsvFileName;
            sfd.Filter = "CSVファイル(*.csv)|*.csv||";
            sfd.Title = "CSVファイルに保存します";
            var result = sfd.ShowDialog();
            if (result != false) 
            {
                return;
            }
            sFileName = sfd.FileName;
            sData = "";
            max = m_dt.Rows.Count;
            for(idx = 0; idx < max; idx++){
                dataRow = m_dt.Rows[idx]; 
                sData = sData + getDataRowToCsv(dataRow) + "\n";
            }
            m_libCmn.SaveFileSJIS(sFileName, sData);
        }
        private void btnUpdate_Click(object sender, RoutedEventArgs e)
        {
            m_bModify = false;
            updateCrtTable();
            txtSearchStartDate.Text = "";
            txtSearchEndDate.Text = "";
            setCrtRecordTables("");
        }
        private void btnInitDB_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;

            // データベース初期化
            m_libOdbc.createUseTable();
            sFileName = m_sExecPath + "\\kden\\syouhin.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initSyouhinRecords(sLine);
            sFileName = m_sExecPath + "\\kden\\seikyukubun.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initSkkubunRecords(sLine);
        }
        private void btnTest_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;

            sFileName = m_sExecPath + "\\kden\\seikyusaki.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initSeikyusakiRecords(sLine);
            sFileName = m_sExecPath + "\\kden\\sasidasinin.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initSasidasininRecords(sLine);
            sFileName = m_sExecPath + "\\kden\\furikomisaki.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initFurikomisakiRecords(sLine);
            m_libOdbc.testDataCreate();
        }
        private void btnReturn_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void updateCrtTable()
        {
            string sCrtymdhm;
            int max, idx;
            DataRow dataRow;
            string sFilds;

            if (m_bInEdit == true)
            {
                dgTable.CommitEdit();
                dgTable.CancelEdit();
            }
            DateTime dt = DateTime.Now;
            sCrtymdhm = dt.ToString("yyyy/MM/dd HH:mm");
            max = m_dt.Rows.Count;
            for (idx = 0; idx < max; idx++)
            {
                dataRow = m_dt.Rows[idx];
                if(dataRow["col_no"].ToString() == ""){
                    if (m_nTableKind == 1 || m_nTableKind == 2
                     || m_nTableKind == 11 || m_nTableKind == 12 || m_nTableKind == 13)
                    {
                        dataRow["col_regdate"] = sCrtymdhm;
                    }
                    sFilds = getDataRowToCsv(dataRow);
                    inseartFilds(sFilds);
                }
                else if ((int)(dataRow["col_chk"]) == 1)
                {
                    if (m_nTableKind == 1 || m_nTableKind == 2
                     || m_nTableKind == 11 || m_nTableKind == 12 || m_nTableKind == 13)
                    {
                        dataRow["col_regdate"] = sCrtymdhm;
                    }
                    sFilds = getDataRowToCsv(dataRow);
                    updateFilds(sFilds);
                }
            }
        }
        private void setCrtRecordTables(string sCondision)
        {
            List<string> lstLine;
            int max, idx, no;
            string[] ary;
            DataRow dataRow;
            int nFildCount;

            nFildCount = 0;
            m_dt.Rows.Clear();
            lstLine = new List<string>();
            if (m_nTableKind == 1)
            {
                lstLine = m_libOdbc.getSyouhinList(sCondision);
                nFildCount = 9;
            }
            else if (m_nTableKind == 2)
            {
                lstLine = m_libOdbc.getSeikyusakiList(sCondision);
                nFildCount = 13;
            }
            else if (m_nTableKind == 3)
            {
                lstLine = m_libOdbc.getSkkubunList("");
                nFildCount = 3;
            }
            else if (m_nTableKind == 4)
            {
                lstLine = m_libOdbc.getSasidasininList("");
                nFildCount = 9;
            }
            else if (m_nTableKind == 5)
            {
                lstLine = m_libOdbc.getFurikomisakiList("");
                nFildCount = 8;
            }
            else if (m_nTableKind == 11)
            {
                lstLine = m_libOdbc.getSougiList(sCondision);
                nFildCount = 13;
            }
            else if (m_nTableKind == 12)
            {
                lstLine = m_libOdbc.getSeikyuList(sCondision);
                nFildCount = 12;
            }
            else if (m_nTableKind == 13)
            {
                lstLine = m_libOdbc.getHanbaiList(sCondision);
                nFildCount = 37;
            }
            max = lstLine.Count;
            no = 1;
            for (idx = 0; idx < max; idx++)
            {
                ary = lstLine[idx].Split(',');
                if (nFildCount <= ary.Length)
                {
                    dataRow = m_dt.NewRow();
                    dataRow["col_no"] = no;
                    dataRow["col_chk"] = 0;
                    setCsvToDataRow(lstLine[idx], dataRow);
                    m_dt.Rows.Add(dataRow);
                    no++;
                }
            }
            dgTable.DataContext = m_dt;
        }
        private void inseartFilds(string sFilds)
        {
            if (m_nTableKind == 1)
            {
                m_libOdbc.insertSyouhin(sFilds);
            }
            else if (m_nTableKind == 2)
            {
                m_libOdbc.insertSeikyusaki(sFilds);
            }
            else if (m_nTableKind == 3)
            {
                m_libOdbc.insertSkkubun(sFilds);
            }
            else if (m_nTableKind == 4)
            {
                m_libOdbc.insertSasidasinin(sFilds);
            }
            else if (m_nTableKind == 5)
            {
                m_libOdbc.insertFurikomisaki(sFilds);
            }
            else if (m_nTableKind == 11)
            {
                m_libOdbc.insertSougi(sFilds);
            }
            else if (m_nTableKind == 12)
            {
                m_libOdbc.insertSeikyu(sFilds);
            }
            else if (m_nTableKind == 13)
            {
                m_libOdbc.insertHanbai(sFilds);
            }
        }
        private void updateFilds(string sFilds)
        {
            if (m_nTableKind == 1)
            {
                m_libOdbc.updateSyouhin(sFilds);
            }
            else if (m_nTableKind == 2)
            {
                m_libOdbc.updateSeikyusaki(sFilds);
            }
            else if (m_nTableKind == 3)
            {
                m_libOdbc.updateSkkubun(sFilds);
            }
            else if (m_nTableKind == 4)
            {
                m_libOdbc.updateSasidasinin(sFilds);
            }
            else if (m_nTableKind == 5)
            {
                m_libOdbc.updateFurikomisaki(sFilds);
            }
            else if (m_nTableKind == 11)
            {
                m_libOdbc.updateSougi(sFilds);
            }
            else if (m_nTableKind == 12)
            {
                m_libOdbc.updateSeikyu(sFilds);
            }
            else if (m_nTableKind == 13)
            {
                m_libOdbc.updateHanbai(sFilds);
            }
        }
        private void setCsvToDataRow(string sCsv, DataRow dataRow)
        {
            string[] ary;

            ary = sCsv.Split(',');
            if (m_nTableKind == 1)
            {
                dataRow["col_id"] = ary[0];
                dataRow["col_regdate"] = ary[1];
                dataRow["col_name"] = ary[2];
                dataRow["col_tanka"] = ary[3];
                dataRow["col_tani"] = ary[4];
                dataRow["col_bunrui"] = ary[5];
                dataRow["col_taxkind"] = ary[6];
                dataRow["col_sortno"] = ary[7];
                dataRow["col_bikou"] = ary[8];
            }
            else if (m_nTableKind == 2)
            {
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
            }
            else if (m_nTableKind == 3)
            {
                dataRow["col_id"] = ary[0];
                dataRow["col_name"] = ary[1];
                dataRow["col_mark"] = ary[2];
                dataRow["col_bikou"] = ary[3];
            }
            else if (m_nTableKind == 4)
            {
                dataRow["col_id"] = ary[0];
                dataRow["col_name"] = ary[1];
                dataRow["col_sisetu"] = ary[2];
                dataRow["col_postno"] = ary[3];
                dataRow["col_adrs1"] = ary[4];
                dataRow["col_adrs2"] = ary[5];
                dataRow["col_tel"] = ary[6];
                dataRow["col_fax"] = ary[7];
                dataRow["col_bikou"] = ary[8];
            }
            else if (m_nTableKind == 5)
            {
                dataRow["col_id"] = ary[0];
                dataRow["col_name"] = ary[1];
                dataRow["col_siten"] = ary[2];
                dataRow["col_kind"] = ary[3];
                dataRow["col_bankno"] = ary[4];
                dataRow["col_kana"] = ary[5];
                dataRow["col_hojyo"] = ary[6];
                dataRow["col_bikou"] = ary[7];
            }
            else if (m_nTableKind == 11)
            {
                dataRow["col_id"] = ary[0];
                dataRow["col_regdate"] = ary[1];
                dataRow["col_seikyuno"] = ary[2];
                dataRow["col_souke"] = ary[3];
                dataRow["col_tuya_date"] = ary[4];
                dataRow["col_tuya_stime"] = ary[5];
                dataRow["col_tuya_etime"] = ary[6];
                dataRow["col_kokubetu_date"] = ary[7];
                dataRow["col_kokubetu_stime"] = ary[8];
                dataRow["col_kokubetu_etime"] = ary[9];
                dataRow["col_sekoubasyo"] = ary[10];
                dataRow["col_tantou"] = ary[11];
                dataRow["col_bikou"] = ary[12];
            }
            else if (m_nTableKind == 12)
            {
                dataRow["col_id"] = ary[0];
                dataRow["col_regdate"] = ary[1];
                dataRow["col_seikyuno"] = ary[2];
                dataRow["col_seikyusakiid"] = ary[3];
                dataRow["col_sasidasininid"] = ary[4];
                dataRow["col_furikomisakiid"] = ary[5];
                dataRow["col_nebiki"] = ary[6];
                dataRow["col_tax"] = ary[7];
                dataRow["col_sumprice"] = ary[8];
                dataRow["col_nouhin"] = ary[9];
                dataRow["col_seikyu"] = ary[10];
                dataRow["col_bikou"] = ary[11];
            }
            else if (m_nTableKind == 13)
            {
                dataRow["col_id"] = ary[0];
                dataRow["col_regdate"] = ary[1];
                dataRow["col_orderdate"] = ary[2];
                dataRow["col_seikyuid"] = ary[3];
                dataRow["col_sougiid"] = ary[4];
                dataRow["col_souke"] = ary[5];
                dataRow["col_kokubetu"] = ary[6];
                dataRow["col_name"] = ary[7];
                dataRow["col_tanka"] = ary[8];
                dataRow["col_tani"] = ary[9];
                dataRow["col_kosu"] = ary[10];
                dataRow["col_taxkind"] = ary[11];
                dataRow["col_tax"] = ary[12];
                dataRow["col_price"] = ary[13];
                dataRow["col_seikyusaki"] = ary[14];
                dataRow["col_seikyubusyo"] = ary[15];
                dataRow["col_skskeisyou"] = ary[16];
                dataRow["col_skakeisyou"] = ary[17];
                dataRow["col_seikyupost"] = ary[18];
                dataRow["col_seikyuadrs1"] = ary[19];
                dataRow["col_seikyuadrs2"] = ary[20];
                dataRow["col_seikyutel"] = ary[21];
                dataRow["col_seikyufax"] = ary[22];
                dataRow["col_seikyupay"] = ary[23];
                dataRow["col_seikyukubun"] = ary[24];
                dataRow["col_receivedate"] = ary[25];
                dataRow["col_receivekind"] = ary[26];
                dataRow["col_vendername"] = ary[27];
                dataRow["col_nafuda1"] = ary[28];
                dataRow["col_nafuda2"] = ary[29];
                dataRow["col_nafuda3"] = ary[30];
                dataRow["col_nafuda4"] = ary[31];
                dataRow["col_nafuda5"] = ary[32];
                dataRow["col_nafuda6"] = ary[33];
                dataRow["col_nafuda7"] = ary[34];
                dataRow["col_nafuda8"] = ary[35];
                dataRow["col_bikou"] = ary[36];
            }
        }
        private string getDataRowToCsv(DataRow dataRow)
        {
            string sCsv;

            sCsv = "";
            if (m_nTableKind == 1)
            {
                sCsv = dataRow["col_id"].ToString();
                sCsv = sCsv + "," + dataRow["col_regdate"].ToString();
                sCsv = sCsv + "," + dataRow["col_name"].ToString();
                sCsv = sCsv + "," + dataRow["col_tanka"].ToString();
                sCsv = sCsv + "," + dataRow["col_tani"].ToString();
                sCsv = sCsv + "," + dataRow["col_bunrui"].ToString();
                sCsv = sCsv + "," + dataRow["col_taxkind"].ToString();
                sCsv = sCsv + "," + dataRow["col_sortno"].ToString();
                sCsv = sCsv + "," + dataRow["col_bikou"].ToString();
            }
            else if (m_nTableKind == 2)
            {
                sCsv = dataRow["col_id"].ToString();
                sCsv = sCsv + "," + dataRow["col_regdate"].ToString();
                sCsv = sCsv + "," + dataRow["col_name"].ToString();
                sCsv = sCsv + "," + dataRow["col_busyo"].ToString();
                sCsv = sCsv + "," + dataRow["col_skeisyou"].ToString();
                sCsv = sCsv + "," + dataRow["col_akeisyou"].ToString();
                sCsv = sCsv + "," + dataRow["col_postno"].ToString();
                sCsv = sCsv + "," + dataRow["col_adrs1"].ToString();
                sCsv = sCsv + "," + dataRow["col_adrs2"].ToString();
                sCsv = sCsv + "," + dataRow["col_tel"].ToString();
                sCsv = sCsv + "," + dataRow["col_fax"].ToString();
                sCsv = sCsv + "," + dataRow["col_payname"].ToString();
                sCsv = sCsv + "," + dataRow["col_bikou"].ToString();
            }
            else if (m_nTableKind == 3)
            {
                sCsv = dataRow["col_id"].ToString();
                sCsv = sCsv + "," + dataRow["col_name"].ToString();
                sCsv = sCsv + "," + dataRow["col_mark"].ToString();
                sCsv = sCsv + "," + dataRow["col_bikou"].ToString();
            }
            else if (m_nTableKind == 4)
            {
                sCsv = dataRow["col_id"].ToString();
                sCsv = sCsv + "," + dataRow["col_name"].ToString();
                sCsv = sCsv + "," + dataRow["col_sisetu"].ToString();
                sCsv = sCsv + "," + dataRow["col_postno"].ToString();
                sCsv = sCsv + "," + dataRow["col_adrs1"].ToString();
                sCsv = sCsv + "," + dataRow["col_adrs2"].ToString();
                sCsv = sCsv + "," + dataRow["col_tel"].ToString();
                sCsv = sCsv + "," + dataRow["col_fax"].ToString();
                sCsv = sCsv + "," + dataRow["col_bikou"].ToString();
            }
            else if (m_nTableKind == 5)
            {
                sCsv = dataRow["col_id"].ToString();
                sCsv = sCsv + "," + dataRow["col_name"].ToString();
                sCsv = sCsv + "," + dataRow["col_siten"].ToString();
                sCsv = sCsv + "," + dataRow["col_kind"].ToString();
                sCsv = sCsv + "," + dataRow["col_bankno"].ToString();
                sCsv = sCsv + "," + dataRow["col_kana"].ToString();
                sCsv = sCsv + "," + dataRow["col_hojyo"].ToString();
                sCsv = sCsv + "," + dataRow["col_bikou"].ToString();
            }
            else if (m_nTableKind == 11)
            {
                sCsv = dataRow["col_id"].ToString();
                sCsv = sCsv + "," + dataRow["col_regdate"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyuno"].ToString();
                sCsv = sCsv + "," + dataRow["col_souke"].ToString();
                sCsv = sCsv + "," + dataRow["col_tuya_date"].ToString();
                sCsv = sCsv + "," + dataRow["col_tuya_stime"].ToString();
                sCsv = sCsv + "," + dataRow["col_tuya_etime"].ToString();
                sCsv = sCsv + "," + dataRow["col_kokubetu_date"].ToString();
                sCsv = sCsv + "," + dataRow["col_kokubetu_stime"].ToString();
                sCsv = sCsv + "," + dataRow["col_kokubetu_etime"].ToString();
                sCsv = sCsv + "," + dataRow["col_sekoubasyo"].ToString();
                sCsv = sCsv + "," + dataRow["col_tantou"].ToString();
                sCsv = sCsv + "," + dataRow["col_bikou"].ToString();
            }
            else if (m_nTableKind == 12)
            {
                sCsv = dataRow["col_id"].ToString();
                sCsv = sCsv + "," + dataRow["col_regdate"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyuno"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyusakiid"].ToString();
                sCsv = sCsv + "," + dataRow["col_sasidasininid"].ToString();
                sCsv = sCsv + "," + dataRow["col_furikomisakiid"].ToString();
                sCsv = sCsv + "," + dataRow["col_nebiki"].ToString();
                sCsv = sCsv + "," + dataRow["col_tax"].ToString();
                sCsv = sCsv + "," + dataRow["col_sumprice"].ToString();
                sCsv = sCsv + "," + dataRow["col_nouhin"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyu"].ToString();
                sCsv = sCsv + "," + dataRow["col_bikou"].ToString();
            }
            else if (m_nTableKind == 13)
            {
                sCsv = dataRow["col_id"].ToString();
                sCsv = sCsv + "," + dataRow["col_regdate"].ToString();
                sCsv = sCsv + "," + dataRow["col_orderdate"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyuid"].ToString();
                sCsv = sCsv + "," + dataRow["col_sougiid"].ToString();
                sCsv = sCsv + "," + dataRow["col_souke"].ToString();
                sCsv = sCsv + "," + dataRow["col_name"].ToString();
                sCsv = sCsv + "," + dataRow["col_tanka"].ToString();
                sCsv = sCsv + "," + dataRow["col_tani"].ToString();
                sCsv = sCsv + "," + dataRow["col_kosu"].ToString();
                sCsv = sCsv + "," + dataRow["col_taxkind"].ToString();
                sCsv = sCsv + "," + dataRow["col_tax"].ToString();
                sCsv = sCsv + "," + dataRow["col_price"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyusaki"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyubusyo"].ToString();
                sCsv = sCsv + "," + dataRow["col_skskeisyou"].ToString();
                sCsv = sCsv + "," + dataRow["col_skakeisyou"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyupost"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyuadrs1"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyuadrs2"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyutel"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyufax"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyupay"].ToString();
                sCsv = sCsv + "," + dataRow["col_seikyukubun"].ToString();
                sCsv = sCsv + "," + dataRow["col_receivedate"].ToString();
                sCsv = sCsv + "," + dataRow["col_receivekind"].ToString();
                sCsv = sCsv + "," + dataRow["col_vendername"].ToString();
                sCsv = sCsv + "," + dataRow["col_nafuda1"].ToString();
                sCsv = sCsv + "," + dataRow["col_nafuda2"].ToString();
                sCsv = sCsv + "," + dataRow["col_nafuda3"].ToString();
                sCsv = sCsv + "," + dataRow["col_nafuda4"].ToString();
                sCsv = sCsv + "," + dataRow["col_nafuda5"].ToString();
                sCsv = sCsv + "," + dataRow["col_nafuda6"].ToString();
                sCsv = sCsv + "," + dataRow["col_nafuda7"].ToString();
                sCsv = sCsv + "," + dataRow["col_nafuda8"].ToString();
                sCsv = sCsv + "," + dataRow["col_bikou"].ToString();
            }
            return (sCsv);
        }
        private void setButtonColor()
        {
            btnSyouhin.Background = m_brushDefButton;
            btnSeikyusaki.Background = m_brushDefButton;
            btnSeikyukubun.Background = m_brushDefButton;
            btnSasidasinin.Background = m_brushDefButton;
            btnFurikomisaki.Background = m_brushDefButton;
            btnSougi.Background = m_brushDefButton;
            btnSeikyu.Background = m_brushDefButton;
            btnHanbai.Background = m_brushDefButton;
            if (m_nTableKind == 1)
            {
                btnSyouhin.Background = Brushes.Cyan;
            }
            else if (m_nTableKind == 2)
            {
                btnSeikyusaki.Background = Brushes.Cyan;
            }
            else if (m_nTableKind == 3)
            {
                btnSeikyukubun.Background = Brushes.Cyan;
            }
            else if (m_nTableKind == 4)
            {
                btnSasidasinin.Background = Brushes.Cyan;
            }
            else if (m_nTableKind == 5)
            {
                btnFurikomisaki.Background = Brushes.Cyan;
            }
            else if (m_nTableKind == 11)
            {
                btnSougi.Background = Brushes.Cyan;
            }
            else if (m_nTableKind == 12)
            {
                btnSeikyu.Background = Brushes.Cyan;
            }
            else if (m_nTableKind == 13)
            {
                btnHanbai.Background = Brushes.Cyan;
            }
        }
        private void dgTable_BeginningEdit(object sender, DataGridBeginningEditEventArgs e)
        {
            m_bInEdit = true;
            if (dgTable.CurrentItem == null)
            {
                return;
            }
            m_nSlctIdx = dgTable.Items.IndexOf(dgTable.CurrentItem);
            if (dgTable.CurrentColumn == null)
            {
                return;
            }
            m_sBaindString = dgTable.CurrentColumn.SortMemberPath;
        }
        private void dgTable_CellEditEnding(object sender, DataGridCellEditEndingEventArgs e)
        {
            DataRowView dataRowView;
            string sDRText;
            string sTBText;

            if (m_nSlctIdx < m_dt.Rows.Count)
            {
                dataRowView = (DataRowView)dgTable.SelectedItems[0];
                sDRText = dataRowView[m_sBaindString].ToString();
            }
            else
            {
                dataRowView = null;
                sDRText = "";
            }
            TextBox element = e.EditingElement as TextBox;
            if (element != null)
            {
                sTBText = element.Text;
                if (sTBText != sDRText)
                {
                    if (dataRowView != null)
                    {
                        dataRowView["col_chk"] = 1;
                    }
                    m_bModify = true;
                }
            }
        }
        private void dgTable_CurrentCellChanged(object sender, EventArgs e)
        {
            if (m_bInEdit == true)
            {
                dgTable.CommitEdit();
                dgTable.CancelEdit();
            }
            if (dgTable.CurrentItem == null)
            {
                return;
            }
            m_nSlctIdx = dgTable.Items.IndexOf(dgTable.CurrentItem);
            if (dgTable.CurrentColumn == null)
            {
                return;
            }
            m_sBaindString = dgTable.CurrentColumn.SortMemberPath;
        }

        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            int max, idx;
            DataRowView dataRowView;
            int id;

            max = dgTable.SelectedItems.Count;
            if (max == 0)
            {
                return;
            }
            if (MessageBox.Show("選択されたデータが削除されますが、よろしいですか。", "削除確認", MessageBoxButton.YesNo,
                     MessageBoxImage.Information) == MessageBoxResult.Yes)
            {
                for (idx = 0; idx < max; idx++)
                {
                    if (dgTable.SelectedItems[idx].ToString() != "{NewItemPlaceholder}")
                    {
                        dataRowView = (DataRowView)dgTable.SelectedItems[idx];
                        id = (int)dataRowView.Row[2];
                        deleteRecord(id);
                    }
                }
                txtSearchStartDate.Text = "";
                txtSearchEndDate.Text = "";
                setCrtRecordTables("");
            }
        }
        private void deleteRecord(int id)
        {
            if (m_nTableKind == 1)
            {
                m_libOdbc.deleteSyouhin(id);
            }
            else if (m_nTableKind == 2)
            {
                m_libOdbc.deleteSeikyusaki(id);
            }
            else if (m_nTableKind == 3)
            {
                m_libOdbc.deleteSkkubun(id);
            }
            else if (m_nTableKind == 4)
            {
                m_libOdbc.deleteSasidasinin(id);
            }
            else if (m_nTableKind == 5)
            {
                m_libOdbc.deleteFurikomisaki(id);
            }
            else if (m_nTableKind == 11)
            {
                m_libOdbc.deleteSougi(id);
            }
            else if (m_nTableKind == 12)
            {
                m_libOdbc.deleteSeikyu(id);
            }
            else if (m_nTableKind == 13)
            {
                m_libOdbc.deleteHanbai(id);
            }
        }

        private void btnSelectStartDate_Click(object sender, RoutedEventArgs e)
        {
            CalWin calWin;

            m_txtboxDate = txtSearchStartDate;
            calWin = new CalWin();
            calWin.SetMasterWindow(this);
            calWin.m_sCrtDate = txtSearchStartDate.Text;
            Point pt = txtSearchStartDate.PointToScreen(new Point(0.0d, 0.0d));
            calWin.Left = pt.X;
            calWin.Top = pt.Y;
            calWin.Owner = this;
            //calWin.Topmost = true;
            //calWin.ShowDialog();
            calWin.ShowDialog();
        }

        private void btnSelectEndDate_Click(object sender, RoutedEventArgs e)
        {
            CalWin calWin;

            m_txtboxDate = txtSearchEndDate;
            calWin = new CalWin();
            calWin.SetMasterWindow(this);
            calWin.m_sCrtDate = txtSearchEndDate.Text;
            Point pt = txtSearchEndDate.PointToScreen(new Point(0.0d, 0.0d));
            calWin.Left = pt.X;
            calWin.Top = pt.Y;
            calWin.Owner = this;
            //calWin.Topmost = true;
            //calWin.ShowDialog();
            calWin.ShowDialog();
        }
        public void SetSlctDate(string sDate)
        {
            m_txtboxDate.Text = sDate;
        }
        private void btnExtract_Click(object sender, RoutedEventArgs e)
        {
            string sCondition;

            sCondition = "";
            if (txtSearchStartDate.Text != "")
            {
                sCondition = sCondition + "(StrComp(regdate,'" + txtSearchStartDate.Text + " 00:00') >= 0)";
            }
            if (txtSearchEndDate.Text != "指定なし")
            {
                if (sCondition != "")
                {
                    sCondition = sCondition + " AND ";
                }
                sCondition = sCondition + "(StrComp(regdate,'" + txtSearchEndDate.Text + " 23:59') <= 0)";
            }
            setCrtRecordTables(sCondition);
        }
    }
}
