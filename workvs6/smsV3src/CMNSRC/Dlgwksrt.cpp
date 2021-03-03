// DlgWkSrt.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgWkSrt.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	WAKUSIZE		150
#define WAKUSPACE		20
#define WAKUCOUNT		6

extern "C" {
	void wakuenvinit(LPSTR envfile);
	void wakuenvgetpaperelement();
	void wakuenvsetpapersize(DBL xsize, DBL ysize);
	void wakuenvgetpapersize(LPDBL xsize, LPDBL ysize);
	void wakuenvgetpaperstart(LPDBL sx, LPDBL sy);
	void wakuenvgetpaperarea(LPDBL paperwidth, LPDBL paperhight);
	void wakuenvgetfullpaperpath(LPSTR paperpath);
	void wakuenvsetselectwakuidx(int idx);
	void wakuenvlistsort(int slctidx1, int slctidx2);
	LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR retstr);
	void basechangesafix(LPSTR FileName, LPSTR Safix);
};
/////////////////////////////////////////////////////////////////////////////
// CDialogWakuSort ダイアログ


CDialogWakuSort::CDialogWakuSort(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWakuSort::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	m_nFirstFlag = TRUE;
	m_nCrtTop = 0;
	m_hDib = 0;
	m_nMouseSetCount = 0;
	//{{AFX_DATA_INIT(CDialogWakuSort)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogWakuSort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuSort)
	DDX_Control(pDX, IDC_NEXTPAGE, m_btnNextPage);
	DDX_Control(pDX, IDC_LSTWAKUNAME, m_lstWakuName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuSort, CDialog)
	//{{AFX_MSG_MAP(CDialogWakuSort)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BEFORPAGE, OnBeforpage)
	ON_BN_CLICKED(IDC_NEXTPAGE, OnNextpage)
	ON_WM_CREATE()
	ON_LBN_SELCHANGE(IDC_LSTWAKUNAME, OnSelchangeLstwakuname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuSort メッセージ ハンドラ

void CDialogWakuSort::OnPaint() 
{
	long	nsx, nsy, nex, ney;
	CDC		*pcDC;
	HPEN	hPen, hOldPen;
	HBRUSH	hBrush, hOldBrush;

	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	if(m_nFirstFlag == TRUE){
		m_nFirstFlag = FALSE;
		//MDIMaximize(this);
	}

	pcDC = GetDC();

    hPen = ::CreatePen(0, 1, RGB(255, 255, 255));
	hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
    hOldPen = (HPEN)::SelectObject(pcDC->m_hDC, hPen);
    hOldBrush = (HBRUSH)::SelectObject(pcDC->m_hDC, hBrush);

	nsx = 0;
	nsy = 0;
	nex = m_nDlgWidth;
	ney = m_nDlgHight;
	::Rectangle(pcDC->m_hDC, nsx, nsy, nex, ney);

    hPen = (HPEN)::SelectObject(pcDC->m_hDC, hOldPen);
    hBrush = (HBRUSH)::SelectObject(pcDC->m_hDC, hOldBrush);
    ::DeleteObject(hPen);
    ::DeleteObject(hBrush);

	WakuAllDraw(pcDC);

	ReleaseDC(pcDC);
}

void CDialogWakuSort::WakuAllDraw(CDC *pcDC)
{
	DBL		paperxsize, paperysize;
	DBL		papersx, papersy;
	DBL		paperwidth, paperhight;
	DBL		wakuxsize, wakuysize;
	char	paperpath[FNAMEMAX];
	int		idx;
	char	wakulistfname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	*sp;
	char	wakuname[FNAMEMAX];
	char	wakufile[FNAMEMAX];
	char	wenvfile[FNAMEMAX];
	DBL		sx, sy;
	long	nsx, nsy;
	long	nex, ney;
	HFONT	hFont, hOldFont;
	int		len;
	HPEN	hPen, hOldPen;

    hPen = ::CreatePen(0, 1, RGB(255, 0, 0));

	wakuenvinit(m_pcView->m_szInfoEnv);
	wakuenvgetpaperelement();
	wakuenvgetpapersize(&paperxsize, &paperysize);
	wakuenvgetpaperstart(&papersx, &papersy);
	wakuenvgetpaperarea(&paperwidth, &paperhight);
	wakuenvgetfullpaperpath(paperpath);

	TrnsTimeSet(paperxsize, paperysize);

	sprintf(wakulistfname, "%s\\wakuname.lst", paperpath);
	if((fp = fopen(wakulistfname, "r")) == NULL){
		if((fp = fopen(wakulistfname, "w+")) == NULL){
			return;
		}
		return;
	}

	int	cnt = (m_nDlgHight + WAKUSPACE * 2 + WAKUSIZE / 2) / (WAKUSPACE + WAKUSIZE);
	int nAddY = ((m_nDlgHight + WAKUSPACE + WAKUSIZE / 2) - ((WAKUSPACE + WAKUSIZE) * cnt)) / (cnt - 1);
	sx = m_nImgTopX + WAKUSPACE / 2;
	sy = m_nImgTopY + WAKUSPACE;
	m_nOneLineCount = 0;
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(idx < m_nCrtTop){
			idx++;
			continue;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, wakuname);
		sprintf(wakufile, "%s\\%s", paperpath, wakuname);
		idx++;

		wakuxsize = paperxsize;
		wakuysize = paperysize;
		strcpy(wenvfile, wakufile);
		len = strlen(wenvfile);
		wenvfile[len-3] = 'e'; wenvfile[len-2] = 'n'; wenvfile[len-1] = 'v';

		GetWakuSize(wenvfile, &wakuxsize, &wakuysize);

		nsx = (long)sx;
		nsy = (long)sy;
		nex = (long)(sx + WAKUSIZE);
		ney = (long)(sy + WAKUSIZE);

		//BoxsDraw(pcDC, nsx, nsy, nex, ney);

        hFont = ::CreateFont((int)(WAKUSPACE*0.7), 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "ＭＳ 明朝");
        hOldFont = (HFONT)::SelectObject(pcDC->m_hDC, hFont);
		len = strlen(wakuname);
		wakuname[len-4] = '\0';
		len = strlen(wakuname);
		::TextOut(pcDC->m_hDC, nsx, (int)(nsy-WAKUSPACE*0.7), wakuname, len);
        hFont = (HFONT)::SelectObject(pcDC->m_hDC, hOldFont);
        ::DeleteObject(hFont);

		TrnsTimeSet(wakuxsize, wakuysize);

		WakuDraw(pcDC, sx, sy, wakufile);
		nsx = (long)sx;
		nsy = (long)sy;
		nex = (long)(wakuxsize * m_dTrnsTime + sx);
		ney = (long)(wakuysize * m_dTrnsTime + sy);
		BoxsDraw(pcDC, nsx, nsy, nex, ney);

		if(m_nSlctWaku1 == idx-1 && m_nMouseSetCount == 1){
			hOldPen = (HPEN)::SelectObject(pcDC->m_hDC, hPen);
			BoxsDraw(pcDC, nsx, nsy, nex, ney);
			hPen = (HPEN)::SelectObject(pcDC->m_hDC, hOldPen);
		}
		else{
			BoxsDraw(pcDC, nsx, nsy, nex, ney);
		}

		sx = sx + WAKUSIZE + WAKUSPACE;
		if((sx + WAKUSIZE) > m_nDlgWidth){
			if(m_nOneLineCount == 0){
				m_nOneLineCount = idx - m_nCrtTop;
			}
			sx = m_nImgTopX + WAKUSPACE / 2;
			sy = sy + WAKUSIZE + WAKUSPACE + nAddY;
			if((sy + WAKUSIZE) > m_nDlgHight){
				break;
			}
		}
	}
	fclose(fp);
	if(m_nOneLineCount == 0){
		m_nOneLineCount = idx - m_nCrtTop;
	}

    ::DeleteObject(hPen);
}

void CDialogWakuSort::WakuDraw(CDC *pcDC, DBL sx, DBL sy, LPSTR wakufname)
{
	char	initfname[FNAMEMAX];
	char	szBuf[16];
	long	lImageSize;

	if(m_pcView->m_bDispImage == TRUE){
		strcpy(initfname, wakufname);
		basechangesafix(initfname, "iti");
		if((_access(initfname, 0 )) == 0){
			CFile file(initfname, CFile::modeRead);
			file.Read(szBuf, 2);
			file.Read(&lImageSize, 4);
			file.Close();
			if(szBuf[0] == 0x42 && szBuf[1] == 0x4d){
				WakuImageDraw(pcDC, sx, sy, initfname, lImageSize);
				WakuBoxsDraw(pcDC, sx, sy, wakufname);
				return;
			}
		}
	}
	WakuBoxsDraw(pcDC, sx, sy, wakufname);
}
void CDialogWakuSort::WakuImageDraw(CDC *pcDC, DBL sx, DBL sy, LPSTR initfname, long nSize)
{
	CFile	cpFile;
	LPBYTE	lpDib;
	LPBYTE 	lpInfo;
	LPSTR	lpBits;
	int		ret;
	DBL	xtime, ytime;
	int	addx, addy;

	if(m_hDib == NULL){
		m_nDibSize = nSize;
		m_hDib = ::GlobalAlloc(GHND, m_nDibSize);
	}else if(m_nDibSize < nSize){
		m_nDibSize = nSize;
		m_hDib = ::GlobalReAlloc(m_hDib, m_nDibSize, GHND);
	}
	lpDib = (LPBYTE)::GlobalLock(m_hDib);
	if(lpDib == NULL){
		return;
	}
	ret = cpFile.Open(initfname, CFile::modeRead);
	if(ret == FALSE){
		::GlobalUnlock(m_hDib);
		return;
	}
	if(cpFile.ReadHuge(lpDib, nSize) != nSize){
		::GlobalUnlock(m_hDib);
		return;
	}
	cpFile.Close();


	lpInfo = lpDib + sizeof(BITMAPFILEHEADER);

	int xsize = ((BITMAPINFOHEADER*)lpInfo)->biWidth;
	int ysize = ((BITMAPINFOHEADER*)lpInfo)->biHeight;

	xtime = (DBL)(WAKUSIZE) / (DBL)xsize;
	ytime = (DBL)(WAKUSIZE) / (DBL)ysize;
	if(ytime < xtime){
		xtime = ytime;
	}
	else{
		ytime = xtime;
	}
	lpBits = (LPSTR)(lpDib + ((BITMAPFILEHEADER *)lpDib)->bfOffBits);
	ret = StretchDIBits(pcDC->m_hDC,
				sx, sy, (int)(xsize * xtime), (int)(ysize * ytime), 
				0, 0, xsize, ysize, 
				lpBits, (LPBITMAPINFO)lpInfo, DIB_RGB_COLORS, SRCCOPY);
	GlobalUnlock(m_hDib);
	return;
}
void CDialogWakuSort::WakuBoxsDraw(CDC *pcDC, DBL sx, DBL sy, LPSTR wakufname)
{
	FILE	*fp;
	char	buf[RECMAX8+16];
	char	*sp;
	char	matchstr[RECMAX];
	char	instr[RECMAX];
	char	fontname[RECMAX];
	char	numstr[HRECMAX];
	DBL		dsx, dsy;
	DBL		dwd, dhi;
	long	nsx, nsy;
	long	nex, ney;

	if((fp = fopen(wakufname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX8, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, matchstr);
		sp = basecsvstrdivcpy(sp, instr);
		sp = basecsvstrdivcpy(sp, fontname);
		sp = basecsvstrdivcpy(sp, numstr);
		sscanf(numstr, "%lf", &dsx);
		sp = basecsvstrdivcpy(sp, numstr);
		sscanf(numstr, "%lf", &dsy);
		sp = basecsvstrdivcpy(sp, numstr);
		sscanf(numstr, "%lf", &dwd);
		sp = basecsvstrdivcpy(sp, numstr);
		sscanf(numstr, "%lf", &dhi);

		nsx = (int)(dsx * m_dTrnsTime + sx);
		nsy = (int)(dsy * m_dTrnsTime + sy);
		nex = (int)((dsx + dwd) * m_dTrnsTime + sx);
		ney = (int)((dsy + dhi) * m_dTrnsTime + sy);

		BoxsDraw(pcDC, nsx, nsy, nex, ney);

	}

	fclose(fp);
}
void CDialogWakuSort::TrnsTimeSet(DBL paperwidth, DBL paperhight)
{
	if(paperwidth > paperhight){
		m_dTrnsTime = WAKUSIZE / paperwidth;
	}
	else{
		m_dTrnsTime = WAKUSIZE / paperhight;
	}
}

void CDialogWakuSort::BoxsDraw(CDC *pcDC, long sx, long sy, long ex, long ey)
{
	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(ex, sy);
	pcDC->LineTo(ex, ey);
	pcDC->LineTo(sx, ey);
	pcDC->LineTo(sx, sy);
}

void CDialogWakuSort::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int		sx, sy;
	int		idx;

	int	cnt = (m_nDlgHight + WAKUSPACE * 2 + WAKUSIZE / 2) / (WAKUSPACE + WAKUSIZE);
	int nAddY = ((m_nDlgHight + WAKUSPACE + WAKUSIZE / 2) - ((WAKUSPACE + WAKUSIZE) * cnt)) / (cnt - 1);

	sx = m_nImgTopX + WAKUSPACE / 2;
	sy = m_nImgTopY + WAKUSPACE;
	idx = 0;
	while(1){
		if(sx <= point.x && point.x <= (sx+WAKUSIZE)
		&& sy <= (point.y-WAKUSPACE) && point.y <= (sy+WAKUSIZE)){
			if(m_nMouseSetCount == 0){
				m_nSlctWaku1 = idx + m_nCrtTop;
				m_nMouseSetCount = 1;
			}
			else{
				m_nSlctWaku2 = idx + m_nCrtTop;
				m_nMouseSetCount = 2;
			}
			break;
		}
		idx++;
		sx = sx + WAKUSIZE + WAKUSPACE;
		if((sx + WAKUSIZE) > m_nDlgWidth){
			sx = m_nImgTopX + WAKUSPACE / 2;
			sy = sy + WAKUSIZE + WAKUSPACE;
			if((sy + WAKUSIZE) > m_nDlgHight){
				break;
			}
		}
	}
	if(m_nMouseSetCount == 2 && m_nSlctWaku2 >= m_nWakuMax){
		m_nSlctWaku2 = m_nWakuMax;
	}

	CDialog::OnLButtonDown(nFlags, point);

	if(m_nSlctWaku1 < m_nWakuMax){
		if(m_nMouseSetCount == 2){
			wakuenvlistsort(m_nSlctWaku1, m_nSlctWaku2);
			m_nMouseSetCount = 0;
			m_nSlctWaku1 = -1;
			m_nSlctWaku2 = -1;
		}
	}
	else{
		m_nMouseSetCount = 0;
		m_nSlctWaku1 = -1;
		m_nSlctWaku2 = -1;
	}
	Invalidate(FALSE);
}

void CDialogWakuSort::GetWakuCountMax()
{
	char	paperpath[FNAMEMAX];
	int		idx;
	char	wakulistfname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	szWakuName[RECMAX];

	wakuenvinit(m_pcView->m_szInfoEnv);
	wakuenvgetpaperelement();
	wakuenvgetfullpaperpath(paperpath);

	sprintf(wakulistfname, "%s\\wakuname.lst", paperpath);
	if((fp = fopen(wakulistfname, "r")) == NULL){
		if((fp = fopen(wakulistfname, "w+")) == NULL){
			return;
		}
		return;
	}

	idx = 0;
	m_lstWakuName.ResetContent();
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		strcpy(szWakuName, &(buf[1]));
		szWakuName[strlen(szWakuName)-2] = '\0'; 
		m_lstWakuName.AddString(szWakuName);
		idx++;
	}
	m_nWakuMax = idx;
	fclose(fp);
}

BOOL CDialogWakuSort::OnInitDialog() 
{
	CRect	Rect, RectLst, RectBtn;
	int		x, y, cx, cy;

	CDialog::OnInitDialog();

	CMainFrame *pcWnd = (CMainFrame*)::AfxGetMainWnd();
	pcWnd->GetWindowRect(&Rect);
	Rect.top = 0;
	Rect.left = 0;
	MoveWindow(Rect, TRUE);

	m_lstWakuName.GetWindowRect(&RectLst);
	m_btnNextPage.GetWindowRect(&RectBtn);
	GetWindowRect(&Rect);
	m_nImgTopX = RectLst.right;
	m_nImgTopY = RectBtn.bottom;
	m_nDlgWidth = Rect.Width();  
	m_nDlgHight = Rect.Height();
	x = RectLst.left;
	y = m_nImgTopY;
	cx = RectLst.Width();
	cy = m_nDlgHight - m_nImgTopY*2;
	m_lstWakuName.SetWindowPos(0, x, y, cx, cy, SWP_NOOWNERZORDER);
	m_lstWakuName.Invalidate(TRUE);

	GetWakuCountMax();
	return TRUE;
}

void CDialogWakuSort::OnBeforpage() 
{
	// 2008.01.08 要望により追加修正
	if(m_nCrtTop == 0){
		m_nCrtTop = m_nWakuMax - m_nOneLineCount;
	}else{
		m_nCrtTop = m_nCrtTop - m_nOneLineCount;
		if(m_nCrtTop < 0){
			m_nCrtTop = 0;
		}
	}
	Invalidate(FALSE);
}

void CDialogWakuSort::OnNextpage() 
{
	m_nCrtTop = m_nCrtTop + m_nOneLineCount;
	if(m_nCrtTop > (m_nWakuMax - m_nOneLineCount)){
		m_nCrtTop = m_nWakuMax - m_nOneLineCount;
	}
	Invalidate(FALSE);
}

void CDialogWakuSort::GetWakuSize(char *wenvfile, DBL *wakuxsize, DBL *wakuysize)
{
	FILE	*fp;
	char	buf[RECMAX];

	if((fp = fopen(wenvfile, "r")) != NULL){
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		fgets(buf, RECMAX, fp);
		if(fgets(buf, RECMAX, fp) != NULL){
			sscanf(buf, "%lf %lf\n", wakuxsize, wakuysize);
		}
		fclose(fp);
	}
}

int CDialogWakuSort::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// ダイアログを最大化
	lpCreateStruct->style |= WS_MAXIMIZE;
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CDialogWakuSort::OnSelchangeLstwakuname() 
{
	m_nCrtTop = m_lstWakuName.GetCurSel() - 1;
	Invalidate(TRUE);
}
