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
using System.Xml;
using System.Xml.Serialization;

namespace sekiji
{
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
    public class LibCommon
    {
        private const double SEIDO = 1.0e-10;
        private double m_dFontSize;
        static Encoding sjisEnc = Encoding.GetEncoding("Shift_JIS");

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
        public string SubStringByte(string str, int count)
        {
            string retstr;
            int max, idx;
            int len, limit;

            limit = count * 2;
            len = 0;
            retstr = "";
            max = str.Length;
            for(idx = 0; idx < max; idx++){
                if(isHankaku(str.Substring(idx, 1)) == true){
                    len += 1;
                }
                else
                {
                    len += 2;
                }
                if (limit < len)
                {
                    max = idx-1;
                    break;
                }
            }
            retstr = str.Substring(0, max);
            return (retstr);
        }
        public static bool isZenkaku(string str)
        {
            int num = sjisEnc.GetByteCount(str);
            return num == str.Length * 2;
        }

        public static bool isHankaku(string str)
        {
            int num = sjisEnc.GetByteCount(str);
            return num == str.Length;
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
        public string DeleteBackm_dSpace(string sStr)
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
        public string setSamaNameSpaceing(string sName)
        {
            string[] aryStr;
            int len, len1, len2;

            aryStr = sName.Split('　');
            len = aryStr.Length;
            if (len == 1)
            {
                len = aryStr[0].Length;
                if (len == 1)
                {
                    sName = sName + "　　　　　様";
                }
                else if (len == 2)
                {
                    sName = sName + "　　　　様";
                }
                else if (len == 3)
                {
                    sName = sName + "　　　様";
                }
                else if (len == 4)
                {
                    sName = sName + "　　様";
                }
                else if (len == 5)
                {
                    sName = sName + "　様";
                }
                else // if (6 <= len)
                {
                    sName = sName + "様";
                }
            }
            else if(len == 2)
            {
                len1 = aryStr[0].Length;
                len2 = aryStr[1].Length;
                if(len1 == 1 && len2 == 1)
                {
                    sName = aryStr[0]+"　　　" + aryStr[1] + "　様";
                }
                else if (len1 == 1 && len2 == 2)
                {
                    sName = aryStr[0] + "　　" + aryStr[1] + "　様";
                }
                else if (len1 == 2 && len2 == 1)
                {
                    sName = aryStr[0] + "　　" + aryStr[1] + "　様";
                }
                else if (len1 == 2 && len2 == 2)
                {
                    sName = aryStr[0] + "　" + aryStr[1] + "　様";
                }
                else if (len1 == 1 && len2 == 3)
                {
                    sName = aryStr[0] + "　" + aryStr[1] + "　様";
                }
                else if (len1 == 3 && len2 == 1)
                {
                    sName = aryStr[0] + "　" + aryStr[1] + "　様";
                }
                else if (len1 == 2 && len2 == 3)
                {
                    sName = aryStr[0] + "" + aryStr[1] + "　様";
                }
                else if (len1 == 3 && len2 == 2)
                {
                    sName = aryStr[0] + "" + aryStr[1] + "　様";
                }
                else // if (len1 == 3 && len2 == 3)以上
                {
                    sName = aryStr[0] + "" + aryStr[1] + "様";
                }
            }
            else
            {
                len = sName.Length;
                if (len == 1)
                {
                    sName = sName + "　　　　　様";
                }
                else if (len == 2)
                {
                    sName = sName + "　　　　様";
                }
                else if (len == 3)
                {
                    sName = sName + "　　　様";
                }
                else if (len == 4)
                {
                    sName = sName + "　　様";
                }
                else if (len == 5)
                {
                    sName = sName + "　様";
                }
                else // if (6 <= len)
                {
                    sName = sName + "様";
                }

            }
            return(sName);
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
        public void DataXmlSave(string xmlfile, Type type, Object obj)
        {
            // ファイルパス(カレントディレトリ直下のText.xml固定)
            XmlTextWriter xtw = null;
            XmlSerializer xs = null;
            string err = "";

            // XMLファイル保存
            try
            {
                //XMLファイル書き込みインスタンス作成
                xtw = new XmlTextWriter(xmlfile, Encoding.UTF8);
                xtw.Formatting = Formatting.Indented;
                xtw.Indentation = 4;

                //XMLシリアライザクラスインスタンス作成
                xs = new XmlSerializer(type);
                //XMLファイルシリアライズ
                xs.Serialize(xtw, obj);
            }
            catch (Exception exp)
            {//例外発生
                err = exp.Message;
            }
            finally
            {//インスタンス破棄
                if (xtw != null)
                {
                    xtw.Close();
                }
            }
        }

        public Object DataXmlLoad(string xmlfile, Type type)
        {
            StreamReader sr = null;
            XmlSerializer xs = null;
            Object obj = null;
            String err = "";

            // XMLファイル読み込み(XmlSerializerによるデシリアライズ)
            try
            {
                //ファイル読み込みインスタンス作成
                sr = new StreamReader(xmlfile, Encoding.ASCII);
                //XMLシリアライザクラスインスタンス作成
                xs = new XmlSerializer(type);
                //XMLファイルデシリアライズ
                obj = (Object)xs.Deserialize(sr);
            }
            catch (Exception exp)
            {//例外発生
                err = exp.Message;
                return (null);
            }
            finally
            {//インスタンス破棄
                if (sr != null)
                {
                    sr.Close();
                    sr.Dispose();
                }
            }
            return (obj);
        }
    }
}
