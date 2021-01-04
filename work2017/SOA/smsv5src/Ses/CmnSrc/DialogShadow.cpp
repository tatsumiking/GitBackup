// DlgShdw.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogShadow.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CDialogShadow ダイアログ


CDialogShadow::CDialogShadow(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogShadow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogShadow)
	m_dDistance = 20.0;
	m_dRoundRag = 45.0;
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
	m_nFirstFlag = TRUE;
}

CDialogShadow::~CDialogShadow()
{
	m_fntImg.DeleteObject();
}

void CDialogShadow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogShadow)
	DDX_Control(pDX, IDC_SPROUNDRAG, m_spRoundrag);
	DDX_Control(pDX, IDC_SPDISTANCE, m_spDistance);
	DDX_Text(pDX, IDC_EDDISTANCE, m_dDistance);
	DDX_Text(pDX, IDC_EDROUNDRAG, m_dRoundRag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogShadow, CDialog)
	//{{AFX_MSG_MAP(CDialogShadow)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_CBSHADOWKIND, OnSelchangeCbshadowkind)
	ON_BN_CLICKED(IDC_STCLR, OnStclr)
	ON_EN_CHANGE(IDC_EDDISTANCE, OnChangeEddistance)
	ON_EN_CHANGE(IDC_EDROUNDRAG, OnChangeEdroundrag)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPDISTANCE, OnDeltaposSpdistance)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPROUNDRAG, OnDeltaposSproundrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogShadow メッセージ ハンドラ

BOOL CDialogShadow::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CComboBox *pcCB = (CComboBox*)GetDlgItem(IDC_CBSHADOWKIND);
	pcCB->AddString(_T("立体の影１"));
	pcCB->AddString(_T("立体の影２"));
	pcCB->SetCurSel(m_nKind-1);

	m_spDistance.SetRange(-SHORTMAX, SHORTMAX);
	m_spRoundrag.SetRange(-360, 360);

	return TRUE;
}

void CDialogShadow::SampleDataOut(CDC *pcDC)
{
	CRect	rect;
	DBL		dxmm, dymm;
	DBL		movex, movey;
	DBL		sx, sy;
	int		last, i;
	DBL		stepx, stepy;
	int		pno, rval, gval, bval;
	DBL		drval, dgval, dbval;

	//UpdateData(TRUE);
	CStatic *pcST = (CStatic*)GetDlgItem(IDC_STIMAGE);
	pcST->GetWindowRect(&rect);
	GetRltvRect(&rect);

	CBrush *pcBrush = new CBrush(RGB(255, 255, 255));
	pcDC->FillRect(&rect, pcBrush);
	delete(pcBrush);

	pcDC->SetBkMode(TRANSPARENT);
	dxmm = rect.Width();
	dymm = rect.Height();

	rval = GetRValue(m_clrShadow);
	gval = GetGValue(m_clrShadow);
	bval = GetBValue(m_clrShadow);
	switch(m_nKind){
	case 1:
		pcDC->SetTextColor(RGB(rval, gval, bval));
		movex = cos(-m_dRoundRag*M_1RAG) * m_dDistance * m_dTime;
		movey = sin(-m_dRoundRag*M_1RAG) * m_dDistance * m_dTime;
		if(fabs(movex) > fabs(movey)){
			last = abs((int)movex);
			stepx = movex / last;
			stepy = movey / last;
		}
		else{
			last = abs((int)movey);
			stepx = movex / last;
			stepy = movey / last;
		}
		sx = dxmm / 4 + movex;
		sy = dymm / 4 + movey;
		for(i = 0; i < last; i++){
			pcDC->TextOut((int)sx, (int)sy, _T("あ"), 2);
			sx -= stepx;
			sy -= stepy;
		}
		break;
	case 2:
		movex = cos(-m_dRoundRag*M_1RAG) * m_dDistance * m_dTime;
		movey = sin(-m_dRoundRag*M_1RAG) * m_dDistance * m_dTime;
		if(fabs(movex) > fabs(movey)){
			last = abs((int)movex);
			stepx = movex / last;
			stepy = movey / last;
		}
		else{
			last = abs((int)movey);
			stepx = movex / last;
			stepy = movey / last;
		}
		drval = (DBL)(255-rval) / (DBL)last;
		dgval = (DBL)(255-gval) / (DBL)last;
		dbval = (DBL)(255-bval) / (DBL)last;
		sx = dxmm / 4 + movex;
		sy = dymm / 4 + movey;
		for(i = 0; i < last; i++){
			rval = 255 - (int)(drval * i);
			gval = 255 - (int)(dgval * i);
			bval = 255 - (int)(dbval * i);
			pcDC->SetTextColor(RGB(rval, gval, bval));
			pcDC->TextOut((int)sx, (int)sy, _T("あ"), 2);
			sx -= stepx;
			sy -= stepy;
		}
		break;
	}

	sx = dxmm / 4;
	sy = dymm / 4;
	pno = senvgetcrtfillpenno();
	senvgetpenrgb(pno, &rval, &gval, &bval);
	pcDC->SetTextColor(RGB(rval, gval, bval));
	pcDC->TextOut((int)sx, (int)sy, _T("あ"), 2);
}

void CDialogShadow::OnPaint()
{
	CStatic	*pcST;
	CRect	rect;
	int		size;
	CFont	*pcOldFont;

	if(m_nFirstFlag == TRUE){
		m_nFirstFlag = FALSE;
		pcST = (CStatic*)GetDlgItem(IDC_STIMAGE);
		pcST->GetWindowRect(&rect);
		size = rect.Width() / 2;
		m_fntImg.CreateFont(size, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, _T("ＭＳ ゴシック"));
		m_dTime = size / m_dOrgSize;
	}
	CDialog::OnPaint();
	CDC* pcDC = GetDC();
	pcOldFont = pcDC->SelectObject(&m_fntImg);

	pcST = (CStatic*)GetDlgItem(IDC_STCLR);
	pcST->GetWindowRect(&rect);
	GetRltvRect(&rect);
	CBrush *pcBrush = new CBrush(m_clrShadow);
	pcDC->FillRect(&rect, pcBrush);
	delete(pcBrush);

	SampleDataOut(pcDC);

	pcDC->SelectObject(&pcOldFont);
	ReleaseDC(pcDC);
}

void CDialogShadow::OnSelchangeCbshadowkind() 
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CBSHADOWKIND);
	m_nKind = pcCB->GetCurSel()+1;
	Invalidate(TRUE);
}

void CDialogShadow::OnOK() 
{
	UpdateData(TRUE);
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CBSHADOWKIND);
	m_nKind = pCB->GetCurSel()+1;
	CDialog::OnOK();
}

void CDialogShadow::OnStclr() 
{
	CColorDialog *pcColorDialog = new CColorDialog(m_clrShadow, CC_FULLOPEN, NULL);
	if(pcColorDialog->DoModal() == IDOK){
		m_clrShadow = pcColorDialog->GetColor();
	}
	delete(pcColorDialog);
	Invalidate(TRUE);
}

void CDialogShadow::GetRltvRect(CRect *rect)
{
	CRect toprect;

	CStatic *pcSTTop = (CStatic*)GetDlgItem(IDC_STTOP);
	pcSTTop->GetWindowRect(&toprect);
	rect->left = rect->left - toprect.left;
	rect->top = rect->top - toprect.top;
	rect->right = rect->right - toprect.left;
	rect->bottom = rect->bottom - toprect.top;
}

void CDialogShadow::OnChangeEddistance() 
{
	UpdateData(TRUE);
	Invalidate(TRUE);
}

void CDialogShadow::OnChangeEdroundrag() 
{
	UpdateData(TRUE);
	Invalidate(TRUE);
}

void CDialogShadow::OnDeltaposSpdistance(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_dDistance -= (DBL)(pNMUpDown->iDelta);
	if(m_dDistance < 0){
		m_dDistance = 0.1;
	}
	*pResult = 0;
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogShadow::OnDeltaposSproundrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_dRoundRag -= (DBL)(pNMUpDown->iDelta);
	*pResult = 0;
	UpdateData(FALSE);
	Invalidate(TRUE);
}

BOOL CDialogShadow::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN
	|| pMsg->message == WM_SYSKEYDOWN){
		switch(pMsg->wParam){
		case VK_RETURN:
			pMsg->wParam = NULL;
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
