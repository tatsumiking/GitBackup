// DClrBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DClrBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogClrBar ダイアログ
extern "C" {
	int senvgetcrtfillpenno();
	int senvgetcrtlinepenno();
	DBL senvgetcrtlinethick();
	void senvsetcrtfillpenno(int penno);
	void senvsetcrtlinepenno(int penno);
	void senvsetcrtlinethick(DBL thick);
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
	void senvsetpenrgb(int pno, int rval, int gval, int bval);
	void senvgetcrtpltname(LPSTR pltname);
	void senvsetcrtpltname(LPSTR pltname);
	int senvgetpltmax();
	void senvgetexepath(LPSTR path);
	LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR retstr);
}

CDialogClrBar::CDialogClrBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	m_nCrtPltTopNo = 0;
	m_nCrtFillPlt = senvgetcrtfillpenno();
	m_nCrtLinePlt = senvgetcrtlinepenno();
	m_nFirstFlag = ON;
	//{{AFX_DATA_INIT(CDialogClrBar)
	m_csPltFilename = _T("");
	m_dLineThick = 0.1;
	//}}AFX_DATA_INIT
	//m_dLineThick = senvgetcrtlinethick();
}

void CDialogClrBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogClrBar)
	DDX_CBString(pDX, IDC_PLTCOMBO, m_csPltFilename);
	DDX_Text(pDX, IDC_THICK, m_dLineThick);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogClrBar, CDialogBar)
	//{{AFX_MSG_MAP(CDialogClrBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_NEXTBTN, OnNextbtn)
	ON_BN_CLICKED(IDC_PREBTN, OnPrebtn)
	ON_CBN_SELCHANGE(IDC_PLTCOMBO, OnSelchangePltcombo)
	ON_WM_RBUTTONDOWN()
	ON_EN_CHANGE(IDC_THICK, OnChangeThick)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogClrBar メッセージ ハンドラ

int CDialogClrBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDialogClrBar::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	int		idx, plt;
	int		rval, gval, bval;
	CBrush	*pcBrush;
	CRect	rect;
	CRect	rect2;
	CString	csStr;

	if(m_nFirstFlag == ON){
		m_nFirstFlag = OFF;

		m_nCrtFillPlt = senvgetcrtfillpenno();
		m_nCrtLinePlt = senvgetcrtlinepenno();

		cPreBtn.SubclassDlgItem(IDC_PREBTN, this);
		cNextBtn.SubclassDlgItem(IDC_NEXTBTN, this);
		cNonClrBtn.SubclassDlgItem(IDC_NONCLRBTN, this);
		cPreBtn.LoadBitmaps(IDB_PREBTN, 0, 0, 0);
		cNextBtn.LoadBitmaps(IDB_NEXTBTN, 0, 0, 0);
		cNonClrBtn.LoadBitmaps(IDB_NONCLRBTN, 0, 0, 0);
		PltListInit();
		cPreBtn.GetWindowRect(&m_rectPre);
		cNextBtn.GetWindowRect(&m_rectNext);
		cNonClrBtn.GetWindowRect(&m_rectNonClr);
		m_nTopSx = m_rectNonClr.right + XSPACE-XSIZE;
		m_nTopSy = TOPSY;
	}

	for(idx = 0, plt = m_nCrtPltTopNo; idx < PLTMAX; idx++, plt++){
		rect.left = m_nTopSx+idx*XSPACE;
		rect.right = rect.left+XSIZE;
		rect.top = m_nTopSy;
		rect.bottom = rect.top+YSIZE;
		senvgetpenrgb(plt, &rval, &gval, &bval);
		pcBrush = new CBrush(RGB(rval, gval, bval));
		dc.FillRect(&rect, pcBrush);
		delete pcBrush;
		dc.Draw3dRect(&rect, RGB(64, 64, 64), RGB(255, 255, 255));
	}
	DrawSelectPaletBoxs(&dc);
}

void CDialogClrBar::DrawSelectPaletBoxs(CPaintDC *dc)
{
	int		pltmax;
	int		idx, plt;
	CBrush	*pcBrush;
	CPen	*pcPen;
	CPen	*pcTPen;
	int		rval, gval, bval;
	CRect	rect;
	int		sx, sy, ex, ey;

	for(idx = 0, plt = m_nCrtPltTopNo; idx < PLTMAX; idx++, plt++){
		sx = m_nTopSx+idx*XSPACE;
		ex = sx+XSIZE;
		sy = m_nTopSy;
		ey = sy+YSIZE;
		if(plt == m_nCrtFillPlt){
			sx = sx+3;
			ex = ex-3;
			sy = sy+3;
			ey = ey-3;
			senvgetpenrgb(plt, &rval, &gval, &bval);
			rval = 255 ^ rval;
			gval = 255 ^ gval;
			bval = 255 ^ bval;
			pcPen = new CPen(PS_INSIDEFRAME, 1, RGB(rval, gval, bval));
			CPen* OldPen=dc->SelectObject(pcPen);
			dc->MoveTo(sx, sy);
			dc->LineTo(ex, sy);
			dc->LineTo(ex, ey);
			dc->LineTo(sx, ey);
			dc->LineTo(sx, sy);
			dc ->SelectObject(OldPen);
			delete pcPen;
		}
		if(plt == m_nCrtLinePlt){
			sx = sx+1;
			ex = ex-1;
			sy = sy+1;
			ey = ey-1;
			senvgetpenrgb(plt, &rval, &gval, &bval);
			rval = 255 ^ rval;
			gval = 255 ^ gval;
			bval = 255 ^ bval;
			pcPen = new CPen(PS_INSIDEFRAME, 1, RGB(rval, gval, bval));
			CPen* OldPen=dc->SelectObject(pcPen);
			dc->MoveTo(sx, sy);
			dc->LineTo(ex, sy);
			dc->LineTo(ex, ey);
			dc->LineTo(sx, ey);
			dc->LineTo(sx, sy);
			dc ->SelectObject(OldPen);
			delete pcPen;
		}
	}

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	pcBrush = NULL;
	pcPen = NULL;
	pltmax = senvgetpltmax();
	
	for(plt = 0; plt < pltmax; plt++){
		if(plt == m_nCrtLinePlt){
			senvgetpenrgb(plt, &rval, &gval, &bval);
			pcPen = new CPen(PS_INSIDEFRAME, 5, RGB(rval, gval, bval));
		}
		if(plt == m_nCrtFillPlt){
			senvgetpenrgb(plt, &rval, &gval, &bval);
			pcBrush = new CBrush(RGB(rval, gval, bval));
		}
	}

	if(pcBrush == NULL){
		sx = TOPSX;
		ex = sx+XSIZESLCT;
		sy = TOPSY;
		ey = sy+YSIZESLCT;
		pcTPen = new CPen(PS_SOLID, 2, RGB(0, 0, 0));
		CPen* OldPen=dc->SelectObject(pcTPen);
		dc->MoveTo(sx, sy);
		dc->LineTo(ex, ey);
		dc->MoveTo(ex, sy);
		dc->LineTo(sx, ey);
		dc->SelectObject(OldPen);
		delete pcTPen;
	}
	else{
		rect.left = TOPSX;
		rect.right = rect.left+XSIZESLCT;
		rect.top = TOPSY;
		rect.bottom = rect.top+YSIZESLCT;
		dc->FillRect(&rect, pcBrush);
		delete pcBrush;
	}

	if(pcPen == NULL){
		sx = TOPSX;
		ex = sx+XSIZESLCT;
		sy = TOPSY;
		ey = sy+YSIZESLCT;
		pcTPen = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen* OldPen=dc->SelectObject(pcTPen);
		dc->MoveTo(sx, sy);
		dc->LineTo(sx+4, ey);
		dc->MoveTo(sx+4, sy);
		dc->LineTo(sx, ey);

		dc->MoveTo(ex, sy);
		dc->LineTo(ex-4, ey);
		dc->MoveTo(ex-4, sy);
		dc->LineTo(ex, ey);

		dc->MoveTo(sx, sy);
		dc->LineTo(ex, sy+4);
		dc->MoveTo(sx, sy+4);
		dc->LineTo(ex, sy);

		dc->MoveTo(sx, ey);
		dc->LineTo(ex, ey-4);
		dc->MoveTo(sx, ey-4);
		dc->LineTo(ex, ey);

		dc->SelectObject(OldPen);
		delete pcTPen;
	}
	else{
		CPen* OldPen=dc->SelectObject(pcPen);
		sx = TOPSX+2;
		ex = sx + XSIZESLCT-2;
		sy = TOPSY+2;
		ey = sy + YSIZESLCT-2;
		dc->MoveTo(sx, sy);
		dc->LineTo(ex, sy);
		dc->LineTo(ex, ey);
		dc->LineTo(sx, ey);
		dc->LineTo(sx, sy);
		dc->SelectObject(OldPen);
		delete pcPen;
	}
}

void CDialogClrBar::OnNextbtn() 
{
	int		pltmax;

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	pltmax = senvgetpltmax();
	m_nCrtPltTopNo += MOVEPLT;
	if((pltmax - MOVEPLT) < m_nCrtPltTopNo){
		m_nCrtPltTopNo = pltmax - MOVEPLT;
	}
	Invalidate();
}

void CDialogClrBar::OnPrebtn() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_nCrtPltTopNo -= MOVEPLT;
	if(0 > m_nCrtPltTopNo){
		m_nCrtPltTopNo = 0;
	}
	Invalidate();
}

void CDialogClrBar::PltListInit()
{
	char	exepath[FNAMEMAX];
	char	pltlistfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	char	pltname[HRECMAX];
	FILE	*fp;

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PLTCOMBO);
	pCB->AddString("penvalue.env");
	senvgetexepath(exepath);
	sprintf(pltlistfname, "%s\\palet.lst", exepath);
	if((fp = fopen(pltlistfname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, pltname);
		pCB->AddString(pltname);
	}
	fclose(fp);
	sprintf(pltname, "penvalue.env");
	senvsetcrtpltname(pltname);
	m_csPltFilename = _T(pltname);
	UpdateData(FALSE);
}

void CDialogClrBar::OnSelchangePltcombo() 
{
	int		idx;
	char	pltname[HRECMAX];

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_PLTCOMBO);
	idx = pCB->GetCurSel();
	pCB->GetLBText(idx, pltname);
	senvsetcrtpltname(pltname);
	Invalidate();
}

void CDialogClrBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	int		idx, plt;
	CRect	rect;
	int		rval, gval, bval;
	char	buf[HRECMAX];
	CTonApp*	theApp;
	CMainFrame*	theWnd;

	for(idx = 0, plt = m_nCrtPltTopNo; idx < PLTMAX; idx++, plt++){
		rect.left = m_nTopSx+idx*XSPACE;
		rect.right = rect.left+XSIZE;
		rect.top = m_nTopSy;
		rect.bottom = rect.top+YSIZE;
		if(rect.PtInRect(point)){
			senvgetpenrgb(plt, &rval, &gval, &bval);
			sprintf(buf, "(R%03d, G%03d, B%03d)%04d", rval, gval, bval, plt);
			theApp = (CTonApp*)AfxGetApp();
			theWnd = (CMainFrame*)theApp->GetMainWnd();
			theWnd->HelpTextOut(1, buf);
			break;
		}
	}
	CDialogBar::OnMouseMove(nFlags, point);
}

void CDialogClrBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int		idx, plt;
	CRect	rect;

	if(m_rectPre.left < point.x && point.x < m_rectPre.right){
		OnPrebtn();
		return;
	}
	if(m_rectNext.left < point.x && point.x < m_rectNext.right){
		OnNextbtn();		
		return;
	}
	if(m_rectNonClr.left < point.x && point.x < m_rectNonClr.right){
		m_nCrtFillPlt = -1;
		senvsetcrtfillpenno(m_nCrtFillPlt);
		SetFillClr();
		Invalidate();
		return;
	}
	for(idx = 0, plt = m_nCrtPltTopNo; idx < PLTMAX; idx++, plt++){
		rect.left = m_nTopSx+idx*XSPACE;
		rect.right = rect.left+XSIZE;
		rect.top = m_nTopSy;
		rect.bottom = rect.top+YSIZE;
		if(rect.PtInRect(point)){
			m_nCrtFillPlt = plt;
			senvsetcrtfillpenno(m_nCrtFillPlt);
			SetFillClr();
			break;
		}
	}
	
	CDialogBar::OnLButtonDown(nFlags, point);
	Invalidate();
}

void CDialogClrBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	int		idx, plt;
	CRect	rect;
	int		r, g, b;
	COLORREF	clr;

	for(idx = 0, plt = m_nCrtPltTopNo; idx < PLTMAX; idx++, plt++){
		rect.left = m_nTopSx+idx*XSPACE;
		rect.right = rect.left+XSIZE;
		rect.top = m_nTopSy;
		rect.bottom = rect.top+YSIZE;
		if(rect.PtInRect(point)){
			senvgetpenrgb(plt, &r, &g, &b);
			clr = RGB(r, g, b);
			CColorDialog *pcColorDialog = new CColorDialog(clr, CC_FULLOPEN, NULL);
			if(pcColorDialog->DoModal() == IDOK){
				clr = pcColorDialog->GetColor();
				r = GetRValue(clr); 
				g = GetGValue(clr); 
				b = GetBValue(clr);
				senvsetpenrgb(plt, r, g, b);
				m_nCrtLinePlt = plt;
				senvsetcrtlinepenno(m_nCrtLinePlt);
				SetOutLineClr();
				break;
			}
		}
	}
	
	//CDialogBar::OnLButtonDblClk(nFlags, point);
	Invalidate();
}

void CDialogClrBar::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	int		idx, plt;
	CRect	rect;

	if(m_rectNonClr.left < point.x && point.x < m_rectNonClr.right){
		m_nCrtLinePlt = -1;
		senvsetcrtlinepenno(m_nCrtLinePlt);
		SetOutLineClr();
		Invalidate();
		return;
	}
	for(idx = 0, plt = m_nCrtPltTopNo; idx < PLTMAX; idx++, plt++){
		rect.left = m_nTopSx+idx*XSPACE;
		rect.right = rect.left+XSIZE;
		rect.top = m_nTopSy;
		rect.bottom = rect.top+YSIZE;
		if(rect.PtInRect(point)){
			m_nCrtLinePlt = plt;
			senvsetcrtlinepenno(m_nCrtLinePlt);
			SetOutLineClr();
			break;
		}
	}
	
	CDialogBar::OnRButtonDown(nFlags, point);
	Invalidate();
}

void CDialogClrBar::OnChangeThick() 
{
	UpdateData(TRUE);
	senvsetcrtlinethick(m_dLineThick);
}

void CDialogClrBar::SetFillClr()
{
	CTonDoc	*pcDoc;
	CTonView *pcCrtView;
	CTonApp*	pcApp;
	CMainFrame*	pcWnd;

	pcApp = (CTonApp*)AfxGetApp();
	pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	pcCrtView = pcWnd->GetCrtView();
	if(pcCrtView != NULL){
		pcDoc = (CTonDoc*)(pcCrtView->GetDocument());
		pcDoc->SetFillClrFlagOn(ACTIVEFLAG);
		pcCrtView->Invalidate();
	}
}	

void CDialogClrBar::SetOutLineClr()
{
	CTonDoc	*pcDoc;
	CTonView *pcCrtView;
	CTonApp*	pcApp;
	CMainFrame*	pcWnd;

	pcApp = (CTonApp*)AfxGetApp();
	pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	pcCrtView = pcWnd->GetCrtView();
	if(pcCrtView != NULL){
		pcDoc = (CTonDoc*)(pcCrtView->GetDocument());
		pcDoc->SetOutLineClrFlagOn(ACTIVEFLAG);
		pcCrtView->Invalidate();
	}
}

