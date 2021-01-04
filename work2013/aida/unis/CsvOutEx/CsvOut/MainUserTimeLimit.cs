using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace CsvOut
{
    class ClsUserTimeLimit
    {
        public int m_nUserNo;
        public DateTime m_dtLimit;

        public ClsUserTimeLimit()
        {
            m_nUserNo = 0;
            m_dtLimit = DateTime.Now;
        }
        public void setElement(int sUserNo, DateTime dtCrt)
        {
            m_nUserNo = sUserNo;
            m_dtLimit = dtCrt.AddMinutes(1);

        }
        public void changeLimit(DateTime dtCrt)
        {
            m_dtLimit = dtCrt.AddMinutes(1);
        }
    }

    public partial class MainWindow : Window
    {
        // 1分以内の入退を無効にするための入退記録リスト
        private List<ClsUserTimeLimit> m_lstUserTimeLimit = new List<ClsUserTimeLimit>();
        private int m_nCheckTimeLimit = 0; // 旭川医大使用時 1

        private void resetListUserTimeLimit(String sDate, String sTime)
        {
            String sYY, sMM, sDD, sHH, sMin, sSS;
            int nYY, nMM, nDD, nHH, nMin, nSS;
            int max, idx;

            if (m_nCheckTimeLimit == 0)
            {
                return;
            }
            if (sDate.Length == 8)
            {
                sYY = sDate.Substring(0, 4);
                sMM = sDate.Substring(4, 2);
                sDD = sDate.Substring(6, 2);
            }
            else
            {
                sYY = "1"; sMM = "1"; sDD = "1";
            }
            if (sTime.Length == 6)
            {
                sHH = sTime.Substring(0, 2);
                sMin = sTime.Substring(2, 2);
                sSS = sTime.Substring(4, 2);
            }
            else
            {
                sHH = "0"; sMin = "0"; sSS = "0";
            }
            nYY = m_libCmn.StrToInt(sYY);
            nMM = m_libCmn.StrToInt(sMM);
            nDD = m_libCmn.StrToInt(sDD);
            nHH = m_libCmn.StrToInt(sHH);
            nMin = m_libCmn.StrToInt(sMin);
            nSS = m_libCmn.StrToInt(sSS);

            DateTime dtCrt = new DateTime(nYY, nMM, nDD, nHH, nMin, nSS);
            max = m_lstUserTimeLimit.Count;
            for (idx = max - 1; idx >= 0; idx--)
            {
                if (m_lstUserTimeLimit[idx].m_dtLimit < dtCrt)
                {
                    m_lstUserTimeLimit.RemoveAt(idx);
                }
            }
        }
        private Boolean checkListUserTimeLimit(String sRecodeCsv)
        {
            String sDate, sTime, sUid;
            String sYY, sMM, sDD, sHH, sMin, sSS;
            int nYY, nMM, nDD, nHH, nMin, nSS;
            int nUid;
            int max, idx;
            ClsUserTimeLimit clsUTL;

            if (m_nCheckTimeLimit == 0)
            {
                return(true);
            }
            string[] ary = sRecodeCsv.Split(',');
            sDate = ary[0];
            sTime = ary[1];
            sUid = ary[3];

            if (sDate.Length == 8)
            {
                sYY = sDate.Substring(0, 4);
                sMM = sDate.Substring(4, 2);
                sDD = sDate.Substring(6, 2);
            }
            else
            {
                sYY = "1"; sMM = "1"; sDD = "1";
            }
            if (sTime.Length == 6)
            {
                sHH = sTime.Substring(0, 2);
                sMin = sTime.Substring(2, 2);
                sSS = sTime.Substring(4, 2);
            }
            else
            {
                sHH = "0"; sMin = "0"; sSS = "0";
            }

            nYY = m_libCmn.StrToInt(sYY);
            nMM = m_libCmn.StrToInt(sMM);
            nDD = m_libCmn.StrToInt(sDD);
            nHH = m_libCmn.StrToInt(sHH);
            nMin = m_libCmn.StrToInt(sMin);
            nSS = m_libCmn.StrToInt(sSS);

            nUid = m_libCmn.StrToInt(sUid);
            DateTime dtCrt = new DateTime(nYY, nMM, nDD, nHH, nMin, nSS);
            max = m_lstUserTimeLimit.Count;
            for (idx = max - 1; idx >= 0; idx--)
            {
                if (m_lstUserTimeLimit[idx].m_nUserNo == nUid)
                {
                    if (m_lstUserTimeLimit[idx].m_dtLimit > dtCrt)
                    {
                        return (false);
                    }
                    else
                    {
                        m_lstUserTimeLimit[idx].changeLimit(dtCrt);
                        return (true);
                    }
                }
            }
            clsUTL = new ClsUserTimeLimit();
            clsUTL.setElement(nUid, dtCrt);
            m_lstUserTimeLimit.Add(clsUTL);
            return (true);
        }
    }
}
