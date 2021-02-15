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
        private int m_nCrctLineCount;
        public int m_nWaitSecond;

        public string SetWndClass(MainWindow wnd)
        {
            String strDate;
            DateTime dt = DateTime.Now;
            strDate = dt.ToString("yyyyMMddHHmmss");

            m_wnd = wnd;
            m_libCmn = m_wnd.m_libCmn;
            m_libExcel = m_wnd.m_libExcel;
            m_nWaitSecond = m_wnd.m_nWaitSecond;
            m_sCrctFile = m_wnd.m_sExePath + "\\outcrect.dat";
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
            int nMode;
            int nSrcLine, nEndLine, nSrcSClum, nSrcEClum;
            int nSrcSLine, nSrcELine;
            int nSrcPageCount; // Pageに格納できる札の数
            int nSkipLine;
            int nSrcAddLine; // 次のPage格納
            int nNextPageAddLine; // 次のPage格納
            int nDstLine, nDstClum;
            int nPageTopLine;
            int nSetLineCount;
            int nCount, nSetCount;
            int nSumLine, nSumClum;
            String sSumClum;
            int idx, max;
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
            nSrcPageCount = 1; // Pageに格納できる札の数
            nSrcAddLine = 1;
            nSkipLine = 1;
            nNextPageAddLine = 1;
            nPageTopLine = 1;
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
            // /*
            max = m_nWaitSecond;
            for (idx = 0; idx < max; idx++)
            {
                m_libCmn.DoDispatch();
                System.Threading.Thread.Sleep(1000);
            }
            // */
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
                    if (ary.Length >= 6)
                    {
                        nSkipLine = m_libCmn.StrToInt(ary[5]);
                    }
                    if (nSkipLine <= 0)
                    {
                        nSkipLine = 1;
                    }
                    nSumClum = ABCto123(sSumClum);
                    xlsDstRng = xlsDstSheet.Cells[line, 1];
                    xlsDstRng.Value2 = "";
                    break;
                }
                else if (ary[0] == "P")
                {
                    nSkipLine = 1;
                    nSrcAddLine = 1;
                    nMode = 1;
                    nDstLine = line + 1;
                    nSrcSClum = ABCto123(ary[1]);
                    nSrcEClum = ABCto123(ary[2]);
                    nDstClum = ABCto123(ary[3]);
                    nSrcELine = m_libCmn.StrToInt(ary[4]);
                    nSrcPageCount = m_libCmn.StrToInt(ary[5]);
                    if (ary.Length >= 7)
                    {
                        nSkipLine = m_libCmn.StrToInt(ary[6]);
                    }
                    nSrcAddLine = nDstLine - 1;
                    nNextPageAddLine = nSrcELine - nSrcSLine;
                    nPageTopLine = nPageTopLine + nNextPageAddLine;

                    xlsDstRng = xlsDstSheet.Cells[nPageTopLine, 1];
                    xlsDstRng.PageBreak = 1;

                    xlsDstRng = xlsDstSheet.Cells[line, 1];
                    xlsDstRng.Value2 = "";
                    break;
                }
                else if (ary[0] == "T")
                {
                    nSkipLine = 1;
                    nSrcAddLine = 1;
                    nMode = 2;
                    nDstLine = line + 1;
                    nSrcSClum = ABCto123(ary[1]);
                    nSrcEClum = ABCto123(ary[2]);
                    nDstClum = ABCto123(ary[3]);
                    nSrcELine = m_libCmn.StrToInt(ary[4]);
                    nSrcPageCount = m_libCmn.StrToInt(ary[5]);
                    if (ary.Length >= 7)
                    {
                        nSkipLine = m_libCmn.StrToInt(ary[6]);
                    }
                    nSrcAddLine = nDstLine-1;
                    nNextPageAddLine = nSrcELine - nSrcSLine + 1;
                    nPageTopLine = nPageTopLine + nNextPageAddLine;

                    xlsDstRng = xlsDstSheet.Cells[line, 1];
                    xlsDstRng.Value2 = "";
                    break;
                }
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
                    if (nMode == 0)
                    {
                        InsertExcelLine(xlsDstSheet, nDstLine, nDstLine+nSkipLine - 1);
                        nEndLine = nSrcLine + nSkipLine-1;
                        RowHeightCopy(xlsSrcSheet, nSrcLine, nEndLine, xlsDstSheet, nDstLine);
                        CopyExcelPage(xlsSrcSheet, nSrcLine, nSrcSClum, nEndLine, nSrcEClum);
                        PasteExcelPage(xlsDstSheet, nDstLine, nSrcSClum);
                    }
                    xlsDstRng = xlsDstSheet.Cells[nDstLine, nDstClum];
                    ary[5] = ary[5].Replace("\"", "");
                    ary[5] = ary[5].Replace("/", " ");
                    xlsDstRng.Value2 = ary[5].Substring(1);

                    nDstLine += nSkipLine;
                    if (nMode == 1)
                    {
                        nCount++;
                        nSetCount++;
                        if (nSrcPageCount <= nCount)
                        {
                            nCount = 0;
                            if (nSetCount < nSetLineCount)
                            {
                                RowHeightCopy(xlsSrcSheet, nSrcSLine, nSrcELine, xlsDstSheet, nPageTopLine);
                                CopyExcelPage(xlsSrcSheet, nSrcSLine, nSrcSClum, nSrcELine, nSrcEClum);
                                PasteExcelPage(xlsDstSheet, nPageTopLine, nSrcSClum);

                                //CopyPictureExcelPage(xlsSrcSheet, nSrcSLine, nSrcSClum, nSrcELine, nSrcEClum);
                                //PastePictureExcelPage(xlsDstSheet, nPageTopLine, nSrcSClum);

                                nDstLine = nPageTopLine + nSrcAddLine;
                                nPageTopLine = nPageTopLine + nNextPageAddLine;

                                xlsDstRng = xlsDstSheet.Cells[nPageTopLine - 1, 1];
                                xlsDstRng.Value2 = " ";

                                xlsDstRng = xlsDstSheet.Cells[nPageTopLine, nSrcEClum + 1];
                                xlsDstRng.PageBreak = 1;

                                xlsDstRng = xlsDstSheet.Cells[nDstLine - 1, 1];
                                xlsDstRng.Value2 = "";
                            }
                        }
                    }
                    if (nMode == 2)
                    {
                        nCount++;
                        nSetCount++;
                        if (nSrcPageCount <= nCount)
                        {
                            nCount = 0;
                            if (nSetCount < nSetLineCount)
                            {
                                nDstLine = nPageTopLine + nSrcAddLine;
                                nPageTopLine = nPageTopLine + nNextPageAddLine;
                            }
                        }
                    }
                }
            }
            if (nMode == 0)
            {
                DeleteExcelLine(xlsDstSheet, nDstLine, nDstLine + nSkipLine - 1);
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
        private void RowHeightCopy(Excel.Worksheet xlsSrcSheet, int nSrcSLine, int nSrcELine, Excel.Worksheet xlsDstSheet, int nDstSLine)
        {
            int nSrcLine, nDstLine;

            for (nSrcLine = nSrcSLine, nDstLine = nDstSLine; nSrcLine <= nSrcELine; nSrcLine++, nDstLine++)
            {
                xlsDstSheet.Rows[nDstLine].RowHeight = xlsSrcSheet.Rows[nSrcLine].RowHeight;
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
