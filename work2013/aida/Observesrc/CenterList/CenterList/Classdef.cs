using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace CenterList
{
    public partial class ClsCard
    {
        [XmlElement("m_sCondition")]
        public string m_sCondition;
        [XmlElement("m_sSetNo")]
        public string m_sSetNo;
        [XmlElement("m_sIP")]
        public string m_sIP;
        [XmlElement("m_sSyoNo")]
        public string m_sSyoNo;
        [XmlElement("m_sSyoName")]
        public string m_sSyoName;
        [XmlElement("m_sAddress1")]
        public string m_sAddress1;
        [XmlElement("m_sAddress2")]
        public string m_sAddress2;
        [XmlElement("m_sTel1")]
        public string m_sTel1;
        [XmlElement("m_sTel2")]
        public string m_sTel2;
        [XmlElement("m_sName")]
        public string m_sName;
        [XmlElement("m_sBikou")]
        public string m_sBikou;
        [XmlElement("m_sStat")]
        public String m_sStat;

        private int x;
        private int y;

        public ClsCard()
        {
            m_sSetNo = "";
            m_sStat = "";
            m_sAddress1 = "";
        }
        public void copySetElement(ClsCard clsCard)
        {
            m_sSetNo = clsCard.m_sSetNo;
            m_sIP = clsCard.m_sIP;
            m_sSyoNo = clsCard.m_sSyoNo;
            m_sAddress1 = clsCard.m_sAddress1;
            m_sAddress2 = clsCard.m_sAddress2;
            m_sTel1 = clsCard.m_sTel1;
            m_sTel2 = clsCard.m_sTel2;
            m_sName = clsCard.m_sName;
            m_sBikou = clsCard.m_sBikou;
        }
    }
}
