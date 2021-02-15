using Microsoft.Win32;
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
using System.Windows.Markup;
using System.Windows.Media;

namespace instr
{
    public class ClsFontPathGeometry
    {
        public double dMoveY;
        public PathGeometry pg;
    }
    public class ClsFontGlyph
    {
        public string str;
        public double fontsize;
        public ClsMatrix mtrx;
        public PathGeometry pg;
        public double dBaseLineTime;
        public double dLineSpacingTime;
    }
    public class ClsMatrix
    {
        public double xsize, ysize;
        public double a11, a12, a13;
        public double a21, a22, a23;

        public ClsMatrix()
        {
            InitMatrix();
        }

        public void InitMatrix()
        {
            a11 = 1.0; a12 = 0.0; a13 = 0.0;
            a21 = 0.0; a22 = 1.0; a23 = 0.0;
        }
        public Point TrnsPoint(Point pt)
        {
            double x, y;

            x = pt.X - xsize / 2;
            y = pt.Y + ysize / 2;
            pt.X = x * a11 + y * a12 + a13 + xsize / 2;
            pt.Y = x * a21 + y * a22 + a23 - ysize / 2;
            return (pt);
        }
    }
    public class ClsArea
    {
        public Boolean flag;
        public double sx;
        public double sy;
        public double ex;
        public double ey;
        public double sizex;
        public double sizey;

        public ClsArea()
        {
            flag = false;
            sx = Constants.NUMMAX; // 格納される可能性のある最大値
            sy = Constants.NUMMAX;
            ex = -Constants.NUMMAX; //  格納される可能性のある最小値
            ey = -Constants.NUMMAX;
        }
        public void SetPoint(Point pt)
        {
            flag = true;
            sx = (pt.X < sx) ? pt.X : sx; // if(pt.X<sx){sx=pt.X;}else{sx=sx;}
            sy = (pt.Y < sy) ? pt.Y : sy;
            ex = (ex < pt.X) ? pt.X : ex;
            ey = (ey < pt.Y) ? pt.Y : ey;
        }
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
        private double m_dFontSize;

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
        public string DeleteDoubleQuotation( string str)
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
        public string AnlizeNengou(String sYear)
        {
            int nYear;
            int nGG;

            nYear = Int32.Parse(sYear);
            if (1989 < nYear)
            {
                nGG = nYear - 1989 + 1;
                return ("平成" + nGG.ToString("D2"));
            }
            else if (nYear >= 1926)
            {
                nGG = nYear - 1926 + 1;
                return ("昭和" + nGG.ToString("D2"));
            }
            else if (nYear >= 1912)
            {
                nGG = nYear - 1912 + 1;
                return ("大正" + nGG.ToString("D2"));
            }
            else if (nYear >= 1868)
            {
                nGG = nYear - 1868 + 1;
                return ("明治" + nGG.ToString("D2"));
            }
            return (sYear);
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
                else
                {
                    sRetStr = sRetStr + sChar;
                }
            }
            return (sRetStr);
        }
        public string DeleteBackSpace(string sStr)
        {
            string sRetStr;
            int len, lest;

            sRetStr = sStr;
            len = sStr.Length;
            for (lest = len; lest <= 0; lest--)
            {
                if (sStr.Substring(lest, 1) != " ")
                {
                    sRetStr = sStr.Substring(0, lest);
                    break;
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
        public void CreateAddCheckBoxCol(DataGrid dg, string name, string title, int width)
        {
            DataGridCheckBoxColumn cbcol;

            cbcol = new DataGridCheckBoxColumn();
            cbcol.Header = title;
            cbcol.Binding = new Binding(name);
            dg.Columns.Add(cbcol);

        }
        public void CreateAddCommboBoxCol(DataGrid dg, string name, string title, List<string> list)
        {
            DataGridComboBoxColumn ccol;
            ccol = new DataGridComboBoxColumn();
            ccol.ItemsSource = list;
            ccol.Header = title;
            ccol.SelectedValueBinding = new Binding(name);
            dg.Columns.Add(ccol);
        }

        public void CreateAddCommboBoxCol(DataGrid dg, string name, string title, List<string> list, int width)
        {
            DataGridComboBoxColumn col;
            col = new DataGridComboBoxColumn();
            col.ItemsSource = list;
            col.Header = title;
            if (width == 0)
                col.MinWidth = 0;
            col.Width = width;
            col.SelectedValueBinding = new Binding(name);
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
        /*
        PathGeometry pg = m_libCmn.GetFontPathGeometry("あ", 20.0, "HGP白洲太楷書体");
        // "ＭＳ 明朝");
        // "ＤＦ平成ゴシック体W9");
        // "ヒラギノ特太行書");
        */
        public ClsFontPathGeometry GetFontPathGeometry(string str, double fontsize, string fontname)
        {
            char ch;
            Uri uri;
            ushort glyphIndex;
            Typeface typeFace;
            GlyphTypeface gt;
            Geometry gm;
            ClsFontPathGeometry clsFGP;
            string name, fontkey;
            int max, idx;

            ch = Convert.ToChar(str);
            m_dFontSize = fontsize;
            typeFace = new Typeface(fontname);

            RegistryKey regkey =
                Registry.LocalMachine.OpenSubKey(@"Software\Microsoft\Windows NT\CurrentVersion\Fonts", false);
            string[] keys = regkey.GetValueNames();
            fontkey = "";
            max = keys.Length;
            for (idx = 0; idx < max; idx++)
            {
                if (keys[idx].Contains(fontname) == true)
                {
                    fontkey = keys[idx];
                    break;
                }
            }
            if (fontkey == "")
            {
                FontFamily fm = new FontFamily(fontname);
                // fm.FamilyNames.TryGetValue(XmlLanguage.GetLanguage("en-us"), out name);
                name = fm.FamilyNames.Values.ToList()[0];
                for (idx = 0; idx < max; idx++)
                {
                    if (keys[idx].Contains(name) == true)
                    {
                        fontkey = keys[idx];
                        break;
                    }
                }
            }
            string ttname = (string)regkey.GetValue(fontkey);
            uri = new Uri("C:\\Windows\\Fonts\\" + ttname);
            //uri = new Uri(fontpath);
            gt = new GlyphTypeface(uri);
            gt.CharacterToGlyphMap.TryGetValue((int)ch, out glyphIndex);

            gm = gt.GetGlyphOutline(glyphIndex, fontsize, fontsize);
            clsFGP = new ClsFontPathGeometry();
            clsFGP.pg = gm.GetOutlinedPathGeometry();
            clsFGP.dMoveY = (gt.Baseline) * fontsize;
            return (clsFGP);
        }
        public PathGeometry MatrixTrnsPathGeometry(PathGeometry pg, ClsMatrix clsMatrix)
        {
            PathGeometry tpg;
            int pfmax, pfidx;
            PathFigure pf, tpf;
            int psmax, psidx;
            PathSegment ps, tps;
            BezierSegment bz, tbz;
            PolyBezierSegment pbz, tpbz;
            LineSegment ln, tln;
            PolyLineSegment pln, tpln;
            string stype;
            int ptmax, ptidx;

            tpg = pg;
            pfmax = pg.Figures.Count;
            for (pfidx = 0; pfidx < pfmax; pfidx++)
            {
                pf = pg.Figures[pfidx];
                tpf = tpg.Figures[pfidx];
                tpf.StartPoint = clsMatrix.TrnsPoint(pf.StartPoint);
                psmax = pf.Segments.Count;
                for (psidx = 0; psidx < psmax; psidx++)
                {
                    ps = pf.Segments[psidx];
                    tps = tpf.Segments[psidx];
                    stype = ps.GetType().Name;
                    if (stype.Equals("PolyBezierSegment"))
                    {
                        pbz = (PolyBezierSegment)ps;
                        tpbz = (PolyBezierSegment)tps;
                        ptmax = pbz.Points.Count;
                        for (ptidx = 0; ptidx < ptmax; ptidx = ptidx + 3)
                        {
                            tpbz.Points[ptidx] = clsMatrix.TrnsPoint(pbz.Points[ptidx]);
                            tpbz.Points[ptidx + 1] = clsMatrix.TrnsPoint(pbz.Points[ptidx + 1]);
                            tpbz.Points[ptidx + 2] = clsMatrix.TrnsPoint(pbz.Points[ptidx + 2]);
                        }
                    }
                    else if (stype.Equals("PolyLineSegment"))
                    {
                        pln = (PolyLineSegment)ps;
                        tpln = (PolyLineSegment)tps;
                        ptmax = pln.Points.Count;
                        for (ptidx = 0; ptidx < ptmax; ptidx++)
                        {
                            tpln.Points[ptidx] = clsMatrix.TrnsPoint(pln.Points[ptidx]);
                        }
                    }
                    else if (stype.Equals("BezierSegment"))
                    {
                        bz = (BezierSegment)ps;
                        tbz = (BezierSegment)tps;
                        tbz.Point1 = clsMatrix.TrnsPoint(bz.Point1);
                        tbz.Point2 = clsMatrix.TrnsPoint(bz.Point2);
                        tbz.Point3 = clsMatrix.TrnsPoint(bz.Point3);
                    }
                    else if (stype.Equals("LineSegment"))
                    {
                        ln = (LineSegment)ps;
                        tln = (LineSegment)tps;
                        tln.Point = clsMatrix.TrnsPoint(ln.Point);
                    }
                    else
                    {
                    }
                }
            }
            return (tpg);
        }

        public ClsFontGlyph TrnsYokoGakiGlyph(ClsFontGlyph clsFG)
        {
            ClsMatrix clsMatrix;
            int pfmax, pfidx;
            PathFigure pf;
            int psmax, psidx;
            PathSegment ps;
            BezierSegment bz;
            PolyBezierSegment pbz;
            LineSegment ln;
            PolyLineSegment pln;
            Point pt;
            string stype;
            int ptmax, ptidx;
            string msg;

            if (clsFG.pg == null)
            {
                return (clsFG);
            }
            clsFG.pg = clsFG.pg.CloneCurrentValue();
            clsMatrix = SetMatrixYokoElementSet(clsFG);
            clsFG.mtrx = clsMatrix;
            pfmax = clsFG.pg.Figures.Count;
            for (pfidx = 0; pfidx < pfmax; pfidx++)
            {
                pf = clsFG.pg.Figures[pfidx];
                pt = clsMatrix.TrnsPoint(pf.StartPoint);
                pf.StartPoint = pt;
                psmax = pf.Segments.Count;
                for (psidx = 0; psidx < psmax; psidx++)
                {
                    ps = pf.Segments[psidx];
                    stype = ps.GetType().Name;
                    if (stype.Equals("PolyBezierSegment"))
                    {
                        pbz = (PolyBezierSegment)ps;
                        ptmax = pbz.Points.Count;
                        for (ptidx = 0; ptidx < ptmax; ptidx = ptidx + 3)
                        {
                            pbz.Points[ptidx] = clsMatrix.TrnsPoint(pbz.Points[ptidx]);
                            pbz.Points[ptidx + 1] = clsMatrix.TrnsPoint(pbz.Points[ptidx + 1]);
                            pbz.Points[ptidx + 2] = clsMatrix.TrnsPoint(pbz.Points[ptidx + 2]);
                        }
                    }
                    else if (stype.Equals("PolyLineSegment"))
                    {
                        pln = (PolyLineSegment)ps;
                        ptmax = pln.Points.Count;
                        for (ptidx = 0; ptidx < ptmax; ptidx++)
                        {
                            pln.Points[ptidx] = clsMatrix.TrnsPoint(pln.Points[ptidx]);
                        }
                    }
                    else if (stype.Equals("BezierSegment"))
                    {
                        bz = (BezierSegment)ps;
                        bz.Point1 = clsMatrix.TrnsPoint(bz.Point1);
                        bz.Point2 = clsMatrix.TrnsPoint(bz.Point2);
                        bz.Point3 = clsMatrix.TrnsPoint(bz.Point3);
                    }
                    else if (stype.Equals("LineSegment"))
                    {
                        ln = (LineSegment)ps;
                        ln.Point = clsMatrix.TrnsPoint(ln.Point);
                    }
                    else
                    {
                    }
                }
            }
            return (clsFG);
        }
        public ClsFontGlyph TrnsTateGakiGlyph(ClsFontGlyph clsFG)
        {
            ClsMatrix clsMatrix;
            int pfmax, pfidx;
            PathFigure pf;
            int psmax, psidx;
            PathSegment ps;
            BezierSegment bz;
            PolyBezierSegment pbz;
            LineSegment ln;
            PolyLineSegment pln;
            Point pt;
            string stype;
            int ptmax, ptidx;
            string msg;

            if (clsFG.pg == null)
            {
                return (clsFG);
            }
            clsFG.pg = clsFG.pg.CloneCurrentValue();
            clsMatrix = SetMatrixTateElementSet(clsFG);
            clsFG.mtrx = clsMatrix;
            pfmax = clsFG.pg.Figures.Count;
            for (pfidx = 0; pfidx < pfmax; pfidx++)
            {
                pf = clsFG.pg.Figures[pfidx];
                pt = clsMatrix.TrnsPoint(pf.StartPoint);
                pf.StartPoint = pt;
                psmax = pf.Segments.Count;
                for (psidx = 0; psidx < psmax; psidx++)
                {
                    ps = pf.Segments[psidx];
                    stype = ps.GetType().Name;
                    if (stype.Equals("PolyBezierSegment"))
                    {
                        pbz = (PolyBezierSegment)ps;
                        ptmax = pbz.Points.Count;
                        for (ptidx = 0; ptidx < ptmax; ptidx = ptidx + 3)
                        {
                            pbz.Points[ptidx] = clsMatrix.TrnsPoint(pbz.Points[ptidx]);
                            pbz.Points[ptidx + 1] = clsMatrix.TrnsPoint(pbz.Points[ptidx + 1]);
                            pbz.Points[ptidx + 2] = clsMatrix.TrnsPoint(pbz.Points[ptidx + 2]);
                        }
                    }
                    else if (stype.Equals("PolyLineSegment"))
                    {
                        pln = (PolyLineSegment)ps;
                        ptmax = pln.Points.Count;
                        for (ptidx = 0; ptidx < ptmax; ptidx++)
                        {
                            pln.Points[ptidx] = clsMatrix.TrnsPoint(pln.Points[ptidx]);
                        }
                    }
                    else if (stype.Equals("BezierSegment"))
                    {
                        bz = (BezierSegment)ps;
                        bz.Point1 = clsMatrix.TrnsPoint(bz.Point1);
                        bz.Point2 = clsMatrix.TrnsPoint(bz.Point2);
                        bz.Point3 = clsMatrix.TrnsPoint(bz.Point3);
                    }
                    else if (stype.Equals("LineSegment"))
                    {
                        ln = (LineSegment)ps;
                        ln.Point = clsMatrix.TrnsPoint(ln.Point);
                    }
                    else
                    {
                    }
                }
            }
            return (clsFG);
        }
        public ClsArea GetGlyphSize(PathGeometry pg)
        {
            ClsArea clsArea;
            int pfmax, pfidx;
            PathFigure pf;
            int psmax, psidx;
            PathSegment ps;
            BezierSegment bz;
            PolyBezierSegment pbz;
            LineSegment ln;
            PolyLineSegment pln;
            string stype;
            Point pt0, pt1, pt2, pt3;
            int ptmax, ptidx;
            string msg;

            clsArea = new ClsArea();
            if (pg == null)
            {
                pt0 = new Point(0, 0);
                clsArea.SetPoint(pt0);
                pt0.X = m_dFontSize;
                pt0.Y = m_dFontSize;
                clsArea.SetPoint(pt0);
                clsArea.sizex = m_dFontSize;
                clsArea.sizey = m_dFontSize;
                return (clsArea);
            }
            pfmax = pg.Figures.Count;
            for (pfidx = 0; pfidx < pfmax; pfidx++)
            {
                pf = pg.Figures[pfidx];
                pt0 = pf.StartPoint;
                clsArea.SetPoint(pt0);
                psmax = pf.Segments.Count;
                for (psidx = 0; psidx < psmax; psidx++)
                {
                    ps = pf.Segments[psidx];
                    stype = ps.GetType().Name;
                    if (stype.Equals("PolyBezierSegment"))
                    {
                        pbz = (PolyBezierSegment)ps;
                        ptmax = pbz.Points.Count;
                        for (ptidx = 0; ptidx < ptmax; ptidx = ptidx + 3)
                        {
                            pt1 = pbz.Points[ptidx];
                            pt2 = pbz.Points[ptidx + 1];
                            pt3 = pbz.Points[ptidx + 2];
                            clsArea.SetPoint(pt1);
                            clsArea.SetPoint(pt2);
                            clsArea.SetPoint(pt3);

                            pt0 = pt3;
                        }
                    }
                    else if (stype.Equals("PolyLineSegment"))
                    {
                        pln = (PolyLineSegment)ps;
                        ptmax = pln.Points.Count;
                        for (ptidx = 0; ptidx < ptmax; ptidx++)
                        {
                            pt1 = pln.Points[ptidx];
                            clsArea.SetPoint(pt1);

                            pt0 = pt1;
                        }
                    }
                    else if (stype.Equals("BezierSegment"))
                    {
                        bz = (BezierSegment)ps;
                        pt1 = bz.Point1;
                        pt2 = bz.Point2;
                        pt3 = bz.Point3;
                        clsArea.SetPoint(pt1);
                        clsArea.SetPoint(pt2);
                        clsArea.SetPoint(pt3);

                        pt0 = pt3;
                    }
                    else if (stype.Equals("LineSegment"))
                    {
                        ln = (LineSegment)ps;
                        pt1 = ln.Point;
                        clsArea.SetPoint(pt1);

                        pt0 = pt1;
                    }
                    else
                    {
                        msg = "「" + stype + "」開発元に連絡し処理を追加してください";
                        MessageBox.Show(msg, "確認", MessageBoxButton.OK);
                    }
                }
            }
            if (clsArea.flag == false)
            {
                pt0 = new Point(0, 0);
                clsArea.SetPoint(pt0);
                pt0.X = m_dFontSize;
                pt0.Y = m_dFontSize;
                clsArea.SetPoint(pt0);
                clsArea.sizex = m_dFontSize;
                clsArea.sizey = m_dFontSize;
            }
            else
            {
                clsArea.sizex = clsArea.ex + clsArea.sx;
                clsArea.sizey = clsArea.ey + clsArea.sy;
            }
            return (clsArea);
        }
        public ClsMatrix SetMatrixYokoElementSet(ClsFontGlyph clsFG)
        {
            string str;
            double fontsize;
            ClsMatrix clsMatrix;

            str = clsFG.str;
            fontsize = clsFG.fontsize;
            clsMatrix = new ClsMatrix();
            clsMatrix.xsize = fontsize;
            clsMatrix.ysize = fontsize;
            if (str == "‘"
            || str == "“" || str == "（"
            || str == "〔" || str == "［"
            || str == "｛" || str == "〈"
            || str == "《" || str == "「"
            || str == "『" || str == "【")
            {
                clsMatrix.a13 = fontsize * (-0.4);
                clsMatrix.a23 = fontsize * 0.86;
            }
            else if (str == "’"
            || str == "”" || str == "）"
            || str == "〕" || str == "］"
            || str == "｝" || str == "〉"
            || str == "》" || str == "」"
            || str == "』" || str == "】")
            {
                clsMatrix.a13 = fontsize * 0.15;
                clsMatrix.a23 = fontsize * 0.86;
            }
            else
            {
                clsMatrix.a23 = fontsize * 0.86;
            }
            return (clsMatrix);
        }
        public ClsMatrix SetMatrixTateElementSet(ClsFontGlyph clsFG)
        {
            string str;
            double fontsize;
            ClsMatrix clsMatrix;

            str = clsFG.str;
            fontsize = clsFG.fontsize;
            clsMatrix = new ClsMatrix();
            clsMatrix.xsize = fontsize;
            clsMatrix.ysize = fontsize;
            if (str == "ー" || str == "～" || str == "…" || str == "‥")
            {
                clsMatrix.a11 = 0;
                clsMatrix.a12 = -1;
                clsMatrix.a21 = 1;
                clsMatrix.a22 = 0;
                clsMatrix.a23 = 0;
                clsMatrix.a13 = fontsize * 0.125;
                clsMatrix.a23 = fontsize * (0.86 + 0.15);
            }
            else if (str == "‘"
            || str == "“" || str == "（"
            || str == "〔" || str == "［"
            || str == "｛" || str == "〈"
            || str == "《" || str == "「"
            || str == "『" || str == "【")
            {
                clsMatrix.a11 = 0;
                clsMatrix.a12 = -1;
                clsMatrix.a21 = 1;
                clsMatrix.a22 = 0;
                clsMatrix.a23 = 0;
                clsMatrix.a13 = fontsize * 0.125;
                clsMatrix.a23 = fontsize * (0.88 - 0.28);
            }
            else if (str == "’"
            || str == "”" || str == "）"
            || str == "〕" || str == "］"
            || str == "｝" || str == "〉"
            || str == "》" || str == "」"
            || str == "』" || str == "】")
            {
                clsMatrix.a11 = 0;
                clsMatrix.a12 = -1;
                clsMatrix.a21 = 1;
                clsMatrix.a22 = 0;
                clsMatrix.a23 = 0;
                clsMatrix.a13 = fontsize * 0.125;
                clsMatrix.a23 = fontsize * (0.88 + 0.28);
            }
            else if (str == "、" || str == "。" || str == "，" || str == "．")
            {
                clsMatrix.a13 = fontsize * 0.6;
                clsMatrix.a23 = fontsize * (0.88 - 0.58);
            }
            else if (str == "ぁ" || str == "ぃ" || str == "ぅ" || str == "ぇ" || str == "ぉ"
            || str == "っ" || str == "ゃ" || str == "ゅ" || str == "ょ" || str == "ゎ"
            || str == "ァ" || str == "ィ" || str == "ゥ" || str == "ェ" || str == "ォ"
            || str == "ォ" || str == "ッ" || str == "ャ" || str == "ュ" || str == "ョ"
            || str == "ヮ" || str == "ヵ" || str == "ヶ")
            {
                clsMatrix.a13 = fontsize * 0.1;
                clsMatrix.a23 = fontsize * (0.88 - 0.08);
            }
            else
            {
                clsMatrix.a23 = fontsize * 0.88;
            }
            return (clsMatrix);
        }
    }
}
