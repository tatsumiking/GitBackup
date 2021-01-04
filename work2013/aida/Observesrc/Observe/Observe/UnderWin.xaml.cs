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
using System.Windows.Threading;

namespace Observe
{
    /// <summary>
    /// UnderWin.xaml の相互作用ロジック
    /// </summary>
    public partial class UnderWin : Window
    {
        private MainWindow m_wndMain;
        public LibCommon m_libCmn;
        public LibCanvas m_libCnvs;
        private Button m_btnReset;
        private DispatcherTimer m_dsptCheckTime;

        public UnderWin()
        {
            InitializeComponent();
            m_libCmn = new LibCommon();
            m_libCnvs = new LibCanvas();
        }
        public void SetMainWindow(MainWindow wnd)
        {
            m_wndMain = wnd;
        }
        private void cnvsMsg_Loaded(object sender, RoutedEventArgs e)
        {
            initCheckThread();
            dispMsg("");
            //dispMsg("持ち去り警報");
            // dispMsg("発報中,センサー発報中　インプット 2 作動検知");
        }
        public void dispMsg(String sMsg)
        {
            double wd, hi;
            double addy;
            string[] aryMsg;

            cnvsMsg.Children.Clear();
            if (sMsg == "")
            {
                m_dsptCheckTime.Stop();
                cnvsMsg.Background = Brushes.White;
            }
            else
            {
                wd = cnvsMsg.ActualWidth;
                hi = cnvsMsg.ActualHeight;
                m_libCnvs.setFontSize(70);
                addy = (hi - 70) / 2;

                cnvsMsg.Background = Brushes.Red;
                m_libCnvs.setFillBrush(Brushes.White);
                aryMsg = sMsg.Split(',');
                m_libCnvs.drawCenterText(cnvsMsg, 0, 0, wd, hi, 0, addy, aryMsg[0]);
                if (aryMsg[0] == "発報中")
                {
                    m_libCnvs.setFillBrush(Brushes.Black);
                    m_libCnvs.setFontSize(30);
                    addy = hi - 50;
                    m_libCnvs.drawCenterText(cnvsMsg, 0, 0, wd, hi, 0, addy, aryMsg[1]);
                }
                if (sMsg == "持ち去り警報")
                {
                    m_btnReset = m_libCnvs.drawButton(cnvsMsg, wd - 120, 30, 120, 30, "解除");
                    m_btnReset.Click += (sender, e) => btnTReset_onClick(sender);
                }
                m_dsptCheckTime.Start();
            }
        }
        private void btnTReset_onClick(object sender)
        {
            m_wndMain.resetUDPFlag();
        }
        public void initCheckThread()
        {
            m_dsptCheckTime = new DispatcherTimer(DispatcherPriority.Normal);
            m_dsptCheckTime.Interval = TimeSpan.FromMilliseconds(500);
            m_dsptCheckTime.Tick += new EventHandler(checkThread);
        }
        private void checkThread(object sender, EventArgs e)
        {
            if(cnvsMsg.Background == Brushes.Red){
                cnvsMsg.Background = Brushes.White;
            }
            else
            {
                cnvsMsg.Background = Brushes.Red;
            }
        }
     }
}
