using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace instr
{
    public class CurveConsts
    {
        public const int NORAG = 6;
    }
    public class ClsPairAns
    {
        public Boolean stat;
        public double ansa;
        public double ansb;

        public ClsPairAns()
        {
            stat = false;
            ansa = 0.0;
            ansb = 0.0;
        }
    }
    public class ClsPoint
    {
        public double x;
        public double y;

        public ClsPoint()
        {
            x = 0;
            y = 0;
        }
        public ClsPoint(double dx, double dy)
        {
            x = dx;
            y = dy;
        }
    }
    public class ClsAtrXY
    {
        public int atr;
        public double x;
        public double y;

        public ClsAtrXY()
        {
            atr = 0;
            x = 0;
            y = 0;
        }
    }
    public class ClsBeze
    {
        public double x1, y1;
        public double x2, y2;
        public double x3, y3;
        public double x4, y4;
        public ClsBeze()
        {
            x1 = 0; y1 = 0;
            x2 = 0; y2 = 0;
            x3 = 0; y3 = 0;
            x4 = 0; y4 = 0;
        }
    }
    public class ClsArcSER
    {
        public Boolean stat;
        public double srag;
        public double erag;
        public double dr;
        public ClsArcSER()
        {
            stat = false;
            srag = 0;
            erag = 0;
            dr = 0;
        }
    }
    public class ClsArcCenter
    {
        public Boolean stat;
        public double x0;
        public double y0;
        public ClsArcCenter()
        {
            stat = false;
            x0= 0;
            y0= 0;
        }
    }
    public class ClsPairRag
    {
        public double srag;
        public double erag;
        public ClsPairRag()
        {
            srag = 0.0;
            erag = 0.0;
        }
    }
    public class LibCurve
    {
        private double m_degree1;
        private double m_hpai;

        public LibCurve()
        {
            m_degree1 = Math.PI / 180.0;
            m_hpai = Math.PI / 2.0;
        }
        public Boolean checkEqual(double num1, double num2)
        {
            if ((num2 - 0.0001) < num1 && num1 < (num2 + 0.0001))
            {
                return (true);
            }
            return (false);
        }
        public Boolean checkParallelRag(double rag1, double rag2)
        {
            if ((rag2 - m_degree1) < rag1 && rag1 < (rag2 + m_degree1))
            {
                return (true);
            }
            return (false);

        }
        public Boolean checkTanErrorRag(double rag)
        {
            rag = Math.Abs(rag);
            if ((m_hpai - m_degree1) < rag && rag < (m_hpai + m_degree1))
            {
                return (true);
            }
            return (false);

        }
        public double calcHypot(double xlen, double ylen)
        {
            double dlen = xlen * xlen + ylen * ylen;
            dlen = Math.Sqrt(dlen);
            return (dlen);
        }
        public ClsPairAns calcSmltqu(double a, double b, double c, double d, double e, double f)
        {
            ClsPairAns pa;

            pa = new ClsPairAns();
            if (checkEqual((a * e - b * d), 0))
            {
                pa.stat = false;
                return(pa);
            }
            pa.stat = true;
            pa.ansa = (c * e - b * f) / (a * e - b * d);
            pa.ansb = (a * f - c * d) / (a * e - b * d);
            return (pa);
        }
        public Boolean checkNorag(double x1, double y1, double x2, double y2, double x3, double y3)
        {
            double rag;

	        if(checkEqual(x2, x1) && checkEqual(y2, y1)){
                return(true);
            }
	        if(checkEqual(x2, x3) && checkEqual(y2, y3)){
                return(true);
            }
            return(false);
        }
        // BEZEとBEZEの接線角度の計算
        public double calc2DivCenterRag(double x1, double y1, double x2, double y2, double x3, double y3)
        {
	        double rag1, rag2;
            double rag;

	        if(checkNorag(x1, y1, x2, y2, x3, y3) == true){
                return(Constants.NORAG);
            }
	        rag1 = Math.Atan2(y2-y1, x2-x1);
	        rag2 = Math.Atan2(y3-y2, x3-x2);
	        rag = (rag1-rag2);
	        if(rag > Math.PI){
		        rag -= Math.PI*2.0;
	        }
	        else if(rag < -Math.PI){
		        rag += Math.PI*2.0;
	        }
	        rag /= 2.0;
	        return(rag);
        }
        // 直線とBEZEの接線角度の計算
        public double calcTangentRag(double x1, double y1, double x2, double y2, double x3, double y3)
        {
	        double rag1, rag2;
            double rag;
	        if(checkNorag(x1, y1, x2, y2, x3, y3) == true){
                return(Constants.NORAG);
            }
	        rag1 = Math.Atan2(y2-y1, x2-x1);
	        rag2 = Math.Atan2(y3-y2, x3-x2);
	        rag = (rag1-rag2);
        	return(rag);
        }
        public ClsArcCenter getCurve3pCenter(double x1, double y1, double x2, double y2, double x3, double y3)
        {
            ClsArcCenter cntr;
            double x12, y12, x23, y23;
            double rag12, rag23;
            double tan12, tan23;

            cntr = new ClsArcCenter();
            cntr.stat = false;
            if (checkEqual(y2, y1) && checkEqual(x2, x1))
            {
                return (cntr);
            }
            if (checkEqual(y3, y2) && checkEqual(x3, x2))
            {
                return (cntr);
            }
            x12 = (x1 + x2) / 2.0; y12 = (y1 + y2) / 2.0;
            x23 = (x2 + x3) / 2.0; y23 = (y2 + y3) / 2.0;
            rag12 = Math.Atan2(y2 - y1, x2 - x1);
            rag23 = Math.Atan2(y3 - y2, x3 - x2);
            if (checkParallelRag(rag12, rag23) == true)
            {
                return (cntr);
            }
            rag12 = (rag12 >= m_hpai) ? (rag12 - m_hpai) : (rag12 + m_hpai);
            rag23 = (rag23 >= m_hpai) ? (rag23 - m_hpai) : (rag23 + m_hpai);
            if (checkTanErrorRag(rag12) == true)
            {
                tan23 = Math.Tan(rag23);
                cntr.stat = true;
                cntr.x0 = x12;
                cntr.y0 = tan23 * (x12 - x23) + y23;
                return (cntr);
            }
            tan12 = Math.Tan(rag12);
            if (checkTanErrorRag(rag23) == true)
            {
                cntr.stat = true;
                cntr.x0 = x23;
                cntr.y0 = tan12 * (x23 - x12) + y12;
                return (cntr);
            }
            tan23 = Math.Tan(rag23);
            if (checkEqual(tan12, tan23))
            {
                return (cntr);
            }
            cntr.x0 = (tan23 * x23 - tan12 * x12 + y12 - y23) / (tan23 - tan12);
            if (-0.0001 < tan12 && tan12 < 0.0001)
            {
                cntr.stat = true;
                cntr.y0 = y12;
                return (cntr);
            }
            tan12 = 1.0 / tan12;
            if (-0.0001 < tan23 && tan23 < 0.0001)
            {
                cntr.stat = true;
                cntr.y0 = y23;
                return (cntr);
            }
            tan23 = 1.0 / tan23;
            if (checkEqual(tan12, tan23))
            {
                return (cntr);
            }
            cntr.stat = true;
            cntr.y0 = (tan23 * y23 - tan12 * y12 + x12 - x23) / (tan23 - tan12);
            return (cntr);
        }
        public ClsArcSER getCurve3pRagR(double x1, double y1, double x2, double y2, double x3, double y3, double x0, double y0)
        {
            ClsArcSER ser;
            double len01, len02, subrag;
            double rag02;

            ser = new ClsArcSER();
            ser.stat = false;
            if ((checkEqual(x1, x0) && checkEqual(y1, y0))
            || (checkEqual(x2, x0) && checkEqual(y2, y0))
            || (checkEqual(x2, x0) && checkEqual(y2, y0)))
            {

                ser.dr = 0.0;
                ser.srag = 0.0;
                ser.erag = 0.0;
                return (ser);
            }
            ser.srag = Math.Atan2(y1 - y0, x1 - x0);
            rag02 = Math.Atan2(y2 - y0, x2 - x0);
            ser.erag = Math.Atan2(y3 - y0, x3 - x0);
            if (checkEqual(ser.srag, rag02) || checkEqual(rag02, ser.erag))
            {
                ser.dr = calcHypot(x1 - x0, y1 - y0);
                ser.srag = ser.erag;
                return (ser);
            }
            if (ser.srag > rag02)
            {
                if (rag02 < ser.erag)
                {
                    ser.erag = (ser.erag) - Math.PI * 2.0;
                    subrag = ser.srag - ser.erag;
                    if (subrag > Math.PI * 2.0)
                    {
                        ser.erag = (ser.erag) + Math.PI * 4;
                    }
                }
            }
            else
            {
                if (rag02 > ser.erag)
                {
                    ser.erag = (ser.erag) + Math.PI * 2.0;
                    subrag = ser.erag - ser.srag;
                    if (subrag > Math.PI * 2.0)
                    {
                        ser.erag = (ser.erag) - Math.PI * 4;
                    }
                }
            }
            ser.stat = true;
            len01 = calcHypot(x1 - x0, y1 - y0);
            len02 = calcHypot(x2 - x0, y2 - y0);
            ser.dr = (len01 + len02) / 2.0;
            return (ser);

        }
        public ClsBeze cnvOnpointToCntrlpoint(ClsBeze bz, double t1, double t2)
        {
            double x1, y1, x2, y2, x3, y3, x4, y4;
            double ta;
            double b1p1, b2p1, b3p1, b4p1;
            double b1p2, b2p2, b3p2, b4p2;
            double a, b, c, d, e, f;
            ClsPairAns pa;

            x1 = bz.x1; y1 = bz.y1;
            x2 = bz.x2; y2 = bz.y2;
            x3 = bz.x3; y3 = bz.y3;
            x4 = bz.x4; y4 = bz.y4;

            ta = 1 - t1;
            b1p1 = ta * ta * ta;
            b2p1 = 3.0 * t1 * ta * ta;
            b3p1 = 3.0 * t1 * t1 * ta;
            b4p1 = t1 * t1 * t1;

            ta = 1 - t2;
            b1p2 = ta * ta * ta;
            b2p2 = 3.0 * t2 * ta * ta;
            b3p2 = 3.0 * t2 * t2 * ta;
            b4p2 = t2 * t2 * t2;

            a = b2p1;
            b = b3p1;
            c = x2 - b1p1 * x1 - b4p1 * x4;
            d = b2p2;
            e = b3p2;
            f = x3 - b1p2 * x1 - b4p2 * x4;
            pa = calcSmltqu(a, b, c, d, e, f);
            if (pa.stat == true)
            {
                bz.x2 = pa.ansa;
                bz.x3 = pa.ansb;
            }
            a = b2p1;
            b = b3p1;
            c = y2 - b1p1 * y1 - b4p1 * y4;
            d = b2p2;
            e = b3p2;
            f = y3 - b1p2 * y1 - b4p2 * y4;
            pa = calcSmltqu(a, b, c, d, e, f);
            if (pa.stat == true)
            {
                bz.y2 = pa.ansa;
                bz.y3 = pa.ansb;
            }
            return (bz);
        }
        public double getCurveStartRag(int atr0, int atr1, double x0, double y0, double x1, double y1, double x2, double y2)
        {
            double rag;
        	if((atr1 & Constants.CURVATR) != 0 && (atr1 & Constants.CONTATR) != 0){
		        if((atr0 & Constants.CURVATR) != 0){
			        rag = calc2DivCenterRag(x0, y0, x1, y1, x2, y2);
		        }
		        else{
			        rag = calcTangentRag(x0, y0, x1, y1, x2, y2);
		        }
        	}
	        else{
		        rag = Constants.NORAG;
	        }
	        return(rag);
        }

        public double getCurveEndRag(int atr1, int atr2, double x1, double y1, double x2, double y2, double x3, double y3)
        {
            double rag;
        	if((atr1 & Constants.CURVATR) != 0){
		        if((atr2 & Constants.CURVATR) != 0 && (atr2 & Constants.CONTATR) != 0){
			        if((atr2 & Constants.BEZEATR) != 0){
				        rag = calcTangentRag(x3, y3, x2, y2, x1, y1);
			        }
			        else{
				        rag = calc2DivCenterRag(x3, y3, x2, y2, x1, y1);
			        }
		        }
		        else if((atr2 & Constants.CONTATR) != 0){
			        rag = calcTangentRag(x3, y3, x2, y2, x1, y1);
		        }
		        else{
			        rag = Constants.NORAG;
		        }
	        }
	        else{
		        rag = Constants.NORAG;
	        }
	        return(rag);
        }
        public ClsPairRag legalCurveRag(ClsPairRag pr)
        {
            if (checkEqual(pr.srag, Constants.NORAG))
            {
                pr.srag = -(pr.erag);
            }
            if (checkEqual(pr.erag, Constants.NORAG))
            {
                pr.erag = -(pr.srag);
            }
            return (pr);
        }
        public ClsPoint getBezeControlPoint(double x1, double y1, double x2, double y2, double rag)
        {
            ClsPoint pt;
            double tsin, tcos;
            double xt, yt;

            pt = new ClsPoint();
            if ((-2.0 * Math.PI) < rag && rag < (2.0 * Math.PI))
            {
                xt = (x2 - x1) * 2.0 / 5.0;
                yt = (y2 - y1) * 2.0 / 5.0;
                tcos = Math.Cos(rag);
                tsin = Math.Sin(rag);
                pt.x = (xt * tcos - yt * tsin) + x1;
                pt.y = (yt * tcos + xt * tsin) + y1;
            }
            else
            {
                pt.x = x1;
                pt.y = y1;
            }
            return (pt);
        }
    }
}
