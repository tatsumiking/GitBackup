// DPrtLoad.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DPrtLoad.h"
#include "..\imgkit\imagekit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IMGSIZE		64
#define IMGSPACE	16

/////////////////////////////////////////////////////////////////////////////
// CDialogPartLoad ダイアログ
extern "C" {
	int baselastspacedel(char *lpstr);
	void senvgetpartpath(LPSTR path);
}


CDialogPartLoad::CDialogPartLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPartLoad::IDD, pParent)
{
	m_pcWnd = (CMainFrame*)::AfxGetMainWnd();
	m_pcView = (CTonView *)pParent;
	m_nFirstFlag = TRUE;
	m_nCrtTop = 0;
	m_nLstMax = 0;
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
	char	szPartPath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	int		idx, tidx;
	char	szName[RECMAX];
	int		sx, sy, ex, ey;
	HANDLE	handle;
	HFONT	hFont, hOldFont;

    hFont = ::CreateFont((int)(IMGSPACE*0.7), 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "ＭＳ 明朝");
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
			sprintf(szFileName, "%s\\%s.bmp", szPartPath, szName);
			handle = ImgKitFileLoad(szFileName, "", "");
			ImgKitDisplayXY(this->m_hWnd, handle, sx, sy);
			ImgKitMemFree(handle);
			::TextOut(pcDC->m_hDC, sx+2, ey+2, szName, strlen(szName));
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

void CDialogPartLoad::InitListImage()
{
	char	szPartPath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	FILE	*fp;
	char	szBuf[RECMAX];
	int		idx;

	senvgetpartpath(szPartPath);
	sprintf(szFileName, "%s\\partlist.txt", szPartPath);
	fp = fopen(szFileName, "r");
	if(fp == NULL){
		CreatePartList();
		fp = fopen(szFileName, "r");
	}
	if(fp == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(fgets(szBuf, RECMAX, fp) == NULL){
			break;
		}
		idx++;
		szBuf[strlen(szBuf)-1] = '\0';
		baselastspacedel(szBuf);
		m_lstImg.AddString(szBuf);
	}
	fclose(fp);
	m_nLstMax = idx;
}

void CDialogPartLoad::CreatePartList()
{
	char	szPartPath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	char	szSrchName[FNAMEMAX];
	CFileFind	finder;
	BOOL	bWorking;
	FILE	*fp;

	senvgetpartpath(szPartPath);
	sprintf(szSrchName, "%s\\*.iti", szPartPath);
	sprintf(szFileName, "%s\\partlist.txt", szPartPath);
	fp = fopen(szFileName, "w");

	bWorking = finder.FindFile(szSrchName);
	while (bWorking){
		bWorking = finder.FindNextFile();
		if(finder.IsDots() == FALSE && finder.IsDirectory() == FALSE){
			strcpy(szFileName, finder.GetFileName());
			szFileName[strlen(szFileName)-4] = '\0';
			fprintf(fp, "%s\n", szFileName);
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

