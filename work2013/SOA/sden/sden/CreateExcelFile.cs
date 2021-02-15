using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Excel = Microsoft.Office.Interop.Excel;

namespace sden
{
    class CreateExcelFile
    {
        private MainWindow m_wnd;
        private LibCommon m_libCmn;
        private LibExcel m_libExcel;
        private ProgressWindows m_pgWnd;
        private String m_sCrctFile;
        private String m_sSrcXlsFile;
        private String m_sDstXlsFile;
        private string[] m_aryLine;
        private int m_nLineCount;

        public string SetWndClass(MainWindow wnd)
        {
            String strDate;
            DateTime dt = DateTime.Now;
            strDate = dt.ToString("yyyyMMddHHmmss");

            m_wnd = wnd;
            m_libCmn = m_wnd.m_libCmn;
            m_libExcel = m_wnd.m_libExcel;
            m_sCrctFile = m_wnd.m_sExePath + "\\outcrect.dat";
            m_sSrcXlsFile = m_wnd.m_sExePath + "\\template.xls";
            m_sDstXlsFile = m_wnd.m_sExcelSavePath + "\\" + strDate + ".xls";
            //m_sDstXlsFile = m_wnd.m_sExcelSavePath + "\\temp.xls";
            return (m_sDstXlsFile);
        }
        public Boolean Create()
        {
            String sMsgs;
            string[] aryMsg;

            if (System.IO.File.Exists(m_sCrctFile) == false)
            {
                System.Windows.MessageBox.Show("ファイルが見つかりません", "エラー");
                return (false);
            }
            if (System.IO.File.Exists(m_sDstXlsFile) == true)
            {
                System.IO.File.Delete(m_sDstXlsFile);
            }
            File.Copy(m_sSrcXlsFile, m_sDstXlsFile);
            try
            {
                if (m_libExcel.InitExcelApp() == false)
                {
                    return (false);
                }
            }
            catch (Exception ex)
            {
                sMsgs = ex.ToString();
                aryMsg = sMsgs.Split('\n');
                System.Windows.MessageBox.Show(aryMsg[0], "エラー");
                return (false);
            }

            try
            {
                m_aryLine = m_libCmn.LoadFileLineSJIS(m_sCrctFile);
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "エラー");
                m_libExcel.CloseExcelApp();
                return (false);
            }
            m_nLineCount = m_aryLine.Length;
            m_pgWnd = new ProgressWindows();
            m_pgWnd.SetTitle("");
            m_pgWnd.SetReng(0, m_nLineCount);
            m_pgWnd.Show();

            try
            {
                m_libExcel.InitExcelBook(m_sSrcXlsFile);
                m_libExcel.InitDstExcelBook(m_sDstXlsFile);

            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "エラー");
                m_libExcel.CloseExcelApp();
                return (false);
            }

            try
            {
                SheetCreate();
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "エラー");
                m_libExcel.CloseExcelApp();
                return (false);
            }

            try
            {
                m_libExcel.EndDstExcelSave();
                m_pgWnd.Close();
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "エラー");
                m_libExcel.CloseExcelApp();
                return (false);
            }
            m_libExcel.CloseExcelApp();

            return (true);
        }
        private void SheetCreate()
        {
            int line, clum;
            String sCellVal;
            int nMode;
            int nSrcLine, nSrcSClum, nSrcEClum;
            int nSrcSLine, nSrcELine;
            int nDstLine, nDstClum;

            int nSumLine, nSumClum;
            String sSumClum;
            int idx;
            Excel.Worksheet xlsSrcSheet;
            Excel.Worksheet xlsDstSheet;
            Excel.Range xlsSrcRng = null;
            Excel.Range xlsDstRng = null;
            String[] ary;

            nMode = 0;
            sCellVal = "";
            nDstLine = 1;
            nDstClum = 1;
            nSrcLine = 1;
            nSrcSClum = 1;
            nSrcEClum = 1;
            nSrcSLine = 1;
            nSrcELine = 1;
            sSumClum = "A";
            nSumClum = 1;
            xlsSrcSheet = m_libExcel.GetExcelSheetToName("Sheet1");
            if (xlsSrcSheet == null)
            {
                return;
            }
            xlsDstSheet = m_libExcel.GetDstExcelSheetToName("Sheet1");
            if (xlsDstSheet == null)
            {
                return;
            }
            for (line = 1, clum = 1; line < 30; line++)
            {
                xlsSrcRng = xlsSrcSheet.Cells[line, clum];
                sCellVal = xlsSrcRng.Text.ToString();
                ary = sCellVal.Split(',');
                if (ary[0] == "S")
                {
                    nDstLine = line+1;
                    nSrcLine = line+1;
                    nSrcSClum = ABCto123(ary[1]);
                    nSrcEClum = ABCto123(ary[2]);
                    nDstClum = ABCto123(ary[3]);
                    sSumClum = ary[4];
                    nSumClum = ABCto123(sSumClum);
                    xlsDstRng = xlsDstSheet.Cells[line, 1];
                    xlsDstRng.Value2 = "";
                    break;
                }
                else if (ary[0] == "P")
                {
                    nMode = 1;
                    nDstLine = line + 1;
                    nSrcSClum = ABCto123(ary[1]);
                    nSrcEClum = ABCto123(ary[2]);
                    nDstClum = ABCto123(ary[3]);
                    nSrcSLine = m_libCmn.StrToInt(ary[4]);
                    nSrcELine = m_libCmn.StrToInt(ary[5]);
                    xlsDstRng = xlsDstSheet.Cells[line, 1];
                    xlsDstRng.Value2 = "";
                    break;
                }
            }
            for (idx = 0; idx < m_nLineCount; idx++)
            {
                ary = m_aryLine[idx].Split(',');
                if (5 <= ary.Length && ary[0] == "\"未\"")
                {
                    IncProgressBar();
                    if (nMode == 0)
                    {
                        InsertExcelLine(xlsDstSheet, nDstLine, nDstLine);
                        CopyExcelPage(xlsSrcSheet, nSrcLine, nSrcSClum, nSrcLine, nSrcEClum);
                        PasteExcelPage(xlsDstSheet, nDstLine, nSrcSClum);
                    }
                    xlsDstRng = xlsDstSheet.Cells[nDstLine, nDstClum];
                    ary[5] = ary[5].Replace("\"", "");
                    ary[5] = ary[5].Replace("/", " ");
                    xlsDstRng.Value2 = ary[5].Substring(1);
                    nDstLine++;
                    if (nMode == 1)
                    {
                        if (nSrcELine < nDstLine)
                        {
                            break;
                        }
                    }
                }
            }
            if (nMode == 0)
            {
                DeleteExcelLine(xlsDstSheet, nDstLine);
                nSumLine = nDstLine;
                nDstLine = nDstLine - 1;
                xlsDstRng = xlsDstSheet.Cells[nSumLine, nSumClum];
                xlsDstRng.Value2 = "=SUM(" + sSumClum + nSrcLine + ":" + sSumClum + nDstLine + ")";
            }

        }
        private int ABCto123(String str)
        {
            int clum;
            byte[] data = System.Text.Encoding.ASCII.GetBytes(str);
            clum = data[0] - 0x40;
            return (clum);
        }
        private void IncProgressBar()
        {
            Clipboard.Clear();
            m_pgWnd.IncrementBar();
            m_wnd.DoDispatch();
        }
        private void InsertExcelLine(Excel.Worksheet xlsSheet, int sl, int el)
        {
            Excel.Range xlsAreaRng = null;
            Excel.Range xlsRngS = null;
            Excel.Range xlsRngE = null;

            xlsRngS = xlsSheet.Cells[sl, 1];
            xlsRngE = xlsSheet.Cells[el, 1];
            xlsAreaRng = xlsSheet.get_Range(xlsRngS, xlsRngE);
            xlsAreaRng.EntireRow.Insert();
        }
        private void DeleteExcelLine(Excel.Worksheet xlsSheet, int line)
        {
            Excel.Range xlsAreaRng = null;
            Excel.Range xlsRngS = null;
            Excel.Range xlsRngE = null;

            xlsRngS = xlsSheet.Cells[line, 1];
            xlsRngE = xlsSheet.Cells[line, 100];
            xlsAreaRng = xlsSheet.get_Range(xlsRngS, xlsRngE);
            xlsAreaRng.Delete();
        }
        private void CopyExcelPage(Excel.Worksheet xlsSheet, int sl, int sc, int el, int ec)
        {
            Excel.Range xlsAreaRng = null;
            Excel.Range xlsRngS = null;
            Excel.Range xlsRngE = null;

            xlsRngS = xlsSheet.Cells[sl, sc];
            xlsRngE = xlsSheet.Cells[el, ec];
            xlsAreaRng = xlsSheet.get_Range(xlsRngS, xlsRngE);
            xlsAreaRng.Copy();
        }
        private void PasteExcelPage(Excel.Worksheet xlsSheet, int sl, int sc)
        {
            Excel.Range xlsRng = null;

            xlsRng = xlsSheet.Cells[sl, sc];
            xlsRng.PasteSpecial(Excel.XlPasteType.xlPasteAll);
        }
    }
}
