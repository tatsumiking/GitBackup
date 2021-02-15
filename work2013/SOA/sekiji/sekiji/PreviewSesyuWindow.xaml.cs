﻿using System;
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
    /// PreviewSesyuWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class PreviewSesyuWindow : Window
    {
        private MainWindow m_wndMain;
        private ClsHaiseki m_clsHaiseki;
        private int m_nTableSeetCount;
        private string m_sExecPath;
        private LibCommon m_libCmn;
        private LibCanvas m_libCanvas;
        private double m_dXSize, m_dYSize;
        private double m_dTime;
        private int[] m_nRType = new int[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        private int m_nRDispCount;
        private int[] m_nHType = new int[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        private int m_nHDispCount;

        public PreviewSesyuWindow()
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
            setRyouriHikidemonoTable();
            printOutCanvas(cnvsPreview);
        }
        private void btnRet_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void btnPrint_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                printOutMain();
            }
            catch (Exception exp)
            {//例外発生
                MessageBox.Show("印刷に失敗しました。\n" + exp.Message, "確認", MessageBoxButton.OK);
            }
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

                setRyouriHikidemonoTable();
                printOutCanvas(canvas);

                dPrt.PrintDocument(doc.DocumentPaginator, "灰寄せ会場座席表");
            }
        }
        private void setRyouriHikidemonoTable()
        {
            int nSet;
            int max, idx;
            int tblmax, tbl;
            int line;
            string sName;

            max = Constants.HIKIDEMONOSETMAX;
            for (idx = 0; idx < max; idx++)
            {
                m_nHType[idx] = 0;
            }
            max = Constants.RYOURISETMAX;
            for (idx = 0; idx < max; idx++)
            {
                m_nRType[idx] = 0;
            }

            nSet = m_clsHaiseki.m_nReizenHikimonoType;
            if (nSet != -1)
            {
                m_nHType[nSet] = m_nHType[nSet] + 1;
            }
            max = m_clsHaiseki.m_nSouryoCount;
            for (idx = 0; idx < max; idx++)
            {
                nSet = m_clsHaiseki.m_lstSouryo[idx].m_nRyouriType;
                if (nSet != -1)
                {
                    m_nRType[nSet] = m_nRType[nSet] + 1;
                }
                nSet = m_clsHaiseki.m_lstSouryo[idx].m_nHikidemonoType;
                if (nSet != -1)
                {
                    m_nHType[nSet] = m_nHType[nSet] + 1;
                }
            }
            m_nTableSeetCount = getTableSeetCount();
            tblmax = m_clsHaiseki.m_nTableBlockCount * 2;
            for (tbl = 0; tbl < tblmax; tbl++)
            {
                for (line = 0; line < m_nTableSeetCount; line++)
                {
                    sName = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_sName;
                    if (sName != "")
                    {
                        nSet = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_nRyouriType;
                        if (nSet != -1)
                        {
                            m_nRType[nSet] = m_nRType[nSet] + 1;
                        }
                        nSet = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_nHikidemonoType;
                        if (nSet != -1)
                        {
                            m_nHType[nSet] = m_nHType[nSet] + 1;
                        }
                    }
                }
            }
            max = Constants.HIKIDEMONOSETMAX;
            m_nHDispCount = 0;
            for (idx = 0; idx < max; idx++)
            {
                if (m_nHType[idx] != 0)
                {
                    m_nHDispCount++;
                }
            }
            m_nRType[7] = 1; // 供養全
            m_nRType[8] = m_clsHaiseki.m_nYudemasu; // ゆでます
            max = Constants.RYOURISETMAX;
            m_nRDispCount = 0;
            for (idx = 0; idx < max; idx++)
            {
                if (m_nRType[idx] != 0)
                {
                    m_nRDispCount++;
                }
            }
        }
        private void printOutCanvas(Canvas cnvsMain)
        {
            double m_dTopX, m_dTopY, m_dEndX, m_dEndY;
            double m_dPageX, m_dPageY;
            double m_dPageSpace, m_dSpace;
            double m_dLineHeight;
            double m_dSouryoWidth;
            double m_dTableWidth;
            double dSumSpace;
            double wd, hi;
            double numx, numy;
            double dLineHeight, dTableWidth;
            double dLineCount;
            double nHikidemonoWidth;
            double dTableSumWidth;
            double dTableTopX;
            int nSekisu = 0;       
            int nSet;
            Brush brush;
            int max, tblblkmax, tblmax;
            int clm, tblblk, tbl;
            int line, no;
            int hk, hkdsp, hkmax;
            double sx, sy, cx, cy, ex, ey;
            double tsx, tsy, tex, tey;
            double sx1, ex1, sx2, ex2;
            double titlefontsize;
            double titlespace;
            double fontsize, fsymv;
            double hkfontsize;
            string sName;
            string str;
            int len, cnt;
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
            hkfontsize = m_dPageX / (18 * 4);
            m_nTableSeetCount = getTableSeetCount();
            if (m_clsHaiseki.m_nSouryoCount == 0)
            {
                dSumSpace = m_dSpace * 3.0;
                dLineCount = m_nTableSeetCount + 3;
            }
            else
            {
                dSumSpace = m_dSpace * 4.0;
                dLineCount = m_nTableSeetCount + 4;
            }
            if (3 < m_nHDispCount)
            {
                // line = 12;
                line = 14;
            }
            else
            {
                //line = 6;
                line = 7;
                if (7 < m_nRDispCount)
                {
                    line = line + m_nRDispCount - 7;
                }
            }
            dLineHeight = ((m_dPageY - hkfontsize * (line) - dSumSpace) / dLineCount);
            titlespace = dLineHeight * 2;
            titlefontsize = m_dPageX / 20;
            if (titlefontsize > (dLineHeight * 1.6))
            {
                titlefontsize = dLineHeight * 1.6;
            }
            else
            {
                titlespace = titlefontsize + m_dSpace * 2.0;
                if (m_clsHaiseki.m_nSouryoCount == 0)
                {
                    dSumSpace = titlespace + m_dSpace * 3.0;
                    dLineCount = m_nTableSeetCount + 1;
                }
                else
                {
                    dSumSpace = titlespace + m_dSpace * 4.0;
                    dLineCount = m_nTableSeetCount + 2;
                }
                dLineHeight = ((m_dPageY - hkfontsize * (line) - dSumSpace) / dLineCount);
            }
            m_dLineHeight = dLineHeight;

            tblmax = m_clsHaiseki.m_nTableBlockCount * 2;
            dTableWidth = ((m_dPageX - (tblmax * m_dSpace - m_dSpace)) / (double)tblmax);
            max = m_clsHaiseki.m_nSouryoCount;
            if (max < 5)
            {
                max = 5; // サイズ算出時のため
            }
            if (m_dPageX < (dTableWidth + m_dLineHeight) * max)
            {
                m_dSouryoWidth = (m_dPageX / max);
                if (dTableWidth > m_dSouryoWidth)
                {
                    dTableWidth = m_dSouryoWidth;
                }
            }
            else
            {
                m_dSouryoWidth = dTableWidth + m_dLineHeight;
            }
            m_dTableWidth = dTableWidth;

            numx = (dTableWidth / 7.4);
            numy = (dLineHeight * 0.75);
            if (numx < numy)
            {
                fontsize = numx;
            }
            else
            {
                fontsize = numy;
            }
            fsymv = fontsize * 0.1;
            fsymv = 0;

            str = m_clsHaiseki.m_sSoukeName + "家　" + m_clsHaiseki.m_sTitle;
            len = str.Length * (int)titlefontsize;
            sx = (int)(m_dTopX + (m_dPageX - len) / 2);
            sy = (int)(m_dTopY);
            m_libCanvas.setFontSize(titlefontsize);
            m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);

            m_libCanvas.setFontSize(fontsize);
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
            sy = (m_dTopY + titlespace);
            ey = (sy + m_dLineHeight);

            tsx = sx;
            wd = m_dTableWidth;
            tsy = sy;
            hi = m_dLineHeight;
            m_libCanvas.setBackgroundBrush(Brushes.White);
            m_libCanvas.setTextBrush(Brushes.Black);
            cnvs = m_libCanvas.drawCanvasCenterString(cnvsMain, tsx, tsy - fsymv, wd, hi, m_clsHaiseki.m_sReizenText);
            m_libCanvas.drawBoxs(cnvsMain, sx, sy, m_dTableWidth, m_dLineHeight);

            nSet = m_clsHaiseki.m_nReizenHikimonoType;
            if (nSet == -1)
            {
                brush = Brushes.White;
            }
            else
            {
                brush = m_clsHaiseki.m_lstHikidemono[nSet].getBrush();
            }
            cnvs.Background = brush;

            // 僧侶席エリア 開始はdLineHeight*3
            max = m_clsHaiseki.m_nSouryoCount;
            hi = m_dLineHeight;
            sx = m_dTopX + (m_dPageX - m_dSouryoWidth * max) / 2;
            sy = m_dTopY + titlespace + m_dLineHeight * 1.0 + m_dSpace;
            for (clm = 0; clm < max; clm++)
            {
                ex = sx + m_dSouryoWidth;
                ey = sy + m_dLineHeight;
                wd = m_dSouryoWidth;
                sName = m_clsHaiseki.m_lstSouryo[clm].m_sName;
                if (sName != "")
                {
                    nSekisu = nSekisu + 1;
                    sName = m_libCmn.setSamaNameSpaceing(sName);
                    cnvs = m_libCanvas.drawCanvasCenterString(cnvsMain, sx, sy - fsymv, wd, hi, sName);
                    nSet = m_clsHaiseki.m_lstSouryo[clm].m_nRyouriType;
                    brush = m_clsHaiseki.m_lstRyouri[nSet].getBrush();
                    tblk = (TextBlock)cnvs.Children[0];
                    tblk.Foreground = brush;

                    nSet = m_clsHaiseki.m_lstSouryo[clm].m_nHikidemonoType;
                    if (nSet == -1)
                    {
                        brush = Brushes.White;
                    }
                    else
                    {
                        brush = m_clsHaiseki.m_lstHikidemono[nSet].getBrush();
                    }
                    cnvs.Background = brush;
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
                if (m_clsHaiseki.m_nSouryoCount == 0)
                {
                    sy = m_dTopY + titlespace + m_dLineHeight * 1 + m_dSpace;
                }
                else
                {
                    sy = m_dTopY + titlespace + m_dLineHeight * 2 + m_dSpace * 2.0;
                }
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
                        nSekisu = nSekisu + 1;
                        sName = m_libCmn.setSamaNameSpaceing(sName);
                        cnvs = m_libCanvas.drawCanvasCenterString(cnvsMain, sx1 + 2, sy - fsymv, wd - 4, hi, sName);
                        //cnvs = m_libCanvas.drawEStrg(cnvsMain, sx1 + 2, sy, wd - 4, hi, sName);
                        nSet = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_nRyouriType;
                        if(nSet != -1)
                        {
                            brush = m_clsHaiseki.m_lstRyouri[nSet].getBrush();
                            tblk = (TextBlock)cnvs.Children[0];
                            tblk.Foreground = brush;
                        }

                        nSet = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_nHikidemonoType;
                        if (nSet == -1)
                        {
                            brush = Brushes.White;
                        }
                        else
                        {
                            brush = m_clsHaiseki.m_lstHikidemono[nSet].getBrush();
                        }
                        cnvs.Background = brush;
                    }
                    sy = ey;
                }
                if (m_clsHaiseki.m_nSouryoCount == 0)
                {
                    sy = m_dTopY + titlespace + m_dLineHeight * 1 + m_dSpace;
                }
                else
                {
                    sy = m_dTopY + titlespace + m_dLineHeight * 2 + m_dSpace * 2.0;
                }
                tbl = tblblk * 2 + 1;
                for (line = 0; line < m_nTableSeetCount; line++)
                {
                    ey = sy + m_dLineHeight;
                    wd = m_dTableWidth;
                    sName = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_sName;
                    if (sName != "")
                    {
                        nSekisu = nSekisu + 1;
                        sName = m_libCmn.setSamaNameSpaceing(sName);
                        cnvs = m_libCanvas.drawCanvasCenterString(cnvsMain, sx2 + 2, sy - fsymv, wd - 4, hi, sName);
                        //cnvs = m_libCanvas.drawEStrg(cnvsMain, sx2 + 2, sy, wd - 4, hi, sName);
                        nSet = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_nRyouriType;
                        if (nSet != -1)
                        {
                            brush = m_clsHaiseki.m_lstRyouri[nSet].getBrush();
                            tblk = (TextBlock)cnvs.Children[0];
                            tblk.Foreground = brush;
                        }

                        nSet = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_nHikidemonoType;
                        if (nSet == -1)
                        {
                            brush = Brushes.White;
                        }
                        else
                        {
                            brush = m_clsHaiseki.m_lstHikidemono[nSet].getBrush();
                        }
                        cnvs.Background = brush;
                    }
                    sy = ey;
                }
                if (m_clsHaiseki.m_nSouryoCount == 0)
                {
                    sy = m_dTopY + titlespace + m_dLineHeight * 1 + m_dSpace;
                }
                else
                {
                    sy = m_dTopY + titlespace + m_dLineHeight * 2 + m_dSpace * 2.0;
                }
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

            m_libCanvas.setFontSize(hkfontsize);
            m_libCanvas.setTextBrush(Brushes.Black);
            str = m_clsHaiseki.m_sRightBottomText;
            len = str.Length;
            sx = (m_dXSize - (hkfontsize * len + m_dSpace));
            if (m_clsHaiseki.m_nSouryoCount == 0)
            {
                sy = (m_dTopY + titlespace + m_dLineHeight * (1 + m_nTableSeetCount) + m_dSpace * 2.0);
            }
            else
            {
                sy = (m_dTopY + titlespace + m_dLineHeight * (2 + m_nTableSeetCount) + m_dSpace * 3.0);
            }
            m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);

            sx = (m_dTopX);
            if (m_clsHaiseki.m_nSouryoCount == 0)
            {
                sy = (m_dTopY + titlespace + m_dLineHeight * (1 + m_nTableSeetCount) + m_dSpace * 2.0);
            }
            else
            {
                sy = (m_dTopY + titlespace + m_dLineHeight * (2 + m_nTableSeetCount) + m_dSpace * 3.0);
            }
            if (3 < m_nHDispCount)
            {
                sy = sy + hkfontsize + m_dSpace;
            }
            nHikidemonoWidth = (int)(m_dPageX / 4);
            int num = 1 + nSekisu;
            str = "座席数＝" + num + "(" + m_clsHaiseki.m_sReizenText + "含め)";
            m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);

            if (m_nRType[0] != 0)
            {
                sy = sy + hkfontsize;
                str = m_clsHaiseki.m_lstRyouri[0].m_sName + "＝" + m_nRType[0];
                brush = m_clsHaiseki.m_lstRyouri[0].getBrush();
                m_libCanvas.setTextBrush(brush);
                m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);
            }

            if (m_nRType[1] != 0)
            {
                sy = sy + hkfontsize;
                str = m_clsHaiseki.m_lstRyouri[1].m_sName + "＝" + m_nRType[1];
                brush = m_clsHaiseki.m_lstRyouri[1].getBrush();
                m_libCanvas.setTextBrush(brush);
                m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);
            }

            if (m_nRType[2] != 0)
            {
                sy = sy + hkfontsize;
                str = m_clsHaiseki.m_lstRyouri[2].m_sName + "＝" + m_nRType[2];
                brush = m_clsHaiseki.m_lstRyouri[2].getBrush();
                m_libCanvas.setTextBrush(brush);
                m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);
            }

            if (m_nRType[3] != 0)
            {
                sy = sy + hkfontsize;
                str = m_clsHaiseki.m_lstRyouri[3].m_sName + "＝" + m_nRType[3];
                brush = m_clsHaiseki.m_lstRyouri[3].getBrush();
                m_libCanvas.setTextBrush(brush);
                m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);
            }

            if (m_nRType[4] != 0)
            {
                sy = sy + hkfontsize;
                str = m_clsHaiseki.m_lstRyouri[4].m_sName + "＝" + m_nRType[4];
                brush = m_clsHaiseki.m_lstRyouri[4].getBrush();
                m_libCanvas.setTextBrush(brush);
                m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);
            }

            if (m_nRType[5] != 0)
            {
                sy = sy + hkfontsize;
                str = m_clsHaiseki.m_lstRyouri[5].m_sName + "＝" + m_nRType[5];
                brush = m_clsHaiseki.m_lstRyouri[5].getBrush();
                m_libCanvas.setTextBrush(brush);
                m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);
            }
            if (m_clsHaiseki.m_lstRyouri[6].m_sName != "" && m_nRType[6] != 0)
            {
                sy = sy + hkfontsize;
                str = m_clsHaiseki.m_lstRyouri[6].m_sName + "＝" + m_nRType[6];
                brush = m_clsHaiseki.m_lstRyouri[6].getBrush();
                m_libCanvas.setTextBrush(brush);
                m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);
            }

            sy = sy + hkfontsize;
            str = m_clsHaiseki.m_lstRyouri[7].m_sName + "＝1";
            brush = m_clsHaiseki.m_lstRyouri[7].getBrush();
            m_libCanvas.setTextBrush(brush);
            m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);

            if (m_clsHaiseki.m_nYudemasu != 0)
            {
                sy = sy + hkfontsize;
                str = "ゆでます＝" + m_clsHaiseki.m_nYudemasu;
                brush = m_clsHaiseki.m_lstRyouri[8].getBrush();
                m_libCanvas.setTextBrush(brush);
                m_libCanvas.drawStrg(cnvsMain, sx, sy, 0.0, 1.0, str);
            }

            sx = (m_dTopX + nHikidemonoWidth);
            ex = (sx + nHikidemonoWidth);
            if (m_clsHaiseki.m_nSouryoCount == 0)
            {
                sy = (m_dTopY + titlespace + m_dLineHeight * (1 + m_nTableSeetCount) + m_dSpace * 2.0);
            }
            else
            {
                sy = (m_dTopY + titlespace + m_dLineHeight * (2 + m_nTableSeetCount) + m_dSpace * 3.0);
            }
            //sy = sy + hkfontsize * 2.0;
            sy = sy + hkfontsize * 3.0;
            ey = sy + (hkfontsize * Constants.HIKIDEMONOMAX);
            wd = ex - sx;
            hi = ey - sy;
            // 引き出物エリア
            m_libCanvas.setTextBrush(Brushes.Black);
            hkmax = Constants.HIKIDEMONOSETMAX;
            for (hk = 0, hkdsp = 0; hk < hkmax; hk++)
            {
                if (hkdsp == Constants.HIKIDEMONOSETMAX / 2)
                {
                    sx = (m_dTopX + nHikidemonoWidth);
                    //sy = (sy + hkfontsize * (Constants.HIKIDEMONOMAX) + hkfontsize);
                    sy = (sy + hkfontsize * (Constants.HIKIDEMONOMAX) + hkfontsize * 2.0);
                    ey = sy + (hkfontsize * Constants.HIKIDEMONOMAX);
                }
                if (m_nHType[hk] != 0)
                {
                    m_libCanvas.setFontSize(hkfontsize);
                    ex = sx + nHikidemonoWidth;

                    str = m_clsHaiseki.m_lstHikidemono[hk].m_sName + " " + m_nHType[hk] + "セット";
                    cx = (wd / 2.0);
                    tsx = sx + cx - (str.Length * hkfontsize) / 2;
                    //tsy = (sy - hkfontsize);
                    tsy = (sy - hkfontsize * 1.5);
                    m_libCanvas.drawStrg(cnvsMain, tsx, tsy, 0.0, 1.0, str);

                    cnvs = m_libCanvas.CreateCanvas(cnvsMain, sx, sy, ex - sx, ey - sy);
                    cnvs.Background = m_clsHaiseki.m_lstHikidemono[hk].getBrush();

                    m_libCanvas.setFontSize(hkfontsize);
                    tsx = (hkfontsize * 0.2);
                    tsy = (hkfontsize * 0.2);
                    tex = (wd - hkfontsize * 0.2);
                    tey = (hi - hkfontsize * 0.2);
                    m_libCanvas.drawLine(cnvs, tsx, 0, 0, tsy);
                    m_libCanvas.drawLine(cnvs, 0, tsy, 0, tey);
                    m_libCanvas.drawLine(cnvs, 0, tey, tsx, hi);

                    m_libCanvas.drawLine(cnvs, tex, 0, wd, tsy);
                    m_libCanvas.drawLine(cnvs, wd, tsy, wd, tey);
                    m_libCanvas.drawLine(cnvs, wd, tey, tex, hi);

                    cy = hkfontsize;
                    for (line = 0; line < Constants.HIKIDEMONOMAX; line++)
                    {
                        str = m_clsHaiseki.m_lstHikidemono[hk].m_lstSyouhin[line];
                        str = m_libCmn.SubStringByte(str, 18);
                        tblk = m_libCanvas.CreateTextBlock(tsx, cy - hkfontsize, str);
                        cnvs.Children.Add(tblk);
                        m_clsHaiseki.m_lstHikidemono[hk].setTextBlock(line, tblk);
                        m_libCanvas.drawLine(cnvs, tsx, cy, tex, cy);
                        cy = (cy + hkfontsize);
                    }
                    sx = sx + nHikidemonoWidth;
                    hkdsp++;
                }
            }

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