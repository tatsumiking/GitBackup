using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Printing;

namespace instr
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        private int m_nFirst;
        private Boolean m_bErrFlag;
        private string m_sMsg;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibCurve m_libCurve;
        public LibCanvas m_libCanvas;
        public LibEsFont m_libEsFont;
        public LibWinFont m_libWinFont;
        public ScrnEnv m_scrnEnv;
        private string m_sWakuBasePath;
        private string m_sFontBasePath;
        private DataTable m_dt;
        private Boolean m_bModify;
        private int m_nNafudaSlctLine;
        private int m_nNafudaSlctColumn;
        private string m_sBaindString;
        private Boolean m_bInEdit;
        private double m_dXSize, m_dYSize;

        public MainWindow()
        {
            Boolean ret;

            m_bErrFlag = false;
            InitializeComponent();
            m_nFirst = 1;
            m_libCmn = new LibCommon();
            m_libCurve = new LibCurve();
            m_libCanvas = new LibCanvas();
            m_libEsFont = new LibEsFont();
            m_libWinFont = new LibWinFont();
            m_scrnEnv = new ScrnEnv();
            m_sExecPath = InitExePath();
            m_bModify = false;
            m_nNafudaSlctLine = 0;
            m_sBaindString = "col_wakuname";
            ret = loadEnv();
            if (ret == false)
            {
                m_bErrFlag = true;
            }
            else
            {
                m_libWinFont.SetLibCmn(m_libCmn);
                m_libWinFont.SetExecPath(m_sExecPath);
                m_libWinFont.SetWinFontPath("c:\\windows\\Fonts");

                m_libEsFont.SetLibCmn(m_libCmn);
                m_libEsFont.SetLibCurve(m_libCurve);
                m_libEsFont.SetEsFontPath(m_sFontBasePath);
            }
        }
        private Boolean loadEnv()
        {
            string sFileName;
            string[] aryLine;

            sFileName = m_sExecPath + "\\instr";
            m_libCmn.CreatePath(sFileName);

            sFileName = m_sExecPath + "\\ses_sys.env";
            if (!(System.IO.File.Exists(sFileName)))
            {
                MessageBox.Show(sFileName+"ファイルが見つかりません", "確認", MessageBoxButton.OK);
                return (false);
            }
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            if (12 <= aryLine.Length)
            {
                m_sFontBasePath = aryLine[7];
                m_sWakuBasePath = aryLine[11];
            }

            sFileName = m_sExecPath + "\\SOAINFO.ENV";
            if (!(System.IO.File.Exists(sFileName)))
            {
                MessageBox.Show(sFileName + "ファイルが見つかりません", "確認", MessageBoxButton.OK);
                return (false);
            }
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            if (9 <= aryLine.Length)
            {
                aryLine[8] = m_libCmn.DeleteDoubleQuotation(aryLine[8]);
                aryLine[8] = setNafudaAreaSize(aryLine[8]);
                lblFolderName.Content = aryLine[8];
            }
            return (true);
        }
        private string setNafudaAreaSize(string sFolder)
        {
            string sFileName;
            string[] aryLine;
            int max, idx;
            string[] ary;

            sFileName = m_sWakuBasePath + "\\PAPERLST.LST";
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_dXSize = 0;
            m_dYSize = 0;
            max = aryLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                ary = aryLine[idx].Split(',');
                ary[0] = m_libCmn.DeleteDoubleQuotation(ary[0]);
                if (ary[0] == sFolder)
                {
                    m_dXSize = m_libCmn.StrToDouble(ary[1]);
                    m_dYSize = m_libCmn.StrToDouble(ary[2]);
                    break;
                }
            }
            if (m_libCmn.IsZero(m_dXSize) == true && m_libCmn.IsZero(m_dYSize) == true)
            {
                ary = aryLine[0].Split(',');
                ary[0] = m_libCmn.DeleteDoubleQuotation(ary[0]);
                sFolder = ary[0];
                m_dXSize = m_libCmn.StrToDouble(ary[1]);
                m_dYSize = m_libCmn.StrToDouble(ary[2]);
            }
            return (sFolder);
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_bErrFlag == true)
            {
                this.Close();
                return;
            }
            initDgNafuda();
            this.WindowState = WindowState.Maximized;

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
        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.F1)
            {
                execSmsOut();
                e.Handled = true;

            }
            else if (e.Key == Key.F12)
            {
                execExit();
                e.Handled = true;

            }
            else if (e.Key == Key.Return)
            {
                if (checkWakuSelectColumn() == true)
                {
                    e.Handled = true;
                    if (m_dt.Rows.Count <= m_nNafudaSlctLine)
                    {
                        selectWakuInseart();
                    }
                    else
                    {
                        selectWakuUpdate();
                    }
                }
                else if (checkCsvOutColumn() == true)
                {
                    e.Handled = true;
                    saveCsvLineOut();
                }
            }
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            SaveTempCsvFile();
        }
        private void btnLoad_Click(object sender, RoutedEventArgs e)
        {
            string sLoadFileName;
            string[] sLine;

            OpenFileDialog ifd = new OpenFileDialog();
            ifd.Filter = "読込みファイル(*.csv)|*.csv|すべてのファイル(*.*)|*.*||";
            ifd.Title = "読込みファイルを選択してください";
            ifd.InitialDirectory = m_sExecPath + "\\instr";
            ifd.DefaultExt = "*.csv";
            if (ifd.ShowDialog() == true)
            {
                sLoadFileName = ifd.FileName;
                sLine = m_libCmn.LoadFileLineSJIS(sLoadFileName);
                setCsvListToDataGridData(sLine);
            }
        }
        private void btnList_Click(object sender, RoutedEventArgs e)
        {
            printMain(); // MainPrinting.cs
        }
        private void btnOut_Click(object sender, RoutedEventArgs e)
        {

            string sFileName;
            StreamWriter stream;
            int max, idx;
            DataRow rowItem;
            string sBool;
            Encoding encode;
            string sCsv;

            sFileName = m_sExecPath + "\\outcrect.dat"; 
            encode = System.Text.Encoding.GetEncoding("shift_jis");
            stream = new StreamWriter(sFileName, false, encode);
            max = m_dt.Rows.Count;
            for(idx = 0; idx < max; idx++){
                rowItem = m_dt.Rows[idx];
                sBool = rowItem["col_chk"].ToString();
                if (sBool == "True")
                {
                    sCsv = createCrctCsvLine(rowItem);
                    stream.Write(sCsv + stream.NewLine);
                }
            }
            stream.Close();
            execSmsOut();
        }
        private string getDataGridDataToCsvString()
        {
            string sData;
            int max, idx;
            DataRow rowItem;

            sData = "";
            max = m_dt.Rows.Count;
            for (idx = 0; idx < max; idx++)
            {
                rowItem = m_dt.Rows[idx];
                sData = sData + rowItem["col_chk"].ToString();
                sData = sData + "," + rowItem["col_wakuname"].ToString();
                sData = sData + "," + rowItem["col_souke"].ToString();
                sData = sData + "," + rowItem["col_count"].ToString();
                sData = sData + "," + rowItem["col_waku1"].ToString();
                sData = sData + "," + rowItem["col_waku2"].ToString();
                sData = sData + "," + rowItem["col_waku3"].ToString();
                sData = sData + "," + rowItem["col_waku4"].ToString();
                sData = sData + "," + rowItem["col_waku5"].ToString();
                sData = sData + "," + rowItem["col_waku6"].ToString();
                sData = sData + "," + rowItem["col_waku7"].ToString();
                sData = sData + "," + rowItem["col_waku8"].ToString();
                sData = sData + ",\n";
            }

            return (sData);
        }
        private void setCsvListToDataGridData(string[] sLine)
        {
            string[] sAry;
            int max, idx;
            DataRow rowItem;

            sAry = sLine[0].Split(',');
            if (sAry.Length < 12)
            {
                MessageBox.Show("ファイルが正しくありません", "確認", MessageBoxButton.OK);
                return;
            }
            m_dt.Rows.Clear();
            m_nNafudaSlctLine = -1;
            max = sLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                sAry = sLine[idx].Split(',');
                if (sAry.Length < 12)
                {
                    break;
                }
                rowItem = m_dt.NewRow();
                rowItem["col_no"] = idx + 1;
                if (sAry[0] == "True")
                {
                    rowItem["col_chk"] = true;
                }
                else
                {
                    rowItem["col_chk"] = false;
                }
                rowItem["col_wakuname"] = sAry[1];
                rowItem["col_souke"] = sAry[2];
                rowItem["col_count"] = m_libCmn.StrToInt(sAry[3]);
                rowItem["col_waku1"] = sAry[4];
                rowItem["col_waku2"] = sAry[5];
                rowItem["col_waku3"] = sAry[6];
                rowItem["col_waku4"] = sAry[7];
                rowItem["col_waku5"] = sAry[8];
                rowItem["col_waku6"] = sAry[9];
                rowItem["col_waku7"] = sAry[10];
                rowItem["col_waku8"] = sAry[11];
                m_dt.Rows.Add(rowItem);
            }
            dgNafuda.DataContext = m_dt;
        }
        private string createCrctCsvLine(DataRow rowItem)
        {
            string sWakuName;
            string sPaperName;
            string sWakuFileName;
            string sCsv;
            int flag;
            string str;

            sPaperName = lblFolderName.Content.ToString();
            sWakuName = rowItem["col_wakuname"].ToString();
            sWakuFileName = m_sWakuBasePath + "\\" + sPaperName + "\\" + sWakuName;
            sCsv = "\"未\",\"waku\"";
            sCsv = sCsv + ",\"" + sWakuName + "\"";
            sCsv = sCsv + ",\"" + sWakuFileName + "\"";
            sCsv = sCsv + ",\"" + rowItem["col_count"].ToString() + "\"";
            flag = 0;
            str = rowItem["col_waku1"].ToString();
            sCsv = sCsv + ",\"/" + str;
            str = rowItem["col_waku2"].ToString();
            if (str == "CSV出力")
            {
                flag = 1;
            }
            else if (flag == 0)
            {
                sCsv = sCsv + "/" + str;
            }
            str = rowItem["col_waku3"].ToString();
            if (str == "CSV出力")
            {
                flag = 1;
            }
            else if (flag == 0)
            {
                sCsv = sCsv + "/" + str;
            }
            str = rowItem["col_waku4"].ToString();
            if (str == "CSV出力")
            {
                flag = 1;
            }
            else if (flag == 0)
            {
                sCsv = sCsv + "/" + str;
            }
            str = rowItem["col_waku5"].ToString();
            if (str == "CSV出力")
            {
                flag = 1;
            }
            else if (flag == 0)
            {
                sCsv = sCsv + "/" + str;
            }
            str = rowItem["col_waku6"].ToString();
            if (str == "CSV出力")
            {
                flag = 1;
            }
            else if (flag == 0)
            {
                sCsv = sCsv + "/" + str;
            }
            str = rowItem["col_waku7"].ToString();
            if (str == "CSV出力")
            {
                flag = 1;
            }
            else if (flag == 0)
            {
                sCsv = sCsv + "/" + str;
            }
            sCsv = sCsv + "\"";
            return (sCsv);
        }
        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            execExit();
        }
        private void execSmsOut()
        {
            string sProcessName;
            System.Diagnostics.Process process;

            sProcessName = "smsout.exe";
            if (File.Exists(sProcessName))
            {
                process = new System.Diagnostics.Process();
                process.StartInfo.FileName = sProcessName;
                process.Start();
            }
            else
            {
                MessageBox.Show("連続出力用プログラムが見つかりません", "確認", MessageBoxButton.OK);
            }
        }
        private void execExit()
        {
            this.Close();
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            SaveTempCsvFile();
        }
        private void SaveTempCsvFile()
        {
            string sSaveFileName;
            string sData;

            if (m_dt == null)
            {
                return;
            }
            if (m_dt.Rows.Count == 0)
            {
                return;
            }
            DataRow rowItem = m_dt.Rows[0];
            DateTime dt = DateTime.Now;
            sSaveFileName = m_sExecPath + "\\instr\\"+ dt.ToString("yyyyMMdd") + rowItem["col_souke"].ToString() + ".csv";
            sData = getDataGridDataToCsvString();
            m_libCmn.SaveFileSJIS(sSaveFileName, sData);
        }
        private void initDgNafuda()
        {
            m_libCmn.CreateAddTextColNonEdit(dgNafuda, "col_no", "No", 30);
            m_libCmn.CreateAddCheckBoxCol(dgNafuda, "col_chk", "印", 10);
            m_libCmn.CreateAddTextColNonEdit(dgNafuda, "col_wakuname", "枠名", 80);
            m_libCmn.CreateAddTextCol(dgNafuda, "col_souke", "葬家", 60);
            m_libCmn.CreateAddTextCol(dgNafuda, "col_count", "枚", 30);
            m_libCmn.CreateAddTextCol(dgNafuda, "col_waku1", "１行目", 80);
            m_libCmn.CreateAddTextCol(dgNafuda, "col_waku2", "２行目", 80);
            m_libCmn.CreateAddTextCol(dgNafuda, "col_waku3", "３行目", 80);
            m_libCmn.CreateAddTextCol(dgNafuda, "col_waku4", "４行目", 80);
            m_libCmn.CreateAddTextCol(dgNafuda, "col_waku5", "５行目", 80);
            m_libCmn.CreateAddTextCol(dgNafuda, "col_waku6", "６行目", 80);
            m_libCmn.CreateAddTextCol(dgNafuda, "col_waku7", "７行目", 80);
            m_libCmn.CreateAddTextCol(dgNafuda, "col_waku8", "８行目", 80);
            m_dt = new DataTable("DataGridTest");
            m_dt.Columns.Add(new DataColumn("col_no", typeof(int)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_chk", typeof(string)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_wakuname", typeof(string)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_souke", typeof(string)));                // 数値
            m_dt.Columns.Add(new DataColumn("col_count", typeof(int)));        // 文字列
            m_dt.Columns.Add(new DataColumn("col_waku1", typeof(string)));         // 文字列
            m_dt.Columns.Add(new DataColumn("col_waku2", typeof(string)));         // 文字列
            m_dt.Columns.Add(new DataColumn("col_waku3", typeof(string)));         // 文字列
            m_dt.Columns.Add(new DataColumn("col_waku4", typeof(string)));         // 文字列
            m_dt.Columns.Add(new DataColumn("col_waku5", typeof(string)));         // 文字列
            m_dt.Columns.Add(new DataColumn("col_waku6", typeof(string)));         // 文字列
            m_dt.Columns.Add(new DataColumn("col_waku7", typeof(string)));         // 文字列
            m_dt.Columns.Add(new DataColumn("col_waku8", typeof(string)));         // 文字列
            dgNafuda.DataContext = m_dt;
        }
        private string getWakuBindingString(int idx)
        {
            string sBindingString;

            sBindingString = "";
            switch (idx)
            {
            case 0:
                sBindingString = "col_waku1";
                break;
            case 1:
                sBindingString = "col_waku2";
                break;
            case 2:
                sBindingString = "col_waku3";
                break;
            case 3:
                sBindingString = "col_waku4";
                break;
            case 4:
                sBindingString = "col_waku5";
                break;
            case 5:
                sBindingString = "col_waku6";
                break;
            case 6:
                sBindingString = "col_waku7";
                break;
            case 7:
                sBindingString = "col_waku8";
                break;
            }
            return (sBindingString);
        }
        private void dgNafuda_CurrentCellChanged(object sender, EventArgs e)
        {
            if (m_bInEdit == true)
            {
//                dgNafuda.CommitEdit();
//                dgNafuda.CancelEdit();
            }

            if (dgNafuda.CurrentItem == null)
            {
                return;
            }
            if (dgNafuda.CurrentColumn == null)
            {
                return;
            }
            m_sBaindString = dgNafuda.CurrentColumn.SortMemberPath;
            m_nNafudaSlctLine = dgNafuda.Items.IndexOf(dgNafuda.CurrentItem);
            m_sMsg = "Select line " + m_nNafudaSlctLine + " column " + m_nNafudaSlctColumn;
            System.Diagnostics.Trace.WriteLine(m_sMsg);
            if (m_sBaindString == "col_no" || m_sBaindString == "col_wakuname")
            {
                if (m_dt.Rows.Count <= m_nNafudaSlctLine)
                {
                    selectWakuInseart();
                }
                else
                {
                    selectWakuUpdate();
                }
            }
            else
            {
                drawCanvasImage();
            }
        }
        private void dgNafuda_BeginningEdit(object sender, DataGridBeginningEditEventArgs e)
        {
            m_bInEdit = true;
        }

        private void dgNafuda_CellEditEnding(object sender, DataGridCellEditEndingEventArgs e)
        {
            DataRow rowItem;

            //m_sMsg = "編集終了CellEditEnding";
            //System.Diagnostics.Trace.WriteLine(m_sMsg);
            SetModify(true);
            TextBox element = e.EditingElement as TextBox;
            if (element != null)
            {
                string text = element.Text;
                if (text != "CSV出力")
                {
                    if (m_nNafudaSlctLine < m_dt.Rows.Count)
                    {
                        rowItem = m_dt.Rows[m_nNafudaSlctLine];
                        rowItem[m_sBaindString] = text;
                        dgNafuda.DataContext = m_dt;
                    }
                }
            }
            drawCanvasImage();
            m_bInEdit = false;
        }
        private Boolean checkWakuSelectColumn()
        {
            if (m_sBaindString == "col_no" || m_sBaindString == "col_wakuname")
            {
                return(true);
            }
            return (false);
        }
        private Boolean checkCsvOutColumn()
        {
            DataRow rowItem;
            string str;

            if (m_dt.Rows.Count <= m_nNafudaSlctLine)
            {
                return (false);
            }
            rowItem = m_dt.Rows[m_nNafudaSlctLine];
            str = rowItem[m_sBaindString].ToString();
            if (str == "CSV出力")
            {
                return (true);
            }
            return (false);
        }
        private void saveCsvLineOut()
        {
            DataRow rowItem;
            string sCsv, str;
            string sFileName;
            StreamWriter stream;

            sCsv = "";
            rowItem = m_dt.Rows[m_nNafudaSlctLine];
            if (MessageBox.Show("CSV出力しますか", "CSV出力", MessageBoxButton.YesNo,
                     MessageBoxImage.Information) == MessageBoxResult.Yes)
            {
                sCsv = rowItem["col_wakuname"].ToString();
                sCsv = sCsv + "," + rowItem["col_souke"].ToString();
                sCsv = sCsv + "," + rowItem["col_count"].ToString();
                str = rowItem["col_waku1"].ToString();
                sCsv = sCsv + "," + str;
                str = rowItem["col_waku2"].ToString();
                if (str == "CSV出力")
                {
                    sCsv = sCsv + ",";
                }
                else
                {
                    sCsv = sCsv + "," + str;
                }
                str = rowItem["col_waku3"].ToString();
                if (str == "CSV出力")
                {
                    sCsv = sCsv + ",";
                }
                else
                {
                    sCsv = sCsv + "," + str;
                }
                str = rowItem["col_waku4"].ToString();
                if (str == "CSV出力")
                {
                    sCsv = sCsv + ",";
                }
                else
                {
                    sCsv = sCsv + "," + str;
                }
                str = rowItem["col_waku5"].ToString();
                if (str == "CSV出力")
                {
                    sCsv = sCsv + ",";
                }
                else
                {
                    sCsv = sCsv + "," + str;
                }
                str = rowItem["col_waku6"].ToString();
                if (str == "CSV出力")
                {
                    sCsv = sCsv + ",";
                }
                else
                {
                    sCsv = sCsv + "," + str;
                }
                str = rowItem["col_waku7"].ToString();
                if (str == "CSV出力")
                {
                    sCsv = sCsv + ",";
                }
                else
                {
                    sCsv = sCsv + "," + str;
                }
                sCsv = sCsv + ",,,,";
                sFileName = m_sExecPath + "\\instr\\instr.csv";
                stream = new System.IO.StreamWriter(
                    sFileName, true, System.Text.Encoding.GetEncoding("shift_jis"));
                stream.Write(sCsv + "\r\n");
                stream.Close();
            }
            rowItem["col_chk"] = true;
            dgNafuda.DataContext = m_dt;

            drawCanvasImage();

            DoDispatch();
            m_nNafudaSlctColumn = 2;
            m_nNafudaSlctLine = m_nNafudaSlctLine + 1;
            dgNafuda.SetSelect(m_nNafudaSlctLine, m_nNafudaSlctColumn);
        }
        private void drawCanvasImage()
        {
            DataRow rowItem;
            string sWakuName;

            if (m_nNafudaSlctLine < m_dt.Rows.Count)
            {
                rowItem = m_dt.Rows[m_nNafudaSlctLine];
                sWakuName = rowItem["col_wakuname"].ToString();
                drawMainCanvasImage(sWakuName);
            }
        }
        private void selectWakuInseart()
        {
            SelectWakuWin selectWakuWin;
            string sWakuPath;

            m_nFirst = 0;
            sWakuPath = m_sWakuBasePath + "\\" + lblFolderName.Content.ToString();
            selectWakuWin = new SelectWakuWin();
            selectWakuWin.SetMainWindow(this);
            selectWakuWin.SetInsertMode();
            selectWakuWin.SetNafudaElement(sWakuPath, m_dXSize, m_dYSize);
            Point pt = dgNafuda.PointToScreen(new Point(120.0d, 20.0d));
            selectWakuWin.Left = pt.X;
            selectWakuWin.Top = pt.Y;
            selectWakuWin.Owner = this;
            selectWakuWin.ShowDialog();
        }
        private void selectWakuUpdate()
        {
            SelectWakuWin selectWakuWin;
            string sWakuPath;

            sWakuPath = m_sWakuBasePath + "\\" + lblFolderName.Content.ToString();
            selectWakuWin = new SelectWakuWin();
            selectWakuWin.SetMainWindow(this);
            selectWakuWin.SetUpdateMode();
            selectWakuWin.SetNafudaElement(sWakuPath, m_dXSize, m_dYSize);
            Point pt = dgNafuda.PointToScreen(new Point(120.0d, 20.0d));
            selectWakuWin.Left = pt.X;
            selectWakuWin.Top = pt.Y;
            selectWakuWin.Owner = this;
            selectWakuWin.ShowDialog();
        }
        public void CreateRowItem(string sWakuName)
        {
            string sWakuPath;
            string sWakuFileName;
            string sSouke;
            string[] aryLine;
            string[] aryCsv;
            string sDefStr;
            DataRow rowItem;

            if (m_nNafudaSlctLine == -1)
            {
                return;
            }
            sWakuPath = m_sWakuBasePath + "\\" + lblFolderName.Content.ToString();
            sWakuFileName = sWakuPath + "\\" + sWakuName;
            aryLine = m_libCmn.LoadFileLineSJIS(sWakuFileName);
            if (m_nNafudaSlctLine == 0)
            {
                sSouke = "";
            }
            else
            {
                rowItem = m_dt.Rows[m_nNafudaSlctLine-1];
                sSouke = rowItem["col_souke"].ToString();
            }
            rowItem = m_dt.NewRow();
            rowItem["col_no"] = m_nNafudaSlctLine + 1;
            rowItem["col_chk"] = false;
            rowItem["col_wakuname"] = sWakuName;
            rowItem["col_souke"] = sSouke;
            rowItem["col_count"] = 1;
            if (1 <= aryLine.Length)
            {
                aryCsv = aryLine[0].Split(',');
                if(2 < aryCsv.Length){
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku1"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku1"] = "CSV出力";
                }
            }
            if (2 <= aryLine.Length)
            {
                aryCsv = aryLine[1].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku2"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku2"] = "CSV出力";
                }
            }
            if (3 <= aryLine.Length)
            {
                aryCsv = aryLine[2].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku3"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku3"] = "CSV出力";
                }
            }
            if (4 <= aryLine.Length)
            {
                aryCsv = aryLine[3].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku4"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku4"] = "CSV出力";
                }
            }
            if (5 <= aryLine.Length)
            {
                aryCsv = aryLine[4].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku5"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku5"] = "CSV出力";
                }
            }
            if (6 <= aryLine.Length)
            {
                aryCsv = aryLine[5].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku6"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku6"] = "CSV出力";
                }
            }
            if (7 <= aryLine.Length)
            {
                aryCsv = aryLine[6].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku7"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku7"] = "CSV出力";
                }
            }
            m_dt.Rows.Add(rowItem);
            dgNafuda.DataContext = m_dt;
            drawCanvasImage();
            DoDispatch();

            m_nNafudaSlctColumn = 3;
            m_nNafudaSlctLine = m_dt.Rows.Count - 1;
            dgNafuda.SetSelect(m_nNafudaSlctLine, m_nNafudaSlctColumn);

        }
        public void UpdateRowItem(string sWakuName)
        {
            string sWakuPath;
            string sWakuFileName;
            string[] aryLine;
            string[] aryCsv;
            string sDefStr;
            DataRow rowItemPre;
            DataRow rowItem;

            sWakuPath = m_sWakuBasePath + "\\" + lblFolderName.Content.ToString();
            sWakuFileName = sWakuPath + "\\" + sWakuName;
            aryLine = m_libCmn.LoadFileLineSJIS(sWakuFileName);
            rowItem = m_dt.Rows[m_nNafudaSlctLine];
            if (rowItem["col_souke"] == "")
            {
                if (m_nNafudaSlctLine != 0)
                {
                    rowItemPre = m_dt.Rows[m_nNafudaSlctLine - 1];
                    rowItem["col_souke"] = rowItemPre["col_souke"].ToString();

                }
            }
            rowItem["col_chk"] = false;
            rowItem["col_wakuname"] = sWakuName;
            rowItem["col_count"] = 1;
            rowItem["col_waku1"] = "";
            rowItem["col_waku2"] = "";
            rowItem["col_waku3"] = "";
            rowItem["col_waku4"] = "";
            rowItem["col_waku5"] = "";
            rowItem["col_waku6"] = "";
            rowItem["col_waku7"] = "";
            if (1 <= aryLine.Length)
            {
                aryCsv = aryLine[0].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku1"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku1"] = "CSV出力";
                }
            }
            if (2 <= aryLine.Length)
            {
                aryCsv = aryLine[1].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku2"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku2"] = "CSV出力";
                }
            }
            if (3 <= aryLine.Length)
            {
                aryCsv = aryLine[2].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku3"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku3"] = "CSV出力";
                }
            }
            if (4 <= aryLine.Length)
            {
                aryCsv = aryLine[3].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku4"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku4"] = "CSV出力";
                }
            }
            if (5 <= aryLine.Length)
            {
                aryCsv = aryLine[4].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku5"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku5"] = "CSV出力";
                }
            }
            if (6 <= aryLine.Length)
            {
                aryCsv = aryLine[5].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku6"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku6"] = "CSV出力";
                }
            }
            if (7 <= aryLine.Length)
            {
                aryCsv = aryLine[6].Split(',');
                if (2 < aryCsv.Length)
                {
                    sDefStr = m_libCmn.DeleteDoubleQuotation(aryCsv[1]);
                    rowItem["col_waku7"] = sDefStr;
                }
                else
                {
                    rowItem["col_waku7"] = "CSV出力";
                }
            }
            dgNafuda.DataContext = m_dt;
            drawCanvasImage();
            DoDispatch();

            m_nNafudaSlctColumn = 3;
            dgNafuda.SetSelect(m_nNafudaSlctLine, m_nNafudaSlctColumn);
            // setBackGroundColor(m_nNafudaSlctLine, m_nNafudaSlctCsvColumn);
        }

        private DataGridCell getDataGridCell(int line, int column)
        {
            DataGridCell cell;

            DataGridRow row = (DataGridRow)dgNafuda.ItemContainerGenerator.ContainerFromIndex(line);
            if (row == null)
            {
                return (null);
            }
            DataGridCellsPresenter presenter = GetVisualChild<DataGridCellsPresenter>(row);
            if (presenter == null)
            {
                return (null);
            }
            cell = (DataGridCell)presenter.ItemContainerGenerator.ContainerFromIndex(column);
            return (cell);
        }
        public static T GetVisualChild<T>(Visual parent) where T : Visual
        {
            T result = default(T);
            int childrenCount = VisualTreeHelper.GetChildrenCount(parent);
            for (int i = 0; i < childrenCount; i++)
            {
                Visual child = (Visual)VisualTreeHelper.GetChild(parent, i);
                result = child as T;
                if (result == null)
                {
                    result = GetVisualChild<T>(child);
                }
                if (result != null)
                {
                    break;
                }
            }
            return result;
        }
        private void btnFolderSelect_Click(object sender, RoutedEventArgs e)
        {
            string sWakuPath;

            System.Windows.Forms.FolderBrowserDialog fbd = new System.Windows.Forms.FolderBrowserDialog();
            sWakuPath = m_sWakuBasePath + "\\" + lblFolderName.Content.ToString();
            fbd.SelectedPath = sWakuPath;
            System.Windows.Forms.DialogResult result = fbd.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                sWakuPath = fbd.SelectedPath;
                lblFolderName.Content = System.IO.Path.GetFileName(sWakuPath);
            }
        }
        private void setBackGroundColor(int line, int column)
        {
            // 行追加時の処理が確立できず保留
            if (line == -1 && column == -1)
            {
                return;
            }
            DataGridCell cell = getDataGridCell(line, column);
            SolidColorBrush sbRed = new SolidColorBrush(Colors.Red);
            cell.Background = sbRed;
        }
        public void DoDispatch()
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
    }
}
