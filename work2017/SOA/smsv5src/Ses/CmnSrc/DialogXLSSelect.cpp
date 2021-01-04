// DXLSSlct.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogXLSSelect.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogXLSSelect ダイアログ

CDialogXLSSelect::CDialogXLSSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogXLSSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogXLSSelect)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

void CDialogXLSSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogXLSSelect)
	DDX_Control(pDX, IDC_LSTNAME, m_lstName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogXLSSelect, CDialog)
	//{{AFX_MSG_MAP(CDialogXLSSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogXLSSelect メッセージ ハンドラ

void CDialogXLSSelect::OnOK() 
{
	int idx = m_lstName.GetCurSel();
	if(idx == -1){
		::AfxMessageBox(_T("接続先が指定されていません"), MB_OK);
		return;
	}
	m_lstName.GetText(idx, m_csSheetName);
	CDialog::OnOK();
}

BOOL CDialogXLSSelect::OnInitDialog() 
{
	TCHAR	szFileName[FNAMEMAX];
	TCHAR	szTitle[RECMAX];

	CDialog::OnInitDialog();
	_tcscpy_s(szFileName, m_csFileName.GetBuffer(FNAMEMAX));
	int idx = basegetparentpath(szFileName);
	_tcscpy_s(szFileName, m_csFileName.GetBuffer(FNAMEMAX));
	_tcscpy_s(szTitle, &(szFileName[idx+1]));
	SetWindowText(szTitle);
	InitNameList();
	return TRUE;
}

void CDialogXLSSelect::InitNameList()
{
	CString	csStr;
	int		idx, ret;

	if(m_cExcelLib.Open(&m_csFileName) == FALSE){
		OnCancel();
		return;
	}
	idx = 1;
	while(1){
		ret = m_cExcelLib.GetSheetName(idx, &csStr);
		if(ret == FALSE){
			break;
		}
		m_lstName.AddString(csStr);
		idx++;
	}
	m_cExcelLib.Close();
	m_lstName.SetCurSel(0);
}

void CDialogXLSSelect::OnCancel() 
{
	CDialog::OnCancel();
}

