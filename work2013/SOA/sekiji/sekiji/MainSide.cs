using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace sekiji
{
    public partial class MainWindow : Window
    {
        private void cnvsRyouriColor_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsRyouriColor.Background = m_brushSlelct;
        }
        private void cnvsRyouriColor_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsRyouriColor.Background = m_brushNomal;
            setRyouriSelectSekiji(0);
        }
        private void cnvsKodomoColor_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsKodomoColor.Background = m_brushSlelct;
        }
        private void cnvsKodomoColor_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsKodomoColor.Background = m_brushNomal;
            setRyouriSelectSekiji(1);
        }
        private void cnvsJyuniaColor_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsJyuniaColor.Background = m_brushSlelct;
        }
        private void cnvsJyuniaColor_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsJyuniaColor.Background = m_brushNomal;
            setRyouriSelectSekiji(2);
        }
        private void cnvsRyouriKColor_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsRyouriKColor.Background = m_brushSlelct;
        }
        private void cnvsRyouriKColor_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsRyouriKColor.Background = m_brushNomal;
            setRyouriSelectSekiji(3);
        }
        private void cnvsRyouriDColor_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsRyouriDColor.Background = m_brushSlelct;
        }
        private void cnvsRyouriDColor_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsRyouriDColor.Background = m_brushNomal;
            setRyouriSelectSekiji(4);
        }
        private void cnvsRyouriKDColor_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsRyouriKDColor.Background = m_brushSlelct;
        }
        private void cnvsRyouriKDColor_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsRyouriKDColor.Background = m_brushNomal;
            setRyouriSelectSekiji(5);
        }
        private void cnvsAddRyouriColor_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsAddRyouriColor.Background = m_brushSlelct;
        }

        private void cnvsAddRyouriColor_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsAddRyouriColor.Background = m_brushNomal;
            setRyouriSelectSekiji(6);
        }
        private void cnvsHiki_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsHiki.Background = m_brushSlelct;
        }
        private void cnvsHiki_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsHiki.Background = m_brushNomal;
            setHikidemonoSelectSekiji(-1);
        }
        private void cnvsHikiA_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiA.Background = m_brushSlelct;
        }
        private void cnvsHikiA_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiA.Background = m_brushNomal;
            setHikidemonoSelectSekiji(0);
        }
        private void cnvsHikiB_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiB.Background = m_brushSlelct;
        }
        private void cnvsHikiB_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiB.Background = m_brushNomal;
            setHikidemonoSelectSekiji(1);
        }
        private void cnvsHikiC_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiC.Background = m_brushSlelct;
        }
        private void cnvsHikiC_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiC.Background = m_brushNomal;
            setHikidemonoSelectSekiji(2);
        }
        private void cnvsHikiD_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiD.Background = m_brushSlelct;
        }
        private void cnvsHikiD_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiD.Background = m_brushNomal;
            setHikidemonoSelectSekiji(3);
        }
        private void cnvsHikiE_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiE.Background = m_brushSlelct;
        }
        private void cnvsHikiE_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiE.Background = m_brushNomal;
            setHikidemonoSelectSekiji(4);
        }
        private void cnvsHikiF_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiF.Background = m_brushSlelct;
        }
        private void cnvsHikiF_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            cnvsHikiF.Background = m_brushNomal;
            setHikidemonoSelectSekiji(5);
        }
        private void setRyouriSelectSekiji(int nType)
        {
            int tmax, tbl;
            int smax, seet;
            Canvas cv;
            TextBlock tb;
            Brush brush;

            if (m_clsHaiseki.m_nTableBlockCount == 0)
            {
                return;
            }
            brush = m_clsHaiseki.m_lstRyouri[nType].getBrush();
            smax = m_clsHaiseki.m_nSouryoCount;
            for (seet = 0; seet < smax; seet++)
            {
                cv = m_clsHaiseki.m_lstSouryo[seet].getSekijiCanvas();
                if (cv.Background == m_brushSlelct)
                {
                    if (m_clsHaiseki.m_lstSouryo[seet].m_sName != "")
                    {
                        m_clsHaiseki.m_lstSouryo[seet].m_nRyouriType = nType;
                        tb = m_clsHaiseki.m_lstSouryo[seet].getNameTextBlock();
                        tb.Foreground = brush;
                    }
                }
            }
            tmax = m_clsHaiseki.m_nTableBlockCount * 2;
            smax = m_clsHaiseki.m_nSeetCount;
            for (tbl = 0; tbl < tmax; tbl++)
            {
                for (seet = 0; seet < smax; seet++)
                {
                    cv = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].getSekijiCanvas();
                    if (cv.Background == m_brushSlelct)
                    {
                        if (m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].m_sName != "")
                        {
                            m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].m_nRyouriType = nType;
                            tb = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].getNameTextBlock();
                            tb.Foreground = brush;
                        }
                    }
                }
            }
        }
        private void setHikidemonoSelectSekiji(int nType)
        {
            int tmax, tbl;
            int smax, seet;
            Canvas cnvs;
            TextBlock tblk;
            Brush brush;
            String sSetStr;

            if (m_clsHaiseki.m_nTableBlockCount == 0)
            {
                return;
            }
            if (nType == -1)
            {
                brush = Brushes.White;
                sSetStr = "";
            }
            else
            {
                brush = m_clsHaiseki.m_lstHikidemono[nType].getBrush();
                sSetStr = m_clsHaiseki.m_lstHikidemono[nType].m_sName;
            }
            cnvs = m_clsHaiseki.getReizenCanvas();
            if (cnvs.Background == m_brushSlelct)
            {
                m_clsHaiseki.m_nReizenHikimonoType = nType;
                tblk = m_clsHaiseki.getHkdmnTextBlock();
                tblk.Text = sSetStr;
                cnvs = m_clsHaiseki.getHkdmnCanvas();
                cnvs.Background = brush;
            }

            smax = m_clsHaiseki.m_nSouryoCount;
            for (seet = 0; seet < smax; seet++)
            {
                cnvs = m_clsHaiseki.m_lstSouryo[seet].getSekijiCanvas();
                if (cnvs.Background == m_brushSlelct)
                {
                    if (m_clsHaiseki.m_lstSouryo[seet].m_sName != "")
                    {
                        cnvs = m_clsHaiseki.m_lstSouryo[seet].getHkdmnCanvas();
                        m_clsHaiseki.m_lstSouryo[seet].m_nHikidemonoType = nType;
                        cnvs.Background = brush;
                        tblk = m_clsHaiseki.m_lstSouryo[seet].getHkdmnTextBlock();
                        tblk.Text = sSetStr;
                    }
                }
            }
            tmax = m_clsHaiseki.m_nTableBlockCount * 2;
            smax = m_clsHaiseki.m_nSeetCount;
            for (tbl = 0; tbl < tmax; tbl++)
            {
                for (seet = 0; seet < smax; seet++)
                {
                    cnvs = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].getSekijiCanvas();
                    if (cnvs.Background == m_brushSlelct)
                    {
                        if (m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].m_sName != "")
                        {
                            cnvs = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].getHkdmnCanvas();
                            m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].m_nHikidemonoType = nType;
                            cnvs.Background = brush;
                            tblk = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].getHkdmnTextBlock();
                            tblk.Text = sSetStr;
                        }
                    }
                }
            }
        }
    }
}
