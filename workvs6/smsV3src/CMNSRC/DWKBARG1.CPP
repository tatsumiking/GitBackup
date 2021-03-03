// DWkBarG1.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DWkBarG1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBarG1 ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	void scrngetmmsize(LPDBL bxmm, LPDBL bymm);
	void scrnsetmmsize(DBL xsize, DBL ysize);
};

CDialogWakuBarG1::CDialogWakuBarG1(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CDialogWakuBarG1)
	m_csWorkXSize = _T("");
	m_csWorkYSize = _T("");
	//}}AFX_DATA_INIT
}

void CDialogWakuBarG1::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuBarG1)
	DDX_Text(pDX, IDC_EDWORKXSIZE, m_csWorkXSize);
	DDX_Text(pDX, IDC_EDWORKYSIZE, m_csWorkYSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogWakuBarG1, CDialogBar)
	//{{AFX_MSG_MAP(CDialogWakuBarG1)
	ON_EN_CHANGE(IDC_EDWORKXSIZE, OnChangeEdworkxsize)
	ON_EN_CHANGE(IDC_EDWORKYSIZE, OnChangeEdworkysize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBarG1 メッセージ ハンドラ

void CDialogWakuBarG1::SetItemElement()
{
	DBL		xmm, ymm;

	scrngetmmsize(&xmm, &ymm);
	m_csWorkXSize.Format("%1.1lf", xmm);
	m_csWorkYSize.Format("%1.1lf", ymm);
	UpdateData(FALSE);
}

void CDialogWakuBarG1::OnChangeEdworkxsize() 
{
	DBL		xmm, ymm;

	CMainFrame *pcWnd = (CMainFrame *)::AfxGetMainWnd();
	if(pcWnd->m_pcCrtView == NULL){
		return;
	}
	UpdateData(TRUE);
	xmm = atof(m_csWorkXSize);
	ymm = atof(m_csWorkYSize);
	if(0 < xmm && 0 < ymm){
		scrnsetmmsize(xmm, ymm);
	}
	pcWnd->m_pcCrtView->WorkSizeReset();
	pcWnd->m_pcCrtView->WorkAreaReset();
	pcWnd->m_pcCrtView->Invalidate(TRUE);
}

void CDialogWakuBarG1::OnChangeEdworkysize() 
{
	DBL		xmm, ymm;

	CMainFrame *pcWnd = (CMainFrame *)::AfxGetMainWnd();
	if(pcWnd->m_pcCrtView == NULL){
		return;
	}
	UpdateData(TRUE);
	xmm = atof(m_csWorkXSize);
	ymm = atof(m_csWorkYSize);
	if(0 < xmm && 0 < ymm){
		scrnsetmmsize(xmm, ymm);
	}
	pcWnd->m_pcCrtView->WorkSizeReset();
	pcWnd->m_pcCrtView->WorkAreaReset();
	pcWnd->m_pcCrtView->Invalidate(TRUE);
}
