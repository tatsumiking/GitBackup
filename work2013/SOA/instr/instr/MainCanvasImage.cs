using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace instr
{
    public partial class MainWindow : Window
    {
        private double m_dTime;
        private double m_dAddX, m_dAddY;

        private void drawMainCanvasImage(string sWakuName)
        {
            double dXBase, dYBase;
            string sWakuPath;
            string sWakuFileName;
            string sEnvFileName;
            int len;
            string[] aryLine;
            string[] ary;
            int max, idx;
            double sx, sy, ex, ey;
            double wd, hi;
            string sFontName;
            string sBindString;
            DataRow rowItem;
            string sSetString;

            canvasImage.Children.Clear();
            dXBase = m_dXSize;
            dYBase = m_dYSize;
            sWakuPath = m_sWakuBasePath + "\\" + lblFolderName.Content.ToString();
            sWakuFileName = sWakuPath + "\\" + sWakuName;
            len = sWakuFileName.Length;
            sEnvFileName = sWakuFileName.Substring(0, len - 3) + "env";
            aryLine = m_libCmn.LoadFileLineSJIS(sEnvFileName);
            if (11 <= aryLine.Length)
            {
                ary = aryLine[10].Split(' ');
                dXBase = m_libCmn.StrToDouble(ary[0]);
                dYBase = m_libCmn.StrToDouble(ary[1]);
            }
            calcBaseTimeOffset(dXBase, dYBase);
            aryLine = m_libCmn.LoadFileLineSJIS(sWakuFileName);
            max = aryLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                ary = aryLine[idx].Split(',');
                if (14 <= ary.Length)
                {
                    sBindString = getWakuBindingString(idx);
                    rowItem = m_dt.Rows[m_nNafudaSlctLine];
                    sSetString = rowItem[sBindString].ToString();
                    sFontName = m_libCmn.DeleteDoubleQuotation(ary[2]);
                    sx = m_libCmn.StrToDouble(ary[3]);
                    sy = m_libCmn.StrToDouble(ary[4]);
                    wd = m_libCmn.StrToDouble(ary[5]);
                    hi = m_libCmn.StrToDouble(ary[6]);
                    ex = sx + wd;
                    ey = sy + hi;

                    sx = sx * m_dTime + m_dAddX;
                    sy = sy * m_dTime + m_dAddY;
                    ex = ex * m_dTime + m_dAddX;
                    ey = ey * m_dTime + m_dAddY;
                    drawBoxs(sx, sy, ex, ey);
                    drawWakuString(sx, sy, ex, ey, sFontName, sSetString);
                }
            }
        }
        private void calcBaseTimeOffset(double dXBase, double dYBase)
        {
            double dXTime, dYTime;

            dXTime = canvasImage.ActualWidth / dXBase;
            dYTime = canvasImage.ActualHeight / dYBase;
            if (dXTime < dYTime)
            {
                m_dTime = dXTime;
                m_dAddX = 0;
                m_dAddY = (canvasImage.ActualHeight - dYBase * m_dTime) / 2;
            }
            else
            {
                m_dTime = dYTime;
                m_dAddX = (canvasImage.ActualWidth - dXBase * m_dTime) / 2;
                m_dAddY = 0;
            }
        }
        private void drawBoxs(double sx, double sy, double ex, double ey)
        {
            m_libCanvas.drawLine(canvasImage, sx, sy, sx, ey);
            m_libCanvas.drawLine(canvasImage, sx, ey, ex, ey);
            m_libCanvas.drawLine(canvasImage, ex, ey, ex, sy);
            m_libCanvas.drawLine(canvasImage, ex, sy, sx, sy);
        }
        private void drawWakuString(double sx, double sy, double ex, double ey, string sFont, string sStr)
        {
            double fsize, space;
            double width, height, time;
            double dXAdd, dYAdd;
            double dXTime, dYTime;
            double dSX, dSY;
            int max, idx;
            string sOneCh;
            byte[] bytesSJIS;
            ObjVectList objVectList;

            dSX = sx;
            dSY = sy;
            dXAdd = 0.0;
            dYAdd = 0.0;
            max = sStr.Length;
            if (ex - sx < ey - sy) // 縦書き
            {
                fsize = ex - sx;
                if (max == 1)
                {
                    space = ((ey - sy) - fsize) / 2.0;
                    time = 1.0;
                    dSY = sy + space;
                }else{
                    height = fsize * max;
                    if (height < (ey - sy))
                    {
                        space = ((ey - sy) - height) / (max - 1);
                        time = 1.0;
                        dYAdd = fsize+space;
                    }
                    else
                    {
                        time = (ey - sy) / height;
                        dYAdd = fsize * time;
                    }
                }
            }
            else
            {
                fsize = ey - sy;
                if (max == 1)
                {
                    space = ((ex - sx) - fsize) / 2.0;
                    time = 1.0;
                    dSX = sx + space;
                }
                else
                {
                    width = fsize * max;
                    if (width < (ex - sx))
                    {
                        space = ((ex - sx) - width) / (max - 1);
                        time = 1.0;
                        dXAdd = fsize+space;
                    }
                    else
                    {
                        time = (ex - sx) / width;
                        dXAdd = fsize*time;
                    }
                }
            }
            if (sFont == "太楷書" || sFont == "Ｅ太角Ｇ")
            {
                dXTime = fsize / 1024.0;
                dYTime = fsize / 1024.0 * time; 
            }else{
                dXTime = fsize / 1024.0;
                dYTime = fsize / 1024.0 * time; 
            }
            for (idx = 0; idx < max; idx++)
            {
                sOneCh = sStr.Substring(idx, 1);
                bytesSJIS = System.Text.Encoding.Default.GetBytes(sOneCh);
                if (bytesSJIS.Length == 1)
                {
                    sOneCh = Microsoft.VisualBasic.Strings.StrConv(
                        sOneCh, Microsoft.VisualBasic.VbStrConv.Wide, 0x411);
                }
                if (sFont == "太楷書" || sFont == "Ｅ太角Ｇ")
                {
                    objVectList = m_libEsFont.ConvMojiToVectList(sOneCh, sFont);
                }
                else
                {
                    objVectList = m_libWinFont.ConvMojiToVectList(sOneCh, sFont);
                }
                if (objVectList != null)
                {
                    objVectList.SetScrnEnvClass(m_scrnEnv);
                    objVectList.SetLibCmnClass(m_libCmn);
                    objVectList.TrnsSize(0.0, 0.0, dXTime, dYTime);
                    objVectList.TrnsMove(dSX, dSY);
                    objVectList.UpdateDrawElement();
                    objVectList.AddChildrenCanvas(canvasImage);
                }
                dSX = dSX + dXAdd;
                dSY = dSY + dYAdd;
            }
        }
    }
}
