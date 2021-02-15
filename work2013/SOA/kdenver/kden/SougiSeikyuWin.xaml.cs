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
    /// SougiSeikyuWin.xaml の相互作用ロジック
    /// </summary>
    public partial class SougiSeikyuWin : Window
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
        private string m_sKokubetu;

        private int m_nCrtSougiId;
        private string m_sSGSeikyuno;

        private int m_nCrtSeikyuId;

        private int m_nCrtSeikyusakiId;
        private string m_sSeikyusakiFilds;

        private int m_nCrtSasidasininId;
        private string m_sSasidasininFilds;

        private int m_nCrtFurikomisakiId;
        private string m_sFurikomisakiFilds;

        public SougiSeikyuWin()
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
        public void setSeikyusaki(string sFilds)
        {
            string[] ary;
            if (sFilds == "")
            {
                m_nCrtSeikyusakiId = 0;
                m_sSeikyusakiFilds = "";
                txtSeikyuusaki.Text = "";
                return;
            }
            ary = sFilds.Split(',');
            m_nCrtSeikyusakiId = m_libCmn.StrToInt(ary[0]);
            m_sSeikyusakiFilds = sFilds;
            txtSeikyuusaki.Text = ary[2] + " " + ary[11];
        }
        public void setSasidasinin(string sFilds)
        {
            string[] ary;
            if (sFilds == "")
            {
                m_nCrtSasidasininId = 0;
                m_sSasidasininFilds = "";
                txtSasidasinin.Text = "";
                return;
            }
            ary = sFilds.Split(',');
            m_nCrtSasidasininId = m_libCmn.StrToInt(ary[0]);
            m_sSasidasininFilds = sFilds;
            txtSasidasinin.Text = ary[1] + " " + ary[2];
        }
        public void setFurikomisaki(string sFilds)
        {
            string[] ary;
            if (sFilds == "")
            {
                m_nCrtFurikomisakiId = 0;
                m_sFurikomisakiFilds = "";
                txtFurikomisaki.Text = "";
                return;
            }
            ary = sFilds.Split(',');
            m_nCrtFurikomisakiId = m_libCmn.StrToInt(ary[0]);
            m_sFurikomisakiFilds = sFilds;
            ary = m_sFurikomisakiFilds.Split(',');
            txtFurikomisaki.Text = ary[1] + " " + ary[2] + " " + ary[3] + " No." + ary[4] + " " + ary[5] + " " + ary[6];
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string[] sAry;
            int nId;
            string sHanbaiFilds;

            m_sSGSeikyuno = "";
            if (m_sRecs == "")
            {
                return;
            }
            sAry = m_sRecs.Split(',');
            nId = m_libCmn.StrToInt(sAry[0]);
            sHanbaiFilds = m_libOdbc.getHanbaiRecord(nId);
            sAry = sHanbaiFilds.Split(',');

            m_nCrtSougiId = m_libCmn.StrToInt(sAry[4]);
            if (m_nCrtSougiId != 0)
            {
                setSougiElement(m_nCrtSougiId);
            }
            else
            {
                setHanbaiSougiElement(sAry);
            }
            setHanbaiSeikyuElement(sAry);
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            saveSougiSeikyuHanbai();
        }
        private void btnRet_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void setSougiElement(int nId)
        {
            string sStr;
            string[] ary; 
            string sWareki;

            sStr = m_libOdbc.getSougiRecord(nId);
            if (sStr != null && sStr == "")
            {
                return;
            }
            ary = sStr.Split(',');
            m_sKokubetu = ary[7];
            m_sSGSeikyuno = ary[2];
            txtSouke.Text = ary[3];
            txtTuyaYY.Text = ary[4].Substring(0, 4);
            txtTuyaMM.Text = ary[4].Substring(5, 2);
            txtTuyaDD.Text = ary[4].Substring(8, 2);
            txtSTuyaHH.Text = ary[5].Substring(0, 2);
            txtSTuyaMin.Text = ary[5].Substring(3, 2);
            txtETuyaHH.Text = ary[6].Substring(0, 2); ;
            txtETuyaMin.Text = ary[6].Substring(3, 2);

            sWareki = m_libCmn.convWakeki(ary[7]);
            txtSikiGMD.Text = sWareki;
            txtSSikiHH.Text = ary[8].Substring(0, 2);
            txtSSikiMin.Text = ary[8].Substring(3, 2);
            txtESikiHH.Text = ary[9].Substring(0, 2);
            txtESikiMin.Text = ary[9].Substring(3, 2);

            txtSekoubasyo.Text = ary[10];
            txtTantou.Text = ary[11];
        }
        private void setHanbaiSougiElement(string[] sAry)
        {
            string sYY, sMM, sDD;
            string sDate;
            int yy, mm, dd;

            txtSouke.Text = sAry[5];
            if (sAry[6] == "")
            {
                return;
            }
            m_sKokubetu = sAry[6];
            sYY = sAry[6].Substring(0, 4);
            sMM = sAry[6].Substring(5, 2);
            sDD = sAry[6].Substring(8, 2);

            sDate = m_libCmn.convWakeki(sAry[6]);
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
        private string getSougiFilds()
        {
            string sSougi;

            if (m_sSGSeikyuno == "")
            {
                m_sSGSeikyuno = txtBangou.Text;
            }
            DateTime dt = DateTime.Now;
            string sYMDHM = dt.ToString("yyyy/MM/dd HH:mm");
            sSougi = m_nCrtSougiId + "," + sYMDHM;
            sSougi = sSougi + "," + m_sSGSeikyuno + "," + txtSouke.Text;
            sSougi = sSougi+"," + txtTuyaYY.Text+"/"+txtTuyaMM.Text+"/"+txtTuyaDD.Text;
            sSougi = sSougi+"," + txtSTuyaHH.Text+":"+txtSTuyaMin.Text;
            sSougi = sSougi+"," + txtETuyaHH.Text+":"+txtETuyaMin.Text;
            sSougi = sSougi+"," + m_sKokubetu;
            sSougi = sSougi+"," + txtSSikiHH.Text+":"+txtSSikiMin.Text;
            sSougi = sSougi+"," + txtESikiHH.Text+":"+txtESikiMin.Text;
            sSougi = sSougi+"," + txtSekoubasyo.Text+","+txtTantou.Text+",";
            return (sSougi);
        }
        private void setHanbaiSeikyuElement(string[] sAry)
        {
            int nSeikyuId;
            string sSeikyuFilds;
            string[] ary;

            nSeikyuId = m_libCmn.StrToInt(sAry[3]);
            if (nSeikyuId == 0)
            {
                m_nCrtSasidasininId = 1;
                m_nCrtFurikomisakiId = 1;

            }
            else
            {
                sSeikyuFilds = m_libOdbc.getSeikyuRecord(nSeikyuId);
                ary = sSeikyuFilds.Split(',');
                m_nCrtSasidasininId = m_libCmn.StrToInt(ary[4]);
                m_nCrtFurikomisakiId = m_libCmn.StrToInt(ary[5]);
            }
            txtBangou.Text = m_sSGSeikyuno;
            txtBunsyou1.Text = loadFileBunsyou1();
            txtBunsyou2.Text = loadFileBunsyou2();

            txtSeikyuusaki.Text = sAry[14] + " " + sAry[15] + " " + sAry[23];

            m_sSasidasininFilds = m_libOdbc.getSasidasininRecord(m_nCrtSasidasininId);
            if (m_sSasidasininFilds != null && m_sSasidasininFilds != "")
            {
                ary = m_sSasidasininFilds.Split(',');
                txtSasidasinin.Text = ary[1] + " " + ary[2];
            }
            else
            {
                m_sSasidasininFilds = "";
            }


            m_sFurikomisakiFilds = m_libOdbc.getFurikomisakiRecord(m_nCrtFurikomisakiId);
            if (m_sFurikomisakiFilds != null && m_sFurikomisakiFilds != "")
            {
                ary = m_sFurikomisakiFilds.Split(',');
                txtFurikomisaki.Text = ary[1] + " " + ary[2] + " " + ary[3] + " No." + ary[4] + " " + ary[5] + " " + ary[6];
            }
            else
            {
                m_sFurikomisakiFilds = "";
            }

        }
        private string getSeikyuFilds()
        {
            int max, idx;
            string[] sAry;
            int nSumTax, nSumPrice;
            string sSeikyu;

            nSumTax = 0;
            nSumPrice = 0;
            max = m_aryRecLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                sAry = m_aryRecLine[idx].Split(',');
                nSumTax = nSumTax + m_libCmn.StrToInt(sAry[12]);
                nSumPrice = nSumPrice + m_libCmn.StrToInt(sAry[13]);
            }

            DateTime dt = DateTime.Now;
            string sYMDHM = dt.ToString("yyyy/MM/dd HH:mm");
            sSeikyu = m_nCrtSeikyuId +"," + sYMDHM + "," + txtBangou.Text;
            sSeikyu = sSeikyu + "," + m_nCrtSeikyusakiId + "," + m_nCrtSasidasininId;
            sSeikyu = sSeikyu + "," + m_nCrtFurikomisakiId;
            sSeikyu = sSeikyu + ",0," + nSumTax + "," + nSumPrice+",,,";
            return (sSeikyu);
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
        private void btnSeikyusaki_Click(object sender, RoutedEventArgs e)
        {
            SelectRecWin selectRecWin;

            selectRecWin = new SelectRecWin();
            selectRecWin.SetSougiSeikyuWin(this);
            selectRecWin.SetSelectTable(1);
            Point pt = this.PointToScreen(new Point(0.0d, 0.0d));
            selectRecWin.Left = pt.X;
            selectRecWin.Top = pt.Y;
            selectRecWin.Owner = this;
            selectRecWin.ShowDialog();
        }
        private void btnSasidasinin_Click(object sender, RoutedEventArgs e)
        {
            SelectRecWin selectRecWin;

            selectRecWin = new SelectRecWin();
            selectRecWin.SetSougiSeikyuWin(this);
            selectRecWin.SetSelectTable(2);
            Point pt = this.PointToScreen(new Point(0.0d, 0.0d));
            selectRecWin.Left = pt.X;
            selectRecWin.Top = pt.Y;
            selectRecWin.Owner = this;
            selectRecWin.ShowDialog();
        }
        private void btnFurikomisaki_Click(object sender, RoutedEventArgs e)
        {
            SelectRecWin selectRecWin;

            selectRecWin = new SelectRecWin();
            selectRecWin.SetSougiSeikyuWin(this);
            selectRecWin.SetSelectTable(3);
            Point pt = this.PointToScreen(new Point(0.0d, 0.0d));
            selectRecWin.Left = pt.X;
            selectRecWin.Top = pt.Y;
            selectRecWin.Owner = this;
            selectRecWin.ShowDialog();
        }
        private void saveSougiSeikyuHanbai()
        {
            string sFilds;

            sFilds = getSougiFilds();
            if (m_nCrtSougiId == 0)
            {
                m_nCrtSougiId = m_libOdbc.insertSougi(sFilds);
            }
            else
            {
                m_libOdbc.updateSougi(sFilds);
            }
            sFilds = getSeikyuFilds();
            if (m_nCrtSeikyuId == 0)
            {
                m_nCrtSeikyuId = m_libOdbc.insertSeikyu(sFilds);
            }
            else
            {
                m_libOdbc.updateSeikyu(sFilds);
            }
            saveHanbaiTable();
        }
        private void btnSeikyusyoOut_Click(object sender, RoutedEventArgs e)
        {
            SeikyuPreviewWin seikyuPreviewWin;
            string sFilds;

            saveSougiSeikyuHanbai();

            seikyuPreviewWin = new SeikyuPreviewWin();
            seikyuPreviewWin.setSougiSeikyuWin(this);
            seikyuPreviewWin.setBunsyou1(txtBunsyou1.Text);
            seikyuPreviewWin.setBunsyou2(txtBunsyou2.Text);
            sFilds = getSougiFilds();
            seikyuPreviewWin.setSougi(sFilds);
            sFilds = getSeikyuFilds();
            seikyuPreviewWin.setSeikyu(sFilds);
            seikyuPreviewWin.setSeikyusaki(m_sSeikyusakiFilds);
            seikyuPreviewWin.setSasidasinin(m_sSasidasininFilds);
            seikyuPreviewWin.setFurikomisaki(m_sFurikomisakiFilds);
            seikyuPreviewWin.setAryRecElement(m_aryRecLine);
            Point pt = m_wnd.PointToScreen(new Point(0.0d, 0.0d));
            seikyuPreviewWin.Left = pt.X;
            seikyuPreviewWin.Top = pt.Y;
            seikyuPreviewWin.Owner = this;
            seikyuPreviewWin.ShowDialog();
        }
        private void btnFuutouOut_Click(object sender, RoutedEventArgs e)
        {
            FuutouPreviewWin fuutouPreviewWin;

            saveSougiSeikyuHanbai();

            fuutouPreviewWin = new FuutouPreviewWin();
            fuutouPreviewWin.setSougiSeikyuWin(this);
            fuutouPreviewWin.setSeikyusaki(m_sSeikyusakiFilds);
            Point pt = m_wnd.PointToScreen(new Point(0.0d, 0.0d));
            fuutouPreviewWin.Left = pt.X;
            fuutouPreviewWin.Top = pt.Y;
            fuutouPreviewWin.Owner = this;
            fuutouPreviewWin.ShowDialog();
        }
        private void showMinSlctWin(List<string> listSelect, Control control)
        {
            MinSlctWin minSlctWin;

            minSlctWin = new MinSlctWin();
            minSlctWin.SetSougiSeikyuWin(this);
            minSlctWin.SetSelectList(listSelect);
            Point pt = control.PointToScreen(new Point(0.0d, 0.0d));
            minSlctWin.Left = pt.X;
            minSlctWin.Top = pt.Y;
            minSlctWin.Owner = this;
            minSlctWin.ShowDialog();
        }
        private void saveHanbaiTable()
        {
            string sTmp;
            string[] sSKAry;
            int max, idx;
            string[] sAry;

            if (m_sSeikyusakiFilds != null && m_sSeikyusakiFilds != "")
            {
                sSKAry = m_sSeikyusakiFilds.Split(',');
            }
            else
            {
                sTmp = "";
                sSKAry = sTmp.Split(',');
            }
            max = m_aryRecLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                sAry = m_aryRecLine[idx].Split(',');
                if (4 < sAry.Length)
                {
                    sAry[3] = m_nCrtSeikyuId.ToString();
                    sAry[4] = m_nCrtSougiId.ToString();
                    if (12 < sSKAry.Length)
                    {
                        sAry[14] = sSKAry[2];
                        sAry[15] = sSKAry[3];
                        sAry[16] = sSKAry[4];
                        sAry[17] = sSKAry[5];
                        sAry[18] = sSKAry[6];
                        sAry[19] = sSKAry[7];
                        sAry[20] = sSKAry[8];
                        sAry[21] = sSKAry[9];
                        sAry[22] = sSKAry[10];
                        sAry[23] = sSKAry[11];
                    }
                    m_aryRecLine[idx] = m_libCmn.AryToCsvString(sAry);
                    m_libOdbc.updateHanbai(m_aryRecLine[idx]);
                }
            }
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
        private string loadFileBunsyou1()
        {
            string sFileName;
            string[] sLine;
            string sStr;
            int max, idx;

            sFileName = m_sExecPath + "\\kden\\bunsyou1.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            sStr = "";
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    if (idx != 1)
                    {
                        sStr = sStr + "\n";
                    }
                    sStr = sStr + sLine[idx];
                }
            }
            return (sStr);
        }
        private string loadFileBunsyou2()
        {
            string sFileName;
            string[] sLine;
            string sStr;
            int max, idx;

            sFileName = m_sExecPath + "\\kden\\bunsyou2.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            sStr = "";
            for (idx = 1; idx < max; idx++)
            {
                if (sLine[idx] != "")
                {
                    if (idx != 1)
                    {
                        sStr = sStr + "\n";
                    }
                    sStr = sStr + sLine[idx];
                }
            }
            return (sStr);
        }
        private string initLoadSasidasinin()
        {
            string sFileName;
            string[] sLine;
            string sStr;
            int max, idx;

            sFileName = m_sExecPath + "\\kden\\sasidasinin.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            sStr = "";
            for (idx = 1; idx < max; idx++)
            {
                sStr = sStr + sLine[idx] + " ";
            }
            return (sStr);
        }
        private string initLoadFurikomisaki()
        {
            string sFileName;
            string[] sLine;
            string sStr;
            int max, idx;

            sFileName = m_sExecPath + "\\kden\\furikomisaki.txt";
            sLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            sStr = "";
            for (idx = 1; idx < max; idx++)
            {
                sStr = sStr + sLine[idx] + " ";
            }
            return (sStr);
        }
    }
}
