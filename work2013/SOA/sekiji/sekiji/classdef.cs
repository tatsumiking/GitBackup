using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Media;
using System.Xml.Serialization;

namespace sekiji
{
    static class Constants
    {
        public const int A3WIDTH = 420;
        public const int A3HEIGHT = 297;
        public const int SOURYOSEETMAX = 10;
        public const int TABLEMAX = 10;
        public const int SEETMAX = 25;
        public const int HIKIDEMONOSETMAX = 6;
        public const int HIKIDEMONOMAX = 5;
        public const int RYOURISETMAX = 9;
        public const int NUMMAX = 10000;
    }
    public partial class ClsTable
    {
        [XmlElement("m_sName")]
        public string m_sName;
        [XmlElement("m_lstSekiji")]
        public List<ClsSekiji> m_lstSekiji = new List<ClsSekiji>();

        public ClsTable()
        {
            m_sName = "";
        }
        public void initElement()
        {
        }
        public void createListElement()
        {
            int idx, max;
            ClsSekiji clsSekiji;

            max = Constants.SEETMAX;
            for (idx = 0; idx < max; idx++)
            {
                clsSekiji = new ClsSekiji();
                m_lstSekiji.Add(clsSekiji);
            }
        }
        public void clearListElement()
        {
            m_lstSekiji.Clear();
        }
        public void resetSelect(Brush brush)
        {
            int idx, max;
            Canvas cnvs;

            max = Constants.SEETMAX;
            for (idx = 0; idx < max; idx++)
            {
                m_lstSekiji[idx].resetSelect();
                cnvs = m_lstSekiji[idx].getNameCanvas();
                if (cnvs != null)
                {
                    cnvs.Background = brush;
                }
            }
        }
    }
    public partial class ClsStockSekiji
    {
        public string m_sName;
        public int m_nRyouriType;
        public int m_nHikidemonoType;
        public int m_nTable;
        public int m_nSeet;
    }
    public partial class ClsSekiji
    {
        [XmlElement("m_sName")]
        public string m_sName;
        [XmlElement("m_nRyouriType")]
        public int m_nRyouriType;
        [XmlElement("m_nHikidemonoType")]
        public int m_nHikidemonoType;

        private Boolean m_bSelect;
        private Canvas m_cnvsSekiji;
        private Canvas m_cnvsHkdmn;
        private Canvas m_cnvsName;

        public ClsSekiji()
        {
            m_sName = "";
            m_nRyouriType = 0;
            m_nHikidemonoType = 0;
            m_bSelect = false;
            m_cnvsSekiji = null;
            m_cnvsHkdmn = null;
            m_cnvsName = null;
        }
        public void setSekijiCanvas(Canvas cnvs)
        {
            m_cnvsSekiji = cnvs;
        }
        public Canvas getSekijiCanvas()
        {
            return (m_cnvsSekiji);
        }
        public void setHkdmnCanvas(Canvas cnvs)
        {
            m_cnvsHkdmn = cnvs;
        }
        public Canvas getHkdmnCanvas()
        {
            return (m_cnvsHkdmn);
        }
        public TextBlock getHkdmnTextBlock()
        {
            TextBlock tb = (TextBlock)(m_cnvsHkdmn.Children[0]);
            return (tb);
        }
        public void setNameCanvas(Canvas cnvs)
        {
            m_cnvsName = cnvs;
        }
        public Canvas getNameCanvas()
        {
            return (m_cnvsName);
        }
        public TextBlock getNameTextBlock()
        {
            TextBlock tb = (TextBlock)(m_cnvsName.Children[0]);
            return (tb);
        }
        public void setSelect()
        {
            m_bSelect = true;
        }
        public void resetSelect()
        {
            m_bSelect = false;
        }
        public Boolean checkSelect()
        {
            return (m_bSelect);
        }
    }
    public partial class ClsHikidemono
    {
        [XmlElement("m_sName")]
        public string m_sName;
        [XmlElement("m_sArgb")]
        public string m_sArgb;
        [XmlElement("m_lstSyouhin")]
        public List<string> m_lstSyouhin = new List<string>();

        private Brush m_brush;
        private List<TextBlock> m_tblk = new List<TextBlock>();

        public ClsHikidemono()
        {
            int idx, max;

            m_sName = "";
            m_sArgb = "0";
            max = Constants.SOURYOSEETMAX;
            for (idx = 0; idx < max; idx++)
            {
                m_tblk.Add(null);
            }
        }
        public void initElement()
        {
        }
        public void createListElement()
        {
            int idx, max;

            max = Constants.SOURYOSEETMAX;
            for (idx = 0; idx < max; idx++)
            {
                m_lstSyouhin.Add("");
            }
        }
        public void clearListElement()
        {
            m_lstSyouhin.Clear();
        }
        public void createBrush()
        {
            int argb;
            int a, r, g, b;
            Color clr;

            if (m_sArgb != "0")
            {
                argb = Convert.ToInt32(m_sArgb, 16);
                a = (argb >> 24) & 0x00ff;
                r = (argb >> 16) & 0x00ff;
                g = (argb >> 8) & 0x00ff;
                b = (argb) & 0x00ff;
                clr = Color.FromArgb((byte)a, (byte)r, (byte)g, (byte)b);
                m_brush = new SolidColorBrush(clr);
            }
        }
        public Brush getBrush()
        {
            return (m_brush);
        }
        public void setTextBlock(int idx, TextBlock tblk)
        {
            m_tblk[idx] = tblk;
        }
        public TextBlock getTextBlock(int idx)
        {
            return (m_tblk[idx]);
        }
    }
    public partial class ClsRyouri
    {
        [XmlElement("m_sName")]
        public string m_sName;
        [XmlElement("m_sArgb")]
        public string m_sArgb;

        private Brush m_brush;

        public ClsRyouri()
        {
            m_sName = "";
            m_sArgb = "0";
        }
        public void createBrush()
        {
            int argb;
            int a, r, g, b;
            Color clr;

            if (m_sArgb != "0")
            {
                argb = Convert.ToInt32(m_sArgb, 16);
                a = (argb >> 24) & 0x00ff;
                r = (argb >> 16) & 0x00ff;
                g = (argb >> 8) & 0x00ff;
                b = (argb) & 0x00ff;
                clr = Color.FromArgb(255, (byte)r, (byte)g, (byte)b);
                m_brush = new SolidColorBrush(clr);
            }
        }
        public Brush getBrush()
        {
            return (m_brush);
        }
    }
    public partial class ClsHaiseki
    {
        [XmlElement("m_sSoukeName")]
        public string m_sSoukeName; // 葬家名
        [XmlElement("m_sTitle")]
        public string m_sTitle; // 表題
        [XmlElement("m_sReizenText")]
        public string m_sReizenText; // 左上文字列
        [XmlElement("m_nReizenAlignment")]
        public int m_nReizenAlignment; // 左上文字列配置　1：中央、0：右寄せ
        [XmlElement("m_nReizenHikimonoType")]
        public int m_nReizenHikimonoType;
        [XmlElement("m_sRightBottomText")]
        public string m_sRightBottomText; // 左上文字列
        [XmlElement("m_nSouryoCount")]
        public int m_nSouryoCount;
        [XmlElement("m_nTableBlockCount")]
        public int m_nTableBlockCount;
        [XmlElement("m_nSeetCount")]
        public int m_nSeetCount;
        [XmlElement("m_nYudemasu")]
        public int m_nYudemasu;
        [XmlElement("m_lstSouryo")]
        public List<ClsSekiji> m_lstSouryo = new List<ClsSekiji>();
        [XmlElement("m_lstTable")]
        public List<ClsTable> m_lstTable = new List<ClsTable>();
        [XmlElement("m_nHikidemonoSetCount")]
        public int m_nHikidemonoSetCount;
        [XmlElement("m_lstHikidemono")]
        public List<ClsHikidemono> m_lstHikidemono = new List<ClsHikidemono>();
        [XmlElement("m_lstRyouri")]
        public List<ClsRyouri> m_lstRyouri = new List<ClsRyouri>();

        private Canvas m_cnvsReizen;
        private Canvas m_cnvsHkdmn;
        private Canvas m_cnvsName;

        public ClsHaiseki()
        {
            m_sTitle = "灰寄せ会場座席表";
            m_sReizenText = "御霊前";
            m_nReizenAlignment = 0;
            m_nReizenHikimonoType = 0;
            m_sRightBottomText = "(順不同　誤字、脱字ございましたらお許しください。)";
            m_nSouryoCount = 6;
            m_nYudemasu = 3;
            m_nHikidemonoSetCount = Constants.HIKIDEMONOSETMAX;
            m_nTableBlockCount = 0;
            m_nSeetCount = 0;

        }
        public void initElement()
        {
        }
        public void createBrush()
        {
            int idx, max;

            max = m_lstHikidemono.Count;
            for (idx = 0; idx < max; idx++)
            {
                m_lstHikidemono[idx].createBrush();
            }
            max = m_lstRyouri.Count;
            for (idx = 0; idx < max; idx++)
            {
                m_lstRyouri[idx].createBrush();
            }
        }
        public void createListElement()
        {
            int idx, max;
            ClsTable clsTable;
            ClsSekiji clsSekiji;
            ClsHikidemono clsHikidemono;
            ClsRyouri clsRyouri;

            max = Constants.SOURYOSEETMAX;
            for (idx = 0; idx < max; idx++)
            {
                clsSekiji = new ClsSekiji();
                m_lstSouryo.Add(clsSekiji);
            }
            max = Constants.TABLEMAX;
            for (idx = 0; idx < max; idx++)
            {
                clsTable = new ClsTable();
                clsTable.createListElement();
                m_lstTable.Add(clsTable);
            }
            max = m_nHikidemonoSetCount + 1;
            for (idx = 0; idx < max; idx++)
            {
                clsHikidemono = new ClsHikidemono();
                clsHikidemono.createListElement();
                m_lstHikidemono.Add(clsHikidemono);
            }
            m_lstHikidemono[0].m_sName = "Ａ";
            m_lstHikidemono[0].m_sArgb = "0xFFC8C8FF";
            m_lstHikidemono[0].createBrush();
            m_lstHikidemono[1].m_sName = "Ｂ";
            m_lstHikidemono[1].m_sArgb = "0xFFC8FFC8";
            m_lstHikidemono[1].createBrush();
            m_lstHikidemono[2].m_sName = "Ｃ";
            m_lstHikidemono[2].m_sArgb = "0xFFFFC8C8";
            m_lstHikidemono[2].createBrush();
            m_lstHikidemono[3].m_sName = "Ｄ";
            m_lstHikidemono[3].m_sArgb = "0xFFC8FFFF";
            m_lstHikidemono[3].createBrush();
            m_lstHikidemono[4].m_sName = "Ｅ";
            m_lstHikidemono[4].m_sArgb = "0xFFFFC8FF";
            m_lstHikidemono[4].createBrush();
            m_lstHikidemono[5].m_sName = "Ｆ";
            m_lstHikidemono[5].m_sArgb = "0xFFFFFFC8";
            m_lstHikidemono[5].createBrush();

            clsRyouri = new ClsRyouri();
            clsRyouri.m_sName = "料理+黒飯+団子";
            clsRyouri.m_sArgb = "0x000000";
            clsRyouri.createBrush();
            m_lstRyouri.Add(clsRyouri);

            clsRyouri = new ClsRyouri();
            clsRyouri.m_sName = "お子様ランチ";
            clsRyouri.m_sArgb = "0xFFFF0000";
            clsRyouri.createBrush();
            m_lstRyouri.Add(clsRyouri);

            clsRyouri = new ClsRyouri();
            clsRyouri.m_sName = "ジュニアランチ";
            clsRyouri.m_sArgb = "0xFF883300";
            clsRyouri.createBrush();
            m_lstRyouri.Add(clsRyouri);

            clsRyouri = new ClsRyouri();
            clsRyouri.m_sName = "料理+黒飯";
            clsRyouri.m_sArgb = "0xFF008800";
            clsRyouri.createBrush();
            m_lstRyouri.Add(clsRyouri);

            clsRyouri = new ClsRyouri();
            clsRyouri.m_sName = "料理+団子";
            clsRyouri.m_sArgb = "0xFF0088FF";
            clsRyouri.createBrush();
            m_lstRyouri.Add(clsRyouri);

            clsRyouri = new ClsRyouri();
            clsRyouri.m_sName = "料理";
            clsRyouri.m_sArgb = "0xFF000088";
            clsRyouri.createBrush();
            m_lstRyouri.Add(clsRyouri);

            clsRyouri = new ClsRyouri();
            clsRyouri.m_sName = "特別メニュー";
            clsRyouri.m_sArgb = "0xFFDD8800";
            clsRyouri.createBrush();
            m_lstRyouri.Add(clsRyouri);

            clsRyouri = new ClsRyouri();
            clsRyouri.m_sName = "供養膳";
            clsRyouri.m_sArgb = "0xFF000000";
            clsRyouri.createBrush();
            m_lstRyouri.Add(clsRyouri);

            clsRyouri = new ClsRyouri();
            clsRyouri.m_sName = "ゆでます";
            clsRyouri.m_sArgb = "0xFF000000";
            clsRyouri.createBrush();
            m_lstRyouri.Add(clsRyouri);
        }
        public void clearListElement()
        {
            int max, idx;
            m_lstSouryo.Clear();
            max = m_lstTable.Count;
            for (idx = 0; idx < max; idx++)
            {
                m_lstTable[idx].clearListElement();
            }
            m_lstTable.Clear();
            max = m_lstHikidemono.Count;
            for (idx = 0; idx < max; idx++)
            {
                m_lstHikidemono[idx].clearListElement();
            }
            m_lstHikidemono.Clear();
            m_lstRyouri.Clear();
        }
        public void setReizenCanvas(Canvas cnvs)
        {
            m_cnvsReizen = cnvs;
        }
        public Canvas getReizenCanvas()
        {
            return (m_cnvsReizen);
        }
        public void setHkdmnCanvas(Canvas cnvs)
        {
            m_cnvsHkdmn = cnvs;
        }
        public Canvas getHkdmnCanvas()
        {
            return (m_cnvsHkdmn);
        }
        public TextBlock getHkdmnTextBlock()
        {
            TextBlock tb = (TextBlock)(m_cnvsHkdmn.Children[0]);
            return (tb);
        }
        public void setNameCanvas(Canvas cnvs)
        {
            m_cnvsName = cnvs;
        }
        public Canvas getNameCanvas()
        {
            return (m_cnvsName);
        }
        public TextBlock getNameTextBlock()
        {
            TextBlock tb = (TextBlock)(m_cnvsName.Children[0]);
            return (tb);
        }
    }
}
