using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;

namespace Ssd
{
    public partial class EnvForm : Form
    {
        public SsdForm m_cSsdForm;
        public String m_strFtpRoot;
        public String m_strFtpID;
        public String m_strFtpPW;
        public int m_nTimmerSpan;
        public String m_strDefAddURL;
        public String m_strDefPath;
        public String m_strEnvPath;
        public List<String> m_aryAddUser = new List<String>();
        public List<String> m_aryDeleteUser = new List<String>();

        public EnvForm()
        {
            InitializeComponent();
        }

        private void EnvForm_Load(object sender, EventArgs e)
        {
            textDefAddUrl.Text = m_strDefAddURL;
            textDefPath.Text = m_strDefPath;
            InitListUser();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            String strEnvFile;
            String strLstFile;
            StreamWriter sw;
            String user;
            int i, max;

            if(textDefAddUrl.Text != m_strDefAddURL){
                MakeFtpFolder();
                m_strDefAddURL = textDefAddUrl.Text;
            }
            else
            {
                MakeDeleteFtpFolder();
            }

            if(textDefPath.Text != m_strDefPath){
                MakePcFolder();
                m_strDefPath = textDefPath.Text;
            }
            else
            {
                MakeDeletePcFolder();
            }

            strEnvFile = m_strEnvPath + "ssd.env";
            sw = new StreamWriter(strEnvFile);
            sw.WriteLine(m_nTimmerSpan.ToString());
            sw.WriteLine(m_strDefAddURL);
            sw.WriteLine(m_strDefPath);
            sw.Close();

            strLstFile = m_strEnvPath + "ssd.lst";
            sw = new StreamWriter(strLstFile);
            max = listUser.Items.Count;
            for (i = 0; i < max; i++)
            {
                user = listUser.Items[i].ToString();
                sw.WriteLine(user);
            }
            sw.Close();
            m_cSsdForm.InitEnv();
            m_cSsdForm.InitCheckPathList();
            m_cSsdForm.TimerStart();
            this.Close();
        }


        private void btnCancel_Click(object sender, EventArgs e)
        {
            m_cSsdForm.TimerStart();
            this.Close();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            int idx;
            String name;

            name = textUser.Text;
            idx = listUser.FindString(name);
            if (idx == -1)
            {
                listUser.Items.Add(name);

                m_aryAddUser.Add(name);
                idx = GetIdxDeleteUser(name);
                if (idx != -1)
                {
                    m_aryDeleteUser.RemoveAt(idx);
                }
            }
            else
            {
                MessageBox.Show("既にユーザ名として登録されています");
            }

        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            int idx;
            String name;

            name = textUser.Text;
            idx = listUser.FindString(name);
            if (idx != -1)
            {
                listUser.Items.RemoveAt(idx);

                m_aryDeleteUser.Add(name);
                idx = GetIdxAddUser(name);
                if (idx != -1)
                {
                    m_aryAddUser.RemoveAt(idx);
                }
            }
            else
            {
                MessageBox.Show("対応するユーザ名は登録されていません");
            }
        }

        private void listUser_SelectedIndexChanged(object sender, EventArgs e)
        {
            textUser.Text = listUser.Text;
        }

        private void InitListUser()
        {
            String strListFile;
            String str;

            strListFile = m_strEnvPath + "nsd.lst";
            listUser.Items.Clear();
            if (File.Exists(strListFile))
            {
                StreamReader sr = new StreamReader(strListFile, Encoding.GetEncoding("Shift_JIS"));
                while ((str = sr.ReadLine()) != null)
                {
                    listUser.Items.Add(str);
                }
                sr.Close();
            }
        }

        private void MakeFtpFolder()
        {
            int max, i;
            String user;
            String strFtpFloder;

            max = listUser.Items.Count;
            for (i = 0; i < max; i++)
            {
                user = listUser.Items[i].ToString();
                strFtpFloder = m_strFtpRoot+m_strDefAddURL+user;
                MakeFtpFolder(strFtpFloder);
            }
        }

        private void MakePcFolder()
        {
            int max, i;
            String user;
            String strPcFloder;

            max = listUser.Items.Count;
            for (i = 0; i < max; i++)
            {
                user = listUser.Items[i].ToString();
                strPcFloder = m_strDefPath+user;
                Directory.CreateDirectory(strPcFloder);
            }
       }

        private void MakeDeleteFtpFolder()
        {
            int max, i;
            String user;
            String strFtpFloder;

            max = m_aryAddUser.Count;
            for(i = 0; i < max; i++){
                user = m_aryAddUser[i];
                strFtpFloder = m_strFtpRoot+m_strDefAddURL+user;
                MakeFtpFolder(strFtpFloder);
            }
            max = m_aryDeleteUser.Count;
            for(i = 0; i < max; i++){
                user = m_aryDeleteUser[i];
                strFtpFloder = m_strFtpRoot+m_strDefAddURL+user;
                DeleteFtpFiles(strFtpFloder);
                DeleteFtpFolder(strFtpFloder);
            }
        }

        private void MakeDeletePcFolder()
        {
            int max, i;
            String user;
            String strPcFloder;

            max = m_aryAddUser.Count;
            for (i = 0; i < max; i++)
            {
                user = m_aryAddUser[i];
                strPcFloder = m_strDefPath + user;
                Directory.CreateDirectory(strPcFloder);
            }
            max = m_aryDeleteUser.Count;
            for (i = 0; i < max; i++)
            {
                user = m_aryDeleteUser[i];
                strPcFloder = m_strDefPath + user;
                Directory.Delete(strPcFloder);
            }
        }

        private void MakeFtpFolder(String strFtpFolder)
        {
            Uri u = new Uri(strFtpFolder);
            FtpWebRequest ftpReq = (FtpWebRequest)WebRequest.Create(u);
            ftpReq.Credentials = new NetworkCredential(m_strFtpID, m_strFtpPW);
            ftpReq.Method = WebRequestMethods.Ftp.MakeDirectory;
            FtpWebResponse ftpRes = (FtpWebResponse)ftpReq.GetResponse();
            ftpRes.Close();
        }

        private void DeleteFtpFolder(String strFtpFolder)
        {
            Uri u = new Uri(strFtpFolder);
            FtpWebRequest ftpReq = (FtpWebRequest)WebRequest.Create(u);
            ftpReq.Credentials = new NetworkCredential(m_strFtpID, m_strFtpPW);
            ftpReq.Method = WebRequestMethods.Ftp.RemoveDirectory;
            FtpWebResponse ftpRes = (FtpWebResponse)ftpReq.GetResponse();
            ftpRes.Close();
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


            Uri uriFolder = new Uri(strFtpFolder+"/");
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

        private int GetIdxAddUser(String strSrc)
        {
            int max, idx;

            max = m_aryAddUser.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (strSrc == m_aryAddUser[idx])
                {
                    return (idx);
                }
            }
            return (-1);
        }

        private int GetIdxDeleteUser(String strSrc)
        {
            int max, idx;

            max = m_aryDeleteUser.Count;
            for (idx = 0; idx < max; idx++)
            {
                if (strSrc == m_aryDeleteUser[idx])
                {
                    return (idx);
                }
            }
            return (-1);
        }
    }
}
