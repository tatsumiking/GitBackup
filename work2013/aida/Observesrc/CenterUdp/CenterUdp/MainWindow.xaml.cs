using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
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

namespace CenterUdp
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        public string m_sExecPath;
        public string m_sEnvPath;
        LibCommon m_libCmn;
        private int m_nIntervalMilliSecond;
        private DispatcherTimer m_dsptReceiveTimer;
        private Thread m_threadReceiveUDP;
        private Boolean m_bUDPIn;
        private string m_sSelfIP;
        private int m_nPort;
        private List<string> m_listMsg;
        private byte[] m_bGpsSKey;
        private byte[] m_bGpsEKey;

        public MainWindow()
        {
                m_bGpsSKey = System.Text.Encoding.UTF8.GetBytes("{{[[");
                m_bGpsEKey = System.Text.Encoding.UTF8.GetBytes("]]}}");
                m_sExecPath = initExePath();
                m_sEnvPath = initEnvPath();
                m_libCmn = new LibCommon();
                loadEnv();
                m_nIntervalMilliSecond = 100;
                m_listMsg = new List<string>();
            InitializeComponent();
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string sFileName;

            sFileName = m_sEnvPath + "\\log\\CenterList.log";
            App.setLogFileName(sFileName);

            try
            {
                m_dsptReceiveTimer = new DispatcherTimer(DispatcherPriority.Normal);
                m_dsptReceiveTimer.Interval = TimeSpan.FromMilliseconds(m_nIntervalMilliSecond);
                m_dsptReceiveTimer.Tick += new EventHandler(receiveUDPTimerTick);
                m_dsptReceiveTimer.Start();
            }
            catch (Exception ex)
            {//例外発生
                App.outLog("Window_Loaded");
            }
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            string sendMsg;

            try
            {
                m_dsptReceiveTimer.Stop();
                if (m_bUDPIn == true)
                {
                    sendMsg = "exit";
                    byte[] sendBytes = System.Text.Encoding.UTF8.GetBytes(sendMsg);
                    sendUdpData(sendBytes, m_sSelfIP, m_nPort);
                    while (true)
                    {
                        m_libCmn.DoDispatch();
                        if (m_bUDPIn == false)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    // receiveを防止して終了
                    m_bUDPIn = true;
                }
            }
            catch (Exception ex)
            {//例外発生
                App.outLog("Window_Closing");
            }
        }
        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void receiveUDPTimerTick(object sender, EventArgs e)
        {
            try
            {
                m_libCmn.DoDispatch();
                if (m_bUDPIn == false)
                {
                    m_threadReceiveUDP = new System.Threading.Thread(new System.Threading.ThreadStart(receiveUdpThread));
                    m_threadReceiveUDP.Start();
                }
            }
            catch (Exception ex)
            {//例外発生
                App.outLog("Window_Closing");
            }
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
            string sEnvOptPath;

            sEnvPath = "c:\\UsesProgram";
            try
            {
                if (!Directory.Exists(sEnvPath))
                {
                    Directory.CreateDirectory(sEnvPath);
                }
                sEnvPath = sEnvPath + "\\CenterList";
                if (!Directory.Exists(sEnvPath))
                {
                    Directory.CreateDirectory(sEnvPath);
                }
                sEnvOptPath = sEnvPath + "\\log";
                if (!Directory.Exists(sEnvOptPath))
                {
                    Directory.CreateDirectory(sEnvOptPath);
                }
                sEnvOptPath = sEnvPath + "\\card";
                if (!Directory.Exists(sEnvOptPath))
                {
                    Directory.CreateDirectory(sEnvOptPath);
                }
            }
            catch (Exception ex)
            {//例外発生
                App.outLog("initEnvPath");
            }
            return (sEnvPath);

        }
        private void loadEnv()
        {
            string sFileName;
            string[] aryLine;

            try
            {
                sFileName = m_sEnvPath + "\\CenterList.env";
                aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
                if (5 <= aryLine.Length)
                {
                    if (aryLine[1] != "")
                    {
                        m_sSelfIP = aryLine[1];
                    }
                    m_nPort = m_libCmn.StrToInt(aryLine[2]);
                }
                else
                {
                    m_sSelfIP = getSelfIpAddress();
                    m_nPort = 5547;
                }
            }
            catch (Exception ex)
            {//例外発生
                App.outLog("loadEnv");
            }
        }
        public string getSelfIpAddress()
        {
            int idx, max;
            string sAddress;
            string[] ary;

            try
            {
                string hostname = Dns.GetHostName();
                IPHostEntry ipInfo = Dns.GetHostEntry(hostname);
                max = ipInfo.AddressList.Length;
                for (idx = 0; idx < max; idx++)
                {
                    sAddress = ipInfo.AddressList[idx].ToString();
                    ary = sAddress.Split('.');
                    if (ary.Length == 4 && ary[0] == "192")
                    {
                        return (sAddress);
                    }
                }
            }
            catch (Exception ex)
            {//例外発生
                App.outLog("loadEnv");
            }
            return ("");
        }
        private void receiveUdpThread()
        {
            IPEndPoint remoteEP = null;
            string sIpAddress;
            byte[] rcvBytes;
            UdpClient udp;
            string sData;

            m_bUDPIn = true;
            m_libCmn.DoDispatch();
            udp = null;
            try
            {
                //udp = new UdpClient(selfEP);

                udp = new UdpClient(m_nPort);
                m_listMsg.Add("receive new" + m_nPort);
                this.Dispatcher.Invoke(updateControlInfo);
                rcvBytes = udp.Receive(ref remoteEP);
                sData = Encoding.UTF8.GetString(rcvBytes);
                m_listMsg.Add("receive data " + rcvBytes + "->" + sData.Substring(0, 10));
                this.Dispatcher.Invoke(updateControlInfo);
                udp.Close();
                m_bUDPIn = false;

                if (rcvBytes[0] == 'D' 
                && rcvBytes[1] == 'E' 
                && rcvBytes[2] == 'L')
                {
                    sIpAddress = remoteEP.Address.ToString();
                    updateSoutiNoFile(rcvBytes, sIpAddress);
                }
                else if (rcvBytes[0] == 'S'
                && rcvBytes[1] == 'E'
                && rcvBytes[2] == 'T')
                {
                    sIpAddress = remoteEP.Address.ToString();
                    updateSoutiNoFile(rcvBytes, sIpAddress);
                }
                else
                {
                    sendGPSBlock(rcvBytes);
                }
            }
            catch (Exception ex)
            {
                App.outLog(ex.ToString());
                if (udp != null)
                {
                    udp.Close();
                }
                m_bUDPIn = false;
                return;
            }
        }
        public void updateControlInfo()
        {
            int max, idx;
            string str;

            try
            {
                max = m_listMsg.Count;
                str = "";
                for (idx = 0; idx < max; idx++)
                {
                    str = str + m_listMsg[idx] + "\r\n";
                }
                txtMsg.Text = str;
                if (100 < max)
                {
                    m_listMsg.Clear();
                }
            }
            catch (Exception ex)
            {//例外発生
                App.outLog("updateControlInfo");
            }
        }

        private void updateSoutiNoFile(byte[] rcvBytes, string sIpAddress)
        {
            string sCard;
            string[] sLine;
            string sFileName;

            try
            {
                sCard = Encoding.UTF8.GetString(rcvBytes);
                sCard = sCard.Replace("\r\n", "|");
                sLine = sCard.Split('|');
                // sLine[2] = sIpAddress;
                sFileName = m_sEnvPath + "\\card\\" + sLine[1] + ".txt";
                m_libCmn.SaveFileLineSJIS(sFileName, sLine);
                m_listMsg.Add(sLine[0]+ " " + sLine[1]);
                if (10 <= m_listMsg.Count)
                {
                    m_listMsg.RemoveAt(0);
                }
                this.Dispatcher.Invoke(updateControlInfo);
            }
            catch (Exception ex)
            {//例外発生
                App.outLog("updateSoutiNoFile");
            }
        }
        private void sendGPSBlock(byte[] rcvBytes)
        {
            int max, i, j;
            string sGpsPos;
            string sSoutiNo;
            double dLat, dLnd;
            string sIPAddress;

            try
            {
                max = rcvBytes.Length - 5;
                byte[] strBytes = new byte[max];
                for (i = 0; i < max; i++)
                {
                    if ((m_bGpsSKey[0] == rcvBytes[i])
                     && (m_bGpsSKey[1] == rcvBytes[i + 1])
                     && (m_bGpsSKey[2] == rcvBytes[i + 2])
                     && (m_bGpsSKey[3] == rcvBytes[i + 3]))
                    {
                        break;
                    }
                }
                if (i == max)
                {
                    return;
                }
                for (i = i + 4, j = 0; i < max; i++, j++)
                {
                    if ((m_bGpsEKey[0] == rcvBytes[i])
                     && (m_bGpsEKey[1] == rcvBytes[i + 1])
                     && (m_bGpsEKey[2] == rcvBytes[i + 2])
                     && (m_bGpsEKey[3] == rcvBytes[i + 3]))
                    {
                        break;
                    }
                    strBytes[j] = rcvBytes[i];
                }
                for (; j < max; j++)
                {
                    strBytes[j] = 0;
                }
                sGpsPos = Encoding.UTF8.GetString(strBytes);
                string[] ary = sGpsPos.Split(',');
                dLat = m_libCmn.StrToDouble(ary[2]);
                dLnd = m_libCmn.StrToDouble(ary[3]);
                if (m_libCmn.IsZero(dLat) == false && m_libCmn.IsZero(dLnd) == false)
                {
                    sSoutiNo = m_libCmn.DeleteDoubleQuotation(ary[0]);
                    sIPAddress = getSoutiNotoIPAddress(sSoutiNo);
                    if (sIPAddress != "")
                    {
                        sendUdpData(rcvBytes, sIPAddress, m_nPort);
                    }
                }
            }
            catch (Exception ex)
            {//例外発生
                App.outLog("sendGPSBlock");
            }
        }
        private void sendUdpData(byte[] sendBytes, string sIPAddress, int nPort)
        {
            UdpClient udp;

            udp = new UdpClient();
            try
            {
                udp.Send(sendBytes, sendBytes.Length, sIPAddress, nPort);
            }
            catch (Exception ex)
            {
                App.outLog(ex.ToString());
                udp.Close();
                return;
            }
            udp.Close();
        }
        private string getSoutiNotoIPAddress(string sSoutiNo)
        {
            string sFileName;
            string[] sLine;
            string sIPAddress;

            sIPAddress = "";
            try
            {
                sFileName = m_sEnvPath + "\\card\\" + sSoutiNo + ".txt";
                sLine = m_libCmn.LoadFileLineSJIS(sFileName);
                if(3 <= sLine.Length){
                    sIPAddress = sLine[2];
                }
            }
            catch (Exception ex)
            {//例外発生
                App.outLog("getSoutiNotoIPAddress");
            }
            return (sIPAddress);
        }
    }
}
