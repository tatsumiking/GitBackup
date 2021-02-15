using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Printing;
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
    /// MeisaiPreviewWin.xaml の相互作用ロジック
    /// </summary>
    public partial class MeisaiPreviewWin : Window
    {
        public SougiWin m_wnd;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibCanvas m_libCanvas;
        public LibOdbc m_libOdbc;
        PrintQueue m_queueCrt;
        private string[] m_aryRecLine;
        private List<clsSyouhinIdx> m_listShyouhin;
        private List<string> m_lstSeikyuKubun;
        private string m_sSoukeName;
        private string m_sSikiGMD;
        private string m_sSikiSHMEHM; 
        private int m_nPageMax, m_nCrtPage;
        private double m_dAddX, m_dAddY;
        private double m_dXTime, m_dYTime;

        public MeisaiPreviewWin()
        {
            InitializeComponent();
        }
        public void setSougiWin(SougiWin wnd)
        {
            m_wnd = wnd;
            m_sExecPath = m_wnd.m_sExecPath;
            m_libCmn = m_wnd.m_libCmn;
            m_libCanvas = m_wnd.m_libCanvas;
            m_libOdbc = m_wnd.m_libOdbc;
        }
        public void setSoukeName(string sSouke)
        {
            m_sSoukeName = sSouke;
        }
        public void setSikiGMD(string sSikiGMD)
        {
            m_sSikiGMD = sSikiGMD;
        }
        public void setSikiSHMEHM(string sSikiSHMEHM)
        {
            m_sSikiSHMEHM = sSikiSHMEHM;
        }
        public void setAryRecElement(string[] aryLine)
        {
            m_aryRecLine = aryLine;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.WindowState = WindowState.Maximized;
            m_queueCrt = getDefaultPrintQueue();
            initSeikyuKubun();
            initSyouhinTable();
            m_nPageMax = calcPageCount();
            m_nCrtPage = 1;
            m_dYTime = canvasPage.ActualHeight / 297.0;
            m_dXTime = m_dYTime;
            m_dAddX = (canvasPage.ActualWidth - (m_dXTime * 210.0)) / 2.0;
            m_dAddY = 0;

            drawPreviewPage();

        }
        private void btnPrint_Click(object sender, RoutedEventArgs e)
        {
            printOutMain();
        }

        private void btnPrevPage_Click(object sender, RoutedEventArgs e)
        {
            m_nCrtPage--;
            if (m_nCrtPage <= 0)
            {
                m_nCrtPage = 1;
            }
            else
            {
                drawPreviewPage();
            }
        }

        private void btnNextPage_Click(object sender, RoutedEventArgs e)
        {
            m_nCrtPage++;
            if (m_nPageMax < m_nCrtPage)
            {
                m_nCrtPage = m_nPageMax;
            }
            else
            {
                drawPreviewPage();
            }
        }
        private void btnPrintSetting_Click(object sender, RoutedEventArgs e)
        {
            printSetting();
        }
        private void btnReturn_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void drawPreviewPage()
        {
            double sx, sy, ex, ey;

            if (m_nCrtPage == 1)
            {
                btnPrevPage.IsEnabled = false;
            }
            if (m_nCrtPage == m_nPageMax)
            {
                btnNextPage.IsEnabled = false;
            }
            sx = trnsX(0.0);
            sy = trnsY(0.0);
            ex = trnsX(210.0);
            ey = trnsY(297.0);
            m_libCanvas.setBrushFill(0xff, 0xff, 0xff);
            m_libCanvas.drawBoxs(canvasPage, sx, sy, ex - sx, ey - sy);

            drawPageToCanvas(canvasPage, m_nCrtPage);
        }
        private PrintQueue getDefaultPrintQueue()
        {
            LocalPrintServer lps = new LocalPrintServer();
            return (lps.DefaultPrintQueue);
        }
        private void initSeikyuKubun()
        {
            m_lstSeikyuKubun = m_libOdbc.getSkkubunList("");
        }
        private void initSyouhinTable()
        {
            int max, idx;
            string[] sAry;

            m_listShyouhin = new List<clsSyouhinIdx>();
            max = m_aryRecLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                sAry = m_aryRecLine[idx].Split(',');
                if (7 < sAry.Length)
                {
                    setListSyouhin(sAry, idx);
                }
            }
        }
        private void setListSyouhin(string[] sAry, int setIdx)
        {
            int max, idx;
            clsSyouhinIdx clsS;

            max = m_listShyouhin.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (m_listShyouhin[idx].sName == sAry[7])
                {
                    m_listShyouhin[idx].listIdx.Add(setIdx);
                    break;
                }
            }
            if (idx == max)
            {
                clsS = new clsSyouhinIdx();
                clsS.sName = sAry[7];
                clsS.listIdx.Add(setIdx);
                m_listShyouhin.Add(clsS);
            }
        }
        private void printSetting()
        {
            PrintDialog dPrt;

            dPrt = new PrintDialog();
            dPrt.PrintQueue = m_queueCrt;
            if (dPrt.ShowDialog() == true)
            {
                m_queueCrt = dPrt.PrintQueue;
            }
        }
        private void printOutMain()
        {
            PageImageableArea area;
            int nPage;
            Canvas canvas;
            FixedPage page;
            PageContent cont;
            FixedDocument doc;
            PrintDialog dPrt;

            // 210 × 297 ミリ（約 8.27 × 約 11.69 インチ）＝794 × 1123 pixel
            area = m_queueCrt.GetPrintCapabilities().PageImageableArea;
            m_dXTime = 794.0 / 210.0;
            m_dYTime = 1123.0 / 297.0;
            m_dAddX = 0;
            m_dAddY = 0;
            doc = new FixedDocument();
            for (nPage = 1; nPage <= m_nPageMax; nPage++)
            {
                canvas = new Canvas();
                canvas.Width = area.OriginWidth + area.ExtentWidth;
                canvas.Height = area.OriginHeight + area.ExtentHeight;

                drawPageToCanvas(canvas, nPage);

                page = new FixedPage();
                page.Children.Add(canvas);
                cont = new PageContent();
                cont.Child = page;
                doc.Pages.Add(cont);
            }

            dPrt = new PrintDialog();
            dPrt.PrintQueue = m_queueCrt;
            dPrt.PrintDocument(doc.DocumentPaginator, "明細書");

        }
        private void drawPageToCanvas(Canvas canvas, int nPage)
        {
            double dCrtY;
            double dFs;
            int blkmax, blkidx;
            int max, idx, no;
            int lmax, lidx, len, lenmax;
            int cmax, cidx;
            string[] sAry;
            string sSyouhin, sMark, sFuda, sKubun;
            double sx, sy, ex, ey;
            double space, time;
            string sStr;

            m_libCanvas.setFontSize(24.0);
            m_libCanvas.setFontName("ＭＳ 明朝");
            m_libCanvas.setFontNomal();
            m_libCanvas.setBrushFill(-1, -1, -1);
            m_libCanvas.setBrushStroke(0, 0, 0, 0.5, 0);
            m_libCanvas.setBrushText(0, 0, 0);

            printPageLayout(canvas, nPage, m_nPageMax);

            dFs = trnsLen(4.0);
            dCrtY = 41;
            space = 0.0;
            time = 1.0;
            m_libCanvas.setFontSize(dFs);

            m_libCanvas.setFontNomal();
            blkmax = m_listShyouhin.Count;
            for (blkidx = 0; blkidx < blkmax; blkidx++)
            {
                if (m_listShyouhin[blkidx].nPageNo == nPage)
                {
                    sSyouhin = m_listShyouhin[blkidx].sName;
                    max = m_listShyouhin[blkidx].listIdx.Count;
                    for (idx = 0; idx < max; idx++)
                    {
                        no = m_listShyouhin[blkidx].listIdx[idx];
                        sMark = "";
                        sFuda = "";
                        sKubun = "";
                        sAry = m_aryRecLine[no].Split(',');
                        if (37 <= sAry.Length)
                        {
                            sMark = getKubunMark(sAry[24]);
                            sFuda = getFudaStr(sAry);
                            sKubun = sAry[24];
                        }

                        m_libCanvas.setFontName("ＭＳ ゴシック");
                        sx = trnsX(26) - (dFs * 2);
                        sy = trnsY(dCrtY + 5);
                        no = idx + 1;
                        sStr = sMark + no;
                        m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
                        m_libCanvas.setFontName("ＭＳ 明朝");
                        sx = trnsX(28.5);
                        sy = trnsY(dCrtY + 1);
                        ex = trnsX(61.0);
                        ey = sy + dFs;
                        sAry = sSyouhin.Split(' ');
                        lmax = sAry.Length;
                        for (lidx = 0; lidx < lmax; lidx++)
                        {
                            m_libCanvas.drawRightEqualStrg(canvas, sx, sy, ex, ey, sAry[lidx]);
                            sy = ey + 1;
                            ey = sy + dFs;
                        }

                        sx = trnsX(62.5);
                        sy = trnsY(dCrtY + 1);
                        ex = trnsX(171.0);
                        lenmax = (int)((ex - sx - dFs) / dFs);
                        sAry = sFuda.Split(' ');
                        cmax = sAry.Length;
                        sStr = sAry[0];
                        lidx = 0;
                        for (cidx = 1; cidx < cmax; cidx++)
                        {
                            len = sStr.Length + 1 + sAry[cidx].Length;
                            if (lenmax < len)
                            {
                                ey = sy + dFs;
                                m_libCanvas.drawLeftEqualStrg(canvas, sx, sy, ex, ey, sStr);
                                sy = sy + dFs + 1;
                                lidx++;
                                if (3 <= lidx)
                                {
                                    sStr = "";
                                    break;
                                }
                                sStr = sAry[cidx];
                            }
                            else
                            {
                                sStr = sStr + " " + sAry[cidx];
                            }
                        }
                        if (sStr != "")
                        {
                            ey = sy + dFs;
                            m_libCanvas.drawLeftEqualStrg(canvas, sx, sy, ex, ey, sStr);
                            sy = sy + dFs + 1;
                        }

                        m_libCanvas.setFontName("ＭＳ ゴシック");
                        sx = trnsX(172.5);
                        sy = trnsY(dCrtY + 1);
                        sStr = sKubun;
                        m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

                        dCrtY = dCrtY + 16;
                    }
                    dCrtY = dCrtY + 16;
                }
            }
        }
        private int calcPageCount()
        {
            int max, idx, line;
            int nPage;

            nPage = 1;
            line = 0;
            max = m_listShyouhin.Count;
            for (idx = 0; idx < max; idx++)
            {
                m_listShyouhin[idx].nPageNo = nPage;
                line = line + m_listShyouhin[idx].listIdx.Count;
                line++;
                if (15 < line)
                {
                    nPage++;
                    m_listShyouhin[idx].nPageNo = nPage;
                    line = line + m_listShyouhin[idx].listIdx.Count;
                    line++;
                }
            }
            return (nPage);
        }
        private void printPageLayout(Canvas canvas, int nPage, int nPageMax)
        {
            double dFs;
            double sx, sy, ex, ey;
            double dx, dy, yy;
            double space, time;
            string sStr;
            int idx, max;

            space = 0.0;
            time = 1.0;

            m_libCanvas.setFontName("ＭＳ ゴシック");
            m_libCanvas.setFontNomal();
            dFs = trnsLen(3.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(18.0);
            sy = trnsY(12.0);
            sStr = "NO. " + nPage + " / " + nPageMax;
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ 明朝");
            m_libCanvas.setFontBold();
            dFs = trnsLen(6.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(16.0);
            sy = trnsY(21.0);
            sStr = m_sSoukeName;
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ ゴシック");
            m_libCanvas.setFontBold();
            dFs = trnsLen(4.5);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(153.0);
            sy = trnsY(15.0);
            sStr = m_sSikiGMD;
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sx = trnsX(169.0);
            sy = trnsY(24.0);
            sStr = m_sSikiSHMEHM;
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ ゴシック");
            m_libCanvas.setFontNomal();
            dFs = trnsLen(4.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(17.0);
            sy = trnsY(34.0);
            sStr = "No";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sx = trnsX(41.0);
            sy = trnsY(31.0);
            sStr = "価格";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(33.0);
            sy = trnsY(36.0);
            sStr = "(消費税込み)";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sx = trnsX(110.0);
            sy = trnsY(34.0);
            sStr = "御芳名";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sx = trnsX(181.0);
            sy = trnsY(34.0);
            sStr = "備考";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sx = trnsX(10.0);
            ex = trnsX(200.0);
            dy = trnsY(30.0);
            m_libCanvas.drawLine(canvas, sx, dy, ex, dy);

            dy = trnsY(41.0);
            m_libCanvas.drawLine(canvas, sx, dy, ex, dy);

            sy = trnsY(30.0);
            ey = trnsY(281.0);
            dx = trnsX(10.0);
            m_libCanvas.drawLine(canvas, dx, sy, dx, ey);

            dx = trnsX(28.0);
            m_libCanvas.drawLine(canvas, dx, sy, dx, ey);

            dx = trnsX(62.0);
            m_libCanvas.drawLine(canvas, dx, sy, dx, ey);

            dx = trnsX(172.0);
            m_libCanvas.drawLine(canvas, dx, sy, dx, ey);

            dx = trnsX(200.0);
            m_libCanvas.drawLine(canvas, dx, sy, dx, ey);

            sx = trnsX(10.0);
            ex = trnsX(200.0);
            yy = 57.0;
            max = 15;
            for (idx = 0; idx < max; idx++)
            {
                dy = trnsY(yy);
                m_libCanvas.drawLine(canvas, sx, dy, ex, dy);
                yy = yy + 16;
            }
        }
        private string getKubunMark(string sKubun)
        {
            string sMark;
            int max, idx;
            string[] sAry;

            sMark = "";
            max = m_lstSeikyuKubun.Count;
            for (idx = 0; idx < max; idx++)
            {
                sAry = m_lstSeikyuKubun[idx].Split(',');
                if (2 <= sAry.Length)
                {
                    if (sAry[1] == sKubun)
                    {
                        sMark = sAry[2];
                        break;
                    }
                }
            }
            if (sMark == "")
            {
                sMark = "　";
            }
            return (sMark);
        }
        private string getFudaStr(string[] sAry)
        {
            string sFuda;

            sFuda = "";
            if (37 <= sAry.Length)
            {
                sFuda = sAry[28] + " " + sAry[29] + " " + sAry[30] + " " + sAry[31] + " ";
                sFuda = sFuda + sAry[32] + " " + sAry[33] + " " + sAry[34] + " " + sAry[35];
            }
            return (sFuda);
        }
        private double trnsLen(double num)
        {
            if (m_dXTime < m_dYTime)
            {
                num = num * m_dXTime;
            }
            else
            {
                num = num * m_dYTime;
            }
            return (num);
        }
        private double trnsX(double dx)
        {
            dx = dx * m_dXTime + m_dAddX;
            return (dx);
        }
        private double trnsY(double dy)
        {
            dy = dy * m_dYTime + m_dAddY;
            return (dy);
        }

    }
}
