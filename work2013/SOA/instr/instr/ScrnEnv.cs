using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace instr
{
    public class ScrnEnv
    {
        double m_dMMWorkXTop, m_dMMWorkYTop; // 作業領域
        double m_dMMWorkXSize, m_dMMWorkYSize; // 作業領域
        double m_dMMXSize, m_dMMYSize; // 余白を含んだ作業領域
        double m_dMMCntrX, m_dMMCntrY; // 座標原点
        double m_dXTime, m_dYTime;
        double m_dRltvXTop, m_dRltvYTop;
        double m_dRltvMoveX, m_dRltvMoveY;

        double m_dBMMCntrX, m_dBMMCntrY;
        double m_dBXTime, m_dBYTime;
        double m_dBRltvXTop, m_dBRltvYTop;
        double m_dBRltvMoveX, m_dBRltvMoveY;

        public ScrnEnv()
        {
            m_dMMXSize = 210.0;
            m_dMMYSize = 297.0;
            m_dXTime = 1.0;
            m_dYTime = 1.0;
            m_dRltvXTop = 0;
            m_dRltvYTop = 0;
            m_dRltvMoveX = 0;
            m_dRltvMoveY = 0;
        }

        public void SetScrnSize(double xsize, double ysize)
        {
            m_dMMWorkXSize = xsize;
            m_dMMWorkYSize = ysize;
        }
        public ClsPoint GetScrnSize()
        {
            ClsPoint retcpt;

            retcpt = new ClsPoint();
            retcpt.x = m_dMMWorkXSize;
            retcpt.y = m_dMMWorkYSize;
            return (retcpt);
        }
        public void InitSizeElement(int width, int height)
        {
            double xtime, ytime;

            m_dMMXSize = m_dMMWorkXSize * 1.1;
            m_dMMYSize = m_dMMWorkYSize * 1.1;
            xtime = m_dMMXSize / (double)(width);
            ytime = m_dMMYSize / (double)(height);
            if (xtime > ytime)
            {
                m_dXTime = xtime;
                m_dYTime = xtime;
                m_dMMYSize = height * m_dYTime;
            }
            else
            {
                m_dXTime = ytime;
                m_dYTime = ytime;
                m_dMMXSize = width * m_dXTime;
            }
            m_dMMWorkXTop = (m_dMMXSize - m_dMMWorkXSize) / 2.0;
            m_dMMWorkYTop = (m_dMMYSize - m_dMMWorkYSize) / 2.0;
            m_dMMCntrX = m_dMMWorkXTop;
            m_dMMCntrY = m_dMMWorkYTop;
        }
        public void SetBackupElement(double xtime, double ytime)
        {
            m_dBXTime = m_dXTime;
            m_dBYTime = m_dYTime;
            m_dBMMCntrX = m_dMMCntrX;
            m_dBMMCntrY = m_dMMCntrY;
            m_dBRltvXTop = m_dRltvXTop;
            m_dBRltvYTop = m_dRltvYTop;
            m_dBRltvMoveX = m_dRltvMoveX;
            m_dBRltvMoveY = m_dRltvMoveY;

            m_dXTime = xtime;
            m_dYTime = ytime;
            m_dMMCntrX = 0;
            m_dMMCntrY = 0;
            m_dRltvXTop = 0;
            m_dRltvYTop = 0;
            m_dRltvMoveX = 0;
            m_dRltvMoveY = 0;

        }
        public void ResetBackupElement()
        {
            m_dXTime = m_dBXTime;
            m_dYTime = m_dBYTime;
            m_dMMCntrX = m_dBMMCntrX;
            m_dMMCntrY = m_dBMMCntrY;
            m_dRltvXTop = m_dBRltvXTop;
            m_dRltvYTop = m_dBRltvYTop;
            m_dRltvMoveX = m_dBRltvMoveX;
            m_dRltvMoveY = m_dBRltvMoveY;
        }
        public void SetRltvMove(double movex, double movey)
        {
            m_dRltvMoveX = movex;
            m_dRltvMoveY = movey;
        }
        public double TrnsMMToRltv(double mm)
        {
            double rltv;

            rltv = mm / m_dXTime;
            return (rltv);
        }
        public ClsPoint TrnsMMToRltv(ClsPoint cpt)
        {
            double mmx, mmy;
            double rltvx, rltvy;
            ClsPoint retcpt;

            mmx = cpt.x;
            mmy = cpt.y;
	        rltvx = ((mmx) + m_dMMCntrX) / m_dXTime;
	        rltvy = ((mmy) + m_dMMCntrY) / m_dYTime;
	        rltvx = rltvx - m_dRltvXTop;
	        rltvy = rltvy - m_dRltvYTop;
            retcpt = new ClsPoint();
	        retcpt.x = rltvx + m_dRltvMoveX;
            retcpt.y = rltvy + m_dRltvMoveY;
            return (retcpt);
        }
        public double TrnsRltvToMM(double rltv)
        {
            double mm;

            mm = rltv * m_dXTime;
            return (mm);
        }
        public ClsPoint TrnsRltvToMM(ClsPoint cpt)
        {
            double rltvx, rltvy;
            double mmx, mmy;
            ClsPoint retcpt;

	        rltvx = cpt.x;
	        rltvy = cpt.y;
	        rltvx = rltvx - m_dRltvMoveX;
	        rltvy = rltvy - m_dRltvMoveY;
	        rltvx = rltvx + m_dRltvXTop;
	        rltvy = rltvy + m_dRltvYTop;
	        mmx = rltvx * m_dXTime - m_dMMCntrX;
	        mmy = rltvy * m_dYTime - m_dMMCntrY;
            retcpt = new ClsPoint();
            retcpt.x = mmx;
            retcpt.y = mmy;
            return (retcpt);
        }
    }
}
