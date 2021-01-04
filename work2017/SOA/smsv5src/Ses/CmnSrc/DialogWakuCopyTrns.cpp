// DWkCptr.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "io.h"
#include "DialogWakuCopyTrns.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Wakuenv.h"
//}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	WAKUSIZE		150
#define WAKUSPACE		20
#define WAKUCOUNT		6

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuCopyTrns ダイアログ

int CALLBACK CDialogWakuCopyTrns::BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if((BFFM_INITIALIZED == uMsg) && lpData){
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;
}

CDialogWakuCopyTrns::CDialogWakuCopyTrns(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWakuCopyTrns::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	m_nFirstFlag = TRUE;
	m_nCrtTop = 0;
	m_hDib = 0;
	//{{AFX_DATA_INIT(CDialogWakuCopyTrns)
	m_csDstFolder = _T("");
	m_csDstXSize = _T("");
	m_csDstYSize = _T("");
	//}}AFX_DATA_INIT
}


void CDialogWakuCopyTrns::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuCopyTrns)
	DDX_Control(pDX, IDC_NEXTPAGE, m_btnNextPage);
	DDX_Control(pDX, IDC_LSTWAKUNAME, m_lstWakuName);
	DDX_Text(pDX, IDC_EDDSTFOLDER, m_csDstFolder);
	DDX_Text(pDX, IDC_EDDSTXSIZE, m_csDstXSize);
	DDX_Text(pDX, IDC_EDDSTYSIZE, m_csDstYSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuCopyTrns, CDialog)
	//{{AFX_MSG_MAP(CDialogWakuCopyTrns)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BEFORPAGE, OnBeforpage)
	ON_BN_CLICKED(IDC_NEXTPAGE, OnNextpage)
	ON_WM_CREATE()
	ON_LBN_SELCHANGE(IDC_LSTWAKUNAME, OnSelchangeLstwakuname)
	ON_BN_CLICKED(IDC_BTNDSTFOLDER, OnBtndstfolder)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNDSTXSIZE, OnDeltaposSpndstxsize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNDSTYSIZE, OnDeltaposSpndstysize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuCopyTrns メッセージ ハンドラ

void CDialogWakuCopyTrns::OnPaint() 
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

void CDialogWakuCopyTrns::WakuAllDraw(CDC *pcDC)
{
	DBL		paperxsize, paperysize;
	DBL		papersx, papersy;
	DBL		paperwidth, paperhight;
	DBL		wakuxsize, wakuysize;
	TCHAR	paperpath[FNAMEMAX];
	int		idx;
	TCHAR	wakulistfname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	TCHAR	wakuname[FNAMEMAX];
	TCHAR	wakufile[FNAMEMAX];
	TCHAR	wenvfile[FNAMEMAX];
	DBL		sx, sy;
	long	nsx, nsy;
	long	nex, ney;
	HFONT	hFont, hOldFont;
	int		len;
	HPEN	hSlctPen, hMainPen;
 

	wakuenvinit(m_pcView->m_szInfoEnv);
	wakuenvgetpaperelement();
	wakuenvgetpapersize(&paperxsize, &paperysize);
	wakuenvgetpaperstart(&papersx, &papersy);
	wakuenvgetpaperarea(&paperwidth, &paperhight);
	wakuenvgetfullpaperpath(paperpath);

	TrnsTimeSet(paperxsize, paperysize);

	_stprintf_s(wakulistfname, _T("%s\\wakuname.lst"), paperpath);
	_tfopen_s(&fp, wakulistfname, _T("r"));
	if(fp == NULL){
		_tfopen_s(&fp, wakulistfname, _T("w+"));
		if(fp == NULL){
			return;
		}
		return;
	}

	hSlctPen = ::CreatePen(0, 1, RGB(255, 0, 0));
    hMainPen = (HPEN)::SelectObject(pcDC->m_hDC, hSlctPen);

	sx = m_nImgTopX + WAKUSPACE / 2;
	sy = m_nImgTopY + WAKUSPACE;
	m_nOneLineCount = 0;
	idx = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(idx < m_nCrtTop){
			idx++;
			continue;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, wakuname);
		_stprintf_s(wakufile, _T("%s\\%s"), paperpath, wakuname);
		idx++;

		wakuxsize = paperxsize;
		wakuysize = paperysize;
		_tcscpy_s(wenvfile, wakufile);
		len = _tcslen(wenvfile);
		wenvfile[len-3] = _T('e'); wenvfile[len-2] = _T('n'); wenvfile[len-1] = _T('v');

		GetWakuSize(wenvfile, &wakuxsize, &wakuysize);

		nsx = (int)sx;
		nsy = (int)sy;
		nex = (int)sx + WAKUSIZE;
		ney = (int)sy + WAKUSIZE;
		if(m_lstWakuName.GetSel(idx-1) == TRUE){
		    ::SelectObject(pcDC->m_hDC, hSlctPen);
		}else{
		    ::SelectObject(pcDC->m_hDC, hMainPen);
		}
		BoxsDraw(pcDC, nsx, nsy, nex, ney);

        hFont = ::CreateFont((int)(WAKUSPACE*0.7), 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, _T("ＭＳ 明朝"));
        hOldFont = (HFONT)::SelectObject(pcDC->m_hDC, hFont);
		len = _tcslen(wakuname);
		wakuname[len-4] = _T('\0');
		len = _tcslen(wakuname);
		::TextOut(pcDC->m_hDC, nsx, (int)(nsy-WAKUSPACE*0.7), wakuname, len);
        hFont = (HFONT)::SelectObject(pcDC->m_hDC, hOldFont);
        ::DeleteObject(hFont);

		TrnsTimeSet(wakuxsize, wakuysize);

		nsx = (int)sx;
		nsy = (int)sy;
		nex = (int)(wakuxsize * m_dTrnsTime + sx);
		ney = (int)(wakuysize * m_dTrnsTime + sy);

		WakuDraw(pcDC, nsx, nsy, wakufile);
		BoxsDraw(pcDC, nsx, nsy, nex, ney);

		sx = sx + WAKUSIZE + WAKUSPACE;
		if((sx + WAKUSIZE) > m_nDlgWidth){
			if(m_nOneLineCount == 0){
				m_nOneLineCount = idx - m_nCrtTop;
			}
			sx = m_nImgTopX + WAKUSPACE / 2;
			sy = sy + WAKUSIZE + WAKUSPACE;
			if((sy + WAKUSIZE) > m_nDlgHight){
				break;
			}
		}
	}
	if(m_nOneLineCount == 0){
		m_nOneLineCount = idx - m_nCrtTop;
	}

	fclose(fp);

	::SelectObject(pcDC->m_hDC, hMainPen);
    ::DeleteObject(hSlctPen);
}
void CDialogWakuCopyTrns::WakuDraw(CDC *pcDC, int sx, int sy, LPTSTR wakufname)
{
	TCHAR	initfname[FNAMEMAX];
	TCHAR	szBuf[16];
	long	lImageSize;

	if(m_pcView->m_bDispImage == TRUE){
		_tcscpy_s(initfname, wakufname);
		basechangesafix(initfname, _T("iti"));
		if((_taccess_s(initfname, 0 )) == 0){
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
void CDialogWakuCopyTrns::WakuImageDraw(CDC *pcDC, int sx, int sy, LPTSTR initfname, long nSize)
{
	CFile	cpFile;
	LPBYTE	lpDib;
	LPBYTE 	lpInfo;
	LPTSTR	lpBits;
	int		ret;
	DBL	xtime, ytime;

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
	if(cpFile.Read(lpDib, nSize) != nSize){
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
	lpBits = (LPTSTR)(lpDib + ((BITMAPFILEHEADER *)lpDib)->bfOffBits);
	ret = StretchDIBits(pcDC->m_hDC,
				sx, sy, (int)(xsize * xtime), (int)(ysize * ytime), 
				0, 0, xsize, ysize, 
				lpBits, (LPBITMAPINFO)lpInfo, DIB_RGB_COLORS, SRCCOPY);
	GlobalUnlock(m_hDib);
	return;
}
void CDialogWakuCopyTrns::WakuBoxsDraw(CDC *pcDC, DBL sx, DBL sy, LPTSTR wakufname)
{
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	*sp;
	TCHAR	matchstr[RECMAX];
	TCHAR	instr[RECMAX];
	TCHAR	fontname[RECMAX];
	TCHAR	numstr[HRECMAX];
	DBL		dsx, dsy;
	DBL		dwd, dhi;
	long	nsx, nsy;
	long	nex, ney;

	_tfopen_s(&fp, wakufname, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, matchstr);
		sp = basecsvstrdivcpy(sp, instr);
		sp = basecsvstrdivcpy(sp, fontname);
		sp = basecsvstrdivcpy(sp, numstr);
		_stscanf_s(numstr, _T("%lf"), &dsx);
		sp = basecsvstrdivcpy(sp, numstr);
		_stscanf_s(numstr, _T("%lf"), &dsy);
		sp = basecsvstrdivcpy(sp, numstr);
		_stscanf_s(numstr, _T("%lf"), &dwd);
		sp = basecsvstrdivcpy(sp, numstr);
		_stscanf_s(numstr, _T("%lf"), &dhi);

		nsx = (int)(dsx * m_dTrnsTime + sx);
		nsy = (int)(dsy * m_dTrnsTime + sy);
		nex = (int)((dsx + dwd) * m_dTrnsTime + sx);
		ney = (int)((dsy + dhi) * m_dTrnsTime + sy);

		BoxsDraw(pcDC, nsx, nsy, nex, ney);

	}

	fclose(fp);
}
void CDialogWakuCopyTrns::TrnsTimeSet(DBL paperwidth, DBL paperhight)
{
	if(paperwidth > paperhight){
		m_dTrnsTime = WAKUSIZE / paperwidth;
	}
	else{
		m_dTrnsTime = WAKUSIZE / paperhight;
	}
}

void CDialogWakuCopyTrns::BoxsDraw(CDC *pcDC, long sx, long sy, long ex, long ey)
{
	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(ex, sy);
	pcDC->LineTo(ex, ey);
	pcDC->LineTo(sx, ey);
	pcDC->LineTo(sx, sy);
}

void CDialogWakuCopyTrns::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int		sx, sy;
	int		idx;

	int	cnt = (m_nDlgHight + WAKUSPACE * 2 + WAKUSIZE / 2) / (WAKUSPACE + WAKUSIZE);
	int nAddY = ((m_nDlgHight + WAKUSPACE + WAKUSIZE / 2) - ((WAKUSPACE + WAKUSIZE) * cnt)) / (cnt - 1);

	sx = m_nImgTopX + WAKUSPACE / 2;
	sy = m_nImgTopY + WAKUSPACE;
	idx = 0;
	while(1){
		// 2001.10.16　横長の細い用紙が選択できないため修正
		if(sx <= point.x && point.x <= (sx+WAKUSIZE)
		&& (sy - WAKUSPACE) <= point.y && point.y <= (sy+WAKUSIZE)){
			m_nSlctWaku = idx + m_nCrtTop;
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
	
	CDialog::OnLButtonDown(nFlags, point);
	
	if(m_nSlctWaku < m_nWakuMax){
		if((nFlags & MK_CONTROL) != MK_CONTROL){
			AllResetSelect();
		}
		if(m_lstWakuName.GetSel(m_nSlctWaku) == TRUE){
			m_lstWakuName.SetSel(m_nSlctWaku, FALSE);
		}else{
			m_lstWakuName.SetSel(m_nSlctWaku, TRUE);
		}
	}
	Invalidate(TRUE);
}

void CDialogWakuCopyTrns::GetWakuCountMax()
{
	TCHAR	paperpath[FNAMEMAX];
	int		idx;
	TCHAR	wakulistfname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	szWakuName[RECMAX];

	wakuenvinit(m_pcView->m_szInfoEnv);
	wakuenvgetpaperelement();
	wakuenvgetfullpaperpath(paperpath);

	_stprintf_s(wakulistfname, _T("%s\\wakuname.lst"), paperpath);
	_tfopen_s(&fp, wakulistfname, _T("r"));
	if(fp == NULL){
		_tfopen_s(&fp, wakulistfname, _T("w+"));
		if(fp == NULL){
			return;
		}
		return;
	}

	idx = 0;
	m_lstWakuName.ResetContent();
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_tcscpy_s(szWakuName, &(buf[1]));
		szWakuName[_tcslen(szWakuName)-1] = _T('\0'); 
		m_lstWakuName.AddString(szWakuName);
		idx++;
	}
	m_nWakuMax = idx;
	fclose(fp);
}

BOOL CDialogWakuCopyTrns::OnInitDialog() 
{
	CRect	Rect, RectLst, RectBtn;
	int		x, y, cx, cy;

	CDialog::OnInitDialog();

	CRect	rect;
	CMainFrame *pcWnd = (CMainFrame*)::AfxGetMainWnd();
	pcWnd->GetWindowRect(&rect);
	rect.top = 0;
	rect.left = 0;
	MoveWindow(rect, TRUE);

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

void CDialogWakuCopyTrns::OnBeforpage() 
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

void CDialogWakuCopyTrns::OnNextpage() 
{
	m_nCrtTop = m_nCrtTop + m_nOneLineCount;
	if(m_nCrtTop > (m_nWakuMax - m_nOneLineCount)){
		m_nCrtTop = m_nWakuMax - m_nOneLineCount;
	}
	Invalidate(FALSE);
}

void CDialogWakuCopyTrns::GetWakuSize(TCHAR *wenvfile, DBL *wakuxsize, DBL *wakuysize)
{
	FILE	*fp;
	TCHAR	buf[RECMAX];

	_tfopen_s(&fp, wenvfile, _T("r"));
	if(fp != NULL){
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		baselibCrDelFgets(buf, RECMAX, fp);
		if(baselibCrDelFgets(buf, RECMAX, fp) != NULL){
			_stscanf_s(buf, _T("%lf %lf\n"), wakuxsize, wakuysize);
		}
		fclose(fp);
	}
}

int CDialogWakuCopyTrns::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// ダイアログを最大化
	lpCreateStruct->style |= WS_MAXIMIZE;
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CDialogWakuCopyTrns::OnSelchangeLstwakuname() 
{
	//m_nCrtTop = m_lstWakuName.GetCurSel() - 1;
	Invalidate(TRUE);
}

void CDialogWakuCopyTrns::OnBtndstfolder() 
{
	TCHAR	szPath[FNAMEMAX];

	UpdateData(TRUE);
	_tcscpy_s(szPath, m_csDstFolder.GetBuffer(FNAMEMAX));
	GetSelectDir(_T("コピー先フォルダー位置設定"), szPath);
	if(szPath[0] != _T('\0')){
		m_csDstFolder = szPath;
	}
	UpdateData(FALSE);
}

void CDialogWakuCopyTrns::OnDeltaposSpndstxsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = _ttof(m_csDstXSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csDstXSize.Format(_T("%1.1lf"), num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogWakuCopyTrns::OnDeltaposSpndstysize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = _ttof(m_csDstYSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csDstYSize.Format(_T("%1.1lf"), num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogWakuCopyTrns::AllResetSelect()
{
	int		idx;

	for(idx = 0; idx < m_nWakuMax; idx++){
		m_lstWakuName.SetSel(idx, FALSE);
	}
}

void CDialogWakuCopyTrns::GetSelectDir(LPTSTR lpTitle, LPTSTR lpPath)
{
	BROWSEINFO	bInfo;
	LPITEMIDLIST	lpFoldPointer;

	memset(&bInfo, 0, sizeof(BROWSEINFO));
	bInfo.hwndOwner = this->m_hWnd;
	bInfo.pidlRoot = 0;
	bInfo.lpszTitle = lpTitle;
	bInfo.ulFlags = 1;
	bInfo.lpfn   = BrowseCallbackProc; // コールバーク関数
	bInfo.lParam = (LPARAM)(lpPath);       // 初期フォルダ
	lpFoldPointer = SHBrowseForFolder(&bInfo);
	SHGetPathFromIDList(lpFoldPointer, lpPath);
}

void CDialogWakuCopyTrns::OnOK() 
{
	TCHAR	szSrcPath[FNAMEMAX];
	CString	csSrcFolder;
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	szWakuSrcList[FNAMEMAX];
	FILE	*fp;
	int		idx;
	TCHAR	szWakuName[RECMAX];
	TCHAR	buf[RECMAX];

	UpdateData(TRUE);
	wakuenvgetfullpaperpath(szSrcPath);
	csSrcFolder = szSrcPath;
	if(m_csDstFolder == csSrcFolder){
		::AfxMessageBox(_T("同じ場所にコピーできません"), MB_OK);
		return;
	}
	senvgettmppath(tmppath);
	_stprintf_s(szWakuSrcList, _T("%s\\wakuname.lst"), tmppath);
	_tfopen_s(&fp, szWakuSrcList, _T("w"));
	for(idx = 0; idx < m_nWakuMax; idx++){
		if(m_lstWakuName.GetSel(idx) == TRUE){
			m_lstWakuName.GetText(idx, szWakuName);
			_stprintf_s(buf, _T("\"%s\"\n"), szWakuName);
			baselibFputs(buf, fp);
		}
		
	}
	fclose(fp);
	CDialog::OnOK();
}
