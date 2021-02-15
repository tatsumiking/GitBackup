using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.IO.Compression;
using System.Collections;
using System.Net;
using System.Threading;

// プロパティ　YUUKAEN　CALFO　SOA

namespace Ssd
{
    public partial class SsdForm : Form
    {
        String m_strFtpRoot;
        String m_strFtpID;
        String m_strFtpPW;
        int m_nTimmerSpan;
        String m_strDefAddURL;
        String m_strDefPath;
        String m_strEnvPath;
        ArrayList m_arryUserName = new ArrayList();
        int[] m_arryIntCheckSum;
        int m_nUserMax;
        int m_nXDot, m_nYDot, m_nXByte;

        public SsdForm()
        {
            InitializeComponent();
            // yuukaen
            //m_strFtpRoot = "ftp://ftp.youkaen-hikkoubu.jp/www/";
            //m_strFtpID = "youkaen-mprtmlt";  // yuukaen
            //m_strFtpPW = "ValfoTlmNea";    // yuukaen
            // c-alfo
            m_strFtpRoot = "ftp://sv58.wadax.ne.jp/public_html/";
            m_strFtpID = "c-alfo-com";  // c-alfo
            m_strFtpPW = "VTqR7aAQ";    // c-alfo
            // soa-co
            //m_strFtpRoot = "ftp://www.soa-co.com/public_html/";
            //m_strFtpID = "soa-co";  // soa-co
            //m_strFtpPW = "z9wr71a5";    // soa-co
        }

        public void TimerStart()
        {
            timer1.Start();
        }

        private void SsdForm_Load(object sender, EventArgs e)
        {
            Directory.CreateDirectory("c:\\UsesProgram");
            Directory.CreateDirectory("c:\\UsesProgram\\nsd");
            m_strEnvPath = "c:\\UsesProgram\\nsd\\";
            InitEnv();
            InitCheckPathList();
            InitDeleteOldFile();
            timer1.Interval = m_nTimmerSpan * 1000;
            timer1.Enabled = true;
            timer1.Start();
        }
        private void btnSpan_Click(object sender, EventArgs e)
        {
            m_nTimmerSpan = int.Parse(textSpan.Text);
        }


        private void btnEnvSet_Click(object sender, EventArgs e)
        {
            timer1.Stop();
            EnvForm fEnv = new EnvForm();
            fEnv.m_cSsdForm = this;
            fEnv.m_strFtpRoot = m_strFtpRoot;
            fEnv.m_strFtpID = m_strFtpID;
            fEnv.m_strFtpPW = m_strFtpPW;
            fEnv.m_nTimmerSpan = m_nTimmerSpan;
            fEnv.m_strDefAddURL = m_strDefAddURL;
            fEnv.m_strDefPath = m_strDefPath;
            fEnv.m_strEnvPath = m_strEnvPath;
            fEnv.Show();
        }

        private void btnEnd_Click(object sender, EventArgs e)
        {
            Environment.Exit(0);
        }

        private void textSpan_TextChanged(object sender, EventArgs e)
        {
            m_nTimmerSpan = int.Parse(textSpan.Text);
        }

        public void InitEnv()
        {
            String strEnvFile;
            String str;

            strEnvFile = m_strEnvPath + "nsd.env";
            if (File.Exists(strEnvFile))
            {
                StreamReader sr = new StreamReader(strEnvFile, Encoding.GetEncoding("Shift_JIS"));
                str = sr.ReadLine();
                m_nTimmerSpan = int.Parse(str);
                m_strDefAddURL = sr.ReadLine();
                m_strDefPath = sr.ReadLine();
                sr.Close();
            }
            else
            {
                m_nTimmerSpan = 2;
                m_strDefAddURL = "down/ssd/";
                m_strDefPath = "c:\\ssd\\";
            }
            textSpan.Text = m_nTimmerSpan.ToString(); 
        }

        public void InitCheckPathList()
        {
            String strLstFile;
            String str;

            m_arryUserName.Clear();
            m_nUserMax = 0;
            strLstFile = m_strEnvPath + "nsd.lst";
            if (File.Exists(strLstFile))
            {
                StreamReader sr = new StreamReader(strLstFile, Encoding.GetEncoding("Shift_JIS"));
                while ((str = sr.ReadLine()) != null)
                {
                    m_arryUserName.Add(str);
                    m_nUserMax++;
                }
                m_arryIntCheckSum = new int[m_nUserMax];
                sr.Close();
            }
            else
            {
                m_arryUserName.Add("user00");
                m_arryUserName.Add("user01");
                m_nUserMax = 2;
                m_arryIntCheckSum = new int[m_nUserMax];
            }
        }
        private void InitDeleteOldFile()
        {
            int idx;
            String strPath;
            String strFtpFolder;
            String strCrctFileName;
            FileStream fs;
            Byte[] bName = new Byte[8];
            String sName;
            String sMsg;

            DateTime dt = DateTime.Now;
            String sCrtDate = dt.ToString("yyyyMMdd");
            for (idx = 0; idx < m_nUserMax; idx++)
            {

                strPath = m_strDefPath + m_arryUserName[idx].ToString() + "\\";
                strCrctFileName = strPath + "crct.lst";
                if (File.Exists(strCrctFileName))
                {
                    fs = new FileStream(strCrctFileName, FileMode.Open, FileAccess.Read);
                    fs.Read(bName, 0, bName.Length);
                    fs.Close();
                    sName = System.Text.Encoding.GetEncoding("shift_jis").GetString(bName);
                    if (sCrtDate.CompareTo(sName) > 0)
                    {
                        strFtpFolder = m_strFtpRoot + m_strDefAddURL + m_arryUserName[idx];
                        DeleteFtpFiles(strFtpFolder);
                        FileDelete(strCrctFileName);
                        sMsg = m_arryUserName[idx].ToString() + "旧データを削除";
                        lstSend.Items.Add(sMsg);
                    }
                }
            }

        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            timer1.Stop();
            timer1.Enabled = false;
            CheckLoop();
            timer1.Interval = m_nTimmerSpan * 1000;
            timer1.Enabled = true;
            timer1.Start();
        }

        private void CheckLoop()
        {
            int i, sum;
            String  strPath;

            for (i = 0; i < m_nUserMax; i++)
            {
                strPath = m_strDefPath + m_arryUserName[i].ToString()+"\\";
                if (Directory.Exists(strPath))
                {
                    String[] strFiles = Directory.GetFiles(strPath);
                    sum = strFiles.Length;
                    if (sum != 0)
                    {
                        if (sum == m_arryIntCheckSum[i])
                        {
                            SendFileLoop(i, strFiles);
                            m_arryIntCheckSum[i] = 0;
                        }
                        else
                        {
                            m_arryIntCheckSum[i] = sum;
                        }
                    }

                }

            }
        }

        private bool SendFileLoop(int idx, String[] strFiles)
        {
            String sUserName;
            FileStream fs;
            String strFtpFolder;
            Image img;
            int i, max, rlen, flen;
            String strRootPath, strUrl, strName;
            int size;
            String strTmpFile;
            String strZipFolder;
            String strCmpFileName;
            string strZipFileName;
            String strDspZipFolder;
            String strBmpFileName;
            string strDspZipFileName;
            String strCrctFileName;
            String strFtpFileName;
            int nLast;
            String sMsg;
            Byte[] bName;
            bool ret;

            sUserName = m_arryUserName[idx].ToString();
            strFtpFolder = m_strFtpRoot + m_strDefAddURL + sUserName;
            //DeleteFtpFiles(strFtpFolder);

            strTmpFile = m_strEnvPath + "tmp.bmp";
            strRootPath = m_strDefPath + sUserName + "\\";
            strUrl = sUserName + "/";
            rlen = strRootPath.Length;
            max = strFiles.Length;
            ret = true;

            strCrctFileName = strRootPath + "\\crct.lst";
            if (File.Exists(strCrctFileName))
            {
                fs = new FileStream(strCrctFileName, FileMode.Open, FileAccess.ReadWrite);
                fs.Seek(0, SeekOrigin.End);
            }
            else
            {
                fs = new FileStream(strCrctFileName, FileMode.Create);
            }
            for (i = 0; i < max; i++)
            {
                flen = strFiles[i].Length;
                strName = strFiles[i].Substring(rlen, flen-rlen-4);
                if (strName != "crct")
                {
                    img = Image.FromFile(strFiles[i]);
                    img.Save(strTmpFile, ImageFormat.Bmp);
                    img.Dispose();

                    FileStream fsIn = new FileStream(strTmpFile, FileMode.Open);
                    size = (int)fsIn.Length;
                    Byte[] imgbuf = new Byte[size];
                    fsIn.Read(imgbuf, 0, size);
                    fsIn.Close();
                    m_nXDot = Getint32(imgbuf, 18);
                    m_nYDot = Getint32(imgbuf, 22);
                    m_nXByte = ((m_nXDot * 3) + 3) / 4 * 4;

                    strZipFolder = m_strEnvPath + strName;
                    strZipFileName = m_strEnvPath + strName + ".zip";
                    Directory.CreateDirectory(strZipFolder);
                    strCmpFileName = strZipFolder + "/" + strName + ".cmp";
                    CreateCmpFile(imgbuf, strCmpFileName);
                    ZipFile.CreateFromDirectory(strZipFolder, strZipFileName);

                    //CreateGZFile(strCmpFileName, strGZFileName);
                    strDspZipFolder = m_strEnvPath + strName + "DSP";
                    strDspZipFileName = m_strEnvPath + strName + "DSP.zip";
                    Directory.CreateDirectory(strDspZipFolder);
                    strBmpFileName = strDspZipFolder + "/" + strName + ".bmp";
                    CreateBmpFile(imgbuf, strBmpFileName);
                    ZipFile.CreateFromDirectory(strDspZipFolder, strDspZipFileName);
                    strFtpFileName = m_strFtpRoot + m_strDefAddURL + m_arryUserName[idx] + "/" + strName + ".zip";
                    ret = SendFtpFile(strZipFileName, strFtpFileName);
                    if (ret == true)
                    {
                        FileDelete(strCmpFileName);
                        FileDelete(strZipFileName);
                        Directory.Delete(strZipFolder);
                        strFtpFileName = m_strFtpRoot + m_strDefAddURL + m_arryUserName[idx] + "/" + strName + "DSP.zip";
                        ret = SendFtpFile(strDspZipFileName, strFtpFileName);
                        if (ret == true)
                        {
                            FileDelete(strBmpFileName);
                            FileDelete(strDspZipFileName);
                            Directory.Delete(strDspZipFolder);
                            FileDelete(strFiles[i]);
                        }

                    }
                    if (ret == false)
                    {
                        break;
                    }

                    sMsg = strName + "を" + sUserName + "に転送";
                    lstSend.Items.Add(sMsg);
                    nLast = lstSend.Items.Count;
                    lstSend.SelectedIndex = nLast - 1;
                    bName = Encoding.ASCII.GetBytes(strName);
                    fs.Write(bName, 0, bName.Length);
                    bName = Encoding.ASCII.GetBytes("\r\n");
                    fs.Write(bName, 0, bName.Length);
                }
            }
            fs.Close();
            if (ret == true)
            {
                strFtpFileName = m_strFtpRoot + m_strDefAddURL + m_arryUserName[idx] + "/crct.lst";
                ret = SendFtpFile(strCrctFileName, strFtpFileName);
            }
            if(ret == true)
            {
                // FileDelete(strCrctFileName);
                return true;
            }
            else
            {
                MessageBox.Show("FTP転送が出来ません、接続を確認してください", "名札転送エラー", MessageBoxButtons.OK);
                return false;
            }
        }

        private void FileDelete(String strFileName)
        {
            File.Delete(strFileName);
        }

        private bool SendFtpFile(String strDataFileName, String strFtpFileName)
        {
            int count;

            while (File.Exists(strDataFileName) == false)
            {
                Thread.Sleep(1000);
            }

            try
            {
                Uri u = new Uri(strFtpFileName);
                FtpWebRequest ftpReq = (FtpWebRequest)WebRequest.Create(u);
                ftpReq.Credentials = new NetworkCredential(m_strFtpID, m_strFtpPW);
                ftpReq.KeepAlive = false;
                ftpReq.UseBinary = true;
                ftpReq.UsePassive = true;
                ftpReq.Timeout = 10000;
                ftpReq.Method = WebRequestMethods.Ftp.UploadFile;
                Byte[] buf = new Byte[1024];
                Stream st = ftpReq.GetRequestStream();
                FileStream fs = new FileStream(strDataFileName, FileMode.Open);
                while (true)
                {
                    count = fs.Read(buf, 0, buf.Length);
                    if (count == 0)
                    {
                        break;
                    }
                    st.Write(buf, 0, count);
                }
                fs.Close();
                st.Close();
                return (true);
            }
            catch (Exception ex)
            {
                Console.Write(ex.Message);
                return (false);
            }
        }
        
        private void CreateCmpFile(Byte[] img, String strCmpFileName)
        {
            int dstxdot, dstydot, dstxbyte;
            int top, idx;
            int size, sumsize;
            int x, y, dst, last;
            int flag, cnt;
            byte mask;
            int linecnt;
            int bitimg;
            Byte[] bTwoImg;

            top = Getint32(img, 10);
            dstxdot = m_nXDot * 2;
            dstydot = m_nYDot * 2;
            dstxbyte = ((dstxdot + 7) / 8 + 3) / 4 * 4;
            bTwoImg = new Byte[dstxbyte];
            Byte[] bOut = new Byte[dstxbyte * 2];

            FileStream fsOut = new FileStream(strCmpFileName, FileMode.Create);
            Setint32(bOut, 0, 0);
            Setint32(bOut, 4, dstxdot);
            Setint32(bOut, 8, dstydot);
            fsOut.Write(bOut, 0, 12);
            sumsize = 12;
            linecnt = 0;
            for (y = m_nYDot-1; y >= 0; y--)
            {
                idx = y * m_nXByte + top;
                dst = 0;
                mask = 0x0080;
                last = 0;
                for (x = 0; x < dstxbyte; x++)
                {
                    bTwoImg[x] = 0;
                }

                for (x = 0; x < m_nXDot; x++)
                {
                    bitimg =(int)(img[idx] & 0x00ff);
                    idx += 3;
                    if (bitimg < 0x0080)
                    {
                        bTwoImg[dst] |= mask;
                        last = dst;
                    }
                    mask >>= 1;
                    if (bitimg < 0x0080)
                    {
                        bTwoImg[dst] |= mask;
                        last = dst;
                    }
                    mask >>= 1;
                    if (mask == 0)
                    {
                        mask = 0x0080;
                        dst++;
                    }
                }
                // compress処理
                if (last == 0)
                {
                    // 空白ラインも２行出力
                    linecnt++;
                    linecnt++;
                    if (linecnt >= 255)　// 改行数をセット
                    {
                        bOut[0] = 0x00; bOut[1] = 0x00; bOut[2] = 0xff;
                        fsOut.Write(bOut, 0, 3);
                        sumsize += 3;
                        if (linecnt == 255)
                        {
                            linecnt = 0;
                        }
                        else
                        {
                            linecnt = 0;
                        }
                    }
                }
                else
                {
                    if (linecnt != 0)　// 改行数をセット
                    {
                        bOut[0] = 0x00; bOut[1] = 0x00; bOut[2] = (byte)linecnt;
                        fsOut.Write(bOut, 0, 3);
                        sumsize += 3;
                        linecnt = 0;
                    }
                    last++; // バイト位置は0から始まるためバイト数は１プラス
                    flag = -1;
                    cnt = 0;
                    dst = 4; // 0,1は一行のサイズ, 2,3は展開した後のバイト数
                    // 1行のバイトイメージを格納
                    for (x = 0; x < last; x++)
                    {
                        if(bTwoImg[x] == 0){
                            if(flag == -1){
                                cnt++;
                                if(cnt >= 255) // 0x00を255個
                                {
                                    bOut[dst++] = 0x00; bOut[dst++] = 0xff;
                                    cnt = 0;
                                }
                            }
                            else if (flag == 1)　// 0xffをcnt個
                            {
                                bOut[dst++] = 0xff; bOut[dst++] = (byte)cnt;
                                cnt = 1;
                            }
                            else
                            {
                                cnt = 1;
                            }
                            flag = -1;
                        }
                        else if(bTwoImg[x] == 0xff)
                        {
                            if(flag == 1){
                                cnt++;
                                if (cnt >= 255) // 0xffを255個
                                {
                                    bOut[dst++] = 0xff; bOut[dst++] = 0xff;
                                    cnt = 0;
                                }
                            }
                            else if (flag == -1)　// 0x00をcnt個
                            {
                                bOut[dst++] = 0x00; bOut[dst++] = (byte)cnt;
                                cnt = 1;
                            }
                            else
                            {
                                cnt = 1;
                            }
                            flag = 1;
                        }
                        else
                        {
                            if (flag == -1)　// 0x00をcnt個
                            {
                                bOut[dst++] = 0x00; bOut[dst++] = (byte)cnt;
                                cnt = 1;
                            }
                            else if (flag == 1)　// 0xffをcnt個
                            {
                                bOut[dst++] = 0xff; bOut[dst++] = (byte)cnt;
                                cnt = 1;
                            }
                            bOut[dst++] = bTwoImg[x]; 
                            flag = 0;
                        }
                    }
                    if (flag == -1)　// 0x00をcnt個
                    {
                        bOut[dst++] = 0x00; bOut[dst++] = (byte)cnt;
                    }
                    else if (flag == 1)　// 0xffをcnt個
                    {
                        bOut[dst++] = 0xff; bOut[dst++] = (byte)cnt;
                    }
                    size = dst - 4;
                    bOut[0] = (byte)(size & 0x00ff); bOut[1] = (byte)((size >> 8) & 0xff);
                    bOut[2] = (byte)(last & 0x00ff); bOut[3] = (byte)((last >> 8) & 0xff);
                    // 同じ物を２ライン分出力
                    fsOut.Write(bOut, 0, dst);
                    sumsize += dst;
                    fsOut.Write(bOut, 0, dst);
                    sumsize += dst;
                }
            }
            if (linecnt != 0)
            {
                bOut[0] = 0x00; bOut[1] = 0x00; bOut[2] = (byte)linecnt;
                fsOut.Write(bOut, 0, 3);
                sumsize += 3;
            }

            for (idx = 0; idx < 16; idx++)
            {
                bOut[0] = 0x00;
            }
            size = 32 - (sumsize % 32);
            fsOut.Write(bOut, 0, size);     // 32バウンダリに設定
            fsOut.Close();
            fsOut = new FileStream(strCmpFileName, FileMode.Open, FileAccess.ReadWrite);
            Setint32(bOut, 0, sumsize);
            fsOut.Write(bOut, 0, 4);
            fsOut.Close();
        }

        private void CreateBmpFile(Byte[] img, String strBmpFileName)
        {
            int    top, ltop, idx;
            double dTime;
            int xdot, ydot, xbyte;
            int num, bitimg;
            int size;
            byte mask;
            int nbyte;
            int x, y;
            Byte[] bOut;

            num = 0;
            top = Getint32(img, 10);
            dTime = ((double)m_nYDot) / 260.0;
            xdot = (int)((double)m_nXDot / dTime);
            ydot = 260;
            xbyte = ((xdot+7)/8+3)/4*4;
            size = 62 + xbyte * ydot;
            if (xbyte < 62)
            {
                bOut = new Byte[62];
            }
            else
            {
                bOut = new Byte[xbyte];
            }

            FileStream fsOut = new FileStream(strBmpFileName, FileMode.Create);
            for (x = 0; x < 62; x++)
            {
                bOut[x] = 0;
            }
            bOut[0] = 0x42; bOut[1] = 0x4D;
            num = size - 62;
            Setint32(bOut, 34, num);
            bOut[10] = 0x3E; bOut[14] = 0x28;
            num = xbyte * 8;
            Setint32(bOut, 18, num);
            Setint32(bOut, 22, ydot);
            bOut[26] = 1; bOut[28] = 1;
            Setint32(bOut, 34, size);
            bOut[38] = 0x12; bOut[39] = 0x0b;
            bOut[42] = 0x12; bOut[43] = 0x0b;
            bOut[54] = 0xff; bOut[55] = 0xff; bOut[56] = 0xff;
            fsOut.Write(bOut, 0, 62);
            for (y = 0; y < ydot; y++)
            {
                
                ltop = (int)((double)y * dTime) * m_nXByte + top;
                mask = 0x0080;
                nbyte = 0;
                for (x = 0; x < xbyte; x++)
                {
                    bOut[x] = 0;
                }
                for (x = 0; x < xdot; x++)
                {
                    idx = ltop + (int)((double)x * dTime) * 3;
                    bitimg =(int)(img[idx] & 0x00ff);
                    if (bitimg < 0x0080)
                    {
                        bOut[nbyte] |= mask;
                    }
                    mask >>= 1;
                    if(mask == 0){
                        mask = 0x080;
                        nbyte++;
                    }
                }
                fsOut.Write(bOut, 0, xbyte);
            }
            fsOut.Close();
        }

        private int Getint32(Byte[] buf, int offset)
        {
            int i1, i2, i3, i4, ret;

            i1 = buf[offset++] & 0x00ff;
            i2 = buf[offset++] & 0x00ff;
            i3 = buf[offset++] & 0x00ff;
            i4 = buf[offset++] & 0x00ff;
            ret = i1 + i2 * 0x100 + i3 * 0x10000 + i4 * 0x1000000;
            return (ret);
        }

        private void Setint32(Byte[] buf, int offset, int num)
        {
            buf[offset++] = (byte)(num & 0x00ff);
            buf[offset++] = (byte)((num >> 8) & 0x00ff);
            buf[offset++] = (byte)((num >> 16) & 0x00ff);
            buf[offset++] = (byte)((num >> 24) & 0x00ff);
        }

        private void CreateGZFile(String szSrcFile, String szDstFile)
        {
            int num;
            long len;

            FileStream fsIn = new FileStream(szSrcFile, FileMode.Open, FileAccess.Read);
            FileStream fsOut = new FileStream(szDstFile, FileMode.Create);
            GZipStream gzsTmp = new GZipStream(fsOut, CompressionMode.Compress);

            len = fsIn.Length;
            byte[] buf = new byte[len];
            num = fsIn.Read(buf, 0, buf.Length);
            gzsTmp.Write(buf, 0, num);
            fsIn.Close();
            fsOut.Close();
        }

        private void DeleteFtpFiles(String strFtpFolder)
        {
            String names;
            String name;
            String[] files;
            int max, idx, len;
            String strFtpFile;
            Uri uriFile;
            FtpWebRequest frFile;
            FtpWebResponse ftpResFile;


            Uri uriFolder = new Uri(strFtpFolder + "/");
            FtpWebRequest frFolder = (FtpWebRequest)WebRequest.Create(uriFolder);
            frFolder.Credentials = new NetworkCredential(m_strFtpID, m_strFtpPW);
            frFolder.Method = WebRequestMethods.Ftp.ListDirectory;
            frFolder.KeepAlive = false;   //要求の完了後に接続を閉じる
            frFolder.UsePassive = false; //PASSIVEモードを無効にする
            FtpWebResponse ftpResFolder = (FtpWebResponse)frFolder.GetResponse();
            StreamReader sr = new StreamReader(ftpResFolder.GetResponseStream());
            names = sr.ReadToEnd();
            sr.Close();
            ftpResFolder.Close();

            files = names.Split('\n');
            max = files.Length;
            for (idx = 0; idx < max; idx++)
            {
                len = files[idx].Length;
                if (len == 0)
                {
                    break;
                }
                name = files[idx].Substring(0, len - 1);
                if (name != "" && name != "." && name != "..")
                {
                    strFtpFile = strFtpFolder + "/" + name;
                    uriFile = new Uri(strFtpFile);
                    frFile = (FtpWebRequest)WebRequest.Create(uriFile);
                    frFile.Credentials = new NetworkCredential(m_strFtpID, m_strFtpPW);
                    frFile.Method = WebRequestMethods.Ftp.DeleteFile;
                    frFile.KeepAlive = false;   //要求の完了後に接続を閉じる
                    frFile.UsePassive = false; //PASSIVEモードを無効にする
                    ftpResFile = (FtpWebResponse)frFile.GetResponse();
                    ftpResFile.Close();
                }
            }
        }
    }
}
