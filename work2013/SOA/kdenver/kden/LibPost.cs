using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace kden
{
    public class clsPS
    {
        public int nNo;
        public int nIdx;
    }
    public class clsPost
    {
        public int nNo;
        public string sKen;
        public string sSiku;
        public string sTyou;
    }
    public class LibPost
    {
        public List<clsPost> m_lstPost;

        public void InitPostList(string sPath)
        {
            string[] sLine;
            string sFileName;
            int max, idx;
            string[] sAry;
            clsPost clsPost;

            sFileName = sPath + "\\postall.csv";
            m_lstPost = new List<clsPost>();
            sLine = LoadFileLineSJIS(sFileName);
            max = sLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                sAry = sLine[idx].Split(',');
                if (4 <= sAry.Length)
                {
                    clsPost = new clsPost();
                    clsPost.nNo = StrToInt(sAry[0]);
                    clsPost.sKen = sAry[1];
                    clsPost.sSiku = sAry[2];
                    clsPost.sTyou = sAry[3];
                    m_lstPost.Add(clsPost);
                }
            }
        }
        public string SearchPostAddress(int nPost, Window win, double dx, double dy)
        {
            int max, idx;
            string sAdrs;
            List<string> lstAdrs;

            sAdrs = "";
            lstAdrs = new List<string>();
            max = m_lstPost.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (nPost == m_lstPost[idx].nNo)
                {
                    sAdrs = m_lstPost[idx].sSiku + m_lstPost[idx].sTyou;
                    lstAdrs.Add(sAdrs);
                }
            }
            if (lstAdrs.Count == 1)
            {
                return (sAdrs);
            }
            PostAdrsWin PostAdrsWin = new PostAdrsWin();
            PostAdrsWin.setList(lstAdrs);
            PostAdrsWin.Left = dx;
            PostAdrsWin.Top = dy;
            PostAdrsWin.Owner = win;
            PostAdrsWin.ShowDialog();
            sAdrs = PostAdrsWin.m_sSlctAdrs;
            return (sAdrs);
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
    }
}
