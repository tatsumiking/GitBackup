using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace sekiji
{
    public class LibOdbc
    {
        public LibCommon m_libCmn;
        public string m_sExecPath;
        private string m_sMsg;
        private string m_sLog;

        public void setLibCommonClass(LibCommon libCmn)
        {
            m_libCmn = libCmn;
        }
        public void setExecPath(string sExecPath)
        {
            m_sExecPath = sExecPath;
        }
        public Boolean createNameTable()
        {
            OdbcConnection con;
            string sSql;

            con = openMdb();
            if (con == null)
            {
                return (false);
            }
            sSql = "DROP TABLE name;";
            execSqlNoError(con, sSql);

            sSql = "CREATE TABLE name (id AUTOINCREMENT PRIMARY KEY";
            sSql = sSql + ", kana VARCHAR(32)"; // カナ
            sSql = sSql + ", kanji VARCHAR(32)"; // 漢字
            sSql = sSql + ");";
            execSql(con, sSql);

            closeMdb(con);
            return (true);
        }
        public void initNameTable(string[] sLine)
        {
            string sRegdate;
            OdbcConnection con;
            string sSql, sInsSql;
            int idx, max;
            string[] sAry;
            string sData;

            con = openMdb();
            if (con != null)
            {
                sSql = "INSERT INTO name (";
                sSql = sSql + "kana";
                sSql = sSql + ",kanji";
                sSql = sSql + ") VALUES (";

                max = sLine.Length;
                for (idx = 1; idx < max; idx++)
                {
                    sAry = sLine[idx].Split(',');
                    if (2 <= sAry.Length)
                    {
                        sData = "'" + sAry[0] + "','" + sAry[1]+"'";
                        sInsSql = sSql + sData + ");";
                        execSql(con, sInsSql);
                    }
                }
                closeMdb(con);
            }
        }
        public string searchNameKwy(string sName)
        {
            string sSql;
            OdbcConnection con;
            List<string> listRet;
            string strRet;

            sSql = "SELECT * FROM name";
            sSql = sSql + " WHERE (";
            sSql = sSql + "kanji LIKE '" + sName + "');";
            strRet = "";
            con = openMdb();
            if (con != null)
            {
                listRet = getRecordList(con, sSql);
                if (listRet.Count != 0)
                {
                    strRet = listRet[0];
                }
                closeMdb(con);
            }
            return (strRet);
        }
        private List<string> getRecordList(OdbcConnection con, String sSql)
        {
            List<string> listRet;
            OdbcCommand cmd;
            OdbcDataReader reader;
            string sId, sKana;
            string sRecord;

            listRet = new List<string>();
            try
            {
                cmd = new OdbcCommand(sSql, con);
                reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    sId = reader.GetString(0);
                    sKana = reader.GetString(1);
                    sRecord = sId + "," + sKana;
                    listRet.Add(sRecord);
                }
            }
            catch (Exception ex)
            {
                m_sMsg = "該当するレコードが見つかりません";
                MessageBox.Show(m_sMsg, "確認", MessageBoxButton.OK);
                return (null);
            }
            return (listRet);
        }
        private OdbcConnection openMdb()
        {
            string sCnct;
            OdbcConnection con;

            try
            {
                sCnct = "Driver={Microsoft Access Driver (*.mdb)};DBQ=" + m_sExecPath + "\\name.mdb";
                con = new OdbcConnection(sCnct);
                con.Open();
            }
            catch (Exception ex)
            {
                m_sMsg = "name.mdbを使用したデータベースに接続できません";
                m_sLog = ex.ToString();
                MessageBox.Show(m_sMsg, "確認", MessageBoxButton.OK);
                return (null);
            }
            return (con);
        }
        private string execSqlNoError(OdbcConnection con, string sSql)
        {
            OdbcCommand cmd;
            try
            {
                cmd = new OdbcCommand(sSql, con);
                cmd.ExecuteNonQuery();
                cmd.Dispose();
            }
            catch (Exception ex)
            {
                return ("0");
            }
            return ("1");
        }
        private string execSql(OdbcConnection con, string sSql)
        {
            OdbcCommand cmd;
            try
            {
                cmd = new OdbcCommand(sSql, con);
                cmd.ExecuteNonQuery();
                cmd.Dispose();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "確認", MessageBoxButton.OK);
                return ("0");
            }
            return ("1");
        }
        private string closeMdb(OdbcConnection con)
        {
            try
            {
                con.Close();
                con.Dispose();
            }
            catch (Exception ex)
            {
                m_sMsg = "name.mdbを切断できません\n" + ex.ToString();
                MessageBox.Show(m_sMsg, "確認", MessageBoxButton.OK);
                return ("0");
            }
            return ("1");
        }
        private string getReaderString(OdbcDataReader reader, int idx)
        {
            Type type;
            string sType;
            string str;

            if (reader.IsDBNull(idx) == true)
            {
                str = "";
            }
            else
            {
                type = reader.GetFieldType(idx);
                sType = type.Name;
                if (sType == "String")
                {
                    str = reader.GetString(idx);
                }
                else if (sType == "Int32")
                {
                    str = reader.GetInt32(idx).ToString();
                }
                else
                {
                    str = "";
                }
            }
            return (str);
        }

    }
}
