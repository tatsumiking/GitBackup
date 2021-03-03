// DlgPtnSv.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgPtnSv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnSave ダイアログ
extern "C" {
	void senvgetwakpath(LPSTR wakpath);
	void ptnenvlistgetname(int slctidx, LPSTR ptnfname);
	void ptnenvgetgruppath(LPSTR path);
	void ptnenvgetpaperelement();
	void ptnenvsetpaperelement();
	void ptnenvsetgrupname(LPSTR name);
	void ptnenvgetgrupname(LPSTR name);
	void ptnenvsetpapersize(DBL xsize, DBL ysize);
	void ptnenvgetpapersize(DBL *xsize, DBL *ysize);
	LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR retstr);
}

CDialogPtnSave::CDialogPtnSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPtnSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPtnSave)
	m_csPtnGrup = _T("");
	//}}AFX_DATA_INIT
	m_nFartFlag = TRUE;
	m_nCrtTop = 0;
	m_nSlctidx = 0;
}

void CDialogPtnSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPtnSave)
	DDX_Control(pDX, IDC_BTN_PREPG, m_bntPrePage);
	DDX_Control(pDX, IDC_BTN_NEXTPG, m_bntNextPage);
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_STFRAME, m_stFrame);
	DDX_Control(pDX, IDC_CMBPTNGRUP, m_cmbPtnGrup);
	DDX_Control(pDX, IDC_LSTSPTN, m_lstPtn);
	DDX_CBString(pDX, IDC_CMBPTNGRUP, m_csPtnGrup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPtnSave, CDialog)
	//{{AFX_MSG_MAP(CDialogPtnSave)
	ON_LBN_SELCHANGE(IDC_LSTSPTN, OnSelchangeLstsptn)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_CMBPTNGRUP, OnSelchangeCmbptngrup)
	ON_BN_CLICKED(IDC_BTN_NEXTPG, OnBtnNextpg)
	ON_BN_CLICKED(IDC_BTN_PREPG, OnBtnPrepg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnSave メッセージ ハンドラ

void CDialogPtnSave::OnSelchangeLstsptn() 
{
	m_nCrtTop = m_lstPtn.GetCurSel();
	Invalidate();
}

BOOL CDialogPtnSave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ComboInitSetItem();
	ListInitSetItem();
	m_nCrtTop = m_nSlctidx;
	if(m_nCrtTop > (m_nSPtnMax-2)){
		m_nCrtTop = m_nSPtnMax-2;
	}
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogPtnSave::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int		sx, sy;
	int		idx;
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	sx = SPTNSPACE;
	sy = SPTNSPACE;
	idx = 0;
	while(1){
		if(sx <= point.x && point.x <= (sx+SPTNSIZE)
		&& sy <= point.y && point.y <= (sy+SPTNSIZE)){
			m_nSlctidx = idx + m_nCrtTop;
			break;
		}

		idx++;
		sx = sx + SPTNSIZE + SPTNSPACE;
		if((sx + SPTNSIZE) > m_nDlgWidth){
			sx = SPTNSPACE;
			sy = sy + SPTNSIZE + SPTNSPACE;
			if((sy + SPTNSIZE) > m_nDlgHight){
				break;
			}
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);

	if(m_nSPtnMax < m_nSlctidx){
		m_nSlctidx = m_nSPtnMax;
	}
	CDialog::OnOK();
}

void CDialogPtnSave::OnPaint() 
{
	RECT	TopRect;
	RECT	Rect;
	int		sx, sy;
	int		tsx, tsy;
	int		idx;
	char	gruppath[FNAMEMAX];
	char	ptnname[FNAMEMAX];
	char	bmpfilename[FNAMEMAX];
	int		len;
	HFONT	hFont, hOldFont;
	int		fwd;

	if(m_nFartFlag == TRUE){
		m_nFartFlag = FALSE;
		m_stTop.GetWindowRect(&TopRect);
		m_nStartX = TopRect.right - TopRect.left;
		m_nStartY = TopRect.bottom - TopRect.top;
		m_stFrame.GetWindowRect(&Rect);
		m_nDlgWidth = Rect.right - Rect.left;
		m_nDlgHight = Rect.bottom - Rect.top;
	}

	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	ptnenvgetgruppath(gruppath);
	sx = m_nStartX;
	sy = m_nStartY;
	idx = 0;
	while(1){
		ptnenvlistgetname(idx+m_nCrtTop, ptnname);
		if(ptnname[0] == '\0'){
			break;
		}
		sprintf(bmpfilename, "%s\\%s", gruppath, ptnname);
		len = strlen(bmpfilename);
		bmpfilename[len-3] = '\0';
		strcat(bmpfilename, "bmp");
		DrawImage(&dc, bmpfilename, sx, sy, SPTNSIZE, SPTNSIZE); 

		tsx = sx;
		tsy = sy + SPTNSIZE;
		fwd = (int)(SPTNSPACE * 0.7);
        hFont = ::CreateFont(fwd, fwd/2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "ＭＳ 明朝");
        hOldFont = (HFONT)::SelectObject(dc.m_hDC, hFont);
		len = strlen(ptnname);
		::TextOut(dc.m_hDC, tsx, tsy, ptnname, len);
        hFont = (HFONT)::SelectObject(dc.m_hDC, hOldFont);
        ::DeleteObject(hFont);

		idx++;
		sx = sx + SPTNSIZE + SPTNSPACE;
		if((sx + SPTNSIZE) > m_nDlgWidth){
			sx = m_nStartX;
			sy = sy + SPTNSIZE + SPTNSPACE;
			if(sy > m_nDlgHight){
				break;
			}
		}
	}
}

void CDialogPtnSave::DrawImage(CDC *pcDC, LPSTR lpBmpFname, int ndsx, int ndsy, int ndw, int ndh)
{
	CFile	cpFile;
	HANDLE	hDib;
	LPBYTE	lpDib;
	ULONG	filesize;
	LPBYTE	lpBits;
	LPBITMAPFILEHEADER lpbmf;
	LPBITMAPINFOHEADER lpbminfo;
	int		nssx, nssy, nsw, nsh;

	if((cpFile.Open(lpBmpFname, CFile::modeRead)) == FALSE){
		return;
	}
	filesize = cpFile.GetLength();
	hDib = GlobalAlloc(GHND, filesize);
	lpDib = (LPBYTE)GlobalLock(hDib);
	if(lpDib == NULL){
		goto ErrorEnd;
	}
	cpFile.SeekToBegin();
	cpFile.ReadHuge(lpDib, filesize);

	lpbmf = (LPBITMAPFILEHEADER)lpDib;
	lpbminfo = (LPBITMAPINFOHEADER)(lpDib + sizeof(BITMAPFILEHEADER));
	lpBits = (LPBYTE)(lpDib + lpbmf->bfOffBits);
	nssx = 0;
	nssy = 0;
	nsw = lpbminfo->biWidth;
	nsh = lpbminfo->biHeight;
	StretchDIBits(pcDC->m_hDC, 
					ndsx, ndsy, ndw, ndh, 
					nssx, nssy, nsw, nsh, 
					lpBits, (LPBITMAPINFO)lpbminfo, DIB_RGB_COLORS, SRCCOPY);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
ErrorEnd:
	cpFile.Close();
	return;
}

void CDialogPtnSave::OnSelchangeCmbptngrup() 
{
	int		idx;
	char	grupname[RECMAX];

	idx = m_cmbPtnGrup.GetCurSel();

	m_cmbPtnGrup.GetLBText(idx, grupname );
	UpdateData(TRUE);
	ptnenvsetgrupname(grupname);
	ptnenvgetpaperelement();
	m_csPtnGrup = _T(grupname);
	ListInitSetItem();
	UpdateData(FALSE);
	Invalidate();
}

void CDialogPtnSave::ListInitSetItem()
{
	int		idx;
	char	ptnname[32];

	m_lstPtn.ResetContent( );
	idx = 0;
	while(1){
		ptnenvlistgetname(idx, ptnname);
		if(ptnname[0] == '\0'){
			break;
		}
		m_lstPtn.AddString(ptnname);
		idx++;
	}
	m_nSPtnMax = idx;
}

void CDialogPtnSave::ComboInitSetItem()
{
	char	wakpath[FNAMEMAX];
	char	paperlistfname[FNAMEMAX];
	char	buf[RECMAX];
	char	*sp;
	FILE	*fp;
	char	grupname[HRECMAX];

	senvgetwakpath(wakpath);
	sprintf(paperlistfname, "%s\\paperlst.lst", wakpath);
	if((fp = fopen(paperlistfname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, grupname);
		m_cmbPtnGrup.AddString(grupname);
	}
	fclose(fp);

	ptnenvgetgrupname(grupname);
	ptnenvgetpaperelement();
	m_csPtnGrup = _T(grupname);
	UpdateData(FALSE);
}



void CDialogPtnSave::OnBtnNextpg() 
{
	m_nCrtTop = m_nCrtTop + 4;
	if(m_nCrtTop > (m_nSPtnMax-2)){
		m_nCrtTop = m_nSPtnMax-2;
	}
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate();
}

void CDialogPtnSave::OnBtnPrepg() 
{
	m_nCrtTop = m_nCrtTop - 4;
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate();
}
