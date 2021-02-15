using System;
using System.Windows.Forms;
using Excel = Microsoft.Office.Interop.Excel;

namespace sden
{
    public partial class ClsCell
    {
        public int m_iLine;
        public int m_iClum;
    }

    public class LibExcel
    {
        private Excel.Application m_ExcelApp = null;
        private Excel.Workbook m_xlsWBook = null;
        private Excel.Workbook m_xlsDstWBook = null;

        /// <summary>
        /// Excelを初期起動する。
        /// </summary>
        /// <returns></returns>
        public Boolean InitExcelApp()
        {
            try
            {
                if (m_ExcelApp == null)
                {
                    //Excelを内部起動
                    m_ExcelApp = new Excel.Application();

#if DEBUG
                    m_ExcelApp.Visible = true;
#else
                    m_ExcelApp.Visible = false;
#endif
                    
                    //Excelを見えなくする。
                    m_ExcelApp.Visible = false;
                    //m_ExcelApp.Visible = true;
                    //Excelのメッセージを強制的に表示しないようにする。
                    m_ExcelApp.DisplayAlerts = false;
                }
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "InitExcelAppエラー");
                ExcelCloseProcess();
                return(false);
            }

//            try
//            {
//#if DEBUG
////                m_ExcelApp.Visible = true;
//#else
//                m_ExcelApp.Visible = false;
//#endif
//                m_ExcelApp.Visible = false;
//            }
//            catch (Exception ex)
//            {
//                m_ExcelApp = null;
//                System.Windows.MessageBox.Show(ex.ToString(), "エラー");
//                return (false);
//            }
//            try
//            {
//                m_ExcelApp.DisplayAlerts = false;
//            }
//            catch (Exception ex)
//            {
//                m_ExcelApp = null;
//                System.Windows.MessageBox.Show(ex.ToString(), "エラー");
//                return (false);
//            }
            return (true);
        }

        /// <summary>
        /// Excelのプロセスを閉じて、
        /// 変数をnullにする。
        /// </summary>
        public void ExcelCloseProcess()
        {
            m_ExcelApp.DisplayAlerts = true;
            System.Runtime.InteropServices.Marshal.ReleaseComObject(m_ExcelApp);
            m_ExcelApp = null;
        }

        public Boolean CloseExcelApp()
        {
            try
            {
                ExcelCloseProcess();
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "CloseExcelAppエラー");
                ExcelCloseProcess();
                return (false);
            }
            return (true);
        }

        public void InitExcelBook(string sExcelFilename)
        {
            Excel.Workbooks xlsWBooks = null;
            Object non = Type.Missing;

            try
            {
                xlsWBooks = m_ExcelApp.Workbooks;
                m_xlsWBook = xlsWBooks.Open(sExcelFilename);
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "InitExcelBookエラー");
            }
        }

        public void InitDstExcelBook(string sExcelFilename)
        {
            Excel.Workbooks xlsWBooks = null;
            Object non = Type.Missing;

            try
            {
                xlsWBooks = m_ExcelApp.Workbooks;
                m_xlsDstWBook = xlsWBooks.Open(sExcelFilename);
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "InitDstExcelBookエラー");
            }
        }

        public void CreateDstExcelBook()
        {
            Object non = Type.Missing;
            int max, i;

            try
            {
                Excel.Workbooks xlsWBooks = null;
                xlsWBooks = m_ExcelApp.Workbooks;
                m_xlsDstWBook = xlsWBooks.Add();
                max = m_xlsDstWBook.Sheets.Count;
                for (i = 1; i <= max; i++)
                {
                    m_xlsDstWBook.Sheets[i].PageSetup.Orientation = Excel.XlPageOrientation.xlLandscape;
                }
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "CreateDstExcelBookエラー");
            }
        }

        public Excel.Worksheet GetExcelSheet(int iSheetNo)
        {
            Excel.Worksheet xlsSheet = null;

            try
            {
                xlsSheet = m_xlsWBook.Sheets[iSheetNo];
                return (xlsSheet);
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString(), "エラー");
                return (null);
            }
        }

        public int GetExcelSheetCount()
        {
            return (m_xlsWBook.Sheets.Count);
        }

        public int GetExcelSheetIdx(string name)
        {
            int max, i;

            Excel.Worksheet xlsSheet = null;

            max = m_xlsWBook.Sheets.Count;
            for (i = 1; i <= max; i++)
            {
                xlsSheet = m_xlsWBook.Sheets[i];
                if (xlsSheet.Name == name)
                {
                    return (i);
                }
            }
            return (-1);
        }

        public Excel.Worksheet GetExcelSheetToName(string name)
        {
            int max, i;
            Excel.Worksheet xlsSheet = null;

            max = m_xlsWBook.Sheets.Count;
            for (i = 1; i <= max; i++)
            {
                xlsSheet = m_xlsWBook.Sheets[i];
                if (xlsSheet.Name == name)
                {
                    return (xlsSheet);
                }
            }
            return (null);
        }

        public Excel.Worksheet GetDstExcelSheet(int iSheetNo)
        {
            Excel.Worksheet xlsSheet = null;

            xlsSheet = m_xlsDstWBook.Sheets[iSheetNo];
            return (xlsSheet);
        }

        public int GetDstExcelSheetCount()
        {
            return (m_xlsDstWBook.Sheets.Count);
        }

        public int GetDstExcelSheetIdx(string name)
        {
            int max, i;
            Excel.Worksheet xlsSheet = null;

            max = m_xlsDstWBook.Sheets.Count;
            for (i = 1; i <= max; i++)
            {
                xlsSheet = m_xlsDstWBook.Sheets[i];
                if (xlsSheet.Name == name)
                {
                    return (i);
                }
            }
            return (-1);
        }

        public Excel.Worksheet GetDstExcelSheetToName(string name)
        {
            int max, i;
            Excel.Worksheet xlsSheet = null;

            max = m_xlsDstWBook.Sheets.Count;
            for (i = 1; i <= max; i++)
            {
                xlsSheet = m_xlsDstWBook.Sheets[i];
                if (xlsSheet.Name == name)
                {
                    return (xlsSheet);
                }
            }
            return (null);
        }
        
        public void DeleteDstExcelSheet(int iSheetNo)
        {
            Excel.Worksheet xlsSheet = null;
            xlsSheet = m_xlsDstWBook.Sheets[iSheetNo];
            xlsSheet.Delete();
        }

        public void DeleteDstExcelSheetToName(string name)
        {
            int max, i;
            Excel.Worksheet xlsSheet = null;

            max = m_xlsDstWBook.Sheets.Count;
            for (i = max; i >= 1; i--)
            {
                xlsSheet = m_xlsDstWBook.Sheets[i];
                if (xlsSheet.Name == name)
                {
                    xlsSheet.Delete();
                }
            }
        }

        public int[] GetDstExcelPageList()
        {
            int[] iPageTbl;
            int max, i;
            int cnt, sum;

            Excel.Worksheet xlsSheet = null;
            sum = 0;
            max = m_xlsDstWBook.Sheets.Count;
            iPageTbl = new int[max + 1];
            iPageTbl[0] = sum;
            for (i = max; i >= 1; i--)
            {
                xlsSheet = m_xlsDstWBook.Sheets[i];
                cnt = xlsSheet.VPageBreaks.Count;
                sum += cnt;
                iPageTbl[i] = sum;
            }
            return (iPageTbl);
        }

        public void EndDstExcelSaveAs(string sExcelFilename)
        {
            Object non = Type.Missing;
            m_xlsWBook.Close(false, non, non);
            m_xlsDstWBook.SaveAs(sExcelFilename);
            m_xlsDstWBook.Close(true, non, non);
            m_ExcelApp.Quit();
        }

        public void EndDstExcelSave()
        {
            Object non = Type.Missing;
            m_xlsWBook.Close(false, non, non);
            m_xlsDstWBook.Save();
            m_xlsDstWBook.Close(true, non, non);
            m_ExcelApp.Quit();
        }
         public void EndExcelApp()
        {
            Object non = Type.Missing;
            m_xlsWBook.Close(true, non, non);
            m_ExcelApp.Quit();
        }
        public string CnvNumToCellClum(int clum)
        {
            // 変換の基準となるアルファベットの数とASCIIコード
            const int ALPHABET_COUNT = 26;
            int asciiFirstIndex = Convert.ToInt32('A');
            // アルファベットの数で0になるまで割り続ける
            int tempIndex = clum;
            string refStr = string.Empty;
            while (tempIndex > 0)
            {
                // 剰余を求める
                int modIndex = tempIndex % ALPHABET_COUNT;
                if (modIndex == 0)
                {
                    // 0 の場合はZに設定し、桁が増えないよう１減算する
                    modIndex = ALPHABET_COUNT;
                    tempIndex--;
                }
                // アルファベットの数で除算する
                tempIndex = tempIndex / ALPHABET_COUNT;
                // 剰余をアルファベットに変換して参照文字とする
                refStr = Convert.ToChar(asciiFirstIndex + modIndex - 1).ToString() + refStr;
            }
            return refStr;
        }
    }
}