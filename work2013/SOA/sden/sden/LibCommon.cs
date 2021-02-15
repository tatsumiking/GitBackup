using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Xml;
using System.Xml.Serialization;

namespace sden
{
    public partial class LibCommon
    {
        private sden.MainWindow m_Wnd;
        private const double SEIDO = 1.0e-10;

        public LibCommon()
        {
            m_Wnd = (sden.MainWindow)Application.Current.MainWindow;
        }
        public int StrToIntRetEr(string str)
        {
            int num;

            if (int.TryParse(str, out num))
            { }
            else
            {
                num = -1;
            }
            return (num);
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

        /// <summary>
        /// Doubleの引数のゼロ確認
        /// </summary>
        /// <param name="a"></param>
        /// <returns></returns>
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

        public string DoubleToPriceStr(double num)
        {
            string retStr;

            retStr = num.ToString();
            return (retStr);
        }
        public void DoDispatch()
        {
            DispatcherFrame frame = new DispatcherFrame();
            Dispatcher.CurrentDispatcher.BeginInvoke(DispatcherPriority.Background, new DispatcherOperationCallback(ExitFrames), frame);
            Dispatcher.PushFrame(frame);
        }
        public object ExitFrames(object frames)
        {
            ((DispatcherFrame)frames).Continue = false;
            return null;
        }
        public string[] LoadFileLineSJIS(string sFileName)
        {
            string strData;
            string[] aryLine;

            strData = LoadFileSJIS(sFileName);
            strData = strData.Replace("\r\n", ";");
            strData = strData.Replace("\n", ";");
            strData = strData.Replace("\r", ";");
            aryLine = strData.Split(';');
            return (aryLine);
        }
        public string[] LoadFileLineUTF8(string sFileName)
        {
            string strData;
            string[] aryLine;

            strData = LoadFileUTF8(sFileName);
            strData = strData.Replace("\r\n", ";");
            strData = strData.Replace("\n", ";");
            strData = strData.Replace("\r", ";");
            aryLine = strData.Split(';');
            return (aryLine);
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
        public Boolean SaveFileSJIS(string sFileName, string sData)
        {
            try
            {
                StreamWriter sw = new StreamWriter(sFileName, false, Encoding.GetEncoding("shift_jis"));
                sw.Write(sData);
                sw.Close();
                return (true);
            }
            catch (System.IO.IOException ex)
            {
                MessageBox.Show("ファイル保存に失敗しました。", "確認", MessageBoxButton.OK);
                return (false);
            }
        }
        public Boolean SaveFileUTF8(string sFileName, string sData)
        {
            try
            {
                StreamWriter sw = new StreamWriter(sFileName);
                sw.Write(sData);
                sw.Close();
                return (true);
            }
            catch (System.IO.IOException ex)
            {
                MessageBox.Show("ファイル転送に失敗しました。", "確認", MessageBoxButton.OK);
                return (false);
            }
        }
        public string LoadFile(string sFileName)
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

        public void SaveFile(string sFileName, string sData)
        {
            StreamWriter sw = new StreamWriter(sFileName, false, Encoding.GetEncoding("shift_jis"));
            sw.Write(sData);
            sw.Close();
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

        public Object DataUTF8XmlLoad(string xmlfile, Type type)
        {
            StreamReader sr = null;
            XmlSerializer xs = null;
            Object obj = null;
            String err = "";

            // XMLファイル読み込み(XmlSerializerによるデシリアライズ)
            try
            {
                //ファイル読み込みインスタンス作成
                sr = new StreamReader(xmlfile, Encoding.UTF8);
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

        public void DeleteFiles(string sDirName)
        {
            //コピー元のディレクトリにあるファイルをコピー(true 上書きコピー)
            string[] files = System.IO.Directory.GetFiles(sDirName);
            foreach (string file in files)
            {
                System.IO.File.Delete(file);
            }
        }

        public void DeleteXmlFiles(string sDirName)
        {
            String sExt;
            //コピー元のディレクトリにあるファイルをコピー(true 上書きコピー)
            string[] files = System.IO.Directory.GetFiles(sDirName);
            foreach (string file in files)
            {
                sExt = System.IO.Path.GetExtension(file);
                if (sExt == ".xml")
                {
                    System.IO.File.Delete(file);
                }
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
                    sDstDataPath + "\\" + System.IO.Path.GetFileName(files[i]), true);
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

        public string GetLastFileName(string sFolder, string sSrch)
        {
            string sLastFile;
            int max, i;

            sLastFile = "";
            string[] files = Directory.GetFiles(sFolder, sSrch);
            max = files.Length;
            for (i = 0; i < max; i++)
            {
                if (sLastFile.CompareTo(files[i]) < 0)
                {
                    sLastFile = files[i];
                }
            }
            return (sLastFile);
        }
    }

}
