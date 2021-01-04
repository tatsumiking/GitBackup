// DialogScrnClr.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogScrnClr.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogScrnClr ダイアログ


CDialogScrnClr::CDialogScrnClr(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogScrnClr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogScrnClr)
	m_nDotSize = 0;
	//}}AFX_DATA_INIT
}


void CDialogScrnClr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogScrnClr)
	DDX_Control(pDX, IDC_BEZECLR, m_stBezeClr);
	DDX_Control(pDX, IDC_BKCLR, m_stBkClr);
	DDX_Control(pDX, IDC_FRCLR, m_stFrClr);
	DDX_Control(pDX, IDC_LINECLR, m_stLineClr);
	DDX_Control(pDX, IDC_CURVCLR, m_stCurvClr);
	DDX_Control(pDX, IDC_ATNCLR, m_stAtnClr);
	DDX_Control(pDX, IDC_SLCTCLR, m_stSlctClr);
	DDX_Control(pDX, IDC_NOSLCTCLR, m_stNoslctClr);
	DDX_Control(pDX, IDC_IMAGECLR, m_stImageClr);
	DDX_Control(pDX, IDC_LSLCTCLR, m_stLslctClr);
	DDX_Control(pDX, IDC_SUBLCLR, m_stSublClr);
	DDX_Control(pDX, IDC_AREACLR, m_stAreaClr);
	DDX_Control(pDX, IDC_GRIDCLR, m_stGridClr);
	DDX_Control(pDX, IDC_SCALCLR, m_stScalClr);
	DDX_Control(pDX, IDC_CNTRCLR, m_stCntrClr);
	DDX_Control(pDX, IDC_ASSTCLR, m_stAsstClr);
	DDX_Control(pDX, IDC_TOPRECT, m_stTopRect);
	DDX_Text(pDX, IDC_EDDOTSIZE, m_nDotSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogScrnClr, CDialog)
	//{{AFX_MSG_MAP(CDialogScrnClr)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDDOTSIZE, OnChangeEddotsize)
	ON_BN_CLICKED(IDC_BTNDEFSET, OnBtndefset)
	ON_BN_CLICKED(IDC_BTN_CLRCHANGE, OnBtnClrchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogScrnClr メッセージ ハンドラ

BOOL CDialogScrnClr::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetItemElement();

	ClrBarItemLocate();

	UpdateData(FALSE);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
	
void CDialogScrnClr::ClrBarItemLocate()
{
	CRect	toprect;
	CRect	rect;
	CPoint	point;

	m_stTopRect.GetWindowRect(&toprect);
	point.x = toprect.top;
	point.y = toprect.left;

	m_stBkClr.GetWindowRect(&m_rectBkClr);
	m_rectBkClr.left -= toprect.left;
	m_rectBkClr.right -= toprect.left;
	m_rectBkClr.top -= toprect.top;
	m_rectBkClr.bottom -= toprect.top;

	m_stFrClr.GetWindowRect(&m_rectFrClr);
	m_rectFrClr.left -= toprect.left;
	m_rectFrClr.right -= toprect.left;
	m_rectFrClr.top -= toprect.top;
	m_rectFrClr.bottom -= toprect.top;

	m_stLineClr.GetWindowRect(&m_rectLineClr);
	m_rectLineClr.left -= toprect.left;
	m_rectLineClr.right -= toprect.left;
	m_rectLineClr.top -= toprect.top;
	m_rectLineClr.bottom -= toprect.top;

	m_stCurvClr.GetWindowRect(&m_rectCurvClr);
	m_rectCurvClr.left -= toprect.left;
	m_rectCurvClr.right -= toprect.left;
	m_rectCurvClr.top -= toprect.top;
	m_rectCurvClr.bottom -= toprect.top;

	m_stBezeClr.GetWindowRect(&m_rectBezeClr);
	m_rectBezeClr.left -= toprect.left;
	m_rectBezeClr.right -= toprect.left;
	m_rectBezeClr.top -= toprect.top;
	m_rectBezeClr.bottom -= toprect.top;

	m_stAtnClr.GetWindowRect(&m_rectAtnClr);
	m_rectAtnClr.left -= toprect.left;
	m_rectAtnClr.right -= toprect.left;
	m_rectAtnClr.top -= toprect.top;
	m_rectAtnClr.bottom -= toprect.top;

	m_stSlctClr.GetWindowRect(&m_rectSlctClr);
	m_rectSlctClr.left -= toprect.left;
	m_rectSlctClr.right -= toprect.left;
	m_rectSlctClr.top -= toprect.top;
	m_rectSlctClr.bottom -= toprect.top;

	m_stNoslctClr.GetWindowRect(&m_rectNoslctClr);
	m_rectNoslctClr.left -= toprect.left;
	m_rectNoslctClr.right -= toprect.left;
	m_rectNoslctClr.top -= toprect.top;
	m_rectNoslctClr.bottom -= toprect.top;

	m_stImageClr.GetWindowRect(&m_rectImageClr);
	m_rectImageClr.left -= toprect.left;
	m_rectImageClr.right -= toprect.left;
	m_rectImageClr.top -= toprect.top;
	m_rectImageClr.bottom -= toprect.top;

	m_stLslctClr.GetWindowRect(&m_rectLslctClr);
	m_rectLslctClr.left -= toprect.left;
	m_rectLslctClr.right -= toprect.left;
	m_rectLslctClr.top -= toprect.top;
	m_rectLslctClr.bottom -= toprect.top;

	m_stSublClr.GetWindowRect(&m_rectSublClr);
	m_rectSublClr.left -= toprect.left;
	m_rectSublClr.right -= toprect.left;
	m_rectSublClr.top -= toprect.top;
	m_rectSublClr.bottom -= toprect.top;

	m_stAreaClr.GetWindowRect(&m_rectAreaClr);
	m_rectAreaClr.left -= toprect.left;
	m_rectAreaClr.right -= toprect.left;
	m_rectAreaClr.top -= toprect.top;
	m_rectAreaClr.bottom -= toprect.top;

	m_stGridClr.GetWindowRect(&m_rectGridClr);
	m_rectGridClr.left -= toprect.left;
	m_rectGridClr.right -= toprect.left;
	m_rectGridClr.top -= toprect.top;
	m_rectGridClr.bottom -= toprect.top;

	m_stScalClr.GetWindowRect(&m_rectScalClr);
	m_rectScalClr.left -= toprect.left;
	m_rectScalClr.right -= toprect.left;
	m_rectScalClr.top -= toprect.top;
	m_rectScalClr.bottom -= toprect.top;

	m_stCntrClr.GetWindowRect(&m_rectCntrClr);
	m_rectCntrClr.left -= toprect.left;
	m_rectCntrClr.right -= toprect.left;
	m_rectCntrClr.top -= toprect.top;
	m_rectCntrClr.bottom -= toprect.top;

	m_stAsstClr.GetWindowRect(&m_rectAsstClr);
	m_rectAsstClr.left -= toprect.left;
	m_rectAsstClr.right -= toprect.left;
	m_rectAsstClr.top -= toprect.top;
	m_rectAsstClr.bottom -= toprect.top;

}

void CDialogScrnClr::SetItemElement()
{
	int		rval, gval, bval;

	senvgetbkclr(&rval, &gval, &bval);
	m_rgbBkClr = RGB(rval, gval, bval);
	senvgetfrclr(&rval, &gval, &bval);
	m_rgbFrClr = RGB(rval, gval, bval);
	senvgetlineclr(&rval, &gval, &bval);
	m_rgbLineClr = RGB(rval, gval, bval);
	senvgetcurvclr(&rval, &gval, &bval);
	m_rgbCurvClr = RGB(rval, gval, bval);
	senvgetbezeclr(&rval, &gval, &bval);
	m_rgbBezeClr = RGB(rval, gval, bval);
	senvgetatnclr(&rval, &gval, &bval);
	m_rgbAtnClr = RGB(rval, gval, bval);
	senvgetslctclr(&rval, &gval, &bval);
	m_rgbSlctClr = RGB(rval, gval, bval);
	senvgetnoslctclr(&rval, &gval, &bval);
	m_rgbNoslctClr = RGB(rval, gval, bval);
	senvgetimageclr(&rval, &gval, &bval);
	m_rgbImageClr = RGB(rval, gval, bval);
	senvgetlslctclr(&rval, &gval, &bval);
	m_rgbLslctClr = RGB(rval, gval, bval);
	senvgetsublclr(&rval, &gval, &bval);
	m_rgbSublClr = RGB(rval, gval, bval);
	senvgetareaclr(&rval, &gval, &bval);
	m_rgbAreaClr = RGB(rval, gval, bval);
	senvgetgridclr(&rval, &gval, &bval);
	m_rgbGridClr = RGB(rval, gval, bval);
	senvgetscalclr(&rval, &gval, &bval);
	m_rgbScalClr = RGB(rval, gval, bval);
	senvgetcntrclr(&rval, &gval, &bval);
	m_rgbCntrClr = RGB(rval, gval, bval);
	senvgetasstclr(&rval, &gval, &bval);
	m_rgbAsstClr = RGB(rval, gval, bval);

	senvgetareadotsize(&m_nDotSize);

}

void CDialogScrnClr::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_rectBkClr.PtInRect(point)){
		ClrSelect(&m_rgbBkClr);
	}
	if(m_rectFrClr.PtInRect(point)){
		ClrSelect(&m_rgbFrClr);
	}
	if(m_rectLineClr.PtInRect(point)){
		ClrSelect(&m_rgbLineClr);
	}
	if(m_rectCurvClr.PtInRect(point)){
		ClrSelect(&m_rgbCurvClr);
	}
	if(m_rectBezeClr.PtInRect(point)){
		ClrSelect(&m_rgbBezeClr);
	}
	if(m_rectAtnClr.PtInRect(point)){
		ClrSelect(&m_rgbAtnClr);
	}
	if(m_rectSlctClr.PtInRect(point)){
		ClrSelect(&m_rgbSlctClr);
	}
	if(m_rectNoslctClr.PtInRect(point)){
		ClrSelect(&m_rgbNoslctClr);
	}
	if(m_rectImageClr.PtInRect(point)){
		ClrSelect(&m_rgbImageClr);
	}
	if(m_rectLslctClr.PtInRect(point)){
		ClrSelect(&m_rgbLslctClr);
	}
	if(m_rectSublClr.PtInRect(point)){
		ClrSelect(&m_rgbSublClr);
	}
	if(m_rectAreaClr.PtInRect(point)){
		ClrSelect(&m_rgbAreaClr);
	}
	if(m_rectGridClr.PtInRect(point)){
		ClrSelect(&m_rgbGridClr);
	}
	if(m_rectScalClr.PtInRect(point)){
		ClrSelect(&m_rgbScalClr);
	}
	if(m_rectCntrClr.PtInRect(point)){
		ClrSelect(&m_rgbCntrClr);
	}
	if(m_rectAsstClr.PtInRect(point)){
		ClrSelect(&m_rgbAsstClr);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDialogScrnClr::ClrSelect(long *lRGB)
{
	CColorDialog *pcColorDialog = new CColorDialog(*lRGB, CC_FULLOPEN, NULL);
	if(pcColorDialog->DoModal() == IDOK){
		*lRGB = pcColorDialog->GetColor();
		Invalidate();
	}
	delete(pcColorDialog);
}

void CDialogScrnClr::GetItemElement()
{
	int		rval, gval, bval;

	UpdateData(TRUE);
	GetRGBValue(m_rgbBkClr, &rval, &gval, &bval);
	senvsetbkclr(rval, gval, bval);
	GetRGBValue(m_rgbFrClr, &rval, &gval, &bval);
	senvsetfrclr(rval, gval, bval);
	GetRGBValue(m_rgbLineClr, &rval, &gval, &bval);
	senvsetlineclr(rval, gval, bval);
	GetRGBValue(m_rgbCurvClr, &rval, &gval, &bval);
	senvsetcurvclr(rval, gval, bval);
	GetRGBValue(m_rgbBezeClr, &rval, &gval, &bval);
	senvsetbezeclr(rval, gval, bval);
	GetRGBValue(m_rgbAtnClr, &rval, &gval, &bval);
	senvsetatnclr(rval, gval, bval);
	GetRGBValue(m_rgbSlctClr, &rval, &gval, &bval);
	senvsetslctclr(rval, gval, bval);
	GetRGBValue(m_rgbNoslctClr, &rval, &gval, &bval);
	senvsetnoslctclr(rval, gval, bval);
	GetRGBValue(m_rgbImageClr, &rval, &gval, &bval);
	senvsetimageclr(rval, gval, bval);
	GetRGBValue(m_rgbLslctClr, &rval, &gval, &bval);
	senvsetlslctclr(rval, gval, bval);
	GetRGBValue(m_rgbSublClr, &rval, &gval, &bval);
	senvsetsublclr(rval, gval, bval);
	GetRGBValue(m_rgbAreaClr, &rval, &gval, &bval);
	senvsetareaclr(rval, gval, bval);
	GetRGBValue(m_rgbGridClr, &rval, &gval, &bval);
	senvsetgridclr(rval, gval, bval);
	GetRGBValue(m_rgbScalClr, &rval, &gval, &bval);
	senvsetscalclr(rval, gval, bval);
	GetRGBValue(m_rgbCntrClr, &rval, &gval, &bval);
	senvsetcntrclr(rval, gval, bval);
	GetRGBValue(m_rgbAsstClr, &rval, &gval, &bval);
	senvsetasstclr(rval, gval, bval);

	senvsetareadotsize(m_nDotSize);

}

void CDialogScrnClr::OnOK() 
{
	GetItemElement();
	CDialog::OnOK();
}

void CDialogScrnClr::GetRGBValue(long lRGBClr, int *rval, int *gval, int *bval)
{
	*rval = GetRValue(lRGBClr);
	*gval = GetGValue(lRGBClr);
	*bval = GetBValue(lRGBClr);
}

void CDialogScrnClr::OnPaint() 
{
	CBrush	brush;

	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	brush.CreateSolidBrush(m_rgbBkClr);
	dc.FillRect(&m_rectBkClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbFrClr);
	dc.FillRect(&m_rectFrClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbLineClr);
	dc.FillRect(&m_rectLineClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbCurvClr);
	dc.FillRect(&m_rectCurvClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbBezeClr);
	dc.FillRect(&m_rectBezeClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbAtnClr);
	dc.FillRect(&m_rectAtnClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbSlctClr);
	dc.FillRect(&m_rectSlctClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbNoslctClr);
	dc.FillRect(&m_rectNoslctClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbImageClr);
	dc.FillRect(&m_rectImageClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbLslctClr);
	dc.FillRect(&m_rectLslctClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbSublClr);
	dc.FillRect(&m_rectSublClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbAreaClr);
	dc.FillRect(&m_rectAreaClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbGridClr);
	dc.FillRect(&m_rectGridClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbScalClr);
	dc.FillRect(&m_rectScalClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbCntrClr);
	dc.FillRect(&m_rectCntrClr, &brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(m_rgbAsstClr);
	dc.FillRect(&m_rectAsstClr, &brush);
	brush.DeleteObject();
}

void CDialogScrnClr::OnChangeEddotsize() 
{
}

void CDialogScrnClr::OnBtndefset() 
{
	senvsetdefscrnclr();
	SetItemElement();
	Invalidate();
}

void CDialogScrnClr::OnBtnClrchange() 
{
	m_rgbBkClr = (~m_rgbBkClr) & 0x00ffffff;
	m_rgbFrClr = (~m_rgbFrClr) & 0x00ffffff;
	m_rgbLineClr = (~m_rgbLineClr) & 0x00ffffff;
	m_rgbScalClr = (~m_rgbScalClr) & 0x00ffffff;
	Invalidate();
}
