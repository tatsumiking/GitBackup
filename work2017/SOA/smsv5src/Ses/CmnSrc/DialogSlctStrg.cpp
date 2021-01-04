// DSlctStrg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogSlctStrg.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctStrg ダイアログ

CDialogSlctStrg::CDialogSlctStrg(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSlctStrg::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogSlctStrg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogSlctStrg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSlctStrg)
	DDX_Control(pDX, IDC_LSTSTRG1, m_lstStrg1);
	DDX_Control(pDX, IDC_LSTSTRG2, m_lstStrg2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSlctStrg, CDialog)
	//{{AFX_MSG_MAP(CDialogSlctStrg)
	ON_LBN_DBLCLK(IDC_LSTSTRG1, OnDblclkLststrg1)
	ON_LBN_DBLCLK(IDC_LSTSTRG2, OnDblclkLststrg2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctStrg メッセージ ハンドラ

BOOL CDialogSlctStrg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	LoadStrg1List();
	LoadStrg2List();
	return TRUE;
}

void CDialogSlctStrg::LoadStrg1List()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	m_lstStrg1.ResetContent();
	senvgetexepath(exepath);
	_stprintf_s(szFileName, _T("%s\\SlctStrg1.txt"), exepath);
	_tfopen_s(&fp, szFileName, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		m_lstStrg1.AddString(buf);
	}
	fclose(fp);
}

void CDialogSlctStrg::LoadStrg2List()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	m_lstStrg2.ResetContent();
	senvgetexepath(exepath);
	_stprintf_s(szFileName, _T("%s\\SlctStrg2.txt"), exepath);
	_tfopen_s(&fp, szFileName, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		m_lstStrg2.AddString(buf);
	}
	fclose(fp);

}

void CDialogSlctStrg::OnDblclkLststrg1() 
{
	int		idx;
	TCHAR	szStr[RECMAX];

	idx = m_lstStrg1.GetCurSel();
	m_lstStrg1.GetText(idx, szStr);
	m_pcView->SetString(szStr);
}

void CDialogSlctStrg::OnDblclkLststrg2() 
{
	int		idx;
	TCHAR	szStr[RECMAX];

	idx = m_lstStrg2.GetCurSel();
	m_lstStrg2.GetText(idx, szStr);
	m_pcView->SetString(szStr);
}
