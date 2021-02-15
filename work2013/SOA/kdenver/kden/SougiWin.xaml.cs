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
    /// <summary>
    /// SougiWin.xaml の相互作用ロジック
    /// </summary>
    public partial class SougiWin : Window
    {
        private MainWindow m_wnd;
        private string m_sMsg;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibCanvas m_libCanvas;
        public LibOdbc m_libOdbc;
        private string m_sRecs;
        private string[] m_aryRecLine;
        private int m_nSelectWinSW;
        private int m_nCrtSougiId;
        private string m_sSGSeikyuno;
        private string m_sKokubetu;

        public SougiWin()
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
        }
        public void SetRecsCsv(string sRecs)
        {
            m_sRecs = sRecs;
        }
        public void SetRecsElement(string sDatas)
        {
            m_aryRecLine = sDatas.Split('\n');
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string[] sAry;
            int nId;
            string sFilds;

            sAry = m_sRecs.Split(',');
            nId = m_libCmn.StrToInt(sAry[0]);
            sFilds = m_libOdbc.getHanbaiRecord(nId);
            sAry = sFilds.Split(',');
            m_nCrtSougiId = m_libCmn.StrToInt(sAry[4]);
            if (m_nCrtSougiId != 0)
            {
                setSougiElement(m_nCrtSougiId);
            }
            else
            {
                setHanbaiSougiElement(sAry);
            }
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            saveSougiTable();
            saveHanbaiTable();
        }
        private void btnRet_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void setSougiElement(int nId)
        {
            string sStr;
            string[] sAry;
            string sWareki;

            sStr = m_libOdbc.getSougiRecord(nId);
            sAry = sStr.Split(',');
            m_sKokubetu = sAry[7];
            m_sSGSeikyuno = sAry[2];
            txtSouke.Text = sAry[3];
            txtTuyaYY.Text = sAry[4].Substring(0, 4);
            txtTuyaMM.Text = sAry[4].Substring(5, 2);
            txtTuyaDD.Text = sAry[4].Substring(8, 2);
            txtSTuyaHH.Text = sAry[5].Substring(0, 2);
            txtSTuyaMin.Text = sAry[5].Substring(3, 2);
            txtETuyaHH.Text = sAry[6].Substring(0, 2); ;
            txtETuyaMin.Text = sAry[6].Substring(3, 2);

            sWareki = m_libCmn.convWakeki(sAry[7]);
            txtSikiGMD.Text = sWareki;
            txtSSikiHH.Text = sAry[8].Substring(0, 2);
            txtSSikiMin.Text = sAry[8].Substring(3, 2);
            txtESikiHH.Text = sAry[9].Substring(0, 2);
            txtESikiMin.Text = sAry[9].Substring(3, 2);

            txtSekoubasyo.Text = sAry[10];
            txtTantou.Text = sAry[11];

        }
        private void setHanbaiSougiElement(string[] sAry)
        {
            string sYY, sMM, sDD;
            int yy, mm, dd;
            string sDate;

            m_sSGSeikyuno = "";
            txtSouke.Text = sAry[5];
            m_sKokubetu = sAry[6];
            if (m_sKokubetu != "")
            {
                sYY = m_sKokubetu.Substring(0, 4);
                sMM = m_sKokubetu.Substring(5, 2);
                sDD = m_sKokubetu.Substring(8, 2);

                sDate = m_libCmn.convWakeki(m_sKokubetu);
                txtSikiGMD.Text = sDate;
                txtSSikiHH.Text = "10";
                txtSSikiMin.Text = "30";
                txtESikiHH.Text = "11";
                txtESikiMin.Text = "30";

                yy = m_libCmn.StrToInt(sYY);
                mm = m_libCmn.StrToInt(sMM);
                dd = m_libCmn.StrToInt(sDD);
                DateTime dt = new DateTime(yy, mm, dd);
                dt = dt.AddDays(-1);
                sDate = dt.ToString("yyyy/MM/dd");
                txtTuyaYY.Text = sDate.Substring(0, 4);
                txtTuyaMM.Text = sDate.Substring(5, 2);
                txtTuyaDD.Text = sDate.Substring(8, 2);
                txtSTuyaHH.Text = "18";
                txtSTuyaMin.Text = "30";
                txtETuyaHH.Text = "19";
                txtETuyaMin.Text = "30";
            }

        }
        private void btnTuyaMM_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            List<string> listLine;

            m_nSelectWinSW = 1;
            listLine = new List<string>();
            sFileName = m_sExecPath + "\\kden\\month.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    listLine.Add(sLine[idx]);
                }
            }
            showMinSlctWin(listLine, btnTuyaMM);
        }

        private void btnTuyaDD_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            List<string> listLine;

            m_nSelectWinSW = 2;
            listLine = new List<string>();
            sFileName = m_sExecPath + "\\kden\\day.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    listLine.Add(sLine[idx]);
                }
            }
            showMinSlctWin(listLine, btnTuyaDD);
        }

        private void btnSTuyaHH_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            List<string> listLine;

            m_nSelectWinSW = 3;
            listLine = new List<string>();
            sFileName = m_sExecPath + "\\kden\\hour.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    listLine.Add(sLine[idx]);
                }
            }
            showMinSlctWin(listLine, btnSTuyaHH);
        }

        private void btnSTuyaMin_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            List<string> listLine;

            m_nSelectWinSW = 4;
            listLine = new List<string>();
            sFileName = m_sExecPath + "\\kden\\minute.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    listLine.Add(sLine[idx]);
                }
            }
            showMinSlctWin(listLine, btnSTuyaMin);
        }

        private void btnETuyaHH_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            List<string> listLine;

            m_nSelectWinSW = 5;
            listLine = new List<string>();
            sFileName = m_sExecPath + "\\kden\\hour.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    listLine.Add(sLine[idx]);
                }
            }
            showMinSlctWin(listLine, btnETuyaHH);
        }

        private void btnETuyaMin_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            List<string> listLine;

            m_nSelectWinSW = 6;
            listLine = new List<string>();
            sFileName = m_sExecPath + "\\kden\\minute.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    listLine.Add(sLine[idx]);
                }
            }
            showMinSlctWin(listLine, btnETuyaMin);
        }

        private void btnSSikiHH_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            List<string> listLine;

            m_nSelectWinSW = 7;
            listLine = new List<string>();
            sFileName = m_sExecPath + "\\kden\\hour.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    listLine.Add(sLine[idx]);
                }
            }
            showMinSlctWin(listLine, btnSSikiHH);
        }

        private void btnSSikiMin_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            List<string> listLine;

            m_nSelectWinSW = 8;
            listLine = new List<string>();
            sFileName = m_sExecPath + "\\kden\\minute.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    listLine.Add(sLine[idx]);
                }
            }
            showMinSlctWin(listLine, btnSSikiMin);
        }

        private void btnESikiHH_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            List<string> listLine;

            m_nSelectWinSW = 9;
            listLine = new List<string>();
            sFileName = m_sExecPath + "\\kden\\hour.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    listLine.Add(sLine[idx]);
                }
            }
            showMinSlctWin(listLine, btnESikiHH);
        }

        private void btnESikiMin_Click(object sender, RoutedEventArgs e)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            List<string> listLine;

            m_nSelectWinSW = 10;
            listLine = new List<string>();
            sFileName = m_sExecPath + "\\kden\\minute.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    listLine.Add(sLine[idx]);
                }
            }
            showMinSlctWin(listLine, btnESikiMin);
        }
        private void showMinSlctWin(List<string> listSelect, Control control)
        {
            MinSlctWin minSlctWin;

            minSlctWin = new MinSlctWin();
            minSlctWin.SetSougiWin(this);
            minSlctWin.SetSelectList(listSelect);
            Point pt = control.PointToScreen(new Point(0.0d, 0.0d));
            minSlctWin.Left = pt.X;
            minSlctWin.Top = pt.Y;
            minSlctWin.Owner = this;
            minSlctWin.ShowDialog();
        }
        public void setTextSelectWin(string sStr)
        {
            switch (m_nSelectWinSW)
            {
                case 1:
                    txtTuyaMM.Text = sStr;
                    break;
                case 2:
                    txtTuyaDD.Text = sStr;
                    break;
                case 3:
                    txtSTuyaHH.Text = sStr;
                    break;
                case 4:
                    txtSTuyaMin.Text = sStr;
                    break;
                case 5:
                    txtETuyaHH.Text = sStr;
                    break;
                case 6:
                    txtETuyaMin.Text = sStr;
                    break;
                case 7:
                    txtSSikiHH.Text = sStr;
                    break;
                case 8:
                    txtSSikiMin.Text = sStr;
                    break;
                case 9:
                    txtESikiHH.Text = sStr;
                    break;
                case 10:
                    txtESikiMin.Text = sStr;
                    break;
            }
        }
        private void saveSougiTable()
        {
            string sData;
            int yy, mm, dd, hh, min;
            string sYMD;
            string sSHM, sEHM;

            DateTime dt = DateTime.Now;

            sData = m_nCrtSougiId + "," + dt.ToString("yyyy/MM/dd hh:mm");
            sData = sData + "," + m_sSGSeikyuno + "," + txtSouke.Text;
            yy = m_libCmn.StrToInt(txtTuyaYY.Text);
            mm = m_libCmn.StrToInt(txtTuyaMM.Text);
            dd = m_libCmn.StrToInt(txtTuyaDD.Text);
            sYMD = yy.ToString("D4") +"/"+ mm.ToString("D2") + "/" + dd.ToString("D2");
            hh = m_libCmn.StrToInt(txtSTuyaHH.Text);
            min = m_libCmn.StrToInt(txtSTuyaMin.Text);
            sSHM =  hh.ToString("D2") + ":" + min.ToString("D2");
            hh = m_libCmn.StrToInt(txtETuyaHH.Text);
            min = m_libCmn.StrToInt(txtETuyaMin.Text);
            sEHM =  hh.ToString("D2") + ":" + min.ToString("D2");
            sData = sData + "," + sYMD + "," + sSHM + "," + sEHM;

            hh = m_libCmn.StrToInt(txtSSikiHH.Text);
            min = m_libCmn.StrToInt(txtSSikiMin.Text);
            sSHM =  hh.ToString("D2") + ":" + min.ToString("D2");
            hh = m_libCmn.StrToInt(txtESikiHH.Text);
            min = m_libCmn.StrToInt(txtESikiMin.Text);
            sEHM =  hh.ToString("D2") + ":" + min.ToString("D2");
            sData = sData + "," + m_sKokubetu + "," + sSHM + "," + sEHM;

            sData = sData + "," + txtSekoubasyo.Text + "," + txtTantou.Text + ",,";
            if (m_nCrtSougiId == 0)
            {
                m_nCrtSougiId = m_libOdbc.insertSougi(sData);
            }
            else
            {
                m_libOdbc.updateSougi(sData);
            }
        }
        private void saveHanbaiTable()
        {
            int max, idx;
            string[] sAry;

            max = m_aryRecLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                sAry = m_aryRecLine[idx].Split(',');
                if (4 < sAry.Length)
                {
                    sAry[4] = m_nCrtSougiId.ToString();
                    m_aryRecLine[idx] = m_libCmn.AryToCsvString(sAry);
                    m_libOdbc.updateHanbai(m_aryRecLine[idx]);
                }
            }
        }
        private void btnPreView_Click(object sender, RoutedEventArgs e)
        {
            MeisaiPreviewWin meisaiPreviewWin;
            string sSikiSHMEHM;

            saveSougiTable();
            saveHanbaiTable();

            sSikiSHMEHM = txtSSikiHH.Text + ":" + txtSSikiMin.Text;
            sSikiSHMEHM = sSikiSHMEHM + " ～ " + txtESikiHH.Text + ":" + txtESikiMin.Text;
            meisaiPreviewWin = new MeisaiPreviewWin();
            meisaiPreviewWin.setSougiWin(this);
            meisaiPreviewWin.setSoukeName(txtSouke.Text);
            meisaiPreviewWin.setSikiGMD(txtSikiGMD.Text);
            meisaiPreviewWin.setSikiSHMEHM(sSikiSHMEHM);
            meisaiPreviewWin.setAryRecElement(m_aryRecLine);
            Point pt = m_wnd.PointToScreen(new Point(0.0d, 0.0d));
            meisaiPreviewWin.Left = pt.X;
            meisaiPreviewWin.Top = pt.Y;
            meisaiPreviewWin.Owner = this;
            meisaiPreviewWin.ShowDialog();
            this.Close();
        }
    }
}
