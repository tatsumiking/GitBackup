using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Xml;
using System.Xml.Serialization;

namespace instr
{
    public partial class ObjFigu
    {
        [XmlElement("kind")]
        public int kind;
        [XmlElement("x1")]
        public double x1;
        [XmlElement("y1")]
        public double y1;
        [XmlElement("x2")]
        public double x2;
        [XmlElement("y2")]
        public double y2;
        [XmlElement("x3")]
        public double x3;
        [XmlElement("y3")]
        public double y3;
        [XmlElement("x4")]
        public double x4;
        [XmlElement("y4")]
        public double y4;

        public ObjFigu()
        {
            kind = Constants.LINEKIND;
            x1 = 0; y1 = 0;
            x2 = 0; y2 = 0;
            x3 = 0; y3 = 0;
            x4 = 0; y4 = 0;
        }
    }
    public partial class ObjVect:ObjData
    {
        [XmlElement("m_lstObjFigu")]
        public List<ObjFigu> m_lstObjFigu = new List<ObjFigu>();

        public ObjVect()
        {
        }

        public override void RenewMiniMax()
        {
            int max, idx;
            ObjFigu objFigu;

            m_objArea.Init();
            max = m_lstObjFigu.Count;
            for (idx = 0; idx < max; idx++)
            {
                objFigu = m_lstObjFigu[idx];
                m_objArea.SetMiniMaxPoint(objFigu.x1, objFigu.y1);
                m_objArea.SetMiniMaxPoint(objFigu.x4, objFigu.y4);
            }
        }
        public override ObjMiniMax GetMiniMax()
        {
            RenewMiniMax();
            return (m_objArea);
        }
        public PathFigure GetCopyPF()
        {
            ObjFigu objFigu;
            ClsPoint cp;
            int max, idx;
            LineSegment lnseg;
            BezierSegment bzseg;

            PathFigure pf;
            pf = new PathFigure();
            objFigu = m_lstObjFigu[0];
            cp = new ClsPoint();
            cp.x = objFigu.x1; cp.y = objFigu.y1;
            cp = m_scrnEnv.TrnsMMToRltv(cp);
            pf.StartPoint = new Point(cp.x, cp.y);
            max = m_lstObjFigu.Count;
            for (idx = 0; idx < max; idx++)
            {
                objFigu = m_lstObjFigu[idx];
                if (objFigu.kind == Constants.LINEKIND)
                {
                    lnseg = new LineSegment();
                    cp.x = objFigu.x4; cp.y = objFigu.y4;
                    cp = m_scrnEnv.TrnsMMToRltv(cp);
                    lnseg.Point = new Point(cp.x, cp.y);
                    pf.Segments.Add(lnseg);
                }
                else
                {
                    bzseg = new BezierSegment();
                    cp.x = objFigu.x2; cp.y = objFigu.y2;
                    cp = m_scrnEnv.TrnsMMToRltv(cp);
                    bzseg.Point1 = new Point(cp.x, cp.y);
                    cp.x = objFigu.x3; cp.y = objFigu.y3;
                    cp = m_scrnEnv.TrnsMMToRltv(cp);
                    bzseg.Point2 = new Point(cp.x, cp.y);
                    cp.x = objFigu.x4; cp.y = objFigu.y4;
                    cp = m_scrnEnv.TrnsMMToRltv(cp);
                    bzseg.Point3 = new Point(cp.x, cp.y);
                    pf.Segments.Add(bzseg);
                }
            }
            return (pf);
        }
        public PathFigure GetPathFigure(PathFigure pf)
        {
            ObjFigu objFigu;
            ClsPoint cp;
            int max, idx;
            LineSegment lnseg;
            BezierSegment bzseg;

            cp = new ClsPoint();
            if(pf == null)
            {
                pf = new PathFigure();
            }
            objFigu = m_lstObjFigu[0];
            cp.x = objFigu.x1; cp.y = objFigu.y1;
            cp = m_scrnEnv.TrnsMMToRltv(cp);
            pf.StartPoint = new Point(cp.x, cp.y);
            max = m_lstObjFigu.Count;
            for (idx = 0; idx < max; idx++)
            {
                objFigu = m_lstObjFigu[idx];
                if(objFigu.kind == Constants.LINEKIND){
                    lnseg = GetLineSegment(pf, idx);
                    cp.x = objFigu.x4; cp.y = objFigu.y4;
                    cp = m_scrnEnv.TrnsMMToRltv(cp);
                    lnseg.Point = new Point(cp.x, cp.y);
                }
                else
                {
                    bzseg = GetBezierSegment(pf, idx);
                    cp.x = objFigu.x2; cp.y = objFigu.y2;
                    cp = m_scrnEnv.TrnsMMToRltv(cp);
                    bzseg.Point1 = new Point(cp.x, cp.y);
                    cp.x = objFigu.x3; cp.y = objFigu.y3;
                    cp = m_scrnEnv.TrnsMMToRltv(cp);
                    bzseg.Point2 = new Point(cp.x, cp.y);
                    cp.x = objFigu.x4; cp.y = objFigu.y4;
                    cp = m_scrnEnv.TrnsMMToRltv(cp);
                    bzseg.Point3 = new Point(cp.x, cp.y);
                }
            }
            return (pf);
        }

        public LineSegment GetLineSegment(PathFigure pf, int idx)
        {
            LineSegment lnseg;

            if (pf.Segments.Count <= idx || pf.Segments[idx].GetType() != typeof(LineSegment))
            {
                lnseg = new LineSegment();
                pf.Segments.Add(lnseg);
            }
            else
            {
                lnseg = (LineSegment)pf.Segments[idx];
            }
            return (lnseg);
        }
        public BezierSegment GetBezierSegment(PathFigure pf, int idx)
        {
            BezierSegment bzseg;

            if (pf.Segments.Count <= idx || pf.Segments[idx].GetType() != typeof(BezierSegment))
            {
                bzseg = new BezierSegment();
                pf.Segments.Add(bzseg);
            }
            else
            {
                bzseg = (BezierSegment)pf.Segments[idx];
            }
            return (bzseg);
        }
        public override void TrnsMove(double movex, double movey)
        {
            int max, idx;
            ObjFigu objFigu;

            max = m_lstObjFigu.Count;
            for (idx = 0; idx < max; idx++)
            {
                objFigu = m_lstObjFigu[idx];
                if (objFigu.kind == Constants.LINEKIND)
                {
                    objFigu.x1 = objFigu.x1 + movex;
                    objFigu.y1 = objFigu.y1 + movey;
                    objFigu.x4 = objFigu.x4 + movex;
                    objFigu.y4 = objFigu.y4 + movey;
                }
                else
                {
                    objFigu.x1 = objFigu.x1 + movex;
                    objFigu.y1 = objFigu.y1 + movey;
                    objFigu.x2 = objFigu.x2 + movex;
                    objFigu.y2 = objFigu.y2 + movey;
                    objFigu.x3 = objFigu.x3 + movex;
                    objFigu.y3 = objFigu.y3 + movey;
                    objFigu.x4 = objFigu.x4 + movex;
                    objFigu.y4 = objFigu.y4 + movey;
                }
            }
            RenewMiniMax();
        }
        public override void TrnsSize(double cx, double cy, double timex, double timey)
        {
            int max, idx;
            ObjFigu objFigu;

            max = m_lstObjFigu.Count;
            for (idx = 0; idx < max; idx++)
            {
                objFigu = m_lstObjFigu[idx];
                if (objFigu.kind == Constants.LINEKIND)
                {
                    objFigu.x1 = (objFigu.x1 - cx) * timex + cx;
                    objFigu.y1 = (objFigu.y1 - cy) * timey + cy;
                    objFigu.x4 = (objFigu.x4 - cx) * timex + cx;
                    objFigu.y4 = (objFigu.y4 - cy) * timey + cy;
                }
                else
                {
                    objFigu.x1 = (objFigu.x1 - cx) * timex + cx;
                    objFigu.y1 = (objFigu.y1 - cy) * timey + cy;
                    objFigu.x2 = (objFigu.x2 - cx) * timex + cx;
                    objFigu.y2 = (objFigu.y2 - cy) * timey + cy;
                    objFigu.x3 = (objFigu.x3 - cx) * timex + cx;
                    objFigu.y3 = (objFigu.y3 - cy) * timey + cy;
                    objFigu.x4 = (objFigu.x4 - cx) * timex + cx;
                    objFigu.y4 = (objFigu.y4 - cy) * timey + cy;
                }
            }
            RenewMiniMax();
        }
    }
}
