using System;
using System.Collections.Generic;
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
    /// SeikyuPreviewWin.xaml の相互作用ロジック
    /// </summary>
    public partial class SeikyuPreviewWin : Window
    {
        public SougiSeikyuWin m_wnd;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibCanvas m_libCanvas;
        public LibOdbc m_libOdbc;
        PrintQueue m_queueCrt;
        private string m_sSougi;
        private string m_sSeikyu;
        private string[] m_aryRecLine;
        private string m_sBunsyou1; // txtBunsyou1.Text
        private string m_sBunsyou2; // txtBunsyou2.Text
        private string m_sSeikyusaki;
        private string m_sSasidasinin;
        private string m_sFurikomisaki;

        private double m_dAddX, m_dAddY;
        private double m_dXTime, m_dYTime;

        public SeikyuPreviewWin()
        {
            InitializeComponent();
        }
        public void setSougiSeikyuWin(SougiSeikyuWin wnd)
        {
            m_wnd = wnd;
            m_sExecPath = m_wnd.m_sExecPath;
            m_libCmn = m_wnd.m_libCmn;
            m_libCanvas = m_wnd.m_libCanvas;
            m_libOdbc = m_wnd.m_libOdbc;
        }
        public void setBunsyou1(string sStr)
        {
            m_sBunsyou1 = sStr;
        }
        public void setBunsyou2(string sStr)
        {
            m_sBunsyou2 = sStr;
        }
        public void setSougi(string sFilds)
        {
            m_sSougi = sFilds;
        }
        public void setSeikyu(string sFilds)
        {
            m_sSeikyu = sFilds;
        }
        public void setSeikyusaki(string sFilds)
        {
            m_sSeikyusaki = sFilds;
        }
        public void setSasidasinin(string sFilds)
        {
            m_sSasidasinin = sFilds;
        }
        public void setFurikomisaki(string sFilds)
        {
            m_sFurikomisaki = sFilds;
        }
        public void setAryRecElement(string[] aryLine)
        {
            m_aryRecLine = aryLine;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.WindowState = WindowState.Maximized;
            m_queueCrt = getDefaultPrintQueue();
            m_dYTime = canvasPreview.ActualHeight / 297.0;
            m_dXTime = m_dYTime;
            m_dAddX = (canvasPreview.ActualWidth - (m_dXTime * 210.0)) / 2.0;
            m_dAddY = 0;

            drawPreview();

        }
        private void btnPrint_Click(object sender, RoutedEventArgs e)
        {
            printOutMain();
        }
        private void btnPrintSetting_Click(object sender, RoutedEventArgs e)
        {
            printSetting();
        }
        private void btnReturn_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private PrintQueue getDefaultPrintQueue()
        {
            LocalPrintServer lps = new LocalPrintServer();
            return (lps.DefaultPrintQueue);
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

            canvas = new Canvas();
            canvas.Width = area.OriginWidth + area.ExtentWidth;
            canvas.Height = area.OriginHeight + area.ExtentHeight;

            drawCanvas(canvas);

            page = new FixedPage();
            page.Children.Add(canvas);
            cont = new PageContent();
            cont.Child = page;
            doc.Pages.Add(cont);

            dPrt = new PrintDialog();
            dPrt.PrintQueue = m_queueCrt;
            dPrt.PrintDocument(doc.DocumentPaginator, "請求書");

        }
        private void drawPreview()
        {
            double sx, sy, ex, ey;

            sx = trnsX(0.0);
            sy = trnsY(0.0);
            ex = trnsX(210.0);
            ey = trnsY(297.0);
            m_libCanvas.setBrushFill(0xff, 0xff, 0xff);
            m_libCanvas.drawBoxs(canvasPreview, sx, sy, ex - sx, ey - sy);

            drawCanvas(canvasPreview);
        }
        private void drawCanvas(Canvas canvas)
        {
            double sx, sy, ex, ey;
            double wd, hi;
            double dx, dy;
            double space, time;
            double dFs;
            string sStr;
            string[] aryRec;
            string[] arySs;
            string[] arySk;
            string[] arySg;
            string[] aryFr;
            string[] ary;
            int max, idx;
            int lmax, lidx;
            int cmax, cidx;
            int lenmax, len;
            string sFuda;
            string[] sAry;
            double m_nCrtY;
            double crty1, crty2;
            int nSumPrice;

            space = 0.0;
            time = 1.0;

            m_libCanvas.setFontSize(24.0);
            m_libCanvas.setFontName("ＭＳ 明朝");
            m_libCanvas.setFontNomal();
            m_libCanvas.setBrushFill(-1, -1, -1);
            m_libCanvas.setBrushStroke(0, 0, 0, 0.5, 0);
            m_libCanvas.setBrushText(0, 0, 0);

            aryRec = m_aryRecLine[0].Split(',');
            arySs = m_sSasidasinin.Split(',');
            arySk = m_sSeikyu.Split(',');
            arySg = m_sSougi.Split(',');
            aryFr = m_sFurikomisaki.Split(',');

            m_libCanvas.setFontName("ＭＳ ゴシック");
            m_libCanvas.setFontNomal();
            dFs = trnsLen(6.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(88.0);
            sy = trnsY(12.0);
            sStr = "請　求　書";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            
            m_libCanvas.setFontName("ＭＳ 明朝");
            dFs = trnsLen(5.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(12.0);
            sy = trnsY(35.0);
            sStr = getAryString(aryRec, 14);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(117.0);
            sStr = getAryString(aryRec,16);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(11.0);
            ex = trnsX(125.0);
            dy = trnsY(41.0);
            m_libCanvas.drawLine(canvas, sx, dy, ex, dy);

            m_libCanvas.setFontName("ＭＳ ゴシック");
            dFs = trnsLen(4.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(15.0);
            sy = trnsY(43.0);
            sStr = "下記のとおりご請求申し上げます。";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ ゴシック");
            dFs = trnsLen(4.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(130.0);
            sy = trnsY(34.0);
            sStr = getAryString(arySs, 1);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            dFs = trnsLen(5.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(130.0);
            sy = trnsY(39.0);
            sStr = getAryString(arySs, 2);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            dFs = trnsLen(3.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(128.0);
            sy = trnsY(46.0);
            sStr = "〒" + getAryString(arySs, 3) + " " + getAryString(arySs, 4);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            if (getAryString(arySs,5) != "")
            {
                sx = trnsX(135.0);
                sy = trnsY(49.0);
                sStr = getAryString(arySs, 5);
                m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            }
            sx = trnsX(141.0);
            sy = trnsY(52.0);
            sStr = "電話　" + getAryString(arySs, 6);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sy = trnsY(57.0);
            sStr = "FAX　" + getAryString(arySs, 7);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ ゴシック");
            sx = trnsX(10.0);
            sy = trnsY(62.0);
            wd = trnsLen(133.0);
            hi = trnsLen(16.0);
            m_libCanvas.drawBoxs(canvas, sx, sy, wd, hi);
            dFs = trnsLen(3.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(12.0);
            sy = trnsY(63.0);
            sStr = "指定コード番号";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(37.0);
            sy = trnsY(63.0);
            sStr = "お振込名";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ 明朝");
            dFs = trnsLen(4.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(14.0);
            sy = trnsY(70.0);
            sStr = getAryString(arySk, 2);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(185.0);
            sy = trnsY(15.0);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sx = trnsX(36.0);
            sy = trnsY(70.0);
            sStr = getAryString(aryRec,23);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ ゴシック");
            dFs = trnsLen(3.5);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(10.0);
            sy = trnsY(81.0);
            // 複数行に対応する必要あり
            sStr = m_sBunsyou1;
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ 明朝");
            dFs = trnsLen(6.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(75.0);
            sy = trnsY(95.0);
            sStr = getAryString(aryRec,5);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ ゴシック");
            dFs = trnsLen(5.5);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(75.0 + 6.0 * aryRec[5].Length);
            sy = trnsY(95.0);
            sStr = "供花代金";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            max = m_aryRecLine.Length;
            sx = trnsX(10.0);
            sy = trnsY(104.0);
            wd = trnsLen(190.0);
            hi = trnsLen(75.0);
            m_libCanvas.drawBoxs(canvas, sx, sy, wd, hi);
            sx = trnsX(10.0);
            ex = trnsX(200.0);
            dy = trnsY(115.0);
            m_libCanvas.drawLine(canvas, sx, dy, ex, dy);
            dy = trnsY(120.0);
            m_libCanvas.drawLine(canvas, sx, dy, ex, dy);
            dx = trnsX(57.0);
            sy = trnsY(115.0);
            ey = trnsY(179.0);
            m_libCanvas.drawLine(canvas, dx, sy, dx, ey);

            m_libCanvas.setFontName("ＭＳ ゴシック");
            dFs = trnsLen(3.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(11.0);
            sy = trnsY(105.0);
            sStr = "通夜";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(58.0);
            sStr = "告別式";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(112.0);
            sStr = "施工場所";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ 明朝");
            dFs = trnsLen(4.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(11.0);
            sy = trnsY(110.0);
            sStr = m_libCmn.convWakeki(getAryString(arySg, 4));
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(58.0);
            sStr = m_libCmn.convWakeki(getAryString(arySg, 7));
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(112.0);
            sStr = getAryString(arySg, 10);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ ゴシック");
            dFs = trnsLen(3.0);
            m_libCanvas.setFontSize(dFs);
            sx = trnsX(22.0);
            sy = trnsY(116.0);
            sStr = "品名・個数・金額";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(120.0);
            sStr = "供花名札";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ 明朝");
            dFs = trnsLen(4.0);
            m_libCanvas.setFontSize(dFs);
            m_nCrtY = 121.0;
            nSumPrice = 0;
            max = m_aryRecLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                aryRec = m_aryRecLine[idx].Split(',');
                sx = trnsX(11.0);
                ex = trnsX(56.0);
                sStr = getAryString(aryRec, 7);
                sAry = sStr.Split(' ');
                crty1 = m_nCrtY;
                lmax = sAry.Length;
                for (lidx = 0; lidx < lmax; lidx++) {
                    sy = trnsY(crty1);
                    ey = sy + dFs;
                    m_libCanvas.drawRightEqualStrg(canvas, sx, sy, ex, ey, sAry[lidx]);
                    crty1 = crty1 + 4.0;
                }

                crty2 = m_nCrtY;
                sx = trnsX(58.0);
                ex = trnsX(200.0);
                sy = trnsY(crty2);
                ey = sy + dFs;
                lenmax = (int)((ex - sx - dFs) / dFs);
                sFuda = getFudaStr(aryRec);
                sAry = sFuda.Split(' ');
                cmax = sAry.Length;
                sStr = sAry[0];
                lidx = 0;
                for (cidx = 1; cidx < cmax; cidx++)
                {
                    len = sStr.Length + 1 + sAry[cidx].Length;
                    if (lenmax < len)
                    {
                        sy = trnsY(crty2);
                        ey = sy + dFs;
                        m_libCanvas.drawLeftEqualStrg(canvas, sx, sy, ex, ey, sStr);
                        crty2 = crty2 + 5.0;
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
                    sy = trnsY(crty2);
                    ey = sy + dFs;
                    m_libCanvas.drawLeftEqualStrg(canvas, sx, sy, ex, ey, sStr);
                    crty2 = crty2 + 5.0;
                }

                nSumPrice = nSumPrice + m_libCmn.StrToInt(getAryString(aryRec, 13));
                if (crty1 > crty2)
                {
                    m_nCrtY = crty1;
                }
                else
                {
                    m_nCrtY = crty2;
                }
            }

            m_libCanvas.setFontName("ＭＳ ゴシック");
            sx = trnsX(43.0);
            sy = trnsY(185.0);
            sStr = "御請求合計金額";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ 明朝");
            sx = trnsX(90.0);
            sStr = nSumPrice.ToString("C") + "也　(消費税込)";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);


            m_libCanvas.setFontName("ＭＳ ゴシック");
            sx = trnsX(11.0);
            sy = trnsY(200.0);
            sStr = "お振込みされるお客様へ";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sy = trnsY(205.0);
            sStr = "誠に勝手ではございますが、振込手数料はお客様にてご負担くださいませ。";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sy = trnsY(210.0);
            sStr = "上記番号・上記名　以外で振込をされますと処理に行き違いが生じますのでお手数ですが";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sy = trnsY(215.0);
            sStr = "必ず指定の振込番号・お名前をご入力いただきますようお願い申し上げます。";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            ary = m_sBunsyou2.Split('\n');
            m_nCrtY = 225.0;
            max = ary.Length;
            for (idx = 0; idx < max; idx++)
            {
                sy = trnsY(m_nCrtY);
                m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, ary[idx]);
                m_nCrtY = m_nCrtY + 5.0;
            }

            sx = trnsX(20.0);
            sy = trnsY(254.0);
            wd = trnsLen(170.0);
            hi = trnsLen(26.0);
            m_libCanvas.drawBoxs(canvas, sx, sy, wd, hi);
            sx = trnsX(73.0);
            sy = trnsY(255.0);
            sStr = "【下記銀行へ御振込お願いします】";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(28.0);
            sy = trnsY(268.0);
            sStr = "【振込先】";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(52.0);
            sy = trnsY(260.0);
            sStr = getAryString(aryFr, 1) + " " + getAryString(aryFr, 2);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(52.0);
            sy = trnsY(265.0);
            sStr = getAryString(aryFr, 3) + "No. " + getAryString(aryFr, 4);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(52.0);
            sy = trnsY(270.0);
            sStr = getAryString(aryFr, 5);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
            sx = trnsX(52.0);
            sy = trnsY(275.0);
            sStr = getAryString(aryFr,6);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            sx = trnsX(125.0);
            sy = trnsY(284.0);
            sStr = "生花部受付";
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);

            m_libCanvas.setFontName("ＭＳ 明朝");
            sx = trnsX(152.0);
            sy = trnsY(284.0);
            sStr = getAryString(arySg, 11);
            m_libCanvas.drawYokoStrg(canvas, sx, sy, space, time, sStr);
        }
        private string getAryString(string[] sAry, int idx)
        {
            if (idx < sAry.Length)
            {
                return (sAry[idx]);
            }
            else
            {
                return ("");
            }
        }
        private string getFudaStr(string[] sAry)
        {
            string sFuda;

            sFuda = getAryString(sAry, 28) + " " + getAryString(sAry, 29);
            sFuda = sFuda + " " + getAryString(sAry, 30) + " " + getAryString(sAry, 31);
            sFuda = sFuda + " " + getAryString(sAry, 32) + " " + getAryString(sAry, 33);
            sFuda = sFuda + " " + getAryString(sAry, 34) + " " + getAryString(sAry, 35);
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
