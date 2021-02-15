using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Xml;
using System.Xml.Serialization;

namespace instr
{
    public partial class ObjVectList : ObjData
    {
        [XmlElement("m_lstObjVect")]
        public List<ObjVect> m_lstObjVect = new List<ObjVect>();

        private Path m_shapPath;
        private GeometryGroup m_gg;
        private PathGeometry m_pg;

        public ObjVectList()
        {
            m_shapPath = null;
            m_gg = null;
            m_pg = null;
        }
        public override void SetScrnEnvClass(ScrnEnv scrnEnv)
        {
            int idx, max;

            m_scrnEnv = scrnEnv;
            max = m_lstObjVect.Count;
            for (idx = 0; idx < max; idx++)
            {
                m_lstObjVect[idx].SetScrnEnvClass(scrnEnv);
            }
        }
        public override void SetLibCmnClass(LibCommon libCmn)
        {
            int idx, max;

            m_libCmn = libCmn;
            max = m_lstObjVect.Count;
            for (idx = 0; idx < max; idx++)
            {
                m_lstObjVect[idx].SetLibCmnClass(libCmn);
            }
        }
        public override void RenewMiniMax()
        {
            int max, idx;
            ObjVect objVect;
            ObjMiniMax area;

            m_objArea.Init();
            max = m_lstObjVect.Count;
            for (idx = 0; idx < max; idx++)
            {
                objVect = m_lstObjVect[idx];
                objVect.RenewMiniMax();
                area = objVect.GetMiniMax();
                m_objArea.SetMiniMaxPoint(area.m_dMinX, area.m_dMinY);
                m_objArea.SetMiniMaxPoint(area.m_dMaxX, area.m_dMaxY);
            }
        }
        public override ObjMiniMax GetMiniMax()
        {
            RenewMiniMax();
            return (m_objArea);
        }
        public void AddVect(ObjVect ObjVect)
        {
            m_lstObjVect.Add(ObjVect);
        }
        public void RemoveVect(ObjVect ObjVect)
        {
            m_lstObjVect.Remove(ObjVect);
        }
        public PathGeometry GetCopyPG()
        {
            int vmax, vidx;
            ObjVect objVect;
            PathFigure pf;

            PathGeometry pg = new PathGeometry();
            vmax = m_lstObjVect.Count;
            for (vidx = 0; vidx < vmax; vidx++)
            {
                objVect = m_lstObjVect[vidx];
                pf = objVect.GetCopyPF();
                pg.Figures.Add(pf);
            }
            return (pg);
        }
        public override void UpdateDrawElement()
        {
            int vmax, vidx;
            ObjVect objVect;
            PathFigure pf;

            if (m_shapPath == null)
            {
                m_shapPath = new System.Windows.Shapes.Path();
                m_pg = new PathGeometry();
                m_shapPath.Fill = Brushes.Black;
                //m_shapPath.Stroke = Brushes.Black;
                //m_shapPath.StrokeThickness = 1.0d;
                m_shapPath.Data = m_pg;
            }
            m_pg = (PathGeometry)m_shapPath.Data;
            m_pg.Figures.Clear();
            vmax = m_lstObjVect.Count;
            for (vidx = 0; vidx < vmax; vidx++)
            {
                objVect = m_lstObjVect[vidx];
                pf = objVect.GetPathFigure(null);
                m_pg.Figures.Add(pf);
            }
        }
        public PathGeometry GetPathGeometry()
        {
            return (m_pg);
        }
        public override void AddChildrenCanvas(Canvas canvas)
        {
            try
            {
                canvas.Children.Add(m_shapPath);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.Message);
            }
        }
        public override void RemoveChildrenCanvas(Canvas canvas)
        {
            canvas.Children.Remove(m_shapPath);
        }
        public override void TrnsMove(double movex, double movey)
        {
            int vmax, vidx;
            ObjVect objVect;

            vmax = m_lstObjVect.Count;
            for (vidx = 0; vidx < vmax; vidx++)
            {
                objVect = m_lstObjVect[vidx];
                objVect.TrnsMove(movex, movey);
            }
            RenewMiniMax();
            UpdateDrawElement();
        }
        public override void TrnsSize(double cx, double cy, double timex, double timey)
        {
            int vmax, vidx;
            ObjVect objVect;

            vmax = m_lstObjVect.Count;
            for (vidx = 0; vidx < vmax; vidx++)
            {
                objVect = m_lstObjVect[vidx];
                objVect.TrnsSize(cx, cy, timex, timey);
            }
            RenewMiniMax();
            UpdateDrawElement();
        }
    }
}
