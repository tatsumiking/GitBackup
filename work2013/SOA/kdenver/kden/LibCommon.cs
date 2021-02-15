using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;

namespace kden
{
    public class clsSyouhinIdx
    {
        public string sName;
        public int nPageNo;
        public List<int> listIdx;

        public clsSyouhinIdx()
        {
            listIdx = new List<int>();
        }
    }
    public class clsStrInt
    {
        public string sStr;
        public int nNum;
    }
    public class TreeViewItemEx : TreeViewItem
    {
        public string m_strIndex = "";
    }
    public class DataGridTextExColumn : DataGridTextColumn
    {
        public Boolean m_bNumFlag;

        public DataGridTextExColumn()
            : base()
        {
            m_bNumFlag = false;
        }
    }
    public class DataGridTextRightColumn : DataGridTextExColumn
    {
        public DataGridTextRightColumn()
            : base()
        {
            m_bNumFlag = true;
        }

        public HorizontalAlignment HorizontalAlignment
        {
            get { return (HorizontalAlignment)GetValue(HorizontalAlignmentProperty); }
            set { SetValue(HorizontalAlignmentProperty, value); }
        }

        /// <summary>水平方向の位置</summary>
        public static readonly DependencyProperty HorizontalAlignmentProperty =
            DependencyProperty.Register("HorizontalAlignment", typeof(HorizontalAlignment),
                                typeof(DataGridTextRightColumn),
                                new UIPropertyMetadata(HorizontalAlignment.Right));

        protected override FrameworkElement GenerateElement(DataGridCell cell, object dataItem)
        {
            var txtBlock = (TextBlock)base.GenerateElement(cell, dataItem);
            txtBlock.HorizontalAlignment = HorizontalAlignment;
            //txtBlock.Style;
            return txtBlock;
        }

        protected override FrameworkElement GenerateEditingElement(DataGridCell cell, object dataItem)
        {
            var textBox = (TextBox)base.GenerateEditingElement(cell, dataItem);

            textBox.TextAlignment = TextAlignment.Right;
            InputMethod.SetIsInputMethodSuspended(textBox, true);
            return textBox;
        }
    }

    public class LibCommon
    {
        private const double SEIDO = 1.0e-10;
        private List<clsStrInt> m_listNengo;

        public LibCommon()
        {
            m_listNengo = new List<clsStrInt>();
        }

        public bool IsZero(double a)
        {
            if (Math.Abs(a) < SEIDO)
                return true;

            return false;
        }
        //Double型の同じ値かチェック
        internal bool IsEqual(object p1, double b)
        {
            if (p1 is Double)
            {
                double a = (double)p1;

                double sabun = Math.Abs(a - b);

                if (IsZero(sabun) == true)
                    return true;
            }
            return false;
        }
        internal bool IsEqual(object p1, string text)
        {
            if (p1 is string)
            {
                string a = (string)p1;
                if (a == text)
                    return true;
            }
            return false;
        }
        public static bool isHankaku(string str)
        {
            Encoding sjisEnc = Encoding.GetEncoding("Shift_JIS");
            int num = sjisEnc.GetByteCount(str);
            return num == str.Length;
        }
        public int StrToInt(string str)
        {
            int num;

            if (int.TryParse(str, out num))
            { }
            else
            {
                num = 0;
            }
            return (num);
        }

        public double StrToDouble(string str)
        {
            double num;

            if (double.TryParse(str, out num))
            { }
            else
            {
                num = 0;
            }
            return (num);
        }

        public double DivideEx(double a, double b)
        {
            double c;

            if (IsZero(b))
                return 0.0;
            //if (a == 0 || b == 0)
            //{
            //    return (0);
            //}
            c = a / b;

            return (c);
        }
        public string AryToCsvString(string[] sAry)
        {
            int max, idx;
            string sData;

            sData = "";
            max = sAry.Length;
            for (idx = 0; idx < max; idx++)
            {
                sData = sData + sAry[idx] + ",";
            }
            return (sData);
        }
        public string CalcTaxPrice(string sTaxKubun, int nTanka, int nCount, double dTaxPar)
        {
            string sStr;
            int nTax, nPrice;

            if (sTaxKubun == "内税")
            {
                nTax = (int)((nTanka * nCount) / (100.0 + dTaxPar) * dTaxPar);
                nPrice = nTanka * nCount;
            }
            else if (sTaxKubun == "外税")
            {
                nTax = (int)((nTanka * nCount) * dTaxPar / 100.0);
                nPrice = nTanka * nCount + nTax;
            }
            else // 非課税、指定なし
            {
                nTax = 0;
                nPrice = nTanka * nCount;
            }
            sStr = nTax + "," + nPrice+",";
            return (sStr);
        }
        public void InitNengou(string sPath)
        {
            string sFileName;
            string[] sLine;
            int max, idx;
            string[] sAry;
            clsStrInt clsSI;

            sFileName = sPath + "\\nengo.lst";
            if (!(System.IO.File.Exists(sFileName)))
            {
                clsSI = new clsStrInt();
                clsSI.sStr = "平成"; clsSI.nNum = 1989;
                m_listNengo.Add(clsSI);
                clsSI = new clsStrInt();
                clsSI.sStr = "昭和"; clsSI.nNum = 1926;
                m_listNengo.Add(clsSI);
                clsSI = new clsStrInt();
                clsSI.sStr = "大正"; clsSI.nNum = 1912;
                m_listNengo.Add(clsSI);
                clsSI = new clsStrInt();
                clsSI.sStr = "明治"; clsSI.nNum = 1868;
                m_listNengo.Add(clsSI);
                return;
            }
            sLine = LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            // １行目コメントとしスキップBOM対策
            for(idx = 1; idx < max; idx++){
                sAry = sLine[idx].Split(',');
                if(2 <= sAry.Length){
                    clsSI = new clsStrInt();
                    clsSI.sStr = sAry[0];
                    clsSI.nNum = StrToInt(sAry[1]);
                    m_listNengo.Add(clsSI);
                }
            }
        }
        public string AnlizeNengou(String sYear)
        {
            int max, idx;
            int nYear;
            int nGG;
            string sWareki;

            sWareki = sYear;
            nYear = Int32.Parse(sYear);
            max = m_listNengo.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (nYear <= m_listNengo[idx].nNum)
                {
                    nGG = nYear - m_listNengo[idx].nNum + 1;
                    sWareki = m_listNengo[idx].sStr + nGG.ToString("D2");
                    return (sWareki);
                }
            }
            return (sWareki);
        }
        public string convWakeki(string sSeireki)
        {
            string sYY, sMM, sDD;
            string sNengou;
            string sWareki;

            if (sSeireki == "" || sSeireki == "//")
            {
                return("");
            }
            sYY = sSeireki.Substring(0, 4);
            sMM = sSeireki.Substring(5, 2);
            sDD = sSeireki.Substring(8, 2);
            sNengou = AnlizeNengou(sYY);
            sWareki = sNengou + "年" + sMM + "月" + sDD + "日";
            return (sWareki);
        }
        public string DeleteDoubleQuotation(string str)
        {
            string retstr;
            int len;

            retstr = str;
            if (str.Substring(0, 1) == "\"")
            {
                len = str.Length;
                retstr = str.Substring(1, len - 2);
            }
            else if (str.Substring(0, 2) == " \"")
            {
                len = str.Length;
                retstr = str.Substring(2, len - 3);
            }
            return (retstr);
        }
        public string NumToKan(int nNum)
        {
            string sRKan;
            string sKan;
            int num, clm;
            int max, i;

            sRKan = "";
            clm = 0;
            while (true)
            {
                if (nNum == 0)
                {
                    break;
                }
                num = nNum % 10;
                if (clm == 1 && num != 0)
                {
                    sRKan = sRKan + "十";
                }
                else if (clm == 2 && num != 0)
                {
                    sRKan = sRKan + "百";
                }
                else if (clm == 3 && num != 0)
                {
                    sRKan = sRKan + "千";
                }
                else if (clm == 4 && num != 0)
                {
                    sRKan = sRKan + "万";
                }
                else if (clm == 5 && num != 0)
                {
                    sRKan = sRKan + "十";
                }
                else if (clm == 6 && num != 0)
                {
                    sRKan = sRKan + "百";
                }
                if (num == 1)
                {
                    if (clm == 0 || clm == 4)
                    {
                        sRKan = sRKan + "一";
                    }
                }
                else if (num == 2)
                {
                    sRKan = sRKan + "二";
                }
                else if (num == 3)
                {
                    sRKan = sRKan + "三";
                }
                else if (num == 4)
                {
                    sRKan = sRKan + "四";
                }
                else if (num == 5)
                {
                    sRKan = sRKan + "五";
                }
                else if (num == 6)
                {
                    sRKan = sRKan + "六";
                }
                else if (num == 7)
                {
                    sRKan = sRKan + "七";
                }
                else if (num == 8)
                {
                    sRKan = sRKan + "八";
                }
                else if (num == 9)
                {
                    sRKan = sRKan + "九";
                }
                clm++;
                nNum = nNum / 10;
            }
            max = sRKan.Length;
            sKan = "";
            for (i = max - 1; i >= 0; i--)
            {
                sKan = sKan + sRKan.Substring(i, 1);
            }
            return (sKan);
        }
        public string NumToKanNum(int nNum)
        {
            string sRKan;
            string sKan;
            int num, clm;
            int max, i;

            sRKan = "";
            clm = 0;
            while (true)
            {
                if (nNum == 0)
                {
                    break;
                }
                num = nNum % 10;
                if (num == 0)
                {
                    sRKan = sRKan + "〇";
                }
                else if (num == 1)
                {
                    sRKan = sRKan + "一";
                }
                else if (num == 2)
                {
                    sRKan = sRKan + "二";
                }
                else if (num == 3)
                {
                    sRKan = sRKan + "三";
                }
                else if (num == 4)
                {
                    sRKan = sRKan + "四";
                }
                else if (num == 5)
                {
                    sRKan = sRKan + "五";
                }
                else if (num == 6)
                {
                    sRKan = sRKan + "六";
                }
                else if (num == 7)
                {
                    sRKan = sRKan + "七";
                }
                else if (num == 8)
                {
                    sRKan = sRKan + "八";
                }
                else if (num == 9)
                {
                    sRKan = sRKan + "九";
                }
                clm++;
                nNum = nNum / 10;
            }
            max = sRKan.Length;
            sKan = "";
            for (i = max - 1; i >= 0; i--)
            {
                sKan = sKan + sRKan.Substring(i, 1);
            }
            return (sKan);
        }
        public string NumToOldKan(int nNum)
        {
            string sRKan;
            string sKan;
            int num, clm;
            int max, i;

            sRKan = "";
            clm = 0;
            while (true)
            {
                if (nNum == 0)
                {
                    break;
                }
                num = nNum % 10;
                if (clm == 1 && num != 0)
                {
                    sRKan = sRKan + "拾";
                }
                else if (clm == 2 && num != 0)
                {
                    sRKan = sRKan + "百";
                }
                else if (clm == 3 && num != 0)
                {
                    sRKan = sRKan + "千";
                }
                else if (clm == 5 && num != 0)
                {
                    sRKan = sRKan + "拾";
                }
                else if (clm == 6 && num != 0)
                {
                    sRKan = sRKan + "百";
                }
                else if (clm == 7 && num != 0)
                {
                    sRKan = sRKan + "千";
                }
                if (clm == 4)
                {
                    if (nNum > 0)
                    {
                        sRKan = sRKan + "萬";
                    }
                }
                if (num == 1)
                {
                    if (clm == 4)
                    {
                        sRKan = sRKan + "壱";
                    }
                }
                else if (num == 2)
                {
                    sRKan = sRKan + "貮";
                }
                else if (num == 3)
                {
                    sRKan = sRKan + "参";
                }
                else if (num == 4)
                {
                    sRKan = sRKan + "四";
                }
                else if (num == 5)
                {
                    sRKan = sRKan + "伍";
                }
                else if (num == 6)
                {
                    sRKan = sRKan + "六";
                }
                else if (num == 7)
                {
                    sRKan = sRKan + "七";
                }
                else if (num == 8)
                {
                    sRKan = sRKan + "八";
                }
                else if (num == 9)
                {
                    sRKan = sRKan + "九";
                }
                clm++;
                nNum = nNum / 10;
            }
            max = sRKan.Length;
            sKan = "";
            for (i = max - 1; i >= 0; i--)
            {
                sKan = sKan + sRKan.Substring(i, 1);
            }
            return (sKan);
        }
        public string AddPriceKanma(int num)
        {
            string sStr;

            sStr = String.Format("{0:#,0}", num);
            return (sStr);
        }
        public string HanToZen(string sStr)
        {
            string sChar;
            string sRetStr;
            int max, idx;

            max = sStr.Length;
            sRetStr = "";
            for (idx = 0; idx < max; idx++)
            {
                sChar = sStr.Substring(idx, 1);
                if (sChar == "0")
                {
                    sRetStr = sRetStr + "０";
                }
                else if (sChar == "1")
                {
                    sRetStr = sRetStr + "１";
                }
                else if (sChar == "2")
                {
                    sRetStr = sRetStr + "２";
                }
                else if (sChar == "3")
                {
                    sRetStr = sRetStr + "３";
                }
                else if (sChar == "4")
                {
                    sRetStr = sRetStr + "４";
                }
                else if (sChar == "5")
                {
                    sRetStr = sRetStr + "５";
                }
                else if (sChar == "6")
                {
                    sRetStr = sRetStr + "６";
                }
                else if (sChar == "7")
                {
                    sRetStr = sRetStr + "７";
                }
                else if (sChar == "8")
                {
                    sRetStr = sRetStr + "８";
                }
                else if (sChar == "9")
                {
                    sRetStr = sRetStr + "９";
                }
                else if (sChar == ",")
                {
                    sRetStr = sRetStr + "，";
                }
                else if (sChar == "/")
                {
                    sRetStr = sRetStr + "／";
                }
                else
                {
                    sRetStr = sRetStr + sChar;
                }
            }
            return (sRetStr);
        }
        public Color GetColor(string sColor)
        {
            int len;
            int r, g, b;
            Color clr;

            len = sColor.Length;
            if (len == 6)
            {
                r = Convert.ToInt32(sColor.Substring(0, 2), 16);
                g = Convert.ToInt32(sColor.Substring(2, 2), 16);
                b = Convert.ToInt32(sColor.Substring(4, 2), 16);
            }
            else
            {
                r = 128;
                g = 128;
                b = 128;
            }
            clr = Color.FromArgb(255, (byte)r, (byte)g, (byte)b);
            return (clr);
        }

        public Color GetAColor(string sColor, string Alpha)
        {
            int len;
            int a, r, g, b;
            Color clr;

            a = Convert.ToInt32(Alpha, 16);
            len = sColor.Length;
            if (len == 6)
            {
                r = Convert.ToInt32(sColor.Substring(0, 2), 16);
                g = Convert.ToInt32(sColor.Substring(2, 2), 16);
                b = Convert.ToInt32(sColor.Substring(4, 2), 16);
            }
            else
            {
                r = 128;
                g = 128;
                b = 128;
            }
            clr = Color.FromArgb((byte)a, (byte)r, (byte)g, (byte)b);
            return (clr);
        }


        // DataGrid Cell 関係
        public void CreateAddTextCol(DataGrid dg, string name, string title, int width)
        {
            DataGridTextColumn col;
            col = new DataGridTextColumn();
            col.Header = title;
            col.Width = width;
            col.Binding = new Binding(name);
            dg.Columns.Add(col);
        }
        // DataGrid Cell 関係
        public void CreateAddRightTextCol(DataGrid dg, string name, string title, int width)
        {
            DataGridTextColumn col;
            col = new DataGridTextRightColumn();
            col.Header = title;
            col.Width = width;
            col.Binding = new Binding(name);
            dg.Columns.Add(col);
        }
        public void CreateAddPriceCol(DataGrid dg, string name, string title, int width)
        {

            DataGridTextRightColumn col;
            col = new DataGridTextRightColumn();
            col.Header = title;
            col.Width = width;
            col.Binding = new Binding(name);
            col.Binding.StringFormat = "{0:N0}";
            dg.Columns.Add(col);
        }

        public void CreateAddTextColNonEdit(DataGrid dg, string name, string title, int width)
        {

            DataGridTextColumn col;
            col = new DataGridTextColumn();
            col.Header = title;
            col.IsReadOnly = true;
            if (width == 0)
            {
                col.MinWidth = 0;
            }
            col.Width = width;
            col.Binding = new Binding(name);
            dg.Columns.Add(col);
        }
        public void CreateAddTextColHidden(DataGrid dg, string title, string name, int width)
        {

            DataGridTextColumn col;
            col = new DataGridTextColumn();
            col.Header = title;
            col.Visibility = Visibility.Hidden;
            col.IsReadOnly = true;
            if (width == 0)
                col.MinWidth = 0;
            col.Width = width;
            col.Binding = new Binding(name);
            dg.Columns.Add(col);
        }

        // ファイル関係関数
        public string[] LoadFileLineSJIS(string sFileName)
        {
            string strData;
            string[] aryLine;

            strData = LoadFileSJIS(sFileName);
            strData = strData.Replace("\r\n", "|");
            strData = strData.Replace("\n", "|");
            strData = strData.Replace("\r", "|");
            aryLine = strData.Split('|');
            return (aryLine);
        }
        public void SaveFileLineSJIS(string sFileName, string[] aryLine)
        {
            int max, idx;
            string strData;

            strData = "";
            max = aryLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                strData = strData + aryLine[idx] + "\r\n";
            }
            SaveFileSJIS(sFileName, strData);
        }
        public void SaveFileListSJIS(string sFileName, List<string> lstStr)
        {
            int max, idx;
            string strData;

            strData = "";
            max = lstStr.Count;
            for (idx = 0; idx < max; idx++)
            {
                strData = strData + lstStr[idx] + "\r\n";
            }
            SaveFileSJIS(sFileName, strData);
        }

        public string LoadFileSJIS(string sFileName)
        {
            string strData;

            strData = "";
            if (File.Exists(sFileName))
            {
                StreamReader sr = new StreamReader(sFileName, Encoding.GetEncoding("Shift_JIS"));
                strData = sr.ReadToEnd();
                sr.Close();
            }
            return (strData);
        }
        public void SaveFileSJIS(string sFileName, string sData)
        {
            StreamWriter sw = new StreamWriter(sFileName, false, Encoding.GetEncoding("Shift_JIS"));
            sw.Write(sData);
            sw.Close();
        }

        public string[] LoadFileLineUTF8(string sFileName)
        {
            string strData;
            string[] aryLine;

            strData = LoadFileUTF8(sFileName);
            strData = strData.Replace("\r\n", "|");
            strData = strData.Replace("\n", "|");
            strData = strData.Replace("\r", "|");
            aryLine = strData.Split('|');
            return (aryLine);
        }

        public void SaveFileLineUTF8(string sFileName, string[] aryLine)
        {
            int max, idx;
            string strData;

           strData = "";
            max = aryLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                strData = strData + aryLine[idx] + "\r\n";
            }
            SaveFileUTF8(sFileName, strData);
        }

        public string LoadFileUTF8(string sFileName)
        {
            string strData;

            strData = "";
            if (File.Exists(sFileName))
            {
                StreamReader sr = new StreamReader(sFileName);
                strData = sr.ReadToEnd();
                sr.Close();
            }
            return (strData);
        }
        public void SaveFileUTF8(string sFileName, string sData)
        {
            StreamWriter sw = new StreamWriter(sFileName);
            sw.Write(sData);
            sw.Close();
        }
        public bool CopyFile(string sSrcFileName, string sDstFileName)
        {
            string err;
            bool bRet = true;

            try
            {
                System.IO.File.Copy(sSrcFileName, sDstFileName, true);
            }
            catch (Exception exp)
            {//例外発生
                err = exp.Message;
                bRet = false;
            }

            return bRet;
        }
        public void CopyDirectory(string sourceDirName, string destDirName)
        {
            //コピー先のディレクトリがないときは作る
            if (!System.IO.Directory.Exists(destDirName))
            {
                System.IO.Directory.CreateDirectory(destDirName);
                //属性もコピー
                //System.IO.File.SetAttributes(destDirName,
                //    System.IO.File.GetAttributes(sourceDirName));
            }

            //コピー先のディレクトリ名の末尾に"\"をつける
            if (destDirName[destDirName.Length - 1] !=
                    System.IO.Path.DirectorySeparatorChar)
                destDirName = destDirName + System.IO.Path.DirectorySeparatorChar;

            //コピー元のディレクトリにあるファイルをコピー(true 上書きコピー)
            string[] files = System.IO.Directory.GetFiles(sourceDirName);
            foreach (string file in files)
            {
                System.IO.File.Copy(file,
                    destDirName + System.IO.Path.GetFileName(file), true);
            }
            //コピー元のディレクトリにあるディレクトリについて、再帰的に呼び出す
            string[] dirs = System.IO.Directory.GetDirectories(sourceDirName);
            foreach (string dir in dirs)
                CopyDirectory(dir, destDirName + System.IO.Path.GetFileName(dir));
        }
        public void DeleteFile(string sFileName)
        {
            System.IO.File.Delete(sFileName);
        }
        public void DeleteDirectory(string sDirName)
        {
            //コピー元のディレクトリにあるディレクトリについて、再帰的に呼び出す
            string[] dirs = System.IO.Directory.GetDirectories(sDirName);
            foreach (string dir in dirs)
            {
                DeleteDirectory(dir);
            }
            //コピー元のディレクトリにあるファイルをコピー(true 上書きコピー)
            string[] files = System.IO.Directory.GetFiles(sDirName);
            foreach (string file in files)
            {
                System.IO.File.Delete(file);
            }
            System.IO.Directory.Delete(sDirName);
        }

        public void CreatePath(string sPath)
        {
            string[] aryCsv;
            int max, idx;
            string sSubPath;

            aryCsv = sPath.Split('\\');
            max = aryCsv.Length;
            sSubPath = aryCsv[0];
            for (idx = 1; idx < max; idx++)
            {
                sSubPath = sSubPath + "\\" + aryCsv[idx];
                System.IO.Directory.CreateDirectory(sSubPath);
            }

        }

        public void BackupFile(string sSrcDataPath, string sDstDataPath, string sSrchKeyName)
        {
            string sSarch;
            int max, i;

            sSarch = sSrchKeyName + "*.xls";
            //なかったら保管先フォルダ作成
            if (!(System.IO.Directory.Exists(sDstDataPath)))
            {
                System.IO.Directory.CreateDirectory(sDstDataPath);
            }

            string[] files = Directory.GetFiles(sSrcDataPath, sSarch);
            max = files.Length;
            for (i = 0; i < max; i++)
            {
                System.IO.File.Copy(files[i],
                    sDstDataPath +"\\" + System.IO.Path.GetFileName(files[i]), true);
                try
                {
                    System.IO.File.Delete(files[i]);
                }
                catch (Exception exp)
                {//例外発生
                    string str;
                    str = exp.ToString();
                }
            }
        }

    }
}
