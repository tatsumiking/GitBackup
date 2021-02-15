using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Printing;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace instr
{
    public partial class MainWindow : Window
    {
        private Canvas m_Canvas;
        private double m_dBaseX, m_dBaseY;
        private double m_dPrnTime;
        private int m_nTate;
        private double m_dLineSpace;
        private double m_dFS;
        private double m_dFSOrg;

        private void printMain()
        {

            PrintDialog dPrt;
            PageImageableArea area;
            FixedDocument doc;
            double dWidth, dHeight;

            dPrt = new PrintDialog();
            if (dPrt.ShowDialog() == true)
            {
                area = dPrt.PrintQueue.GetPrintCapabilities().PageImageableArea;
                dWidth = area.OriginWidth + area.ExtentWidth;
                dHeight = area.OriginHeight + area.ExtentHeight;

                doc = new FixedDocument();
                printDocumentCraete(doc, dWidth, dHeight);

                dPrt.PrintDocument(doc.DocumentPaginator, "枠入力リスト");
            }
        }
        private void printDocumentCraete(FixedDocument doc, double dWidth, double dHeight)
        {
            double dPageTopX, dPageTopY, dPageEndX, dPageEndY;
            double dLineSize;
            double[] dArySX;
            string sData;
            string[] sLine;
            int nPageMax, nPageIdx;
            int nLineMax, nLineLest;
            Canvas canvas;
            FixedPage page;
            PageContent content;
            double sx, sy, ex, ey;
            int idx, i;
            string[] sAry;

            dPageTopX = 10.0;
            dPageTopY = 10.0;
            dPageEndX = 200.0;
            dPageEndY = 287.0;
            dLineSize = 5.0;
            dArySX = new double[] { 10.0, 5.0, 24.0, 18.0, 5.0, 32.0, 32.0, 32.0, 32.0 };

            sData = getDataGridDataToPrintString();
            sLine = sData.Split('\n');
            nLineMax = (int)((dPageEndY - dPageTopY - dLineSize) / dLineSize);
            nPageMax = sLine.Length / nLineMax;
            nLineLest = sLine.Length % nLineMax;
            if (nLineLest != 0)
            {
                nPageMax = nPageMax + 1;
            }
            idx = 0;
            for (nPageIdx = 0; nPageIdx < nPageMax; nPageIdx++)
            {
                canvas = new Canvas();
                canvas.Width = dWidth;
                canvas.Height = dHeight;
                initPrintCanvasElement(canvas);
                setPrintBase(0.0, 0.0);
                sx = dPageTopX;
                sy = dPageTopY;
                sx = dPageTopX;
                ex = sx + dArySX[0] + dArySX[1] + dArySX[2] + dArySX[3] + dArySX[4];
                ex = ex + dArySX[5] + dArySX[6] + dArySX[7] + dArySX[8];
                setPrintLine(sx, sy, ex, sy);
                printingPageHead(sx, sy, dArySX, dLineSize);
                sy = sy + dLineSize;
                for (i = 0; i < nLineMax; i++, idx++)
                {
                    if (sLine.Length <= idx)
                    {
                        break;
                    }
                    sAry = sLine[idx].Split(',');
                    if (sAry.Length < 12)
                    {
                        break;
                    }
                    printingLineData(sx, sy, dArySX, dLineSize, sAry);
                    ex = sx + dArySX[0] + dArySX[1] + dArySX[2] + dArySX[3] + dArySX[4];
                    ex = ex + dArySX[5] + dArySX[6] + dArySX[7] + dArySX[8];
                    setPrintLine(sx, sy, ex, sy);
                    sy = sy + dLineSize;
                }
                sx = dPageTopX;
                ex = sx + dArySX[0] + dArySX[1] + dArySX[2] + dArySX[3] + dArySX[4];
                ex = ex + dArySX[5] + dArySX[6] + dArySX[7] + dArySX[8];
                setPrintLine(sx, sy, ex, sy);
                ey = sy;
                sy = dPageTopY;
                setPrintLine(sx, sy, sx, ey);
                sx = sx + dArySX[0];
                setPrintLine(sx, sy, sx, ey);
                sx = sx + dArySX[1];
                setPrintLine(sx, sy, sx, ey);
                sx = sx + dArySX[2];
                setPrintLine(sx, sy, sx, ey);
                sx = sx + dArySX[3];
                setPrintLine(sx, sy, sx, ey);
                sx = sx + dArySX[4];
                setPrintLine(sx, sy, sx, ey);
                sx = sx + dArySX[5];
                setPrintLine(sx, sy, sx, ey);
                sx = sx + dArySX[6];
                setPrintLine(sx, sy, sx, ey);
                sx = sx + dArySX[7];
                setPrintLine(sx, sy, sx, ey);
                sx = sx + dArySX[8];
                setPrintLine(sx, sy, sx, ey);

                page = new FixedPage();
                page.Children.Add(canvas);
                content = new PageContent();
                content.Child = page;
                doc.Pages.Add(content);
            }
        }
        private string getDataGridDataToPrintString()
        {
            string sData;
            int max, idx, no;
            DataRow rowItem;

            sData = "";
            max = m_dt.Rows.Count;
            for (idx = 0; idx < max; idx++)
            {
                rowItem = m_dt.Rows[idx];
                no = idx + 1;
                sData = sData + no.ToString("000");
                if (rowItem["col_chk"].ToString() == "True")
                {
                    sData = sData + ",＊";
                }
                else
                {
                    sData = sData + ",";
                }
                sData = sData + "," + rowItem["col_wakuname"].ToString();
                sData = sData + "," + rowItem["col_souke"].ToString();
                sData = sData + "," + rowItem["col_count"].ToString();
                if (rowItem["col_waku1"].ToString() == "CSV出力")
                {
                    sData = sData + ",";
                }
                else
                {
                    sData = sData + "," + rowItem["col_waku1"].ToString();
                }
                if (rowItem["col_waku2"].ToString() == "CSV出力")
                {
                    sData = sData + ",";
                }
                else
                {
                    sData = sData + "," + rowItem["col_waku2"].ToString();
                }
                if (rowItem["col_waku3"].ToString() == "CSV出力")
                {
                    sData = sData + ",";
                }
                else
                {
                    sData = sData + "," + rowItem["col_waku3"].ToString();
                }
                if (rowItem["col_waku4"].ToString() == "CSV出力")
                {
                    sData = sData + ",";
                }
                else
                {
                    sData = sData + "," + rowItem["col_waku4"].ToString();
                }
                if (rowItem["col_waku5"].ToString() == "CSV出力")
                {
                    sData = sData + ",";
                }
                else
                {
                    sData = sData + "," + rowItem["col_waku5"].ToString();
                }
                if (rowItem["col_waku6"].ToString() == "CSV出力")
                {
                    sData = sData + ",";
                }
                else
                {
                    sData = sData + "," + rowItem["col_waku6"].ToString();
                }
                if (rowItem["col_waku7"].ToString() == "CSV出力")
                {
                    sData = sData + ",";
                }
                else
                {
                    sData = sData + "," + rowItem["col_waku7"].ToString();
                }
                if (rowItem["col_waku8"].ToString() == "CSV出力")
                {
                    sData = sData + ",";
                }
                else
                {
                    sData = sData + "," + rowItem["col_waku8"].ToString();
                }
                sData = sData + ",\n";
            }

            return (sData);
        }
        private void printingPageHead(double sx, double sy, double[] dArySX, double dLineSize)
        {
            double dFSize;
            double ex, ey;

            dFSize = dLineSize - 1.4;
            setPrintFont(dFSize, dFSize, "ＭＳ ゴシック", 0);
            ey = sy + dLineSize;

            ex = sx + dArySX[0];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, "No");
            sx = ex;
            ex = sx + dArySX[1];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, "印");
            sx = ex;
            ex = sx + dArySX[2];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, "枠名");
            sx = ex;
            ex = sx + dArySX[3];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, "葬家");
            sx = ex;
            ex = sx + dArySX[4];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, "枚");
            sx = ex;
            ex = sx + dArySX[5];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, "１行目/５行目");
            sx = ex;
            ex = sx + dArySX[6];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, "２行目/６行目");
            sx = ex;
            ex = sx + dArySX[7];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, "３行目/７行目");
            sx = ex;
            ex = sx + dArySX[8];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, "４行目/８行目");
        }
        private void printingLineData(double sx, double sy, double[] dArySX, double dLineSize, string[] sAry)
        {
            double dFSize;
            double ex, ey;

            dFSize = dLineSize - 1.4;
            setPrintFont(dFSize, dFSize, "ＭＳ ゴシック", 0);

            ey = sy + dLineSize;

            ex = sx + dArySX[0];
            setPrintBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, sAry[0]);
            sx = ex;
            ex = sx + dArySX[1];
            setPrintBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, sAry[1]);
            sx = ex;
            ex = sx + dArySX[2];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, sAry[2]);
            sx = ex;
            ex = sx + dArySX[3];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, sAry[3]);
            sx = ex;
            ex = sx + dArySX[4];
            setPrintCBoxStrg(sx + 0.5, sy + 0.7, ex - 0.5, ey - 0.7, sAry[4]);

            dFSize = (dLineSize - 0.2) / 2;
            setPrintFont(dFSize, dFSize, "ＭＳ ゴシック", 0);
            sx = ex;
            ex = sx + dArySX[5];
            setPrintBoxStrg(sx + 1.5, sy + 0.1, ex - 0.5, ey - dLineSize / 2, sAry[5]);
            setPrintBoxStrg(sx + 1.5, sy + dLineSize / 2, ex - 0.5, ey - 0.1, sAry[9]);
            sx = ex;
            ex = sx + dArySX[6];
            setPrintBoxStrg(sx + 1.5, sy + 0.1, ex - 0.5, ey - dLineSize / 2, sAry[6]);
            setPrintBoxStrg(sx + 1.5, sy + dLineSize / 2, ex - 0.5, ey - 0.1, sAry[10]);
            sx = ex;
            ex = sx + dArySX[7];
            setPrintBoxStrg(sx + 1.5, sy + 0.1, ex - 0.5, ey - dLineSize / 2, sAry[7]);
            setPrintBoxStrg(sx + 1.5, sy + dLineSize / 2, ex - 0.5, ey - 0.1, sAry[11]);
            sx = ex;
            ex = sx + dArySX[8];
            setPrintBoxStrg(sx + 1.5, sy + 0.1, ex - 0.5, ey - dLineSize / 2, sAry[8]);
            setPrintBoxStrg(sx + 1.5, sy + dLineSize / 2, ex - 0.5, ey - 0.1, sAry[12]);
        }
        public void initPrintCanvasElement(Canvas canvas)
        {
            double timex, timey;

            m_Canvas = canvas;
            timex = m_Canvas.Width / 210.0;
            timey = m_Canvas.Height / 297.0;
            if (timex < timey)
            {
                m_dPrnTime = timex;
            }
            else
            {
                m_dPrnTime = timey;
            }
            m_dLineSpace = 5.0 * m_dPrnTime;
            m_libCanvas.setBrushFill(-1,-1,-1);
            m_libCanvas.setBrushStroke(0,0,0,1,0);
            m_libCanvas.setBrushText(0,0,0);
        }
        private void setPrintBase(double sx, double sy)
        {
            m_dBaseX = sx * m_dPrnTime;
            m_dBaseY = sy * m_dPrnTime;
        }
        private void setPrintFont(double hi, double wd, string sFontName, int nTate) 
        {
            m_nTate = nTate;
            m_libCanvas.setFontName(sFontName);
            m_libCanvas.setFontNomal();
            if (m_nTate == 0)
            {
                m_dFSOrg = hi;
                hi = hi * m_dPrnTime;
                m_libCanvas.setFontSize(hi);
                m_dFS = hi;
            }
            else
            {
                m_dFSOrg = wd;
                wd = wd * m_dPrnTime;
                m_libCanvas.setFontSize(wd);
                m_dFS = wd;
            }
        }
        private void setPrintBoldFont(double hi, double wd, string sFontName, int nTate)
        {
            m_nTate = nTate;
            m_libCanvas.setFontName(sFontName);
            m_libCanvas.setFontBold();
            if (m_nTate == 0)
            {
                m_dFSOrg = hi;
                hi = hi * m_dPrnTime;
                m_libCanvas.setFontSize(hi);
                m_dFS = hi;
            }
            else
            {
                m_dFSOrg = wd;
                wd = wd * m_dPrnTime;
                m_libCanvas.setFontSize(wd);
                m_dFS = wd;
            }
        }
        private void setPrintBrush(int r, int g, int b)
        {
            m_libCanvas.setBrushFill(r, g, b);
        }
        private void setPrintCPen(int r, int g, int b, double thick, int type)
        {
            thick = thick * m_dPrnTime;
            m_libCanvas.setBrushStroke(r, g, b, thick, type);
        }
        private void setPrintTextColor(int r, int g, int b)
        {
            m_libCanvas.setBrushText(r, g, b);
        }
        private void setPrintStrg(double sx, double sy, string sStr)
        {
            sx = sx * m_dPrnTime + m_dBaseX;
            sy = sy * m_dPrnTime + m_dBaseY;
            if (m_nTate == 0)
            {
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
            else
            {
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
        }
        private void setPrintRStrg(double sx, double sy, string sStr)
        {
            double len;
            sx = sx * m_dPrnTime + m_dBaseX;
            sy = sy * m_dPrnTime + m_dBaseY;
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                sx = sx - len;
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
            else
            {
                sy = sy - len;
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
        }
        private void setPrintCStrg(double sx, double sy, string sStr)
        {
            double len;
            sx = sx * m_dPrnTime + m_dBaseX;
            sy = sy * m_dPrnTime + m_dBaseY;
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                sx = sx - len / 2;
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
            else
            {
                sy = sy - len / 2;
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
        }
        private void setPrintBoxStrg(double sx, double sy, double ex, double ey, string sStr)
        {
            double len, time;
            sx = sx * m_dPrnTime + m_dBaseX;
            sy = sy * m_dPrnTime + m_dBaseY;
            ex = ex * m_dPrnTime + m_dBaseX;
            ey = ey * m_dPrnTime + m_dBaseY;
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                if(len < (ex-sx))
                {
                    time = 1.0;
                }else{
                    time = (ex - sx) / len;
                }
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
            else
            {
                if (len < (ey - sy))
                {
                    time = 1.0;
                }
                else
                {
                    time = (ey - sy) / len;
                }
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
        }
        private void setPrintRBoxStrg(double sx, double sy, double ex, double ey, string sStr)
        {
            double len, time;
            sx = sx * m_dPrnTime + m_dBaseX;
            sy = sy * m_dPrnTime + m_dBaseY;
            ex = ex * m_dPrnTime + m_dBaseX;
            ey = ey * m_dPrnTime + m_dBaseY;
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                if (len < (ex - sx))
                {
                    time = 1.0;
                    sx = ex - len;
                }
                else
                {
                    time = (ex - sx) / len;
                }
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
            else
            {
                if (len < (ey - sy))
                {
                    time = 1.0;
                    sy = ey - len;
                }
                else
                {
                    time = (ey - sy) / len;
                }
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
        }
        private void setPrintCBoxStrg(double sx, double sy, double ex, double ey, string sStr)
        {
            double len, time;
            sx = sx * m_dPrnTime + m_dBaseX;
            sy = sy * m_dPrnTime + m_dBaseY;
            ex = ex * m_dPrnTime + m_dBaseX;
            ey = ey * m_dPrnTime + m_dBaseY;
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                if (len < (ex - sx))
                {
                    time = 1.0;
                    sx = (sx + ex - len)/2;
                }
                else
                {
                    time = (ex - sx) / len;
                }
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
            else
            {
                if (len < (ey - sy))
                {
                    time = 1.0;
                    sy = (sy + ey - len)/2;
                }
                else
                {
                    time = (ey - sy) / len;
                }
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
        }
        private void setPrintEBoxStrg(double sx, double sy, double ex, double ey, string sStr)
        {
            double len, space, time;
            sx = sx * m_dPrnTime + m_dBaseX;
            sy = sy * m_dPrnTime + m_dBaseY;
            ex = ex * m_dPrnTime + m_dBaseX;
            ey = ey * m_dPrnTime + m_dBaseY;
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                if (len < (ex - sx))
                {
                    time = 1.0;
                    space = (ex - sx) - len / (sStr.Length-1);
                }
                else
                {
                    time = (ex - sx) / len;
                }
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
            else
            {
                if (len < (ey - sy))
                {
                    time = 1.0;
                    space = (ey - sy) - len / (sStr.Length - 1);
                }
                else
                {
                    time = (ey - sy) / len;
                }
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
        }
        private void setPrintLine(double sx, double sy, double ex, double ey)
        {
            sx = sx * m_dPrnTime + m_dBaseX;
            sy = sy * m_dPrnTime + m_dBaseY;
            ex = ex * m_dPrnTime + m_dBaseX;
            ey = ey * m_dPrnTime + m_dBaseY;
            m_libCanvas.drawLine(m_Canvas, sx, sy, ex, ey);
        }
        private void setPrintBoxs(double sx, double sy, double ex, double ey)
        {
            double wd, hi;

            sx = sx * m_dPrnTime + m_dBaseX;
            sy = sy * m_dPrnTime + m_dBaseY;
            ex = ex * m_dPrnTime + m_dBaseX;
            ey = ey * m_dPrnTime + m_dBaseY;
            wd = ex - sx;
            hi = ey - sy;
            m_libCanvas.drawBoxs(m_Canvas, sx, sy, wd, hi);
        }
    }
}
