using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace Observe
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        public string m_sExecPath;
        public string m_sEnvPath;
        public string m_sMdbPath;
        public string m_sSelfIP;
        public int m_nPort;
        public string m_sRemoteIP;
        public string m_sCenterIP;
        private Boolean m_bCheckMdb;
        private Thread m_rcvUDPThread;
        private UdpClient m_rcvUDPClient;
        private Boolean m_bUDPFlag;
        public Boolean m_bUDPInFlag;
        public LibCommon m_libCmn;
        public LibCanvas m_libCnvs;
        public LibOdbc m_libOdbc;
        public BlockWin m_blockWin;
        public UnderWin m_underWin;
        public CardWin m_cardWin;
        private DispatcherTimer m_dsptCheckTime;
        private DispatcherTimer m_dsptWaitTime;
        public string m_sMapPath;
        //実際の地図は+-4データあり 10   11    12    13    14     15     16     17      18
        private int[] m_tblTopX = { 903, 1810, 3624, 7253, 14511, 29026, 58057, 116119, 232243 };
        private int[] m_tblTopY = { 398,  800, 1605, 3215,  6435, 12874, 25748,  51496, 102992 };
        private int[] m_tblEndX = { 915, 1823, 3643, 7283, 14563, 29123, 58247, 116495, 232991 };
        private int[] m_tblEndY = { 410,  811, 1619, 3234,  6465, 12927, 25855,  51711, 103423 };
        //                               10         11         12         13         14
        //                               15         16         17         18
        private double[] m_tblTopLnd = { 137.46095, 138.1641,  138.51565, 138.73535, 138.8452
                                       , 138.8892,  138.91665, 138.93036, 138.937225, };
        private double[] m_tblTopLat = { 35.906629, 35.906629, 35.906629, 35.906629, 35.906629 
                                       , 35.90685,  35.906629, 35.906629, 35.906629, };
        private double[] m_tblLndBlock = { 0.351562496, 0.175781248, 0.087890624, 0.043945312, 0.021972656
                                          ,0.010986328, 0.005493164, 0.002746582, 0.001373291, 0};
        private double[] m_tblLatBlock = { -0.284444, -0.142222, -0.071111, -0.035555, -0.017777
                                         , -0.008888, -0.004444, -0.002222, -0.001111,  0};
        private int[] m_tblBaseY = { 402, 804, 1608, 3216, 6432, 12864, 25728, 51456, 102912, 0 };
        // 離島関係
        private double[] m_tblRitouSX = {232192, 232704, 234496, 0};
        private double[] m_tblRitouSY = {103936, 105984, 110336, 0};
        private double[] m_tblRitouEX = {232959, 232959, 234751, 0};
        private double[] m_tblRitouEY = {105727, 106239, 111103, 0};
        private double[] m_tblRitouSLnd = {138.8671834, 139.5703134, 142.0312534, 0};
        private double[] m_tblRitouSLat = {34.88594011, 32.54681976,27.37177233, 0};
        private double[] m_tblRitouELnd = {139.9205034, 139.9205034,142.3814434, 0};
        private double[] m_tblRitouELat = {32.84382011, 32.25113976,26.43245233, 0};
        private double[] m_tblRitouLndBlock = { 0.001373294, 0.001373294,0.001373294, 0};
        private double[] m_tblRitouLatBlock = { -0.001140212, -0.001159529,-0.001224668, 0};
        private double m_dBaseLat;
        private int m_n18BlockLatLast;
        private double m_d18BlockLatAdd; 
        private double m_dStepLatSub;
        private int m_nCanvasWidth, m_nCanvasHeight;
        private int m_nWidthDiv, m_nHeightDiv;
        private int m_nCenterXBlock, m_nCenterYBlock;
        private int m_nCenterXAdd, m_nCenterYAdd;
        private double m_dLatDotStep, m_dLndDotStep;
        private int m_nLastX, m_nLastY;
        private string[] m_aryPlaceNameLine;
        private Boolean m_bRetouMode;
        private Image m_imgCamera;
        private TextBlock m_tbCrt;
        public int m_nClsCardCrtIdx;
        private ClsCard m_clsCardBack;
        private ClsCard m_clsCardNew;
        public ClsObserve m_clsObserve;

        public MainWindow()
        {
            InitializeComponent();
            m_sExecPath = initExePath();
            m_libCmn = new LibCommon();
            m_libCnvs = new LibCanvas();
            m_libOdbc = new LibOdbc();
            m_libOdbc.setLibCommonClass(m_libCmn);
            m_libOdbc.setExecPath(m_sExecPath);

            m_sEnvPath = initEnvPath();
            m_libOdbc.setEnvPath(m_sEnvPath);
            loadEnv();

            odbcLoadEnv();

            m_sMapPath = m_sEnvPath + "\\東京都";
            m_dBaseLat = 36.03150;
            m_d18BlockLatAdd = -0.001111;
            m_dStepLatSub = -0.0000001579;
            //m_d18BlockLatAdd = -0.0011185;
            //m_dStepLatSub = -0.0000001578;
            //m_d18BlockLatAdd = -0.001111;
            //m_dStepLatSub = -0.0000006;
            m_n18BlockLatLast = m_tblEndY[8] - m_tblBaseY[8];
            m_bRetouMode = false;
            m_tbCrt = null;
            m_clsObserve = null;
            m_nClsCardCrtIdx = -1;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Type type;
            string sXmlFile;

            //this.MaximizeBox
            sXmlFile = m_sEnvPath + "\\Observe.xml";
            type = typeof(ClsObserve);
            m_clsObserve = (ClsObserve)m_libCmn.DataXmlLoad(sXmlFile, type);
            if (m_clsObserve == null)
            {
                m_clsObserve = new ClsObserve();
            }
            setCrtCardIdx(-1);
            m_clsCardNew = null;

            m_cnvsMove = new Canvas();

            initMouseEvent();
            initMapElement();
            initCmbGroup();
            initCmbPlaceName();
            m_dZoomTime = 1.0;
            m_cnvsMove.RenderTransform = null;
            initMapArea();
            initBlockWin();
            initUnderWin();
            m_cardWin = null;
            startCheckThread();
            //m_underWin.dispMsg("");
            //m_underWin.dispMsg("持ち去り警報");
            //m_underWin.dispMsg("センサー発報中　インプット 7 作動検知");
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Type type;
            string sXmlFile;

            saveCrtTizuPos();
            if (m_clsObserve == null)
            {
                return;
            }
            sXmlFile = m_sEnvPath + "\\Observe.xml";
            type = typeof(ClsObserve);
            m_libCmn.DataXmlSave(sXmlFile, type, m_clsObserve);
        }
        public void setCrtCardIdx(int idx)
        {
            m_nClsCardCrtIdx = idx;
        }
        public int getCrtCardIdx()
        {
            return (m_nClsCardCrtIdx);
        }
        public void startCheckThread()
        {
            m_dsptCheckTime = new DispatcherTimer(DispatcherPriority.Normal);
            m_dsptCheckTime.Interval = TimeSpan.FromMilliseconds(1000 * 10);
            m_dsptCheckTime.Tick += new EventHandler(checkThread);
            m_dsptCheckTime.Start();
        }
        public void resetUDPFlag()
        {
            int max, idx;

            max = m_clsObserve.m_lstClsCard.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (m_clsObserve.m_lstClsCard[idx].m_sStat == "2")
                {
                    m_clsObserve.m_lstClsCard[idx].m_sStat = "";
                    setUnderMsg("");
                    setBlockWin();
                }
            }
            m_bUDPFlag = false;
            m_bUDPInFlag = false;
        }
        public void setCheckStartUdp()
        {
            m_bUDPFlag = true;
        }
        private void checkThread(object sender, EventArgs e)
        {
            checkMdbElement();
            if (m_bUDPFlag == true)
            {
                if(m_bUDPInFlag == true)
                {
                    udpSendThreadExit();
                }
                else
                {
                    drawCardMark(); // GPSマーク表示

                    m_rcvUDPThread = new System.Threading.Thread(new System.Threading.ThreadStart(checkUdpThread));
                    m_rcvUDPThread.Start();
                }
            }
        }

        public void checkUdpThread()
        {
            m_bUDPInFlag = true;
            udpReceiveGPSData();
            m_bUDPInFlag = false;
        }
        private int searchCardIdx(String sId)
        {
            int max, idx;

            max = m_clsObserve.m_lstClsCard.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (m_clsObserve.m_lstClsCard[idx].m_sSetNo == sId)
                {
                    return (idx);
                }
            }
            return (-1);
        }
        private void setUnderMsg(String sMsg)
        {
            m_underWin.dispMsg(sMsg);
        }
        private void setBlockWin()
        {
            m_blockWin.SetListElement();
        }
        private void dispMameoryCrt()
        {
            long currentMem = Environment.WorkingSet;
            long currentCGMem = Environment.WorkingSet;
            //long currentCGMem = GC.GetTotalMemory(true);
            currentMem = currentMem / 1024;
            currentMem = currentMem / 1024;
            currentCGMem = currentCGMem / 1024;
            currentCGMem = currentCGMem / 1024;
        }
        private string initExePath()
        {
            string path;

            Assembly myAssembly = Assembly.GetEntryAssembly();
            path = myAssembly.Location;
            return (path.Substring(0, path.LastIndexOf("\\")));
        }
        private string initEnvPath()
        {
            string sEnvPath;
            string sOptFIleName;
            string[] aryLine;

            sEnvPath = "c:\\Observe";
            sOptFIleName = m_sExecPath + "\\observeopt.txt";
            aryLine = m_libCmn.LoadFileLineSJIS(sOptFIleName);
            if (aryLine[0] != "")
            {
                sEnvPath = aryLine[1];
            }
            m_libCmn.CreatePath(sEnvPath);
            return (sEnvPath);

        }
        private void loadEnv()
        {
            string sFileName;
            string[] aryLine;

            sFileName = m_sEnvPath + "\\Observe.env";
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            if (5 <= aryLine.Length)
            {
                m_sMdbPath = aryLine[1];
                m_sSelfIP = aryLine[2];
                m_nPort = m_libCmn.StrToInt(aryLine[3]);
                m_sRemoteIP = aryLine[4];
                m_sCenterIP = aryLine[5];
            }
            else
            {
                m_sMdbPath = "C:\\Center V2\\EventLog";
                m_sSelfIP = getSelfIpAddress();
                m_nPort = 5547;
                m_sRemoteIP = "";
                m_sCenterIP = "172.16.10.210";
            }
        }
        public string getSelfIpAddress()
        {
            int idx, max;
            string sAddress;
            string[] ary;

            string hostname = Dns.GetHostName();
            IPHostEntry ipInfo = Dns.GetHostEntry(hostname);
            max = ipInfo.AddressList.Length;
            for (idx = 0; idx < MaxHeight; idx++)
            {
                sAddress = ipInfo.AddressList[idx].ToString();
                ary = sAddress.Split('.');
                if (ary.Length == 4 && ary[0] == "192")
                {
                    return (sAddress);
                }
            }
            return("");
        }
        public Boolean checkRemoteIP(IPAddress checkip)
        {
            if (m_sRemoteIP == "")
            {
                return (true);
            }
            IPAddress ipadrs = IPAddress.Parse(m_sRemoteIP);
            if (checkip == ipadrs)
            {
                return (true);
            }
            return (false);
        }
        public void ResetEnv()
        {
            if (m_cnvsMove == null)
            {
                return;
            }
            m_dZoomTime = 1.0;
            m_cnvsMove.RenderTransform = null;
            initMapArea();
        }
        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (m_cnvsMove == null)
            {
                return;
            }
            m_dZoomTime = 1.0;
            m_cnvsMove.RenderTransform = null;
            initMapArea();
        }
        private void initCmbGroup()
        {
            cmbGroup.Items.Add("23区");
            cmbGroup.Items.Add("市町");
            cmbGroup.Items.Add("離島");
            cmbGroup.SelectedIndex = 0;
            m_bRetouMode = false;
        }
        private void cmbGroup_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (cmbGroup != null && cmbPlaceName != null)
            {
                initCmbPlaceName();
            }
        }
        private void initCmbPlaceName()
        {
            string sFileName;
            int idx, max;
            string[] ary;

            cmbPlaceName.Items.Clear();
            if (cmbGroup.SelectedIndex == 0)
            {
                m_bRetouMode = false;
                sFileName = m_sExecPath + "\\ku.txt";
            }
            else if (cmbGroup.SelectedIndex == 1)
            {
                m_bRetouMode = false;
                sFileName = m_sExecPath + "\\si.txt";
            }
            else
            {
                m_bRetouMode = false;
                sFileName = m_sExecPath + "\\tou.txt";
            }
            m_aryPlaceNameLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = m_aryPlaceNameLine.Length;
            for (idx = 1; idx < max; idx++)
            {
                if (m_aryPlaceNameLine[idx] == "" || m_aryPlaceNameLine[idx] == " ")
                {
                    break;
                }
                ary = m_aryPlaceNameLine[idx].Split(',');
                cmbPlaceName.Items.Add(ary[0]);
            }
        }
        private void initBlockWin()
        {
            m_blockWin = new BlockWin();
            m_blockWin.SetMainWindow(this);
            m_blockWin.Owner = this;
            m_blockWin.Show();
        }
        private void initUnderWin()
        {
            m_underWin = new UnderWin();
            m_underWin.SetMainWindow(this);
            m_underWin.Owner = this;
            m_underWin.Show();
        }
        private void initMapElement()
        {
            int width;
            int height;

            width = (int)gridDrawArea.ActualWidth;
            height = (int)gridDrawArea.ActualHeight;
            m_nCanvasWidth = width;
            m_nCanvasHeight = height;
            m_nCenterXBlock = (m_nCanvasWidth / 2) / Constants.MAPDOTSIZE;
            m_nCenterYBlock = (m_nCanvasHeight / 2) / Constants.MAPDOTSIZE;
            m_nCenterXAdd = m_nCanvasWidth - m_nCenterXBlock * Constants.MAPDOTSIZE;
            m_nCenterYAdd = m_nCanvasHeight - m_nCenterXBlock * Constants.MAPDOTSIZE;

            m_nWidthDiv = m_nCanvasWidth / Constants.MAPDOTSIZE;
            if (m_nCanvasWidth % Constants.MAPDOTSIZE != 0)
            {
                m_nWidthDiv++;
            }
            m_nHeightDiv = m_nCanvasHeight / Constants.MAPDOTSIZE;
            if (m_nCanvasHeight % Constants.MAPDOTSIZE != 0)
            {
                m_nHeightDiv++;
            }

            m_nLastX = m_tblEndX[m_nMapTblIdx] - m_tblTopX[m_nMapTblIdx] - m_nWidthDiv * 3 + 2;
            m_nLastY = m_tblEndY[m_nMapTblIdx] - m_tblTopY[m_nMapTblIdx] - m_nHeightDiv * 3 + 2;

            m_cnvsMove.Width = m_nWidthDiv * 3 * Constants.MAPDOTSIZE;
            m_cnvsMove.Height = m_nHeightDiv * 3 * Constants.MAPDOTSIZE;
        }
        private void timerWait()
        {
            m_dsptWaitTime = new DispatcherTimer(DispatcherPriority.Normal);
            m_dsptWaitTime.Interval = TimeSpan.FromMilliseconds(50);
            m_dsptWaitTime.Tick += new EventHandler(TickWaitTimer);
            m_dsptWaitTime.Start();
        }
        private void TickWaitTimer(object sender, EventArgs e)
        {
            m_dsptWaitTime.Stop();
        }
        public void moveLatLnd(ClsLatLnd clsLatLnd)
        {
            ClsPagePosXY clsPagePosXY;
            int setxPage, setyPage;
            int setxdot, setydot;

            //m_nMapBase = 15;
            //setTableElement(); // m_nMapTblIdx, m_nLastX, m_nLastY, m_dLatDotStep, m_dLndDotStep 設定

            // ページ数＋ページ位置算出
            clsPagePosXY = convLatLndToRltvPagePosXY(clsLatLnd);
            // ページ数セット
            setxPage = (int)(clsPagePosXY.m_dPagePosX);
            setyPage = (int)(clsPagePosXY.m_dPagePosY);
            // ドット数
            setxdot = (int)((clsPagePosXY.m_dPagePosX - setxPage) * Constants.MAPDOTSIZE);
            setydot = (int)((clsPagePosXY.m_dPagePosY - setyPage) * Constants.MAPDOTSIZE);

            m_nCrtX = setxPage - m_nCenterXBlock - m_nWidthDiv;
            m_nCrtY = setyPage - m_nCenterYBlock - m_nHeightDiv;
            int nPageX = m_nCrtX + m_tblTopX[m_nMapTblIdx];
            int nPageY = m_nCrtY + m_tblTopY[m_nMapTblIdx];
            m_nAddX = m_nCanvasWidth / 2 - (m_nCenterXBlock * Constants.MAPDOTSIZE + setxdot);
            m_nAddY = m_nCanvasHeight / 2 - (m_nCenterYBlock * Constants.MAPDOTSIZE + setydot);
            m_nAddX = m_nAddX - m_nWidthDiv * Constants.MAPDOTSIZE;
            m_nAddY = m_nAddY - m_nHeightDiv * Constants.MAPDOTSIZE;
            m_nMoveX = 0;
            m_nMoveY = 0;
            m_dZoomTime = 1.0;
            m_nZoomAddX = 0;
            m_nZoomAddY = 0;
            m_cnvsMove.RenderTransform = null;
            initMapArea();
        }
        private ClsPagePosXY convLatLndToRltvPagePosXY(ClsLatLnd clsLatLnd)
        {
            ClsPagePosXY clsPagePosXY;
            double dSubLat, dSubLnd;
            int idx;
            double dPage18X, dPage18Y;
            double dLnd18DotStep, dLat18DotStep;
            int nLevel;

            clsPagePosXY = new ClsPagePosXY();
            if (m_tblRitouSLat[0] < clsLatLnd.m_dLat)
            {
                dSubLnd = (clsLatLnd.m_dLnd - m_tblTopLnd[m_nMapTblIdx]);
                clsPagePosXY.m_dPagePosX = dSubLnd / m_tblLndBlock[m_nMapTblIdx];
                m_dLndDotStep = m_tblLndBlock[m_nMapTblIdx] / Constants.MAPDOTSIZE;
                if (clsPagePosXY.m_dPagePosX < 0)
                {
                    clsPagePosXY.m_dPagePosX = 0;
                }
                if ((double)(m_nLastX + m_nWidthDiv * 3) < clsPagePosXY.m_dPagePosX)
                {
                    clsPagePosXY.m_dPagePosX = (double)m_nLastX;
                }
                dSubLat = clsLatLnd.m_dLat - m_dBaseLat;
                clsPagePosXY.m_dPagePosY = getLatBlockPagePos(dSubLat);
            }
            else{
                if (m_tblRitouSLat[1] < clsLatLnd.m_dLat) // 大島 三宅島 御蔵島 八丈島
                {
                    idx = 0;
                }
                else if (m_tblRitouSLat[2] < clsLatLnd.m_dLat) // 青ヶ島
                {
                    idx = 1;
                }
                else
                {
                    idx = 2;
                }
                dSubLnd = (clsLatLnd.m_dLnd - m_tblRitouSLnd[idx]);
                dPage18X = m_tblRitouSX[idx] + dSubLnd / m_tblRitouLndBlock[idx];

                dSubLat = (clsLatLnd.m_dLat - m_tblRitouSLat[idx]);
                dPage18Y = m_tblRitouSY[idx] + dSubLat / m_tblRitouLatBlock[idx];

                dLnd18DotStep = m_tblRitouLndBlock[idx] / Constants.MAPDOTSIZE;
                dLat18DotStep = m_tblRitouLatBlock[idx] / Constants.MAPDOTSIZE;
                m_dLndDotStep = dLnd18DotStep;
                m_dLatDotStep = dLat18DotStep;
                clsPagePosXY.m_dPagePosX = dPage18X;
                clsPagePosXY.m_dPagePosY = dPage18Y;
                for(idx = 8; idx >= m_nMapTblIdx; idx--){
                    m_dLndDotStep = m_dLndDotStep * 2;
                    m_dLatDotStep = m_dLatDotStep * 2;
                    clsPagePosXY.m_dPagePosX = clsPagePosXY.m_dPagePosX * 0.5;
                    clsPagePosXY.m_dPagePosY = clsPagePosXY.m_dPagePosY * 0.5;
                }
            }
            return (clsPagePosXY);
        }
        public double getLatBlockPagePos(double dSubLat)
        {
            double  dCrtSubLat;
            double  d18AddStep;
            double  dPagePos;
            int     subPage;
            int     n18LastY;
            int     idx;

            dCrtSubLat = 0;
            d18AddStep = m_d18BlockLatAdd;
            dPagePos = dSubLat / d18AddStep;
            dPagePos = 0.0;
            n18LastY = m_tblEndY[8] - m_tblBaseY[8];
            // 18z時のページポジションを求める
            for (subPage = 10; subPage < n18LastY; subPage += 10)
            {
                if((dCrtSubLat + d18AddStep * 10) < dSubLat){
                    dPagePos = subPage - 10;
                    dPagePos = dPagePos + ((dSubLat - dCrtSubLat) / (d18AddStep));
                    break;
                }
                dCrtSubLat = dCrtSubLat + d18AddStep * 10;
                d18AddStep = d18AddStep + m_dStepLatSub;
            }
            m_dLatDotStep = d18AddStep / Constants.MAPDOTSIZE;
            for(idx = 8; idx >= 0; idx--){
                if(m_nMapTblIdx == idx){
                    dPagePos = dPagePos + m_tblBaseY[m_nMapTblIdx] - m_tblTopY[m_nMapTblIdx];
                    if(dPagePos < 0){
                        dPagePos = 0;
                    }
                    if((double)(m_nLastY + m_nHeightDiv*3) < dPagePos){
                        dPagePos = (double)m_nLastY;
                    }
                    return(dPagePos);
                }
                m_dLatDotStep = m_dLatDotStep * 2;
                dPagePos = dPagePos / 2.0;
            }
            return (0.0);
        }
        public void selectCrtCardWin()
        {
            if (m_cardWin != null)
            {
                m_cardWin.Left = 380;
                m_cardWin.Top = 240;
                //m_cardWin.Activate();
            }
        }
        public void setCrtCardWinDisp()
        {
            int nCrtIdx;
            ClsCard clsCard;
            ClsLatLnd clsLatLnd;

            if (m_cardWin != null)
            {
                return;
            }
            nCrtIdx = getCrtCardIdx();
            m_cardWin = new CardWin();
            m_cardWin.SetMainWindow(this);
            if (nCrtIdx == -1)
            {
                m_clsCardBack = null;
                m_clsCardNew = new ClsCard();
                nCrtIdx = m_clsObserve.m_lstClsCard.Count;
                m_clsObserve.m_lstClsCard.Add(m_clsCardNew);
                setCrtCardIdx(nCrtIdx);
                m_cardWin.SetClsCard(m_clsCardNew);
            }
            else
            {
                m_clsCardNew = null;
                m_clsCardBack = new ClsCard();
                m_clsCardBack.copySetElement(m_clsObserve.m_lstClsCard[nCrtIdx]);
                clsCard = m_clsCardBack;
                clsLatLnd = new ClsLatLnd();
                clsLatLnd.m_dLat = clsCard.m_dLat;
                clsLatLnd.m_dLnd = clsCard.m_dLnd;
                moveLatLnd(clsLatLnd);
                m_cardWin.SetClsCard(clsCard);
            }
            m_cardWin.Owner = this;
            m_cardWin.Show();
        }
        public void RemoveNewCard()
        {
            if (m_clsCardNew != null)
            {
                m_clsObserve.m_lstClsCard.Remove(m_clsCardNew);
            }
            setCrtCardIdx(-1);
            m_blockWin.SetListElement();
            drawCardMark();
        }
        public Boolean checkSamebSetNo(string sSetNo)
        {
            int idx, max;
            int nCrtIdx;
            string sIdxSetNo;

            nCrtIdx = getCrtCardIdx();
            max = m_clsObserve.m_lstClsCard.Count;
            for (idx = 0; idx < max; idx++)
            {

                if (m_clsObserve.m_lstClsCard[idx].m_sSetNo == sSetNo)
                {
                    if (nCrtIdx != idx)
                    {
                        return (true);
                    }
                }
            }
            return (false);
        }
        public void DeleteCrtCard()
        {
            int nCrtIdx;

            if (m_clsCardNew == null)
            {
                nCrtIdx = getCrtCardIdx();
                udpSendCardDeleteMessage(m_clsObserve.m_lstClsCard[nCrtIdx]);
                m_clsObserve.m_lstClsCard.RemoveAt(nCrtIdx);
            }
            else
            {
                m_clsObserve.m_lstClsCard.Remove(m_clsCardNew);
            }
            setCrtCardIdx(-1);
            m_blockWin.SetListElement();
            drawCardMark();
        }
        public void SetCrtCardElement(ClsCard clsCard)
        {
            int nCrtIdx;

            nCrtIdx = getCrtCardIdx();
            if (nCrtIdx == -1)
            {
                return;
            }
            m_clsObserve.m_lstClsCard[nCrtIdx].copySetElement(clsCard);
            udpSendCardSetMessage(clsCard);
            setCrtCardIdx(-1);
            m_blockWin.SetListElement();
            drawCardMark();
        }
        public void ResetClsCardElement()
        {
            int nCrtIdx;

            nCrtIdx = getCrtCardIdx();
            if(nCrtIdx == -1)
            {
                return;
            }
            if (m_clsCardBack == null)
            {
                // m_clsObserve.m_lstClsCard.Clear();
                setCrtCardIdx(-1);
                return;
            }
            m_clsObserve.m_lstClsCard[nCrtIdx].copySetElement(m_clsCardBack);
            drawCardMark();
        }
        public void SetCrtCardLatLnd(double dLat, double dLnd)
        {
            int nCrtIdx;

            nCrtIdx = getCrtCardIdx();
            if (nCrtIdx == -1)
            {
                return;
            }
            m_clsObserve.m_lstClsCard[nCrtIdx].m_dLat = dLat;
            m_clsObserve.m_lstClsCard[nCrtIdx].m_dLnd = dLnd;

            drawCardMark();
        }
        private void cmbPlaceName_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string sPlace;
            string[] ary;
            int nSXBlock;
            int nSYBlock;
            ClsLatLnd clsLatLnd;

            int idx = cmbPlaceName.SelectedIndex;
            if (idx == -1)
            {
                return;
            }
            sPlace = m_aryPlaceNameLine[idx + 1];
            ary = sPlace.Split(',');
            if (cmbGroup.SelectedIndex == 2)
            {
                m_nMapBase = 15;
                m_nMapTblIdx = m_nMapBase - 10;
                m_nLastX = m_tblEndX[m_nMapTblIdx] - m_tblTopX[m_nMapTblIdx] - m_nWidthDiv * 3 + 2;
                m_nLastY = m_tblEndY[m_nMapTblIdx] - m_tblTopY[m_nMapTblIdx] - m_nHeightDiv * 3 + 2;
                m_bRetouMode = true;
                nSXBlock = m_libCmn.StrToInt(ary[3]) - m_tblTopX[m_nMapTblIdx];
                nSYBlock = m_libCmn.StrToInt(ary[4]) - m_tblTopY[m_nMapTblIdx];
                m_nCrtX = nSXBlock - m_nWidthDiv;
                m_nCrtY = nSYBlock - m_nHeightDiv;
                m_nAddX = -m_nWidthDiv * Constants.MAPDOTSIZE;
                m_nAddY = -m_nHeightDiv * Constants.MAPDOTSIZE;
                m_dZoomTime = 1.0;
                m_cnvsMove.RenderTransform = null;
                initMapArea();
            }
            else
            {
                m_bRetouMode = false;
                clsLatLnd = new ClsLatLnd();
                clsLatLnd.m_dLat = m_libCmn.StrToDouble(ary[1]);
                clsLatLnd.m_dLnd = m_libCmn.StrToDouble(ary[2]);
                moveLatLnd(clsLatLnd);
            }
        }
        private void btnMove_Click(object sender, RoutedEventArgs e)
        {
            string sAddress;
            ClsLatLnd clsLatLnd;

            sAddress = txtAddress.Text;
            clsLatLnd = getAddressToLatLnd(sAddress);
            moveLatLnd(clsLatLnd);
        }
        public ClsLatLnd getAddressToLatLnd(String sAddress)
        {
            ClsLatLnd latlnd;
            OdbcConnection con;
            int max, len;
            bool bRet;
            string sBeforeSql;
            string sSql;
            OdbcCommand com;
            OdbcDataReader reader;
            string sSubStr;

            latlnd = new ClsLatLnd();
            latlnd.m_dLat = 0;
            latlnd.m_dLnd = 0;
            sAddress = m_libCmn.StrNumToKan(sAddress);
            if (3 < sAddress.Length && sAddress.Substring(0, 3) == "東京都")
            {
                sAddress = sAddress.Substring(3);
            }
            con = m_libOdbc.openMdb();
            if (con != null)
            {
                sBeforeSql = "";
                max = sAddress.Length;
                for(len = 2; len <= max; len++){
                    sSubStr = sAddress.Substring(0, len);
                    sSql = "SELECT * FROM adrslatlnd";
                    sSql = sSql + " WHERE adrs LIKE '" + sSubStr + "%';";
                    // 前後のあいまいさを排除
                    //sSql = sSql + " WHERE adrs LIKE '%" + sSubStr + "%';";
                    com = new OdbcCommand(sSql, con);
                    try
                    {
                        reader = com.ExecuteReader();
                        bRet = reader.Read();
                        if(bRet == true){
                            sBeforeSql = sSql;
                        }else{
                            break;
                        }
                    }
                    catch (Exception ex)
                    {
                    }
                }
                com = new OdbcCommand(sBeforeSql, con);
                try
                {
                    reader = com.ExecuteReader();
                    while (reader.Read())
                    {
                        //len = reader.GetInt32(0);
                        //sSubStr = reader.GetString(1);
                        latlnd.m_dLat = reader.GetDouble(2);
                        latlnd.m_dLnd = reader.GetDouble(3);
                        //txtInfo.Text = sSubStr + "(" + latlnd.m_dLat + "," + latlnd.m_dLnd + ")";
                        break;
                    }
                }
                catch (Exception ex)
                {
                }
                m_libOdbc.closeMdb(con);
            }
            return (latlnd);
        }

    }
}
