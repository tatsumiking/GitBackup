// DGjSave.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "io.h"
#include "DGjSave.h"
#include "..\imgkit\imagekit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IMGSIZE		128
#define IMGSPACE	16

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSave ダイアログ
extern "C" {
	int basesjtoj(int far sjis);
	int basejtosj(int far jis);
	int basekutentojis(int kuten);
	void basesjtokuten(int far sjis, int far *ku, int far *ten);
	void basejtokuten(int far jis, int far *ku, int far *ten);
	void senvgetpartpath(LPSTR path);
	void senvgetexepath(LPSTR path);
	void mojienvgetfontpath(LPSTR fontpath);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
}

CDialogGaijiSave::CDialogGaijiSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGaijiSave::IDD, pParent)
{
	m_pcWnd = (CMainFrame*)::AfxGetMainWnd();
	m_pcView = (CTonView *)pParent;
	m_nFirstFlag = TRUE;
	m_nFirstKuten = 9501;
	m_nLastKuten = 11494;
	m_nCrtTop = m_nFirstKuten;
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
	if(m_nCrtTop < m_nFirstKuten){
		m_nCrtTop = m_nFirstKuten;
	}
	Invalidate(TRUE);
}

void CDialogGaijiSave::OnNextpage() 
{
	m_nCrtTop += (m_nPageMax/2);
	if((m_nLastKuten - m_nPageMax) < m_nCrtTop){
		m_nCrtTop = m_nLastKuten - m_nPageMax;
	}
	if(m_nCrtTop < m_nFirstKuten){
		m_nCrtTop = m_nFirstKuten;
	}
	Invalidate(TRUE);
}

void CDialogGaijiSave::OnSelchangeCmbfont() 
{
	int		idx;
	int		fonttype;
	char	fontname[RECMAX];
	char	accessname[RECMAX];

	UpdateData(TRUE);
	idx = m_cmbFont.GetCurSel();
	mojienvgetflist(idx, &fonttype, fontname, accessname);
	m_csFont = _T(fontname);
	strcpy(m_szFolderName, accessname);
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogGaijiSave::OnSelchangeLstimg() 
{
	int		idx;
	char	szCode[QRECMAX];
	int		nSJis, nKu, nTen;

	UpdateData(TRUE);
	idx = m_lstImg.GetCurSel();
	m_lstImg.GetText(idx, szCode);
	sscanf(szCode, "0x%X", &nSJis);
	basesjtokuten(nSJis, &nKu, &nTen);
	m_nCrtTop = nKu*100+nTen;
	if(m_nCrtTop < m_nFirstKuten){
		m_nCrtTop = m_nFirstKuten;
	}
	Invalidate(TRUE);
}

void CDialogGaijiSave::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	rect;
	int		sx, sy, ex, ey;
	int		nKu, nTen, nKuten;
	int		nJis, nSJis;

	CDialog::OnLButtonDown(nFlags, point);
	m_stImg.GetWindowRect(&rect);
	LegalRect(&rect);
	sx = rect.left+2;
	sy = rect.top+2;
	nKuten = m_nCrtTop;
	while(1){
		ex = sx + IMGSIZE;
		ey = sy + IMGSIZE;
		if(sx <= point.x && point.x <= ex
		&& sy <= point.y && point.y <= ey){
			if(nKuten <= m_nLastKuten){
				nJis = basekutentojis(nKuten);
				nSJis = basejtosj(nJis);
				m_csCode.Format("%04X", nSJis);
				UpdateData(FALSE);
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
		nKu = nKuten / 100;
		nTen = nKuten % 100;
		nTen++;
		if(94 < nTen){
			nKu++;
			nTen = 1;
		}
		nKuten = nKu * 100 + nTen;
	}
}

void CDialogGaijiSave::OnBtnsave() 
{
	UpdateData(TRUE);
	if(m_csBusyu.IsEmpty() == TRUE){
		::AfxMessageBox("部首を選択してください", MB_OK);
		return;
	}
	if(m_csKakusu.IsEmpty() == TRUE){
		::AfxMessageBox("画数を選択してください", MB_OK);
		return;
	}
	if(m_csCode.IsEmpty() == TRUE){
		::AfxMessageBox("シフトJISコードを入力してください", MB_OK);
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
	char	szFontPath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	int		idx;
	int		nKu, nTen, nKuten;
	int		nJis, nSJis;
	char	szCode[QRECMAX];
	int		sx, sy, ex, ey;
	HANDLE	handle;
	HFONT	hFont, hOldFont;

    hFont = ::CreateFont((int)(IMGSPACE*0.7), 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "ＭＳ 明朝");
    hOldFont = (HFONT)::SelectObject(pcDC->m_hDC, hFont);
	mojienvgetfontpath(szFontPath);
	nKuten = m_nCrtTop;
	idx = 0;
	sx = rect.left+2;
	sy = rect.top+2;
	while(1){
		ex = sx + IMGSIZE;
		ey = sy + IMGSIZE;
		BoxsDraw(pcDC, sx, sy, ex, ey);
		nJis = basekutentojis(nKuten);
		nSJis = basejtosj(nJis);
		sprintf(szCode, "%04X", nSJis);
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
		nKu = nKuten / 100;
		nTen = nKuten % 100;
		nTen++;
		if(94 < nTen){
			nKu++;
			nTen = 1;
		}
		nKuten = nKu * 100 + nTen;
		if(m_nLastKuten < nKuten){
			break;
		}
		idx++;
	}
	m_nPageMax = idx;
	hFont = (HFONT)::SelectObject(pcDC->m_hDC, hOldFont);
	::DeleteObject(hFont);
}


void CDialogGaijiSave::InitCmbBusyu()
{
	char	szExePath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	FILE	*fp;
	char	szBuf[RECMAX];

	senvgetexepath(szExePath);
	sprintf(szFileName, "%s\\ListBusyu.txt", szExePath);
	fp = fopen(szFileName, "r");
	if(fp == NULL){
		return;
	}
	while(1){
		if(fgets(szBuf, RECMAX, fp) == NULL){
			break;
		}
		szBuf[strlen(szBuf)-1] = '\0';
		m_cmbBusyu.AddString(szBuf);
	}
	fclose(fp);
}

void CDialogGaijiSave::InitCmbKakusu()
{
	char	szExePath[FNAMEMAX];
	char	szFileName[FNAMEMAX];
	FILE	*fp;
	char	szBuf[RECMAX];

	senvgetexepath(szExePath);
	sprintf(szFileName, "%s\\Listkakusu.txt", szExePath);
	fp = fopen(szFileName, "r");
	if(fp == NULL){
		return;
	}
	while(1){
		if(fgets(szBuf, RECMAX, fp) == NULL){
			break;
		}
		szBuf[strlen(szBuf)-1] = '\0';
		m_cmbKakusu.AddString(szBuf);
	}
	fclose(fp);
}

void CDialogGaijiSave::InitCmbFont()
{
	int		idx, nSel;
	int		fonttype;
	char	fontname[RECMAX];
	char	accessname[RECMAX];
	char	crtfontname[RECMAX];

	mojienvgetfontname(crtfontname);
	idx = 0;
	nSel = 0;
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == '\0' && accessname[0] == '\0'){
			break;
		}
		if(strcmp(crtfontname, fontname) == 0){
			nSel = idx;
		}
		m_cmbFont.AddString(fontname);
		idx++;
	}
	mojienvgetflist(nSel, &fonttype, fontname, accessname);
	m_csFont = _T(fontname);
	strcpy(m_szFolderName, accessname);
	UpdateData(FALSE);
}

void CDialogGaijiSave::InitListImage()
{
	int		nKu, nTen, nKuten;
	int		nJis, nSJis;
	char	szCode[QRECMAX];

	m_lstImg.ResetContent();
	nKu = m_nFirstKuten / 100;
	nTen = m_nFirstKuten % 100;
	while(1){
		nKuten = nKu * 100 + nTen;
		if(m_nLastKuten < nKuten){
			break;
		}
		nJis = basekutentojis(nKuten);
		nSJis = basejtosj(nJis);
		sprintf(szCode, "0x%04X", nSJis);
		m_lstImg.AddString(szCode);
		nTen += 50;
		if(94 < nTen){
			nKu++;
			nTen = 1;
		}
	}
}

void CDialogGaijiSave::BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey)
{
	pcDC->MoveTo(sx-1, sy-1);
	pcDC->LineTo(ex+1, sy-1);
	pcDC->LineTo(ex+1, ey+1);
	pcDC->LineTo(sx-1, ey+1);
	pcDC->LineTo(sx-1, sy-1);
}

