using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace sekiji
{
    public partial class MainWindow : Window
    {
        private void moveDownSelectSeet(string str)
        {
            string[] ary;
            int tbl, seet;
            ClsSekiji clsSekiji;

            clsSekiji = null;
            ary = str.Split('_');
            if (ary[0].Substring(0, 1) == "S")
            {
                seet = m_libCmn.StrToInt(ary[2]);
                clsSekiji = m_clsHaiseki.m_lstSouryo[seet];
            }
            else if (ary[0].Substring(0, 1) == "T")
            {
                tbl = m_libCmn.StrToInt(ary[1]);
                seet = m_libCmn.StrToInt(ary[2]);
                clsSekiji = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet];
            }
            m_bMoveSeet = false;
            if (clsSekiji != null)
            {
                if (clsSekiji.checkSelect() == true)
                {
                    m_bMoveSeet = true;
                }
            }
        }
        private void moveUpSelectSeet(string str)
        {
            string[] ary;
            int tbl, seet;
            ClsSekiji clsSekiji;

            clsSekiji = null;
            tbl = -1;
            seet = -1;
            ary = str.Split('_');
            if (ary[0].Substring(0, 1) == "S")
            {
                tbl = -1;
                seet = m_libCmn.StrToInt(ary[2]);
                clsSekiji = m_clsHaiseki.m_lstSouryo[seet];
            }
            else if (ary[0].Substring(0, 1) == "T")
            {
                tbl = m_libCmn.StrToInt(ary[1]);
                seet = m_libCmn.StrToInt(ary[2]);
                clsSekiji = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet];
            }
            m_bMoveSeet = false;
            if (clsSekiji != null)
            {
                if (checkSelectTable() == true)
                {
                    changeTableSeet(tbl);
                }
                else
                {
                    moveSelectSeet(tbl, seet);
                }
            }
        }
        private Boolean checkSelectTable()
        {
            int tmax, tbl;
            int smax, seet;
            Boolean ret;

            if (m_nCrtTable == -1)
            {
                return(false);
            }
            tmax = m_clsHaiseki.m_nTableBlockCount * 2;
            smax = m_clsHaiseki.m_nSeetCount;
            for (tbl = 0; tbl < tmax; tbl++)
            {
                for (seet = 0; seet < smax; seet++)
                {
                    ret = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].checkSelect();
                    if (m_nCrtTable == tbl)
                    {
                        if (ret == false)
                        {
                            return (false);
                        }
                    }
                    else
                    {
                        if (ret == true)
                        {
                            return (false);
                        }
                    }
                }
            }
            return (true);
        }
        private void changeTableSeet(int tblDst)
        {
            int tblSrc;
            int smax, seet;
            ClsSekiji skjSrc, skjDst;
            string sStr;
            int nInt; 
            Canvas cnvsSekijiSrc;
            Canvas cnvsSekijiDst;
            Canvas cnvsHkdmnSrc;
            Canvas cnvsHkdmnDst;
            Canvas cnvsNameSrc;
            Canvas cnvsNameDst;

            resetSelect();

            tblSrc = m_nCrtTable;
            smax = m_clsHaiseki.m_nSeetCount;
            for (seet = 0; seet < smax; seet++)
            {
                skjSrc = m_clsHaiseki.m_lstTable[tblSrc].m_lstSekiji[seet];
                skjDst = m_clsHaiseki.m_lstTable[tblDst].m_lstSekiji[seet];

                sStr = skjSrc.m_sName;
                skjSrc.m_sName = skjDst.m_sName;
                skjDst.m_sName = sStr;

                nInt = skjSrc.m_nRyouriType;
                skjSrc.m_nRyouriType = skjDst.m_nRyouriType;
                skjDst.m_nRyouriType = nInt;

                nInt = skjSrc.m_nHikidemonoType;
                skjSrc.m_nHikidemonoType = skjDst.m_nHikidemonoType;
                skjDst.m_nHikidemonoType = nInt;

                cnvsSekijiSrc = skjSrc.getSekijiCanvas();
                cnvsSekijiDst = skjDst.getSekijiCanvas();
                cnvsHkdmnSrc = skjSrc.getHkdmnCanvas();
                cnvsHkdmnDst = skjDst.getHkdmnCanvas();
                cnvsNameSrc = skjSrc.getNameCanvas();
                cnvsNameDst = skjDst.getNameCanvas();

                skjSrc.setHkdmnCanvas(cnvsSekijiDst);
                skjDst.setHkdmnCanvas(cnvsSekijiSrc);
                skjSrc.setNameCanvas(cnvsSekijiDst);
                skjDst.setNameCanvas(cnvsSekijiSrc);

                cnvsSekijiSrc.Children.Clear();
                cnvsSekijiDst.Children.Clear();
                if (cnvsHkdmnSrc != null)
                {
                    cnvsSekijiDst.Children.Add(cnvsHkdmnSrc);
                }
                if (cnvsNameSrc != null)
                {
                    cnvsSekijiDst.Children.Add(cnvsNameSrc);
                }
                if (cnvsHkdmnDst != null)
                {
                    cnvsSekijiSrc.Children.Add(cnvsHkdmnDst);
                }
                if (cnvsNameDst != null)
                {
                    cnvsSekijiSrc.Children.Add(cnvsNameDst);
                }
            }
        }
        private void setCanvasSelect(string str)
        {
            string[] ary;

            resetInputTextBox(m_nSelectTable, m_nSelectSeet);
            ary = str.Split('_');
            if (ary[0].Substring(0, 1) == "R")
            {
                m_nSelectTable = -1;
                m_nSelectSeet = -1;
            }
            else if (ary[0].Substring(0, 1) == "S")
            {
                m_nSelectTable = -1;
                m_nSelectSeet = m_libCmn.StrToInt(ary[2]);
                setInputTextBox(m_nSelectTable, m_nSelectSeet);
            }
            else if (ary[0].Substring(0, 1) == "T")
            {
                m_nSelectTable = m_libCmn.StrToInt(ary[1]);
                m_nSelectSeet = m_libCmn.StrToInt(ary[2]);
                setInputTextBox(m_nSelectTable, m_nSelectSeet);
            }
            setSelect();
        }
        private void setSelect()
        {
            Canvas cnvs;
            int seet;
            int sseet, eseet;

            ModifierKeys modifierKeys = Keyboard.Modifiers;
            if (m_nSelectTable == -1 && m_nSelectSeet == -1)
            {
                if ((modifierKeys & ModifierKeys.Control) == ModifierKeys.None)
                {
                    resetSelect();
                }
                cnvs = m_clsHaiseki.getReizenCanvas();
                cnvs.Background = m_brushSlelct;
            }
            else if (m_nCrtTable != m_nSelectTable)
            {
                m_nCrtTable = m_nSelectTable;
                m_nCrtSeet = m_nSelectSeet;
                if ((modifierKeys & ModifierKeys.Control) == ModifierKeys.None)
                {
                    if (m_nCrtSeet != -1)
                    {
                        m_nSSeet = m_nCrtSeet;
                    }
                    resetSelect();
                }
                if (m_nSelectTable == -1)
                {
                    m_clsHaiseki.m_lstSouryo[m_nCrtSeet].setSelect();
                    cnvs = m_clsHaiseki.m_lstSouryo[m_nCrtSeet].getSekijiCanvas();
                    cnvs.Background = m_brushSlelct;
                }
                else
                {
                    m_clsHaiseki.m_lstTable[m_nCrtTable].m_lstSekiji[m_nCrtSeet].setSelect();
                    cnvs = m_clsHaiseki.m_lstTable[m_nCrtTable].m_lstSekiji[m_nCrtSeet].getSekijiCanvas();
                    cnvs.Background = m_brushSlelct;
                }
            }
            else if (m_nSelectSeet != -1)
            {
                m_nCrtTable = m_nSelectTable;
                m_nCrtSeet = m_nSelectSeet;
                if ((modifierKeys & ModifierKeys.Shift) != ModifierKeys.None)
                {
                    resetSelect();
                    m_nESeet = m_nCrtSeet;
                    if (m_nSSeet < m_nESeet)
                    {
                        sseet = m_nSSeet;
                        eseet = m_nESeet;
                    }
                    else
                    {
                        sseet = m_nESeet;
                        eseet = m_nSSeet;
                    }
                    for (seet = sseet; seet <= eseet; seet++)
                    {
                        if (m_nSelectTable == -1)
                        {
                            m_clsHaiseki.m_lstSouryo[m_nCrtSeet].setSelect();
                            cnvs = m_clsHaiseki.m_lstSouryo[m_nCrtSeet].getSekijiCanvas();
                            cnvs.Background = m_brushSlelct;
                        }
                        else
                        {
                            m_clsHaiseki.m_lstTable[m_nCrtTable].m_lstSekiji[seet].setSelect();
                            cnvs = m_clsHaiseki.m_lstTable[m_nCrtTable].m_lstSekiji[seet].getSekijiCanvas();
                            cnvs.Background = m_brushSlelct;
                        }
                    }
                }
                else if ((modifierKeys & ModifierKeys.Control) == ModifierKeys.None)
                {
                    m_nSSeet = m_nCrtSeet;
                    resetSelect();
                }
                if (m_nSelectTable == -1)
                {
                    m_clsHaiseki.m_lstSouryo[m_nCrtSeet].setSelect();
                    cnvs = m_clsHaiseki.m_lstSouryo[m_nCrtSeet].getSekijiCanvas();
                    cnvs.Background = m_brushSlelct;
                }
                else
                {
                    m_clsHaiseki.m_lstTable[m_nCrtTable].m_lstSekiji[m_nCrtSeet].setSelect();
                    cnvs = m_clsHaiseki.m_lstTable[m_nCrtTable].m_lstSekiji[m_nCrtSeet].getSekijiCanvas();
                    cnvs.Background = m_brushSlelct;
                }
            }
        }
        private void resetSelect()
        {
            Canvas cnvs;
            int tmax, tbl;
            int smax, seet;

            cnvs = m_clsHaiseki.getReizenCanvas();
            cnvs.Background = Brushes.Black;

            smax = m_clsHaiseki.m_nSouryoCount;
            for (seet = 0; seet < smax; seet++)
            {
                m_clsHaiseki.m_lstSouryo[seet].resetSelect();
                cnvs = m_clsHaiseki.m_lstSouryo[seet].getSekijiCanvas();
                cnvs.Background = m_brushNomal;
            }
            tmax = m_clsHaiseki.m_nTableBlockCount * 2;
            smax = m_clsHaiseki.m_nSeetCount;
            for (tbl = 0; tbl < tmax; tbl++)
            {
                for (seet = 0; seet < smax; seet++)
                {
                    m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].resetSelect();
                    cnvs = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet].getSekijiCanvas();
                    cnvs.Background = m_brushNomal;
                }
            }
        }
        private void moveSelectSeet(int setTbl, int setSeet)
        {
            List<ClsStockSekiji> lstSelectStockSekiji = new List<ClsStockSekiji>();
            int seet;

            resetInputTextBox(m_nSelectTable, m_nSelectSeet);
            seet = deleteSouryoSelectSekiji(lstSelectStockSekiji, setTbl, setSeet);
            if (setTbl == -1)
            {
                setSeet = seet;
            }
            seet = deleteTableSelectSekiji(lstSelectStockSekiji, setTbl, setSeet);
            if (setTbl != -1)
            {
                setSeet = seet;
            }
            if (setTbl == -1)
            {
                insertSouryoSelectSekiji(lstSelectStockSekiji, setTbl, setSeet);
            }
            else
            {
                insertTableSelectSekiji(lstSelectStockSekiji, setTbl, setSeet);
            }
            resetSelect();
        }
        private int deleteSouryoSelectSekiji(List<ClsStockSekiji> lstSelectStockSekiji, int setTbl, int setSeet)
        {
            List<ClsStockSekiji> lstRemainStockSekiji = new List<ClsStockSekiji>();
            ClsStockSekiji clsStockSekiji;
            int smax, seet;
            int max, idx;
            Boolean bFlag;
            ClsSekiji clsSekiji;
            string sName;
            int nRType;
            int nHType;

            bFlag = false;
            smax = m_clsHaiseki.m_nSouryoCount;
            for (seet = 0; seet < smax; seet++)
            {
                clsSekiji = m_clsHaiseki.m_lstSouryo[seet];
                clsStockSekiji = new ClsStockSekiji();
                clsStockSekiji.m_sName = clsSekiji.m_sName;
                clsStockSekiji.m_nRyouriType = clsSekiji.m_nRyouriType;
                clsStockSekiji.m_nHikidemonoType = clsSekiji.m_nHikidemonoType;
                clsStockSekiji.m_nTable = -1;
                clsStockSekiji.m_nSeet = seet;
                if (clsSekiji.checkSelect() == true)
                {
                    lstSelectStockSekiji.Add(clsStockSekiji);
                    bFlag = true;
                }
                else
                {
                    lstRemainStockSekiji.Add(clsStockSekiji);
                }
            }
            if (bFlag)
            {
                max = lstRemainStockSekiji.Count;
                for (seet = 0; seet < max; seet++)
                {
                    clsSekiji = m_clsHaiseki.m_lstSouryo[seet];
                    sName = lstRemainStockSekiji[seet].m_sName;
                    nRType = lstRemainStockSekiji[seet].m_nRyouriType;
                    nHType = lstRemainStockSekiji[seet].m_nHikidemonoType;
                    setSekijiElement(clsSekiji, sName, nRType, nHType);
                    if (setTbl == lstRemainStockSekiji[seet].m_nTable && setSeet == lstRemainStockSekiji[seet].m_nSeet)
                    {
                        setSeet = seet;
                    }
                }
                for (; seet < smax; seet++)
                {
                    clsSekiji = m_clsHaiseki.m_lstSouryo[seet];
                    sName = "";
                    nRType = 0;
                    nHType = 0;
                    setSekijiElement(clsSekiji, sName, nRType, nHType);
                }
            }
            lstRemainStockSekiji.Clear();
            return (setSeet);
        }
        private int deleteTableSelectSekiji(List<ClsStockSekiji> lstSelectStockSekiji, int setTbl, int setSeet)
        {
            List<ClsStockSekiji> lstRemainStockSekiji = new List<ClsStockSekiji>();
            ClsStockSekiji clsStockSekiji;
            int tmax, tbl;
            int smax, seet;
            int max, idx;
            Boolean bFlag;
            ClsSekiji clsSekiji;
            string sName;
            int nRType;
            int nHType;

            tmax = m_clsHaiseki.m_nTableBlockCount * 2;
            smax = m_clsHaiseki.m_nSeetCount;
            for (tbl = 0; tbl < tmax; tbl++)
            {
                bFlag = false;
                for (seet = 0; seet < smax; seet++)
                {
                    clsSekiji = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet];
                    clsStockSekiji = new ClsStockSekiji();
                    clsStockSekiji.m_sName = clsSekiji.m_sName;
                    clsStockSekiji.m_nRyouriType = clsSekiji.m_nRyouriType;
                    clsStockSekiji.m_nHikidemonoType = clsSekiji.m_nHikidemonoType;
                    clsStockSekiji.m_nTable = -1;
                    clsStockSekiji.m_nSeet = seet;
                    if (clsSekiji.checkSelect() == true)
                    {
                        lstSelectStockSekiji.Add(clsStockSekiji);
                        bFlag = true;
                    }
                    else
                    {
                        lstRemainStockSekiji.Add(clsStockSekiji);
                    }
                }
                if (bFlag)
                {
                    max = lstRemainStockSekiji.Count;
                    for (seet = 0; seet < max; seet++)
                    {
                        clsSekiji = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet];
                        sName = lstRemainStockSekiji[seet].m_sName;
                        nRType = lstRemainStockSekiji[seet].m_nRyouriType;
                        nHType = lstRemainStockSekiji[seet].m_nHikidemonoType;
                        setSekijiElement(clsSekiji, sName, nRType, nHType);
                        if (setTbl == lstRemainStockSekiji[seet].m_nTable && setSeet == lstRemainStockSekiji[seet].m_nSeet)
                        {
                            setSeet = seet;
                        }
                    }
                    for (; seet < smax; seet++)
                    {
                        clsSekiji = m_clsHaiseki.m_lstTable[tbl].m_lstSekiji[seet];
                        sName = "";
                        nRType = 0;
                        nHType = 0;
                        setSekijiElement(clsSekiji, sName, nRType, nHType);
                    }
                }
                lstRemainStockSekiji.Clear();
            }
            return (setSeet);
        }
        private void insertSouryoSelectSekiji(List<ClsStockSekiji> lstSelectStockSekiji, int setTbl, int setSeet)
        {
            List<ClsStockSekiji> lstRemainStockSekiji = new List<ClsStockSekiji>();
            ClsStockSekiji clsStockSekiji;
            int smax, seet;
            int max, idx;
            ClsSekiji clsSekiji;
            string sName;
            int nRType;
            int nHType;

            smax = m_clsHaiseki.m_nSouryoCount;
            for (seet = setSeet; seet < smax; seet++)
            {
                clsSekiji = m_clsHaiseki.m_lstSouryo[seet];
                clsStockSekiji = new ClsStockSekiji();
                clsStockSekiji.m_sName = clsSekiji.m_sName;
                clsStockSekiji.m_nRyouriType = clsSekiji.m_nRyouriType;
                clsStockSekiji.m_nHikidemonoType = clsSekiji.m_nHikidemonoType;
                clsStockSekiji.m_nTable = -1;
                clsStockSekiji.m_nSeet = seet;
                lstRemainStockSekiji.Add(clsStockSekiji);
            }
            max = lstSelectStockSekiji.Count;
            for (idx = 0, seet = setSeet; idx < max; idx++, seet++)
            {
                if (smax <= seet)
                {
                    return;
                }
                clsSekiji = m_clsHaiseki.m_lstSouryo[seet];
                sName = lstSelectStockSekiji[idx].m_sName;
                nRType = lstSelectStockSekiji[idx].m_nRyouriType;
                nHType = lstSelectStockSekiji[idx].m_nHikidemonoType;
                setSekijiElement(clsSekiji, sName, nRType, nHType);
            }
            max = lstRemainStockSekiji.Count;
            for (idx = 0; seet < smax; idx++, seet++)
            {
                if (max <= idx)
                {
                    return;
                }
                clsSekiji = m_clsHaiseki.m_lstSouryo[seet];
                sName = lstRemainStockSekiji[idx].m_sName;
                nRType = lstRemainStockSekiji[idx].m_nRyouriType;
                nHType = lstRemainStockSekiji[idx].m_nHikidemonoType;
                setSekijiElement(clsSekiji, sName, nRType, nHType);
            }
            for (; seet < smax; seet++)
            {
                clsSekiji = m_clsHaiseki.m_lstSouryo[seet];
                sName = "";
                nRType = 0;
                nHType = 0;
                setSekijiElement(clsSekiji, sName, nRType, nHType);
            }
        }
        private void insertTableSelectSekiji(List<ClsStockSekiji> lstSelectStockSekiji, int setTbl, int setSeet)
        {
            List<ClsStockSekiji> lstRemainStockSekiji = new List<ClsStockSekiji>();
            ClsStockSekiji clsStockSekiji;
            int smax, seet;
            int max, idx;
            ClsSekiji clsSekiji;
            string sName;
            int nRType;
            int nHType;

            smax = m_clsHaiseki.m_nSeetCount;
            for (seet = setSeet; seet < smax; seet++)
            {
                clsSekiji = m_clsHaiseki.m_lstTable[setTbl].m_lstSekiji[seet];
                clsStockSekiji = new ClsStockSekiji();
                clsStockSekiji.m_sName = clsSekiji.m_sName;
                clsStockSekiji.m_nRyouriType = clsSekiji.m_nRyouriType;
                clsStockSekiji.m_nHikidemonoType = clsSekiji.m_nHikidemonoType;
                clsStockSekiji.m_nTable = -1;
                clsStockSekiji.m_nSeet = seet;
                lstRemainStockSekiji.Add(clsStockSekiji);
            }
            max = lstSelectStockSekiji.Count;
            for (idx = 0, seet = setSeet; idx < max; idx++, seet++)
            {
                if (smax <= seet)
                {
                    return;
                }
                clsSekiji = m_clsHaiseki.m_lstTable[setTbl].m_lstSekiji[seet];
                sName = lstSelectStockSekiji[idx].m_sName;
                nRType = lstSelectStockSekiji[idx].m_nRyouriType;
                nHType = lstSelectStockSekiji[idx].m_nHikidemonoType;
                setSekijiElement(clsSekiji, sName, nRType, nHType);
            }
            max = lstRemainStockSekiji.Count;
            for (idx = 0; seet < smax; idx++, seet++)
            {
                if (max <= idx)
                {
                    return;
                }
                clsSekiji = m_clsHaiseki.m_lstTable[setTbl].m_lstSekiji[seet];
                sName = lstRemainStockSekiji[idx].m_sName;
                nRType = lstRemainStockSekiji[idx].m_nRyouriType;
                nHType = lstRemainStockSekiji[idx].m_nHikidemonoType;
                setSekijiElement(clsSekiji, sName, nRType, nHType);
            }
            for (; seet < smax; seet++)
            {
                clsSekiji = m_clsHaiseki.m_lstTable[setTbl].m_lstSekiji[seet];
                sName = "";
                nRType = 0;
                nHType = 0;
                setSekijiElement(clsSekiji, sName, nRType, nHType);
            }
        }

    }
}
