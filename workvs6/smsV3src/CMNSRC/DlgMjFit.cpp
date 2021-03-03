// DlgMjFit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgMjFit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiFiting ダイアログ


CDialogMojiFiting::CDialogMojiFiting(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMojiFiting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMojiFiting)
	m_dStep = 0.0;
	m_dXSize = 0.0;
	m_dYSize = 0.0;
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
	m_btnBig.LoadBitmaps(IDB_SIZEBIG1, IDB_SIZEBIG2, IDB_SIZEBIG3);
	m_btnSmall.LoadBitmaps(IDB_SIZESMALL1, IDB_SIZESMALL2, IDB_SIZESMALL3);
	m_btnRLBig.LoadBitmaps(IDB_SIZERLBIG1, IDB_SIZERLBIG2, IDB_SIZERLBIG3);
	m_btnRLSmall.LoadBitmaps(IDB_SIZERLSMALL1, IDB_SIZERLSMALL2, IDB_SIZERLSMALL3);
	m_btnUDBig.LoadBitmaps(IDB_SIZEUDBIG1, IDB_SIZEUDBIG2, IDB_SIZEUDBIG3);
	m_btnUDSmall.LoadBitmaps(IDB_SIZEUDSMALL1, IDB_SIZEUDSMALL2, IDB_SIZEUDSMALL3);
	m_btnUpMove.LoadBitmaps(IDB_MOVEUP1, IDB_MOVEUP2, IDB_MOVEUP3); 
	m_btnDownMove.LoadBitmaps(IDB_MOVEDOWN1, IDB_MOVEDOWN2, IDB_MOVEDOWN3);
	m_btnLeftMove.LoadBitmaps(IDB_MOVELEFT1, IDB_MOVELEFT2, IDB_MOVELEFT3);
	m_btnRightMove.LoadBitmaps(IDB_MOVERIGHT1, IDB_MOVERIGHT2, IDB_MOVERIGHT3);
}

void CDialogMojiFiting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMojiFiting)
	DDX_Text(pDX, IDC_EDSTEP, m_dStep);
	DDX_Text(pDX, IDC_EDXSIZE, m_dXSize);
	DDX_Text(pDX, IDC_EDYSIZE, m_dYSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMojiFiting, CDialog)
	//{{AFX_MSG_MAP(CDialogMojiFiting)
	ON_BN_CLICKED(IDC_BTN_BIG, OnBtnBig)
	ON_BN_CLICKED(IDC_BTN_SMALL, OnBtnSmall)
	ON_BN_CLICKED(IDC_BTN_RLBIG, OnBtnRLBig)
	ON_BN_CLICKED(IDC_BTN_RLSMALL, OnBtnRLSmall)
	ON_BN_CLICKED(IDC_BTN_UDBIG, OnBtnUDBig)
	ON_BN_CLICKED(IDC_BTN_UDSMALL, OnBtnUDSmall)
	ON_BN_CLICKED(IDC_BTN_UPMOVE, OnBtnUpMove)
	ON_BN_CLICKED(IDC_BTN_DOWNMOVE, OnBtnDownMove)
	ON_BN_CLICKED(IDC_BTN_LEFTMOVE, OnBtnLeftMove)
	ON_BN_CLICKED(IDC_BTN_RIGHTMOVE, OnBtnRightMove)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_BN_CLICKED(IDC_SET, OnSet)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPSTEP, OnDeltaposSpstep)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiFiting メッセージ ハンドラ

BOOL CDialogMojiFiting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	VERIFY(m_btnBig.SubclassDlgItem(IDC_BTN_BIG, this));
	VERIFY(m_btnSmall.SubclassDlgItem(IDC_BTN_SMALL, this));
	VERIFY(m_btnRLBig.SubclassDlgItem(IDC_BTN_RLBIG, this));
	VERIFY(m_btnRLSmall.SubclassDlgItem(IDC_BTN_RLSMALL, this));
	VERIFY(m_btnUDBig.SubclassDlgItem(IDC_BTN_UDBIG, this));
	VERIFY(m_btnUDSmall.SubclassDlgItem(IDC_BTN_UDSMALL, this));
	VERIFY(m_btnUpMove.SubclassDlgItem(IDC_BTN_UPMOVE, this));
	VERIFY(m_btnDownMove.SubclassDlgItem(IDC_BTN_DOWNMOVE, this));
	VERIFY(m_btnLeftMove.SubclassDlgItem(IDC_BTN_LEFTMOVE, this));
	VERIFY(m_btnRightMove.SubclassDlgItem(IDC_BTN_RIGHTMOVE, this));
	m_btnBig.SizeToContent();
	m_btnSmall.SizeToContent();
	m_btnRLBig.SizeToContent();
	m_btnRLSmall.SizeToContent();
	m_btnUDBig.SizeToContent();
	m_btnUDSmall.SizeToContent();
	m_btnUpMove.SizeToContent();
	m_btnDownMove.SizeToContent();
	m_btnLeftMove.SizeToContent();
	m_btnRightMove.SizeToContent();
	return TRUE;
}

void CDialogMojiFiting::GetItemElement()
{
	UpdateData(TRUE);
}

void CDialogMojiFiting::SetItemElement()
{
	UpdateData(FALSE);
}

void CDialogMojiFiting::OnBtnBig() 
{
	m_pcView->CommandExec(USER1EVENT, 0, 0);
}

void CDialogMojiFiting::OnBtnSmall() 
{
	m_pcView->CommandExec(USER2EVENT, 0, 0);
}

void CDialogMojiFiting::OnBtnRLBig() 
{
	m_pcView->CommandExec(USER3EVENT, 0, 0);
}

void CDialogMojiFiting::OnBtnRLSmall() 
{
	m_pcView->CommandExec(USER4EVENT, 0, 0);
}

void CDialogMojiFiting::OnBtnUDBig() 
{
	m_pcView->CommandExec(USER5EVENT, 0, 0);
}

void CDialogMojiFiting::OnBtnUDSmall() 
{
	m_pcView->CommandExec(USER6EVENT, 0, 0);
}

void CDialogMojiFiting::OnBtnUpMove() 
{
	m_pcView->CommandExec(USER7EVENT, 0, 0);
}

void CDialogMojiFiting::OnBtnDownMove() 
{
	m_pcView->CommandExec(USER8EVENT, 0, 0);
}

void CDialogMojiFiting::OnBtnLeftMove() 
{
	m_pcView->CommandExec(USER9EVENT, 0, 0);
}

void CDialogMojiFiting::OnBtnRightMove() 
{
	m_pcView->CommandExec(USER10EVENT, 0, 0);
}

void CDialogMojiFiting::OnReset() 
{
	m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
}

void CDialogMojiFiting::OnSet() 
{
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogMojiFiting::OnCancel() 
{
	m_pcView->CommandExec(DLGENDEVENT, 0, 0);
}


void CDialogMojiFiting::OnDeltaposSpstep(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_dStep -= (DBL)(pNMUpDown->iDelta);
	*pResult = 1;
	UpdateData(FALSE);
}
