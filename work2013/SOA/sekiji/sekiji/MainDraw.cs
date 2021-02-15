using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace sekiji
{
    public partial class MainWindow : Window
    {
        private double m_dTopX, m_dTopY, m_dEndX, m_dEndY;
        private double m_dPageX, m_dPageY;
        private double m_dPageSpace, m_dNoSpace, m_dSpace;
        private double m_dLineHeight;
        private double m_dSouryoWidth;
        private double m_dTableWidth;

        private void drawTable()
        {
            double xsize, ysize;
            double crtx, crty;
            double wd, hi;
            double numx, numy, time;
            double dLineCount;
            double dWidthSpace;
            int nSekisu = 0;
            int[] nRType = new int[] { 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0};
            int nHikidemonoWidth;
            double dTableSumWidth;
            double dTableTopX;
            int nSet;
            Brush brush;
            int max, tblblkmax, tblmax;
            int clm, tblblk, tbl;
            int line, no;
            int hk, hkmax;
            double sx, sy, cx, cy, ex, ey;
            double tsx, tsy, tex, tey;
            double sx1, tx1, ex1, sx2, tx2, ex2;
            double fontsize;
            string sName;
            string str;
            double len;
            Canvas cnvsReizen;
            Canvas cnvsSekiji;
            TextBlock tblk;
            Canvas cnvs;

            if (m_clsHaiseki.m_nTableBlockCount == 0)
            {
                return;
            }
            resetInputTextBox(m_nSelectTable, m_nSelectSeet);
            cnvsLayout.Children.Clear();

            m_libCanvas.setFontName(m_sFontFamilyNameCrt);
            xsize = cnvsLayout.ActualWidth;
            ysize = cnvsLayout.ActualHeight;
            numx = xsize / Constants.A3WIDTH;
            numy = ysize / Constants.A3HEIGHT;
            if (numx < numy)
            {
                time = numx;
                ysize = Constants.A3HEIGHT * time;
            }
            else
            {
                time = numy;
                xsize = Constants.A3WIDTH * time;
            }
            m_dPageSpace = 10.0 * time;
            m_dTopX = (int)(m_dPageSpace);
            m_dTopY = (int)(m_dPageSpace);
            m_dEndX = xsize - m_dPageSpace;
            m_dEndY = ysize - m_dPageSpace;
            m_dPageX = m_dEndX - m_dTopX;
            m_dPageY = m_dEndY - m_dTopY;

            m_dNoSpace = 10.0 * time;
            m_dSpace = 2.0 * time;
            dLineCount = m_clsHaiseki.m_nSeetCount + 12.5;
            m_dLineHeight = (int)((m_dEndY - m_dTopY) / dLineCount);
            tblblkmax = m_clsHaiseki.m_nTableBlockCount;
            tblmax = tblblkmax * 2;
            dWidthSpace = (m_dNoSpace + m_dSpace) * tblblkmax - m_dSpace;
            m_dTableWidth = ((m_dPageX - dWidthSpace) / (double)tblmax);

            m_clsHaiseki.m_nSouryoCount = m_libCmn.StrToInt(txtTakasago.Text);
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

            fontsize = (int)(m_dLineHeight * 1.4);
            m_clsHaiseki.m_sSoukeName = txtSouke.Text;
            str = m_clsHaiseki.m_sSoukeName + "家　" + txtTitle.Text;
            if (m_dPageX < (str.Length+2) * fontsize)
            {
                fontsize = (int)(m_dPageX / (str.Length + 2));
            }
            len = str.Length * fontsize;
            sx = (m_dTopX + (m_dPageX - len) / 2);
            sy = m_dTopY;
            m_libCanvas.setFontSize(fontsize);
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);

            numx = (m_dTableWidth / 8.6);
            numy = (m_dLineHeight * 0.65);
            if (numx < numy)
            {
                fontsize = numx;
            }
            else
            {
                fontsize = numy;
            }
            m_libCanvas.setFontSize(fontsize);

            m_clsHaiseki.m_sReizenText = txtRTStr.Text;
            if (rdoCenter.IsChecked == true)
            {
                m_clsHaiseki.m_nReizenAlignment = 1;
                sx = (m_dTopY + m_dEndX - m_dTableWidth) / 2;
                ex = (sx + m_dTableWidth);
            }
            else
            {
                m_clsHaiseki.m_nReizenAlignment = 0;
                sx = (m_dEndX - m_dTableWidth);
                ex = m_dEndX;
            }
            sy = (m_dTopY + m_dLineHeight * 1.6);
            ey = (sy + m_dLineHeight);
            cnvsReizen = m_libCanvas.CreateCanvas(cnvsLayout, sx, sy, m_dTableWidth, m_dLineHeight);
            cnvsReizen.MouseUp += cnvsMouseUp;
            cnvsReizen.Name = "RC_0_0";
            m_clsHaiseki.setReizenCanvas(cnvsReizen);

            tsx = (m_dLineHeight + 2);
            tsy = 2;
            wd = m_dTableWidth - m_dLineHeight - 4;
            hi = m_dLineHeight - 4;
            m_libCanvas.setBackgroundBrush(Brushes.White);
            m_libCanvas.setTextBrush(Brushes.Black);
            m_libCanvas.drawCanvasCenterString(cnvsReizen, tsx, tsy, wd, hi, m_clsHaiseki.m_sReizenText);
            m_libCanvas.drawBoxs(cnvsLayout, sx, sy, m_dTableWidth, m_dLineHeight);

            tsx = 2;
            wd = m_dLineHeight - 4;
            nSet = m_clsHaiseki.m_nReizenHikimonoType;
            if (nSet == -1)
            {
                str = "";
                brush = Brushes.White;
            }
            else
            {
                str = m_clsHaiseki.m_lstHikidemono[nSet].m_sName;
                brush = m_clsHaiseki.m_lstHikidemono[nSet].getBrush();
            }
            m_libCanvas.setBackgroundBrush(brush);
            m_libCanvas.setTextBrush(Brushes.Black);
            cnvs = m_libCanvas.drawCanvasString(cnvsReizen, tsx, tsy, wd, hi, str);
            cnvs.Name = "RC_0_0";
            m_clsHaiseki.setHkdmnCanvas(cnvs);

            // 僧侶席エリア
            max = m_clsHaiseki.m_nSouryoCount;
            hi = m_dLineHeight;
            crtx = m_dTopX + (m_dPageX - m_dSouryoWidth * max) / 2;
            crty = m_dTopY + m_dLineHeight * 2.8;
            for (clm = 0; clm < max; clm++)
            {
                cnvsSekiji = m_libCanvas.CreateCanvas(cnvsLayout, crtx, crty, m_dSouryoWidth, m_dLineHeight);
                cnvsSekiji.MouseDown += cnvsMouseDown;
                cnvsSekiji.MouseUp += cnvsMouseUp;
                cnvsSekiji.Name = "SC_0_" + clm;
                m_clsHaiseki.m_lstSouryo[clm].setSekijiCanvas(cnvsSekiji);

                sx = fontsize * 0.2;
                sy = fontsize * 0.2;
                ex = (sx + m_dSouryoWidth);
                ey = (sy + m_dLineHeight);

                tsx = (sx + m_dSouryoWidth - fontsize * 1.2);
                tsy = sy;
                m_libCanvas.drawStrg(cnvsSekiji, tsx, tsy, 0.0, 1.0, "様");

                tsx = sx + fontsize*1.2;
                wd = (m_dSouryoWidth - fontsize * 2.4);
                hi = ey - sy;
                sName = m_clsHaiseki.m_lstSouryo[clm].m_sName;
                nSet = m_clsHaiseki.m_lstSouryo[clm].m_nRyouriType;
                brush = m_clsHaiseki.m_lstRyouri[nSet].getBrush();
                m_libCanvas.setBackgroundBrush(Brushes.White);
                m_libCanvas.setTextBrush(brush);
                cnvs = m_libCanvas.drawCanvasString(cnvsSekiji, tsx, tsy, wd, hi, sName);
                m_clsHaiseki.m_lstSouryo[clm].setNameCanvas(cnvs);
                if (sName != "")
                {
                    nSekisu++;
                    nRType[nSet] = nRType[nSet] + 1;
                }

                tsx = sx;
                wd = fontsize;
                nSet = m_clsHaiseki.m_lstSouryo[clm].m_nHikidemonoType;
                if (nSet == -1)
                {
                    str = "";
                    brush = Brushes.White;
                }
                else
                {
                    str = m_clsHaiseki.m_lstHikidemono[nSet].m_sName;
                    brush = m_clsHaiseki.m_lstHikidemono[nSet].getBrush();
                }
                m_libCanvas.setBackgroundBrush(brush);
                m_libCanvas.setTextBrush(Brushes.Black);
                cnvs = m_libCanvas.drawCanvasString(cnvsSekiji, tsx, tsy, wd, hi, str);
                m_clsHaiseki.m_lstSouryo[clm].setHkdmnCanvas(cnvs);
                m_libCanvas.drawBoxs(cnvsLayout, crtx, crty, m_dSouryoWidth, m_dLineHeight);

                crtx = crtx + m_dSouryoWidth;
            }
            tblblkmax = m_clsHaiseki.m_nTableBlockCount;
            dTableSumWidth = (m_dTableWidth) * tblblkmax * 2 + (m_dNoSpace + m_dSpace) * tblblkmax - m_dSpace;
            if ((m_dEndX - m_dTopX) > dTableSumWidth)
            {
                dTableTopX = (m_dTopX + m_dEndX - dTableSumWidth) / 2;
            }
            else
            {
                dTableTopX = m_dTopX;
            }
            // テーブルエリア
            for (tblblk = 0; tblblk < tblblkmax; tblblk++)
            {
                crtx = dTableTopX + ((m_dTableWidth * 2) + (m_dNoSpace + m_dSpace)) * tblblk;
                crty = m_dTopY + m_dLineHeight * 4;

                tbl = tblblk * 2;
                for (line = 0; line < m_clsHaiseki.m_nSeetCount; line++)
                {
                    cnvsSekiji = m_libCanvas.CreateCanvas(cnvsLayout, crtx, crty, m_dTableWidth, m_dLineHeight);
                    cnvsSekiji.MouseDown += cnvsMouseDown;
                    cnvsSekiji.MouseUp += cnvsMouseUp;
                    cnvsSekiji.Name = "TC_" + tbl + "_" + line;
                    m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].setSekijiCanvas(cnvsSekiji);

                    sx = fontsize * 0.2;
                    sy = fontsize * 0.2;
                    ex = (sx + m_dTableWidth);
                    ey = (sy + m_dLineHeight);
                    cx = (sx + m_dTableWidth);

                    tsx = sx + m_dTableWidth - fontsize * 1.2;
                    tsy = sy;
                    m_libCanvas.drawStrg(cnvsSekiji, tsx, tsy, 0.0, 1.0, "様");

                    tsx = sx + fontsize * 1.2;
                    wd = m_dTableWidth - fontsize * 2.4;
                    hi = ey - sy;
                    sName = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_sName;
                    nSet = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_nRyouriType;
                    brush = m_clsHaiseki.m_lstRyouri[nSet].getBrush();
                    m_libCanvas.setBackgroundBrush(Brushes.White);
                    m_libCanvas.setTextBrush(brush);
                    cnvs = m_libCanvas.drawCanvasString(cnvsSekiji, tsx, tsy, wd, hi, sName);
                    m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].setNameCanvas(cnvs);
                    if (sName != "")
                    {
                        nSekisu++;
                        nRType[nSet] = nRType[nSet] + 1;
                    }

                    tsx = sx;
                    wd = fontsize;
                    nSet = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_nHikidemonoType;
                    if (nSet == -1)
                    {
                        str = "";
                        brush = Brushes.White;
                    }
                    else
                    {
                        str = m_clsHaiseki.m_lstHikidemono[nSet].m_sName;
                        brush = m_clsHaiseki.m_lstHikidemono[nSet].getBrush();
                    }
                    m_libCanvas.setBackgroundBrush(brush);
                    m_libCanvas.setTextBrush(Brushes.Black);
                    cnvs = m_libCanvas.drawCanvasString(cnvsSekiji, tsx, tsy, wd, hi, str);
                    m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].setHkdmnCanvas(cnvs);
                    crty = crty + m_dLineHeight;
                }

                crtx = crtx + (m_dTableWidth + m_dNoSpace);
                crty = m_dTopY + m_dLineHeight * 4;
                tbl = tblblk * 2 + 1;
                for (line = 0; line < m_clsHaiseki.m_nSeetCount; line++)
                {
                    cnvsSekiji = m_libCanvas.CreateCanvas(cnvsLayout, crtx, crty, m_dTableWidth, m_dLineHeight);
                    cnvsSekiji.MouseDown += cnvsMouseDown;
                    cnvsSekiji.MouseUp += cnvsMouseUp;
                    cnvsSekiji.Name = "TC_" + tbl + "_" + line;
                    m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].setSekijiCanvas(cnvsSekiji);

                    sx = fontsize * 0.2;
                    sy = fontsize * 0.2;
                    ex = (sx + m_dTableWidth);
                    ey = (sy + m_dLineHeight);
                    cx = (sx + m_dTableWidth);

                    tsx = sx + m_dTableWidth - fontsize * 1.2;
                    tsy = sy;
                    m_libCanvas.drawStrg(cnvsSekiji, tsx, tsy, 0.0, 1.0, "様");

                    tsx = sx + fontsize * 1.2;
                    wd = m_dTableWidth - fontsize * 2.4;
                    hi = ey - sy;
                    sName = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_sName;
                    nSet = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_nRyouriType;
                    brush = m_clsHaiseki.m_lstRyouri[nSet].getBrush();
                    m_libCanvas.setBackgroundBrush(Brushes.White);
                    m_libCanvas.setTextBrush(brush);
                    cnvs = m_libCanvas.drawCanvasString(cnvsSekiji, tsx, tsy, wd, hi, sName);
                    m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].setNameCanvas(cnvs);
                    if (sName != "")
                    {
                        nSekisu++;
                        nRType[nSet] = nRType[nSet] + 1;
                    }


                    tsx = sx;
                    wd = fontsize;
                    nSet = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].m_nHikidemonoType;
                    if (nSet == -1)
                    {
                        str = "";
                        brush = Brushes.White;
                    }else{
                        str = m_clsHaiseki.m_lstHikidemono[nSet].m_sName;
                        brush = m_clsHaiseki.m_lstHikidemono[nSet].getBrush();
                    }
                    m_libCanvas.setBackgroundBrush(brush);
                    m_libCanvas.setTextBrush(Brushes.Black);
                    cnvs = m_libCanvas.drawCanvasString(cnvsSekiji, tsx, tsy, wd, hi, str);
                    m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[line].setHkdmnCanvas(cnvs);
                    crty = crty + m_dLineHeight;
                }

                sx1 = dTableTopX + ((m_dTableWidth * 2) + (m_dNoSpace + m_dSpace)) * tblblk;
                sx2 = sx1 + m_dTableWidth + m_dNoSpace;
                ex1 = (sx1 + m_dTableWidth);
                ex2 = (sx2 + m_dTableWidth);
                sy = (m_dTopY + m_dLineHeight * 4);
                m_libCanvas.drawLine(cnvsLayout, sx1, sy, ex2, sy);
                for (line = 0; line < m_clsHaiseki.m_nSeetCount; line++)
                {
                    no = line + 1;
                    ey = (int)(sy + m_dLineHeight);
                    m_libCanvas.drawLine(cnvsLayout, sx1, sy, sx1, ey);
                    m_libCanvas.drawLine(cnvsLayout, ex1, sy, ex1, ey);

                    str = string.Format("{0, 2}", no);
                    m_libCanvas.drawStrg(cnvsLayout, ex1 + fontsize * 0.2, sy + fontsize * 0.2, 0.0, 1.0, str);
                    m_libCanvas.drawLine(cnvsLayout, sx2, sy, sx2, ey);
                    m_libCanvas.drawLine(cnvsLayout, ex2, sy, ex2, ey);

                    m_libCanvas.drawLine(cnvsLayout, sx1, ey, ex2, ey);
                    sy = ey;
                }
            }

            m_libCanvas.setTextBrush(Brushes.Black);
            m_clsHaiseki.m_sRightBottomText = txtRBStr.Text;
            str = m_clsHaiseki.m_sRightBottomText;
            len = str.Length;
            sx = (xsize - (fontsize * len + m_dSpace));
            sy = (m_dTopY + m_dLineHeight * (4 + m_clsHaiseki.m_nSeetCount));
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);

            //fontsize = (int)(fontsize * 0.7);
            m_libCanvas.setFontSize(fontsize);
            sx = (m_dTopX);
            sy = (m_dTopY + m_dLineHeight * (4 + m_clsHaiseki.m_nSeetCount));
            sy = sy + fontsize * 2;

            int num = 1 + nSekisu;
            str = "座席数＝" + num + "(" + txtRTStr.Text + "含め)";
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);

            sy = (sy + fontsize);
            str = m_clsHaiseki.m_lstRyouri[0].m_sName + "＝" + nRType[0];
            brush = m_clsHaiseki.m_lstRyouri[0].getBrush();
            m_libCanvas.setTextBrush(brush);
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);

            sy = (sy + fontsize);
            str = m_clsHaiseki.m_lstRyouri[1].m_sName + "＝" + nRType[1];
            brush = m_clsHaiseki.m_lstRyouri[1].getBrush();
            m_libCanvas.setTextBrush(brush);
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);

            sy = (sy + fontsize);
            str = m_clsHaiseki.m_lstRyouri[2].m_sName + "＝" + nRType[2];
            brush = m_clsHaiseki.m_lstRyouri[2].getBrush();
            m_libCanvas.setTextBrush(brush);
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);

            sy = (sy + fontsize);
            str = m_clsHaiseki.m_lstRyouri[3].m_sName + "＝" + nRType[3];
            brush = m_clsHaiseki.m_lstRyouri[3].getBrush();
            m_libCanvas.setTextBrush(brush);
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);

            sy = (sy + fontsize);
            str = m_clsHaiseki.m_lstRyouri[4].m_sName + "＝" + nRType[4];
            brush = m_clsHaiseki.m_lstRyouri[4].getBrush();
            m_libCanvas.setTextBrush(brush);
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);

            sy = (sy + fontsize);
            str = m_clsHaiseki.m_lstRyouri[5].m_sName + "＝" + nRType[5];
            brush = m_clsHaiseki.m_lstRyouri[5].getBrush();
            m_libCanvas.setTextBrush(brush);
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);

            if (m_clsHaiseki.m_lstRyouri[6].m_sName != "")
            {
                sy = (sy + fontsize);
                str = m_clsHaiseki.m_lstRyouri[6].m_sName + "＝" + nRType[6];
                brush = m_clsHaiseki.m_lstRyouri[6].getBrush();
                m_libCanvas.setTextBrush(brush);
                m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);
            }

            sy = (sy + fontsize);
            str = m_clsHaiseki.m_lstRyouri[7].m_sName + "＝1";
            brush = m_clsHaiseki.m_lstRyouri[7].getBrush();
            m_libCanvas.setTextBrush(brush);
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);

            sy = (sy + fontsize);
            str =  m_clsHaiseki.m_lstRyouri[8].m_sName + "＝" + m_clsHaiseki.m_nYudemasu;
            brush = m_clsHaiseki.m_lstRyouri[8].getBrush();
            m_libCanvas.setTextBrush(brush);
            m_libCanvas.drawStrg(cnvsLayout, sx, sy, 0.0, 1.0, str);
            

            nHikidemonoWidth = (int)(m_dPageX / 4);

            sx = (m_dTopX + nHikidemonoWidth);
            ex = (sx + nHikidemonoWidth);
            sy = (m_dTopY + m_dLineHeight * (4 + m_clsHaiseki.m_nSeetCount) + fontsize*2);
            ey = sy + (fontsize * Constants.HIKIDEMONOMAX);
            wd = ex - sx;
            hi = ey - sy;
            // 引き出物エリア
            hkmax = Constants.HIKIDEMONOSETMAX;
            for (hk = 0; hk < hkmax; hk++)
            {
                if (hk == (Constants.HIKIDEMONOSETMAX / 2))
                {
                    sx = (m_dTopX + nHikidemonoWidth);
                    sy = (sy + fontsize * (Constants.HIKIDEMONOMAX) + fontsize);
                    ey = sy + (fontsize * Constants.HIKIDEMONOMAX);
                }
                m_libCanvas.setFontSize(fontsize);
                ex = sx + nHikidemonoWidth;

                str = m_clsHaiseki.m_lstHikidemono[hk].m_sName + "セット";
                cx = (wd / 2.0);
                tsx = sx + cx - (str.Length * fontsize) / 2;
                tsy = (sy - fontsize);
                m_libCanvas.drawStrg(cnvsLayout, tsx, tsy, 0.0, 1.0, str);
 
                cnvs = m_libCanvas.CreateCanvas(cnvsLayout, sx, sy, ex - sx, ey - sy);
                cnvs.Background = m_clsHaiseki.m_lstHikidemono[hk].getBrush();
                cnvs.Name = "HC_" + hk;
                cnvs.MouseDown += cnvsHikimonoMouseDown;

                m_libCanvas.setFontSize(fontsize);
                tsx = (fontsize / 4);
                tsy = (fontsize / 4);
                tex = (wd - fontsize / 4);
                tey = (hi - fontsize / 4);
                m_libCanvas.drawLine(cnvs, tsx, 0, 0, tsy);
                m_libCanvas.drawLine(cnvs, 0, tsy, 0, tey);
                m_libCanvas.drawLine(cnvs, 0, tey, tsx, hi);

                m_libCanvas.drawLine(cnvs, tex, 0, wd, tsy);
                m_libCanvas.drawLine(cnvs, wd, tsy, wd, tey);
                m_libCanvas.drawLine(cnvs, wd, tey, tex, hi);

                cy = (fontsize);
                for (line = 0; line < Constants.HIKIDEMONOMAX; line++)
                {
                    str = m_clsHaiseki.m_lstHikidemono[hk].m_lstSyouhin[line];
                    str = m_libCmn.SubStringByte(str, 14);
                    tblk = m_libCanvas.CreateTextBlock(tsx, cy - fontsize, str);
                    cnvs.Children.Add(tblk);
                    m_clsHaiseki.m_lstHikidemono[hk].setTextBlock(line, tblk);
                    m_libCanvas.drawLine(cnvs, tsx, cy, tex, cy);
                    cy = (cy + fontsize);
                }
                sx = sx + nHikidemonoWidth;
            }

            //m_libCanvas.drawBoxs(cnvsLayout, m_dTopX, m_dTopY, m_dEndX - m_dTopX, m_dEndY - m_dTopY);
            m_libCanvas.drawBoxs(cnvsLayout, 1, 1, xsize - 2, ysize - 2);
        }
    }
}
