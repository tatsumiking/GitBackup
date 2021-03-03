// DlgTrImg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgTrImg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTraceImage ダイアログ
extern "C" {
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
}

CDialogTraceImage::CDialogTraceImage(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogTraceImage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTraceImage)
	m_nAreaLimit = 0;
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;

	m_szBmpFname[0] = '\0';
	m_hDib = 0;
}

CDialogTraceImage::~CDialogTraceImage()
{
	if(m_hDib != 0){
		GlobalFree(m_hDib);
	}
}

void CDialogTraceImage::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTraceImage)
	DDX_Control(pDX, IDC_SLIDER1, m_slidAreaLimit);
	DDX_Text(pDX, IDC_EDIT1, m_nAreaLimit);
	DDV_MinMaxInt(pDX, m_nAreaLimit, 0, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTraceImage, CMyDialog)
	//{{AFX_MSG_MAP(CDialogTraceImage)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_LINE_TRACE, OnLineTrace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTraceImage メッセージ ハンドラ

BOOL CDialogTraceImage::OnInitDialog() 
{
	CMyDialog::OnInitDialog();
	m_clrPicup = 0;
	m_hDib = 0;

	m_slidAreaLimit.SetRange(0, 255);
	m_slidAreaLimit.SetPos(16);
	m_slidAreaLimit.SetTicFreq(16);
	m_nAreaLimit = 16;

	PicupClrDraw();
	
	CRect	imgrect;
	int		xsize, ysize, xbyte;
	long	size;

	CStatic *pcSTImg = (CStatic*)GetDlgItem(IDC_STIMAGE_AREA);
	pcSTImg->GetWindowRect(&imgrect);
	xsize = imgrect.right - imgrect.left;
	ysize = imgrect.bottom - imgrect.top;
	xbyte = (((DWORD)(xsize * 8 + 7) / 8) + 3L) / 4L * 4L;
	size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;
	size = size + xbyte * ysize;
	m_hDib = GlobalAlloc(GHND, size);

	UpdateData(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogTraceImage::SetClr(COLORREF picupClr)
{
	m_clrPicup = picupClr;
	PicupClrDraw();
}

void CDialogTraceImage::InitItemElement()
{

}

void CDialogTraceImage::SetItemElement()
{

}

void CDialogTraceImage::KeyIn(UINT nChar, UINT nID)
{

}

void CDialogTraceImage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	clrrect;
	CRect	toprect;
	int		xsize, ysize;
	DBL		sx, sy, ex, ey;
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CStatic *pcSTClr = (CStatic*)GetDlgItem(IDC_STSAMPLE_CLR);
	CStatic *pcSTTop = (CStatic*)GetDlgItem(IDC_STAREA_TOP);
	pcSTTop->GetWindowRect(&toprect);
	pcSTClr->GetWindowRect(&clrrect);
	xsize = clrrect.right - clrrect.left;
	ysize = clrrect.bottom - clrrect.top;
	sx = clrrect.left - toprect.left;
	sy = clrrect.top - toprect.top;
	ex = sx+xsize;
	ey = sy+ysize;
	
	if(sx < point.x && point.x < ex
	&& sy < point.y && point.y < ey){
		CColorDialog *pcColorDialog = new CColorDialog(m_clrPicup, CC_ANYCOLOR, NULL);
		//CColorDialog *pcColorDialog = new CColorDialog(m_clrPicup, CC_FULLOPEN|CC_RGBINIT, NULL);
		if(pcColorDialog->DoModal() == IDOK){
			m_clrPicup = pcColorDialog->GetColor();
			PicupClrDraw();
			m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
		}
		delete(pcColorDialog);
	}
	CMyDialog::OnLButtonDown(nFlags, point);
}

void CDialogTraceImage::PicupClrDraw()
{
	Invalidate();
	//CPaintDC dc(this); // 描画用のデバイス コンテキスト
}

void CDialogTraceImage::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	CBrush	*pcBrush;
	CRect	clrrect;
	CRect	imgrect;
	CRect	toprect;
	int		xsize, ysize;
	LPBYTE	lpDib, lpBits;
	LPBITMAPFILEHEADER lpbmf;
	LPBITMAPINFOHEADER lpbminfo;
	int		nssx, nssy, nsw, nsh;
	int		ndsx, ndsy;

	CStatic *pcSTImg = (CStatic*)GetDlgItem(IDC_STIMAGE_AREA);
	CStatic *pcSTClr = (CStatic*)GetDlgItem(IDC_STSAMPLE_CLR);
	CStatic *pcSTTop = (CStatic*)GetDlgItem(IDC_STAREA_TOP);
	pcSTTop->GetWindowRect(&toprect);
	pcSTClr->GetWindowRect(&clrrect);
	xsize = clrrect.right - clrrect.left;
	ysize = clrrect.bottom - clrrect.top;
	clrrect.left = clrrect.left - toprect.left;
	clrrect.top = clrrect.top - toprect.top;
	clrrect.right = clrrect.left+xsize;
	clrrect.bottom = clrrect.top+ysize;
	pcBrush = new CBrush(m_clrPicup);
	dc.FillRect(&clrrect, pcBrush);
	delete pcBrush;

	if(m_hDib == 0){
		return;
	}

	pcSTImg->GetWindowRect(&imgrect);
	ndsx = imgrect.left - toprect.left;
	ndsy = imgrect.top - toprect.top;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		return;
	}

	lpbmf = (LPBITMAPFILEHEADER)lpDib;
	lpbminfo = (LPBITMAPINFOHEADER)(lpDib + sizeof(BITMAPFILEHEADER));
	lpBits = (LPBYTE)(lpDib + lpbmf->bfOffBits);
	nssx = 0;
	nssy = 0;
	nsw = lpbminfo->biWidth;
	nsh = lpbminfo->biHeight;
	StretchDIBits(dc.m_hDC, 
					ndsx, ndsy, nsw, nsh, 
					nssx, nssy, nsw, nsh, 
					lpBits, (LPBITMAPINFO)lpbminfo, DIB_RGB_COLORS, SRCCOPY);
	GlobalUnlock(m_hDib);

	// 描画用メッセージとして CMyDialog::OnPaint() を呼び出してはいけません
}

void CDialogTraceImage::GetImageAreaSize(LPDBL xsize, LPDBL ysize)
{
	CRect	imgrect;

	CStatic *pcSTImg = (CStatic*)GetDlgItem(IDC_STIMAGE_AREA);
	pcSTImg->GetWindowRect(&imgrect);
	*xsize = imgrect.right - imgrect.left;
	*ysize = imgrect.bottom - imgrect.top;
}

void CDialogTraceImage::SetImage(LPSTR lpBmpFname)
{
	CFile	cpFile;
	LPBYTE	lpDib;
	ULONG	filesize;

	strcpy(m_szBmpFname, lpBmpFname);
	if((cpFile.Open(m_szBmpFname, CFile::modeRead)) == FALSE){
		return;
	}

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		goto ErrorEnd;
	}
	filesize = cpFile.GetLength();
	cpFile.SeekToBegin();
	cpFile.ReadHuge(lpDib, filesize);
	GlobalUnlock(m_hDib);

ErrorEnd:
	cpFile.Close();

	ChengRGBQUAD();

	return;
}

void CDialogTraceImage::OnChangeEdit1() 
{
	UpdateData(TRUE);
	m_slidAreaLimit.SetPos(m_nAreaLimit); 
}

void CDialogTraceImage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(nSBCode == SB_ENDSCROLL){
		if((CSliderCtrl*)pScrollBar == &m_slidAreaLimit){
			m_nAreaLimit = m_slidAreaLimit.GetPos();
			UpdateData(FALSE);
			ChengRGBQUAD();
		}
	}
	CMyDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDialogTraceImage::ChengRGBQUAD()
{
	LPBYTE		lpDib;
	LPRGBQUAD	lpRGBQuad;
	int			idx;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	if(lpDib == NULL){
		return;
	}
	lpRGBQuad = (RGBQUAD FAR *)(lpDib + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	for(idx = 0; idx <= m_nAreaLimit; idx++){
		lpRGBQuad[idx].rgbBlue = 0;
		lpRGBQuad[idx].rgbGreen = 0;
		lpRGBQuad[idx].rgbRed = 0;
		lpRGBQuad[idx].rgbReserved = 0;
	}
	for(; idx < 256; idx++){
		lpRGBQuad[idx].rgbBlue = 255;
		lpRGBQuad[idx].rgbGreen = 255;
		lpRGBQuad[idx].rgbRed = 255;
		lpRGBQuad[idx].rgbReserved = 0;
	}
	GlobalUnlock(m_hDib);
	Invalidate();
}

void CDialogTraceImage::OnLineTrace() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogTraceImage::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	m_pcView->CommandExec(DLGENDEVENT, 0, 0);
}
