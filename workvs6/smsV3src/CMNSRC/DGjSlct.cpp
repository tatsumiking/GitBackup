// DGjSlct.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "io.h"
#include "DGjSlct.h"
#include "..\imgkit\imagekit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define	IMGSIZE		64
#define	IMGSIZE		128
#define IMGSPACE	16

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSlct ダイアログ
extern "C" {
	int basesjtoj(int far sjis);
	int basejtosj(int far jis);
	int basekutentojis(int kuten);
	void basesjtokuten(int far sjis, int far *ku, int far *ten);
	void basejtokuten(int far jis, int far *ku, int far *ten);
	void senvgetpartpath(LPSTR path);
	void mojienvgetfontpath(LPSTR fontpath);
}

CDialogGaijiSlct::CDialogGaijiSlct(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGaijiSlct::IDD, pParent)
{
	m_pcWnd = (CMainFrame*)::AfxGetMainWnd();
	m_pcView = (CTonView *)pParent;
	m_nFirstFlag = TRUE;
	m_nFirstKuten = 9501;
	m_nLastKuten = 11494;
	m_nCrtTop = 0;
	//{{AFX_DATA_INIT(CDialogGaijiSlct)
	m_csCode = _T("");
	m_nSortKind = 0;
	//}}AFX_DATA_INIT
}


void CDialogGaijiSlct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGaijiSlct)
	DDX_Control(pDX, IDC_LSTCODE, m_lstCode);
	DDX_Control(pDX, IDC_LSTIMG, m_lstImg);
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_STIMG, m_stImg);
	DDX_Text(pDX, IDC_EDCODE, m_csCode);
	DDX_Radio(pDX, IDC_RDOCODESORT, m_nSortKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogGaijiSlct, CDialog)
	//{{AFX_MSG_MAP(CDialogGaijiSlct)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BEFORPAGE, OnBeforpage)
	ON_BN_CLICKED(IDC_NEXTPAGE, OnNextpage)
	ON_WM_LBUTTONDOWN()
	ON_LBN_SELCHANGE(IDC_LSTIMG, OnSelchangeLstimg)
	ON_BN_CLICKED(IDC_BTNLOAD, OnBtnload)
	ON_BN_CLICKED(IDC_RDOCODESORT, OnRdocodesort)
	ON_BN_CLICKED(IDC_RDOBUSYUSORT, OnRdobusyusort)
	ON_BN_CLICKED(IDC_RDOKAKUSUSORT, OnRdokakususort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSlct メッセージ ハンドラ

void CDialogGaijiSlct::OnPaint() 
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

BOOL CDialogGaijiSlct::OnInitDialog() 
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

	InitListImageCode();
	return TRUE;
}

void CDialogGaijiSlct::OnBeforpage() 
{
	m_nCrtTop -= (m_nPageMax/2);
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate(TRUE);
}

void CDialogGaijiSlct::OnNextpage() 
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

void CDialogGaijiSlct::OnRdocodesort() 
{
	m_nSortKind = 0;
	InitListImageCode();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogGaijiSlct::OnRdobusyusort() 
{
	m_nSortKind = 1;
	InitListImageCode();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogGaijiSlct::OnRdokakususort() 
{
	m_nSortKind = 2;
	InitListImageCode();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogGaijiSlct::OnSelchangeLstimg() 
{
	int		idx;
	char	szStr[RECMAX];
	char	szDmy[RECMAX];

	UpdateData(TRUE);
	idx = m_lstImg.GetCurSel();
	m_lstImg.GetText(idx, szStr);
	sscanf(szStr, "%s %d", szDmy, &idx);
	m_nCrtTop = idx;
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate(TRUE);
}

void CDialogGaijiSlct::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CDialogGaijiSlct::OnBtnload() 
{
	int		nSjis;
	UpdateData(TRUE);
	if(m_csCode.IsEmpty() == TRUE){
		::AfxMessageBox("シフトJISコードを入力してください", MB_OK);
		return;
	}
	sscanf(m_csCode, "%x", &nSjis);
	m_szGaijiText[0] = (nSjis >> 8) & 0x00ff;
	m_szGaijiText[1] = nSjis & 0x00ff;
	m_szGaijiText[2] = '\0'; 
	CDialog::OnOK();
}

void CDialogGaijiSlct::LegalRect(CRect *rect)
{
	CRect	rectTop;

	m_stTop.GetWindowRect(&rectTop);
	rect->left -= rectTop.left;
	rect->right -= rectTop.left;
	rect->top -= rectTop.top;
	rect->bottom -= rectTop.top;
}

void CDialogGaijiSlct::DrawPartImg(CDC *pcDC, CRect rect)
{
	char	szFontPath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	int		idx, tidx;
	char	szCode[QRECMAX];
	int		sx, sy, ex, ey;
	HANDLE	handle;
	HFONT	hFont, hOldFont;

    hFont = ::CreateFont((int)(IMGSPACE*0.7), 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "ＭＳ 明朝");
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
		sprintf(szFileName, "%s\\%s\\%s.bmp", szFontPath, m_szFolderName, szCode);
		if(_access(szFileName, 0) == 0){
			handle = ImgKitFileLoad(szFileName, "", "");
			ImgKitDisplayXY(this->m_hWnd, handle, sx, sy);
			ImgKitMemFree(handle);
		}
		::TextOut(pcDC->m_hDC, sx+2, ey+2, szCode, strlen(szCode));
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

void CDialogGaijiSlct::InitListImageCode()
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

void CDialogGaijiSlct::InitListCodeSort()
{
	int		idx;
	int		nKu, nTen, nKuten;
	int		nJis, nSJis;
	char	szCode[QRECMAX];

	nKu = m_nFirstKuten / 100;
	nTen = m_nFirstKuten % 100;
	idx = 0;
	while(1){
		nKuten = nKu * 100 + nTen;
		if(m_nLastKuten < nKuten){
			break;
		}
		nJis = basekutentojis(nKuten);
		nSJis = basejtosj(nJis);
		if(nTen == 1
		|| nTen == 25
		|| nTen == 49
		|| nTen == 73){
		/*
		if(nTen == 1
		|| nTen == 20
		|| nTen == 60
		|| nTen == 80){
		*/
			sprintf(szCode, "%04X                    %d", nSJis, idx);
			m_lstImg.AddString(szCode);
		}
		sprintf(szCode, "%04X", nSJis);
		m_lstCode.AddString(szCode);
		idx++;
		nTen++;
		if(94 < nTen){
			nKu++;
			nTen = 1;
		}
	}
	m_nLstMax = idx;
}

void CDialogGaijiSlct::InitListBusyuSort()
{
	char	szFontPath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	FILE	*fp;
	char	szBuf[RECMAX4];
	int		idx;
	char	szCode[QRECMAX];
	int		len, i, hc, lc;

	mojienvgetfontpath(szFontPath);
	sprintf(szFileName, "%s\\%s\\ListBusyu.txt", szFontPath, m_szFolderName);
	fp = fopen(szFileName, "r");
	if(fp == NULL){
		m_nLstMax = 0;
		return;
	}
	idx = 0;
	while(1){
		if(fgets(szBuf, RECMAX4, fp) == NULL){
			break;
		}
		szBuf[strlen(szBuf)-1] = '\0';
		if(szBuf[0] == '('){
			sprintf(szCode, "%s                    %d", szBuf, idx);
			m_lstImg.AddString(szCode);
		}else{
			len = strlen(szBuf);
			for(i = 0; i < len ; i += 2){
				hc = (szBuf[i]) & 0x00ff;
				lc = (szBuf[i+1]) & 0x00ff;
				sprintf(szCode, "%02X%02X", hc, lc); 
				m_lstCode.AddString(szCode);
				idx++;
			}
			m_lstCode.AddString("");
			idx++;
		}
	}
	fclose(fp);
	m_nLstMax = idx;
}

void CDialogGaijiSlct::InitListKakusuSort()
{
	char	szFontPath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	FILE	*fp;
	char	szBuf[RECMAX4];
	int		idx;
	char	szCode[RECMAX];
	int		len, i, hc, lc;

	mojienvgetfontpath(szFontPath);
	sprintf(szFileName, "%s\\%s\\ListKakusu.txt", szFontPath, m_szFolderName);
	fp = fopen(szFileName, "r");
	if(fp == NULL){
		m_nLstMax = 0;
		return;
	}
	idx = 0;
	while(1){
		if(fgets(szBuf, RECMAX4, fp) == NULL){
			break;
		}
		szBuf[strlen(szBuf)-1] = '\0';
		if(strncmp(&(szBuf[2]), "画", 2) == 0){
			sprintf(szCode, "%s                    %d", szBuf, idx);
			m_lstImg.AddString(szCode);
		}else{
			len = strlen(szBuf);
			for(i = 0; i < len ; i += 2){
				hc = (szBuf[i]) & 0x00ff;
				lc = (szBuf[i+1]) & 0x00ff;
				sprintf(szCode, "%02X%02X", hc, lc); 
				m_lstCode.AddString(szCode);
				idx++;
			}
			m_lstCode.AddString("");
			idx++;
		}
	}
	fclose(fp);
	m_nLstMax = idx;
}

void CDialogGaijiSlct::BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey)
{
	pcDC->MoveTo(sx-1, sy-1);
	pcDC->LineTo(ex+1, sy-1);
	pcDC->LineTo(ex+1, ey+1);
	pcDC->LineTo(sx-1, ey+1);
	pcDC->LineTo(sx-1, sy-1);
}

