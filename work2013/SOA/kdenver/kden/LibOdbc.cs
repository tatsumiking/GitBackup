using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace kden
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
        public Boolean createUseTable()
        {
            OdbcConnection con;
            string sSql;

            con = kdenOpen();
            if (con == null)
            {
                return (false);
            }

            sSql = "DROP TABLE sougi;";
            kdenSqlExecNoError(con, sSql);

            sSql = "CREATE TABLE sougi (id AUTOINCREMENT PRIMARY KEY";
            sSql = sSql + ", regdate VARCHAR(16)"; // 更新日
            sSql = sSql + ", seikyuno VARCHAR(48)"; // 識別番号
            sSql = sSql + ", souke VARCHAR(48)";  // 葬家
            sSql = sSql + ", tuya_date VARCHAR(16)"; // 通夜日 2018/01/01
            sSql = sSql + ", tuya_stime VARCHAR(16)"; // 通夜開始 18:30
            sSql = sSql + ", tuya_etime VARCHAR(16)"; // 通夜終了 19:30
            sSql = sSql + ", kokubetu_date VARCHAR(16)"; // 告別式日 2018/01/02
            sSql = sSql + ", kokubetu_stime VARCHAR(16)"; // 告別式開始 10:30
            sSql = sSql + ", kokubetu_etime VARCHAR(16)"; // 告別式終了 11:30
            sSql = sSql + ", sekoubasyo VARCHAR(96)"; // 施行場所
            sSql = sSql + ", tantou VARCHAR(24)"; // 担当者
            sSql = sSql + ", bikou VARCHAR(96)";
            sSql = sSql + ");";
            kdenSqlExec(con, sSql);

            sSql = "DROP TABLE seikyu;";
            kdenSqlExecNoError(con, sSql);

            sSql = "CREATE TABLE seikyu (id AUTOINCREMENT PRIMARY KEY";
            sSql = sSql + ", regdate VARCHAR(48)"; // 更新日
            sSql = sSql + ", seikyuno VARCHAR(48)"; // 識別番号
            sSql = sSql + ", seikyusakiid INT"; // 請求先ID
            sSql = sSql + ", sasidasininid INT"; // 差出人ID
            sSql = sSql + ", furikomisakiid INT"; // 振込先ID
            sSql = sSql + ", nebiki INT"; // 値引額
            sSql = sSql + ", tax INT"; // 税金
            sSql = sSql + ", sumprice INT"; // 請求合計
            sSql = sSql + ", nouhin VARCHAR(48)"; // 請求書発効日
            sSql = sSql + ", seikyu VARCHAR(48)"; // 納品書発効日
            sSql = sSql + ", bikou VARCHAR(96)";
            sSql = sSql + ");";
            kdenSqlExec(con, sSql);

            sSql = "DROP TABLE hanbai;";
            kdenSqlExecNoError(con, sSql);

            sSql = "CREATE TABLE hanbai (id AUTOINCREMENT PRIMARY KEY";
            sSql = sSql + ", regdate VARCHAR(48)"; // 更新日
            sSql = sSql + ", orderdate VARCHAR(48)"; // 受注日
            sSql = sSql + ", seikyuid INT"; // 請求ID
            sSql = sSql + ", sougiid INT"; // 葬儀ID
            sSql = sSql + ", souke VARCHAR(48)"; // 葬家名
            sSql = sSql + ", kokubetu VARCHAR(48)"; // 告別式
            sSql = sSql + ", name VARCHAR(96)"; // 商品名
            sSql = sSql + ", tanka INT"; // 単価
            sSql = sSql + ", tani VARCHAR(12)"; // 単位
            sSql = sSql + ", kosu INT"; // 個数
            sSql = sSql + ", taxkind VARCHAR(12)"; // 税区分
            sSql = sSql + ", tax INT"; // 消費税
            sSql = sSql + ", price INT"; // 総額
            sSql = sSql + ", seikyusaki VARCHAR(96)"; // 請求先名
            sSql = sSql + ", seikyubusyo VARCHAR(96)"; // 請求部署
            sSql = sSql + ", skskeisyou VARCHAR(16)"; // 請求書敬称
            sSql = sSql + ", skakeisyou VARCHAR(16)"; // 宛名書敬称
            sSql = sSql + ", seikyupost VARCHAR(24)"; // 請求先郵便番号
            sSql = sSql + ", seikyuadrs1 VARCHAR(96)"; // 請求先住所１
            sSql = sSql + ", seikyuadrs2 VARCHAR(96)"; // 請求先住所２
            sSql = sSql + ", seikyutel VARCHAR(48)"; // 請求先電話
            sSql = sSql + ", seikyufax VARCHAR(48)"; // 請求先FAX
            sSql = sSql + ", seikyupay VARCHAR(96)"; // 振込名
            sSql = sSql + ", seikyukubun VARCHAR(48)"; // 請求区分
            sSql = sSql + ", receivedate VARCHAR(48)"; // 入金日
            sSql = sSql + ", receivekind VARCHAR(48)"; // 入金種別
            sSql = sSql + ", vendername VARCHAR(96)"; // 仕入先
            sSql = sSql + ", nafuda1 VARCHAR(96)"; // 名札１
            sSql = sSql + ", nafuda2 VARCHAR(96)"; // 名札２
            sSql = sSql + ", nafuda3 VARCHAR(96)"; // 名札３
            sSql = sSql + ", nafuda4 VARCHAR(96)"; // 名札４
            sSql = sSql + ", nafuda5 VARCHAR(96)"; // 名札５
            sSql = sSql + ", nafuda6 VARCHAR(96)"; // 名札６
            sSql = sSql + ", nafuda7 VARCHAR(96)"; // 名札７
            sSql = sSql + ", nafuda8 VARCHAR(96)"; // 名札８
            sSql = sSql + ", bikou VARCHAR(96)"; // 備考
            sSql = sSql + ");";
            kdenSqlExec(con, sSql);

            sSql = "DROP TABLE syouhin;";
            kdenSqlExecNoError(con, sSql);

            sSql = "CREATE TABLE syouhin (id AUTOINCREMENT PRIMARY KEY";
            sSql = sSql + ", regdate VARCHAR(48)"; // 更新日
            sSql = sSql + ", name VARCHAR(96)"; // 商品名
            sSql = sSql + ", tanka INT"; // 単価
            sSql = sSql + ", tani VARCHAR(12)"; // 単位
            sSql = sSql + ", bunrui VARCHAR(12)"; // 分類
            sSql = sSql + ", taxkind VARCHAR(12)"; // 税区分
            sSql = sSql + ", sortno INT"; // 並び順
            sSql = sSql + ", bikou VARCHAR(48)";
            sSql = sSql + ");";
            kdenSqlExec(con, sSql);

            sSql = "DROP TABLE seikyusaki;";
            kdenSqlExecNoError(con, sSql);

            sSql = "CREATE TABLE seikyusaki (id AUTOINCREMENT PRIMARY KEY";
            sSql = sSql + ", regdate VARCHAR(48)"; // 更新日
            sSql = sSql + ", name VARCHAR(96)"; // 請求先名
            sSql = sSql + ", busyo VARCHAR(96)"; // 請求先部署
            sSql = sSql + ", skeisyou VARCHAR(16)"; // 請求書敬称
            sSql = sSql + ", akeisyou VARCHAR(16)"; // 宛名敬称
            sSql = sSql + ", postno VARCHAR(24)"; // 請求先郵便番号
            sSql = sSql + ", adrs1 VARCHAR(96)"; // 請求先住所１
            sSql = sSql + ", adrs2 VARCHAR(96)"; // 請求先住所２
            sSql = sSql + ", tel VARCHAR(48)"; // 請求先電話
            sSql = sSql + ", fax VARCHAR(48)"; // 請求先FAX
            sSql = sSql + ", payname VARCHAR(96)"; // 振込名
            sSql = sSql + ", bikou VARCHAR(48)";
            sSql = sSql + ");";
            kdenSqlExec(con, sSql);

            sSql = "DROP TABLE sasidasinin;";
            kdenSqlExecNoError(con, sSql);

            sSql = "CREATE TABLE sasidasinin (id AUTOINCREMENT PRIMARY KEY";
            sSql = sSql + ", name VARCHAR(96)"; // 会社名
            sSql = sSql + ", sisetu VARCHAR(96)"; // 施設名
            sSql = sSql + ", postno VARCHAR(24)"; // 郵便番号
            sSql = sSql + ", adrs1 VARCHAR(96)"; // 住所１
            sSql = sSql + ", adrs2 VARCHAR(96)"; // 住所２
            sSql = sSql + ", tel VARCHAR(48)"; // 電話
            sSql = sSql + ", fax VARCHAR(48)"; // FAX
            sSql = sSql + ", bikou VARCHAR(48)";
            sSql = sSql + ");";
            kdenSqlExec(con, sSql);

            sSql = "DROP TABLE furikomisaki;";
            kdenSqlExecNoError(con, sSql);

            sSql = "CREATE TABLE furikomisaki (id AUTOINCREMENT PRIMARY KEY";
            sSql = sSql + ", name VARCHAR(96)"; // 銀行名
            sSql = sSql + ", siten VARCHAR(96)"; // 支店名
            sSql = sSql + ", kind VARCHAR(16)"; // 預金種別
            sSql = sSql + ", bankno VARCHAR(24)"; // 口座番号
            sSql = sSql + ", kana VARCHAR(96)"; // カナ名義
            sSql = sSql + ", hojyo VARCHAR(96)"; // 漢字名義
            sSql = sSql + ", bikou VARCHAR(48)";
            sSql = sSql + ");";
            kdenSqlExec(con, sSql);

            sSql = "DROP TABLE skkubun;";
            kdenSqlExecNoError(con, sSql);

            sSql = "CREATE TABLE skkubun (id AUTOINCREMENT PRIMARY KEY";
            sSql = sSql + ", name VARCHAR(96)"; // 区分名
            sSql = sSql + ", mark VARCHAR(12)"; // マーク
            sSql = sSql + ", bikou VARCHAR(48)";
            sSql = sSql + ");";
            kdenSqlExec(con, sSql);

            kdenClose(con);
            return (true);
        }
        public void initSyouhinRecords(string[] sLine)
        {
            string sRegdate;
            OdbcConnection con;
            string sSql, sInsSql;
            int idx, max;
            string[] sAry;
            string sData;

            DateTime dt = DateTime.Now;
            sRegdate = dt.ToString("yyyy/MM/dd hh:mm");
            con = kdenOpen();
            if (con != null)
            {
                sSql = "INSERT INTO syouhin (";
                sSql = sSql + "regdate";
                sSql = sSql + ",name,tanka,tani";
                sSql = sSql + ",taxkind,bunrui";
                sSql = sSql + ",sortno";
                sSql = sSql + ") VALUES (";

                max = sLine.Length;
                for (idx = 1; idx < max; idx++)
                {
                    sAry = sLine[idx].Split(',');
                    if (6 <= sAry.Length)
                    {
                        sData = "'" + sRegdate + "'," + sLine[idx];
                        sInsSql = sSql + sData + ");";
                        kdenSqlExec(con, sInsSql);
                    }
                }
                kdenClose(con);
            }
        }
        public void initSeikyusakiRecords(string[] sLine)
        {
            string sCrtymdhm;
            OdbcConnection con;
            string sSql, sInsSql;
            int idx, max;
            string[] sAry;

            DateTime dt = DateTime.Now;
            sCrtymdhm = dt.ToString("yyyy/MM/dd HH:mm");

            con = kdenOpen();
            if (con != null)
            {
                sSql = "INSERT INTO seikyusaki (";
                sSql = sSql + "regdate,name,busyo";
                sSql = sSql + ",skeisyou,akeisyou";
                sSql = sSql + ",postno,adrs1,adrs2";
                sSql = sSql + ",tel,fax,payname";
                sSql = sSql + ") VALUES (";

                max = sLine.Length;
                for (idx = 1; idx < max; idx++)
                {
                    sAry = sLine[idx].Split(',');
                    if (8 <= sAry.Length)
                    {
                        sInsSql = sSql + "'"+sCrtymdhm +"',"+sLine[idx] + ");";
                        kdenSqlExec(con, sInsSql);
                    }
                }
                kdenClose(con);
            }
        }
        public void initSasidasininRecords(string[] sLine)
        {
            OdbcConnection con;
            string sSql, sInsSql;
            int idx, max;
            string[] sAry;

            con = kdenOpen();
            if (con != null)
            {
                sSql = "INSERT INTO sasidasinin (";
                sSql = sSql + "name,sisetu";
                sSql = sSql + ",postno,adrs1,adrs2";
                sSql = sSql + ",tel,fax";
                sSql = sSql + ") VALUES (";

                max = sLine.Length;
                for (idx = 1; idx < max; idx++)
                {
                    sAry = sLine[idx].Split(',');
                    if (7 <= sAry.Length)
                    {
                        sInsSql = sSql + sLine[idx] + ");";
                        kdenSqlExec(con, sInsSql);
                    }
                }
                kdenClose(con);
            }
        }
        public void initFurikomisakiRecords(string[] sLine)
        {
            OdbcConnection con;
            string sSql, sInsSql;
            int idx, max;
            string[] sAry;

            con = kdenOpen();
            if (con != null)
            {
                sSql = "INSERT INTO furikomisaki (";
                sSql = sSql + "name,siten,kind";
                sSql = sSql + ",bankno,kana,hojyo";
                sSql = sSql + ") VALUES (";

                max = sLine.Length;
                for (idx = 1; idx < max; idx++)
                {
                    sAry = sLine[idx].Split(',');
                    if (6 <= sAry.Length)
                    {
                        sInsSql = sSql + sLine[idx] + ");";
                        kdenSqlExec(con, sInsSql);
                    }
                }
                kdenClose(con);
            }
        }
        public void initSkkubunRecords(string[] sLine)
        {
            OdbcConnection con;
            string sSql, sInsSql;
            int idx, max;
            string[] sAry;

            con = kdenOpen();
            if (con != null)
            {
                sSql = "INSERT INTO skkubun (";
                sSql = sSql + "name,mark";
                sSql = sSql + ") VALUES (";

                max = sLine.Length;
                for (idx = 1; idx < max; idx++)
                {
                    sAry = sLine[idx].Split(',');
                    if (2 <= sAry.Length)
                    {
                        sInsSql = sSql + sLine[idx] + ");";
                        kdenSqlExec(con, sInsSql);
                    }
                }
                kdenClose(con);
            }
        }
        public void testDataCreate()
        {
            string sInsertSougi;
            int nSougiId;
            string sInsertSeikyu;
            int nSeikyuId;
            string sHanbai;

            sInsertSougi = ",2018/01/01 11:00,00001,吉田家,2018/01/01,18:30,19:30,2018/01/02,09:30,10:30,○○斎場,山田,,";
            nSougiId = insertSougi(sInsertSougi);

            sInsertSeikyu = ",2018/01/02 11:01,00001,1,1,1,0,2800,37800,2018/01/02,,,";
            nSeikyuId = insertSeikyu(sInsertSeikyu);

            sHanbai = ",2018/01/02 11:01,2018/01/02," + nSeikyuId + "," + nSougiId + ",吉田家,2018/01/02";
            sHanbai = sHanbai + ",生花1基16200円,16200,基,1,内税,1200,16200";
            sHanbai = sHanbai + ",株式会社グリーン,ご担当者,様,様,,,,,,カ)グリーン,,,,";
            sHanbai = sHanbai + ",株式会社,グリーン,会長,吉田浩三,,,,,";
            insertHanbai(sHanbai);
            sHanbai = ",2018/01/02 11:02,2018/01/02," + nSeikyuId + "," + nSougiId + ",吉田家,2018/01/02";
            sHanbai = sHanbai + ",生花1基16200円,16200,基,1,内税,1200,16200";
            sHanbai = sHanbai + ",株式会社グリーン,ご担当者,様,様,,,,,,カ)グリーン,別請求,,,";
            sHanbai = sHanbai + ",株式会社,グリーン,専務,佐々木正行,,,,,";
            insertHanbai(sHanbai);
            sHanbai = ",2018/01/02 11:03,2018/01/02," + nSeikyuId + "," + nSougiId + ",吉田家,2018/01/02";
            sHanbai = sHanbai + ",樒盛1基5400円,5400,対,1,内税,400,5400";
            sHanbai = sHanbai + ",株式会社グリーン,ご担当者,様,様,,,,,,カ)グリーン,自治会別扱い,,,";
            sHanbai = sHanbai + ",株式会社,グリーン,常務,田中太郎,,,,,";
            insertHanbai(sHanbai);

            sInsertSeikyu = ",2018/01/02 11:01,00001,2,1,1,0,4800,64800,2018/01/02,,,";
            nSeikyuId = insertSeikyu(sInsertSeikyu);

            sHanbai = ",2018/01/02 11:05,2018/01/02," + nSeikyuId + "," + nSougiId + ",吉田家,2018/01/02";
            sHanbai = sHanbai + ",生花1基16200円,16200,基,1,内税,1200,16200";
            sHanbai = sHanbai + ",株式会社グリーンサービス,生花部,様,様,,,,,,カ)グリーンサービス,,,,";
            sHanbai = sHanbai + ",株式会社,グリーン,社長,吉田正行,,,,,";
            insertHanbai(sHanbai);
            sHanbai = ",2018/01/02 11:06,2018/01/02," + nSeikyuId + "," + nSougiId + ",吉田家,2018/01/02";
            sHanbai = sHanbai + ",樒盛1基5400円,5400,対,1,内税,400,5400";
            sHanbai = sHanbai + ",株式会社グリーンサービス,生花部,様,様,,,,,,カ)グリーンサービス,別請求,,,";
            sHanbai = sHanbai + ",株式会社,グリーン,常務,田中正行,,,,,";
            insertHanbai(sHanbai);
            sHanbai = ",2018/01/02 11:07,2018/01/02," + nSeikyuId + "," + nSougiId + ",吉田家,2018/01/02";
            sHanbai = sHanbai + ",花馬車1基43200円,43200,対,1,内税,3200,43200";
            sHanbai = sHanbai + ",株式会社グリーンサービス,生花部,様,様,,,,,,カ)グリーンサービス,別請求,,,";
            sHanbai = sHanbai + ",株式会社,グリーン,主事,佐藤次郎,,,,,";
            insertHanbai(sHanbai);

            sInsertSougi = ",2018/01/04 11:08,00002,山本家,2018/01/05,18:00,19:00,2018/01/06,09:00,10:00,××斎場,山田,,";
            nSougiId = insertSougi(sInsertSougi);

            sInsertSeikyu = ",2018/01/06 11:09,00002,1,1,1,0,3600,49600,2018/01/06,,,";
            nSeikyuId = insertSeikyu(sInsertSeikyu);

            sHanbai = ",2018/01/06 11:10,2018/01/06," + nSeikyuId + "," + nSougiId + ",山本家,2018/01/06";
            sHanbai = sHanbai + ",生花1基16200円,16200,基,1,内税,1200,16200";
            sHanbai = sHanbai + ",株式会社グリーン,ご担当者,様,様,,,,,,カ)グリーン,,,,";
            sHanbai = sHanbai + ",株式会社,レッド,社長,田中直行,,,,,";
            insertHanbai(sHanbai);
            sHanbai = ",2018/01/06 11:11,2018/01/06," + nSeikyuId + "," + nSougiId + ",山本家,2018/01/06";
            sHanbai = sHanbai + ",生花1基16200円,16200,基,1,内税,1200,16200";
            sHanbai = sHanbai + ",株式会社グリーン,ご担当者,様,様,,,,,,カ)グリーン,,,,";
            sHanbai = sHanbai + ",株式会社,レッド,主任,吉田直行,,,,,";
            insertHanbai(sHanbai);
            sHanbai = ",2018/01/06 11:12,2018/01/06," + nSeikyuId + "," + nSougiId + ",山本家,2018/01/06";
            sHanbai = sHanbai + ",生花1基16200円,16200,基,1,内税,1200,16200";
            sHanbai = sHanbai + ",株式会社グリーン,ご担当者,様,様,,,,,,カ)グリーン,,,,";
            sHanbai = sHanbai + ",株式会社,レッド,課長,吉永直行,,,,,";
            insertHanbai(sHanbai);

        }
        public int insertSougi(string sInsertSougi)
        {
            int nId;
            string[] ary;
            string sSql;

            nId = 0;
            ary = sInsertSougi.Split(',');
            sSql = "INSERT INTO sougi (";
            sSql = sSql + "regdate,seikyuno, souke";
            sSql = sSql + ",tuya_date,tuya_stime,tuya_etime";
            sSql = sSql + ",kokubetu_date,kokubetu_stime,kokubetu_etime";
            sSql = sSql + ",sekoubasyo,tantou,bikou";
            sSql = sSql + ") VALUES (";
            sSql = sSql + "'" + ary[1] + "','" + ary[2] + "','" + ary[3] + "'";
            sSql = sSql + ",'"+ary[4]+"','"+ary[5]+"','"+ary[6]+"'";
            sSql = sSql + ",'"+ary[7]+"','"+ary[8]+"','"+ary[9]+"'";
            sSql = sSql + ",'" + ary[10] + "','" + ary[11] + "','" + ary[12] + "'";
            sSql = sSql + ");";
            nId = insertRecordSql(sSql);
            return (nId);
        }
        public int insertSeikyu(string sInsertSeikyu)
        {
            int nId;
            string[] ary;
            string sSql;

            nId = 0;
            ary = sInsertSeikyu.Split(',');
            sSql = "INSERT INTO seikyu (";
            sSql = sSql + "regdate,seikyuno";
            sSql = sSql + ",seikyusakiid,sasidasininid";
            sSql = sSql + ",furikomisakiid";
            sSql = sSql + ",nebiki,tax,sumprice";
            sSql = sSql + ",nouhin,seikyu,bikou";
            sSql = sSql + ") VALUES (";
            sSql = sSql + "'" + ary[1] + "','" + ary[2] + "'";
            sSql = sSql + "," + ary[3] + "," + ary[4] + "";
            sSql = sSql + "," + ary[5] + "";
            sSql = sSql + "," + ary[6] + "," + ary[7] + "," + ary[8] + "";
            sSql = sSql + ",'" + ary[9] + "','" + ary[10] + "','" + ary[11] + "'";
            sSql = sSql + ");";
            nId = insertRecordSql(sSql);
            return (nId);
        }
        public int insertHanbai(string sInsertHanbai)
        {
            int nId;
            string[] ary;
            string sSql;

            nId = 0;
            ary = sInsertHanbai.Split(',');
            sSql = "INSERT INTO hanbai (";
            sSql = sSql + "regdate,orderdate";
            sSql = sSql + ",seikyuid,sougiid";
            sSql = sSql + ",souke,kokubetu,name";
            sSql = sSql + ",tanka,tani,kosu";
            sSql = sSql + ",taxkind,tax,price";
            sSql = sSql + ",seikyusaki,seikyubusyo";
            sSql = sSql + ",skskeisyou,skakeisyou";
            sSql = sSql + ",seikyupost,seikyuadrs1,seikyuadrs2";
            sSql = sSql + ",seikyutel,seikyufax";
            sSql = sSql + ",seikyupay,seikyukubun";
            sSql = sSql + ",receivedate,receivekind,vendername";
            sSql = sSql + ",nafuda1,nafuda2,nafuda3,nafuda4";
            sSql = sSql + ",nafuda5,nafuda6,nafuda7,nafuda8";
            sSql = sSql + ",bikou";
            sSql = sSql + ") VALUES (";
            sSql = sSql + "'" + ary[1] + "','" + ary[2] + "'";
            sSql = sSql + "," + ary[3] + "," + ary[4] + "";
            sSql = sSql + ",'" + ary[5] + "','" + ary[6] + "','" + ary[7] + "'";
            sSql = sSql + "," + ary[8] + ",'" + ary[9] + "'," + ary[10] + "";
            sSql = sSql + ",'" + ary[11] + "'," + ary[12] + "," + ary[13] + "";
            sSql = sSql + ",'" + ary[14] + "','" + ary[15] + "'";
            sSql = sSql + ",'" + ary[16] + "','" + ary[17] + "'";
            sSql = sSql + ",'" + ary[18] + "','" + ary[19] + "','" + ary[20] + "'";
            sSql = sSql + ",'" + ary[21] + "','" + ary[22] + "'";
            sSql = sSql + ",'" + ary[23] + "','" + ary[24] + "'";
            sSql = sSql + ",'" + ary[25] + "','" + ary[26] + "','" + ary[27] + "'";
            sSql = sSql + ",'" + ary[28] + "','" + ary[29] + "','" + ary[30] + "','" + ary[31] + "'";
            sSql = sSql + ",'" + ary[32] + "','" + ary[33] + "','" + ary[34] + "','" + ary[35] + "'";
            sSql = sSql + ",'" + ary[36] + "'";
            sSql = sSql + ");";
            nId = insertRecordSql(sSql);
            return (nId);
        }
        public int insertSyouhin(string sInsertSyouhin)
        {
            int nId;
            string[] ary;
            string sSql;

            nId = 0;
            ary = sInsertSyouhin.Split(',');
            sSql = "INSERT INTO syouhin (";
            sSql = sSql + "regdate,name";
            sSql = sSql + ",tanka,tani";
            sSql = sSql + ",bunrui,taxkind";
            sSql = sSql + ",sortno,bikou";
            sSql = sSql + ") VALUES (";
            sSql = sSql + "'" + ary[1] + "','" + ary[2] + "'";
            sSql = sSql + "," + ary[3] + ",'" + ary[4] + "'";
            sSql = sSql + ",'" + ary[5] + "','" + ary[6] + "'";
            sSql = sSql + "," + ary[7] + ",'" + ary[8] + "'";
            sSql = sSql + ");";
            nId = insertRecordSql(sSql);
            return (nId);
        }
        public int insertSeikyusaki(string sInsertSeikyusaki)
        {
            int nId;
            string[] ary;
            string sSql;

            nId = 0;
            ary = sInsertSeikyusaki.Split(',');
            sSql = "INSERT INTO seikyusaki (";
            sSql = sSql + "regdate,name,busyo";
            sSql = sSql + ",skeisyou,akeisyou";
            sSql = sSql + ",postno,adrs1,adrs2";
            sSql = sSql + ",tel,fax";
            sSql = sSql + ",payname,bikou";
            sSql = sSql + ") VALUES (";
            sSql = sSql + "'" + ary[1] + "','" + ary[2] + "','" + ary[3] + "'";
            sSql = sSql + ",'" + ary[4] + "','" + ary[5] + "'";
            sSql = sSql + ",'" + ary[6] + "','" + ary[7] + "','" + ary[8] + "'";
            sSql = sSql + ",'" + ary[9] + "','" + ary[10] + "'";
            sSql = sSql + ",'" + ary[11] + "','" + ary[12] + "'";
            sSql = sSql + ");";
            nId = insertRecordSql(sSql);
            return (nId);
        }
        public int insertSasidasinin(string sInsertSasidasinin)
        {
            int nId;
            string[] ary;
            string sSql;

            nId = 0;
            ary = sInsertSasidasinin.Split(',');
            sSql = "INSERT INTO sasidasinin (";
            sSql = sSql + ",name,sisetu";
            sSql = sSql + ",postno,adrs1,adrs2";
            sSql = sSql + ",tel,fax";
            sSql = sSql + ",bikou";
            sSql = sSql + ") VALUES (";
            sSql = sSql + ",'" + ary[1] + "','" + ary[2] + "'";
            sSql = sSql + ",'" + ary[3] + "','" + ary[4] + "','" + ary[5] + "'";
            sSql = sSql + ",'" + ary[6] + "','" + ary[7] + "'";
            sSql = sSql + ",'" + ary[8] + "'";
            sSql = sSql + ");";
            nId = insertRecordSql(sSql);
            return (nId);
        }
        public int insertFurikomisaki(string sInsertFurikomisaki)
        {
            int nId;
            string[] ary;
            string sSql;

            nId = 0;
            ary = sInsertFurikomisaki.Split(',');
            sSql = "INSERT INTO furikomisaki (";
            sSql = sSql + ",name,siten";
            sSql = sSql + ",kind,bankno";
            sSql = sSql + ",kana,hojyo";
            sSql = sSql + ",bikou";
            sSql = sSql + ") VALUES (";
            sSql = sSql + ",'" + ary[1] + "','" + ary[2] + "'";
            sSql = sSql + ",'" + ary[3] + "','" + ary[4] + "'";
            sSql = sSql + ",'" + ary[5] + "','" + ary[6] + "'";
            sSql = sSql + ",'" + ary[7] + "'";
            sSql = sSql + ");";
            nId = insertRecordSql(sSql);
            return (nId);
        }
        public int insertSkkubun(string sInsertSkkubun)
        {
            int nId;
            string[] ary;
            string sSql;

            nId = 0;
            ary = sInsertSkkubun.Split(',');
            sSql = "INSERT INTO skkubun (";
            sSql = sSql + ",name,mark";
            sSql = sSql + ",bikou";
            sSql = sSql + ") VALUES (";
            sSql = sSql + ",'" + ary[1] + "','" + ary[2] + "'";
            sSql = sSql + ",'" + ary[3] + "'";
            sSql = sSql + ");";
            nId = insertRecordSql(sSql);
            return (nId);
        }
        public int insertRecordSql(string sSql)
        {
            int nId;
            string sSqlGet;
            OdbcConnection con;
            OdbcCommand cmd;

            nId = 0;

            // 最後に挿入したIDを取得する
            sSqlGet = "SELECT @@IDENTITY AS 'Identity'";
            // sSqlGet = "SELECT id FROM denpyou WHERE id = SCOPE_IDENTITY();";

            con = kdenOpen();
            if (con != null)
            {
                kdenSqlExec(con, sSql);
                try
                {
                    cmd = new OdbcCommand(sSqlGet, con);
                    nId = (int)cmd.ExecuteScalar();
                    cmd.Dispose();
                }
                catch (Exception ex)
                {
                    //MessageBox.Show(ex.ToString(), "確認", MessageBoxButton.OK);
                }
                kdenClose(con);
            }
            return (nId);
        }
        public string getSougiRecord(int nId)
        {
            string sSql;
            List<string> listRet;
            string sRetStr;

            if (nId == 0)
            {
                return ("");
            }
            sRetStr = "";
            sSql = "SELECT * FROM sougi";
            sSql = sSql + " WHERE (";
            sSql = sSql + "id = " + nId;
            sSql = sSql + ");";
            listRet = getRecord(sSql, 13);
            if (listRet.Count != 0)
            {
                sRetStr = listRet[0];
            }
            return (sRetStr);
        }
        public string getSeikyuRecord(int nId)
        {
            string sSql;
            List<string> listRet;
            string sRetStr;

            if (nId == 0)
            {
                return ("");
            }
            sRetStr = "";
            sSql = "SELECT * FROM seikyu";
            sSql = sSql + " WHERE (";
            sSql = sSql + "id = " + nId;
            sSql = sSql + ");";
            listRet = getRecord(sSql, 12);
            if (listRet.Count != 0)
            {
                sRetStr = listRet[0];
            }
            return (sRetStr);
        }
        public string getHanbaiRecord(int nId)
        {
            string sSql;
            List<string> listRet;
            string sRetStr;

            if (nId == 0)
            {
                return("");
            }
            sRetStr = "";
            sSql = "SELECT * FROM hanbai";
            sSql = sSql + " WHERE (";
            sSql = sSql + "id = " + nId;
            sSql = sSql + ");";
            listRet = getRecord(sSql, 37);
            if (listRet.Count != 0)
            {
                sRetStr = listRet[0];
            }
            return (sRetStr);
        }
        public string getSeikyusakiRecord(int nId)
        {
            string sSql;
            List<string> listRet;
            string sRetStr;

            if (nId == 0)
            {
                return ("");
            }
            sRetStr = "";
            sSql = "SELECT * FROM seikyusaki";
            sSql = sSql + " WHERE (";
            sSql = sSql + "id = " + nId;
            sSql = sSql + ");";
            listRet = getRecord(sSql, 13);
            if (listRet.Count != 0)
            {
                sRetStr = listRet[0];
            }
            return (sRetStr);
        }
        public string getSasidasininRecord(int nId)
        {
            string sSql;
            List<string> listRet;
            string sRetStr;

            if (nId == 0)
            {
                return ("");
            }
            sRetStr = "";
            sSql = "SELECT * FROM sasidasinin";
            sSql = sSql + " WHERE (";
            sSql = sSql + "id = " + nId;
            sSql = sSql + ");";
            listRet = getRecord(sSql, 9);
            if (listRet.Count != 0)
            {
                sRetStr = listRet[0];
            }
            return (sRetStr);
        }
        public string getFurikomisakiRecord(int nId)
        {
            string sSql;
            List<string> listRet;
            string sRetStr;

            if (nId == 0)
            {
                return ("");
            }
            sRetStr = "";
            sSql = "SELECT * FROM furikomisaki";
            sSql = sSql + " WHERE (";
            sSql = sSql + "id = " + nId;
            sSql = sSql + ");";
            listRet = getRecord(sSql, 8);
            if (listRet.Count != 0)
            {
                sRetStr = listRet[0];
            }
            return (sRetStr);
        }
        public List<string> getSougiList(string sConditions)
        {
            string sSql;
            List<string> listRet;

            sSql = "SELECT * FROM sougi";
            if (sConditions == "")
            {
                sSql = sSql + ";";
            }else{
                sSql = sSql + " WHERE (";
                sSql = sSql + sConditions;
                sSql = sSql + ");";
            }
            listRet = getRecord(sSql, 13);
            return (listRet);
        }
        public List<string> getSeikyuList(string sConditions)
        {
            string sSql;
            List<string> listRet;

            sSql = "SELECT * FROM seikyu";
            if (sConditions == "")
            {
                sSql = sSql + ";";
            }
            else
            {
                sSql = sSql + " WHERE (";
                sSql = sSql + sConditions;
                sSql = sSql + ");";
            }
            listRet = getRecord(sSql, 12);
            return (listRet);
        }
        public List<string> getHanbaiList(string sConditions)
        {
            string sSql;
            List<string> listRet;

            sSql = "SELECT * FROM hanbai";
            if (sConditions == "")
            {
                sSql = sSql + ";";
            }else{
                sSql = sSql + " WHERE (";
                sSql = sSql + sConditions;
                sSql = sSql + ");";
            }
            listRet = getRecord(sSql, 37);
            return (listRet);
        }
        public List<string> getOderHanbaiList(string sConditions, string oder)
        {
            string sSql;
            List<string> listRet;

            sSql = "SELECT * FROM hanbai";
            if (sConditions != "")
            {
                sSql = sSql + " WHERE (";
                sSql = sSql + sConditions;
                sSql = sSql + ")";
            }
            if (oder != "")
            {
                sSql = sSql + " ORDER BY ";
                sSql = sSql + oder;
                sSql = sSql + "";
            }
            sSql = sSql + ";";
            listRet = getRecord(sSql, 37);
            return (listRet);
        }
        public List<string> getSyouhinList(string sConditions)
        {
            string sSql;
            List<string> listRet;

            sSql = "SELECT * FROM syouhin";
            if (sConditions == "")
            {
                sSql = sSql + ";";
            }
            else
            {
                sSql = sSql + " WHERE (";
                sSql = sSql + sConditions;
                sSql = sSql + ");";
            }
            listRet = getRecord(sSql, 9);
            return (listRet);
        }
        public List<string> getSeikyusakiList(string sConditions)
        {
            string sSql;
            List<string> listRet;

            sSql = "SELECT * FROM seikyusaki";
            if (sConditions != "")
            {
                sSql = sSql + " WHERE (";
                sSql = sSql + sConditions;
                sSql = sSql + ")";
            }
            sSql = sSql + ";";
            listRet = getRecord(sSql, 13);
            return (listRet);
        }
        public List<string> getSeikyusakiRegOderList(string sConditions)
        {
            string sSql;
            List<string> listRet;

            sSql = "SELECT * FROM seikyusaki";
            if (sConditions != "")
            {
                sSql = sSql + " WHERE (";
                sSql = sSql + sConditions;
                sSql = sSql + ")";
            }
            sSql = sSql + " ORDER BY ";
            sSql = sSql + "regdate DESC";
            sSql = sSql + ";";
            listRet = getRecord(sSql, 13);
            return (listRet);
        }
        public List<string> getSasidasininList(string sConditions)
        {
            string sSql;
            List<string> listRet;

            sSql = "SELECT * FROM sasidasinin";
            if (sConditions != "")
            {
                sSql = sSql + " WHERE (";
                sSql = sSql + sConditions;
                sSql = sSql + ")";
            }
            sSql = sSql + ";";
            listRet = getRecord(sSql, 9);
            return (listRet);
        }
        public List<string> getFurikomisakiList(string sConditions)
        {
            string sSql;
            List<string> listRet;

            sSql = "SELECT * FROM furikomisaki";
            if (sConditions != "")
            {
                sSql = sSql + " WHERE (";
                sSql = sSql + sConditions;
                sSql = sSql + ")";
            }
            sSql = sSql + ";";
            listRet = getRecord(sSql, 8);
            return (listRet);
        }
        public List<string> getSkkubunList(string sConditions)
        {
            string sSql;
            List<string> listRet;

            sSql = "SELECT * FROM skkubun";
            if (sConditions != "")
            {
                sSql = sSql + " WHERE (";
                sSql = sSql + sConditions;
                sSql = sSql + ")";
            }
            sSql = sSql + ";";
            listRet = getRecord(sSql, 4);
            return (listRet);
        }
        public List<string> getRecord(string sSql, int max)
        {
            string sStr, sRecodeStr;
            List<string> listRet;
            OdbcConnection con;
            OdbcCommand cmd;
            OdbcDataReader reader;
            int idx;

            listRet = new List<string>();
            con = kdenOpen();
            if (con != null)
            {
                try
                {
                    cmd = new OdbcCommand(sSql, con);
                    reader = cmd.ExecuteReader();
                    while (reader.Read())
                    {
                        sRecodeStr = "";
                        for (idx = 0; idx < max; idx++)
                        {
                            sStr = getReaderString(reader, idx);
                            if (idx == 0)
                            {
                                sRecodeStr = sStr;
                            }
                            else
                            {
                                sRecodeStr = sRecodeStr + "," + sStr;
                            }
                        }
                        listRet.Add(sRecodeStr);
                    }
                }
                catch (Exception ex)
                {
                    m_sMsg = "kdenのデータ取得に失敗しました";
                    m_sLog = ex.ToString();
                    MessageBox.Show(m_sMsg, "確認", MessageBoxButton.OK);
                    listRet.Add("");
                    return (listRet);
                }
                kdenClose(con);
            }
            return (listRet);
        }
        public void updateSougi(string sRecord)
        {
            string[] sAry;
            string sSql;

            sAry = sRecord.Split(',');
            sSql = "UPDATE sougi SET ";
            sSql = sSql + "regdate='" + sAry[1] + "'";
            sSql = sSql + ",seikyuno='" + sAry[2] + "'";
            sSql = sSql + ",souke='" + sAry[3] + "'";
            sSql = sSql + ",tuya_date='" + sAry[4] + "'";
            sSql = sSql + ",tuya_stime='" + sAry[5] + "'";
            sSql = sSql + ",tuya_etime='" + sAry[6] + "'";
            sSql = sSql + ",kokubetu_date='" + sAry[7] + "'";
            sSql = sSql + ",kokubetu_stime='" + sAry[8] + "'";
            sSql = sSql + ",kokubetu_etime='" + sAry[9] + "'";
            sSql = sSql + ",sekoubasyo='" + sAry[10] + "'";
            sSql = sSql + ",tantou='" + sAry[11] + "'";
            sSql = sSql + ",bikou='" + sAry[12] + "'";
            sSql = sSql + " WHERE ( ";
            sSql = sSql + "id=" + sAry[0];
            sSql = sSql + " );";
            execSql(sSql);
        }
        public void updateSeikyu(string sRecord)
        {
            string[] sAry;
            string sSql;

            sAry = sRecord.Split(',');
            sSql = "UPDATE seikyu SET ";
            sSql = sSql + "regdate='" + sAry[1] + "'";
            sSql = sSql + ",seikyuno='" + sAry[2] + "'";
            sSql = sSql + ",seikyusakiid=" + sAry[3] + "";
            sSql = sSql + ",sasidasininid=" + sAry[4] + "";
            sSql = sSql + ",furikomisakiid=" + sAry[5] + "";
            sSql = sSql + ",nebiki=" + sAry[6] + "";
            sSql = sSql + ",tax=" + sAry[7] + "";
            sSql = sSql + ",sumprice=" + sAry[8] + "";
            sSql = sSql + ",nouhin='" + sAry[9] + "'";
            sSql = sSql + ",seikyu='" + sAry[10] + "'";
            sSql = sSql + ",bikou='" + sAry[11] + "'";
            sSql = sSql + " WHERE ( ";
            sSql = sSql + "id=" + sAry[0];
            sSql = sSql + " );";
            execSql(sSql);
        }
        public void updateHanbai(string sRecord)
        {
            string[] sAry;
            string sSql;

            sAry = sRecord.Split(',');
            sSql = "UPDATE hanbai SET ";
            sSql = sSql + "regdate='" + sAry[1] + "'";
            sSql = sSql + ",orderdate='" + sAry[2] + "'";
            sSql = sSql + ",seikyuid='" + sAry[3] + "'";
            sSql = sSql + ",sougiid='" + sAry[4] + "'";
            sSql = sSql + ",souke='" + sAry[5] + "'";
            sSql = sSql + ",kokubetu='" + sAry[6] + "'";
            sSql = sSql + ",name='" + sAry[7] + "'";
            sSql = sSql + ",tanka='" + sAry[8] + "'";
            sSql = sSql + ",tani='" + sAry[9] + "'";
            sSql = sSql + ",kosu='" + sAry[10] + "'";
            sSql = sSql + ",taxkind='" + sAry[11] + "'";
            sSql = sSql + ",tax='" + sAry[12] + "'";
            sSql = sSql + ",price='" + sAry[13] + "'";
            sSql = sSql + ",seikyusaki='" + sAry[14] + "'";
            sSql = sSql + ",seikyubusyo='" + sAry[15] + "'";
            sSql = sSql + ",skskeisyou='" + sAry[16] + "'";
            sSql = sSql + ",skakeisyou='" + sAry[17] + "'";
            sSql = sSql + ",seikyupost='" + sAry[18] + "'";
            sSql = sSql + ",seikyuadrs1='" + sAry[19] + "'";
            sSql = sSql + ",seikyuadrs2='" + sAry[20] + "'";
            sSql = sSql + ",seikyutel='" + sAry[21] + "'";
            sSql = sSql + ",seikyufax='" + sAry[22] + "'";
            sSql = sSql + ",seikyupay='" + sAry[23] + "'";
            sSql = sSql + ",seikyukubun='" + sAry[24] + "'";
            sSql = sSql + ",receivedate='" + sAry[25] + "'";
            sSql = sSql + ",receivekind='" + sAry[26] + "'";
            sSql = sSql + ",vendername='" + sAry[27] + "'";
            sSql = sSql + ",nafuda1='" + sAry[28] + "'";
            sSql = sSql + ",nafuda2='" + sAry[29] + "'";
            sSql = sSql + ",nafuda3='" + sAry[30] + "'";
            sSql = sSql + ",nafuda4='" + sAry[31] + "'";
            sSql = sSql + ",nafuda5='" + sAry[32] + "'";
            sSql = sSql + ",nafuda6='" + sAry[33] + "'";
            sSql = sSql + ",nafuda7='" + sAry[34] + "'";
            sSql = sSql + ",nafuda8='" + sAry[35] + "'";
            sSql = sSql + ",bikou='" + sAry[36] + "'";
            sSql = sSql + " WHERE ( ";
            sSql = sSql + "id=" + sAry[0];
            sSql = sSql + " );";
            execSql(sSql);
        }
        public void updateSyouhin(string sRecord)
        {
            string[] sAry;
            string sSql;

            sAry = sRecord.Split(',');
            sSql = "UPDATE syouhin SET ";
            sSql = sSql + "regdate='" + sAry[1] + "'";
            sSql = sSql + ",name='" + sAry[2] + "'";
            sSql = sSql + ",tanka=" + sAry[3] + "";
            sSql = sSql + ",tani='" + sAry[4] + "'";
            sSql = sSql + ",bunrui='" + sAry[5] + "'";
            sSql = sSql + ",taxkind='" + sAry[6] + "'";
            sSql = sSql + ",sortno=" + sAry[7] + "";
            sSql = sSql + ",bikou='" + sAry[8] + "'";
            sSql = sSql + " WHERE ( ";
            sSql = sSql + "id=" + sAry[0];
            sSql = sSql + " );";
            execSql(sSql);
        }
        public void updateSeikyusakiRegdate(int sRecordId)
        {
            string sCrtymdhm;
            string sSql;

            DateTime dt = DateTime.Now;
            sCrtymdhm = dt.ToString("yyyy/MM/dd HH:mm");

            sSql = "UPDATE seikyusaki SET ";
            sSql = sSql + "regdate='" + sCrtymdhm + "'";
            sSql = sSql + " WHERE ( ";
            sSql = sSql + "id=" + sRecordId;
            sSql = sSql + " );";
            execSql(sSql);
        }
        public void updateSeikyusaki(string sRecord)
        {
            string[] sAry;
            string sSql;

            sAry = sRecord.Split(',');
            sSql = "UPDATE seikyusaki SET ";
            sSql = sSql + "regdate='" + sAry[1] + "'";
            sSql = sSql + ",name='" + sAry[2] + "'";
            sSql = sSql + ",busyo='" + sAry[3] + "'";
            sSql = sSql + ",skeisyou='" + sAry[4] + "'";
            sSql = sSql + ",akeisyou='" + sAry[5] + "'";
            sSql = sSql + ",postno='" + sAry[6] + "'";
            sSql = sSql + ",adrs1='" + sAry[7] + "'";
            sSql = sSql + ",adrs2='" + sAry[8] + "'";
            sSql = sSql + ",tel='" + sAry[9] + "'";
            sSql = sSql + ",fax='" + sAry[10] + "'";
            sSql = sSql + ",payname='" + sAry[11] + "'";
            sSql = sSql + ",bikou='" + sAry[12] + "'";
            sSql = sSql + " WHERE ( ";
            sSql = sSql + "id=" + sAry[0];
            sSql = sSql + " );";
            execSql(sSql);
        }
        public void updateSasidasinin(string sRecord)
        {
            string[] sAry;
            string sSql;

            sAry = sRecord.Split(',');
            sSql = "UPDATE sasidasinin SET ";
            sSql = sSql + "name='" + sAry[1] + "'";
            sSql = sSql + ",sisetu='" + sAry[2] + "'";
            sSql = sSql + ",postno='" + sAry[3] + "'";
            sSql = sSql + ",adrs1='" + sAry[4] + "'";
            sSql = sSql + ",adrs2='" + sAry[5] + "'";
            sSql = sSql + ",tel='" + sAry[6] + "'";
            sSql = sSql + ",fax='" + sAry[7] + "'";
            sSql = sSql + ",bikou='" + sAry[8] + "'";
            sSql = sSql + " WHERE ( ";
            sSql = sSql + "id=" + sAry[0];
            sSql = sSql + " );";
            execSql(sSql);
        }
        public void updateFurikomisaki(string sRecord)
        {
            string[] sAry;
            string sSql;

            sAry = sRecord.Split(',');
            sSql = "UPDATE furikomisaki SET ";
            sSql = sSql + "name='" + sAry[1] + "'";
            sSql = sSql + ",siten='" + sAry[2] + "'";
            sSql = sSql + ",kind='" + sAry[3] + "'";
            sSql = sSql + ",bankno='" + sAry[4] + "'";
            sSql = sSql + ",kana='" + sAry[5] + "'";
            sSql = sSql + ",hojyo='" + sAry[6] + "'";
            sSql = sSql + ",bikou='" + sAry[7] + "'";
            sSql = sSql + " WHERE ( ";
            sSql = sSql + "id=" + sAry[0];
            sSql = sSql + " );";
            execSql(sSql);
        }
        public void updateSkkubun(string sRecord)
        {
            string[] sAry;
            string sSql;

            sAry = sRecord.Split(',');
            sSql = "UPDATE skkubun SET ";
            sSql = sSql + "name='" + sAry[1] + "'";
            sSql = sSql + ",mark='" + sAry[2] + "'";
            sSql = sSql + ",bikou='" + sAry[3] + "'";
            sSql = sSql + " WHERE ( ";
            sSql = sSql + "id=" + sAry[0];
            sSql = sSql + " );";
            execSql(sSql);
        }
        public void deleteSougi(int nId)
        {
            string sSql;

            sSql = "DELETE FROM sougi WHERE id = " + nId + ";";
            execSql(sSql);
        }
        public void deleteSeikyu(int nId)
        {
            string sSql;

            sSql = "DELETE FROM seikyu WHERE id = " + nId + ";";
            execSql(sSql);
        }
        public void deleteHanbai(int nId)
        {
            string sSql;

            sSql = "DELETE FROM hanbai WHERE id = " + nId + ";";
            execSql(sSql);
        }
        public void deleteSyouhin(int nId)
        {
            string sSql;

            sSql = "DELETE FROM syouhin WHERE id = " + nId + ";";
            execSql(sSql);
        }
        public void deleteSeikyusaki(int nId)
        {
            string sSql;

            sSql = "DELETE FROM seikyusaki WHERE id = " + nId + ";";
            execSql(sSql);
        }
        public void deleteSasidasinin(int nId)
        {
            string sSql;

            sSql = "DELETE FROM sasidasinin WHERE id = " + nId + ";";
            execSql(sSql);
        }
        public void deleteFurikomisaki(int nId)
        {
            string sSql;

            sSql = "DELETE FROM furikomisaki WHERE id = " + nId + ";";
            execSql(sSql);
        }
        public void deleteSkkubun(int nId)
        {
            string sSql;

            sSql = "DELETE FROM skkubun WHERE id = " + nId + ";";
            execSql(sSql);
        }
        private void execSql(string sSql)
        {
            OdbcConnection con;

            con = kdenOpen();
            if (con != null)
            {
                kdenSqlExec(con, sSql);
                kdenClose(con);
            }
        }
        private OdbcConnection kdenOpen()
        {
            string sCnct;
            OdbcConnection con;

            try
            {
                sCnct = "Driver={Microsoft Access Driver (*.mdb)};DBQ=" + m_sExecPath + "\\kden.mdb";
                con = new OdbcConnection(sCnct);
                con.Open();
            }
            catch (Exception ex)
            {
                m_sMsg = "kden.mdbを使用したデータベースに接続できません";
                m_sLog = ex.ToString();
                MessageBox.Show(m_sMsg, "確認", MessageBoxButton.OK);
                return (null);
            }
            return (con);
        }
        private string kdenSqlExec(OdbcConnection con, string sSql)
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
        private string kdenSqlExecNoError(OdbcConnection con, string sSql)
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
        private string kdenClose(OdbcConnection con)
        {
            try
            {
                con.Close();
                con.Dispose();
            }
            catch (Exception ex)
            {
                m_sMsg = "kdenに切断できません\n" + ex.ToString();
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
