using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace Observe
{
    public partial class MainWindow : Window
    {
        public void udpSendThreadExit()
        {
            UdpClient udp;

            if (m_sSelfIP == "")
            {
                return;
            }

            udp = new UdpClient();
            byte[] sendBytes = System.Text.Encoding.UTF8.GetBytes("exit");
            try
            {
                udp.Send(sendBytes, sendBytes.Length, m_sSelfIP, m_nPort);
            }
            catch (Exception ex)
            {
                udp.Close();
                return;
            }
            udp.Close();

        }
        public void udpReceiveGPSData()
        {
            int nIdx;
            int max, i, j;
            string sGpsPos;
            ClsGpsPos clsGpsPos;
            UdpClient udp;
            byte[] rcvBytes;
            string sSoutiNo;
            string sDate;

            if (m_sSelfIP == "")
            {
                return;
            }
            //UdpClientを作成し、ローカルエンドポイントにバインドする
            udp = new UdpClient(m_nPort);
            m_rcvUDPClient = udp;
            IPEndPoint remoteEP = null;

            try
            {
                rcvBytes = udp.Receive(ref remoteEP);
            }
            catch (Exception ex)
            {
                udp.Close();
                return;
            }
            if (rcvBytes[0] == 'e'
            && rcvBytes[1] == 'x'
            && rcvBytes[2] == 'i'
            && rcvBytes[3] == 't')
            {
                m_bUDPFlag = false;
                return;
            }
            m_bUDPFlag = false;
            max = rcvBytes.Length - 5;
            byte[] strBytes = new byte[max];
            byte[] skey = System.Text.Encoding.UTF8.GetBytes("{{[[");
            byte[] ekey = System.Text.Encoding.UTF8.GetBytes("]]}}");
            for (i = 0; i < max; i++)
            {
                if ((skey[0] == rcvBytes[i])
                 && (skey[1] == rcvBytes[i+1])
                 && (skey[2] == rcvBytes[i+2])
                 && (skey[3] == rcvBytes[i+3]))
                {
                    break;
                }
            }
            if (i == max)
            {
                udp.Close();
                return;
            }
            for (i = i + 4, j = 0; i < max; i++, j++)
            {
                if ((ekey[0] == rcvBytes[i])
                 && (ekey[1] == rcvBytes[i + 1])
                 && (ekey[2] == rcvBytes[i + 2])
                 && (ekey[3] == rcvBytes[i + 3]))
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
            //受信した送信者の情報
            // remoteEP.Address, remoteEP.Port;
            // if (m_clsCardCrt.checkIP(remoteEP.Address) == true)
            //if (checkRemoteIP(remoteEP.Address) == true)
            //{
                clsGpsPos = new ClsGpsPos();
                sSoutiNo = m_libCmn.DeleteDoubleQuotation(ary[0]);
                nIdx = searchCardIdx(sSoutiNo);
                if (nIdx != -1)
                {
                    sDate = m_libCmn.DeleteDoubleQuotation(ary[1]);
                    clsGpsPos.m_sDate = sDate;
                    clsGpsPos.m_dLat = m_libCmn.StrToDouble(ary[2]);
                    clsGpsPos.m_dLnd = m_libCmn.StrToDouble(ary[3]);
                    m_clsObserve.m_lstClsCard[nIdx].m_lstGpsPos.Add(clsGpsPos);
                }
            //}
            //UdpClientを閉じる
            udp.Close();
        }
        public void udpSendCardDeleteMessage(ClsCard clsCard)
        {
            string sData;

            sData = "DEL\r\n";
            sData = sData + clsCard.m_sSetNo + "\r\n";
            sData = sData + clsCard.m_sIP + "\r\n";
            sData = sData + clsCard.m_sSyoNo + "\r\n";
            sData = sData + clsCard.m_sSyoName + "\r\n";
            sData = sData + clsCard.m_sAddress1 + "\r\n";
            sData = sData + clsCard.m_sAddress2 + "\r\n";
            sData = sData + clsCard.m_sTel1 + "\r\n";
            sData = sData + clsCard.m_sTel2 + "\r\n";
            sData = sData + clsCard.m_sName + "\r\n";
            sData = sData + clsCard.m_sBikou + "\r\n";

            udpSendDataForCenter(sData);
            udpSendDataForCenter(sData);
            udpSendDataForCenter(sData);
            udpSendDataForCenter(sData);
            udpSendDataForCenter(sData);
        }
        public void udpSendCardSetMessage(ClsCard clsCard)
        {
            string sData;

            sData = "SET\r\n";
            sData = sData + clsCard.m_sSetNo + "\r\n";
            sData = sData + clsCard.m_sIP + "\r\n";
            sData = sData + clsCard.m_sSyoNo + "\r\n";
            sData = sData + clsCard.m_sSyoName + "\r\n";
            sData = sData + clsCard.m_sAddress1 + "\r\n";
            sData = sData + clsCard.m_sAddress2 + "\r\n";
            sData = sData + clsCard.m_sTel1 + "\r\n";
            sData = sData + clsCard.m_sTel2 + "\r\n";
            sData = sData + clsCard.m_sName + "\r\n";
            sData = sData + clsCard.m_sBikou + "\r\n";

            udpSendDataForCenter(sData);
            udpSendDataForCenter(sData);
            udpSendDataForCenter(sData);
            udpSendDataForCenter(sData);
            udpSendDataForCenter(sData);
        }
        public void udpSendDataForCenter(string sData)
        {
            UdpClient udp;

            DoEvents();
            udp = new UdpClient();
            byte[] sendBytes = System.Text.Encoding.UTF8.GetBytes(sData);
            try
            {
                udp.Send(sendBytes, sendBytes.Length, m_sCenterIP, m_nPort);
            }
            catch (Exception ex)
            {
                udp.Close();
                return;
            }
            udp.Close();
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
