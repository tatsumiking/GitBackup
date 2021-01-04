using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace CsvOut
{
    public partial class MainWindow : Window
    {
        private void InitProgDef()
        {
            string[] aryLine;
            string sFileNameDef;
            string sFileNameMajerFlow;
            string sFileNameVicsell;
            string sData;

            try
            {
                sFileNameDef = m_sExecPath + "\\" + m_sProgName + "def.txt";
                sFileNameMajerFlow = m_sExecPath + "\\MajerFlow";
                sFileNameVicsell = m_sExecPath + "\\Vicsell";
                if (File.Exists(sFileNameDef))
                {
                    // デフォルト出力ファイル関連保存ファイル
                    sData = m_libCmn.LoadFileSJIS(sFileNameDef);
                    aryLine = sData.Split('\n');
                    m_sDefSavePath = aryLine[1];
                    m_sDefPreFileName = aryLine[2];
                    m_sDefDateFileName = aryLine[3];
                    m_sDefPostFileName = aryLine[4];
                    m_sDefTypeName = aryLine[5];
                }
                else if (File.Exists(sFileNameMajerFlow))
                {
                    // MajerFlow(csvout)時のデフォルト出力ファイル関連値
                    m_sDefSavePath = m_sEnvPath;
                    m_sDefPreFileName = "";
                    m_sDefDateFileName = "yyyyMMddHHmm";
                    m_sDefPostFileName = "major";
                    m_sDefTypeName = "MajerFlow";
                }
                else if (File.Exists(sFileNameVicsell))
                {
                    // Vicsell(csvex)時のデフォルト出力ファイル関連値
                    m_sDefSavePath = m_sEnvPath;
                    m_sDefPreFileName = "KINTAIDAKOKU_";
                    m_sDefDateFileName = "yyyyMMdd";
                    m_sDefPostFileName = "00";
                    m_sDefTypeName = "Vicsell";
                }
                else
                {
                    // プログラム名からデフォルト値を設定
                    if (m_sProgName == "csvout")
                    {
                        // csvout デフォルト出力ファイル関連値 "MajerFlow"
                        m_sDefSavePath = m_sEnvPath;
                        m_sDefPreFileName = "";
                        m_sDefDateFileName = "yyyyMMddHHmm";
                        m_sDefPostFileName = "major";
                        m_sDefTypeName = "MajerFlow";
                    }
                    else
                    {
                        // csvex デフォルト出力ファイル関連値 "Vicsell"
                        m_sDefSavePath = m_sEnvPath;
                        m_sDefPreFileName = "KINTAIDAKOKU_";
                        m_sDefDateFileName = "yyyyMMdd";
                        m_sDefPostFileName = "00";
                        m_sDefTypeName = "Vicsell";
                    }
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("InitProgDef");
                App.LogOut(ex.ToString());
            }
        }

        private void FildKeyFileLoad()
        {
            string sTitles;
            string sFilfkeys;
            string sFormats;
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
                    m_nUIDUniqueIdx = 5;
                }
                else
                {
                    sTitles = "ユーザID,西暦/月/日,時:分,出退勤フラグ,固定値";
                    sFilfkeys = "%C_Unique%,%C_Date%,%C_Time%,%L_Mode%,0";
                    sFormats = ",yyyy/MM/dd,HH:mm,,";
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
                }
                m_aryTitleTbl = sTitles.Split(',');
                m_aryFildKeyTbl = sFilfkeys.Split(',');
                m_aryFormatTbl = sFormats.Split(',');
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
        private void DefaultEnvFileLoad()
        {
            String sFncStrFileName;
            string sData;
            string[] aryLine;

            m_sSavePath = m_sDefSavePath;
            m_sUnisDBPath = System.Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);
            m_sUnisDBPath = m_sUnisDBPath + "\\UNIS";
            m_sPostFileName = m_sDefPostFileName;
            if (m_sDefTypeName == "MajerFlow")
            {
                m_sDelimiter = ",";
                m_sFncStrs = "出勤,退勤,外出,戻り";
            }
            else if (m_sDefTypeName == "Vicsell")
            {
                m_sDelimiter = "";
                m_sFncStrs = "01,02,00,00";
            }
            m_nCheckKind = 0;
            m_sIntervalList = "05分,01時間,03時間,06時間,12時間";
            m_sCheckTimeList = "0800,0900,1300,1700,1800,2300";
            m_sInterval = "0005";
            m_sBaseDate = "0";
            m_sBaseTime = "0";

            m_nOutputId = 1;
            m_nOutputType = 0;

            m_sPreFileName = m_sDefPreFileName;
            m_sDateFileName = m_sDefDateFileName;

            try
            {
                sFncStrFileName = m_sExecPath + "\\default.env";
                sData = m_libCmn.LoadFileSJIS(sFncStrFileName);
                if (sData != "")
                {
                    sData = sData.Replace("\r\n", "\n");
                    aryLine = sData.Split('\n');
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
                    m_nOutputId = m_libCmn.StrToInt(aryLine[12]);
                    m_nOutputType = m_libCmn.StrToInt(aryLine[13]);
                    m_sPreFileName = aryLine[14];
                    m_sDateFileName = aryLine[15];
                }

            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("DefaultEnvFileLoad");
                App.LogOut(ex.ToString());
            }
            m_aryFucStrTbl = m_sFncStrs.Split(',');
            m_aryInterval = m_sIntervalList.Split(',');
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
        private void EnvFileLoad()
        {
            string sLoadFileName;
            string sData;
            string[] aryLine;

            try
            {
                sLoadFileName = m_sEnvPath + "\\" + m_sProgName + ".env";
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
        private void EnvFileSave()
        {
            string sSaveFileName;
            string sData;
            int max, idx;

            try
            {
                max = m_aryInterval.Length;
                m_sIntervalList = m_aryInterval[0];
                for (idx = 1; idx < max; idx++)
                {
                    m_sIntervalList = m_sIntervalList + "," + m_aryInterval[idx];
                }
                max = m_aryFucStrTbl.Length;
                m_sFncStrs = m_aryFucStrTbl[0];
                for (idx = 1; idx < max; idx++)
                {
                    m_sFncStrs = m_sFncStrs + "," + m_aryFucStrTbl[idx];
                }
                sSaveFileName = m_sEnvPath + "\\" + m_sProgName + ".env";
                sData = "//,1000,"+m_sProgName+" env\r\n";
                sData = sData + m_sSavePath + "\r\n";
                sData = sData + m_sUnisDBPath + "\r\n";
                sData = sData + m_sPostFileName + "\r\n";
                sData = sData + m_sDelimiter + "\r\n";
                sData = sData + m_sFncStrs + "\r\n";
                sData = sData + m_nCheckKind + "\r\n"; ;
                sData = sData + m_sIntervalList + "\r\n"; ;
                sData = sData + m_sCheckTimeList + "\r\n"; ;
                sData = sData + m_sInterval + "\r\n"; ;
                sData = sData + m_sBaseDate + "\r\n";
                sData = sData + m_sBaseTime + "\r\n";
                sData = sData + m_nOutputId + "\r\n";
                sData = sData + m_nOutputType + "\r\n";
                sData = sData + m_sPreFileName + "\r\n";
                sData = sData + m_sDateFileName + "\r\n";

                m_libCmn.SaveFileSJIS(sSaveFileName, sData);
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("EnvFileSave");
                App.LogOut(ex.ToString());
            }
        }
    }
}
