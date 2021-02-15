using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml;
using System.Xml.Serialization;

namespace sekiji
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        public ClsHaiseki m_clsHaiseki;
        public string m_sExecPath;
        public string m_sEnvPath;
        public string m_sItiranPath;
        public LibCommon m_libCmn;
        public LibCanvas m_libCanvas;
        public LibOdbc m_libOdbc;
        List<FontFamily> m_lstFontFamilys;
        List<string> m_lstFontFamilyNames;
        FontFamily m_FontFamilyCrt;
        string m_sFontFamilyNameCrt;
        private string m_sWakuBasePath;
        private string m_sSekiFudaFolder;
        private string m_sAnySekijiFolder;
        private string m_sSekiFudaWaku;
        private string m_sAnySekijiWaku;
        private int m_nAnySekijiCount;
        private Boolean m_bModify;
        TextBox m_textboxInput;
        Canvas m_cnvsCrtParent;
        private int m_nCrtTable;
        private int m_nCrtSeet;
        private int m_nSelectTable;
        private int m_nSelectSeet;
        private int m_nSSeet, m_nESeet;
        private Boolean m_bMoveSeet;
        Brush m_brushNomal;
        Thickness m_thicknessNomal;
        Brush m_brushSlelct;
        Thickness m_thicknessSelect;
        private int m_nCrtHikimonoIdx;

        public MainWindow()
        {
            InitializeComponent();
            m_bModify = false;
            m_sExecPath = initExePath();
            App.m_sExecPath = m_sExecPath;
            m_libCmn = new LibCommon();
            m_libCanvas = new LibCanvas();
            m_libOdbc = new LibOdbc();
            m_libOdbc.setExecPath(m_sExecPath);
            m_libOdbc.setLibCommonClass(m_libCmn);
            m_sEnvPath = initEnvPath();
            m_clsHaiseki = new ClsHaiseki();
            m_clsHaiseki.createListElement();
            m_nCrtTable = -1;
            m_nCrtSeet = -1;
            m_bMoveSeet = false;
            m_brushNomal = txtSouke.BorderBrush;
            m_thicknessNomal = txtSouke.BorderThickness;
            m_brushSlelct = m_libCanvas.createBruch(0x00ff0000);
            m_thicknessSelect = new Thickness(3);
            initElement();
        }
        private void initElement()
        {
            loadSekijiEnv();
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            m_textboxInput = new TextBox();
            m_textboxInput.GotFocus += new RoutedEventHandler(TextBox_GotFocus);

            m_cnvsCrtParent = null;
            initCmbFont();
            initCmbLayout();
        }
        private void initCmbFont()
        {
            // ICollection<FontFamily>
            m_lstFontFamilys = new List<FontFamily>();
            m_lstFontFamilyNames = new List<String>();
            String[] ary;
            int sidx, nSelectIdx;

            sidx = 0;
            nSelectIdx = 0;
            ary = new String[10];
            foreach (FontFamily fonts in Fonts.SystemFontFamilies)
            {
                int idx = 0;
                foreach (var key in fonts.FamilyNames.Keys)
                {
                    if ("ja-jp" == key.ToString())
                    {
                        Label item = new Label();
                        item.FontFamily = fonts;
                        fonts.FamilyNames.Values.CopyTo(ary, 0);
                        item.Content = ary[idx];
                        item.FontSize = 16;
                        cmbFont.Items.Add(item);
                        m_lstFontFamilys.Add(fonts);
                        m_lstFontFamilyNames.Add(ary[idx]);
                        if (m_sFontFamilyNameCrt == ary[idx])
                        {
                            m_FontFamilyCrt = fonts;
                            nSelectIdx = sidx;
                        }
                        sidx++;
                    }
                    idx++;
                }
            }
            cmbFont.SelectedIndex = nSelectIdx;
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            MessageBoxResult result;

            if (m_bModify == false)
            {
                result = MessageBox.Show("データは保存しますか？", "確認", MessageBoxButton.OKCancel);
                if (MessageBoxResult.OK == result)
                {
                    saveDialogSekiji();
                }
            }
            saveSekijiEnv();
            // e.Cancel = true;
            return;
        }
        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            resetTable();
            drawTable();
        }
        private void TextBox_GotFocus(object sender, EventArgs e)
        {
            TextBox tb;

            tb = (TextBox)sender;
            InputMethod.SetPreferredImeState(tb, InputMethodState.On);
        }
        private void cmbFont_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int idx = cmbFont.SelectedIndex;

            m_FontFamilyCrt = m_lstFontFamilys[idx];
            m_sFontFamilyNameCrt = m_lstFontFamilyNames[idx];
            drawTable();
        }
        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
            {
                m_bModify = true;
                if (m_cnvsCrtParent == null)
                {
                    return;
                }
                resetInputTextBox(m_nSelectTable, m_nSelectSeet);
                m_nSelectTable = m_nCrtTable;
                m_nSelectSeet = m_nCrtSeet;
                ModifierKeys modifierKeys = Keyboard.Modifiers;
                if ((modifierKeys & ModifierKeys.Shift) != ModifierKeys.None)
                {
                    m_nSelectSeet--;
                }
                else
                {
                    m_nSelectSeet++;
                }

                if (m_nSelectTable == -1)
                {
                    if (m_clsHaiseki.m_nSouryoCount <= m_nSelectSeet)
                    {
                        m_nSelectTable = 0;
                        m_nSelectSeet = 0;
                    }
                    else if (m_nSelectSeet < 0)
                    {
                        m_nSelectTable = -1;
                        m_nSelectSeet = 0;
                    }
                }
                else
                {
                    if (m_clsHaiseki.m_nSeetCount <= m_nSelectSeet)
                    {
                        m_nSelectTable++;
                        if (m_clsHaiseki.m_nTableBlockCount * 2 <= m_nSelectTable)
                        {
                            m_nSelectTable = m_nCrtTable;
                            m_nSelectSeet = m_nCrtSeet;
                        }
                        else
                        {
                            m_nSelectSeet = 0;
                        }
                    }
                    else if (m_nSelectSeet < 0)
                    {
                        m_nSelectTable--;
                        if (m_nSelectTable < 0)
                        {
                            m_nSelectSeet = m_clsHaiseki.m_nSouryoCount - 1;
                        }
                        else
                        {
                            m_nSelectSeet = m_clsHaiseki.m_nSeetCount - 1;
                        }
                    }
                }

                setInputTextBox(m_nSelectTable, m_nSelectSeet);

                m_nSSeet = m_nSelectSeet;
                resetSelect();
                setSelect();
            }
        }
        private void resetInputTextBox(int tidx, int sidx)
        {
            TextBlock tblk;

            if (m_cnvsCrtParent != null && sidx != -1)
            {
                tblk = (TextBlock)m_cnvsCrtParent.Children[0];
                tblk.Text = m_textboxInput.Text;
                m_cnvsCrtParent.Children.Remove(m_textboxInput);
                m_cnvsCrtParent = null;
            }
        }

        private void setInputTextBox(int tidx, int sidx)
        {
            TextBlock tblk;
            Canvas cnvs;

            if (tidx == -1)
            {
                cnvs = m_clsHaiseki.m_lstSouryo[sidx].getNameCanvas();
                m_textboxInput.Width = m_dSouryoWidth - m_dLineHeight * 1.4;
                m_textboxInput.Height = m_dLineHeight;
            }
            else
            {
                cnvs = m_clsHaiseki.m_lstTable[tidx].m_lstSekiji[sidx].getNameCanvas();
                m_textboxInput.Width = m_dTableWidth - m_dLineHeight * 1.4;
                m_textboxInput.Height = m_dLineHeight;
            }
            m_cnvsCrtParent = cnvs;
            tblk = (TextBlock)m_cnvsCrtParent.Children[0];
            m_textboxInput.Text = tblk.Text;
            m_cnvsCrtParent.Children.Add(m_textboxInput);
            m_textboxInput.Focus();
            m_textboxInput.SelectAll();
            InputMethod.SetPreferredImeState(m_textboxInput, InputMethodState.On);
        }
        private string initExePath()
        {
            string path;

            Assembly myAssembly = Assembly.GetEntryAssembly();
            path = myAssembly.Location;
            return (path.Substring(0, path.LastIndexOf("\\")));
        }
        private String initEnvPath()
        {
            string sPath;

            sPath = m_sExecPath + "\\sekiji";
            m_libCmn.CreatePath(sPath);
            return (sPath);

        }
        private void cnvsMouseDown(object sender, MouseButtonEventArgs e)
        {
            Canvas cnvs;
            string str;

            cnvs = (Canvas)sender;
            str = cnvs.Name;
            moveDownSelectSeet(str);
        }
        private void cnvsMouseUp(object sender, MouseButtonEventArgs e)
        {
            Canvas cnvs;
            string str;

            m_bModify = true; 
            cnvs = (Canvas)sender;
            str = cnvs.Name;
            if (m_bMoveSeet == true)
            {
                moveUpSelectSeet(str);
            }
            else
            {
                setCanvasSelect(str);
            }
        }
        private void setSekijiElement(ClsSekiji clsSekiji, String sName, int nRType, int nHType)
        {
            Canvas cnvs;
            TextBlock tblk;

            clsSekiji.m_sName = sName;
            clsSekiji.m_nRyouriType = nRType;
            clsSekiji.m_nHikidemonoType = nHType;

            cnvs = clsSekiji.getNameCanvas();
            //tb.BorderBrush = m_brushNomal;
            //tb.BorderThickness = m_thicknessNomal;
            tblk = (TextBlock)cnvs.Children[0];
            tblk.Text = sName;
            tblk.Foreground = m_clsHaiseki.m_lstRyouri[nRType].getBrush();

            cnvs = clsSekiji.getHkdmnCanvas();
            tblk = (TextBlock)cnvs.Children[0];
            if (0 <= nHType)
            {
                cnvs.Background = m_clsHaiseki.m_lstHikidemono[nHType].getBrush();
                tblk.Text = m_clsHaiseki.m_lstHikidemono[nHType].m_sName;
            }
            else
            {
                cnvs.Background = null;
                tblk.Text = "";
            }
        }
        private void initCmbLayout()
        {
            cmbLayout.Items.Add("～２６名");
            cmbLayout.Items.Add("４０名");
            cmbLayout.Items.Add("４１名～６０名");
            cmbLayout.Items.Add("６１名～７８名");
            cmbLayout.Items.Add("７３名～１０４名");
            cmbLayout.Items.Add("９７名～１１２名");
            cmbLayout.Items.Add("１１３名～１２８名");
            cmbLayout.Items.Add("１２９名～１６８名");
        }
        private void cmbLayout_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            m_bModify = true;
            int idx = cmbLayout.SelectedIndex;
            string sSelect = cmbLayout.Items[idx].ToString();
            if (sSelect == "～２６名")
            {
                m_clsHaiseki.m_nTableBlockCount = 1;
                m_clsHaiseki.m_nSeetCount = 13;
            }
            if (sSelect == "４０名")
            {
                m_clsHaiseki.m_nTableBlockCount = 2;
                m_clsHaiseki.m_nSeetCount = 13;
            }
            else if (sSelect == "４１名～６０名")
            {
                m_clsHaiseki.m_nTableBlockCount = 3;
                m_clsHaiseki.m_nSeetCount = 10;
            }
            else if (sSelect == "６１名～７８名")
            {
                m_clsHaiseki.m_nTableBlockCount = 3;
                m_clsHaiseki.m_nSeetCount = 13;
            }
            else if (sSelect == "７３名～１０４名")
            {
                m_clsHaiseki.m_nTableBlockCount = 4;
                m_clsHaiseki.m_nSeetCount = 13;
            }
            else if (sSelect == "９７名～１１２名")
            {
                m_clsHaiseki.m_nTableBlockCount = 4;
                m_clsHaiseki.m_nSeetCount = 14;
            }
            else if (sSelect == "１１３名～１２８名")
            {
                m_clsHaiseki.m_nTableBlockCount = 4;
                m_clsHaiseki.m_nSeetCount = 16;
            }
            else if (sSelect == "１２９名～１６８名")
            {
                m_clsHaiseki.m_nTableBlockCount = 4;
                m_clsHaiseki.m_nSeetCount = 21;
            }
            drawTable();
        }
        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            saveDialogSekiji();
        }
        private void saveDialogSekiji()
        {
            string sSaveFileName;

            DateTime dt = DateTime.Now;
            sSaveFileName = dt.ToString("yyyyMMdd") + txtSouke.Text + ".xml";
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "保存ファイル(*.xml)|*.xml|すべてのファイル(*.*)|*.*||";
            sfd.Title = "保存ファイルを選択してください";
            sfd.InitialDirectory = m_sEnvPath;
            sfd.DefaultExt = "*.xml";
            sfd.FileName = sSaveFileName;
            if (sfd.ShowDialog() == true)
            {
                m_bModify = false;
                sSaveFileName = sfd.FileName;
                saveSekiji(sSaveFileName);
            }
        }
        private void btnLoad_Click(object sender, RoutedEventArgs e)
        {
            string sLoadFileName;
            string[] sLine;

            OpenFileDialog ifd = new OpenFileDialog();
            ifd.Filter = "読込みファイル(*.xml)|*.xml|すべてのファイル(*.*)|*.*||";
            ifd.Title = "読込みファイルを選択してください";
            ifd.InitialDirectory = m_sEnvPath;
            ifd.DefaultExt = "*.xml";
            if (ifd.ShowDialog() == true)
            {
                sLoadFileName = ifd.FileName;
                loadSekiji(sLoadFileName);

            }
        }

        private void btnItiran_Click(object sender, RoutedEventArgs e)
        {
            string sSaveFileName;

            DateTime dt = DateTime.Now;
            sSaveFileName = dt.ToString("yyyyMMdd") + txtSouke.Text + ".csv";
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "保存ファイル(*.csv)|*.csv|すべてのファイル(*.*)|*.*||";
            sfd.Title = "保存ファイルを選択してください";
            sfd.InitialDirectory = m_sItiranPath;
            sfd.DefaultExt = "*.csv";
            sfd.FileName = sSaveFileName;
            if (sfd.ShowDialog() == true)
            {
                sSaveFileName = sfd.FileName;
                m_sItiranPath = System.IO.Path.GetDirectoryName(sSaveFileName);
                saveItiran(sSaveFileName);
            }
        }

        private void btnSesyuPrint_Click(object sender, RoutedEventArgs e)
        {
            PreviewSesyuWindow previewSesyuWindow;

            if (m_clsHaiseki.m_nTableBlockCount == 0)
            {
                return;
            }
            resetTable();
            m_clsHaiseki.m_sTitle = txtTitle.Text;
            m_clsHaiseki.m_sSoukeName = txtSouke.Text;
            m_clsHaiseki.m_sReizenText = txtRTStr.Text;
            m_clsHaiseki.m_nSouryoCount = m_libCmn.StrToInt(txtTakasago.Text);
            m_clsHaiseki.m_lstRyouri[0].m_sName = txtRyouriName.Text;
            m_clsHaiseki.m_lstRyouri[6].m_sName = txtAddRyouri.Text;
            m_clsHaiseki.m_lstRyouri[7].m_sName = txtKuyouButu.Text;
            m_clsHaiseki.m_nYudemasu = m_libCmn.StrToInt(txtYudemasu.Text);
            m_clsHaiseki.m_sRightBottomText = txtRBStr.Text;
            previewSesyuWindow = new PreviewSesyuWindow();
            previewSesyuWindow.SetMainWindow(this);
            previewSesyuWindow.Owner = this;
            //calWin.m_dTopmost = true;
            //calWin.ShowDialog();
            previewSesyuWindow.ShowDialog();
        }
        private void btnSekijiPrint_Click(object sender, RoutedEventArgs e)
        {
            PreviewSekijiWindow previewSekijiWindow;

            if (m_clsHaiseki.m_nTableBlockCount == 0)
            {
                return;
            }
            resetTable();
            m_clsHaiseki.m_sTitle = txtTitle.Text;
            m_clsHaiseki.m_sSoukeName = txtSouke.Text;
            m_clsHaiseki.m_sReizenText = txtRTStr.Text;
            m_clsHaiseki.m_nSouryoCount = m_libCmn.StrToInt(txtTakasago.Text);
            m_clsHaiseki.m_lstRyouri[0].m_sName = txtRyouriName.Text;
            m_clsHaiseki.m_lstRyouri[6].m_sName = txtAddRyouri.Text;
            m_clsHaiseki.m_lstRyouri[7].m_sName = txtKuyouButu.Text;
            m_clsHaiseki.m_nYudemasu = m_libCmn.StrToInt(txtYudemasu.Text);
            m_clsHaiseki.m_sRightBottomText = txtRBStr.Text;
            previewSekijiWindow = new PreviewSekijiWindow();
            previewSekijiWindow.SetMainWindow(this);
            previewSekijiWindow.Owner = this;
            //calWin.m_dTopmost = true;
            //calWin.ShowDialog();
            previewSekijiWindow.ShowDialog();
        }
        private void btnReset_Click(object sender, RoutedEventArgs e)
        {
            resetTable();
            drawTable();
        }
        private void saveSekiji(string xmlfile)
        {
            Type type;

            resetTable();
            type = typeof(ClsHaiseki);
            m_libCmn.DataXmlSave(xmlfile, type, m_clsHaiseki);
        }
        private void loadSekiji(string xmlfile)
        {
            Type type;
            int tblblk, seet;

            m_clsHaiseki.clearListElement();
            type = typeof(ClsHaiseki);
            m_clsHaiseki = (ClsHaiseki)m_libCmn.DataXmlLoad(xmlfile, type);
            m_clsHaiseki.createBrush();
            if (m_clsHaiseki.m_nReizenAlignment == 1){
                rdoCenter.IsChecked = true;
            }else{
                rdoRight.IsChecked = true;
            }
            txtSouke.Text = m_clsHaiseki.m_sSoukeName;
            txtTitle.Text = m_clsHaiseki.m_sTitle;
            txtTakasago.Text = m_clsHaiseki.m_nSouryoCount.ToString();
            txtRTStr.Text = m_clsHaiseki.m_sReizenText;
            txtRBStr.Text = m_clsHaiseki.m_sRightBottomText;
            txtRyouriName.Text = m_clsHaiseki.m_lstRyouri[0].m_sName;
            txtAddRyouri.Text = m_clsHaiseki.m_lstRyouri[6].m_sName;
            txtKuyouButu.Text = m_clsHaiseki.m_lstRyouri[7].m_sName;
            txtYudemasu.Text = m_clsHaiseki.m_nYudemasu.ToString();
            tblblk = m_clsHaiseki.m_nTableBlockCount;
            seet = m_clsHaiseki.m_nSeetCount;
            if ((tblblk == 1) && (seet == 13))
            { // "～２６名"
                cmbLayout.SelectedIndex = 0;
            }
            else if ((tblblk == 2) && (seet == 13))
            { // "４０名"
                cmbLayout.SelectedIndex = 1;
            }
            else if ((tblblk == 3) && (seet == 10))
            { // "４１名～６０名"
                cmbLayout.SelectedIndex = 2;
            }
            else if ((tblblk == 3) && (seet == 13))
            { // "６１名～７８名")
                cmbLayout.SelectedIndex = 3;
            }
            else if ((tblblk == 4) && (seet == 13))
            { // "７３名～９６名"
                cmbLayout.SelectedIndex = 4;
            }
            else if ((tblblk == 4) && (seet == 14))
            { // "９７名～１１２名"
                cmbLayout.SelectedIndex = 5;
            }
            else if ((tblblk == 4) && (seet == 16))
            { //  "１１３名～１２８名"
                cmbLayout.SelectedIndex = 6;
            }
            else if ((tblblk == 4) && (seet == 21))
            { // "１２９名～１６８名"
                cmbLayout.SelectedIndex = 7;
            }
            drawTable();
        }
        private void saveItiran(string sFileName)
        {
            int tblblk, tblblkmax, tbl;
            int line, linemax;
            string sName, sKey, sSetStr;
            List<string> lstName;
            string[] ary;
            int max, idx;

            lstName = new List<string>();
            max = m_clsHaiseki.m_nSouryoCount;
            for (idx = 0; idx < max; idx++)
            {
                sName = m_clsHaiseki.m_lstSouryo[idx].m_sName;
                if (sName == "")
                {
                    continue;
                }
                ary = sName.Split('　');
                sKey = m_libOdbc.searchNameKwy(ary[0]);
                sSetStr = sKey + "," + sName;
                setListName(lstName, sSetStr);
            }
            tblblkmax = m_clsHaiseki.m_nTableBlockCount;
            linemax = m_clsHaiseki.m_nSeetCount;
            for (tblblk = 0; tblblk < tblblkmax; tblblk++)
            {
                tbl = tblblk * 2;
                for (line = 0; line < linemax; line++)
                {
                    sName = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_sName;
                    if(sName == ""){
                        continue;
                    }
                    ary = sName.Split('　');
                    sKey = m_libOdbc.searchNameKwy(ary[0]);
                    if (sKey == "")
                    {
                        sKey = "0,無";
                    }
                    sSetStr = sKey+","+sName;
                    setListName(lstName, sSetStr);
                }
                tbl = tblblk * 2+1;
                for (line = 0; line < linemax; line++)
                {
                    sName = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_sName;
                    if (sName == "")
                    {
                        continue;
                    }
                    ary = sName.Split('　');
                    sKey = m_libOdbc.searchNameKwy(ary[0]);
                    if (sKey == "")
                    {
                        sKey = "0,無";
                    }
                    sSetStr = sKey + "," + sName;
                    setListName(lstName, sSetStr);
                }
            }
            max = lstName.Count;
            sKey = "";
            sSetStr = "";
            for (idx = max-1; idx >= 0; idx--)
            {
                ary = lstName[idx].Split(',');

                if (sKey != ary[1].Substring(0, 1))
                {
                    sKey = ary[1].Substring(0, 1);
                    if (3 <= ary.Length)
                    {
                        sSetStr = sSetStr + sKey + "," + ary[2] + "\r\n";
                    }
                    else
                    {
                        sSetStr = sSetStr + sKey + "," + ary[1] + "\r\n";
                    }
                }
                else
                {
                    if (3 <= ary.Length)
                    {
                        sSetStr = sSetStr + "," + ary[2] + "\r\n";
                    }
                    else
                    {
                        sSetStr = sSetStr + "," + ary[1] + "\r\n";
                    }
                }
            }
            m_libCmn.SaveFileSJIS(sFileName, sSetStr);
        }
        private void setListName(List<string> lstName, string sSetStr)
        {
            int max, idx;
            string sStr;
            string[] ary;
            int nCrt, nSetNo;

            ary = sSetStr.Split(',');
            nSetNo = m_libCmn.StrToInt(ary[0]);
            max = lstName.Count;
            for (idx = 0; idx < max; idx++)
            {
                sStr = lstName[idx];
                ary = sStr.Split(',');
                nCrt = m_libCmn.StrToInt(ary[0]);
                if (nCrt < nSetNo)
                {
                    lstName.Insert(idx, sSetStr);
                    return;
                }
            }
            if (max == idx)
            {
                lstName.Add(sSetStr);
                return;
            }

        }
        private void cnvsHikimonoMouseDown(object sender, MouseButtonEventArgs e)
        {
            Canvas cnvs;
            string str;
            string[] ary;
            int hk;
            List<string> lstSyouhin;
            HikidemonoWindow hikidemonoWindow;

            cnvs = (Canvas)sender;
            str = cnvs.Name;
            ary = str.Split('_');
            hk = m_libCmn.StrToInt(ary[1]);
            lstSyouhin = m_clsHaiseki.m_lstHikidemono[hk].m_lstSyouhin;
            m_nCrtHikimonoIdx = hk;
            hikidemonoWindow = new HikidemonoWindow();
            hikidemonoWindow.SetMainWindow(this);
            hikidemonoWindow.setSyouhinList(lstSyouhin);
            hikidemonoWindow.Owner = this;
            //calWin.m_dTopmost = true;
            //calWin.ShowDialog();
            hikidemonoWindow.ShowDialog();
        }
        public void setCrtHikimonoList(List<String> lstSyouhin)
        {
            int max, idx;
            TextBlock tblk;
            int len;

            m_clsHaiseki.m_lstHikidemono[m_nCrtHikimonoIdx].m_lstSyouhin = lstSyouhin;
            drawTable();
            /*
            max = lstSyouhin.Count;
            for (idx = 0; idx < max; idx++)
            {
                tblk = m_clsHaiseki.m_lstHikidemono[m_nCrtHikimonoIdx].getTextBlock(idx);
                if (tblk != null)
                {
                    len = lstSyouhin[idx].Length;
                    if (9 < len)
                    {
                        tblk.Text = lstSyouhin[idx].Substring(0, 9);
                    }
                    else
                    {
                        tblk.Text = lstSyouhin[idx];
                    }
                }
            }
            */
        }
        private void resetTable()
        {
            int idx, max, tbl, tblmax;
            List<ClsSekiji> lstSekiji;
            TextBlock tblk;

            if (m_clsHaiseki.m_nTableBlockCount == 0)
            {
                return;
            }
            resetInputTextBox(m_nSelectTable, m_nSelectSeet);
            resetSelect();
            if (rdoCenter.IsChecked == true)
            {
                m_clsHaiseki.m_nReizenAlignment = 1;
            }
            else
            {
                m_clsHaiseki.m_nReizenAlignment = 0;
            }
            m_clsHaiseki.m_lstRyouri[0].m_sName = txtRyouriName.Text;
            m_clsHaiseki.m_lstRyouri[6].m_sName = txtAddRyouri.Text;
            m_clsHaiseki.m_lstRyouri[7].m_sName = txtKuyouButu.Text;
            m_clsHaiseki.m_nYudemasu = m_libCmn.StrToInt(txtYudemasu.Text);
            max = m_clsHaiseki.m_nSouryoCount;
            for (idx = 0; idx < max; idx++)
            {
                tblk = m_clsHaiseki.m_lstSouryo[idx].getNameTextBlock();
                if (tblk != null)
                {
                    m_clsHaiseki.m_lstSouryo[idx].m_sName = tblk.Text;
                }
            }
            tblmax = m_clsHaiseki.m_nTableBlockCount * 2;
            for (tbl = 0; tbl < tblmax; tbl++)
            {
                lstSekiji = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji;
                max = m_clsHaiseki.m_nSeetCount;
                for (idx = 0; idx < max; idx++)
                {
                    tblk = lstSekiji[idx].getNameTextBlock();
                    if (tblk != null)
                    {
                        lstSekiji[idx].m_sName = tblk.Text;
                    }
                }
            }
        }

        private void cmbSmsOutOne_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int idx;

            idx = cmbSmsOutOne.SelectedIndex;
            m_sSekiFudaWaku = cmbSmsOutOne.Items[idx].ToString();
        }

        private void cmbSmsOutAny_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int idx;
            string str;
            string[] ary;

            idx = cmbSmsOutAny.SelectedIndex;
            str = cmbSmsOutAny.Items[idx].ToString();
            ary = str.Split(',');
            m_sAnySekijiWaku = ary[0];
            m_nAnySekijiCount = m_libCmn.StrToInt(ary[1]);
        }
        private void btnSmsOutOne_Click(object sender, RoutedEventArgs e)
        {
            String sData;
            int tbl, tblmax;
            string sWakuFileName;
            string sHead;
            string[] ary;
            int idx, max;
            string sRecs;

            if (m_clsHaiseki.m_nTableBlockCount == 0)
            {
                return;
            }
            resetTable();

            sWakuFileName = m_sWakuBasePath + "\\" + m_sSekiFudaFolder + "\\" + m_sSekiFudaWaku;
            sHead = "\"未\",\"waku\"";
            sHead = sHead + ",\"" + m_sSekiFudaWaku + "\"";
            sHead = sHead + ",\"" + sWakuFileName + ".wak\"";
            sHead = sHead + ",\"1\"";

            sData = "";
            max = m_clsHaiseki.m_nSouryoCount;
            for (idx = 0; idx < max; idx++)
            {
                sData = sData + m_clsHaiseki.m_lstSouryo[idx].m_sName + ",";
            }
            tblmax = m_clsHaiseki.m_nTableBlockCount * 2;
            for (tbl = 0; tbl < tblmax; tbl++)
            {
                max = m_clsHaiseki.m_nSeetCount;
                for (idx = 0; idx < max; idx++)
                {
                    sData = sData + m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[idx].m_sName + ","; ;
                }
            }
            ary = sData.Split(',');
            max = ary.Length;
            sRecs = "";
            for(idx = 0; idx < max; idx++)
            {
                if (ary[idx] != "")
                {
                    sRecs = sRecs + sHead + ",\"/" + ary[idx] + "\",\r\n";
                }
            }
            saveOutCrct(sRecs);
            execSmsOut();
        }
        private void btnSmsOutAny_Click(object sender, RoutedEventArgs e)
        {
            String sData;
            int tbl, tblmax;
            string sWakuFileName;
            string sHead;
            string[] ary;
            string sRecs;
            int max, idx;

            if (m_clsHaiseki.m_nTableBlockCount == 0)
            {
                return;
            }
            resetTable();

            sWakuFileName = m_sWakuBasePath + "\\" + m_sAnySekijiFolder + "\\" + m_sAnySekijiWaku;
            sHead = "\"未\",\"waku\"";
            sHead = sHead + ",\"" + m_sAnySekijiWaku + "\"";
            sHead = sHead + ",\"" + sWakuFileName + ".wak\"";
            sHead = sHead + ",\"1\"";

            sData = "";
            max = m_clsHaiseki.m_nSouryoCount;
            for (idx = 0; idx < max; idx++)
            {
                sData = sData + m_clsHaiseki.m_lstSouryo[idx].m_sName + ",";
            }
            tblmax = m_clsHaiseki.m_nTableBlockCount * 2;
            for (tbl = 0; tbl < tblmax; tbl++)
            {
                max = m_clsHaiseki.m_nSeetCount;
                for (idx = 0; idx < max; idx++)
                {
                    sData = sData + m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[idx].m_sName + ","; ;
                }
            }
            ary = sData.Split(',');
            sRecs = "";
            sRecs = setAnyFudaRecords(sRecs, sHead, ary);
            saveOutCrct(sRecs);
            execSmsOut();
        }
        private string setAnyFudaRecords(string sRecs, string sHead, string[] ary)
        {
            int idx, max, set;

            max = ary.Length;
            set = 0;
            for (idx = 0; idx < max; idx++)
            {
                if (ary[idx] != "")
                {
                    if(set == 0){
                        sRecs = sRecs+sHead + ",\"/";
                    }
                    sRecs = sRecs + ary[idx] + "/";
                    set++;
                    if (m_nAnySekijiCount <= set)
                    {
                        sRecs = sRecs + "\"\r\n";
                        set = 0;
                    }
                }
            }
            if (set != 0)
            {
                for (; set < m_nAnySekijiCount; set++)
                {
                    sRecs = sRecs + "/";
                }
                sRecs = sRecs + "\"\r\n";
            }
            return (sRecs);
        }
        private void saveSekijiEnv()
        {
            string sData;
            string sFileName;

            sData = "//\r\n";
            sData = sData + m_sFontFamilyNameCrt + "\r\n";
            sData = sData + m_sItiranPath + "\r\n";
            sFileName = m_sEnvPath + "\\sekiji.txt";
            m_libCmn.SaveFileSJIS(sFileName, sData);
        }
        private void loadSekijiEnv()
        {
            string sFileName;
            string[] aryLine;
            int idx, max;
            string[] ary;
            string str;

            sFileName = m_sExecPath + "\\ses_sys.env";
            if (!(System.IO.File.Exists(sFileName)))
            {
                MessageBox.Show(sFileName + "ファイルが見つかりません", "確認", MessageBoxButton.OK);
            }
            else
            {
                aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
                if (12 <= aryLine.Length)
                {
                    m_sWakuBasePath = aryLine[11];
                }
            }
            sFileName = m_sEnvPath + "\\sekiji.txt";
            if (!(System.IO.File.Exists(sFileName)))
            {
                m_sFontFamilyNameCrt = "メイリオ";
                m_sItiranPath = m_sEnvPath;
            }
            else
            {
                aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
                m_sFontFamilyNameCrt = aryLine[1];
                m_sItiranPath = aryLine[2];

            }
            sFileName = m_sEnvPath + "\\smsoutone.txt";
            if (!(System.IO.File.Exists(sFileName)))
            {
                m_sSekiFudaFolder = "席札";
                m_sSekiFudaWaku = "席札";
                cmbSmsOutOne.Items.Add(m_sSekiFudaWaku);
                cmbSmsOutOne.SelectedIndex = 0;
            }
            else
            {
                aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
                ary = aryLine[0].Split(',');
                m_sSekiFudaFolder = ary[1];
                max = aryLine.Length;
                for (idx = 1; idx < max; idx++)
                {
                    if (aryLine[idx] != "")
                    {
                        cmbSmsOutOne.Items.Add(aryLine[idx]);
                    }
                }
                cmbSmsOutOne.SelectedIndex = 0;
                m_sSekiFudaWaku = aryLine[1];
            }
            sFileName = m_sEnvPath + "\\smsoutany.txt";
            if (!(System.IO.File.Exists(sFileName)))
            {
                m_sAnySekijiFolder = "席札";
                m_sAnySekijiWaku = "並列";
                m_nAnySekijiCount = 10;
                str = m_sAnySekijiWaku + "," + m_nAnySekijiCount;
                cmbSmsOutAny.Items.Add(str);
                cmbSmsOutAny.SelectedIndex = 0;
            }
            else
            {
                aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
                ary = aryLine[0].Split(',');
                m_sAnySekijiFolder = ary[1];
                max = aryLine.Length;
                for (idx = 1; idx < max; idx++)
                {
                    if (aryLine[idx] != "")
                    {
                        cmbSmsOutAny.Items.Add(aryLine[idx]);
                    }
                }
                cmbSmsOutAny.SelectedIndex = 0;
                ary = aryLine[1].Split(',');
                m_sAnySekijiWaku = ary[0];
                m_nAnySekijiCount = m_libCmn.StrToInt(ary[1]);
            }
        }
        private void saveOutCrct(string sData)
        {
            string sFileName;
            StreamWriter stream;
            Encoding encode;

            sFileName = m_sExecPath + "\\outcrect.dat";
            encode = System.Text.Encoding.GetEncoding("shift_jis");
            stream = new StreamWriter(sFileName, false, encode);
            stream.Write(sData);
            stream.Close();
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

        private void btnTool_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] aryLine;
            string msg;

            m_libOdbc.createNameTable();
            sFileName = m_sExecPath + "\\name1.csv";
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initNameTable(aryLine);
            sFileName = m_sExecPath + "\\name2.csv";
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_libOdbc.initNameTable(aryLine);
            msg = "nameテーブルを作成しました";
            MessageBox.Show(msg, "確認", MessageBoxButton.OK);
        }
    }
}
