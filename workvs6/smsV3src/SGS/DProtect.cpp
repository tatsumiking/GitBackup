// DProtect.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DProtect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogProtect ダイアログ


CDialogProtect::CDialogProtect(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProtect::IDD, pParent)
{
	m_nDelProtect = OFF;
	m_szPassword[0] = '\0';
	//{{AFX_DATA_INIT(CDialogProtect)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogProtect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogProtect)
	DDX_Control(pDX, IDC_PRTCTL, m_prtCtl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogProtect, CDialog)
	//{{AFX_MSG_MAP(CDialogProtect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogProtect メッセージ ハンドラ

BOOL CDialogProtect::OnInitDialog() 
{
	int		ret;

	CDialog::OnInitDialog();
	if(m_nDelProtect == ON){
		m_prtCtl.DelProtectInfo();
	}
	else if(m_szPassword[0] != '\0'){
		m_prtCtl.SetPassWord(m_szPassword);
	}
	else{
		m_lProtectID = m_prtCtl.GetProtectID();
	}
	ret = m_prtCtl.CheckProtect();
	m_nStat = m_prtCtl.GetProtectStatus();
	m_nDays = m_prtCtl.GetDaysProtect();
	if(ret == TRUE){
		OnOK();
	}
	else{
		OnCancel();
	}
	return TRUE;
}
