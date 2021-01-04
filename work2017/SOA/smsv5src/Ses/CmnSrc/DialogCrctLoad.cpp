// DlCrctLd.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogCrctLoad.h"


//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogCrctLoad ダイアログ

CDialogCrctLoad::CDialogCrctLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCrctLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogCrctLoad)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogCrctLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogCrctLoad)
	DDX_Control(pDX, IDC_LST_CRCT, m_lstCrct);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogCrctLoad, CDialog)
	//{{AFX_MSG_MAP(CDialogCrctLoad)
	ON_LBN_SELCHANGE(IDC_LST_CRCT, OnSelchangeLstCrct)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogCrctLoad メッセージ ハンドラ

BOOL CDialogCrctLoad::OnInitDialog() 
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	FILE	*crctfp;
	TCHAR	csvbuf[RECMAX8];
	int		len;
	LPTSTR	csvsp[RECMAX];
	TCHAR	buf[RECMAX];

	CDialog::OnInitDialog();
	senvgetexepath(exepath);
	_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);
	_tfopen_s(&crctfp, crctfname, _T("r+,ccs=UNICODE"));
	if(crctfp == NULL){
		return TRUE;
	}
	m_lstCrct.SetHorizontalExtent(10000);
	while(1){
		if(_fgetts(csvbuf, RECMAX8, crctfp) == NULL){
			break;
		}
		len = _tcslen(csvbuf);
		csvbuf[len-1] = _T('\0');
		basedivcsvdata(csvsp, csvbuf);
		_stprintf_s(buf, _T("%s %s"), csvsp[2], csvsp[5]);
		m_lstCrct.AddString(buf);
	}
	fclose(crctfp);
	m_nSlctIdx = -1;
	return TRUE;
}

void CDialogCrctLoad::OnSelchangeLstCrct() 
{
	m_nSlctIdx = m_lstCrct.GetCurSel();
}
