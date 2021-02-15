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

namespace instr
{
    /// <summary>
    /// SelectWakuWin.xaml の相互作用ロジック
    /// </summary>
    public partial class SelectWakuWin : Window
    {
        private MainWindow m_wnd;
        public LibCommon m_libCmn;
        public LibCanvas m_libCanvas;
        private List<string> m_sWakuNameList;
        private int m_nSelectMode;
        private string m_sWakuPath;
        private double m_dXSize, m_dYSize;
        private double m_dTime;
        private double m_dAddX, m_dAddY;
        private int m_nXCount, m_nYCount;
        private int m_nSlctIdx, m_nNafudaMax;


        public SelectWakuWin()
        {
            InitializeComponent();
            m_sWakuNameList = new List<string>();
            m_nSelectMode = 0;
            m_sWakuPath = "waku\\waku";
            m_nSlctIdx = 0;
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wnd = wnd;
            m_libCmn = m_wnd.m_libCmn;
            m_libCanvas = m_wnd.m_libCanvas;
        }
        public void SetInsertMode()
        {
            m_nSelectMode = 0;
        }
        public void SetUpdateMode()
        {
            m_nSelectMode = 1;
        }
        public void SetNafudaElement(string sWakuPath, double xsize, double ysize)
        {
            m_sWakuPath = sWakuPath;
            m_dXSize = xsize;
            m_dYSize = ysize;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            initLoadWaku();
        }
        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Left){
                m_nSlctIdx--;
                updateSelectRedrow();
            }
            else if (e.Key == Key.Right) {
                m_nSlctIdx++;
                updateSelectRedrow();
            }
            else if (e.Key == Key.Up)
            {
                m_nSlctIdx = m_nSlctIdx - m_nXCount;
                updateSelectRedrow();
            }
            else if (e.Key == Key.Down)
            {
                m_nSlctIdx = m_nSlctIdx + m_nXCount;
                updateSelectRedrow();
            }
            else if (e.Key == Key.Return)
            {
                selectWakuFixed();
                e.Handled = true;
            }
            else if (e.Key == Key.Escape)
            {
                this.Close();
            }
        }
        private void updateSelectRedrow()
        {
            if (m_nSlctIdx < 0)
            {
                m_nSlctIdx = 0;
            }
            if (m_nNafudaMax <= m_nSlctIdx)
            {
                m_nSlctIdx = m_nNafudaMax - 1;
            }
            initLoadWaku();
            int ypos = ((int)(m_nSlctIdx / m_nXCount)) * (Constants.ONEBLOCKSIZE + Constants.SPACEY);
            scrlCanvas.ScrollToVerticalOffset(ypos);
        }
        private void btnSelect_Click(object sender, RoutedEventArgs e)
        {
            selectWakuFixed();
        }
        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void scrlCanvas_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            int dx, dy;
            int idxx, idxy;

            Point pos = e.GetPosition(canvasWakuImage);
            dx = (int)pos.X;
            dy = (int)pos.Y;
            idxx = dx / (Constants.ONEBLOCKSIZE + Constants.SPACEX);
            idxy = dy / (Constants.ONEBLOCKSIZE + Constants.SPACEY);
            m_nSlctIdx = idxy * m_nXCount + idxx;
            initLoadWaku();
        }
        private void scrlCanvas_PreviewMouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            selectWakuFixed();
        }
        private void selectWakuFixed()
        {
            if (m_nSlctIdx == -1)
            {
                MessageBox.Show("枠が選択されていません。", "確認", MessageBoxButton.OK);
                return;
            }
            if (m_nSelectMode == 0)
            {
                m_wnd.CreateRowItem(m_sWakuNameList[m_nSlctIdx]);
            }
            else
            {
                m_wnd.UpdateRowItem(m_sWakuNameList[m_nSlctIdx]);
            }
            this.Close();
        }
        private void initLoadWaku()
        {
            int sx, sy;
            string sFileName;
            string[] aryLine;
            int max, idx;
            string sWakuName;
            string sWakuFileName;

            canvasWakuImage.Children.Clear();
            m_nXCount = 0;
            m_nYCount = 0;
            sFileName = m_sWakuPath + "\\WAKUNAME.LST";
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            sx = 0; sy = 0;
            m_nNafudaMax = 0;
            max = aryLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                if (aryLine[idx] != "") {
                    sWakuName = m_libCmn.DeleteDoubleQuotation(aryLine[idx]);
                    m_sWakuNameList.Add(sWakuName);
                    sWakuFileName = m_sWakuPath + "\\" + sWakuName;
                    drawWakuImage(idx, sx, sy, sWakuFileName);
                    m_nNafudaMax++;
                    if (sy == 0)
                    {
                        m_nXCount++;
                    }
                    sx = sx + Constants.ONEBLOCKSIZE + Constants.SPACEX;
                    if (canvasWakuImage.ActualWidth < (sx + Constants.ONEBLOCKSIZE))
                    {
                        sx = 0;
                        sy = sy + Constants.ONEBLOCKSIZE + Constants.SPACEY;
                        canvasWakuImage.Height = sy + Constants.ONEBLOCKSIZE + Constants.SPACEY;
                        m_nYCount++;
                    }
                }
            }
        }
        private void drawWakuImage(int lineidx, int topx, int topy, string sWakuFileName)
        {
            int len;
            string sEnvFileName;
            string sName;
            string[] aryLine;
            int max, idx;
            string[] ary;
            double dXBase, dYBase;
            double sx, sy, ex, ey;
            double wd, hi;

            dXBase = m_dXSize;
            dYBase = m_dYSize;
            len = sWakuFileName.Length;
            sEnvFileName = sWakuFileName.Substring(0, len - 3) + "env";
            aryLine = m_libCmn.LoadFileLineSJIS(sEnvFileName);
            if (11 <= aryLine.Length)
            {
                ary = aryLine[10].Split(' ');
                dXBase = m_libCmn.StrToDouble(ary[0]);
                dYBase = m_libCmn.StrToDouble(ary[1]);
            }
            calcBaseTimeOffset(topx, topy, dXBase, dYBase);

            if (lineidx == m_nSlctIdx)
            {
                m_libCanvas.setBrushStroke(0xff,0,0,1.0,0);
            }else{
                m_libCanvas.setBrushStroke(0, 0, 0, 1.0, 0);
            }
            sx = 0;
            sy = 0;
            ex = dXBase;
            ey = dYBase;
            drawBoxs(sx, sy, ex, ey);
            sName = System.IO.Path.GetFileName(sWakuFileName);
            len = sName.Length;
            sName = sName.Substring(0, len - 4);
            sx = topx;
            sy = topy+ Constants.ONEBLOCKSIZE;
            m_libCanvas.setBrushStroke(0, 0, 0, 1.0, 0);
            m_libCanvas.setFontSize(20);
            m_libCanvas.drawYokoStrg(canvasWakuImage, sx, sy, 0.0, 1.0, sName);

            aryLine = m_libCmn.LoadFileLineSJIS(sWakuFileName);
            max = aryLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                ary = aryLine[idx].Split(',');
                if (14 <= ary.Length)
                {
                    sx = m_libCmn.StrToDouble(ary[3]);
                    sy = m_libCmn.StrToDouble(ary[4]);
                    wd = m_libCmn.StrToDouble(ary[5]);
                    hi = m_libCmn.StrToDouble(ary[6]);
                    ex = sx + wd;
                    ey = sy + hi;
                    drawBoxs(sx, sy, ex, ey);
                }
            }
        }
        private void calcBaseTimeOffset(int nTopx, int nTopy, double dXBase, double dYBase)
        {
            double dOneBlock;
            double dXTime, dYTime;

            dOneBlock = Constants.ONEBLOCKSIZE;
            dXTime = dOneBlock / dXBase;
            dYTime = dOneBlock / dYBase;
            if (dXTime < dYTime)
            {
                m_dTime = dXTime;
                m_dAddX = nTopx;
                m_dAddY = nTopy + (dOneBlock - dYBase * m_dTime) / 2;
            }
            else
            {
                m_dTime = dYTime;
                m_dAddX = nTopx + (dOneBlock - dXBase * m_dTime) / 2;
                m_dAddY = nTopy;
            }
        }
        private void drawBoxs(double sx, double sy, double ex, double ey)
        {
            sx = sx * m_dTime + m_dAddX;
            sy = sy * m_dTime + m_dAddY;
            ex = ex * m_dTime + m_dAddX;
            ey = ey * m_dTime + m_dAddY;
            m_libCanvas.drawLine(canvasWakuImage, sx, sy, sx, ey);
            m_libCanvas.drawLine(canvasWakuImage, sx, ey, ex, ey);
            m_libCanvas.drawLine(canvasWakuImage, ex, ey, ex, sy);
            m_libCanvas.drawLine(canvasWakuImage, ex, sy, sx, sy);
        }
    }
}
