// DWkBarG1.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWakuBarG1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBarG1 ダイアログ
//extern "C" {
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Scrnlib.h"
//}

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
	m_csWorkXSize.Format(_T("%1.1lf"), xmm);
	m_csWorkYSize.Format(_T("%1.1lf"), ymm);
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
	xmm = _ttof(m_csWorkXSize);
	ymm = _ttof(m_csWorkYSize);
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
	xmm = _ttof(m_csWorkXSize);
	ymm = _ttof(m_csWorkYSize);
	if(0 < xmm && 0 < ymm){
		scrnsetmmsize(xmm, ymm);
	}
	pcWnd->m_pcCrtView->WorkSizeReset();
	pcWnd->m_pcCrtView->WorkAreaReset();
	pcWnd->m_pcCrtView->Invalidate(TRUE);
}
