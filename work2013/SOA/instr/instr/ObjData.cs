using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Xml;
using System.Xml.Serialization;

namespace instr
{
    public partial class ObjMiniMax
    {
        [XmlElement("m_dMinX")]
        public double m_dMinX = 1000000;
        [XmlElement("m_dMinY")]
        public double m_dMinY = 1000000;
        [XmlElement("m_dMaxX")]
        public double m_dMaxX = -1000000;
        [XmlElement("m_dMaxY")]
        public double m_dMaxY = -1000000;

        public void Init()
        {
            m_dMinX = 1000000;
            m_dMinY = 1000000;
            m_dMaxX = -1000000;
            m_dMaxY = -1000000;
        }
        public void SetNull()
        {
            m_dMinX = 0;
            m_dMinY = 0;
            m_dMaxX = 0;
            m_dMaxY = 0;
        }
        public Boolean CheckNull()
        {
            if(m_dMinX == 0 && m_dMinY == 0
            && m_dMaxX == 0 && m_dMaxY == 0)
            {
                return (true);
            }
            return (false);
        }
        public void SetMiniMaxPoint(double dx, double dy)
        {
            if (m_dMinX > dx)
            {
                m_dMinX = dx;
            }
            if (m_dMinY > dy)
            {
                m_dMinY = dy;
            }
            if (m_dMaxX < dx)
            {
                m_dMaxX = dx;
            }
            if (m_dMaxY < dy)
            {
                m_dMaxY = dy;
            }
        }
    }
    public partial class ObjData : Object
    {
        [XmlElement("m_objArea")]
        public ObjMiniMax m_objArea;
        [XmlElement("m_nFlag")]
        public int m_nFlag;
        [XmlElement("m_nKind")]
        public int m_nKind;
        [XmlElement("m_nLinkNo")]
        public int m_nLinkNo; // XML保存のためのリンク番号

        protected ScrnEnv m_scrnEnv;
        protected LibCommon m_libCmn;

        public ObjData()
        {
            m_objArea = new ObjMiniMax();
        }
        public virtual void SetScrnEnvClass(ScrnEnv scrnEnv)
        {
            m_scrnEnv = scrnEnv;
        }
        public virtual void SetLibCmnClass(LibCommon libCmn)
        {
            m_libCmn = libCmn;
        }
        public void SetFlag(int flag)
        {
            m_nFlag = flag;
        }
        public int GetFlag()
        {
            return (m_nFlag);
        }
        public virtual void UpdateDrawElement()
        {
        }
        public virtual void AddChildrenCanvas(Canvas canvas)
        {
        }
        public virtual void RemoveChildrenCanvas(Canvas canvas)
        {
        }
        public virtual void Draw(Canvas canvas)
        {
        }
        public virtual ObjMiniMax GetMiniMax()
        {
            return (m_objArea);
        }
        public virtual void RenewMiniMax()
        {
        }
        public virtual void TrnsMove(double movex, double movey)
        {
        }
        public virtual void TrnsSize(double cx, double cy, double timex, double timey)
        {
        }
    }
}
