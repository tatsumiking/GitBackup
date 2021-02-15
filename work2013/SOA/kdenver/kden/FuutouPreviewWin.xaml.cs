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
    /// FuutouPreviewWin.xaml の相互作用ロジック
    /// </summary>
    public partial class FuutouPreviewWin : Window
    {
        public SougiSeikyuWin m_wnd;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibCanvas m_libCanvas;
        public LibOdbc m_libOdbc;
        PrintQueue m_queueCrt;
        private string m_sSeikyusaki;
        private string[] m_arySeikyusaki;
        private string m_sPost3, m_sPost4;
        private double m_dAddX, m_dAddY;
        private double m_dXTime, m_dYTime;
        private double m_dFontSize, m_dFontTime, m_dFontSpace;
        private string m_sFontName;
        private int m_nTateGaki;

        public FuutouPreviewWin()
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
        public void setSeikyusaki(string sFilds)
        {
            string sPostNo;

            m_sSeikyusaki = sFilds;
            m_arySeikyusaki = m_sSeikyusaki.Split(',');
            sPostNo = getAryString(m_arySeikyusaki, 6);
            if (sPostNo.Length == 7)
            {
                m_sPost3 = sPostNo.Substring(0, 3);
                m_sPost4 = sPostNo.Substring(3, 4);
            }
            else if (sPostNo.Length == 8)
            {
                m_sPost3 = sPostNo.Substring(0, 3);
                m_sPost4 = sPostNo.Substring(4, 4);
            }
            else
            {
                m_sPost3 = "";
                m_sPost4 = "";
            }
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.WindowState = WindowState.Maximized;
            m_queueCrt = getDefaultPrintQueue();

            m_dYTime = canvasPreview.ActualHeight / 235.0;
            m_dXTime = m_dYTime;
            m_dAddX = (canvasPreview.ActualWidth - (m_dXTime * 120.0)) / 2.0;
            m_dAddY = 0;

            drawPreview();
        }
        private void btnPrint_Click(object sender, RoutedEventArgs e)
        {
            printOutMain();
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
        private void printOutMain()
        {
            PageImageableArea area;
            Canvas canvas;
            double cx, cy;
            FixedPage page;
            PageContent cont;
            FixedDocument doc;
            PrintDialog dPrt;

            dPrt = new PrintDialog();
            dPrt.PrintQueue = m_queueCrt;
            if (dPrt.ShowDialog() == false)
            {
                return;
            }
            m_queueCrt = dPrt.PrintQueue;
            // 210 × 297 ミリ（約 8.27 × 約 11.69 インチ）＝794 × 1123 pixel
            area = m_queueCrt.GetPrintCapabilities().PageImageableArea;
            m_dXTime = 794.0 / 210.0;
            m_dYTime = 1123.0 / 297.0;
            m_dAddX = 0;
            m_dAddY = 0;
            doc = new FixedDocument();

            canvas = new Canvas();
            canvas.Width = trnsLen(120.0);
            canvas.Height = trnsLen(235.0);

            drawCanvas(canvas);
            cx = canvas.Width / 2.0;
            cy = canvas.Height / 2.0;
            canvas.RenderTransform = new RotateTransform(180.0, cx, cy);
            page = new FixedPage();
            page.Children.Add(canvas);
            cont = new PageContent();
            cont.Child = page;
            doc.Pages.Add(cont);

            dPrt = new PrintDialog();
            dPrt.PrintQueue = m_queueCrt;
            dPrt.PrintDocument(doc.DocumentPaginator, "封筒");

        }
        private void drawPreview()
        {
            double sx, sy, ex, ey;

            sx = trnsX(0.0);
            sy = trnsY(0.0);
            ex = trnsX(120.0);
            ey = trnsY(235.0);
            m_libCanvas.setBrushFill(0xff, 0xff, 0xff);
            m_libCanvas.drawBoxs(canvasPreview, sx, sy, ex - sx, ey - sy);

            drawCanvas(canvasPreview);
        }
        private void drawCanvas(Canvas canvas)
        {
            double fs, sp;
            string sFileName;
            string[] aryLine;
            int max, idx;
            string[] ary;
            double dx, dy;
            double sx, sy;
            double wd, hi;
            string str;

            m_dFontSize = 5.0;
            m_dFontTime = 1.0;
            m_dFontSpace = 0.0;
            m_sFontName = "ＭＳ 明朝";
            m_nTateGaki = 0;
            fs = trnsLen(m_dFontSize);
            m_libCanvas.setFontSize(fs);
            m_libCanvas.setFontName(m_sFontName);
            m_libCanvas.setFontNomal();
            m_libCanvas.setBrushFill(-1, -1, -1);
            m_libCanvas.setBrushStroke(0, 0, 0, 0.5, 0);
            m_libCanvas.setBrushText(0, 0, 0);

            sFileName = m_sExecPath + "\\kden\\fuutou.lay";
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = aryLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                ary = aryLine[idx].Split(',');
                if (ary.Length < 2)
                {
                    continue;
                }
                switch (ary[0]) 
                {
                    case "FONT":
                        wd = m_libCmn.StrToDouble(ary[1]);
                        hi = m_libCmn.StrToDouble(ary[2]);
                        m_sFontName = ary[3];
                        m_nTateGaki = m_libCmn.StrToInt(ary[4]);
                        if (m_nTateGaki == 0)
                        {
                            m_dFontSize = hi;
                            m_dFontTime = wd / hi;
                        }
                        else
                        {
                            m_dFontSize = wd;
                            m_dFontTime = hi / wd;
                        }
                        fs = trnsLen(m_dFontSize);
                        m_libCanvas.setFontSize(fs);
                        m_libCanvas.setFontName(m_sFontName);
                        break;
                    case "FSPC":
                        m_dFontSpace = m_libCmn.StrToDouble(ary[1]);
                        break;
                    case "STRG":
                        sp = trnsLen(m_dFontSpace);
                        str = analyzeString(ary[3]);
                        dx = m_libCmn.StrToDouble(ary[1]);
                        dy = m_libCmn.StrToDouble(ary[2]);
                        sx = trnsX(dx);
                        sy = trnsY(dy);
                        if (m_nTateGaki == 0)
                        {
                            m_libCanvas.drawYokoStrg(canvas, sx, sy, sp, m_dFontTime, str);
                        }
                        else 
                        {
                            m_libCanvas.drawTateStrg(canvas, sx, sy, sp, m_dFontTime, str);
                        }
                        break;
                    case "RSTR":
                        sp = trnsLen(m_dFontSpace);
                        fs = trnsLen(m_dFontSize);
                        str = analyzeString(ary[3]);
                        dx = m_libCmn.StrToDouble(ary[1]);
                        dy = m_libCmn.StrToDouble(ary[2]);
                        if (m_nTateGaki == 0)
                        {
                            sx = trnsX(dx) - fs * m_dFontTime * str.Length -  - sp * m_dFontTime * (str.Length-1);
                            sy = trnsY(dy);
                            m_libCanvas.drawYokoStrg(canvas, sx, sy, sp, m_dFontTime, str);
                        }
                        else
                        {
                            sx = trnsX(dx);
                            sy = trnsY(dy) - fs * m_dFontTime * str.Length -  - sp * m_dFontTime * (str.Length-1);
                            m_libCanvas.drawTateStrg(canvas, sx, sy, sp, m_dFontTime, str);
                        }
                        break;
                }
            }
        }
        private string analyzeString(string sSrcStr)
        {
            string sDstStr;
            string[] ary;
            int max, idx;
            string sStr;

            sDstStr = "";
            ary = sSrcStr.Split('%');
            max = ary.Length;
            for (idx = 0; idx < max; idx++)
            {
                if ((idx % 2) == 1)
                {
                    sStr = analyzeCmdStr(ary[idx]);
                    sDstStr = sDstStr + sStr;
                }
                else
                {
                    sDstStr = sDstStr + ary[idx];
                }
            }
            return (sDstStr);
        }
        private string analyzeCmdStr(string sCmdStr)
        {
            string sDstStr;

            sDstStr = "";
            switch (sCmdStr) 
            {
                case "POST3":
                    sDstStr = m_sPost3;
                    break;
                case "POST4":
                    sDstStr = m_sPost4;
                    break;
                case "ADRS1":
                    sDstStr = getAryString(m_arySeikyusaki,7);
                    break;
                case "ADRS2":
                    sDstStr = getAryString(m_arySeikyusaki,8);
                    break;
                case "SEIKYU":
                    sDstStr = getAryString(m_arySeikyusaki,2);
                    break;
                case "BUSYO":
                    sDstStr = getAryString(m_arySeikyusaki,3);
                    break;
                case "AKEISYOU":
                    sDstStr = getAryString(m_arySeikyusaki,5);
                    break;

            }
            return (sDstStr);
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
