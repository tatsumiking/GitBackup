using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace CsvUtil
{
    class ClsUserTimeLimit
    {
        public String m_sUserKey;
        public DateTime m_dtLimit;

        public ClsUserTimeLimit()
        {
            m_sUserKey = "";
            m_dtLimit = DateTime.Now;
        }
        public void setElement(String sUserKey, DateTime dtCrt)
        {
            m_sUserKey = sUserKey;
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
        private int m_nCheckTimeLimit = 1; // 旭川医大使用時 1

        private Boolean checkListUserTimeLimit(String[] aryCsv)
        {
            int idxId;
            String sUserKey;
            String sDate, sTime, sUid;
            String sYY, sMM, sDD, sHH, sMin, sSS;
            int nYY, nMM, nDD, nHH, nMin, nSS;
            int max, idx;
            ClsUserTimeLimit clsUTL;

            if (m_nCheckTimeLimit == 0)
            {
                return(true);
            }
            sDate = aryCsv[0];
            sTime = aryCsv[1];
            if (m_nOutputId == 0)
            {
                idxId = 3;
            }
            else
            {
                idxId = 6;
            }
            sUserKey = aryCsv[idxId];

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
                if (m_lstUserTimeLimit[idx].m_sUserKey == sUserKey)
                {
                    if (m_lstUserTimeLimit[idx].m_dtLimit >= dtCrt)
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
            clsUTL.setElement(sUserKey, dtCrt);
            m_lstUserTimeLimit.Add(clsUTL);
            return (true);
        }
    }
}
