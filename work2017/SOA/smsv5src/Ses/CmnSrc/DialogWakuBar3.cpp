// DialogWakuBar3.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWakuBar3.h"

#include "DialogShadow.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Mojienv.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar3 ダイアログ

CDialogWakuBar3::CDialogWakuBar3(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	m_pcApp = (CTonApp*)::AfxGetApp();
	m_pcWnd = NULL;
	m_pcView = NULL;
	m_nFirstFlag = ON;
	//{{AFX_DATA_INIT(CDialogWakuBar3)
	m_csSizeX = _T("");
	m_csSizeY = _T("");
	m_csStartX = _T("");
	m_csStartY = _T("");
	m_csInText = _T("");
	m_nShadow = FALSE;
	//}}AFX_DATA_INIT
}


void CDialogWakuBar3::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuBar3)
	DDX_Text(pDX, IDC_EDSIZEX, m_csSizeX);
	DDX_Text(pDX, IDC_EDSIZEY, m_csSizeY);
	DDX_Text(pDX, IDC_EDSTARTX, m_csStartX);
	DDX_Text(pDX, IDC_EDSTARTY, m_csStartY);
	DDX_Text(pDX, IDC_EDINTEXT, m_csInText);
	DDX_Check(pDX, IDC_CHK_SHADOW, m_nShadow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuBar3, CDialogBar)
	//{{AFX_MSG_MAP(CDialogWakuBar3)
	ON_WM_PAINT()
	ON_EN_SETFOCUS(IDC_EDINTEXT, OnSetfocusEdintext)
	ON_EN_KILLFOCUS(IDC_EDINTEXT, OnKillfocusEdintext)
	ON_BN_CLICKED(IDC_CHK_SHADOW, OnChkShadow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CDialogWakuBar3::OnPaint() 
{
	int		ftategaki;
	int		nstartkind;
	int		nconerkind;

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	if(m_nFirstFlag == ON){
		m_nFirstFlag = OFF;
		m_pcWnd = (CMainFrame*)::AfxGetMainWnd();
		m_btnTateYoko.SubclassDlgItem(ID_WAKU_TATEYOKO, this);
		m_btnStart.SubclassDlgItem(ID_WAKU_STARTKIND, this);
		m_btnConer.SubclassDlgItem(ID_WAKU_CONERKIND, this);
		InitItemElement();
	}
	mojienvgettategakiflag(&ftategaki);
	TateYokoFlagBottonSet(ftategaki);
	mojienvgetstartkind(&nstartkind);
	StartKindBottonSet(nstartkind);
	senvgetwakuconerkind(&nconerkind);
	ConerKindBottonSet(nconerkind);
}

void CDialogWakuBar3::InitItemElement()
{
}

void CDialogWakuBar3::GetItemElement()
{
	CSysEnv		*pcSysEnv;

	//if(m_nFirstFlag == ON){
	//	return;
	//}

	UpdateData(TRUE);

	pcSysEnv = m_pcApp->m_pcSysEnv;
	pcSysEnv->m_nShadowKind = m_nShadowKind;
	pcSysEnv->m_clrShadow = m_clrShadow;
	pcSysEnv->m_dShadowDistance = m_dShadowDistance;
	pcSysEnv->m_dShadowRoundRag = m_dShadowRoundRag;
	_tcscpy_s(pcSysEnv->m_szStrg, m_csInText.GetBuffer(RECMAX));
	mojienvgettategakiflag(&(pcSysEnv->m_nTateYoko));
	GetLocate();
}

void CDialogWakuBar3::GetLocate()
{
	CSysEnv		*pcSysEnv;
	DBL			workmmx, workmmy;
	int			nstartkind;
	int			nconerkind;
	DBL			dwidth, dhight;
	DBL			dmmsx, dmmsy;
	DBL			addx, addy;

	pcSysEnv = m_pcApp->m_pcSysEnv;
	if(pcSysEnv->m_dWidth == NOSETMM && pcSysEnv->m_dHight == NOSETMM){
		return;
	}
	scrngetmmsize(&workmmx, &workmmy);
	dwidth = _ttof(m_csSizeX);
	if(dwidth < 0){
		m_csSizeX.Format(_T("%1.1lf"), fabs(dwidth));
		dwidth = fabs(dwidth);
		UpdateData(FALSE);
	}
	dhight = _ttof(m_csSizeY);
	if(dhight < 0){
		m_csSizeY.Format(_T("%1.1lf"), fabs(dhight));
		dhight = fabs(dhight);
		UpdateData(FALSE);
	}

	dmmsx = _ttof(m_csStartX);
	dmmsy = _ttof(m_csStartY);
	pcSysEnv->m_dWidth = dwidth;
	pcSysEnv->m_dHight = dhight;
	senvgetwakuconerkind(&nconerkind);
	mojienvgetstartkind(&nstartkind);
	switch(nconerkind){
	case CNTRCNTR:
		addx = dwidth / 2;
		addy = dhight / 2;
		break;
	case LEFTUP: // 枠支点
		addx = 0;
		addy = dhight;
		break;
	case LEFTDOWN:
		addx = 0;
		addy = 0;
		break;
	case RIGHTDOWN:
		addx = dwidth;
		addy = 0;
		break;
	case RIGHTUP:
		addx = dwidth;
		addy = dhight;
		break;
	}

	switch(nstartkind){
	case RIGHTDOWN:
		dmmsx += addx;
		dmmsy -= addy;
		dmmsx = workmmx - dmmsx;
		break;
	case RIGHTUP:
		dmmsx += addx;
		dmmsy += addy;
		dmmsx = workmmx - dmmsx;
		dmmsy = workmmy - dmmsy;
		break;
	case LEFTUP:
		dmmsx -= addx;
		dmmsy += addy;
		dmmsy = workmmy - dmmsy;
		break;
	case LEFTDOWN:	//作図原点
		dmmsx -= addx;
		dmmsy -= addy;
		break;
	case CNTRCNTR:
		dmmsx += addx;
		dmmsy += addy;
		dmmsx = workmmx / 2 - dmmsx;
		dmmsy = workmmy / 2 - dmmsy;
		break;
	}
	pcSysEnv->m_dSX = dmmsx;
	pcSysEnv->m_dSY = dmmsy;
}

void CDialogWakuBar3::SetItemElement()
{
	CSysEnv		*pcSysEnv;

	//if(m_nFirstFlag == ON){
	//	return;
	//}

	pcSysEnv = m_pcApp->m_pcSysEnv;
	SetLocate();
	m_nShadowKind = pcSysEnv->m_nShadowKind;
	m_clrShadow = pcSysEnv->m_clrShadow;
	m_dShadowDistance = pcSysEnv->m_dShadowDistance;
	m_dShadowRoundRag = pcSysEnv->m_dShadowRoundRag;
	m_csInText = pcSysEnv->m_szStrg;
	mojienvsettategakiflag(pcSysEnv->m_nTateYoko);
	TateYokoFlagBottonSet(pcSysEnv->m_nTateYoko);
	UpdateData(FALSE);
}

void CDialogWakuBar3::SetLocate()
{
	CSysEnv		*pcSysEnv;
	DBL			workmmx, workmmy;
	int			nstartkind;
	int			nconerkind;
	DBL			dwidth, dhight;
	DBL			dmmsx, dmmsy;
	DBL			addx, addy;

	UpdateData(TRUE);

	pcSysEnv = m_pcApp->m_pcSysEnv;
	if(pcSysEnv->m_dWidth == NOSETMM && pcSysEnv->m_dHight == NOSETMM){
		m_csSizeX = _T("");
		m_csSizeY = _T("");
		m_csStartX = _T("");
		m_csStartY = _T("");
		UpdateData(FALSE);
		return;
	}
	scrngetmmsize(&workmmx, &workmmy);
	dwidth = pcSysEnv->m_dWidth;
	dhight = pcSysEnv->m_dHight;
	dmmsx = pcSysEnv->m_dSX;
	dmmsy = pcSysEnv->m_dSY;

	m_csSizeX.Format(_T("%1.1lf"), dwidth);
	m_csSizeY.Format(_T("%1.1lf"), dhight);

	senvgetwakuconerkind(&nconerkind);
	mojienvgetstartkind(&nstartkind);
	switch(nconerkind){
	case CNTRCNTR:
		addx = dwidth / 2;
		addy = dhight / 2;
		break;
	case LEFTUP: // 枠支点
		addx = 0;
		addy = dhight;
		break;
	case LEFTDOWN:
		addx = 0;
		addy = 0;
		break;
	case RIGHTDOWN:
		addx = dwidth;
		addy = 0;
		break;
	case RIGHTUP:
		addx = dwidth;
		addy = dhight;
		break;
	}

	switch(nstartkind){
	case RIGHTDOWN:
		dmmsx = workmmx - dmmsx;
		dmmsx -= addx;
		dmmsy += addy;
		break;
	case RIGHTUP:
		dmmsx = workmmx - dmmsx;
		dmmsy = workmmy - dmmsy;
		dmmsx -= addx;
		dmmsy -= addy;
		break;
	case LEFTUP:
		dmmsy = workmmy - dmmsy;
		dmmsx += addx;
		dmmsy -= addy;
		break;
	case LEFTDOWN:	//作図原点
		dmmsx += addx;
		dmmsy += addy;
		break;
	case CNTRCNTR:
		dmmsx = workmmx / 2 - dmmsx;
		dmmsy = workmmy / 2 - dmmsy;
		dmmsx -= addx;
		dmmsy -= addy;
		break;
	}

	m_csStartX.Format(_T("%1.1lf"), dmmsx);
	m_csStartY.Format(_T("%1.1lf"), dmmsy);

	UpdateData(FALSE);
}

void CDialogWakuBar3::SetCrtView(CTonView * pcView)
{
	m_pcView = pcView;
}

void CDialogWakuBar3::TateyokoChange() 
{
	int		ftategaki;

	mojienvgettategakiflag(&ftategaki);
	if(ftategaki == -1){
		return;
	}
	switch(ftategaki){
	case YOKOGAKI:
		ftategaki = TATEGAKI;
		break;
	case TATEGAKI:
		ftategaki = YOKOOKI;
		break;
	case YOKOOKI:
		ftategaki = TATEOKI;
		break;
	case TATEOKI:
		ftategaki = YOKOGAKI;
		break;
	}
	mojienvsettategakiflag(ftategaki);
	Invalidate(TRUE);
}

void CDialogWakuBar3::StartKindChange() 
{
	int		nstartkind;

	mojienvgetstartkind(&nstartkind);
	switch(nstartkind){
	case LEFTDOWN:
		nstartkind = LEFTUP;
		break;
	case LEFTUP:
		nstartkind = RIGHTUP;
		break;
	case RIGHTUP:
		nstartkind = RIGHTDOWN;
		break;
	case RIGHTDOWN:
		nstartkind = CNTRCNTR;
		break;
	case CNTRCNTR:
		nstartkind = LEFTDOWN;
		break;
	}
	mojienvsetstartkind(nstartkind);
	SetLocate();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogWakuBar3::ConerKindChange() 
{
	int		nconerkind;

	senvgetwakuconerkind(&nconerkind);
	switch(nconerkind){
	case LEFTDOWN:
		nconerkind = LEFTUP;
		break;
	case LEFTUP:
		nconerkind = RIGHTUP;
		break;
	case RIGHTUP:
		nconerkind = RIGHTDOWN;
		break;
	case RIGHTDOWN:
		nconerkind = CNTRCNTR;
		break;
	case CNTRCNTR:
		nconerkind = LEFTDOWN;
		break;
	}
	senvsetwakuconerkind(nconerkind);
	SetLocate();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogWakuBar3::TateYokoFlagBottonSet(int ftategaki)
{
	if(m_nFirstFlag == ON){
		return;
	}
	switch(ftategaki){
	case YOKOGAKI:
		m_btnTateYoko.SetWindowText(_T("横書"));
		break;
	case TATEGAKI:
		m_btnTateYoko.SetWindowText(_T("縦書"));
		break;
	case YOKOOKI:
		m_btnTateYoko.SetWindowText(_T("横置"));
		break;
	case TATEOKI:
		m_btnTateYoko.SetWindowText(_T("縦置"));
		break;
	}
}

void CDialogWakuBar3::StartKindBottonSet(int nstartkind)
{
	switch(nstartkind){
	case LEFTDOWN:
		m_btnStart.LoadBitmaps(IDB_STARTLB);
		break;
	case LEFTUP:
		m_btnStart.LoadBitmaps(IDB_STARTLT);
		break;
	case RIGHTUP:
		m_btnStart.LoadBitmaps(IDB_STARTRT);
		break;
	case RIGHTDOWN:
		m_btnStart.LoadBitmaps(IDB_STARTRB);
		break;
	case CNTRCNTR:
		m_btnStart.LoadBitmaps(IDB_STARTCT);
		break;
	}
}

void CDialogWakuBar3::ConerKindBottonSet(int nconerkind)
{
	switch(nconerkind){
	case LEFTDOWN:
		m_btnConer.LoadBitmaps(IDB_CONERLB);
		break;
	case LEFTUP:
		m_btnConer.LoadBitmaps(IDB_CONERLT);
		break;
	case RIGHTUP:
		m_btnConer.LoadBitmaps(IDB_CONERRT);
		break;
	case RIGHTDOWN:
		m_btnConer.LoadBitmaps(IDB_CONERRB);
		break;
	case CNTRCNTR:
		m_btnConer.LoadBitmaps(IDB_CONERCT);
		break;
	}
}

BOOL CDialogWakuBar3::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){
		if(pMsg->wParam == VK_RETURN){
			UpdateData(TRUE);
			if(m_pcView != NULL){
				GetItemElement();
				m_pcView->CallOnWakuApply();
			}
		}
	}
	return CDialogBar::PreTranslateMessage(pMsg);
}

void CDialogWakuBar3::OnSetfocusEdintext() 
{
	CEdit *pcEd = (CEdit*)GetDlgItem(IDC_EDINTEXT);
	m_hImc = ::ImmCreateContext();
	::ImmAssociateContext(pcEd->m_hWnd, m_hImc);
	::ImmSetOpenStatus(m_hImc, true);
	if(m_pcView != NULL){
		m_pcView->m_fStrgInputMode = ON;
	}
}

void CDialogWakuBar3::OnKillfocusEdintext() 
{
	CEdit* pcEd = (CEdit*)GetDlgItem(IDC_EDINTEXT);
	::ImmAssociateContext(pcEd->m_hWnd, NULL);
	::ImmDestroyContext(m_hImc);
	if(m_pcView != NULL){
		m_pcView->m_fStrgInputMode = OFF;
	}
}

void CDialogWakuBar3::OnChkShadow() 
{
	CDialogShadow	*pcDialogShadow;
	DBL		xsize, ysize;
	int		ret;

	UpdateData(TRUE);
	xsize = _ttof(m_csSizeX);
	ysize = _ttof(m_csSizeY);
	if(xsize == 0 || ysize == 0){
		m_nShadow = FALSE;
		UpdateData(FALSE);
		return;
	}
	if(m_nShadow == TRUE){
		pcDialogShadow = new CDialogShadow(m_pcView);
		if(xsize < ysize){
			pcDialogShadow->m_dOrgSize = xsize;
		}else{
			pcDialogShadow->m_dOrgSize = ysize;
		}
		pcDialogShadow->m_nKind = m_nShadowKind;
		pcDialogShadow->m_clrShadow = m_clrShadow;
		pcDialogShadow->m_dDistance = m_dShadowDistance;
		pcDialogShadow->m_dRoundRag = m_dShadowRoundRag;
		ret = pcDialogShadow->DoModal();
		if(ret == IDOK){
			m_nShadowKind = pcDialogShadow->m_nKind;
			m_clrShadow = pcDialogShadow->m_clrShadow;
			m_dShadowDistance = pcDialogShadow->m_dDistance;
			m_dShadowRoundRag = pcDialogShadow->m_dRoundRag;
		}
		delete(pcDialogShadow);
	}else{
		m_nShadowKind = 0;
	}
	GetItemElement();
	m_pcView->CallOnWakuApply();
}
