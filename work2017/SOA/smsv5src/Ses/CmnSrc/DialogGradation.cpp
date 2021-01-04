// DlgGrdtn.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogGradation.h"

//extern "C" {
#include "../../ObjLib/Senvlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	ONEBMPSIZE	64

/////////////////////////////////////////////////////////////////////////////
// CDialogGradation ダイアログ

CDialogGradation::CDialogGradation(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogGradation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogGradation)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
	m_nSelectMark = -1;
}

CDialogGradation::~CDialogGradation()
{
	int		ptn;

	for(ptn = 0; ptn < PTNMAX; ptn++){
		if(m_hDibGraPtn[ptn] != 0){
			GlobalFree(m_hDibGraPtn[ptn]);
		}
	}

	GlobalFree(m_hDib);
}

void CDialogGradation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGradation)
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogGradation, CDialog)
	//{{AFX_MSG_MAP(CDialogGradation)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGradation メッセージ ハンドラ

BOOL CDialogGradation::OnInitDialog() 
{
	CRect	rect;
	CRect	toprect;
	int		ptn, i;
	int		sx, sy;
	int		xsize, ysize;

	CMyDialog::OnInitDialog();
	GetWindowRect(&rect);
	xsize = rect.right - rect.left;
	ysize = rect.bottom - rect.top; 
	m_stTop.GetWindowRect(&toprect);
	m_nTopx = toprect.right - toprect.left;
	m_nTopy = toprect.bottom - toprect.top;
	m_nXSpace = m_nTopx;
	m_nYSpace = m_nTopy;
	m_nBarSx = m_nTopx;
	m_nBarEx = m_nTopx + 256;
	m_nBarSy = m_nTopy + ONEBMPSIZE * 2 + m_nYSpace * 5;

	sx = m_nTopx;
	sy = m_nTopy;
	for(ptn = 0; ptn < PTNMAX; ptn++){
		m_hDibGraPtn[ptn] = 0;
		GraPtnLoad(ptn);
		if(m_hDibGraPtn[ptn] == 0){
			break;
		}
		m_nPtnSx[ptn] = sx;
		m_nPtnSy[ptn] = sy;
		sx += ONEBMPSIZE + m_nXSpace;
		if((sx+ONEBMPSIZE) > xsize){
			sy += ONEBMPSIZE + m_nYSpace;
			sx = m_nTopx;
			//if((sy+ONEBMPSIZE) > ysize){
			//	break;
			//}
		}
	}
	m_nPtnMax = ptn;
	for(; ptn < PTNMAX; ptn++){
		m_hDibGraPtn[ptn] = 0;
	}
	m_nCrtSelect = 0;

	m_nSetx[0] = m_nBarSx;
	m_nSetx[1] = m_nBarEx;
	m_clrSet[0] = RGB(0, 0, 0);
	m_clrSet[1] = RGB(255, 255, 255);
	for(i = 2; i < MARKMAX; i++){
		m_nSetx[i] = m_nBarEx+1;
		m_clrSet[i] = RGB(255, 255, 255);
	}

	Create256Dib();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogGradation::OnPaint() 
{
	int		sx, sy, ex, ey;
	CRect	rect;
	int		ptn, mark;
	CBrush	brush;

	LPBYTE	lpDib;
	LPBYTE	lpbits;
	LPBITMAPFILEHEADER lpbmf;
	LPBITMAPINFOHEADER lpbminfo;
	LPRGBQUAD	lpclrgraptn;
	int		xsize, ysize;

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	dc.SetROP2(R2_COPYPEN);

	SetGraBarPtn();

	for(ptn = 0; ptn < m_nPtnMax; ptn++){
		if(m_hDibGraPtn[ptn] != 0){
			sx = m_nPtnSx[ptn];
			sy = m_nPtnSy[ptn];
			ex = sx + ONEBMPSIZE;
			ey = sy + ONEBMPSIZE;
			lpDib = (LPBYTE)GlobalLock(m_hDibGraPtn[ptn]);
			lpbmf = (LPBITMAPFILEHEADER)(lpDib);
			lpbminfo = (LPBITMAPINFOHEADER)(lpDib + sizeof(BITMAPFILEHEADER));
			if(m_nBitCount[ptn] == 8){
				lpclrgraptn = (LPRGBQUAD)(lpDib + sizeof(BITMAPFILEHEADER)
						+ sizeof(BITMAPINFOHEADER));
				PltCopy(lpclrgraptn);
			}
			xsize = lpbminfo->biWidth;
			ysize = lpbminfo->biHeight;
			lpbits = lpDib + lpbmf->bfOffBits;
			dc.SetStretchBltMode(COLORONCOLOR);
			::StretchDIBits(dc.m_hDC, 
					sx, sy, ONEBMPSIZE, ONEBMPSIZE, 
					0, 0, xsize, ysize, 
					lpbits, (LPBITMAPINFO)lpbminfo, DIB_RGB_COLORS, SRCCOPY);
			GlobalUnlock(m_hDibGraPtn[ptn]);
		}
		if(m_nCrtSelect == ptn){
			rect.left = sx;
			rect.right = sx + ONEBMPSIZE;
			rect.top = sy;
			rect.bottom = sy + ONEBMPSIZE;
			brush.CreateSolidBrush(RGB(255,0,0));
			dc.FrameRect(&rect, &brush);
			brush.DeleteObject();
		}
	}

	for(mark = 0; mark < MARKMAX; mark++){
		if(m_nSetx[mark] < m_nBarSx || m_nBarEx < m_nSetx[mark]){
			continue;
		}
		SelectMarkDraw(&dc, mark);
	}

	xsize = ONEBMPSIZE*4;
	ysize = ONEBMPSIZE/4;
	sx = m_nBarSx;
	sy = m_nBarSy;


	lpDib = (LPBYTE)GlobalLock(m_hDib);

	lpbminfo = (LPBITMAPINFOHEADER)(lpDib + sizeof(BITMAPFILEHEADER));
	lpbits = lpDib + sizeof(BITMAPFILEHEADER)
			+ sizeof(BITMAPINFOHEADER)
			+ sizeof(RGBQUAD)*256;

	StretchDIBits(dc.m_hDC, 
					sx, sy, xsize, ysize, 
					0, 0, xsize, ysize, 
					lpbits, (LPBITMAPINFO)lpbminfo, DIB_RGB_COLORS, SRCCOPY);

	GlobalUnlock(m_hDib);
}

void CDialogGradation::SetGraBarPtn()
{
	LPBYTE	lpDib;
	LPRGBQUAD	lpclr;
	DBL		pv[MARKMAX], tt;
	DBL		rv[MARKMAX], gv[MARKMAX], bv[MARKMAX];
	DBL		stepr, stepg, stepb;
	int		nt, i, j, k, max;
	int		setr, setg, setb;

	for(i = 0, j = 0; i < MARKMAX; i++){
		if(m_nSetx[i] < m_nBarSx || m_nBarEx < m_nSetx[i]){
			continue;
		}
		pv[j] = m_nSetx[i]-m_nBarSx;
		rv[j] = GetRValue(m_clrSet[i]);
		gv[j] = GetGValue(m_clrSet[i]);
		bv[j] = GetBValue(m_clrSet[i]);
		j++;
	}
	max = j;
	for(i = 0; i < max-1; i++){
		for(j = i+1; j < max; j++){
			if(pv[i] > pv[j]){
				tt = pv[i];
				pv[i] = pv[j];
				pv[j] = tt;

				tt = rv[i];
				rv[i] = rv[j];
				rv[j] = tt;

				tt = gv[i];
				gv[i] = gv[j];
				gv[j] = tt;

				tt = bv[i];
				bv[i] = bv[j];
				bv[j] = tt;
			}
		}
	}

	lpDib = (LPBYTE)GlobalLock(m_hDib);

	lpclr = (LPRGBQUAD)(lpDib + sizeof(BITMAPFILEHEADER)
					+ sizeof(BITMAPINFOHEADER));
	for(i = 0; i < max-1; i++){
		nt = (int)(pv[i+1] - pv[i]);
		stepr = (rv[i+1] - rv[i]) / (DBL)nt;
		stepg = (gv[i+1] - gv[i]) / (DBL)nt;
		stepb = (bv[i+1] - bv[i]) / (DBL)nt;
		for(j = 0; j < nt; j++){
			setr = (int)(rv[i] + stepr * j);
			setg = (int)(gv[i] + stepg * j);
			setb = (int)(bv[i] + stepb * j);
			k = (int)(pv[i]+j);
			lpclr[k].rgbBlue = setb;
			lpclr[k].rgbGreen = setg;
			lpclr[k].rgbRed = setr;
			lpclr[k].rgbReserved = 0;
		}
	}
	GlobalUnlock(m_hDib);
}

void CDialogGradation::Create256Dib()
{
	long	xsize, ysize, headsize;
	LPBYTE		lpDib;
	LPBITMAPFILEHEADER lpbmf;
	LPBITMAPINFOHEADER lpbminfo;
	LPRGBQUAD	lpclr;
	LPBYTE		lpbits;
	BYTE		plt;
	long		x, y, ybyte, byte; 

	xsize = ONEBMPSIZE*4;
	ysize = ONEBMPSIZE/2;

	headsize = sizeof(BITMAPFILEHEADER)
					+ sizeof(BITMAPINFOHEADER)
					+ sizeof(RGBQUAD)*256;

	m_sizeDib = headsize + xsize * ysize;
	m_hDib = GlobalAlloc(GHND, m_sizeDib);
	lpDib = (LPBYTE)GlobalLock(m_hDib);

	lpbmf = (LPBITMAPFILEHEADER)(lpDib);
	lpbminfo = (LPBITMAPINFOHEADER)(lpDib + sizeof(BITMAPFILEHEADER));
	lpclr = (LPRGBQUAD)(lpDib + sizeof(BITMAPFILEHEADER)
					+ sizeof(BITMAPINFOHEADER));
	lpbmf->bfType = 0x4d42;
	lpbmf->bfOffBits =  sizeof(BITMAPFILEHEADER)
					+ sizeof(BITMAPINFOHEADER)
					+ sizeof(RGBQUAD)*256;

	lpbits = lpDib + sizeof(BITMAPFILEHEADER)
			+ sizeof(BITMAPINFOHEADER)
			+ sizeof(RGBQUAD)*256;
	lpbmf->bfSize = lpbmf->bfOffBits + xsize * ysize;

	lpbminfo->biSize = sizeof(BITMAPINFOHEADER); // 0x28;
	lpbminfo->biWidth = xsize;
	lpbminfo->biHeight = ysize;
	lpbminfo->biBitCount = 8;
	lpbminfo->biPlanes = 1;
	lpbminfo->biSizeImage = xsize * ysize;
	lpbminfo->biXPelsPerMeter = 72;
	lpbminfo->biYPelsPerMeter = 72;

	for(plt = 0; plt < 256; plt++){
		lpclr[plt].rgbBlue = plt;
		lpclr[plt].rgbGreen = plt;
		lpclr[plt].rgbRed = plt;
		lpclr[plt].rgbReserved = 0;
	}

	for(y = 0; y < ysize; y++){
		ybyte = y * xsize;
		for(x = 0; x < xsize; x++){
			byte = ybyte + x;
			lpbits[byte] = (BYTE)x;
		}
	}

	GlobalUnlock(m_hDib);
}

void CDialogGradation::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	int		ptn, mark;
	int		sx, sy, ex, ey;

	sy = m_nBarSy - m_nYSpace*3;
	ey = m_nBarSy + m_nYSpace;
	if(sy < point.y && point.y < ey){
		for(mark = 0; mark < MARKMAX; mark++){
			sx = m_nSetx[mark] - m_nXSpace;
			ex = m_nSetx[mark] + m_nXSpace;
			if(sx < point.x && point.x < ex){
				if(mark != 0 && mark != 1){
					m_nSelectMark = mark;
				}
				break;
			}
		}
		if(mark == MARKMAX){
			for(mark = 2; mark < MARKMAX; mark++){
				if(m_nSetx[mark] == m_nBarEx+1){
					m_nSetx[mark] = point.x;					
					m_nSelectMark = mark;
					break;
				}
			}
		}
	}
	if(m_nSelectMark == -1){
		for(ptn = 0; ptn < m_nPtnMax; ptn++){
			sx = m_nPtnSx[ptn];
			sy = m_nPtnSy[ptn];
			ex = sx + ONEBMPSIZE;
			ey = sy + ONEBMPSIZE;
			if(sx < point.x && point.x < ex
			&& sy < point.y && point.y < ey){
				m_nCrtSelect = ptn;
				break;
			}
		}
	}
	//Invalidate();
	CDialog::OnLButtonDown(nFlags, point);
}

void CDialogGradation::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogGradation::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	m_pcView->CommandExec(ENDEVENT, 0, 0);
}

void CDialogGradation::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int		mark;
	int		sx, sy, ex, ey;

	for(mark = 0; mark < MARKMAX; mark++){
		sx = m_nSetx[mark] - m_nXSpace;
		ex = m_nSetx[mark] + m_nXSpace;
		sy = m_nBarSy - m_nYSpace*3;
		ey = m_nBarSy + m_nYSpace;

		if(sx < point.x && point.x < ex
		&& sy < point.y && point.y < ey){
			//CColorDialog *pcColorDialog = new CColorDialog(m_clrSet[mark], CC_ANYCOLOR, NULL);
			CColorDialog *pcColorDialog = new CColorDialog(m_clrSet[mark], CC_FULLOPEN, NULL);
			if(pcColorDialog->DoModal() == IDOK){
				m_clrSet[mark] = pcColorDialog->GetColor();
			}
			delete(pcColorDialog);
			break;
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
	Invalidate();
}

void CDialogGradation::OnMouseMove(UINT nFlags, CPoint point) 
{
	int		sy, ey;
	CRect	rect;

	sy = m_nBarSy - m_nYSpace*3;
	ey = m_nBarSy;
	if(sy < point.y && point.y < ey
	&& m_nSelectMark != -1){
		m_nSetx[m_nSelectMark] = point.x;
		if(m_nBarSx > m_nSetx[m_nSelectMark]){
			m_nSetx[m_nSelectMark] = m_nBarSx;
		}
		if(m_nBarEx+1 < m_nSetx[m_nSelectMark]){
			m_nSetx[m_nSelectMark] = m_nBarEx+1;
		}
		rect.left = m_nBarSx-m_nXSpace;
		rect.right = m_nBarEx+m_nXSpace;
		rect.top = sy;
		rect.bottom = ey+m_nYSpace*2;
		InvalidateRect(&rect);
	}	
	CDialog::OnMouseMove(nFlags, point);
}

void CDialogGradation::SelectMarkDraw(CDC *pcdc, int mark)
{
	int		x1, y1, x2, y2;
	CBrush	brush;
	CBrush	*pcoldbrush;

	x1 = m_nSetx[mark] - m_nXSpace/2;
	y1 = m_nBarSy - m_nYSpace*2;
	x2 = m_nSetx[mark] + m_nXSpace/2;
	y2 = m_nBarSy;
		
	brush.CreateSolidBrush(m_clrSet[mark]);
	pcoldbrush = pcdc->SelectObject(&brush);
	pcdc->Ellipse(x1, y1, x2, y2);
	pcdc->SelectObject(pcoldbrush);
	brush.DeleteObject();
}

void CDialogGradation::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	m_nSelectMark = -1;
	CDialog::OnLButtonUp(nFlags, point);
	Invalidate();
}

void CDialogGradation::GraPtnLoad(int ptn)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	CFile	cpFile;
	int		ret;
	LPBYTE	lpDib;
	BITMAPFILEHEADER bmf;
	LPBITMAPINFOHEADER lpbminfo;
	ULONG	lFileSize;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\defenv\\GraBase%d.bmp"), exepath, ptn+1);
	if((cpFile.Open(fname, CFile::modeRead)) == FALSE){
		return;
	}
	ret = cpFile.Read(&bmf, sizeof(bmf));
	if(ret != sizeof(bmf) || bmf.bfType != 0x4d42){
		goto ErrorEnd;
	}
	lFileSize = (ULONG)cpFile.GetLength();
	m_hDibGraPtn[ptn] = ::GlobalAlloc(GHND, lFileSize);
	if(m_hDibGraPtn[ptn] != 0){
		lpDib = (LPBYTE)::GlobalLock(m_hDibGraPtn[ptn]);
		if(lpDib != NULL){
			cpFile.SeekToBegin();
			cpFile.Read(lpDib, lFileSize);
			lpbminfo = (LPBITMAPINFOHEADER)(lpDib + sizeof(BITMAPFILEHEADER));
			m_nBitCount[ptn] = lpbminfo->biBitCount;
			::GlobalUnlock(m_hDibGraPtn[ptn]);
		}
		else{
			::GlobalFree(m_hDibGraPtn[ptn]);
			m_hDibGraPtn[ptn] = 0;
		}
	}
ErrorEnd:
	cpFile.Close();
}

void CDialogGradation::PltCopy(LPRGBQUAD lpclrgraptn)
{
	LPBYTE		lpDib;
	LPRGBQUAD	lpclr;
	int		plt;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	lpclr = (LPRGBQUAD)(lpDib + sizeof(BITMAPFILEHEADER)
					+ sizeof(BITMAPINFOHEADER));
	for(plt = 0; plt < 256; plt++){
		lpclrgraptn[plt].rgbBlue = lpclr[plt].rgbBlue;
		lpclrgraptn[plt].rgbGreen = lpclr[plt].rgbGreen;
		lpclrgraptn[plt].rgbRed =lpclr[plt].rgbRed;
		lpclrgraptn[plt].rgbReserved = lpclr[plt].rgbReserved;
	}
	GlobalUnlock(m_hDib);
}
