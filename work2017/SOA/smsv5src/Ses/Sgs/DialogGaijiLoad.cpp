// DGjLoad.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "io.h"
#include "DialogGaijiLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	IMGSIZE		128
#define IMGSPACE	16

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiLoad ダイアログ

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Mojienv.h"
	#include "../../ObjLib/Senvlib.h"
//}

CDialogGaijiLoad::CDialogGaijiLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGaijiLoad::IDD, pParent)
{
	m_pcWnd = (CMainFrame*)::AfxGetMainWnd();
	m_pcView = (CTonView *)pParent;
	m_nFirstFlag = TRUE;

#ifdef UNICODE
	m_nFirstCode = 0x00e000;
	m_nLastCode = 0x00f8ff;
#else
	m_nFirstCode = 9501;
	m_nLastCode = 11494;
#endif
	m_nCrtTop = 0;
	m_hDib = 0;
	//{{AFX_DATA_INIT(CDialogGaijiLoad)
	m_csCode = _T("");
	m_csFont = _T("");
	m_nSortKind = 0;
	//}}AFX_DATA_INIT
}


void CDialogGaijiLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGaijiLoad)
	DDX_Control(pDX, IDC_LSTCODE, m_lstCode);
	DDX_Control(pDX, IDC_CMBFONT, m_cmbFont);
	DDX_Control(pDX, IDC_LSTIMG, m_lstImg);
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_STIMG, m_stImg);
	DDX_Text(pDX, IDC_EDCODE, m_csCode);
	DDX_CBString(pDX, IDC_CMBFONT, m_csFont);
	DDX_Radio(pDX, IDC_RDOCODESORT, m_nSortKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogGaijiLoad, CDialog)
	//{{AFX_MSG_MAP(CDialogGaijiLoad)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BEFORPAGE, OnBeforpage)
	ON_BN_CLICKED(IDC_NEXTPAGE, OnNextpage)
	ON_WM_LBUTTONDOWN()
	ON_LBN_SELCHANGE(IDC_LSTIMG, OnSelchangeLstimg)
	ON_BN_CLICKED(IDC_BTNLOAD, OnBtnload)
	ON_BN_CLICKED(IDC_RDOCODESORT, OnRdocodesort)
	ON_BN_CLICKED(IDC_RDOBUSYUSORT, OnRdobusyusort)
	ON_BN_CLICKED(IDC_RDOKAKUSUSORT, OnRdokakususort)
	ON_CBN_SELCHANGE(IDC_CMBFONT, OnSelchangeCmbfont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiLoad メッセージ ハンドラ

void CDialogGaijiLoad::OnPaint() 
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

BOOL CDialogGaijiLoad::OnInitDialog() 
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

	InitCmbFont();
	InitListImageCode();
	return TRUE;
}

void CDialogGaijiLoad::OnBeforpage() 
{
	m_nCrtTop -= (m_nPageMax/2);
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate(TRUE);
}

void CDialogGaijiLoad::OnNextpage() 
{
	m_nCrtTop += (m_nPageMax/2);
	if((m_nLstMax - (m_nPageMax/2)) < m_nCrtTop){
		m_nCrtTop = m_nLstMax - m_nPageMax/2;
	}
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate(TRUE);
}

void CDialogGaijiLoad::OnRdocodesort() 
{
	m_nSortKind = 0;
	InitListImageCode();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogGaijiLoad::OnRdobusyusort() 
{
	m_nSortKind = 1;
	InitListImageCode();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogGaijiLoad::OnRdokakususort() 
{
	m_nSortKind = 2;
	InitListImageCode();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogGaijiLoad::OnSelchangeCmbfont() 
{
	int		idx;
	int		fonttype;
	TCHAR	fontname[RECMAX];
	TCHAR	accessname[RECMAX];

	UpdateData(TRUE);
	idx = m_cmbFont.GetCurSel();
	mojienvflistopen(_T("rb"));
	mojienvgetflist(idx, &fonttype, fontname, accessname);
	mojienvflistclose();
	m_csFont = fontname;
	_tcscpy_s(m_szFolderName, accessname);
	InitListImageCode();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogGaijiLoad::OnSelchangeLstimg() 
{
	int		idx;
	TCHAR	szStr[RECMAX];
	TCHAR	szDmy[RECMAX];

	UpdateData(TRUE);
	idx = m_lstImg.GetCurSel();
	m_lstImg.GetText(idx, szStr);
	_stscanf_s(szStr, _T("%s %d"), szDmy, RECMAX, &idx);
	m_nCrtTop = idx;
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate(TRUE);
}

void CDialogGaijiLoad::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	rect;
	int		sx, sy, ex, ey;
	int		idx;

	CDialog::OnLButtonDown(nFlags, point);
	m_stImg.GetWindowRect(&rect);
	LegalRect(&rect);
	sx = rect.left+2;
	sy = rect.top+2;
	idx = m_nCrtTop;
	while(1){
		if(m_nLstMax <= idx){
			break;
		}
		ex = sx + IMGSIZE;
		ey = sy + IMGSIZE;
		if(sx <= point.x && point.x <= ex
		&& sy <= point.y && point.y <= ey){
			m_lstCode.GetText(idx, m_csCode);
			UpdateData(FALSE);
			break;
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

void CDialogGaijiLoad::OnBtnload() 
{
	UpdateData(TRUE);
	if(m_csCode.IsEmpty() == TRUE){
		::AfxMessageBox(_T("シフトJISコードを入力してください"), MB_OK);
		return;
	}
	CDialog::OnOK();
}

void CDialogGaijiLoad::LegalRect(CRect *rect)
{
	CRect	rectTop;

	m_stTop.GetWindowRect(&rectTop);
	rect->left -= rectTop.left;
	rect->right -= rectTop.left;
	rect->top -= rectTop.top;
	rect->bottom -= rectTop.top;
}

void CDialogGaijiLoad::DrawPartImg(CDC *pcDC, CRect rect)
{
	TCHAR	szFontPath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	int		idx, tidx;
	TCHAR	szCode[QRECMAX];
	int		sx, sy, ex, ey;
	HFONT	hFont, hOldFont;
	TCHAR	szBuf[16];
	ULONG	lImageSize;

    hFont = ::CreateFont((int)(IMGSPACE*0.7), 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, _T("ＭＳ 明朝"));
    hOldFont = (HFONT)::SelectObject(pcDC->m_hDC, hFont);
	mojienvgetfontpath(szFontPath);
	idx = m_nCrtTop;
	tidx = 0;
	sx = rect.left+2;
	sy = rect.top+2;
	while(1){
		if(m_nLstMax <= idx){
			break;
		}
		ex = sx + IMGSIZE;
		ey = sy + IMGSIZE;
		BoxsDraw(pcDC, sx, sy, ex, ey);
		m_lstCode.GetText(idx, szCode); 
		_stprintf_s(szFileName, _T("%s\\%s\\%s.bmp"), szFontPath, m_szFolderName, szCode);
		if(_taccess_s(szFileName, 0) == 0){
			ImageDraw(pcDC, sx, sy, szFileName);
		}
		::TextOut(pcDC->m_hDC, sx+2, ey+2, szCode, _tcslen(szCode));
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
void CDialogGaijiLoad::ImageDraw(CDC *pcDC, int sx, int sy, LPTSTR filename)
{
	TCHAR	szBuf[16];
	ULONG	nSize;
	CFile	cpFile;
	LPBYTE	lpDib;
	LPBYTE 	lpInfo;
	LPTSTR	lpBits;
	int		ret;
	DBL	xtime, ytime;
	int dstxsize, dstysize;

	CFile file(filename, CFile::modeRead);
	file.Read(szBuf, 2);
	file.Read(&nSize, 4);
	file.Close();
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
void CDialogGaijiLoad::InitCmbFont()
{
	int		idx;
	int		fonttype;
	TCHAR	crtfontname[RECMAX];
	TCHAR	fontname[RECMAX];
	TCHAR	accessname[RECMAX];
	int		nSel;

	idx = 0;
	nSel = 0;
	mojienvflistopen(_T("rb"));
	mojienvgetfontname(crtfontname);
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == _T('\0') && accessname[0] == _T('\0')){
			break;
		}
		if(_tcscmp(crtfontname, fontname) == 0){
			nSel = idx;
		}
		m_cmbFont.AddString(fontname);
		idx++;
	}
	mojienvgetflist(nSel, &fonttype, fontname, accessname);
	_tcscpy_s(m_szFolderName, accessname);
	m_csFont = fontname;
	mojienvflistclose();
	UpdateData(FALSE);
}

void CDialogGaijiLoad::InitListImageCode()
{
	m_lstImg.ResetContent();
	m_lstCode.ResetContent();
	switch(m_nSortKind){
	case 0:
		InitListCodeSort();
		break;
	case 1:
		InitListBusyuSort();
		break;
	case 2:
		InitListKakusuSort();
		break;
	}
}

void CDialogGaijiLoad::InitListCodeSort()
{
	int		idx;
	TCHAR	szCode[QRECMAX];
#ifdef UNICODE
	int		nCode;

	nCode = m_nFirstCode;
	idx = 0;
	while (1) {
		if (m_nLastCode < nCode) {
			break;
		}
		if ((idx % 30) == 0) {
			_stprintf_s(szCode, _T("0x%04X                    %d"), nCode, idx);
			m_lstImg.AddString(szCode);
		}
		_stprintf_s(szCode, _T("%04X"), nCode);
		m_lstCode.AddString(szCode);
		nCode++;
		idx++;
	}
	m_nLstMax = idx;
#else
	int		nKu, nTen, nKuten;
	int		nJis, nSJis;
	nKu = m_nFirstCode / 100;
	nTen = m_nFirstCode % 100;
	idx = 0;
	while(1){
		nKuten = nKu * 100 + nTen;
		if(m_nLastCode < nKuten){
			break;
		}
		nJis = basekutentojis(nKuten);
		nSJis = basejtosj(nJis);
		if ((idx % 30) == 0) {
			_stprintf_s(szCode, _T("0x%04X                    %d"), nSJis, idx);
			m_lstImg.AddString(szCode);
		}
		_stprintf_s(szCode, _T("%04X"), nSJis);
		m_lstCode.AddString(szCode);
		idx++;
		nTen++;
		if(94 < nTen){
			nKu++;
			nTen = 1;
		}
	}
	m_nLstMax = idx;
#endif
}

void CDialogGaijiLoad::InitListBusyuSort()
{
	TCHAR	szFontPath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;
	TCHAR	szBuf[RECMAX];
	int		idx;
	TCHAR	szCode[RECMAX];
	int		len, i;

	mojienvgetfontpath(szFontPath);
	_stprintf_s(szFileName, _T("%s\\%s\\ListBusyu.txt"), szFontPath, m_szFolderName);
	_tfopen_s(&fp, szFileName, _T("r,ccs=UNICODE"));
	if(fp == NULL){
		m_nLstMax = 0;
		return;
	}
	idx = 0;
	while(1){
		if(_fgetts(szBuf, RECMAX4, fp) == NULL){
			break;
		}
		len = _tcslen(szBuf);
		szBuf[len-1] = _T('\0');
		if(szBuf[0] == '('){
			_stprintf_s(szCode, _T("%s                    %d"), szBuf, idx);
			m_lstImg.AddString(szCode);
		}else{
#ifdef UNICODE
			int		code;
			len = _tcslen(szBuf);
			for (i = 0; i < len; i++) {
				code = (szBuf[i]) & 0x00ff;
				_stprintf_s(szCode, _T("%04X"), code);
				m_lstCode.AddString(szCode);
				idx++;
			}
			m_lstCode.AddString(_T(""));
			idx++;
#else
			int		hc, lc;
			len = _tcslen(szBuf);
			for (i = 0; i < len; i += 2) {
				hc = (szBuf[i]) & 0x00ff;
				lc = (szBuf[i + 1]) & 0x00ff;
				_stprintf_s(szCode, _T("%02X%02X"), hc, lc);
				m_lstCode.AddString(szCode);
				idx++;
			}
			m_lstCode.AddString(_T(""));
			idx++;
#endif
		}
	}
	fclose(fp);
	m_nLstMax = idx;
}

void CDialogGaijiLoad::InitListKakusuSort()
{
	TCHAR	szFontPath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;
	TCHAR	szBuf[RECMAX4];
	int		idx;
	TCHAR	szCode[RECMAX];
	int		len, i;
	int		len1, len2;

	mojienvgetfontpath(szFontPath);
	_stprintf_s(szFileName, _T("%s\\%s\\ListKakusu.txt"), szFontPath, m_szFolderName);
	_tfopen_s(&fp, szFileName, _T("r,ccs=UNICODE"));
	if(fp == NULL){
		m_nLstMax = 0;
		return;
	}
	idx = 0;
	while(1){
		if(_fgetts(szBuf, RECMAX4, fp) == NULL){
			break;
		}
		len = _tcslen(szBuf);
		szBuf[len-1] = _T('\0');
		len1 = _tcslen(_T("01"));
		len2 = _tcslen(_T("画"));
		if(_tcsnccmp(&(szBuf[len1]), _T("画"), len2) == 0){
			_stprintf_s(szCode, _T("%s                    %d"), szBuf, idx);
			m_lstImg.AddString(szCode);
		}else{
#ifdef UNICODE
			int code;
			len = _tcslen(szBuf);
			for (i = 0; i < len; i++) {
				code = szBuf[i];
				_stprintf_s(szCode, _T("%04X"), code);
				m_lstCode.AddString(szCode);
				idx++;
			}
			m_lstCode.AddString(_T(""));
			idx++;
#else
			int		hc, lc;
			len = _tcslen(szBuf);
			for(i = 0; i < len ; i += 2){
				hc = (szBuf[i]) & 0x00ff;
				lc = (szBuf[i+1]) & 0x00ff;
				_stprintf_s(szCode, _T("%02X%02X"), hc, lc); 
				m_lstCode.AddString(szCode);
				idx++;
			}
			m_lstCode.AddString(_T(""));
			idx++;
#endif
		}
	}
	fclose(fp);
	m_nLstMax = idx;
}

void CDialogGaijiLoad::BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey)
{
	pcDC->MoveTo(sx-1, sy-1);
	pcDC->LineTo(ex+1, sy-1);
	pcDC->LineTo(ex+1, ey+1);
	pcDC->LineTo(sx-1, ey+1);
	pcDC->LineTo(sx-1, sy-1);
}

