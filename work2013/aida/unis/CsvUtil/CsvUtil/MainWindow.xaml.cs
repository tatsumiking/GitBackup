using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
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

namespace CsvUtil
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        private string m_sExecPath;
        private string m_sEnvPath;
        private string m_sEnvName;
        private LibCommon m_libCmn;

        private string[] m_aryTitleTbl;
        private string[] m_aryFildKeyTbl;
        private string[] m_aryFormatTbl;
        private string[] m_aryCountTbl;

        private int m_nTidNOD, m_nUidNOD;

        private string m_sSavePath;
        private string m_sUnisDBPath;
        private string m_sPostFileName;
        private string m_sDelimiter;
        private string m_sFncStrs;
        private int m_nCheckKind;
        private string m_sIntervalList;
        private string m_sCheckTimeList;
        private string m_sInterval;
        private string m_sBaseDate;
        private string m_sBaseTime;
        private int m_nOutputId;
        private int m_nOutputType;
        private string m_sPreFileName;
        private string m_sDateFileName;
        private int m_nUIDUniqueIdx;


        private string[] m_aryFucStrTbl;
        private string[] m_aryInterval;

        private List<string> m_lstCheckTime;
        private string m_sTempDate;
        private string m_sLastDate;
        private string m_sLastTime;
        private string m_sCheckTime;
        private DateTime m_dtCheckTime;
        private Boolean m_bCheckOutIn;
        private int m_nIncNo;

        private string m_sCsvFileName;
        private string m_sTargetFileName;
        private int m_nIdxDate;
        private int m_nIdxTime;
        private int m_nIdxTid;
        private int m_nIdxUid;
        private int m_nIdxName;
        private int m_nIdxKid; // 
        private int m_nIdxMode;
        private int m_nIdxResult;


        public MainWindow()
        {
            string[] aryLine;
            string sFileName;

            m_sCsvFileName = "";
            m_sTargetFileName = "";
            InitializeComponent();
            try
            {
                Assembly asm = System.Reflection.Assembly.GetExecutingAssembly();
                aryLine = asm.FullName.Split(',');
                if (aryLine[0] == "CsvExUtil")
                {
                    m_sEnvName = "csvex";
                    this.Title = "CsvExUtil";
                }
                else
                {
                    m_sEnvName = "csvout";
                    this.Title = "CsvOutUtil";
                }
                App.m_sEnvName = m_sEnvName;
                m_nIncNo = 1;
                m_bCheckOutIn = false;
                m_libCmn = new LibCommon();
                m_sExecPath = InitExePath();
                m_sEnvPath = InitEnvPath();
                sFileName = m_sEnvPath + "\\" + m_sEnvName + "log.txt";
                if (File.Exists(sFileName))
                {
                    // ログファイルがある時ログ情報を保存
                    App.m_sArgv = "log";
                }
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("MainWindow関数");
                App.LogOut(ex.ToString());
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                DefNODEnvFileLoad();
                FildKeyFileLoad();
                EnvFileLoad();
                OptFileLoad();
            }
            catch (Exception ex)
            {
                App.ErrorLogAppend("Window_Loaded");
                App.LogOut(ex.ToString());
            }
        }
        private void selectUnisCsvFileName()
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Unis保存CSVファイル(*.csv)|*.csv|すべてのファイル(*.*)|*.*||";
            ofd.Title = "Unis保存CSVファイルを選択してください";
            ofd.InitialDirectory = m_sSavePath;
            ofd.DefaultExt = "*.csv";
            if (ofd.ShowDialog() == false)
            {
                return;
            }
            txtCsvFile.Text = ofd.FileName;
        }
        private void selectTargetFileName()
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "ターゲットファイル(*.csv,*.txt)|*.csv;*.txt|すべてのファイル(*.*)|*.*||";
            ofd.Title = "ターゲットファイルを選択してください";
            ofd.InitialDirectory = m_sSavePath;
            ofd.DefaultExt = "*.csv,*.txt";
            if (ofd.ShowDialog() == false)
            {
                return;
            }
            txtTargetFile.Text = ofd.FileName;
        }
        private void margeRecodes()
        {
            String sMsg;
            int len;
            String sBackFileName;
            int idx, max;
            int tidx, tmax;
            String sLogCsv;
            String sSaveCsv;
            String[] aryLog;
            String[] arySave;
            String sSaveData;
            int ret;
            List<ClsRecode> lstLog;
            List<ClsRecode> lstSave;

            m_sCsvFileName = txtCsvFile.Text;
            if (m_sCsvFileName == "")
            {
                sMsg = "Unisせ保存したCsvファイルを指定してください\n";
                MessageBox.Show(sMsg);
                return;
            }
            m_sTargetFileName = txtTargetFile.Text;
            if (m_sTargetFileName == "")
            {
                sMsg = "CsvExで出力したファイルを指定してください\n";
                MessageBox.Show(sMsg);
                return;
            }
            len = m_sTargetFileName.Length;
            sBackFileName = m_sTargetFileName.Substring(0, len - 3) + "bak";
            m_libCmn.CopyFile(m_sTargetFileName, sBackFileName);
            string[] aryLogLine = m_libCmn.LoadFileLineSJIS(m_sCsvFileName);
            setFildIdx(aryLogLine[0]); // フィールドの位置特定
            max = aryLogLine.Length;
            for (idx = 1, tidx = 1; idx < max; idx++)
            {
                if (aryLogLine[idx] == "")
                {
                    continue;
                }
                sLogCsv = convertLogCsvToRecodeCsv(aryLogLine[idx]);
                aryLog = sLogCsv.Split(',');
                if(aryLog[7] == ""){
                    continue;
                }
                aryLogLine[tidx] = FormatingRecord(sLogCsv);
                tidx++;
            }
            Array.Resize(ref aryLogLine, tidx);
            max = tidx; // aryLogCsvLineの数を更新

            string[] arySaveLine = m_libCmn.LoadFileLineSJIS(m_sTargetFileName);

            tmax = arySaveLine.Length;
            sSaveData = "";
            for (idx = 1, tidx = 0; idx < max; idx++)
            {
                if (aryLogLine[idx] == "")
                {
                    continue;
                }
                sLogCsv = convertSaveLineToRecodeCsv(aryLogLine[idx]);
                aryLog = sLogCsv.Split(',');
                while (true)
                {
                    if (tidx >= tmax)
                    {
                        if (checkListUserTimeLimit(aryLog) == true)
                        {
                            sSaveData = sSaveData + aryLogLine[idx] + "\r\n";
                        }
                        break;
                    }
                    if (arySaveLine[tidx] == "")
                    {
                        tidx++;
                        continue;
                    }
                    sSaveCsv = convertSaveLineToRecodeCsv(arySaveLine[tidx]);
                    arySave = sSaveCsv.Split(',');
                    ret = compareRecodeStr(aryLog, arySave);
                    if (ret == 0)
                    {
                        lstLog = picupSameDateData(aryLogLine, ref idx);
                        lstSave = picupSameDateData(arySaveLine, ref tidx);
                        sSaveData = sSaveData + compareRecodeStr(ref lstLog, ref lstSave);
                        tidx++;
                        break;
                    }
                    else if(ret == -1)
                    {
                        if (checkListUserTimeLimit(arySave) == true)
                        {
                            sSaveData = sSaveData + arySaveLine[tidx] + "\r\n";
                        }
                        tidx++;
                    }
                    else if(ret == 1)
                    {
                        if (checkListUserTimeLimit(aryLog) == true)
                        {
                            sSaveData = sSaveData + aryLogLine[idx] + "\r\n";
                        }
                        break;
                    }
                }
            }
            while (true)
            {
                if (tidx >= tmax)
                {
                    break;
                }
                sSaveCsv = convertSaveLineToRecodeCsv(arySaveLine[tidx]);
                arySave = sSaveCsv.Split(',');
                if (checkListUserTimeLimit(arySave) == true)
                {
                    sSaveData = sSaveData + arySaveLine[tidx] + "\r\n";
                }
                tidx++;
            }
            m_libCmn.SaveFileSJIS(m_sTargetFileName, sSaveData);
            sMsg = "処理が終了しました\n";
            MessageBox.Show(sMsg);

        }
        private void exitProgram()
        {
            try
            {
                this.Close();
            }
            catch (Exception ex)
            {
                App.LogOut(ex.ToString());
            }

        }
        private void setFildIdx(String sFildLine)
        {
            int max, idx;

            String[] aryFild = sFildLine.Split(',');
            max = aryFild.Length;
            for (idx = 0; idx < max; idx++)
            {
                if (aryFild[idx] == "日付")
                {
                    m_nIdxDate = idx;
                }
                else if (aryFild[idx] == "発生時間")
                {
                    m_nIdxTime = idx;
                }
                else if (aryFild[idx] == "端末ID")
                {
                    m_nIdxTid = idx;
                }
                else if (aryFild[idx] == "ユーザID")
                {
                    m_nIdxUid = idx;
                }
                else if (aryFild[idx] == "名前")
                {
                    m_nIdxName = idx;
                }

                else if (aryFild[idx] == "社員番号")
                {
                    m_nIdxKid = idx;
                }
                else if (aryFild[idx] == "モード")
                {
                    m_nIdxMode = idx;
                }
                else if (aryFild[idx] == "結果")
                {
                    m_nIdxResult = idx;
                }
            }

        }
        private String convertLogCsvToRecodeCsv(String sCsvStr)
        {
            String sDate, sTime;
            int nTid, nUid;
            String sFormat;
            String sTid, sUid;
            String sName;
            String sMode;
            String sKno;
            String sResult;
            String sRecord;

            sRecord = "";
            String[] ary = sCsvStr.Split(',');
            // 日付 2020-04-14 C_Date
            String[] aryYMD = ary[m_nIdxDate].Split('-');
            sDate = aryYMD[0] + aryYMD[1] + aryYMD[2];
            // 発生時間 08:22:22 C_Time
            String[] aryHMS = ary[m_nIdxTime].Split(':');
            sTime = aryHMS[0] + aryHMS[1] + aryHMS[2];
            // 端末ID 0001 : AC7000 0001 L_TID
            String[] tary = ary[m_nIdxTid].Split(' ');
            nTid = m_libCmn.StrToInt(tary[0]);
            sFormat = "D" + m_nTidNOD;
            sTid = nTid.ToString(sFormat);
            // ユーザID **** or 10 L_UID
            nUid = m_libCmn.StrToInt(ary[m_nIdxUid]);
            sFormat = "D" + m_nUidNOD;
            sUid = nUid.ToString(sFormat);
            // 名前 xxxx C_Name
            sName = ary[m_nIdxName];
            // モード 入退室 F1 F2 F3 F4 F5 L_Mode
            sMode = analyzeMode(ary[m_nIdxMode]);
            // 社員番号 C_Unique
            sKno = ary[m_nIdxKid];
            // 結果 L_Result
            sResult = analyzeResult(ary[m_nIdxResult]);

            sRecord = sDate + "," + sTime + "," + sTid + "," + sUid + "," + sName;
            sRecord = sRecord + "," + sMode + "," + sKno + "," + sResult + ",";
            return (sRecord);
        }
        private String convertSaveLineToRecodeCsv(String sSaveLine)
        {
            int fldmax, fldidx;
            int sp, len;
            String sStr;
            String[] ary;
            String key;
            int fildno;
            String sFormat;
            DateTime dt;
            int nTid;
            int nUid;
            String sDate = "";
            String sTime = "";
            String sTid = "";
            String sUid = "";
            String sName = "";
            String sMode = "";
            String sKno = "";
            String sResult = "0";
            String sRecord;


            sRecord = "";
            if (m_sDelimiter == "") // 固定長
            {
                sStr = "";
                sp = 0;
                fldmax = m_aryFildKeyTbl.Length;
                for (fldidx = 0; fldidx < fldmax; fldidx++)
                {
                    len = m_libCmn.StrToInt(m_aryCountTbl[fldidx]);
                    sStr = sStr + sSaveLine.Substring(sp, len) + ",";
                    sp = sp + len;
                }
                ary = sStr.Split(',');
            }
            else
            {
                char[] dlms = m_sDelimiter.ToCharArray();
                ary = sSaveLine.Split(dlms);
            }
            fldmax = m_aryFildKeyTbl.Length;
            for (fldidx = 0; fldidx < fldmax; fldidx++)
            {
                key = m_aryFildKeyTbl[fldidx];
                fildno = getOdbcKeyToIndex(key);
                switch (fildno)
                {
                    case 0:
                        // 日付 C_Date
                        dt = convFormatStrToDate(m_aryFormatTbl[fldidx], ary[fldidx]);
                        sDate = dt.ToString("yyyyMMdd");
                        sTime = dt.ToString("HHmmss");
                        break;
                    case 1:
                        // 時間 C_Time
                        dt = convFormatStrToDate(m_aryFormatTbl[fldidx], ary[fldidx]);
                        sTime = dt.ToString("HHmmss");
                        break;
                    case 2:
                        // 端末ID L_TID
                        nTid = m_libCmn.StrToInt(ary[fldidx]);
                        sFormat = "D" + m_nTidNOD;
                        sTid = nTid.ToString(sFormat);
                        break;
                    case 3:
                        // ユーザID L_UID
                        nUid = m_libCmn.StrToInt(ary[fldidx]);
                        sFormat = "D" + m_nUidNOD;
                        sUid = nUid.ToString(sFormat);
                        break;
                    case 4:
                        // 名前 C_Name
                        sName = m_libCmn.deleteSpace(ary[fldidx]);
                        break;
                    case 5:
                        // モード L_Mode
                        sMode = m_libCmn.deleteSpace(ary[fldidx]);
                        break;
                    case 6:
                        // 社員番号 C_Unique
                        sKno = m_libCmn.deleteSpace(ary[fldidx]);
                        break;
                    case 7:
                        // 結果 L_Result
                        sResult = m_libCmn.deleteSpace(ary[fldidx]);
                        break;
                }

            }
            sRecord = sDate + "," + sTime + "," + sTid + "," + sUid + "," + sName;
            sRecord = sRecord + "," + sMode + "," + sKno + "," + sResult + ",";
            return (sRecord);
        }
        public int compareRecodeStr(String[] aryLog, String[] arySave)
        {
            int ret = String.Compare(arySave[0], aryLog[0]); // arySave > aryLog -> 1  arySave < aryLog -> -1
            if(ret == -1){
                return(-1);
            }else if(ret == 1){
                return(1);
            }
            ret = String.Compare(arySave[1], aryLog[1]);
            if(ret == -1){
                return(-1);
            }else if(ret == 1){
                return(1);
            }
            return(0);
        }
        public List<ClsRecode> picupSameDateData(String[] aryLine, ref int refidx)
        {
            String sCsv;
            String[] aryCsv;
            ClsRecode rec;
            int max, idx;
            String sDate, sTime;

            List<ClsRecode> list = new List<ClsRecode>();

            sCsv = convertSaveLineToRecodeCsv(aryLine[refidx]);
            aryCsv = sCsv.Split(',');
            rec = new ClsRecode(aryLine[refidx], aryCsv);
            list.Add(rec);
            sDate = aryCsv[0];
            sTime = aryCsv[1];

            max = aryLine.Length;
            for(idx = refidx+1; idx < max; idx++){
                sCsv = convertSaveLineToRecodeCsv(aryLine[idx]);
                aryCsv = sCsv.Split(',');
                if(sDate == aryCsv[0] && sTime == aryCsv[1])
                {
                    rec = new ClsRecode(aryLine[idx], aryCsv);
                    list.Add(rec);
                    refidx = idx;
                }else{
                    break;
                }
            }
            return (list);
        }
        public String compareRecodeStr(ref List<ClsRecode> lstLg, ref List<ClsRecode> lstSv)
        {
            int idxLg, maxLg;
            int idxSv, maxSv;
            int idxId;
            String sSaveStr;

            if(m_nOutputId == 0){
                idxId = 3;
            }else{
                idxId = 6;
            }
            maxLg = lstLg.Count;
            maxSv = lstSv.Count;
            for (idxLg = 0; idxLg < maxLg; idxLg++)
            {
                for (idxSv = 0; idxSv < maxSv; idxSv++)
                {
                    if (lstLg[idxLg].ary[idxId] == lstSv[idxSv].ary[idxId])
                    {
                        lstLg[idxLg].ary[7] = "1";
                        break;
                    }
                }
            }
            sSaveStr = "";
            for (idxSv = 0; idxSv < maxSv; idxSv++)
            {
                if (checkListUserTimeLimit(lstSv[idxSv].ary) == true)
                {
                    sSaveStr = sSaveStr + lstSv[idxSv].sStr + "\r\n";
                }
            }
            for (idxLg = 0; idxLg < maxLg; idxLg++)
            {
                if (lstLg[idxLg].ary[7] == "0")
                {
                    if (checkListUserTimeLimit(lstLg[idxLg].ary) == true)
                    {
                        sSaveStr = sSaveStr + lstLg[idxLg].sStr + "\r\n";
                    }
                }
            }
            return (sSaveStr);
        }
        public DateTime convFormatStrToDate(String sFormat, String sDateTime)
        {
            DateTime dTime = DateTime.ParseExact(sDateTime, sFormat, null);
            return (dTime);
        }
        public String analyzeMode(String sMode)
        {
            String sRetMode;

            if (sMode == "F1")
            {
                sRetMode = "1";
            }
            else if (sMode == "F2")
            {
                sRetMode = "2";
            }
            else if (sMode == "F4")
            {
                sRetMode = "4";
            }
            else if (sMode == "F5")
            {
                sRetMode = "5";
            }
            else
            {
                sRetMode = "3";
            }
            return(sRetMode);
        }
        public String analyzeResult(String sResult)
        {
            String sRetResult;

            if (sResult == "成功")
            {
                sRetResult = "0";
            }
            else if (sResult == "照合エラー")
            {
                sRetResult = "1";
            }
            else
            {
                sRetResult = "1";
            }
            return (sRetResult);
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
                    fldno = getOdbcKeyToIndex(key);
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
        private int getOdbcKeyToIndex(string key)
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

    }
}
