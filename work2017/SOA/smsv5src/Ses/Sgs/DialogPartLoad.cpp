// DPrtLoad.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogPartLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	IMGSIZE		64
#define IMGSPACE	16

/////////////////////////////////////////////////////////////////////////////
// CDialogPartLoad ダイアログ
//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Senvlib.h"
//}


CDialogPartLoad::CDialogPartLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPartLoad::IDD, pParent)
{
	m_pcWnd = (CMainFrame*)::AfxGetMainWnd();
	m_pcView = (CTonView *)pParent;
	m_nFirstFlag = TRUE;
	m_nCrtTop = 0;
	m_nLstMax = 0;
	m_hDib = 0;
	//{{AFX_DATA_INIT(CDialogPartLoad)
	//}}AFX_DATA_INIT
}


void CDialogPartLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPartLoad)
	DDX_Control(pDX, IDC_LSTIMG, m_lstImg);
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_STIMG, m_stImg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPartLoad, CDialog)
	//{{AFX_MSG_MAP(CDialogPartLoad)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BEFORPAGE, OnBeforpage)
	ON_BN_CLICKED(IDC_NEXTPAGE, OnNextpage)
	ON_WM_LBUTTONDOWN()
	ON_LBN_SELCHANGE(IDC_LSTIMG, OnSelchangeLstimg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPartLoad メッセージ ハンドラ
void CDialogPartLoad::OnPaint() 
{
	CRect	rectImg;

	if(m_nFirstFlag == TRUE){
		m_nFirstFlag = FALSE;
	}
	CPaintDC dc(this);
	m_stImg.GetWindowRect(&rectImg);
	LegalRect(&rectImg);
	DrawPartImg(&dc, rectImg);
}

BOOL CDialogPartLoad::OnInitDialog() 
{
	CRect	rectDlg;
	CRect	rectImg;
	int		x, y, cx, cy;

	CDialog::OnInitDialog();

	CMainFrame *pcWnd = (CMainFrame*)::AfxGetMainWnd();
	pcWnd->GetWindowRect(&rectDlg);
	rectDlg.top = 0;
	rectDlg.left = 0;
	MoveWindow(rectDlg, TRUE);

	GetWindowRect(&rectDlg);
	m_lstImg.GetWindowRect(&rectImg);
	LegalRect(&rectImg);
	x = rectImg.left;
	y = rectImg.top;
	cx = rectImg.Width();
	cy = rectDlg.Height() - (rectImg.top + 48);
	m_lstImg.SetWindowPos(0, x, y, cx, cy, SWP_NOOWNERZORDER);
	m_lstImg.Invalidate(TRUE);

	m_stImg.GetWindowRect(&rectImg);
	LegalRect(&rectImg);
	x = rectImg.left;
	y = rectImg.top;
	cx = rectDlg.Width() - (rectImg.left + 48);
	cy = rectDlg.Height() - (rectImg.top + 48);
	m_stImg.SetWindowPos(0, x, y, cx, cy, SWP_NOOWNERZORDER);
	m_stImg.Invalidate(TRUE);

	InitListImage();
	return TRUE;
}

void CDialogPartLoad::OnBeforpage() 
{
	m_nCrtTop -= (m_nPageMax/2);
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate(TRUE);
}

void CDialogPartLoad::OnNextpage() 
{
	m_nCrtTop += (m_nPageMax/2);
	if((m_nLstMax - m_nPageMax/2) < m_nCrtTop){
		m_nCrtTop = m_nLstMax - m_nPageMax/2;
	}
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate(TRUE);
}


void CDialogPartLoad::OnSelchangeLstimg() 
{
	m_nCrtTop = m_lstImg.GetCurSel();
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate(TRUE);
}

void CDialogPartLoad::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	rect;
	int		idx;
	int		sx, sy, ex, ey;

	CDialog::OnLButtonDown(nFlags, point);
	m_stImg.GetWindowRect(&rect);
	LegalRect(&rect);
	sx = rect.left+2;
	sy = rect.top+2;
	idx = m_nCrtTop;
	while(1){
		ex = sx + IMGSIZE;
		ey = sy + IMGSIZE;
		if(sx <= point.x && point.x <= ex
		&& sy <= point.y && point.y <= ey){
			if(idx < m_nLstMax){
				m_lstImg.GetText(idx, m_szName);
				CDialog::OnOK();
				break;
			}
		}
		sx += (IMGSIZE + IMGSPACE);
		if(rect.right < (sx + IMGSIZE)){
			sx = rect.left+2;
			sy += (IMGSIZE + IMGSPACE);
			if(rect.bottom < (sy + IMGSIZE)){
				break;
			}
		}
		idx++;
	}
}

void CDialogPartLoad::LegalRect(CRect *rect)
{
	CRect	rectTop;

	m_stTop.GetWindowRect(&rectTop);
	rect->left -= rectTop.left;
	rect->right -= rectTop.left;
	rect->top -= rectTop.top;
	rect->bottom -= rectTop.top;
}

void CDialogPartLoad::DrawPartImg(CDC *pcDC, CRect rect)
{
	TCHAR	szPartPath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	int		idx, tidx;
	TCHAR	szName[RECMAX];
	int		sx, sy, ex, ey;
	HANDLE	handle;
	HFONT	hFont, hOldFont;
	TCHAR	szBuf[16];
	ULONG	lImageSize;

    hFont = ::CreateFont((int)(IMGSPACE*0.7), 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, _T("ＭＳ 明朝"));
    hOldFont = (HFONT)::SelectObject(pcDC->m_hDC, hFont);
	senvgetpartpath(szPartPath);
	idx = m_nCrtTop;
	tidx = 0;
	sx = rect.left+2;
	sy = rect.top+2;
	while(1){
		ex = sx + IMGSIZE;
		ey = sy + IMGSIZE;
		BoxsDraw(pcDC, sx, sy, ex, ey);
		if(idx < m_nLstMax){
			m_lstImg.GetText(idx, szName);
			_stprintf_s(szFileName, _T("%s\\%s.bmp"), szPartPath, szName);
		 
			if (_taccess_s(szFileName, 0) == 0) {
				CFile file(szFileName, CFile::modeRead);
				file.Read(szBuf, 2);
				file.Read(&lImageSize, 4);
				file.Close();
				ImageDraw(pcDC, sx, sy, szFileName, lImageSize);
			}

			::TextOut(pcDC->m_hDC, sx+2, ey+2, szName, _tcslen(szName));
		}
		sx += (IMGSIZE + IMGSPACE);
		if(rect.right < (sx + IMGSIZE)){
			sx = rect.left+2;
			sy += (IMGSIZE + IMGSPACE);
			if(rect.bottom < (sy + IMGSIZE)){
				break;
			}
		}
		idx++;
		tidx++;
	}
	m_nPageMax = tidx;
	hFont = (HFONT)::SelectObject(pcDC->m_hDC, hOldFont);
	::DeleteObject(hFont);
}
void CDialogPartLoad::ImageDraw(CDC *pcDC, int sx, int sy, LPTSTR filename, ULONG nSize)
{
	CFile	cpFile;
	LPBYTE	lpDib;
	LPBYTE 	lpInfo;
	LPTSTR	lpBits;
	int		ret;
	DBL	xtime, ytime;
	int dstxsize, dstysize;

	if (m_hDib == NULL) {
		m_nDibSize = nSize;
		m_hDib = ::GlobalAlloc(GHND, m_nDibSize);
	}
	else if (m_nDibSize < nSize) {
		m_nDibSize = nSize;
		m_hDib = ::GlobalReAlloc(m_hDib, m_nDibSize, GHND);
	}
	lpDib = (LPBYTE)::GlobalLock(m_hDib);
	if (lpDib == NULL) {
		return;
	}
	ret = cpFile.Open(filename, CFile::modeRead);
	if (ret == FALSE) {
		::GlobalUnlock(m_hDib);
		return;
	}
	if (cpFile.Read(lpDib, nSize) != nSize) {
		::GlobalUnlock(m_hDib);
		return;
	}
	cpFile.Close();


	lpInfo = lpDib + sizeof(BITMAPFILEHEADER);

	int xsize = ((BITMAPINFOHEADER*)lpInfo)->biWidth;
	int ysize = ((BITMAPINFOHEADER*)lpInfo)->biHeight;

	xtime = (DBL)(IMGSIZE) / (DBL)xsize;
	ytime = (DBL)(IMGSIZE) / (DBL)ysize;
	if (ytime < xtime) {
		xtime = ytime;
	}
	else {
		ytime = xtime;
	}
	dstxsize = (int)((DBL)xsize * xtime);
	dstysize = (int)((DBL)ysize * ytime);
	lpBits = (LPTSTR)(lpDib + ((BITMAPFILEHEADER *)lpDib)->bfOffBits);
	ret = StretchDIBits(pcDC->m_hDC, sx, sy, dstxsize, dstysize,
		0, 0, xsize, ysize, lpBits, (LPBITMAPINFO)lpInfo, DIB_RGB_COLORS, SRCCOPY);
	GlobalUnlock(m_hDib);
	return;
}
void CDialogPartLoad::InitListImage()
{
	TCHAR	szPartPath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;
	TCHAR	szBuf[RECMAX];
	int		idx;

	senvgetpartpath(szPartPath);
	_stprintf_s(szFileName, _T("%s\\partlist.txt"), szPartPath);
	_tfopen_s(&fp, szFileName, _T("r"));
	if(fp == NULL){
		CreatePartList();
		_tfopen_s(&fp, szFileName, _T("r"));
	}
	if(fp == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(baselibCrDelFgets(szBuf, RECMAX, fp) == NULL){
			break;
		}
		idx++;
		baselastspacedel(szBuf);
		m_lstImg.AddString(szBuf);
	}
	fclose(fp);
	m_nLstMax = idx;
}

void CDialogPartLoad::CreatePartList()
{
	TCHAR	szPartPath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	TCHAR	szSrchName[FNAMEMAX];
	CFileFind	finder;
	BOOL	bWorking;
	FILE	*fp;

	senvgetpartpath(szPartPath);
	_stprintf_s(szSrchName, _T("%s\\*.iti"), szPartPath);
	_stprintf_s(szFileName, _T("%s\\partlist.txt"), szPartPath);
	_tfopen_s(&fp, szFileName, _T("w"));
	if (fp == NULL) {
		return;
	}
	bWorking = finder.FindFile(szSrchName);
	while (bWorking){
		bWorking = finder.FindNextFile();
		if(finder.IsDots() == FALSE && finder.IsDirectory() == FALSE){
			_tcscpy_s(szFileName, finder.GetFileName());
			szFileName[_tcslen(szFileName)-4] = '\0';
			_ftprintf_s(fp, _T("%s\n"), szFileName);
		}
	}
	finder.Close();
	fclose(fp);
}

void CDialogPartLoad::BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey)
{
	pcDC->MoveTo(sx-1, sy-1);
	pcDC->LineTo(ex+1, sy-1);
	pcDC->LineTo(ex+1, ey+1);
	pcDC->LineTo(sx-1, ey+1);
	pcDC->LineTo(sx-1, sy-1);
}

