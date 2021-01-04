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
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace CenterList
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        private string m_sExecPath;
        private string m_sEnvPath;
        private string m_sMdbPath;
        private LibCommon m_libCmn;
        private LibCanvas m_libCnvs;
        private int m_nWinWidth;
        private int m_nWinHeight;
        private int m_nCardHeight;
        private int m_nFontSize;
        private int m_nScrollbarSpace;
        private int m_nAreaWidth;
        private int m_nAreaHeight;
        private int m_nWidth1, m_nWidth2, m_nWidth3;
        private int m_nBlockWidth;
        private int m_nLineMax;
        private List<ClsCard> m_lstClsCard;
        public int m_nCardIdx;
        public CardWin m_cardWin;
        private DispatcherTimer m_dsptCheckTime;

        public MainWindow()
        {
            InitializeComponent();
            m_nCardHeight = 24;
            m_nFontSize = 16;
            m_nWidth1 = m_nFontSize * 13;
            m_nWidth2 = m_nFontSize * 10;
            m_nWidth3 = m_nFontSize * 10;
            m_nBlockWidth = m_nWidth1 + m_nWidth2 + m_nWidth3;
            int nBar = 1;
            m_nWinWidth = (int)( nBar * 2 + m_nBlockWidth * 3 + SystemInformation.VerticalScrollBarWidth);
            m_nWinHeight = (int)(SystemInformation.CaptionHeight + m_nCardHeight * 35 + nBar * 2);

            m_sExecPath = initExePath();
            m_libCmn = new LibCommon();
            m_libCnvs = new LibCanvas();
            m_sEnvPath = initEnvPath();
            loadEnv();
            m_lstClsCard = new List<ClsCard>();
            m_nCardIdx = -1;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.Width = m_nWinWidth + 14;
            this.Height = m_nWinHeight + 14;
            odbcLoadEnv();
            initClsCardList();
            DrawElement();
            startCheckThread();
        }
        private void wndMain_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            int max, idx;
            string sCardFileName;
            string sData;

            max = m_lstClsCard.Count;
            for (idx = 0; idx < max; idx++)
            {
                sCardFileName = m_sEnvPath + "\\card\\" + m_lstClsCard[idx].m_sSetNo + ".txt";
                sData = m_lstClsCard[idx].m_sCondition + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sSetNo + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sIP + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sSyoNo + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sSyoName + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sAddress1 + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sAddress2 + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sTel1 + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sTel2 + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sName + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sBikou + "\r\n";
                sData = sData + m_lstClsCard[idx].m_sStat + "\r\n";
                m_libCmn.SaveFileSJIS(sCardFileName, sData);
            }

        }
        private string initExePath()
        {
            string path;

            Assembly myAssembly = Assembly.GetEntryAssembly();
            path = myAssembly.Location;
            return (path.Substring(0, path.LastIndexOf("\\")));
        }
        private string initEnvPath()
        {
            string sEnvPath;
            string sEnvOptPath;

            sEnvPath = "c:\\UsesProgram";
            if (!Directory.Exists(sEnvPath))
            {
                Directory.CreateDirectory(sEnvPath);
            }
            sEnvPath = sEnvPath + "\\CenterList";
            if (!Directory.Exists(sEnvPath))
            {
                Directory.CreateDirectory(sEnvPath);
            }
            sEnvOptPath = sEnvPath + "\\log";
            if (!Directory.Exists(sEnvOptPath))
            {
                Directory.CreateDirectory(sEnvOptPath);
            }
            sEnvOptPath = sEnvPath + "\\card";
            if (!Directory.Exists(sEnvOptPath))
            {
                Directory.CreateDirectory(sEnvOptPath);
            }
            return (sEnvPath);

        }
        private void loadEnv()
        {
            string sFileName;
            string[] aryLine;

            sFileName = m_sEnvPath + "\\CenterList.env";
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            if (5 <= aryLine.Length)
            {
                m_sMdbPath = aryLine[1];
            }
            else
            {
                m_sMdbPath = "C:\\Center V2\\EventLog";
            }
        }
        public void startCheckThread()
        {
            m_dsptCheckTime = new DispatcherTimer(DispatcherPriority.Normal);
            m_dsptCheckTime.Interval = TimeSpan.FromMilliseconds(1000 * 10);
            m_dsptCheckTime.Tick += new EventHandler(checkThread);
            m_dsptCheckTime.Start();
        }
        private void checkThread(object sender, EventArgs e)
        {
            updateCardListSetStat();
        }
        private void updateCardListSetStat()
        {
            initClsCardList();
            DrawElement();
        }

        public void initClsCardList()
        {
            string sEnvCardPath;
            string sExt;
            string[] sLine;
            int idx;
            ClsCard clsCard;

            m_nCardIdx = -1;

            sEnvCardPath = m_sEnvPath + "\\card";
            string[] files = System.IO.Directory.GetFiles(sEnvCardPath);

            foreach (string sFileName in files)
            {
                sExt = System.IO.Path.GetExtension(sFileName);
                if (sExt == ".txt")
                {
                    sLine = m_libCmn.LoadFileLineSJIS(sFileName);
                    if (11 <= sLine.Length && sLine[0] != "NON")
                    {
                        idx = searchCardIdx(sLine[1]);
                        if (idx == -1)
                        {
                            clsCard = new ClsCard();
                            clsCard.m_sCondition = sLine[0];
                            clsCard.m_sSetNo = sLine[1];
                            clsCard.m_sIP = sLine[2];
                            clsCard.m_sSyoNo = sLine[3];
                            clsCard.m_sSyoName = sLine[4];
                            clsCard.m_sAddress1 = sLine[5];
                            clsCard.m_sAddress2 = sLine[6];
                            clsCard.m_sTel1 = sLine[7];
                            clsCard.m_sTel2 = sLine[8];
                            clsCard.m_sName = sLine[9];
                            clsCard.m_sBikou = sLine[10];
                            clsCard.m_sStat = sLine[11];
                            m_lstClsCard.Add(clsCard);
                        }
                        else
                        {
                            m_lstClsCard[idx].m_sCondition = sLine[0];
                            m_lstClsCard[idx].m_sSetNo = sLine[1];
                            m_lstClsCard[idx].m_sIP = sLine[2];
                            m_lstClsCard[idx].m_sSyoNo = sLine[3];
                            m_lstClsCard[idx].m_sSyoName = sLine[4];
                            m_lstClsCard[idx].m_sAddress1 = sLine[5];
                            m_lstClsCard[idx].m_sAddress2 = sLine[6];
                            m_lstClsCard[idx].m_sTel1 = sLine[7];
                            m_lstClsCard[idx].m_sTel2 = sLine[8];
                            m_lstClsCard[idx].m_sName = sLine[9];
                            m_lstClsCard[idx].m_sBikou = sLine[10];
                        }
                    }
                }
            }
        }
        public void deleteCard(string sSetNo)
        {
            int idx;
            ClsCard clsCard;
            string sData;
            string sEnvCardFile;

            if (sSetNo == "")
            {
                return;
            }
            idx = searchCardIdx(sSetNo);
            if(idx == -1){
                return;
            }
            clsCard = m_lstClsCard[idx];
            sData = "NON\r\n";
            sData = sData + clsCard.m_sSetNo + "\r\n";
            sData = sData + clsCard.m_sIP + "\r\n";
            sData = sData + clsCard.m_sSyoNo + "\r\n";
            sData = sData + clsCard.m_sSyoName + "\r\n";
            sData = sData + clsCard.m_sAddress1 + "\r\n";
            sData = sData + clsCard.m_sAddress2 + "\r\n";
            sData = sData + clsCard.m_sTel1 + "\r\n";
            sData = sData + clsCard.m_sTel2 + "\r\n";
            sData = sData + clsCard.m_sName + "\r\n";
            sData = sData + clsCard.m_sBikou + "\r\n";
            sData = sData + clsCard.m_sStat + "\r\n";
            sEnvCardFile = m_sEnvPath + "\\card\\" + sSetNo + ".txt";
            m_libCmn.SaveFileSJIS(sEnvCardFile, sData);
            m_lstClsCard.RemoveAt(idx);
            updateCardListSetStat();
        }
        public void DrawElement()
        {
            cnvsTitle.Children.Clear();
            cnvsList.Children.Clear();
            m_libCnvs.setStrokeBrush(Brushes.Black, 1.0);
            DrawTitleElement(m_nWidth1, m_nWidth2, m_nWidth3);
            DrawCardListElement(m_nWidth1, m_nWidth2, m_nWidth3);
        }
        public void DrawTitleElement(int w1, int w2, int w3)
        {
            int sx, sy, ex, ey;
            int tx;
            int wd, hi;
            int addx, addy;
            int idx;

            sx = 0;
            sy = 0;
            ex = m_nBlockWidth*3;
            ey = m_nCardHeight;
            addx = 0;
            addy = 4;

            m_libCnvs.setFontSize(m_nFontSize);
            m_libCnvs.drawLine(cnvsTitle, sx, sy, ex, sy);
            m_libCnvs.drawLine(cnvsTitle, sx, ey, ex, ey);
            m_libCnvs.drawLine(cnvsTitle, sx, sy, sx, ey);
            m_libCnvs.drawLine(cnvsTitle, sx, sy, sx, ey);
            for (idx = 0; idx < 3; idx++)
            {
                tx = sx + m_nFontSize;
                wd = m_nFontSize * 5;
                hi = m_nCardHeight;
                m_libCnvs.setFillBrush(Brushes.Black);
                m_libCnvs.drawLeftText(cnvsTitle, tx, sy, wd, hi, addx, addy, "稼動●警報");
                tx = tx + wd;
                wd = m_nFontSize;
                m_libCnvs.setFillBrush(Brushes.Red);
                m_libCnvs.drawLeftText(cnvsTitle, tx, sy, wd, hi, addx, addy, "●");
                tx = tx + wd;
                wd = m_nFontSize * 4;
                m_libCnvs.setFillBrush(Brushes.Black);
                m_libCnvs.drawLeftText(cnvsTitle, tx, sy, wd, hi, addx, addy, "持ち去り");
                tx = tx + wd;
                wd = m_nFontSize * 4;
                m_libCnvs.setFillBrush(Brushes.Green);
                m_libCnvs.drawLeftText(cnvsTitle, tx, sy, wd, hi, addx, addy, "●");

                tx = sx + w1;
                wd = w2;
                m_libCnvs.setFillBrush(Brushes.Black);
                m_libCnvs.drawCenterText(cnvsTitle, tx, sy, wd, hi, addx, addy, "通報装置番号");

                tx = sx + w1 + w2;
                wd = w3;
                m_libCnvs.setFillBrush(Brushes.Black);
                m_libCnvs.drawCenterText(cnvsTitle, tx, sy, wd, hi, addx, addy, "通報装置署番号");

                tx = sx + w1;
                m_libCnvs.drawLine(cnvsTitle, tx, sy, tx, ey);
                tx = tx + w2;
                m_libCnvs.drawLine(cnvsTitle, tx, sy, tx, ey);
                tx = tx + w3;
                m_libCnvs.drawLine(cnvsTitle, tx, sy, tx, ey);

                sx = sx + m_nBlockWidth;
            }
        }
        public void DrawCardListElement(int w1, int w2, int w3)
        {
            int max, idx, linemax;
            int wd, hi;
            int sx, sy, ex, ey;
            int tx, ty;
            int addx, addy;
            ClsCard card;
            int blkcnt;

            m_nAreaWidth = (int)(gridList.ActualWidth);
            m_nAreaHeight = (int)(gridList.ActualHeight);
            if (m_lstClsCard == null)
            {
                max = 0;
            }else{
                max = m_lstClsCard.Count;
            }
            linemax = (max + 2) / 3;
            cnvsList.Width = m_nBlockWidth * 3 + 40;
            hi = m_nCardHeight * linemax;
            if (m_nAreaHeight < hi)
            {
                cnvsList.Height = hi;
            }
            else
            {
                linemax = (m_nAreaHeight + m_nCardHeight - 1) / m_nCardHeight;
                cnvsList.Height = m_nAreaHeight;
            }
            sx = 0;
            sy = 0;
            ex = m_nBlockWidth * 3;
            ey = linemax * m_nCardHeight;
            for (idx = 0; idx < linemax; idx++)
            {
                ty = m_nCardHeight * idx;
                m_libCnvs.drawLine(cnvsList, sx, ty, ex, ty);
            }
            for (idx = 0; idx < 3; idx++)
            {
                tx = sx + w1 + m_nBlockWidth * idx;
                m_libCnvs.drawLine(cnvsList, tx, sy, tx, ey);
                tx = tx + w2;
                m_libCnvs.drawLine(cnvsList, tx, sy, tx, ey);
                tx = tx + w3;
                m_libCnvs.drawLine(cnvsList, tx, sy, tx, ey);
            }
            addx = 0;
            addy = 4;
            blkcnt = 0;
            for (idx = 0; idx < max; idx++)
            {
                card = m_lstClsCard[idx];
                tx = sx + m_nBlockWidth * blkcnt;
                wd = w1;
                if (card.m_sStat == "")
                {
                    m_libCnvs.drawCenterText(cnvsList, tx, sy, wd, hi, addx, addy, "");
                }
                else
                {
                    if (card.m_sStat == "0")
                    {
                        m_libCnvs.setFillBrush(Brushes.Black);
                    }
                    else if (card.m_sStat == "1")
                    {
                        m_libCnvs.setFillBrush(Brushes.Red);
                    }
                    else if (card.m_sStat == "2")
                    {
                        m_libCnvs.setFillBrush(Brushes.Green);
                    }
                    m_libCnvs.drawCenterText(cnvsList, tx, sy, wd, hi, addx, addy, "●");
                }
                tx = tx + w1;
                wd = w2;
                if (card.m_sCondition == "DEL")
                {
                    m_libCnvs.setFillBrush(Brushes.LightGray);
                }
                else
                {
                    m_libCnvs.setFillBrush(Brushes.Black);
                }
                m_libCnvs.drawCenterText(cnvsList, tx, sy, wd, hi, addx, addy, card.m_sSetNo);
                tx = tx + w2;
                wd = w3;
                m_libCnvs.drawCenterText(cnvsList, tx, sy, wd, hi, addx, addy, card.m_sSyoNo);

                blkcnt++;
                if (3 <= blkcnt)
                {
                    blkcnt = 0;
                    sy = sy + m_nCardHeight;
                }
            }
        }
        private void Window_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            int idxX, idxY, idx;

            if (m_nCardIdx != -1)
            {
                selectCrtCardWin();
                return;
            }
            Point pos = e.GetPosition(cnvsList);
            idxX = (int)(pos.X / m_nBlockWidth);
            idxY = (int)(pos.Y / m_nCardHeight);
            idx = idxX + idxY * 3; 
            if (0 > idx)
            {
                return;
            }
            if (m_lstClsCard.Count <= idx)
            {
                m_nCardIdx = -1;
            }
            else
            {
                m_nCardIdx = idx;
            }
            setCrtCardWinDisp();
        }
        public void selectCrtCardWin()
        {
            if (m_cardWin != null)
            {
                m_cardWin.Left = 380;
                m_cardWin.Top = 240;
                //m_cardWin.Activate();
            }
        }
        public void setCrtCardWinDisp()
        {
            if (m_cardWin != null)
            {
                return;
            }
            if (m_nCardIdx == -1)
            {
                return;
            }
            m_cardWin = new CardWin();
            m_cardWin.SetMainWindow(this);
            m_cardWin.SetClsCard(m_lstClsCard[m_nCardIdx]);
            m_cardWin.Owner = this;
            m_cardWin.Show();
        }
        private int searchCardIdx(String sId)
        {
            int max, idx;

            max = m_lstClsCard.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (m_lstClsCard[idx].m_sSetNo == sId)
                {
                    return (idx);
                }
            }
            return (-1);
        }
        private void outputError(string sMsg, string sError)
        {

        }
    }
}
