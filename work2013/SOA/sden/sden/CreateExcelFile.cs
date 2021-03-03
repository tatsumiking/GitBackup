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
        private String m_sSrcDefFile; 
        private String m_sSrcXlsFile;
        private String m_sDstXlsFile;
        private string[] m_aryLine;
        private int m_nCrctLineCount;
        public int m_nWaitSecond;

        private int m_nSetClum;
        private int m_nSetLine;
        private int m_nSetClum2;
        private int m_nSetLine2;
        private int m_nSetCountClum;
        private int m_nSetCountLine;
        private int m_nSetCountClum2;
        private int m_nSetCountLine2;

        private string m_sMode;
        private int m_nSrcLine, nEndLine, m_nSrcSClum, m_nSrcEClum;
        private int m_nSrcSLine, m_nSrcELine;
        private int m_nSrcPageCount; // Pageに格納できる札の数
        private int m_nSkipCount; // １つの札を構成する行数orカラム数
        private int m_nSrcAddLine; // 次のPage格納
        private String m_sSumClum;
        private int m_nDstLine, m_nDstClum;
        private int m_nSumLine, m_nSumClum;

        public string SetWndClass(MainWindow wnd)
        {
            String strDate;
            DateTime dt = DateTime.Now;
            strDate = dt.ToString("yyyyMMddHHmmss");

            m_wnd = wnd;
            m_libCmn = m_wnd.m_libCmn;
            m_libExcel = m_wnd.m_libExcel;
            m_nWaitSecond = m_wnd.m_nWaitSecond;
            m_sCrctFile = m_wnd.m_sCrctPath + "\\outcrect.dat";
            m_sSrcDefFile = m_wnd.m_sExePath + "\\template.txt";
            m_sSrcXlsFile = m_wnd.m_sExePath + "\\template.xlsx";
            m_sDstXlsFile = m_wnd.m_sExcelSavePath + "\\" + strDate + ".xlsx";
            if (System.IO.File.Exists(m_sSrcXlsFile) == false)
            {
                m_sSrcXlsFile = m_wnd.m_sExePath + "\\template.xls";
                m_sDstXlsFile = m_wnd.m_sExcelSavePath + "\\" + strDate + ".xls";
            }
            //m_sDstXlsFile = m_wnd.m_sExcelSavePath + "\\temp.xls";
            return (m_sDstXlsFile);
        }
        public Boolean Create()
        {
            String sMsg;
            string[] aryMsg;
            MessageBoxResult result;
            Boolean ret;

            if (System.IO.File.Exists(m_sCrctFile) == false)
            {
                sMsg = "貯込ファイルが見つかりません";
                System.Windows.MessageBox.Show(sMsg, "エラー");
                return (false);
            }
            if (System.IO.File.Exists(m_sSrcXlsFile) == false)
            {
                sMsg = "テンプレートファイルが見つかりません";
                System.Windows.MessageBox.Show(sMsg, "エラー");
                return (false);
            }
            if (System.IO.File.Exists(m_sDstXlsFile) == true)
            {
                sMsg = "同じ名前のファイルがありますので削除します";
                result = System.Windows.MessageBox.Show(sMsg, "確認", MessageBoxButton.YesNo);
                if (result == MessageBoxResult.No)
                {
                    return (false);
                }
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
                sMsg = ex.ToString();
                aryMsg = sMsg.Split('\n');
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
            m_nCrctLineCount = m_aryLine.Length;
            m_pgWnd = new ProgressWindows();
            m_pgWnd.SetTitle("");
            m_pgWnd.SetReng(0, m_nCrctLineCount);
            m_pgWnd.Show();

            try
            {
                if (m_libExcel.InitExcelBook(m_sSrcXlsFile) == false)
                {
                    m_libExcel.CloseExcelApp();
                    return (false);
                }
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "エラー");
                //m_libExcel.EndExcelApp();
                m_libExcel.CloseExcelApp();
                return (false);
            }

            try
            {
                if (m_libExcel.InitDstExcelBook(m_sDstXlsFile) == false)
                {
                    m_libExcel.CloseExcelBook();
                    m_libExcel.CloseExcelApp();
                    return (false);
                }

            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "エラー");
                m_libExcel.CloseExcelBook();
                //m_libExcel.EndExcelApp();
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
                m_libExcel.CloseDstExcelBook();
                m_libExcel.CloseExcelBook();
                //m_libExcel.EndExcelApp();
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
                m_libExcel.CloseDstExcelBook();
                m_libExcel.CloseExcelBook();
                //m_libExcel.EndExcelApp();
                m_libExcel.CloseExcelApp();
                return (false);
            }
            m_libExcel.CloseDstExcelBook();
            m_libExcel.CloseExcelBook();
            //m_libExcel.EndExcelApp();
            m_libExcel.CloseExcelApp();

            return (true);
        }
        private void SheetCreate()
        {
            int line, clum;
            String sCellVal;
            int nNextPageAddLine; // 次のPage格納
            int nPageTopLine;
            int nSetLineCount;
            int nCount, nSetCount;
            int idx, max;
            int setline, setclum;
            Excel.Worksheet xlsSrcSheet;
            Excel.Worksheet xlsDstSheet;
            Excel.Range xlsSrcRng = null;
            Excel.Range xlsDstRng = null;
            String[] ary;


            m_nSetLine = 0;
            m_nSetClum = 0;
            m_nSetClum2 = 0;
            m_nSetLine2 = 0;
            m_nSetCountClum = 0;
            m_nSetCountLine = 0;
            m_nSetCountClum2 = 0;
            m_nSetCountLine2 = 0;
            m_sMode = "P";
            sCellVal = "";
            m_nDstLine = 1;
            m_nDstClum = 1;
            m_nSrcLine = 1;
            m_nSrcSClum = 1;
            m_nSrcEClum = 1;
            m_nSrcSLine = 1;
            m_nSrcELine = 1;
            m_nSrcPageCount = 1; // Pageに格納できる札の数
            m_nSrcAddLine = 1;
            m_nSkipCount = 1;
            m_sSumClum = "A";
            m_nSumClum = 1;

            nNextPageAddLine = 1;
            nPageTopLine = 1;
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
            // /*
            max = m_nWaitSecond;
            for (idx = 0; idx < max; idx++)
            {
                m_libCmn.DoDispatch();
                System.Threading.Thread.Sleep(1000);
            }
            // */

            String[] aryLine = m_libCmn.LoadFileLineSJIS(m_sSrcDefFile);
            if (aryLine[0] == "")
            {
                for (line = 1, clum = 1; line < 30; line++)
                {
                    xlsSrcRng = xlsSrcSheet.Cells[line, clum];
                    sCellVal = xlsSrcRng.Text.ToString();
                    if (sCellVal != "")
                    {
                        m_nSetClum = 3;
                        m_nSetLine = line;
                        ary = sCellVal.Split(',');
                        if (ary[0] == "S" || ary[0] == "P" || ary[0] == "T"
                        ||  ary[0] == "p" || ary[0] == "t")
                        {
                            setParameter(ary);
                            break;
                        }
                    }
                }
                xlsDstRng = xlsDstSheet.Cells[line, 1];
                xlsDstRng.Value2 = "";
            }else{
                ary = aryLine[1].Split(',');
                m_nSetClum = ABCto123(ary[0]);
                m_nSetLine = m_libCmn.StrToInt(ary[1]);
                if (ary.Length >= 4)
                {
                    m_nSetClum2 = ABCto123(ary[2]);
                    m_nSetLine2 = m_libCmn.StrToInt(ary[3]);
                }
                ary = aryLine[2].Split(',');
                setParameter(ary);
                if (aryLine.Length >= 4 && aryLine[3] != "")
                {
                    ary = aryLine[3].Split(',');
                    m_nSetCountClum = ABCto123(ary[0]);
                    m_nSetCountLine = m_libCmn.StrToInt(ary[1]);
                    if (ary.Length >= 4)
                    {
                        m_nSetCountClum2 = ABCto123(ary[2]);
                        m_nSetCountLine2 = m_libCmn.StrToInt(ary[3]);
                    }

                }
            }

            if (m_sMode == "P")
            {
                nNextPageAddLine = m_nSrcELine - m_nSrcSLine;
                nPageTopLine = nPageTopLine + nNextPageAddLine;

                xlsDstRng = xlsDstSheet.Cells[nPageTopLine, 1];
                xlsDstRng.PageBreak = 1;
            }
            else if (m_sMode == "T")
            {
                nNextPageAddLine = m_nSrcELine - m_nSrcSLine + 1;
                nPageTopLine = nPageTopLine + nNextPageAddLine;
            }
            else if (m_sMode == "p")
            {
                nNextPageAddLine = m_nSrcELine - m_nSrcSLine + 1;
                nPageTopLine = nPageTopLine + nNextPageAddLine;

                xlsDstRng = xlsDstSheet.Cells[nPageTopLine, 1];
                xlsDstRng.PageBreak = 1;
            }
            else if (m_sMode == "t")
            {
                nNextPageAddLine = m_nSrcELine - m_nSrcSLine + 1;
                nPageTopLine = nPageTopLine + nNextPageAddLine;
            }



            nSetLineCount = 0;
            for (idx = 0; idx < m_nCrctLineCount; idx++)
            {
                ary = m_aryLine[idx].Split(',');
                if (5 <= ary.Length && ary[0] == "\"未\"")
                {
                    nSetLineCount++;
                }
            }
            nSetCount = 0;
            nCount = 0;
            for (idx = 0; idx < m_nCrctLineCount; idx++)
            {
                ary = m_aryLine[idx].Split(',');
                if (5 <= ary.Length && ary[0] == "\"未\"")
                {
                    IncProgressBar();
                    if (m_sMode == "S")
                    {
                        InsertExcelLine(xlsDstSheet, m_nDstLine, m_nDstLine+m_nSkipCount - 1);
                        nEndLine = m_nSrcLine + m_nSkipCount-1;
                        RowHeightCopy(xlsSrcSheet, m_nSrcLine, nEndLine, xlsDstSheet, m_nDstLine);
                        CopyExcelPage(xlsSrcSheet, m_nSrcLine, m_nSrcSClum, nEndLine, m_nSrcEClum);
                        PasteExcelPage(xlsDstSheet, m_nDstLine, m_nSrcSClum);
                    }
                    xlsDstRng = xlsDstSheet.Cells[m_nDstLine, m_nDstClum];
                    ary[5] = ary[5].Replace("\"", "");
                    ary[5] = ary[5].Replace("/", " ");
                    xlsDstRng.Value2 = ary[5].Substring(1);
                    if (m_nSetClum2 != 0 && m_nSetLine2 != 0)
                    {
                        setline = m_nDstLine + m_nSetLine2 - m_nSetLine;
                        setclum = m_nDstClum + m_nSetClum2 - m_nSetClum;
                        xlsDstRng = xlsDstSheet.Cells[setline, setclum];
                        ary[5] = ary[5].Replace("\"", "");
                        ary[5] = ary[5].Replace("/", " ");
                        xlsDstRng.Value2 = ary[5].Substring(1);
                    }
                    if (m_nSetCountClum != 0 && m_nSetCountLine != 0)
                    {
                        setline = m_nDstLine + m_nSetCountLine - m_nSetLine;
                        setclum = m_nDstClum + m_nSetCountClum - m_nSetClum;
                        xlsDstRng = xlsDstSheet.Cells[setline, setclum];
                        ary[4] = ary[4].Replace("\"", "");
                        xlsDstRng.Value2 = ary[4];
                    }
                    if (m_nSetCountClum2 != 0 && m_nSetCountLine2 != 0)
                    {
                        setline = m_nDstLine + m_nSetCountLine2 - m_nSetLine;
                        setclum = m_nDstClum + m_nSetCountClum2 - m_nSetClum;
                        xlsDstRng = xlsDstSheet.Cells[setline, setclum];
                        ary[4] = ary[4].Replace("\"", "");
                        xlsDstRng.Value2 = ary[4];
                    }

                    if (m_sMode == "S" || m_sMode == "P"
                     || m_sMode == "D" || m_sMode == "T")
                    {
                        m_nDstLine += m_nSkipCount;
                    }
                    else // "p","t"
                    {
                        m_nDstClum -= m_nSkipCount;
                    }

                    if (m_sMode == "P" || m_sMode == "p"
                     || m_sMode == "D" || m_sMode == "t"
                     || m_sMode == "T" || m_sMode == "t")
                    {
                        nCount++;
                        nSetCount++;
                        if (m_nSrcPageCount <= nCount)
                        {
                            nCount = 0;
                            if (nSetCount < nSetLineCount)
                            {
                                if (m_sMode == "P" || m_sMode == "p")
                                {
                                    RowHeightCopy(xlsSrcSheet, m_nSrcSLine, m_nSrcELine, xlsDstSheet, nPageTopLine);
                                    CopyExcelPage(xlsSrcSheet, m_nSrcSLine, m_nSrcSClum, m_nSrcELine, m_nSrcEClum);
                                    PasteExcelPage(xlsDstSheet, nPageTopLine, m_nSrcSClum);
                                }
                                if (m_sMode == "P" || m_sMode == "T")
                                {
                                    m_nDstLine = nPageTopLine + m_nSrcAddLine;
                                }
                                else if (m_sMode == "p" || m_sMode == "t")
                                {
                                    m_nDstClum = m_nSetClum;
                                    m_nDstLine += (m_nSrcELine - m_nSrcSLine + 1);
                                }

                                nPageTopLine = nPageTopLine + nNextPageAddLine;

                                if (m_sMode == "P" || m_sMode == "p")
                                {
                                    xlsDstRng = xlsDstSheet.Cells[nPageTopLine - 1, 1];
                                    xlsDstRng.Value2 = " ";

                                    xlsDstRng = xlsDstSheet.Cells[nPageTopLine, m_nSrcEClum + 1];
                                    xlsDstRng.PageBreak = 1;

                                    xlsDstRng = xlsDstSheet.Cells[m_nDstLine - 1, 1];
                                    xlsDstRng.Value2 = "";
                                }
                            }
                        }
                    }
                }
            }
            if (m_sMode == "S")
            {
                DeleteExcelLine(xlsDstSheet, m_nDstLine, m_nDstLine + m_nSkipCount - 1);
                m_nSumLine = m_nDstLine;
                m_nDstLine = m_nDstLine - 1;
                xlsDstRng = xlsDstSheet.Cells[m_nSumLine, m_nSumClum];
                xlsDstRng.Value2 = "=SUM(" + m_sSumClum + m_nSrcLine + ":" + m_sSumClum + m_nDstLine + ")";
            }

        }
        private void setParameter(String[] ary)
        {
            if (ary[0] == "S")
            {
                m_sMode = "S";
                m_nDstLine = m_nSetLine + 1;
                m_nSrcLine = m_nSetLine + 1;
                m_nSrcSClum = ABCto123(ary[1]);
                m_nSrcEClum = ABCto123(ary[2]);
                m_nDstClum = ABCto123(ary[3]);
                m_sSumClum = ary[4];
                if (ary.Length >= 6)
                {
                    m_nSkipCount = m_libCmn.StrToInt(ary[5]);
                }
                if (m_nSkipCount <= 0)
                {
                    m_nSkipCount = 1;
                }
                m_nSumClum = ABCto123(m_sSumClum);
            }
            else if (ary[0] == "P")
            {
                m_nSkipCount = 1;
                m_nSrcAddLine = 1;
                m_sMode = "P";
                m_nDstLine = m_nSetLine + 1;
                m_nSrcSClum = ABCto123(ary[1]);
                m_nSrcEClum = ABCto123(ary[2]);
                m_nDstClum = ABCto123(ary[3]);
                m_nSrcELine = m_libCmn.StrToInt(ary[4]);
                m_nSrcPageCount = m_libCmn.StrToInt(ary[5]);
                if (ary.Length >= 7)
                {
                    m_nSkipCount = m_libCmn.StrToInt(ary[6]);
                }
                m_nSrcAddLine = m_nDstLine - 1;

            }
            else if (ary[0] == "T")
            {
                m_nSkipCount = 1;
                m_nSrcAddLine = 1;
                m_sMode = "T";
                m_nDstLine = m_nSetLine + 1;
                m_nSrcSClum = ABCto123(ary[1]);
                m_nSrcEClum = ABCto123(ary[2]);
                m_nDstClum = ABCto123(ary[3]);
                m_nSrcELine = m_libCmn.StrToInt(ary[4]);
                m_nSrcPageCount = m_libCmn.StrToInt(ary[5]);
                if (ary.Length >= 7)
                {
                    m_nSkipCount = m_libCmn.StrToInt(ary[6]);
                }
                m_nSrcAddLine = m_nDstLine - 1;


            }
            else if (ary[0] == "D")
            {
                m_nSkipCount = 1;
                m_nSrcAddLine = 1;
                m_sMode = "P";
                m_nDstLine = m_nSetLine;
                m_nSrcSClum = ABCto123(ary[1]);
                m_nSrcEClum = ABCto123(ary[2]);
                m_nDstClum = ABCto123(ary[3]);
                m_nSrcELine = m_libCmn.StrToInt(ary[4]);
                m_nSrcPageCount = m_libCmn.StrToInt(ary[5]);
                if (ary.Length >= 7)
                {
                    m_nSkipCount = m_libCmn.StrToInt(ary[6]);
                }
                m_nSrcAddLine = m_nDstLine - 1;
            }
            else if (ary[0] == "p")
            {
                m_nSkipCount = 1;
                m_nSrcAddLine = 1;
                m_sMode = "p";
                m_nSrcSClum = ABCto123(ary[1]);
                m_nSrcEClum = ABCto123(ary[2]);
                m_nDstClum = ABCto123(ary[3]);
                m_nSrcSLine = m_libCmn.StrToInt(ary[4]);
                m_nSrcELine = m_libCmn.StrToInt(ary[5]);
                m_nDstLine = m_libCmn.StrToInt(ary[6]);
                m_nSrcPageCount = m_libCmn.StrToInt(ary[7]);
                if (ary.Length >= 9)
                {
                    m_nSkipCount = m_libCmn.StrToInt(ary[8]);
                }
                m_nSrcAddLine = m_nDstLine - 1;
            }
            else if (ary[0] == "t")
            {
                m_nSkipCount = 1;
                m_nSrcAddLine = 1;
                m_sMode = "t";
                m_nSrcSClum = ABCto123(ary[1]);
                m_nSrcEClum = ABCto123(ary[2]);
                m_nDstClum = ABCto123(ary[3]);
                m_nSrcSLine = m_libCmn.StrToInt(ary[4]);
                m_nSrcELine = m_libCmn.StrToInt(ary[5]);
                m_nDstLine = m_libCmn.StrToInt(ary[6]);
                m_nSrcPageCount = m_libCmn.StrToInt(ary[7]);
                if (ary.Length >= 9)
                {
                    m_nSkipCount = m_libCmn.StrToInt(ary[8]);
                }
                m_nSrcAddLine = m_nDstLine - 1;
            }
        }
        private int ABCto123(String str)
        {
            int clum;

            byte[] data = System.Text.Encoding.ASCII.GetBytes(str);
            if (data.Length == 1)
            {
                clum = data[0] - 0x40;
            }
            else
            {
                clum = (data[0] - 0x40) * 26 + data[1] - 0x40;
            }
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
        private void DeleteExcelLine(Excel.Worksheet xlsSheet, int sline, int eline)
        {
            Excel.Range xlsAreaRng = null;
            Excel.Range xlsRngS = null;
            Excel.Range xlsRngE = null;

            xlsRngS = xlsSheet.Cells[sline, 1];
            xlsRngE = xlsSheet.Cells[eline, 100];
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
        private void CopyPictureExcelPage(Excel.Worksheet xlsSheet, int sl, int sc, int el, int ec)
        {
            Excel.Range xlsAreaRng = null;
            Excel.Range xlsRngS = null;
            Excel.Range xlsRngE = null;

            xlsRngS = xlsSheet.Cells[sl, sc];
            xlsRngE = xlsSheet.Cells[el, ec];
            xlsAreaRng = xlsSheet.get_Range(xlsRngS, xlsRngE);
            xlsAreaRng.CopyPicture();
        }
        private void RowHeightCopy(Excel.Worksheet xlsSrcSheet, int m_nSrcSLine, int m_nSrcELine, Excel.Worksheet xlsDstSheet, int nDstSLine)
        {
            int m_nSrcLine, m_nDstLine;

            for (m_nSrcLine = m_nSrcSLine, m_nDstLine = nDstSLine; m_nSrcLine <= m_nSrcELine; m_nSrcLine++, m_nDstLine++)
            {
                xlsDstSheet.Rows[m_nDstLine].RowHeight = xlsSrcSheet.Rows[m_nSrcLine].RowHeight;
            }
        }
        private void PasteExcelPage(Excel.Worksheet xlsSheet, int sl, int sc)
        {
            Excel.Range xlsRng = null;

            xlsRng = xlsSheet.Cells[sl, sc];
            try
            {
                xlsRng.PasteSpecial(Excel.XlPasteType.xlPasteAll);
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "PasteExcelPage");
            }
        }
        private void PastePictureExcelPage(Excel.Worksheet xlsSheet, int sl, int sc)
        {
            Excel.Range xlsRng = null;

            xlsRng = xlsSheet.Cells[sl, sc];
            try
            {
                xlsRng.PasteSpecial(Excel.XlPasteType.xlPasteAll);
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "PasteExcelPage");
            }
        }
    }
}
