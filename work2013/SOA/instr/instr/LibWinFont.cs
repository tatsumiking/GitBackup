using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace instr
{
    public class LibWinFont
    {
        public string m_sExecPath;
        public string m_sWinFontPath;
        public LibCommon m_libCmn;
        public string m_strFontData;

        public void SetExecPath(string path)
        {
            m_sExecPath = path;
        }
        public void SetWinFontPath(string path)
        {
            m_sWinFontPath = path;
        }
        public void SetLibCmn(LibCommon libCmn)
        {
            m_libCmn = libCmn;
        }
        public ObjVectList ConvMojiToVectList(string str, string sFontName)
        {
            string sTTFontName;
            double dFontSize;
            ObjVectList objVectList;
            ObjVect objVect;
            ObjFigu objFigu;
            ClsFontPathGeometry clsFGP;
            PathGeometry pg;
            int psmax, psidx;
            PathSegment ps;
            int pfmax, pfidx;
            PathFigure pf;
            BezierSegment bz;
            PolyBezierSegment pbz;
            LineSegment ln;
            PolyLineSegment pln;
            int ptmax, ptidx;
            string stype;
            double x1, y1;

            dFontSize = 1024;
            sTTFontName = GetTTFontName(sFontName);
            clsFGP = m_libCmn.GetFontPathGeometry(str, dFontSize, sFontName);
            pfmax = clsFGP.pg.Figures.Count;
            objVectList = new ObjVectList();
            for (pfidx = 0; pfidx < pfmax; pfidx++)
            {
                objVect = new ObjVect();
                pf = clsFGP.pg.Figures[pfidx];
                x1 = pf.StartPoint.X;
                y1 = pf.StartPoint.Y + clsFGP.dMoveY;
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
                            objFigu = new ObjFigu();
                            objFigu.x1 = x1;
                            objFigu.y1 = y1;
                            objFigu.x2 = pbz.Points[ptidx].X;
                            objFigu.y2 = pbz.Points[ptidx].Y + clsFGP.dMoveY;
                            objFigu.x3 = pbz.Points[ptidx + 1].X;
                            objFigu.y3 = pbz.Points[ptidx + 1].Y + clsFGP.dMoveY;
                            objFigu.x4 = pbz.Points[ptidx + 2].X;
                            objFigu.y4 = pbz.Points[ptidx + 2].Y + clsFGP.dMoveY;
                            objFigu.kind = Constants.BEZEKIND;
                            objVect.m_lstObjFigu.Add(objFigu);
                            x1 = objFigu.x4;
                            y1 = objFigu.y4;
                        }
                    }
                    else if (stype.Equals("PolyLineSegment"))
                    {
                        pln = (PolyLineSegment)ps;
                        ptmax = pln.Points.Count;
                        for (ptidx = 0; ptidx < ptmax; ptidx++)
                        {
                            objFigu = new ObjFigu();
                            objFigu.x1 = x1;
                            objFigu.y1 = y1;
                            objFigu.x4 = pln.Points[ptidx].X;
                            objFigu.y4 = pln.Points[ptidx].Y + clsFGP.dMoveY;
                            objFigu.kind = Constants.LINEKIND;
                            objVect.m_lstObjFigu.Add(objFigu);
                            x1 = objFigu.x4;
                            y1 = objFigu.y4;
                        }
                    }
                    else if (stype.Equals("BezierSegment"))
                    {
                        objFigu = new ObjFigu();
                        bz = (BezierSegment)ps;
                        objFigu.x1 = x1;
                        objFigu.y1 = y1;
                        objFigu.x2 = bz.Point1.X;
                        objFigu.y2 = bz.Point1.Y + clsFGP.dMoveY;
                        objFigu.x3 = bz.Point2.X;
                        objFigu.y3 = bz.Point2.Y + clsFGP.dMoveY;
                        objFigu.x4 = bz.Point3.X;
                        objFigu.y4 = bz.Point3.Y + clsFGP.dMoveY;
                        objFigu.kind = Constants.BEZEKIND;
                        objVect.m_lstObjFigu.Add(objFigu);
                        x1 = objFigu.x4;
                        y1 = objFigu.y4;
                    }
                    else if (stype.Equals("LineSegment"))
                    {
                        objFigu = new ObjFigu();
                        ln = (LineSegment)ps;
                        objFigu.x1 = x1;
                        objFigu.y1 = y1;
                        objFigu.x4 = ln.Point.X;
                        objFigu.y4 = ln.Point.Y + clsFGP.dMoveY;
                        objFigu.kind = Constants.LINEKIND;
                        objVect.m_lstObjFigu.Add(objFigu);
                        x1 = objFigu.x4;
                        y1 = objFigu.y4;
                    }
                }
                objVectList.AddVect(objVect);
            }
            return (objVectList);
        }
        private string GetTTFontName(string sFontName)
        {
            string sFileName;
            string[] aryLine;
            int idx, max;
            byte[] bSrc, bDst;
            string sName, sTTName;
            string sTTFontName;

            bDst = new byte[40];

            sTTFontName = sFontName;
            sFileName = m_sExecPath + "\\fontlist.lst";
            aryLine = m_libCmn.LoadFileLineSJIS(sFileName);
            max = aryLine.Length;
            for (idx = 0; idx < max; idx++)
            {
                bSrc = Encoding.Default.GetBytes(aryLine[idx]);
                if (bSrc.Length < 64)
                {
                    break;
                }
                Buffer.BlockCopy(bSrc, 3, bDst, 0, 40);
                sName = System.Text.Encoding.GetEncoding("shift_jis").GetString(bDst);
                sName = m_libCmn.DeleteBackSpace(sName);
                if (sName == sFontName)
                {
                    Buffer.BlockCopy(bSrc, 64, bDst, 0, 40);
                    sTTName = System.Text.Encoding.GetEncoding("shift_jis").GetString(bDst);
                    sTTName = m_libCmn.DeleteBackSpace(sTTName);
                    sTTFontName = sTTName;
                    break;
                }
            }
            return (sTTFontName);
        }
        public ClsMatrix SetMatrixYokoElementSet(string str, double fontsize)
        {
            ClsMatrix clsMatrix;

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
        public ClsMatrix SetMatrixTateElementSet(string str, double fontsize)
        {
            ClsMatrix clsMatrix;

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
