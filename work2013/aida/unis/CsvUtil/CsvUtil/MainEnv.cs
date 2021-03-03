using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace CsvUtil
{
    public partial class MainWindow : Window
    {
        private void FildKeyFileLoad()
        {
            string sTitles;
            string sFilfkeys;
            string sFormats;
            string sCounts;
            string sFildKeyFileName;
            string sData;
            string[] aryLine;
            int idx, max;

            try
            {
                if (m_sDelimiter == "")
                {
                    // 固定長
                    sTitles = "データ区分,打刻日,打刻時間,シフトコード,出退勤フラグ,ユーザID,例外コード,ターミナルNO";
                    sFilfkeys = "  ,%C_Date%,%C_Time%,  ,%L_Mode%,%L_UID%,  ,%L_TID%";
                    sFormats = "2,yyyMMdd,HHmm,2,,s|-10,2,2";
                    sCounts = "2,8,4,2,2,10,2,2";
                    m_nUIDUniqueIdx = 5;
                }
                else
                {
                    sTitles = "ユーザID,西暦/月/日,時:分,出退勤フラグ,固定値";
                    sFilfkeys = "%C_Unique%,%C_Date%,%C_Time%,%L_Mode%,0";
                    sFormats = ",yyyy/MM/dd,HH:mm,,";
                    sCounts = "2,8,4,2,2,10,2,2";
                    m_nUIDUniqueIdx = 0;
                }
                sFildKeyFileName = m_sExecPath + "\\fieldkey.env";
                sData = m_libCmn.LoadFileSJIS(sFildKeyFileName);
                if (sData != "")
                {
                    sData = sData.Replace("\r\n", "\n");
                    aryLine = sData.Split('\n');
                    sTitles = aryLine[1];
                    sFilfkeys = aryLine[2];
                    sFormats = aryLine[3];
                    sCounts = aryLine[4];
                }
                m_aryTitleTbl = sTitles.Split(',');
                m_aryFildKeyTbl = sFilfkeys.Split(',');
                m_aryFormatTbl = sFormats.Split(',');
                m_aryCountTbl = sCounts.Split(',');
                max = m_aryFildKeyTbl.Length;
                for (idx = 0; idx < max; idx++)
                {
                    if (m_aryFildKeyTbl[idx] == "%L_UID%" || m_aryFildKeyTbl[idx] == "%C_Unique%")
                    {
                        m_nUIDUniqueIdx = idx;
                        break;
                    }
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("FildKeyFileLoad");
                App.LogOut(ex.ToString());
            }
        }
        private void DefNODEnvFileLoad()
        {
            String sEnvFileName;
            string sData;
            string[] aryLine;

            m_nTidNOD = 4;
            m_nUidNOD = 8;
            try
            {
                sEnvFileName = m_sExecPath + "\\NumberOfDigit.env";
                sData = m_libCmn.LoadFileSJIS(sEnvFileName);
                if (sData != "")
                {
                    sData = sData.Replace("\r\n", "\n");
                    aryLine = sData.Split('\n');
                    m_nTidNOD = m_libCmn.StrToInt(aryLine[1]);
                    m_nUidNOD = m_libCmn.StrToInt(aryLine[2]);
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("DefNODEnvFileLoad");
                App.LogOut(ex.ToString());
            }
        }
        private void EnvFileLoad()
        {
            string sLoadFileName;
            string sData;
            string[] aryLine;

            try
            {
                sLoadFileName = m_sEnvPath + "\\" + m_sEnvName + ".env";
                sData = m_libCmn.LoadFileSJIS(sLoadFileName);
                if (sData != "")
                {
                    sData = sData.Replace("\r\n", "\n");
                    aryLine = sData.Split('\n');
                    if (12 <= aryLine.Length)
                    {
                        m_sSavePath = aryLine[1];
                        m_sUnisDBPath = aryLine[2];
                        m_sPostFileName = aryLine[3];
                        m_sDelimiter = aryLine[4];
                        m_sFncStrs = aryLine[5];
                        m_nCheckKind = m_libCmn.StrToInt(aryLine[6]);
                        m_sIntervalList = aryLine[7];
                        m_sCheckTimeList = aryLine[8];
                        m_sInterval = aryLine[9];
                        m_sBaseDate = aryLine[10];
                        m_sBaseTime = aryLine[11];
                        if(14 <= aryLine.Length){
                            m_nOutputId = m_libCmn.StrToInt(aryLine[12]);
                            m_nOutputType = m_libCmn.StrToInt(aryLine[13]);
                            if (16 <= aryLine.Length)
                            {// 新しいフォーマット
                                m_sPreFileName = aryLine[14];
                                m_sDateFileName = aryLine[15];
                            }
                         }
                    }
                }
                m_aryFucStrTbl = m_sFncStrs.Split(',');
                m_aryInterval = m_sIntervalList.Split(',');
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("EnvFileLoad");
                App.LogOut(ex.ToString());
            }
        }
        private void OptFileLoad()
        {
            String sOptionFileName;
            string sData;
            string[] aryLine;

            m_nCheckTimeLimit = 0; // 旭川医大使用時 1
            try
            {
                sOptionFileName = m_sExecPath + "\\option.env";
                sData = m_libCmn.LoadFileSJIS(sOptionFileName);
                if (sData != "")
                {
                    sData = sData.Replace("\r\n", "\n");
                    aryLine = sData.Split('\n');
                    m_nCheckTimeLimit = m_libCmn.StrToInt(aryLine[1]);
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("OptFileLoad");
                App.LogOut(ex.ToString());
            }

        }
    }
}
