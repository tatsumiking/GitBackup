using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace instr
{
    public class LibEsFont
    {
        public LibCommon m_libCmn;
        public LibCurve m_libCurve;
        public string m_sEsFontPath;
        public int m_nTateGaki;

        public LibEsFont()
        {
            m_nTateGaki = 1;
        }
        public void SetLibCmn(LibCommon libCmn)
        {
            m_libCmn = libCmn;
        }
        public void SetLibCurve(LibCurve libCurve)
        {
            m_libCurve = libCurve;
        }
        public void SetEsFontPath(string path)
        {
            m_sEsFontPath = path;
        }
        public ObjVectList ConvMojiToVectList(string sStr, String sFontName)
        {
            int sjis;
            byte[] bytesSJIS;
            String sTblFontFileName;
            String sDataFontFileName;
            int seekpoint;
            FileStream fsTbl;
            FileStream fsData;
            int nDataLen;
            long nSeekp;
            byte atr;
            int pp, pointmax;
            int sp, ep;
            ObjVectList objVectList;
            ObjVect objVect;

            bytesSJIS = System.Text.Encoding.Default.GetBytes(sStr);
            sjis = ((int)(bytesSJIS[0])) << 8 | bytesSJIS[1];
            byte[] byteIdx = new Byte[Constants.FIDXSIZE];
            sTblFontFileName = getTblFontFileName(sjis, sFontName);
            sDataFontFileName = getDataFontFileName(sjis, sFontName);
            seekpoint = getSeekPoint(sjis);
            if (seekpoint == 0)
            {
                return(null);
            }
            fsTbl = new FileStream(sTblFontFileName, FileMode.Open);
            fsTbl.Seek(seekpoint, SeekOrigin.Begin);
            fsTbl.Read(byteIdx, 0, Constants.FIDXSIZE);
            nDataLen = BitConverter.ToInt16(byteIdx, 0);
            nSeekp = BitConverter.ToInt32(byteIdx, 2);
            if (nDataLen == 0)
            {
                fsTbl.Close();
                return(null);
            }
            byte[] byteData = new Byte[nDataLen];
            fsData = new FileStream(sDataFontFileName, FileMode.Open);
            fsData.Seek(nSeekp, SeekOrigin.Begin);
            fsData.Read(byteData, 0, nDataLen);
            if (m_nTateGaki == 1)
            {
                byteData = tategakiTrns(sjis, byteData, nDataLen);
            }
            pointmax = nDataLen / Constants.ESF0SIZADD;
            objVectList = new ObjVectList();
            sp = 0;
            for (pp = 1; pp < pointmax; pp++)
            {
                atr = AtrGet(byteData, pp);
                if((atr & Constants.F0EORMASK) != 0){
                    ep = pp;
                    objVect = cnvRejionVect(byteData, sp, ep);
                    if (objVect != null)
                    {
                        objVectList.AddVect(objVect);
                    }
                    sp = ep + 1;
                }
            }
            fsTbl.Close();
            fsData.Close();
            return (objVectList);
        }
        private byte[] tategakiTrns(int sjis, byte[] byteData, int nDataLen)
        {
	        int		pcnt, pp;
            ClsAtrXY atrxy;
            double      xx, yy;
	        double		minx, miny, maxx, maxy;
	        double		movex, movey;

            pcnt = nDataLen / Constants.ESF0SIZADD;
	        if(sjis == 0x8141 || sjis == 0x8142 || sjis == 0x8143 || sjis == 0x8144){
                minx = Constants.ESF0MESH;
                miny = Constants.ESF0MESH;
		        maxx = 0;
		        maxy = 0;
		        for(pp = 0; pp < pcnt; pp++){
                    atrxy = FAtrDXYGet(byteData, pp);
                    if (maxx < atrxy.x)
                    {
                        maxx = atrxy.x;
			        }
                    if (minx > atrxy.x)
                    {
                        minx = atrxy.x;
			        }
                    if (maxy < atrxy.y)
                    {
                        maxy = atrxy.y;
			        }
                    if (miny > atrxy.y)
                    {
                        miny = atrxy.y;
			        }
		        }
                movex = Constants.ESF0MESH - maxx - minx;
                movey = Constants.ESF0MESH - maxy - miny;
		        for(pp = 0; pp < pcnt; pp++){
                    atrxy = FAtrDXYGet(byteData, pp);
                    atrxy.x = atrxy.x + movex;
                    atrxy.y = atrxy.y + movey;
                    byteData = AtrDXYSet(byteData, pp, atrxy);
		        }
	        }
	        else if(0x8150 == sjis || sjis == 0x8151
	             || 0x8163 == sjis || sjis == 0x8164
		         ||(0x8169 <= sjis && sjis <= 0x8186)){
                     for (pp = 0; pp < pcnt; pp++)
                     {
                         atrxy = FAtrDXYGet(byteData, pp);
                         xx = Constants.ESF0MESH - atrxy.y;
                         yy = atrxy.x;
                         atrxy.x = xx;
                         atrxy.y = yy;
                         byteData = AtrDXYSet(byteData, pp, atrxy);
                     }
		    }
	        else if(sjis == 0x815b || sjis == 0x815c
		         || sjis == 0x815d || sjis == 0x8160
		         || sjis == 0x8161 || sjis == 0x8162){
                     for (pp = 0; pp < pcnt; pp++)
                     {
                         atrxy = FAtrDXYGet(byteData, pp);
                         xx = atrxy.y;
                         yy = atrxy.x;
                         atrxy.x = xx;
                         atrxy.y = yy;
                         byteData = AtrDXYSet(byteData, pp, atrxy);
                     }
		    }
            return (byteData);
        }
        private ObjVect cnvRejionVect(byte[] byteData, int sp, int ep)
        {
            ObjVect objVect;
            int pp, p0, p1, p2, p3, p4; ;
            ClsAtrXY atrxy0;
            ClsAtrXY atrxy1;
            ClsAtrXY atrxy2;
            ClsAtrXY atrxy3;
            ClsAtrXY atrxy4;
            ObjFigu objFigu;

            if (sp == ep)
            {
                return(null);
            }
            if (sp+1 == ep)
            {
                return (null);
            }
            for (pp = sp; pp <= ep; pp++)
            {
                atrxy0 = AtrDXYGet(byteData, pp);
            }
            objVect = new ObjVect();
            for (pp = sp; pp <= ep; )
            {
                p0 = getLegalPoint(pp - 1, sp, ep);
                p1 = getLegalPoint(pp, sp, ep);
                p2 = getLegalPoint(pp + 1, sp, ep);
                p3 = getLegalPoint(pp + 2, sp, ep);
                p4 = getLegalPoint(pp + 3, sp, ep);
                atrxy0 = AtrDXYGet(byteData, p0);
                atrxy1 = AtrDXYGet(byteData, p1);
                atrxy2 = AtrDXYGet(byteData, p2);
                atrxy3 = AtrDXYGet(byteData, p3);
                atrxy4 = AtrDXYGet(byteData, p4);
                if (checkBeze(atrxy1.atr, atrxy2.atr, atrxy3.atr) == true)
                {
                    objFigu = cnvBezeToFigu(atrxy1, atrxy2, atrxy3, atrxy4);
                    objVect.m_lstObjFigu.Add(objFigu);
                    pp = pp + 3;
                }
                else if (checkArc(atrxy1.atr, atrxy2.atr, atrxy3.atr) == true)
                {
                    objFigu = cnvArcToFigu(atrxy1, atrxy2, atrxy3);
                    objVect.m_lstObjFigu.Add(objFigu);
                    pp = pp + 2;
                }
                else if (checkArc(atrxy1.atr) == true)
                {
                    objFigu = cnvLineToFigu(atrxy1, atrxy2);
                    objVect.m_lstObjFigu.Add(objFigu);
                    pp = pp + 1;
                    //objFigu = cnvCruveToFigu(atrxy0, atrxy1, atrxy2, atrxy3);
                    //objVect.m_lstObjFigu.Add(objFigu);
                    //pp = pp + 1;
                }
                else
                {
                    objFigu = cnvLineToFigu(atrxy1, atrxy2);
                    objVect.m_lstObjFigu.Add(objFigu);
                    pp = pp + 1;
                }
            }
            return (objVect);
        }
        private Boolean checkBeze(int atr1, int atr2, int atr3)
        {
            atr1 = atr1 & Constants.PATRMASK;
            atr2 = atr2 & Constants.PATRMASK;
            atr3 = atr3 & Constants.PATRMASK;
	        if(((atr1 == (Constants.CURVATR | Constants.BEZEATR))
             || (atr1 == (Constants.CONTATR | Constants.CURVATR | Constants.BEZEATR)))
	        && (atr2 == Constants.BEZEATR)
	        && (atr3 == Constants.BEZEATR)){
                return(true);
            }
            return(false);
        }
        private Boolean checkArc(int atr1, int atr2, int atr3)
        {
            atr1 = atr1 & Constants.PATRMASK;
            atr2 = atr2 & Constants.PATRMASK;
            atr3 = atr3 & Constants.PATRMASK;
	        if((atr1 == Constants.CURVATR)
	        && (atr2 == (Constants.CURVATR | Constants.CONTATR))
	        && ((atr3 & Constants.CONTATR) == 0)){
                return(true);
            }
            return(false);
        }
        private Boolean checkArc(int atr1)
        {
            atr1 = atr1 & Constants.PATRMASK;
	        if(atr1 == Constants.CURVATR){
                return(true);
            }
            return(false);
        }
        private int getLegalPoint(int pp, int sp, int ep)
        {
            if (pp < sp)
            {
                pp = (ep+1) + (pp-sp);
            }
            else if (ep < pp)
            {
                pp = pp - (ep+1) + sp;
            }
            return (pp);
        }
        private byte AtrGet(byte[] byteData, int pp)
        {
            int bytep;
            byte atr;
            bytep = pp * Constants.ESF0SIZADD;
            atr = byteData[bytep + Constants.ESF0ATRADD];
            return (atr);
        }
        private ClsAtrXY AtrDXYGet(byte[] byteData, int pp)
        {
            int bytep;
            int bitx, bity, ix, iy;
            ClsAtrXY atrxy;
            atrxy = new ClsAtrXY();
            bytep = pp * Constants.ESF0SIZADD;
            atrxy.atr = (byteData[bytep + Constants.ESF0ATRADD] & 0x0e);
            bitx = (byteData[bytep + Constants.ESF0ATRADD] & 0xc0) >> 6;
            bity = (byteData[bytep + Constants.ESF0ATRADD] & 0x30) >> 4;
            ix = byteData[bytep + Constants.ESF0XCDADD];
            iy = byteData[bytep + Constants.ESF0YCDADD];
            atrxy.x = (double)(((ix << 2) & 0x3fc) | (bitx & 0x003));
            atrxy.y = (double)(((iy << 2) & 0x3fc) | (bity & 0x003));
            return (atrxy);
        }
        private ClsAtrXY FAtrDXYGet(byte[] byteData, int pp)
        {
            int bytep;
            int bitx, bity, ix, iy;
            ClsAtrXY atrxy;
            atrxy = new ClsAtrXY();
            bytep = pp * Constants.ESF0SIZADD;
            atrxy.atr = (byteData[bytep + Constants.ESF0ATRADD] & 0x0f);
            bitx = (byteData[bytep + Constants.ESF0ATRADD] & 0xc0) >> 6;
            bity = (byteData[bytep + Constants.ESF0ATRADD] & 0x30) >> 4;
            ix = byteData[bytep + Constants.ESF0XCDADD];
            iy = byteData[bytep + Constants.ESF0YCDADD];
            atrxy.x = (double)(((ix << 2) & 0x3fc) | (bitx & 0x003));
            atrxy.y = (double)(((iy << 2) & 0x3fc) | (bity & 0x003));
            return (atrxy);
        }
        private byte[] AtrDXYSet(byte[] byteData, int pp, ClsAtrXY atrxy)
        {
            int bytep;
            int bitx, bity, ix, iy;

            bitx = (byte)(atrxy.x) & 0x03;
            bity = (byte)(atrxy.y) & 0x03;
            ix = (int)(atrxy.x) >> 2;
            iy = (int)(atrxy.y) >> 2;
            bytep = pp * Constants.ESF0SIZADD;
            byteData[bytep + Constants.ESF0ATRADD] = (byte)((bitx << 6) | (bity << 4) | atrxy.atr);
            byteData[bytep + Constants.ESF0XCDADD] = (byte)ix;
            byteData[bytep + Constants.ESF0YCDADD] = (byte)iy;
            return (byteData);
        }
        private String getTblFontFileName(int sjis, String sFontName)
        {
            String sIdxFontFileName;

            if (sjis < 0x989e)
            {
                sIdxFontFileName = m_sEsFontPath + "\\" + sFontName + ".tbl";
            }
            else
            {
                sIdxFontFileName = m_sEsFontPath + "\\" + sFontName + ".tb2";
            }
            return (sIdxFontFileName);
        }
        private String getDataFontFileName(int sjis, String sFontName)
        {
            String sIdxFontFileName;

            if (sjis < 0x989e)
            {
                sIdxFontFileName = m_sEsFontPath + "\\" + sFontName + ".dat";
            }
            else
            {
                sIdxFontFileName = m_sEsFontPath + "\\" + sFontName + ".da2";
            }
            return (sIdxFontFileName);
        }
        private int getSeekPoint(int sjis)
        {
            int seekpoint, hcd, lcd;

            hcd = (sjis >> 8) & 0xff;
            lcd = (sjis) & 0xff;
            seekpoint = 0;
            if (sjis < 0x989e)
            {
                seekpoint = ((hcd - 0x81) * 0xc0 + lcd - 0x40) * Constants.FIDXSIZE;
            }
            else if (hcd < 0xe0)
            {
                seekpoint = ((hcd - 0x98) * 0xc0 + lcd - 0x40) * Constants.FIDXSIZE;
            }
            else if (hcd < 0xf2)
            {
                seekpoint = ((hcd - 0xd8) * 0xc0 + lcd - 0x40) * Constants.FIDXSIZE;
            }
            else
            {
                seekpoint = ((hcd - 0xf2) * 0xc0 + lcd - 0x40) * Constants.FIDXSIZE;
            }
            return (seekpoint);
        }
        private ObjFigu cnvLineToFigu(ClsAtrXY atrxy1, ClsAtrXY atrxy2)
        {
            ObjFigu objFigu;

            objFigu = new ObjFigu();
            objFigu.kind = Constants.LINEKIND;
            objFigu.x1 = atrxy1.x; objFigu.y1 = atrxy1.y;
            objFigu.x4 = atrxy2.x; objFigu.y4 = atrxy2.y;
            return (objFigu);
        }
        private ObjFigu cnvBezeToFigu(ClsAtrXY atrxy1, ClsAtrXY atrxy2, ClsAtrXY atrxy3, ClsAtrXY atrxy4)
        {
            ObjFigu objFigu;

            objFigu = new ObjFigu();
            objFigu.kind = Constants.BEZEKIND;
            objFigu.x1 = atrxy1.x; objFigu.y1 = atrxy1.y;
            objFigu.x2 = atrxy2.x; objFigu.y2 = atrxy2.y;
            objFigu.x3 = atrxy3.x; objFigu.y3 = atrxy3.y;
            objFigu.x4 = atrxy4.x; objFigu.y4 = atrxy4.y;
            return (objFigu);
        }
        private ObjFigu cnvArcToFigu(ClsAtrXY atrxy1, ClsAtrXY atrxy2, ClsAtrXY atrxy3)
        {
            ObjFigu objFigu;
            double x1, y1, x2, y2, x3, y3;
            ClsArcCenter cntr;
            ClsArcSER ser;
            ClsBeze bz;
            double rag;
            double t1, t2;

            x1 = atrxy1.x;
            y1 = atrxy1.y;
            x2 = atrxy2.x;
            y2 = atrxy2.y;
            x3 = atrxy3.x;
            y3 = atrxy3.y;
            objFigu = new ObjFigu();
            objFigu.kind = Constants.LINEKIND;
            objFigu.x1 = x1; objFigu.y1 = y1;
            objFigu.x4 = x3; objFigu.y4 = y3;
            cntr = m_libCurve.getCurve3pCenter(x1, y1, x2, y2, x3, y3);
            if (cntr.stat == false)
            {
                return (objFigu);
            }
            ser = m_libCurve.getCurve3pRagR(x1, y1, x2, y2, x3, y3, cntr.x0, cntr.y0);
            if (ser.stat == false)
            {
                return (objFigu);
            }
            bz = new ClsBeze();
            bz.x1 = x1; bz.y1 = y1;
            rag = (ser.erag - ser.srag) / 3.0 + ser.srag;
            bz.x2 = Math.Cos(rag) * ser.dr + cntr.x0;
            bz.y2 = Math.Sin(rag) * ser.dr + cntr.y0;
            rag = (ser.erag - ser.srag) / 3.0 * 2.0 + ser.srag;
            bz.x3 = Math.Cos(rag) * ser.dr + cntr.x0;
            bz.y3 = Math.Sin(rag) * ser.dr + cntr.y0;
            bz.x4 = x3; bz.y4 = y3;
            t1 = 1.0 / 3.0; t2 = 1.0 / 3.0 * 2.0;
            bz = m_libCurve.cnvOnpointToCntrlpoint(bz, t1, t2);
            objFigu.kind = Constants.BEZEKIND;
            objFigu.x1 = bz.x1; objFigu.y1 = bz.y1;
            objFigu.x2 = bz.x2; objFigu.y2 = bz.y2;
            objFigu.x3 = bz.x3; objFigu.y3 = bz.y3;
            objFigu.x4 = bz.x4; objFigu.y4 = bz.y4;
            return (objFigu);
        }
        private ObjFigu cnvCruveToFigu(ClsAtrXY atrxy0, ClsAtrXY atrxy1, ClsAtrXY atrxy2, ClsAtrXY atrxy3)
        {
            ObjFigu objFigu;
            int atr0, atr1, atr2; 
            double x0, y0, x1, y1, x2, y2, x3, y3;
            ClsBeze bz;
            ClsPairRag pr;
            ClsPoint pt;

            objFigu = new ObjFigu();
            objFigu.kind = Constants.BEZEKIND;
            objFigu.x1 = atrxy1.x; objFigu.y1 = atrxy1.y;
            objFigu.x4 = atrxy2.x; objFigu.y4 = atrxy2.y;
            atr0 = atrxy0.atr;
            atr1 = atrxy1.atr;
            atr2 = atrxy2.atr;
            x0 = atrxy0.x; y0 = atrxy0.y;
            x1 = atrxy1.x; y1 = atrxy1.y;
            x2 = atrxy2.x; y2 = atrxy2.y;
            x3 = atrxy3.x; y3 = atrxy3.y;
            pr = new ClsPairRag();
            pr.srag = m_libCurve.getCurveStartRag(atr0, atr1, x0, y0, x1, y1, x2, y2);
            pr.erag = m_libCurve.getCurveEndRag(atr1, atr2, x1, y1, x2, y2, x3, y3);
            pr = m_libCurve.legalCurveRag(pr);
            pt = m_libCurve.getBezeControlPoint(x1, y1, x2, y2, pr.srag);
            objFigu.x2 = pt.x; objFigu.y2 = pt.y;
            pt = m_libCurve.getBezeControlPoint(x2, y2, x1, y1, pr.erag);
            objFigu.x3 = pt.x; objFigu.y3 = pt.y;
            return (objFigu);
        }
    }
}
