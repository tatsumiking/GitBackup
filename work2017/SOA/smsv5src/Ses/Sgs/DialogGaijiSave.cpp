// DGjSave.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "io.h"
#include "DialogGaijiSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	IMGSIZE		128
#define IMGSPACE	16

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSave ダイアログ
//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Mojienv.h"
	#include "../../ObjLib/Senvlib.h"
//}

CDialogGaijiSave::CDialogGaijiSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGaijiSave::IDD, pParent)
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
	m_nCrtTop = m_nFirstCode;
	m_hDib = 0;
	//{{AFX_DATA_INIT(CDialogGaijiSave)
	m_csCode = _T("");
	m_csBusyu = _T("");
	m_csFont = _T("");
	m_csKakusu = _T("");
	//}}AFX_DATA_INIT
}

void CDialogGaijiSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGaijiSave)
	DDX_Control(pDX, IDC_CMBBUSYU, m_cmbBusyu);
	DDX_Control(pDX, IDC_CMBKAKUSU, m_cmbKakusu);
	DDX_Control(pDX, IDC_CMBFONT, m_cmbFont);
	DDX_Control(pDX, IDC_LSTIMG, m_lstImg);
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_STIMG, m_stImg);
	DDX_Text(pDX, IDC_EDCODE, m_csCode);
	DDX_CBString(pDX, IDC_CMBBUSYU, m_csBusyu);
	DDX_CBString(pDX, IDC_CMBFONT, m_csFont);
	DDX_CBString(pDX, IDC_CMBKAKUSU, m_csKakusu);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LSTCODE, m_lstCode);
}

BEGIN_MESSAGE_MAP(CDialogGaijiSave, CDialog)
	//{{AFX_MSG_MAP(CDialogGaijiSave)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BEFORPAGE, OnBeforpage)
	ON_BN_CLICKED(IDC_NEXTPAGE, OnNextpage)
	ON_WM_LBUTTONDOWN()
	ON_LBN_SELCHANGE(IDC_LSTIMG, OnSelchangeLstimg)
	ON_CBN_SELCHANGE(IDC_CMBFONT, OnSelchangeCmbfont)
	ON_BN_CLICKED(IDC_BTNSAVE, OnBtnsave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSave メッセージ ハンドラ

void CDialogGaijiSave::OnPaint() 
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

BOOL CDialogGaijiSave::OnInitDialog() 
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

	InitCmbBusyu();
	InitCmbKakusu();
	InitCmbFont();
	InitListImage();
	return TRUE;
}

void CDialogGaijiSave::OnBeforpage() 
{
	m_nCrtTop -= (m_nPageMax/2);
	if(m_nCrtTop < m_nFirstCode){
		m_nCrtTop = m_nFirstCode;
	}
	Invalidate(TRUE);
}

void CDialogGaijiSave::OnNextpage() 
{
	m_nCrtTop += (m_nPageMax/2);
	if((m_nLastCode - m_nPageMax) < m_nCrtTop){
		m_nCrtTop = m_nLastCode - m_nPageMax;
	}
	if(m_nCrtTop < m_nFirstCode){
		m_nCrtTop = m_nFirstCode;
	}
	Invalidate(TRUE);
}

void CDialogGaijiSave::OnSelchangeCmbfont() 
{
	int		idx;
	int		fonttype;
	TCHAR	fontname[RECMAX];
	TCHAR	accessname[RECMAX];

	UpdateData(TRUE);
	idx = m_cmbFont.GetCurSel();
	mojienvflistopen(_T("r+b"));
	mojienvgetflist(idx, &fonttype, fontname, accessname);
	mojienvflistclose();
	m_csFont = fontname;
	_tcscpy_s(m_szFolderName, accessname);
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogGaijiSave::OnSelchangeLstimg() 
{
	int		idx;
	TCHAR	szCode[QRECMAX];

	UpdateData(TRUE);
	idx = m_lstImg.GetCurSel();
#ifdef UNICODE
	m_lstImg.GetText(idx, szCode);
	_stscanf_s(szCode, _T("0x%X"), &m_nCrtTop);
#else
	int		nSJis, nKu, nTen;
	m_lstImg.GetText(idx, szCode);
	_stscanf_s(szCode, _T("0x%X"), &nSJis);
	basesjtokuten(nSJis, &nKu, &nTen);
	m_nCrtTop = nKu*100+nTen;
	if(m_nCrtTop < m_nFirstCode){
		m_nCrtTop = m_nFirstCode;
	}
#endif
	Invalidate(TRUE);
}

void CDialogGaijiSave::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	rect;
	int		sx, sy, ex, ey;
#ifdef UNICODE
	int		nCode;
#else
	int		nKu, nTen, nKuten;
	int		nJis, nSJis;
#endif

	CDialog::OnLButtonDown(nFlags, point);
	m_stImg.GetWindowRect(&rect);
	LegalRect(&rect);
	sx = rect.left+2;
	sy = rect.top+2;
#ifdef UNICODE
	nCode = m_nCrtTop;
#else
	nKuten = m_nCrtTop;
#endif
	while(1){
		ex = sx + IMGSIZE;
		ey = sy + IMGSIZE;
		if(sx <= point.x && point.x <= ex
		&& sy <= point.y && point.y <= ey){
#ifdef UNICODE
			if (nCode <= m_nLastCode) {
				m_csCode.Format(_T("%04X"), nCode);
				UpdateData(FALSE);
				break;
			}
#else
			if (nKuten <= m_nLastCode) {
				nJis = basekutentojis(nKuten);
				nSJis = basejtosj(nJis);
				m_csCode.Format(_T("%04X"), nSJis);
				UpdateData(FALSE);
				break;
			}
#endif
		}
		sx += (IMGSIZE + IMGSPACE);
		if(rect.right < (sx + IMGSIZE)){
			sx = rect.left+2;
			sy += (IMGSIZE + IMGSPACE);
			if(rect.bottom < (sy + IMGSIZE)){
				break;
			}
		}
#ifdef UNICODE
		nCode++;
#else
		nKu = nKuten / 100;
		nTen = nKuten % 100;
		nTen++;
		if (94 < nTen) {
			nKu++;
			nTen = 1;
		}
		nKuten = nKu * 100 + nTen;
#endif
	}
}

void CDialogGaijiSave::OnBtnsave() 
{
	UpdateData(TRUE);
	/*
	if(m_csBusyu.IsEmpty() == TRUE){
		::AfxMessageBox(_T("部首を選択してください"), MB_OK);
		return;
	}
	if(m_csKakusu.IsEmpty() == TRUE){
		::AfxMessageBox(_T("画数を選択してください"), MB_OK);
		return;
	}
	*/
	if(m_csCode.IsEmpty() == TRUE){
		::AfxMessageBox(_T("コードを入力してください"), MB_OK);
		return;
	}
	CDialog::OnOK();
}

void CDialogGaijiSave::LegalRect(CRect *rect)
{
	CRect	rectTop;

	m_stTop.GetWindowRect(&rectTop);
	rect->left -= rectTop.left;
	rect->right -= rectTop.left;
	rect->top -= rectTop.top;
	rect->bottom -= rectTop.top;
}

void CDialogGaijiSave::DrawPartImg(CDC *pcDC, CRect rect)
{
	TCHAR	szFontPath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	int		idx;
#ifdef UNICODE
	int		nCode;
#else
	int		nKu, nTen, nKuten;
	int		nJis, nSJis;
#endif
	TCHAR	szCode[QRECMAX];
	int		sx, sy, ex, ey;
	HANDLE	handle;
	HFONT	hFont, hOldFont;
	TCHAR	szBuf[16];
	ULONG	lImageSize;

	
	hFont = ::CreateFont((int)(IMGSPACE*0.7), 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, _T("ＭＳ 明朝"));
    hOldFont = (HFONT)::SelectObject(pcDC->m_hDC, hFont);
	mojienvgetfontpath(szFontPath);
#ifdef UNICODE
	nCode = m_nCrtTop;
#else
	nKuten = m_nCrtTop;
#endif
	idx = 0;
	sx = rect.left+2;
	sy = rect.top+2;
	while(1){
		ex = sx + IMGSIZE;
		ey = sy + IMGSIZE;
		BoxsDraw(pcDC, sx, sy, ex, ey);
#ifdef UNICODE
		_stprintf_s(szCode, _T("%04X"), nCode);
#else
		nJis = basekutentojis(nKuten);
		nSJis = basejtosj(nJis);
		_stprintf_s(szCode, _T("%04X"), nSJis);
#endif
		_stprintf_s(szFileName, _T("%s\\%s\\%s.bmp"), szFontPath, m_szFolderName, szCode);
		if (_taccess_s(szFileName, 0) == 0) {
			CFile file(szFileName, CFile::modeRead);
			file.Read(szBuf, 2);
			file.Read(&lImageSize, 4);
			file.Close();
			ImageDraw(pcDC, sx, sy, szFileName, lImageSize);
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
#ifdef UNICODE
		nCode++;
#else
		nKu = nKuten / 100;
		nTen = nKuten % 100;
		nTen++;
		if (94 < nTen) {
			nKu++;
			nTen = 1;
		}
		nKuten = nKu * 100 + nTen;
		if (m_nLastCode < nKuten) {
			break;
		}
#endif
		idx++;
	}
	m_nPageMax = idx;
	hFont = (HFONT)::SelectObject(pcDC->m_hDC, hOldFont);
	::DeleteObject(hFont);
}
void CDialogGaijiSave::ImageDraw(CDC *pcDC, int sx, int sy, LPTSTR filename, ULONG nSize)
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
void CDialogGaijiSave::InitCmbBusyu()
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;
	TCHAR	szBuf[RECMAX4];
	int		len;

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\ListBusyu.txt"), szExePath);
	_tfopen_s(&fp, szFileName, _T("r,ccs=UNICODE"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(_fgetts(szBuf, RECMAX4, fp) == NULL){
			break;
		}
		len = _tcslen(szBuf);
		szBuf[len-1] = _T('\0');
		m_cmbBusyu.AddString(szBuf);
	}
	fclose(fp);
}

void CDialogGaijiSave::InitCmbKakusu()
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];
	FILE	*fp;
	TCHAR	szBuf[RECMAX4];
	int		len;

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\Listkakusu.txt"), szExePath);
	_tfopen_s(&fp, szFileName, _T("r,ccs=UNICODE"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(_fgetts(szBuf, RECMAX4, fp) == NULL){
			break;
		}
		len = _tcslen(szBuf);
		szBuf[len-1] = _T('\0');
		m_cmbKakusu.AddString(szBuf);
	}
	fclose(fp);
}

void CDialogGaijiSave::InitCmbFont()
{
	int		idx, nSel;
	int		fonttype;
	TCHAR	fontname[RECMAX];
	TCHAR	accessname[RECMAX];
	TCHAR	crtfontname[RECMAX];

	mojienvflistopen(_T("rb"));
	mojienvgetfontname(crtfontname);
	idx = 0;
	nSel = 0;
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
	m_csFont = fontname;
	_tcscpy_s(m_szFolderName, accessname);
	mojienvflistclose();
	UpdateData(FALSE);
}

void CDialogGaijiSave::InitListImage()
{
	int		idx;
	TCHAR	szCode[QRECMAX];

#ifdef UNICODE
	int nCode;

	m_lstImg.ResetContent();
	m_lstCode.ResetContent();
	idx = 0;
	nCode = m_nFirstCode;
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
#else
	int		nKu, nTen, nKuten;
	int		nJis, nSJis;

	m_lstImg.ResetContent();
	m_lstCode.ResetContent();
	idx = 0;
	nKu = m_nFirstCode / 100;
	nTen = m_nFirstCode % 100;
	while (1) {
		nKuten = nKu * 100 + nTen;
		if (m_nLastCode < nKuten) {
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
		nTen++;
		idx++;
		if (94 < nTen) {
			nKu++;
			nTen = 1;
		}
	}
#endif
}

void CDialogGaijiSave::BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey)
{
	pcDC->MoveTo(sx-1, sy-1);
	pcDC->LineTo(ex+1, sy-1);
	pcDC->LineTo(ex+1, ey+1);
	pcDC->LineTo(sx-1, ey+1);
	pcDC->LineTo(sx-1, sy-1);
}

