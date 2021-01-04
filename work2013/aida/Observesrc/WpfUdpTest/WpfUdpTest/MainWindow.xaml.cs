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

namespace WpfUdpTest
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        public string m_sExecPath;
        public string m_sEnvPath;
        private int m_nUdpMode; // 0→送信　1→受信
        private DispatcherTimer m_dsptSendTimer;
        private int m_nIntervalMilliSecond;
        private DispatcherTimer m_dsptReceiveTimer;
        private int m_nCount;
        private int m_nThreadCount;
        private Thread m_threadSendUDP;
        private Thread m_threadReceiveUDP;
        private Boolean m_bUDPIn;
        private string m_sMessage;
        private string m_sSelfIpAddress;
        private string m_sIpAddress;
        private int m_nPort;
        private string m_sInformation;

        public MainWindow()
        {
            m_sExecPath = initExePath();
            m_sEnvPath = initEnvPath();
            InitializeComponent();
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string sFileName;

            int idx, max;
            string sAddress;
            string[] ary;

            sFileName = m_sEnvPath + "\\log\\WpfUdpTest.log";
            App.setLogFileName(sFileName);
            m_dsptSendTimer = null;
            m_dsptReceiveTimer = null;
            m_nIntervalMilliSecond = 1000;
            m_nUdpMode = 0;
            m_bUDPIn = false;
            m_nThreadCount = 0;
            m_sMessage = "送信テスト用の文字列を\r\n入力してください";
            m_sSelfIpAddress = "192.168.1.1";
            m_nPort = 12345;
            m_sInformation = "IPアドレスを送信先のIPアドレスに変更し送信開始ボタンを押してください";
            string hostname = Dns.GetHostName();
            IPHostEntry ipInfo = Dns.GetHostEntry(hostname);
            max = ipInfo.AddressList.Length;
            for (idx = 0; idx < max; idx++)
            {
                sAddress = ipInfo.AddressList[idx].ToString();
                ary = sAddress.Split('.');
                if (ary.Length == 4)
                {
                    cmbSelfIpAddress.Items.Add(sAddress);
                    if (ary[0] == "192")
                    {
                        m_sSelfIpAddress = sAddress;
                    }
                }
            }
            //sAddress = "219.116.92.193";
            sAddress = "172.16.10.210";
            cmbSelfIpAddress.Items.Add(sAddress);

            m_sIpAddress = m_sSelfIpAddress;
            cmbSelfIpAddress.Text = m_sSelfIpAddress;
            txtMessage.Text = m_sMessage;
            txtIpAddress.Text = m_sIpAddress;
            txtPort.Text = m_nPort.ToString();
            lblInformation.Content = m_sInformation;
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            MessageBoxResult result;
            result = MessageBox.Show("終了して宜しいですか", "確認", MessageBoxButton.YesNo);
            if (result == MessageBoxResult.No)
            {
                e.Cancel = true;
            }
            if (m_dsptSendTimer != null)
            {
                sendStopTimer();
            }
            if (m_dsptReceiveTimer != null)
            {
                receiveStopTimer();
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
            if (!Directory.Exists(sEnvPath))
            {
                Directory.CreateDirectory(sEnvPath);
            }
            sEnvPath = sEnvPath + "\\WpfUdpTest";
            if (!Directory.Exists(sEnvPath))
            {
                Directory.CreateDirectory(sEnvPath);
            }
            sEnvOptPath = sEnvPath + "\\log";
            if (!Directory.Exists(sEnvOptPath))
            {
                Directory.CreateDirectory(sEnvOptPath);
            }
            return (sEnvPath);

        }
        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        public void updateControlInfo()
        {
            lblInformation.Content = m_sInformation;
            m_sIpAddress = txtIpAddress.Text;
            m_nPort = StrToInt(txtPort.Text);
            if (m_nUdpMode == 0)
            {
                m_sMessage = txtMessage.Text;
            }
            else
            {
                txtMessage.Text = m_sMessage;
            }
        }
        private void rdoSend_Click(object sender, RoutedEventArgs e)
        {
            m_nUdpMode = 0;
            btnUdpStart.Content = "送信開始";
            btnUdpStop.Content = "送信終了";
            m_sMessage = "送信テスト用の文字列を\r\n入力してください";
            m_sInformation = "IPアドレスを送信先のIPアドレスに変更し送信開始ボタンを押してください";
            updateControlInfo();
        }
        private void rdoReceive_Click(object sender, RoutedEventArgs e)
        {
            m_nUdpMode = 1;
            btnUdpStart.Content = "受信開始";
            btnUdpStop.Content = "受信終了";
            m_sSelfIpAddress = cmbSelfIpAddress.Text;
            txtIpAddress.Text = m_sSelfIpAddress;
            m_sInformation = "IPアドレスが自分のIPアドレスか確認し受信開始ボタンを押してください";
            updateControlInfo();
        }
        private void btnUdpStart_Click(object sender, RoutedEventArgs e)
        {
            if (m_nUdpMode == 0)
            {
                if (m_dsptReceiveTimer != null)
                {
                    receiveStopTimer();
                }
                if (m_dsptSendTimer == null)
                {
                    m_sInformation = "UDP送信開始";
                    m_dsptSendTimer = new DispatcherTimer(DispatcherPriority.Normal);
                    m_dsptSendTimer.Interval = TimeSpan.FromMilliseconds(m_nIntervalMilliSecond);
                    m_dsptSendTimer.Tick += new EventHandler(sendUDPTimerTick);
                    m_dsptSendTimer.Start();
                    m_nCount = 1;
                }
            }
            else
            {
                if (m_dsptSendTimer != null)
                {
                    sendStopTimer();
                }
                if (m_dsptReceiveTimer == null)
                {
                    m_sInformation = "UDP受信開始";
                    this.Dispatcher.Invoke(updateControlInfo);
                    m_dsptReceiveTimer = new DispatcherTimer(DispatcherPriority.Normal);
                    m_dsptReceiveTimer.Interval = TimeSpan.FromMilliseconds(m_nIntervalMilliSecond);
                    m_dsptReceiveTimer.Tick += new EventHandler(receiveUDPTimerTick);
                    m_dsptReceiveTimer.Start();
                    m_nCount = 1;
                }
            }
        }
        private void btnUdpStop_Click(object sender, RoutedEventArgs e)
        {
            if (m_nUdpMode == 0)
            {
                if (m_dsptSendTimer != null)
                {
                    sendStopTimer();
                }
            }
            else
            {
                if (m_dsptReceiveTimer != null)
                {
                    receiveStopTimer();
                }
            }
        }
        private void sendStopTimer()
        {
            m_sInformation = "UDP送信停止";
            this.Dispatcher.Invoke(updateControlInfo);
            m_dsptSendTimer.Stop();
            m_dsptSendTimer = null;
            DoEvents();
        }
        private void receiveStopTimer()
        {
            m_sInformation = "UDP受信停止";
            this.Dispatcher.Invoke(updateControlInfo);
            m_dsptReceiveTimer.Stop();
            m_dsptReceiveTimer = null;
            DoEvents();
            exitReceiveThread(m_sSelfIpAddress, m_nPort);
        }
        private void sendUDPTimerTick(object sender, EventArgs e)
        {
            m_threadSendUDP = new System.Threading.Thread(new System.Threading.ThreadStart(sendUdpThread));
            m_threadSendUDP.Start();
            m_nThreadCount++;
        }
        private void sendUdpThread()
        {
            UdpClient udp;

            m_sInformation = "UDP送信" + m_nCount + "回目";
            this.Dispatcher.Invoke(updateControlInfo);
            DoEvents();
            byte[] sendBytes = System.Text.Encoding.UTF8.GetBytes(m_sMessage);
            try
            {
                udp = new UdpClient();
            }
            catch (Exception ex1)
            {
                App.outLog(ex1.ToString());
                return;
            }
            try
            {
                udp.Send(sendBytes, sendBytes.Length, m_sIpAddress, m_nPort);
            }
            catch (Exception ex2)
            {
                App.outLog(ex2.ToString());
                udp.Close();
                m_nThreadCount--;
                return;
            }
            udp.Close();
            m_nCount++;
            m_nThreadCount--;
        }
        private void receiveUDPTimerTick(object sender, EventArgs e)
        {
            if (m_bUDPIn == true)
            {
                m_dsptReceiveTimer.Stop();
                exitReceiveThread(m_sSelfIpAddress, m_nPort);
                while (true)
                {
                    if (m_bUDPIn == true)
                    {
                        break;
                    }
                    Thread.Sleep(50);
                }
                m_dsptReceiveTimer.Start();
            }
            m_threadReceiveUDP = new System.Threading.Thread(new System.Threading.ThreadStart(receiveUdpThread));
            m_threadReceiveUDP.Start();
            m_nThreadCount++;
        }
        private void exitReceiveThread(string sIP, int nPort)
        {
            UdpClient udp;
            string sendMsg;

            udp = new UdpClient();
            sendMsg = "exit";
            byte[] sendBytes = System.Text.Encoding.UTF8.GetBytes(sendMsg);
            try
            {
                udp.Send(sendBytes, sendBytes.Length, sIP, nPort);
            }
            catch (Exception ex)
            {
                App.outLog(ex.ToString());
                udp.Close();
                return;
            }
            udp.Close();
        }
        private void receiveUdpThread()
        {
            IPEndPoint remoteEP = null;
            byte[] rcvBytes;
            UdpClient udp;
            string sMsg;

            m_bUDPIn = true;
            m_sInformation = "UDP受信" + m_nCount + "回目";
            this.Dispatcher.Invoke(updateControlInfo);
            DoEvents();
            udp = null;
            IPAddress selfAddress = IPAddress.Parse(m_sIpAddress);
            IPEndPoint selfEP = new IPEndPoint(selfAddress, m_nPort);
            try
            {
                //udp = new UdpClient(selfEP);
                udp = new UdpClient(m_nPort);
                rcvBytes = udp.Receive(ref remoteEP);
                sMsg = remoteEP.Address.ToString();
                sMsg = System.Text.Encoding.UTF8.GetString(rcvBytes);
                m_sMessage = sMsg + "<- IP = " + remoteEP.ToString();
                this.Dispatcher.Invoke(updateControlInfo);
                if (sMsg == "exit")
                {
                    udp.Close();
                    m_bUDPIn = false;
                    m_nThreadCount--;
                    return;
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
                m_nThreadCount--;
                return;
            }

            if (udp != null)
            {
                udp.Close();
            }
            m_nCount++;
            m_bUDPIn = false;
            m_nThreadCount--;
        }
        public int StrToInt(string str)
        {
            int num;

            if (int.TryParse(str, out num))
            { }
            else
            {
                num = 0;
            }
            return (num);
        }
        private void DoEvents()
        {
            DispatcherFrame frame = new DispatcherFrame();
            var callback = new DispatcherOperationCallback(ExitFrames);
            Dispatcher.CurrentDispatcher.BeginInvoke(DispatcherPriority.Background, callback, frame);
            Dispatcher.PushFrame(frame);
        }
        private object ExitFrames(object obj)
        {
            ((DispatcherFrame)obj).Continue = false;
            return null;
        }
    }
}
