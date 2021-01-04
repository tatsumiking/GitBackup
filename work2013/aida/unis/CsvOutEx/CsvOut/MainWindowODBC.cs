using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Net;
using System.Net.Mail;
using System.Windows.Threading;


namespace CsvOut
{
    public partial class MainWindow : Window
    {
        private string m_sUnisDBFile;
        private OdbcConnection m_conn = null;
        private OdbcCommand m_com;

        private int m_nLastDate;
        private int m_nLastTime;

        public void MainWindowODBCInit()
        {
            try
            {
                m_sUnisDBFile = m_sUnisDBPath+"\\unis.mdb";
                m_lstUserTimeLimit.Clear();
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("MainWindowODBCInit");
                App.LogOut(ex.ToString());
            }
        }
        public string GetODBCDBPath()
        {
            return (m_sUnisDBFile);
        }
        public Boolean ODBCOpenUnisDB()
        {
            Boolean bFlag;
            int idx, max;

            try
            {
                m_lstCsvRecord = new List<string>();
                m_lstErrorStr = new List<string>();
                bFlag = false;
                max = 10;
                m_conn = new OdbcConnection();
                m_conn.ConnectionString = "Driver={Microsoft Access Driver (*.mdb)};DBQ=" + m_sUnisDBFile + ";PWD=unisamho";
                for (idx = 0; idx < max; idx++)
                {
                    try
                    {
                        m_conn.Open();
                        bFlag = true;
                    }
                    catch (Exception ex1)
                    {
                        App.ErrorLogAppend(m_sUnisDBFile+"　Connect");
                        App.LogOut(ex1.ToString());
                        bFlag = false;
                    }
                    if (bFlag == true)
                    {
                        break;
                    }
                }
                if (bFlag == false)
                {
                    App.LogOut("データーベースに接続できません。");
                    return (false);
                }
                return (true);
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("ODBCOpenUnisDB");
                App.LogOut(ex.ToString());
                return (false);
            }
        }
        public void DoDispatch()
        {
            try
            {
                DispatcherFrame frame = new DispatcherFrame();
                Dispatcher.CurrentDispatcher.BeginInvoke(DispatcherPriority.Background, new DispatcherOperationCallback(ExitFrames), frame);
                Dispatcher.PushFrame(frame);
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("DoDispatch");
                App.LogOut(ex.ToString());
            }
        }
        public object ExitFrames(object frames)
        {
            try
            {
                ((DispatcherFrame)frames).Continue = false;
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("ExitFrames");
                App.LogOut(ex.ToString());
            }
            return null;
        }
        public void ODBCCloseUnisDB()
        {
            try
            {
                if(m_conn == null){
                    return;
                }
                m_conn.Close();
                m_conn = null;
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("ODBCCloseUnisDB");
                App.LogOut(ex.ToString());
            }
        }
        public void ODBCSelecttEnterAll()
        {
            string sSql;
            OdbcDataReader reader;
            string sDate, sTime;
            int nTid, nUid;
            string sTid, sUid;
            string sName, sMode;
            string sUnique, sResult;
            string sCsvRecord;
            string sFormatRecord;
            string sFormat;

            sSql = "";
            try
            {
                sSql = "SELECT ";
                sSql = sSql + "C_Date,C_Time,L_TID,L_UID";
                sSql = sSql + ",C_Name,L_Mode,C_Unique,L_Result";
                sSql = sSql + " FROM tEnter";
                m_com = new OdbcCommand(sSql, m_conn);
                reader = m_com.ExecuteReader();
                while (reader.Read())
                {
                    sDate = GetReaderString(reader, 0);
                    sTime = GetReaderString(reader, 1);
                    nTid = GetReaderInt32(reader, 2);
                    sFormat = "D" + m_nTidNOD;
                    sTid = nTid.ToString(sFormat);
                    nUid = GetReaderInt32(reader, 3);
                    sFormat = "D" + m_nUidNOD;
                    sUid = nUid.ToString(sFormat);
                    sName = GetReaderString(reader, 4);
                    sMode = GetReaderString(reader, 5);
                    sUnique = GetReaderString(reader, 6);
                    sResult = GetReaderString(reader, 7);
                    sCsvRecord = sDate + "," + sTime + "," + sTid + "," + sUid + "," + sName;
                    sCsvRecord = sCsvRecord + "," + sMode + "," + sUnique + "," + sResult;
                    sFormatRecord = "<" + FormatingRecord(sCsvRecord) + ">";
                }

            }
            catch (Exception ex)
            {
                App.ErrorLogAppend(sSql + " ODBCSelecttEnterAll");
                App.LogOut(ex.ToString());
            }

        }
        public void ODBCSelecttEnter()
        {
            string sSql;
            OdbcDataReader reader;
            string sCsvStr;
            string sCheckDate;
            string sCheckTime;
            string sDate, sTime;
            int nTid, nUid;
            string sTid, sUid;
            string sName, sMode;
            string sUnique, sResult;
            string sFormat;
            int max;

            sSql = "";
            try
            {
                resetListUserTimeLimit(m_sBaseDate, m_sBaseTime);
                sCheckDate = m_sCheckTime.Substring(0, 8);
                sCheckTime = m_sCheckTime.Substring(8, 6);
                sSql = "SELECT ";
                sSql = sSql + "C_Date,C_Time,L_TID,L_UID";
                sSql = sSql + ",C_Name,L_Mode,C_Unique,L_Result";
                sSql = sSql + " FROM tEnter";
                sSql = sSql + " WHERE (";
                sSql = sSql + " (StrComp(C_Date+C_Time,'" + m_sBaseDate + m_sBaseTime + "') >= 0)";
                sSql = sSql + " AND (StrComp(C_Date+C_Time,'" + sCheckDate + sCheckTime + "') < 0)";
                sSql = sSql + ");";
                m_com = new OdbcCommand(sSql, m_conn);
                reader = m_com.ExecuteReader();
                while (reader.Read())
                {
                    sDate = GetReaderString(reader, 0);
                    sTime = GetReaderString(reader, 1);
                    UpdateLastDateTime(reader);
                    if (checkRecordError(reader) == false)
                    {
                        nTid = GetReaderInt32(reader, 2);
                        sFormat = "D" + m_nTidNOD;
                        sTid = nTid.ToString(sFormat);
                        nUid = GetReaderInt32(reader, 3);
                        sFormat = "D" + m_nUidNOD;
                        sUid = nUid.ToString(sFormat);
                        sName = GetReaderString(reader, 4);
                        sMode = GetReaderString(reader, 5);
                        sUnique = GetReaderString(reader, 6);
                        sResult = GetReaderString(reader, 7);
                        sCsvStr = sDate + "," + sTime + "," + sTid + "," + sUid + "," + sName;
                        sCsvStr = sCsvStr + "," + sMode + "," + sUnique + "," + sResult;
                        m_lstCsvRecord.Add(sCsvStr);
                    }
                }
                max = m_lstCsvRecord.Count;
                if (max < 10000){
                    SortListCsvRecord();
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend(sSql + "　ODBCSelecttEnter");
                App.LogOut(ex.ToString());
            }
        }
        private Boolean checkRecordError(OdbcDataReader reader)
        {
            string sStr, sName, sUnique;
            int uid, mode, result;
            string sError;

            try
            {
                sStr = GetReaderString(reader, 3);
                uid = m_libCmn.StrToInt(sStr);
                sName = GetReaderString(reader, 4);
                sStr = GetReaderString(reader, 5);
                mode = m_libCmn.StrToInt(sStr);
                sUnique = GetReaderString(reader, 6);
                sStr = GetReaderString(reader, 7);
                result = m_libCmn.StrToInt(sStr);
                if (uid != -1 && result == 0 && (mode == 1 || mode == 2 || mode == 4 || mode == 5))
                {
                    return (false);
                }
                sStr = GetReaderString(reader, 0);
                sError = sStr.Substring(0, 4) + "/" + sStr.Substring(4, 2) + "/" + sStr.Substring(6, 2);
                sError = sError + m_sDelimiter;
                sStr = GetReaderString(reader, 1);
                sError = sError + sStr.Substring(0, 2) + ":" + sStr.Substring(2, 2) + ":" + sStr.Substring(4, 2);
                sError = sError + m_sDelimiter + "L_UID=" + uid + m_sDelimiter + "C_Name=" + sName;
                sError = sError + m_sDelimiter + "L_Mode=" + mode + "C_Unique" + sUnique + "L_Result=" + result;
                m_lstErrorStr.Add(sError);
                return (true);
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("checkRecordError");
                App.LogOut(ex.ToString());
                return (false);
            }
        }
        private void SortListCsvRecord()
        {
            int max, i, j;
            string sCsvTmp;
            string[] ary;
            string sDateI, sTimeI;
            string sDateJ, sTimeJ;
            string sDateTmp, sTimeTmp;

            try
            {
                max = m_lstCsvRecord.Count;
                for (i = 0; i < max - 1; i++)
                {
                    ary = m_lstCsvRecord[i].Split(',');
                    sDateI = ary[0];
                    sTimeI = ary[1];
                    for (j = i; j < max; j++)
                    {
                        ary = m_lstCsvRecord[j].Split(',');
                        sDateJ = ary[0];
                        sTimeJ = ary[1];
                        if (sDateI.CompareTo(sDateJ) > 0)
                        {
                            sCsvTmp = m_lstCsvRecord[i];
                            m_lstCsvRecord[i] = m_lstCsvRecord[j];
                            m_lstCsvRecord[j] = sCsvTmp;

                            sDateTmp = sDateI;
                            sDateI = sDateJ;
                            sDateJ = sDateTmp;

                            sTimeTmp = sTimeI;
                            sTimeI = sTimeJ;
                            sTimeJ = sTimeTmp;
                        }
                        else if (sDateI == sDateJ && sTimeI.CompareTo(sTimeJ) > 0)
                        {
                            sCsvTmp = m_lstCsvRecord[i];
                            m_lstCsvRecord[i] = m_lstCsvRecord[j];
                            m_lstCsvRecord[j] = sCsvTmp;

                            sDateTmp = sDateI;
                            sDateI = sDateJ;
                            sDateJ = sDateTmp;

                            sTimeTmp = sTimeI;
                            sTimeI = sTimeJ;
                            sTimeJ = sTimeTmp;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("SortListCsv");
                App.LogOut(ex.ToString());
            }
        }
        private void UpdateLastDateTime(OdbcDataReader reader)
        {
            string sDate;
            string sTime;
            int nDate;
            int nTime;

            try
            {
                sDate = GetReaderString(reader, 0);
                sTime = GetReaderString(reader, 1);
                nDate = m_libCmn.StrToInt(sDate);
                nTime = m_libCmn.StrToInt(sTime);
                if (m_nLastDate < nDate)
                {
                    m_nLastDate = nDate;
                    m_nLastTime = 0;
                    m_sLastDate = sDate;
                    m_sLastTime = "000000";
                }
                else if (m_nLastDate == nDate)
                {
                    if (m_nLastTime < nTime)
                    {
                        m_nLastTime = nTime;
                        m_sLastTime = sTime;
                    }
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("UpdateLastDateTime");
                App.LogOut(ex.ToString());
            }
        }
        public String FormatingRecord(string sRecord)
        {
            string[] aryFild;
            string strRet;
            int fldmax, fldidx;
            string key;
            int fldno;
            string[] aryForm;
            string sStr, sTmp, sFrm;
            int nNum;
            DateTime dt;

            aryFild = sRecord.Split(',');
            strRet = "";
            try
            {
                dt = GetRenderDateTime(aryFild[0], aryFild[1]);
                fldmax = m_aryFildKeyTbl.Length;
                for (fldidx = 0; fldidx < fldmax; fldidx++)
                {
                    key = m_aryFildKeyTbl[fldidx];
                    fldno = CnvOdbcKeyToIndex(key);
                    if (fldno == -1)
                    {
                        sStr = key;
                    }
                    else
                    {
                        sStr = aryFild[fldno];
                    }
                    switch (fldno)
                    {
                        case 0: // C_Date
                        case 1: // C_Time
                            if (m_aryFormatTbl[fldidx] != "")
                            {
                                sStr = dt.ToString(m_aryFormatTbl[fldidx]);
                            }
                            break;
                        case 2: // L_TID
                        case 3: // L_UID
                            aryForm = m_aryFormatTbl[fldidx].Split('|');
                            if (aryForm.Length == 2)
                            {
                                if (aryForm[0] == "s")
                                {
                                    // L_UID(3)　0フィル8桁
                                    sFrm = "{0," + aryForm[1] + "}";
                                    sTmp = String.Format(sFrm, sStr);
                                    sStr = sTmp;
                                }
                                else
                                {
                                    nNum = m_libCmn.StrToInt(sStr);
                                    sFrm = "D" + aryForm[1];
                                    sStr = nNum.ToString(sFrm);
                                }
                            }
                            else
                            {
                                if (m_aryFormatTbl[fldidx] != "")
                                {
                                    nNum = m_libCmn.StrToInt(sStr);
                                    sFrm = "D" + m_aryFormatTbl[fldidx];
                                    sStr = nNum.ToString(sFrm);
                                }
                            }
                            break;
                        case 4: // C_Name
                            aryForm = m_aryFormatTbl[fldidx].Split('|');
                            if (aryForm.Length == 2)
                            {
                                if (aryForm[0] == "i")
                                {
                                    nNum = m_libCmn.StrToInt(sStr);
                                    sFrm = "D" + aryForm[1];
                                    sStr = nNum.ToString(sFrm);
                                }
                                else
                                {
                                    sFrm = "{0," + aryForm[1] + "}";
                                    sTmp = String.Format(sFrm, sStr);
                                    sStr = sTmp;
                                }
                            }
                            else
                            {
                                if (m_aryFormatTbl[fldidx] != "")
                                {
                                    sFrm = "{0," + m_aryFormatTbl[fldidx] + "}";
                                    sTmp = String.Format(sFrm, sStr);
                                    sStr = sTmp;
                                }
                            }
                            break;
                        case 5: // L_Mode
                            sStr = GetFucStr(sStr);
                            break;
                        case 6: // C_Unique
                            aryForm = m_aryFormatTbl[fldidx].Split('|');
                            if (aryForm.Length == 2)
                            {
                                if (aryForm[0] == "i")
                                {
                                    nNum = m_libCmn.StrToInt(sStr);
                                    sFrm = "D" + aryForm[1];
                                    sStr = nNum.ToString(sFrm);
                                }
                                else
                                {
                                    sFrm = "{0," + aryForm[1] + "}";
                                    sTmp = String.Format(sFrm, sStr);
                                    sStr = sTmp;
                                }
                            }
                            else
                            {
                                if (m_aryFormatTbl[fldidx] != "")
                                {
                                    sFrm = "{0," + m_aryFormatTbl[fldidx] + "}";
                                    sTmp = String.Format(sFrm, sStr);
                                    sStr = sTmp;
                                }
                            }
                            break;
                        case 7: // L_Result
                            aryForm = m_aryFormatTbl[fldidx].Split('|');
                            if (aryForm.Length == 2)
                            {
                                if (aryForm[0] == "s")
                                {
                                    sFrm = "{0," + aryForm[1] + "}";
                                    sTmp = String.Format(sFrm, sStr);
                                    sStr = sTmp;
                                }
                                else
                                {
                                    nNum = m_libCmn.StrToInt(sStr);
                                    sFrm = "D" + aryForm[1];
                                    sStr = nNum.ToString(sFrm);
                                }
                            }
                            else
                            {
                                if (m_aryFormatTbl[fldidx] != "")
                                {
                                    nNum = m_libCmn.StrToInt(sStr);
                                    sFrm = "D" + m_aryFormatTbl[fldidx];
                                    sStr = nNum.ToString(sFrm);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    if (strRet == "")
                    {
                        strRet = sStr;
                    }
                    else
                    {
                        strRet = strRet + m_sDelimiter + sStr;
                    }
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("PicupCsvStrRecordElement");
                App.LogOut(ex.ToString());
            }
            return (strRet);
        }
        private DateTime GetRenderDateTime(String sDate, String sTime)
        {
            int yy, mm, dd, hh, mn, ss;
            DateTime dt;

            yy = m_libCmn.StrToInt(sDate.Substring(0, 4));
            mm = m_libCmn.StrToInt(sDate.Substring(4, 2));
            dd = m_libCmn.StrToInt(sDate.Substring(6, 2));
            hh = m_libCmn.StrToInt(sTime.Substring(0, 2));
            mn = m_libCmn.StrToInt(sTime.Substring(2, 2));
            ss = m_libCmn.StrToInt(sTime.Substring(4, 2));
            dt = new DateTime(yy, mm, dd, hh, mn, ss);
            return (dt);
        }
        private String GetFucStr(String sStr)
        {
            int nNum;
            nNum = m_libCmn.StrToInt(sStr);
            if (nNum == 0)
            { // 数字以外の時
                if (sStr == "出勤")
                {
                    sStr = m_aryFucStrTbl[0];
                }
                else if (sStr == "退勤")
                {
                    sStr = m_aryFucStrTbl[1];
                }
                else
                {
                    sStr = "00";
                }
            }
            else if (nNum == 1)
            {
                sStr = m_aryFucStrTbl[0];
            }
            else if (nNum == 2)
            {
                sStr = m_aryFucStrTbl[1];
            }
            else if (nNum == 3)
            {
                if (m_sDelimiter != "")
                {
                    sStr = "その他";
                }
                else
                {
                    sStr = "00";
                }
            }
            else if (nNum == 4)
            {
                sStr = m_aryFucStrTbl[2];
            }
            else if (nNum == 5)
            {
                sStr = m_aryFucStrTbl[3];
            }
            return (sStr);
        }
        private string GetReaderString(OdbcDataReader reader, int idx)
        {
            Type type;
            string sType;
            string str;

            str = "";
            try
            {
                type = reader.GetFieldType(idx);
                sType = type.Name;
                if (sType == "String")
                {
                    str = reader.GetString(idx);
                }
                else if (sType == "Int32")
                {
                    str = reader.GetInt32(idx).ToString();
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("GetReaderString");
                App.LogOut(ex.ToString());
            }
            return (str);
        }
        private int GetReaderInt32(OdbcDataReader reader, int idx)
        {
            Type type;
            string sType;
            int nNum;

            nNum = 0;
            try
            {
                type = reader.GetFieldType(idx);
                sType = type.Name;
                if (sType == "Int32")
                {
                    nNum = reader.GetInt32(idx);
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("GetReaderString");
                App.LogOut(ex.ToString());
            }
            return (nNum);
        }
        private int CnvOdbcKeyToIndex(string key)
        {
            try
            {
                if (key == "%C_Date%") // string
                {
                    return (0);
                }
                else if (key == "%C_Time%") // string
                {
                    return (1);
                }
                else if (key == "%L_TID%") // int
                {
                    return (2);
                }
                else if (key == "%L_UID%") // int
                {
                    return (3);
                }
                else if (key == "%C_Name%") // string
                {
                    return (4);
                }
                else if (key == "%L_Mode%") // int
                {
                    return (5);
                }
                else if (key == "%C_Unique%") // string
                {
                    return (6);
                }
                else if (key == "%L_Result%") // int
                {
                    return (7);
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("CnvOdbcKeyToIndex");
                App.LogOut(ex.ToString());
            }
            return (-1);
        }
    }
}
