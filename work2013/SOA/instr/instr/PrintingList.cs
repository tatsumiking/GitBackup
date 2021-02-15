using System;
using System.Collections.Generic;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Printing;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace instr
{
    public class AnalyzeFile
    {
        private MainWindow m_wnd;
        public string m_sExecPath;
        public LibCommon m_libCmn;
        public LibCanvas m_libCanvas;
        private Canvas m_Canvas;
        private double m_dBaseX, m_dBaseY;
        private double m_dTime;
        private int m_nTate;
        private double m_dFS;
        private string[] m_sLine;

        public void SetMainWindowElement(MainWindow wnd, string sData)
        {
            string[] ary;
            int max, idx;

            m_wnd = wnd;
            m_sExecPath = m_wnd.m_sExecPath;
            m_libCmn = m_wnd.m_libCmn;
            m_libCanvas = m_wnd.m_libCanvas;

            InitCommandString();

            ary = sDenpyou.Split(',');
            m_sName = ary[1];
            m_sSouke = ary[3];
            m_sTuya = ary[4];
            m_sKokubetu = ary[5];
            m_sSekou = ary[6];
            m_nSumPrice = m_libCmn.StrToInt(ary[7]);

            m_list = new List<hanbai>();
            max = lstHanbai.Count;
            for (idx = 0; idx < max; idx++)
            {
                hanbai = new hanbai();
                ary = lstHanbai[idx].Split(',');
                hanbai.name = ary[2];
                hanbai.naiyou = ary[3];
                hanbai.tanka = m_libCmn.StrToInt(ary[4]);
                hanbai.kosu = m_libCmn.StrToInt(ary[5]);
                hanbai.price = m_libCmn.StrToInt(ary[6]);
                hanbai.bikou = ary[7];
                m_list.Add(hanbai);
            }
        }
        public void InitCommandString()
        {
            string sFileName;
            string[] aryLine;

            sFileName = m_sExecPath + "\\kden\\element.txt";
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            m_sKaisya1 = aryLine[1];
            m_sKaisya2 = aryLine[2];
            m_sPostNo = aryLine[3];
            m_sAddress = aryLine[4];
            m_sTel = aryLine[5];
            m_sFax = aryLine[6];

            sFileName = m_sExecPath + "\\kden\\elebank.txt";
            m_sBank = m_libCmn.LoadFileLineSJIS(sFileName);

            sFileName = m_sExecPath + "\\kden\\elenouhin.txt";
            m_sNouhin = m_libCmn.LoadFileLineSJIS(sFileName);

            sFileName = m_sExecPath + "\\kden\\eleseikyu.txt";
            m_sSeikyu = m_libCmn.LoadFileLineSJIS(sFileName);

        }
        public void SettingCanvas(string sFileName, Canvas canvas)
        {
            double timex, timey;
            string[] aryLine;
            int max, idx;

            m_Canvas = canvas;
            timex = m_Canvas.Width / 210.0;
            timey = m_Canvas.Height / 297.0;
            if (timex < timey)
            {
                m_dTime = timex;
            }
            else
            {
                m_dTime = timey;
            }
            m_dBaseX = 0;
            m_dBaseY = 0;

            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);

            m_libCanvas.setFontSize(24.0);
            m_libCanvas.setFontName("ＭＳ ゴシック");
            m_libCanvas.setFontNomal();
            m_libCanvas.setBrushFill(-1,-1,-1);
            m_libCanvas.setBrushStroke(0,0,0,1,0);
            m_libCanvas.setBrushText(0,0,0);

            max = aryLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                analysisLine(aryLine[idx]);
            }
        }
        private void analysisLine(string sLine)
        {
            string[] aryColumn;
            double sx, sy, ex, ey;
            double wd, hi;
            int r, g, b;
            int nTate;
            double thick;
            string  sFileName;


            if (sLine == "" || sLine.Substring(0,2) == "//")
            {
                return;
            }
            aryColumn = sLine.Split(',');
            if (aryColumn[0] == "BASE")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                setBase(sx, sy);
            }
            else if (aryColumn[0] == "FONT")
            {
                hi = m_libCmn.StrToDouble(aryColumn[1]);
                wd = m_libCmn.StrToDouble(aryColumn[2]);
                if (4 < aryColumn.Length)
                {
                    nTate = m_libCmn.StrToInt(aryColumn[4]);
                }
                else
                {
                    nTate = 0;
                }
                setFont(hi, wd, aryColumn[3], nTate);
            }
            else if (aryColumn[0] == "BFONT")
            {
                hi = m_libCmn.StrToDouble(aryColumn[1]);
                wd = m_libCmn.StrToDouble(aryColumn[2]);
                if (4 < aryColumn.Length)
                {
                    nTate = m_libCmn.StrToInt(aryColumn[4]);
                }
                else
                {
                    nTate = 0;
                }
                setBoldFont(hi, wd, aryColumn[3], nTate);
            }
            else if (aryColumn[0] == "BRUSH")
            {
                r = m_libCmn.StrToInt(aryColumn[1]);
                g = m_libCmn.StrToInt(aryColumn[2]);
                b = m_libCmn.StrToInt(aryColumn[3]);
                setBrush(r, g, b);
            }
            else if (aryColumn[0] == "CPEN")
            {
                r = m_libCmn.StrToInt(aryColumn[1]);
                g = m_libCmn.StrToInt(aryColumn[2]);
                b = m_libCmn.StrToInt(aryColumn[3]);
                thick = m_libCmn.StrToDouble(aryColumn[4]);
                setCPen(r, g, b, thick, 0);
            }
            else if (aryColumn[0] == "CTEXT")
            {
                r = m_libCmn.StrToInt(aryColumn[1]);
                g = m_libCmn.StrToInt(aryColumn[2]);
                b = m_libCmn.StrToInt(aryColumn[3]);
                setTextColor(r, g, b);
            }
            else if (aryColumn[0] == "LOGO")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                ex = m_libCmn.StrToDouble(aryColumn[3]);
                ey = m_libCmn.StrToDouble(aryColumn[4]);
                if (aryColumn.Length == 5)
                {
                    sFileName = "";
                }
                else
                {
                    sFileName = aryColumn[5];
                }
                setLogoDraw(sx, sy, ex, ey, sFileName);
                //setBoxs(sx, sy, ex, ey);
            }
            else if (aryColumn[0] == "STRG")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                setStrg(sx, sy, aryColumn[3]);
                //setBoxs(sx, sy, sx + m_dFSOrg, sy + m_dFSOrg);
            }
            else if (aryColumn[0] == "RSTR")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                setRStrg(sx, sy, aryColumn[3]);
                //setBoxs(sx, sy, sx + m_dFSOrg, sy + m_dFSOrg);
            }
            else if (aryColumn[0] == "CSTR")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                setCStrg(sx, sy, aryColumn[3]);
                //setBoxs(sx, sy, sx + m_dFSOrg, sy + m_dFSOrg);
            }
            else if (aryColumn[0] == "SBOX")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                ex = m_libCmn.StrToDouble(aryColumn[3]);
                ey = m_libCmn.StrToDouble(aryColumn[4]);
                setBoxStrg(sx, sy, ex, ey, aryColumn[5]);
                //setBoxs(sx, sy, ex, ey);
            }
            else if (aryColumn[0] == "RSBX")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                ex = m_libCmn.StrToDouble(aryColumn[3]);
                ey = m_libCmn.StrToDouble(aryColumn[4]);
                setRBoxStrg(sx, sy, ex, ey, aryColumn[5]);
                //setBoxs(sx, sy, ex, ey);
            }
            else if (aryColumn[0] == "CSBX")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                ex = m_libCmn.StrToDouble(aryColumn[3]);
                ey = m_libCmn.StrToDouble(aryColumn[4]);
                setCBoxStrg(sx, sy, ex, ey, aryColumn[5]);
                //setBoxs(sx, sy, ex, ey);
            }
            else if (aryColumn[0] == "ESBX")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                ex = m_libCmn.StrToDouble(aryColumn[3]);
                ey = m_libCmn.StrToDouble(aryColumn[4]);
                setEBoxStrg(sx, sy, ex, ey, aryColumn[5]);
                //setBoxs(sx, sy, ex, ey);
            }
            else if (aryColumn[0] == "LINE")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                ex = m_libCmn.StrToDouble(aryColumn[3]);
                ey = m_libCmn.StrToDouble(aryColumn[4]);
                setLine(sx, sy, ex, ey);
            }
            else if (aryColumn[0] == "BOXS")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                ex = m_libCmn.StrToDouble(aryColumn[3]);
                ey = m_libCmn.StrToDouble(aryColumn[4]);
                setBoxs(sx, sy, ex, ey);
            }
            else if (aryColumn[0] == "SEAL")
            {
                sx = m_libCmn.StrToDouble(aryColumn[1]);
                sy = m_libCmn.StrToDouble(aryColumn[2]);
                ex = m_libCmn.StrToDouble(aryColumn[3]);
                ey = m_libCmn.StrToDouble(aryColumn[4]);
                setSeal(sx, sy, ex, ey, aryColumn[5]);
            }
        }
        private void setBase(double sx, double sy)
        {
            m_dBaseX = sx * m_dTime;
            m_dBaseY = sy * m_dTime;
        }
        private void setFont(double hi, double wd, string sFontName, int nTate) 
        {
            m_nTate = nTate;
            m_libCanvas.setFontName(sFontName);
            m_libCanvas.setFontNomal();
            if (m_nTate == 0)
            {
                m_dFSOrg = hi;
                hi = hi * m_dTime;
                m_libCanvas.setFontSize(hi);
                m_dFS = hi;
            }
            else
            {
                m_dFSOrg = wd;
                wd = wd * m_dTime;
                m_libCanvas.setFontSize(wd);
                m_dFS = wd;
            }
        }
        private void setBoldFont(double hi, double wd, string sFontName, int nTate)
        {
            m_nTate = nTate;
            m_libCanvas.setFontName(sFontName);
            m_libCanvas.setFontBold();
            if (m_nTate == 0)
            {
                m_dFSOrg = hi;
                hi = hi * m_dTime;
                m_libCanvas.setFontSize(hi);
                m_dFS = hi;
            }
            else
            {
                m_dFSOrg = wd;
                wd = wd * m_dTime;
                m_libCanvas.setFontSize(wd);
                m_dFS = wd;
            }
        }
        private void setBrush(int r, int g, int b)
        {
            m_libCanvas.setBrushFill(r, g, b);
        }
        private void setCPen(int r, int g, int b, double thick, int type)
        {
            thick = thick * m_dTime;
            m_libCanvas.setBrushStroke(r, g, b, thick, type);
        }
        private void setTextColor(int r, int g, int b)
        {
            m_libCanvas.setBrushText(r, g, b);
        }
        private void setLogoDraw(double sx, double sy, double ex, double ey, string sFileName)
        {
            double wd, hi;

            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            ex = ex * m_dTime + m_dBaseX;
            ey = ey * m_dTime + m_dBaseY;
            if (sFileName == "")
            {
                sFileName = m_sExecPath + "\\kden\\logo.jpg";
            }
            else if (sFileName.Substring(1, 1) != ":")
            {
                sFileName = m_sExecPath + "\\kden\\" + sFileName;
            }
            wd = ex - sx;
            hi = ey - sy;
            m_libCanvas.drawImage(m_Canvas, sx, sy, wd, hi, sFileName);
        }
        private void setStrg(double sx, double sy, string sStr)
        {
            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            sStr = analyzeString(sStr);
            if (m_nTate == 0)
            {
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
            else
            {
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
        }
        private void setRStrg(double sx, double sy, string sStr)
        {
            double len;
            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            sStr = analyzeString(sStr);
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                sx = sx - len;
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
            else
            {
                sy = sy - len;
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
        }
        private void setCStrg(double sx, double sy, string sStr)
        {
            double len;
            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            sStr = analyzeString(sStr);
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                sx = sx - len / 2;
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
            else
            {
                sy = sy - len / 2;
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, 1.0, sStr);
            }
        }
        private void setBoxStrg(double sx, double sy, double ex, double ey,string sStr)
        {
            double len, time;
            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            ex = ex * m_dTime + m_dBaseX;
            ey = ey * m_dTime + m_dBaseY;
            sStr = analyzeString(sStr);
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                if(len < (ex-sx))
                {
                    time = 1.0;
                }else{
                    time = (ex - sx) / len;
                }
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
            else
            {
                if (len < (ey - sy))
                {
                    time = 1.0;
                }
                else
                {
                    time = (ey - sy) / len;
                }
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
        }
        private void setRBoxStrg(double sx, double sy, double ex, double ey, string sStr)
        {
            double len, time;
            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            ex = ex * m_dTime + m_dBaseX;
            ey = ey * m_dTime + m_dBaseY;
            sStr = analyzeString(sStr);
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                if (len < (ex - sx))
                {
                    time = 1.0;
                    sx = ex - len;
                }
                else
                {
                    time = (ex - sx) / len;
                }
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
            else
            {
                if (len < (ey - sy))
                {
                    time = 1.0;
                    sy = ey - len;
                }
                else
                {
                    time = (ey - sy) / len;
                }
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
        }
        private void setCBoxStrg(double sx, double sy, double ex, double ey, string sStr)
        {
            double len, time;
            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            ex = ex * m_dTime + m_dBaseX;
            ey = ey * m_dTime + m_dBaseY;
            sStr = analyzeString(sStr);
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                if (len < (ex - sx))
                {
                    time = 1.0;
                    sx = ex - len/2;
                }
                else
                {
                    time = (ex - sx) / len;
                }
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
            else
            {
                if (len < (ey - sy))
                {
                    time = 1.0;
                    sy = ey - len/2;
                }
                else
                {
                    time = (ey - sy) / len;
                }
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
        }
        private void setEBoxStrg(double sx, double sy, double ex, double ey, string sStr)
        {
            double len, space, time;
            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            ex = ex * m_dTime + m_dBaseX;
            ey = ey * m_dTime + m_dBaseY;
            sStr = analyzeString(sStr);
            len = sStr.Length * m_dFS;
            if (m_nTate == 0)
            {
                if (len < (ex - sx))
                {
                    time = 1.0;
                    space = (ex - sx) - len / (sStr.Length-1);
                }
                else
                {
                    time = (ex - sx) / len;
                }
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
            else
            {
                if (len < (ey - sy))
                {
                    time = 1.0;
                    space = (ey - sy) - len / (sStr.Length - 1);
                }
                else
                {
                    time = (ey - sy) / len;
                }
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, time, sStr);
            }
        }
        private void setLine(double sx, double sy, double ex, double ey)
        {
            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            ex = ex * m_dTime + m_dBaseX;
            ey = ey * m_dTime + m_dBaseY;
            m_libCanvas.drawLine(m_Canvas, sx, sy, ex, ey);
        }
        private void setBoxs(double sx, double sy, double ex, double ey)
        {
            double wd, hi;

            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            ex = ex * m_dTime + m_dBaseX;
            ey = ey * m_dTime + m_dBaseY;
            wd = ex - sx;
            hi = ey - sy;
            m_libCanvas.drawBoxs(m_Canvas, sx, sy, wd, hi);
        }
        private void setSeal(double sx, double sy, double ex, double ey, string sStr)
        {
            double sub;
            double wd, hi;

            sx = sx * m_dTime + m_dBaseX;
            sy = sy * m_dTime + m_dBaseY;
            ex = ex * m_dTime + m_dBaseX;
            ey = ey * m_dTime + m_dBaseY;
            sStr = analyzeString(sStr);
            sub = (ex - sx) * 0.05;
            wd = ex - sx;
            hi = ey - sy;
            m_libCanvas.drawEllipse(m_Canvas, sx, sy, wd, hi);
            sx = sx + sub;
            sy = sy + sub;
            ex = ex - sub;
            ey = ey - sub;
            m_libCanvas.setFontSize(ex-sx);
            if (m_nTate == 0)
            {
                m_libCanvas.drawYokoStrg(m_Canvas, sx, sy, 0.0, 0.5, sStr);
            }
            else
            {
                m_libCanvas.drawTateStrg(m_Canvas, sx, sy, 0.0, 0.5, sStr);
            }
        }
        private string analyzeString(string sSrcStr)
        {
            string[] ary;
            string sDstStr;
            int max, idx;
            string sStr;

            ary = sSrcStr.Split('%');
            sDstStr = "";
            max = ary.Length;
            for (idx = 0; idx < max; idx++)
            {
                if((idx % 2) == 1){
                    sStr = analyzeCmdStr(ary[idx]);
                    sDstStr = sDstStr + sStr;
                }else{
                    sDstStr = sDstStr + ary[idx];
                }
            }
            return (sDstStr);
        }
        private string analyzeCmdStr(string sCmdStr)
        {
            string sDstStr;
            string sStr;
            string mm, dd;
            int idx;
            int num;

            sDstStr = "";
            if (sCmdStr == "PRICE")
            {
                if (m_nSumPrice == 0)
                {
                    sDstStr = "";
                }
                else
                {
                    sStr = m_libCmn.AddPriceKanma(m_nSumPrice);
                    sDstStr = m_libCmn.HanToZen(sStr); ;
                }
            }
            else if (sCmdStr == "CRTTIME")
            {
                DateTime dt = DateTime.Now;
                sStr = m_libCmn.AnlizeNengou(dt.ToString("yyyy"));
                mm = dt.ToString("MM");
                dd = dt.ToString("dd");
                sDstStr = sStr + "年" + mm + "月" + dd + "日";
                sDstStr = m_libCmn.HanToZen(sDstStr);
            }
            else if (sCmdStr == "NAME")
            {
                sDstStr = m_sName;
            }
            else if (sCmdStr == "SOUKE")
            {
                sDstStr = m_sSouke;
            }
            else if (sCmdStr == "TUYA")
            {
                sStr = m_libCmn.AnlizeNengou(m_sTuya.Substring(0,4));
                mm = m_sTuya.Substring(5,2);
                dd = m_sTuya.Substring(8,2);
                sDstStr = sStr+"年"+mm+"月"+dd+"日";
                sDstStr = m_libCmn.HanToZen(sDstStr);
            }
            else if (sCmdStr == "KOKUBETU")
            {
                sStr = m_libCmn.AnlizeNengou(m_sKokubetu.Substring(0,4));
                mm = m_sKokubetu.Substring(5,2);
                dd = m_sKokubetu.Substring(8,2);
                sDstStr = sStr+"年"+mm+"月"+dd+"日";
                sDstStr = m_libCmn.HanToZen(sDstStr);
            }
            else if (sCmdStr == "SEKOU")
            {
                sDstStr = m_sSekou;
            }
            else if (sCmdStr == "KAISYA1")
            {
                sDstStr = m_sKaisya1;
            }
            else if (sCmdStr == "KAISYA2")
            {
                sDstStr = m_sKaisya2;
            }
            else if (sCmdStr == "POSTNO")
            {
                sDstStr = m_sPostNo;
            }
            else if (sCmdStr == "ADDRESS")
            {
                sDstStr = m_sAddress;
            }
            else if (sCmdStr == "TEL")
            {
                sDstStr = m_sTel;
            }
            else if (sCmdStr == "FAX")
            {
                sDstStr = m_sFax;
            }
            else if (sCmdStr.Substring(0,6) == "BSBANK")
            {
                idx = m_libCmn.StrToInt(sCmdStr.Substring(6));
                sDstStr = m_sBank[idx];
            }
            else if (sCmdStr.Substring(0,6) == "NOUHIN")
            {
                idx = m_libCmn.StrToInt(sCmdStr.Substring(6));
                sDstStr = m_sNouhin[idx];
            }
            else if (sCmdStr.Substring(0,6) == "SEIKYU")
            {
                idx = m_libCmn.StrToInt(sCmdStr.Substring(6));
                sDstStr = m_sSeikyu[idx];
            }
            else if (sCmdStr.Substring(0, 6) == "HBNAME")
            {
                idx = m_libCmn.StrToInt(sCmdStr.Substring(6)) - 1;
                if (idx < m_list.Count)
                {
                    sDstStr = m_list[idx].name;
                }
            }
            else if (sCmdStr.Substring(0, 6) == "HNAIYU")
            {
                idx = m_libCmn.StrToInt(sCmdStr.Substring(6)) - 1;
                if (idx < m_list.Count)
                {
                    sDstStr = m_list[idx].naiyou;
                }
            }
            else if (sCmdStr.Substring(0, 6) == "HTANKA")
            {
                idx = m_libCmn.StrToInt(sCmdStr.Substring(6)) - 1;
                if (idx < m_list.Count)
                {
                    num = m_list[idx].tanka;
                    sDstStr = String.Format("{0:#,0}", num);
                    sDstStr = m_libCmn.HanToZen(sDstStr);
                }
            }
            else if (sCmdStr.Substring(0, 6) == "HBKOSU")
            {
                idx = m_libCmn.StrToInt(sCmdStr.Substring(6)) - 1;
                if (idx < m_list.Count)
                {
                    num = m_list[idx].kosu;
                    sDstStr = String.Format("{0:#,0}", num);
                    sDstStr = m_libCmn.HanToZen(sDstStr);
                }
            }
            else if (sCmdStr.Substring(0, 6) == "HPRICE")
            {
                idx = m_libCmn.StrToInt(sCmdStr.Substring(6)) - 1;
                if (idx < m_list.Count)
                {
                    num = m_list[idx].price;
                    sDstStr = String.Format("{0:#,0}", num);
                    sDstStr = m_libCmn.HanToZen(sDstStr);
                }
            }
            else if (sCmdStr.Substring(0, 6) == "HBIKOU")
            {
                idx = m_libCmn.StrToInt(sCmdStr.Substring(6)) - 1;
                if (idx < m_list.Count)
                {
                    sDstStr = m_list[idx].bikou;
                }
            }
            return (sDstStr);
        }
    }
}
