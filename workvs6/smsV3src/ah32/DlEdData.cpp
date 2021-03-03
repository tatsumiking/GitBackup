// DlEdData.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlEdData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEditData ダイアログ


CDialogEditData::CDialogEditData(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditData::IDD, pParent)
{
	long	lsize;

	m_pcAh32Dlg = (CAh32Dlg *)pParent;
	lsize = 600000;
	m_hEditBuf = ::GlobalAlloc(GHND, lsize);
	//{{AFX_DATA_INIT(CDialogEditData)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

void CDialogEditData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditData)
	DDX_Control(pDX, IDC_EDIT_DATA, m_edData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEditData, CDialog)
	//{{AFX_MSG_MAP(CDialogEditData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEditData メッセージ ハンドラ

void CDialogEditData::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	SetStrgTextToFile();
	CDialog::OnOK();
}

BOOL CDialogEditData::OnInitDialog() 
{
	char	szFname[FNAMEMAX];
	FILE	*fp;
	char	strg[RECMAX];
	char	dda[5];
	LPSTR	ret;


	CDialog::OnInitDialog();
	strcpy(szFname, m_pcAh32Dlg->m_szDataFname);
	if((fp = fopen(szFname, "r+")) == NULL){
		if((fp = fopen(szFname, "w+")) == NULL){
			return TRUE;
		}
	}
	dda[0] = 0x0d;
	dda[1] = 0x0d;
	dda[2] = 0x0a;
	dda[3] = 0x00;
	m_lpEditBuf = (LPSTR)::GlobalLock(m_hEditBuf);
	while(1){
		if(CrDelFgets(strg, RECMAX, fp) == NULL){
			break;
		}
		strcat(m_lpEditBuf, strg);
		strcat(m_lpEditBuf, dda);
	}
	SetDlgItemText(IDC_EDIT_DATA, m_lpEditBuf);
	::GlobalUnlock(m_hEditBuf);
	return TRUE;
}

LPSTR CDialogEditData::CrDelFgets(char * buf, int max, FILE * fp)
{
	char	*ret;
	int		len;

	ret = fgets(buf, max, fp);
	if(ret != NULL){
		len = strlen(buf);
		if(buf[len-1] == 0x0a){
			if(buf[len-2] == 0x0d){
				if(buf[len-3] == 0x0d){
					buf[len-3] = '\0';
				}
				else{
					buf[len-2] = '\0';
				}
			}
			else{
				buf[len-1] = '\0';
			}
		}
		else if(buf[len-1] == 0x0d){
			buf[len-1] = '\0';
		}
	}
	return(ret);
}

BOOL CDialogEditData::DestroyWindow() 
{
	if(m_hEditBuf != NULL){
		::GlobalFree(m_hEditBuf);
		m_hEditBuf = NULL;
	}
	return CDialog::DestroyWindow();
}

void CDialogEditData::SetStrgTextToFile()
{
	char	szFname[FNAMEMAX];
	FILE	*fp;
	char	strg[RECMAX];
	int		linemax, idx, len;

	UpdateData(TRUE);
	strcpy(szFname, m_pcAh32Dlg->m_szDataFname);
	if((fp = fopen(szFname, "w")) != NULL){
		m_lpEditBuf = (LPSTR)::GlobalLock(m_hEditBuf);
		if(m_lpEditBuf != 0){
			linemax = m_edData.GetLineCount();
			if(linemax == 1){
				m_edData.GetWindowText(m_lpEditBuf, RECMAX);
				len = strlen(m_lpEditBuf);
				if(len != 0){
					m_lpEditBuf[len] = '\n';
					m_lpEditBuf[len+1] = '\0';
					fputs(m_lpEditBuf, fp);
				}
			}
			else{
				m_edData.GetWindowText(m_lpEditBuf, RECMAX);
				for(idx = 0; idx < linemax; idx++){
					len = m_edData.GetLine(idx, m_lpEditBuf);
					if(len != 0){
						m_lpEditBuf[len] = '\n';
						m_lpEditBuf[len+1] = '\0';
						fputs(m_lpEditBuf, fp);
					}
				}
			}
			::GlobalUnlock(m_hEditBuf);
		}
		fclose(fp);
	}
}

