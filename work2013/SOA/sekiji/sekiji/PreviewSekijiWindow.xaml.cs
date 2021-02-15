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

namespace sekiji
{
    /// <summary>
    /// PreviewSekijiWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class PreviewSekijiWindow : Window
    {
        private MainWindow m_wndMain;
        private ClsHaiseki m_clsHaiseki;
        private int m_nTableSeetCount;
        private string m_sExecPath;
        private LibCommon m_libCmn;
        private LibCanvas m_libCanvas;
        private double m_dXSize, m_dYSize;
        private double m_dTime;

        public PreviewSekijiWindow()
        {
            InitializeComponent();
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wndMain = wnd;
            m_clsHaiseki = m_wndMain.m_clsHaiseki;
            m_libCmn = m_wndMain.m_libCmn;
            m_libCanvas = m_wndMain.m_libCanvas;
            m_sExecPath = m_wndMain.m_sExecPath;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            double timex, timey;

            m_dXSize = cnvsPreview.ActualWidth;
            m_dYSize = cnvsPreview.ActualHeight;
            timex = m_dXSize / Constants.A3WIDTH;
            timey = m_dYSize / Constants.A3HEIGHT;
            if (timex < timey)
            {
                m_dTime = timex;
                m_dYSize = Constants.A3HEIGHT * m_dTime;
            }
            else
            {
                m_dTime = timey;
                m_dXSize = Constants.A3WIDTH * m_dTime;
            }
            printOutCanvas(cnvsPreview);
        }
        private void btnRet_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void btnPrint_Click(object sender, RoutedEventArgs e)
        {
            printOutMain();
            this.Close();
        }
        private void printOutMain()
        {
            double timex, timey;
            PrintDialog dPrt;
            PageImageableArea area;
            Canvas canvas;
            FixedPage page;
            PageContent cont;
            FixedDocument doc;

            if (m_clsHaiseki.m_nTableBlockCount == 0)
            {
                MessageBox.Show("灰寄せ会場設定されていません。", "確認", MessageBoxButton.OK);
                return;
            }

            LocalPrintServer lps = new LocalPrintServer();
            PrintQueue queue = lps.DefaultPrintQueue;

            PrintTicket ticket = queue.DefaultPrintTicket;
            ticket.PageMediaSize = new PageMediaSize(PageMediaSizeName.ISOA3);
            ticket.PageOrientation = PageOrientation.Landscape;

            dPrt = new PrintDialog();
            dPrt.PrintQueue = queue;
            dPrt.PrintTicket = ticket;
            if (dPrt.ShowDialog() == true)
            {
                area = dPrt.PrintQueue.GetPrintCapabilities(dPrt.PrintTicket).PageImageableArea;
                m_dXSize = (area.OriginWidth + area.ExtentWidth);
                m_dYSize = (area.OriginHeight + area.ExtentHeight);
                timex = m_dXSize / Constants.A3WIDTH;
                timey = m_dYSize / Constants.A3HEIGHT;
                if (timex < timey)
                {
                    m_dTime = timex;
                    m_dYSize = Constants.A3HEIGHT * m_dTime;
                }
                else
                {
                    m_dTime = timey;
                    m_dXSize = Constants.A3WIDTH * m_dTime;
                }

                canvas = new Canvas();
                canvas.Width = area.OriginWidth + area.ExtentWidth;
                canvas.Height = area.OriginHeight + area.ExtentHeight;

                doc = new FixedDocument();
                var PageSize = new Size(canvas.Width, canvas.Height);
                page = new FixedPage();
                page.Children.Add(canvas);
                page.Measure(PageSize);
                page.UpdateLayout();
                page.Width = canvas.Width;
                page.Height = canvas.Height;
                cont = new PageContent();
                cont.Child = page;
                doc.Pages.Add(cont);

                printOutCanvas(canvas);

                dPrt.PrintDocument(doc.DocumentPaginator, "灰寄せ会場座席表");
            }
        }
        private void printOutCanvas(Canvas cnvsMain)
        {
            double m_dTopX, m_dTopY, m_dEndX, m_dEndY;
            double m_dPageX, m_dPageY;
            double wd, hi;
            double numx, numy;
            double m_dPageSpace, m_dSpace;
            double dLineHeight, dTableWidth;
            double dLineCount;
            double m_dLineHeight;
            double m_dSouryoWidth;
            double m_dTableWidth;
            double dTableSumWidth;
            double dTableTopX;
            int max, tblblkmax, tblmax;
            int clm, tblblk, tbl;
            int line, no;
            double sx, sy, ex, ey;
            double tsx, tsy;
            double sx1, ex1, sx2, ex2;
            double titlefontsize;
            double fontsize;
            string sName;
            string str;
            double len;
            TextBlock tblk;
            Canvas cnvs;

            m_dPageSpace = 10.0 * m_dTime;
            m_dTopX = (int)(m_dPageSpace);
            m_dTopY = (int)(m_dPageSpace);
            m_dEndX = m_dXSize - m_dPageSpace;
            m_dEndY = m_dYSize - m_dPageSpace;
            m_dPageX = m_dEndX - m_dTopX;
            m_dPageY = m_dEndY - m_dTopY;

            m_dSpace = 2.0 * m_dTime;
            m_nTableSeetCount = getTableSeetCount();
            dLineCount = m_nTableSeetCount + 6;
            m_dLineHeight = ((m_dEndY - m_dTopY) / dLineCount);
            dLineHeight = m_dLineHeight;
            tblmax = m_clsHaiseki.m_nTableBlockCount * 2;
            m_dTableWidth = (m_dPageX - (tblmax * m_dSpace - m_dSpace)) / (double)tblmax;

            max = m_clsHaiseki.m_nSouryoCount;
            if (m_dPageX < (m_dTableWidth + m_dLineHeight) * max)
            {
                m_dSouryoWidth = (int)(m_dPageX / max);
                if (m_dTableWidth > m_dSouryoWidth)
                {
                    m_dTableWidth = m_dSouryoWidth;
                }
            }
            else
            {
                m_dSouryoWidth = m_dTableWidth + m_dLineHeight;
            }

            dTableWidth = m_dTableWidth;
            numx = (dTableWidth / 7);
            numy = (dLineHeight * 0.7);
            if (numx < numy)
            {
                fontsize = numx;
            }
            else
            {
                fontsize = numy;
            }

            m_libCanvas.setBackgroundBrush(Brushes.White);
            m_libCanvas.setTextBrush(Brushes.Black);

            titlefontsize = (int)(dLineHeight * 1.4);
            if (m_dPageY < titlefontsize * 20)
            {
                titlefontsize = m_dPageY / 20;
            }
            str = m_clsHaiseki.m_sSoukeName + "家　" + m_clsHaiseki.m_sTitle;
            len = (str.Length * titlefontsize);
            sx = (m_dTopX + (m_dPageX - len) / 2);
            sy = m_dTopY + dLineHeight * 0.3;
            m_libCanvas.setFontSize(titlefontsize);
            m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);

            m_libCanvas.setFontSize(fontsize);
            m_libCanvas.setStrokeBrush(Brushes.Black);
            m_libCanvas.setBackgroundBrush(Brushes.White);
            m_libCanvas.setTextBrush(Brushes.Black);

            if (m_clsHaiseki.m_nReizenAlignment == 0)
            {
                sx = (m_dEndX - m_dTableWidth);
                ex = m_dEndX;
            }
            else
            {
                sx = (m_dTopX + m_dEndX - m_dTableWidth) / 2;
                ex = (sx + m_dTableWidth);
            }
            sy = (m_dTopY + m_dLineHeight * 1.6);
            ey = (sy + m_dLineHeight);

            tsx = sx;
            tsy = sy;
            wd = m_dTableWidth;
            hi = m_dLineHeight;
            cnvs = m_libCanvas.drawCanvasCenterString(cnvsMain, tsx, tsy, wd, hi, m_clsHaiseki.m_sReizenText);
            m_libCanvas.drawBoxs(cnvsMain, sx, sy, wd, hi);

            // 僧侶席エリア
            max = m_clsHaiseki.m_nSouryoCount;
            hi = m_dLineHeight;
            sx = (m_dTopX + (m_dPageX - m_dSouryoWidth * max) / 2);
            sy = (m_dTopY + dLineHeight * 2.8);
            for (clm = 0; clm < max; clm++)
            {
                ex = sx + m_dSouryoWidth;
                ey = sy + m_dLineHeight;
                wd = m_dSouryoWidth;
                sName = m_clsHaiseki.m_lstSouryo[clm].m_sName;
                if (sName != "")
                {
                    sName = m_libCmn.setSamaNameSpaceing(sName);
                    cnvs = m_libCanvas.drawCanvasCenterString(cnvsMain, sx, sy, wd, hi, sName);
                }
                m_libCanvas.drawBoxs(cnvsMain, sx, sy, ex - sx, ey - sy);
                sx = ex;
            }
            // テーブルエリア
            tblblkmax = m_clsHaiseki.m_nTableBlockCount;
            dTableSumWidth = (m_dTableWidth + m_dSpace) * tblblkmax * 2 - m_dSpace;
            if ((m_dEndX - m_dTopX) > dTableSumWidth)
            {
                dTableTopX = (m_dTopX + m_dEndX - dTableSumWidth) / 2;
            }
            else
            {
                dTableTopX = m_dTopX;
            }
            for (tblblk = 0; tblblk < tblblkmax; tblblk++)
            {
                sy = (m_dTopY + m_dLineHeight * 4);
                sx1 = (dTableTopX + (m_dTableWidth + m_dSpace) * tblblk * 2 + m_dSpace / 2);
                sx2 = (sx1 + m_dTableWidth);
                ex1 = sx1 + m_dTableWidth;
                ex2 = sx2 + m_dTableWidth;

                tbl = tblblk * 2;
                for (line = 0; line < m_nTableSeetCount; line++)
                {
                    ey = sy + m_dLineHeight;
                    wd = m_dTableWidth;
                    sName = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_sName;
                    if (sName != "")
                    {
                        sName = m_libCmn.setSamaNameSpaceing(sName);
                        cnvs = m_libCanvas.drawCanvasCenterString(cnvsMain, sx1, sy, wd, hi, sName);
                    }
                    sy = ey;
                }
                sy = (m_dTopY + m_dLineHeight * 4);
                tbl = tblblk * 2 + 1;
                for (line = 0; line < m_nTableSeetCount; line++)
                {
                    ey = sy + m_dLineHeight;
                    wd = m_dTableWidth;
                    sName = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_sName;
                    if (sName != "")
                    {
                        sName = m_libCmn.setSamaNameSpaceing(sName);
                        cnvs = m_libCanvas.drawCanvasCenterString(cnvsMain, sx2, sy, wd, hi, sName);
                    }
                    sy = ey;
                }
                sy = (m_dTopY + m_dLineHeight * 4);
                m_libCanvas.drawLine(cnvsMain, sx1, sy, ex2, sy);
                for (line = 0; line < m_nTableSeetCount; line++)
                {
                    no = line + 1;
                    ey = sy + m_dLineHeight;
                    m_libCanvas.drawLine(cnvsMain, sx1, sy, sx1, ey);
                    m_libCanvas.drawLine(cnvsMain, sx2, sy, sx2, ey);
                    m_libCanvas.drawLine(cnvsMain, ex2, sy, ex2, ey);
                    m_libCanvas.drawLine(cnvsMain, sx1, ey, ex2, ey);
                    sy = ey;
                }
            }

            str = m_clsHaiseki.m_sRightBottomText;
            len = str.Length;
            sx = (m_dXSize - (fontsize * len + m_dSpace));
            sy = (m_dTopY + m_dLineHeight * (4.5 + m_nTableSeetCount));
            m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);

            //m_libCanvas.drawBoxs(cnvsMain, m_dTopX, m_dTopY, m_dEndX - m_dTopX, m_dEndY - m_dTopY);
            //m_libCanvas.drawBoxs(cnvsMain, 1, 1, m_dXSize - 2, m_dYSize - 2);
        }
        private int getTableSeetCount()
        {
            int tblmax, tbl;
            int seetmax, seet;
            string sName;

            seetmax = m_clsHaiseki.m_nSeetCount;
            tblmax = m_clsHaiseki.m_nTableBlockCount * 2;
            // テーブルエリア
            for (seet = seetmax - 1; seet >= 0; seet--)
            {
                for (tbl = 0; tbl < tblmax; tbl++)
                {
                    sName = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].m_sName;
                    if (sName != "")
                    {
                        return (seet + 1);
                    }
                }

            }
            return (m_clsHaiseki.m_nSeetCount);
        }
    }
}
