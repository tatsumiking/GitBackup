// ViewFDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "ViewFDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewFileDialog
extern "C" {
	void senvgetexepath(LPSTR path);
}

IMPLEMENT_DYNAMIC(CViewFileDialog, CFileDialog)

CViewFileDialog::CViewFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	//{{AFX_DATA_INIT(CViewFileDialog)
	//}}AFX_DATA_INIT
	m_ofn.Flags |= OFN_ENABLETEMPLATE | OFN_HIDEREADONLY;
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_VIEW_FILE);
	m_ofn.lpstrTitle = "";
	m_lImgFileSize = 0;
	m_lDibSize = 0;
	m_hDib = 0;
}

CViewFileDialog::~CViewFileDialog()
{
	if(m_hDib != NULL){
		GlobalFree(m_hDib);
	}
}

void CViewFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewFileDialog)
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_STIMG, m_stImg);
	DDX_Control(pDX, IDC_CMBGYOUSYA, m_cmbGyousya);
	DDX_Control(pDX, IDC_CMBKOUJI, m_cmbKouji);
	DDX_Text(pDX, IDC_CMBGYOUSYA, m_csGyousya);
	DDX_Text(pDX, IDC_CMBKOUJI, m_csKouji);
	DDX_Text(pDX, IDC_EDCOMENT1, m_csComent1);
	DDX_Text(pDX, IDC_EDCOMENT2, m_csComent2);
	DDX_Text(pDX, IDC_EDCOMENT3, m_csComent3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CViewFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CViewFileDialog)
	ON_WM_PAINT()
	ON_CBN_EDITCHANGE(IDC_CMBGYOUSYA, OnEditchangeCmbgyousya)
	ON_CBN_EDITCHANGE(IDC_CMBKOUJI, OnEditchangeCmbkouji)
	ON_EN_CHANGE(IDC_EDCOMENT1, OnChangeEdcoment1)
	ON_EN_CHANGE(IDC_EDCOMENT2, OnChangeEdcoment2)
	ON_EN_CHANGE(IDC_EDCOMENT3, OnChangeEdcoment3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CViewFileDialog::OnFileNameChange()
{
	CString	csPath;
	FILE	*fp;
	char	fname[FNAMEMAX];

	strcpy(fname, GetPathName().GetBuffer(FNAMEMAX));
	if((fp = fopen(fname, "rb")) != NULL){
		fclose(fp);
		FileViewLoad(fname);
	}
	else{
		m_lImgFileSize = 0;
	}
}

BOOL CViewFileDialog::OnInitDialog() 
{
	CRect	toprect;
	CRect	imgrect;
	int		sizex, sizey;

	CFileDialog::OnInitDialog();
	m_stTop.GetWindowRect(&toprect);
	m_stImg.GetWindowRect(&imgrect);
	m_nImgSX = imgrect.left - toprect.left;
	m_nImgSY = imgrect.top - toprect.top;
	sizex = imgrect.right - imgrect.left;
	sizey = imgrect.bottom - imgrect.top;
	if(sizex < sizey){
		m_nImgSize = sizex;
	}
	else{
		m_nImgSize = sizey;
	}
	LoadCmbGyousya();
	LoadCmbKouji();
	return TRUE;
}

void CViewFileDialog::FileViewLoad(LPSTR fname)
{
	int		nVerNo;

	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	ar >> nVerNo;
	if(nVerNo != 105){
		ar.Close();
		file.Close();
		if(m_hDib != 0){
			GlobalFree(m_hDib);
			m_hDib = 0;
		}
		m_lImgFileSize = 0;	
		UpdateData(FALSE);
		Invalidate();
		return;	
	}
	BitMapImageLoad(ar);
	ar >> m_csGyousya;
	ar >> m_csKouji;
	ar >> m_csComent1;
	ar >> m_csComent2;
	ar >> m_csComent3;

	ar.Close();
	file.Close();

	UpdateData(FALSE);
	Invalidate();
}

void CViewFileDialog::BitMapImageLoad(CArchive &ar)
{
	long	lFileSize;
	LPBYTE	lpDib;
	LPBYTE	lpp;

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	ar >> lFileSize;
	m_lImgFileSize = lFileSize;
	if(lFileSize == 0){
		return;
	}
	if(m_lDibSize < lFileSize){
		if(m_hDib != 0){
			GlobalFree(m_hDib);
		}
		m_lDibSize = lFileSize;
		m_hDib = GlobalAlloc(GHND, m_lDibSize);
	}
	lpDib = (LPBYTE)GlobalLock(m_hDib);
	lpp = lpDib;
	while(lFileSize--){
		ar >> *lpp++;
	}
	GlobalUnlock(m_hDib);
}

void CViewFileDialog::BitMapImageDisp(CDC *dc)
{
	LPBYTE	lpDib;
	LPBYTE	lpp;
	LPBYTE	lpbits;
	LPBITMAPFILEHEADER lpbmf;
	LPBITMAPINFOHEADER lpbminfo;
	int		xsize, ysize;

	lpDib = (LPBYTE)GlobalLock(m_hDib);
	lpp = lpDib;
	lpbmf = (LPBITMAPFILEHEADER)(lpDib);
	lpbminfo = (LPBITMAPINFOHEADER)(lpDib + sizeof(BITMAPFILEHEADER));
	xsize = lpbminfo->biWidth;
	ysize = lpbminfo->biHeight;
	lpbits = lpDib + lpbmf->bfOffBits;
	dc->SetStretchBltMode(COLORONCOLOR);
	::StretchDIBits(dc->m_hDC, 
		m_nImgSX, m_nImgSY, m_nImgSize, m_nImgSize, 
		0, 0, xsize, ysize, 
		lpbits, (LPBITMAPINFO)lpbminfo, DIB_RGB_COLORS, SRCCOPY);
	GlobalUnlock(m_hDib);
}

void CViewFileDialog::LoadCmbGyousya()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	int		len;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\fgyousya.lst", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		len = strlen(buf);
		buf[len-1] = '\0';
		m_cmbGyousya.AddString(buf);
	}
	fclose(fp);
}

void CViewFileDialog::LoadCmbKouji()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	int		len;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\fkouji.lst", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		len = strlen(buf);
		buf[len-1] = '\0';
		m_cmbKouji.AddString(buf);
	}
	fclose(fp);
}

void CViewFileDialog::OnPaint() 
{
	CRect	toprect;
	CRect	imgrect;

	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	CFileDialog::OnPaint();
	if(m_lImgFileSize != 0){
		m_stTop.GetWindowRect(&toprect);
		m_stImg.GetWindowRect(&imgrect);
		m_nImgSX = imgrect.left - toprect.left;
		m_nImgSY = imgrect.top - toprect.top;
		BitMapImageDisp(&dc);
	}
}

void CViewFileDialog::SetTitle(LPSTR szTitle)
{
	m_ofn.lpstrTitle = szTitle;
}

void CViewFileDialog::OnEditchangeCmbgyousya() 
{
	UpdateData(TRUE);
}

void CViewFileDialog::OnEditchangeCmbkouji() 
{
	UpdateData(TRUE);
}

void CViewFileDialog::OnChangeEdcoment1() 
{
	UpdateData(TRUE);
}

void CViewFileDialog::OnChangeEdcoment2() 
{
	UpdateData(TRUE);
}

void CViewFileDialog::OnChangeEdcoment3() 
{
	UpdateData(TRUE);
}
