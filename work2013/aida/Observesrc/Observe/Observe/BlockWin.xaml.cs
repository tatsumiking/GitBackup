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

namespace Observe
{
    /// <summary>
    /// BlockWin.xaml の相互作用ロジック
    /// </summary>
    public partial class BlockWin : Window
    {
        private MainWindow m_wndMain;
        public LibCanvas m_libCnvs;
        private double m_dOneCardHeight;
        private double m_dFontSize;
        private double m_dScrollbarSpace;
        private double m_dAreaWidth;
        private double m_dAreaHeight;

        public BlockWin()
        {
            InitializeComponent();
            m_libCnvs = new LibCanvas();
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wndMain = wnd;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            SetListElement();
        }
        public void SetListElement()
        {
            int max;
            double x1, x2;

            m_dOneCardHeight = 40.0;
            m_dFontSize = 30.0;
            m_dScrollbarSpace = 30;
            x1 = m_dFontSize * 12;
            x2 = m_dFontSize * 21;
            if (m_wndMain.m_clsObserve != null)
            {
                m_dAreaWidth = gridBlock.ActualWidth;
                m_dAreaHeight = gridBlock.ActualHeight;
                cnvsTitle.Width = m_dAreaWidth;
                cnvsTitle.Height = m_dOneCardHeight;
                SetTitleElement(x1, x2);
                if (m_wndMain == null || m_wndMain.m_clsObserve == null
                 || m_wndMain.m_clsObserve.m_lstClsCard == null)
                {
                    max = 3;
                }
                else
                {
                    max = m_wndMain.m_clsObserve.m_lstClsCard.Count;
                    if (max < 3)
                    {
                        max = 3;
                    }
                    else
                    {
                        max++;
                    }
                }
                cnvsList.Width = m_dAreaWidth;
                cnvsList.Height = m_dOneCardHeight + m_dOneCardHeight * (max-1);
                SetCardElement(x1, x2);
            }
        }
        private void SetTitleElement(double x1, double x2)
        {

            double addx, addy;
            double sx, sy, ex, ey;
            double wd, hi;

            m_libCnvs.setFontSize(m_dFontSize);

            sx = 0; ex = m_dAreaWidth;
            sy = 0; ey = m_dOneCardHeight;
            drowVerticalLine(cnvsTitle, sx, x1, x2, ex, sy, ey);

            addx = 0;
            addy = 4;
            sx = m_dFontSize * 0.5;
            wd = m_dFontSize * 5;
            sy = 0;
            hi = m_dFontSize;
            m_libCnvs.setFillBrush(Brushes.Black);
            m_libCnvs.drawLeftText(cnvsTitle, sx, sy, wd, hi, addx, addy, "稼動●警報");
            sx = sx + wd;
            wd = m_dFontSize;
            m_libCnvs.setFillBrush(Brushes.Red);
            m_libCnvs.drawLeftText(cnvsTitle, sx, sy, wd, hi, addx, addy, "●");
            sx = sx + wd;
            wd = m_dFontSize * 4;
            m_libCnvs.setFillBrush(Brushes.Black);
            m_libCnvs.drawLeftText(cnvsTitle, sx, sy, wd, hi, addx, addy, "持ち去り");
            sx = sx + wd;
            wd = m_dFontSize * 4;
            m_libCnvs.setFillBrush(Brushes.Green);
            m_libCnvs.drawLeftText(cnvsTitle, sx, sy, wd, hi, addx, addy, "●");

            sx = x1;
            wd = x2 - x1;
            m_libCnvs.setFillBrush(Brushes.Black);
            m_libCnvs.drawCenterText(cnvsTitle, sx, sy, wd, hi, addx, addy, "通報装置番号");

            sx = x2;
            wd = m_dAreaWidth - x2 - m_dScrollbarSpace;
            m_libCnvs.setFillBrush(Brushes.Black);
            m_libCnvs.drawCenterText(cnvsTitle, sx, sy, wd, hi, addx, addy, "通報装置署番号");
        }
        private void SetCardElement(double x1, double x2)
        {
            double addx, addy;
            double sx, sy, ex, ey;
            double wd, hi;
            int max, idx, idxmax;
            ClsCard card;


            m_libCnvs.setStrokeBrush(Brushes.Black, 1.0);
            cnvsList.Children.Clear();
            m_wndMain.m_clsObserve.sortClsCardSetNo();
            m_libCnvs.setStrokeBrush(Brushes.Black, 1.0);
            addx = 0;
            addy = 4;
            sx = 0; ex = m_dAreaWidth;
            sy = 0; ey = 0;
            ey = sy;
            sy = 0;
            if (m_wndMain == null || m_wndMain.m_clsObserve == null
             || m_wndMain.m_clsObserve.m_lstClsCard == null)
            {
                idxmax = 0;
                max = 3;
            }
            else
            {
                idxmax = m_wndMain.m_clsObserve.m_lstClsCard.Count;
                max = idxmax;
                if (max < 3)
                {
                    max = 3;
                }
                else
                {
                    max++;
                }
            }
            sy = 0; ey = m_dOneCardHeight * max;
            drowVerticalLine(cnvsList, sx, x1, x2, ex, sy, ey);

            hi = m_dOneCardHeight;
            for (idx = 0; idx < max; idx++)
            {
                sy = idx * m_dOneCardHeight;
                if (idx < idxmax)
                {
                    card = m_wndMain.m_clsObserve.m_lstClsCard[idx];
                    sx = 0;
                    sy = m_dOneCardHeight * (idx);
                    wd = x1;
                    if (card.m_sStat == "")
                    {
                        m_libCnvs.drawCenterText(cnvsList, sx, sy, wd, hi, addx, addy, "");
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
                        m_libCnvs.drawCenterText(cnvsList, sx, sy, wd, hi, addx, addy, "●");
                    }
                    sx = x1;
                    wd = x2 - x1;
                    m_libCnvs.setFillBrush(Brushes.Black);
                    m_libCnvs.drawCenterText(cnvsList, sx, sy, wd, hi, addx, addy, card.m_sSetNo);
                    sx = x2;
                    wd = m_dAreaWidth - x2;
                    m_libCnvs.setFillBrush(Brushes.Black);
                    m_libCnvs.drawCenterText(cnvsList, sx, sy, wd, hi, addx, addy, card.m_sSyoNo);
                }
                sx = 0;
                m_libCnvs.drawLine(cnvsList, sx, sy, ex, sy);
            }

        }
        private void drowVerticalLine(Canvas canvas, double x1, double x2, double x3, double x4, double sy, double ey)
        {
            m_libCnvs.drawLine(canvas, x1, sy, x1, ey);
            m_libCnvs.drawLine(canvas, x2, sy, x2, ey);
            m_libCnvs.drawLine(canvas, x3, sy, x3, ey);
            m_libCnvs.drawLine(canvas, x4, sy, x4, ey);
        }
        private void Window_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            int idx;

            if (m_wndMain.m_nClsCardCrtIdx != -1)
            {
                m_wndMain.selectCrtCardWin();
                // MessageBox.Show("他の警報機が開かれています。", "確認", MessageBoxButton.OK);
                return;
            }
            Point pos = e.GetPosition(gridBlock);
            if((m_dAreaWidth-m_dScrollbarSpace) < pos.X){
                return;
            } 
            idx = (int)((pos.Y + scrlList.VerticalOffset) / m_dOneCardHeight) - 1;
            if (0 > idx)
            {
                return;
            }
            if (m_wndMain.m_clsObserve.m_lstClsCard.Count <= idx)
            {
                m_wndMain.m_nClsCardCrtIdx = -1;
                m_wndMain.setCrtCardIdx(-1);
            }
            else
            {
                m_wndMain.m_nClsCardCrtIdx = idx;
            }
            m_wndMain.setCrtCardWinDisp();
        }
    }
}
