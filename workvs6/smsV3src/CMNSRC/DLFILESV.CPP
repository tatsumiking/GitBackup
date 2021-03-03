// DlgFileSv.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlFileSv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogFileSave ダイアログ

extern "C" {
	void senvgetexepath(LPSTR path);
}

CDialogFileSave::CDialogFileSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFileSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogFileSave)
	m_csPath = _T("");
	//}}AFX_DATA_INIT
	m_nFirst = ON;
	m_lDibSize = 0;
	m_hDib = 0;
	m_lImgFileSize = 0;
}


void CDialogFileSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogFileSave)
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_STIMG, m_stImg);
	DDX_Control(pDX, IDC_EDSLCTFILE, m_edSlctFile);
	DDX_Control(pDX, IDC_CMBGYOUSYA, m_cmbGyousya);
	DDX_Control(pDX, IDC_CMBKOUJI, m_cmbKouji);
	DDX_Control(pDX, IDC_EDCOMENT1, m_edComent1);
	DDX_Control(pDX, IDC_EDCOMENT2, m_edComent2);
	DDX_Control(pDX, IDC_EDCOMENT3, m_edComent3);
	DDX_Control(pDX, IDC_LSTFILE, m_lstFile);
	DDX_Control(pDX, IDC_LSTSLCTFILE, m_lstSlctFile);
	DDX_Text(pDX, IDC_STPATH, m_csPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogFileSave, CDialog)
	//{{AFX_MSG_MAP(CDialogFileSave)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LSTSLCTFILE, OnSelchangeLstslctfile)
	ON_LBN_SELCHANGE(IDC_LSTSLCTDIR, OnSelchangeLstslctdir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogFileSave メッセージ ハンドラ
BOOL CDialogFileSave::OnInitDialog() 
{
	CRect	toprect;
	CRect	imgrect;
	int		sizex, sizey;

	CDialog::OnInitDialog();
	m_edSlctFile.SetWindowText(m_szFileName);
	UpDateFileList();
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

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogFileSave::SetInitPath(LPSTR path)
{
	strcpy(m_szPathName, path);
}

void CDialogFileSave::SetSafixs(LPSTR safixs)
{
	strcpy(m_szSafixs, safixs);
	sprintf(m_szFileName, "*.%s", m_szSafixs);
}

void CDialogFileSave::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	char	szBuf[RECMAX];

	dc.SetROP2(R2_COPYPEN);
	strcpy(szBuf, "");
	if(m_nFirst == ON){
		m_nFirst = OFF;
		m_cmbGyousya.SetWindowText(szBuf);
		m_cmbKouji.SetWindowText(szBuf);
	}
	if(m_lImgFileSize != 0){
		BitMapImageDisp(&dc);
	}
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
}

void CDialogFileSave::UpDateFileList()
{
	int		flag;
	char	szSrchPath[RECMAX];
	int		len;

	//strcpy(m_szPathName, m_csDirSlct.GetBuffer(FNAMEMAX));
	sprintf(szSrchPath, "%s\\*.*", m_szPathName);
	flag = DDL_DIRECTORY | DDL_DRIVES | DDL_EXCLUSIVE;
	DlgDirList(szSrchPath, IDC_LSTSLCTDIR, IDC_STPATH, flag);
	sprintf(szSrchPath, "%s\\*.%s", m_szPathName, m_szSafixs);
	flag = DDL_READWRITE;
	DlgDirList(szSrchPath, IDC_LSTFILE, IDC_STPATH, flag);

	UpdateData(TRUE);
	strcpy(m_szPathName, m_csPath.GetBuffer(FNAMEMAX));
	len = strlen(m_szPathName);
	if(m_szPathName[len-1] == '\\'){
		m_szPathName[len-1] = '\0';
	}
	SlctFileListInit();
	
	m_lstSlctFile.Invalidate();
}

void CDialogFileSave::OnSelchangeLstslctfile() 
{
	int		idx;

	idx = m_lstSlctFile.GetCurSel();
	m_lstSlctFile.GetText(idx, m_szFileName);
	m_edSlctFile.SetWindowText(m_szFileName);
	UpdateData(FALSE);
	FileHeadLoad();
	Invalidate();
}

void CDialogFileSave::OnSelchangeLstslctdir() 
{
	int		len;
	char	szPath[FNAMEMAX];
	char	szTmpPath[FNAMEMAX];

	DlgDirSelect(szPath, IDC_LSTSLCTDIR);
	len = strlen(szPath);
	if(szPath[len-1] == '\\'){
		szPath[len-1] = '\0';
		sprintf(szTmpPath, "%s\\%s", m_szPathName, szPath);
	}
	else{
		strcpy(szTmpPath, szPath);
	}
	strcpy(m_szPathName, szTmpPath);
	UpDateFileList();
}

void CDialogFileSave::SlctFileListInit()
{
	int		idx, idxmax;
	char	szGyousya[QRECMAX];
	char	szKouji[QRECMAX];
	char	szFile[FNAMEMAX];

	m_cmbGyousya.SetWindowText(szGyousya);
	m_cmbKouji.SetWindowText(szKouji);

	idxmax = m_lstFile.GetCount();
	m_lstSlctFile.ResetContent();	
	for(idx = 0; idx < idxmax; idx++){
		m_lstFile.GetText(idx, szFile);
		m_lstSlctFile.AddString(szFile);
	}
}

void CDialogFileSave::OnOK() 
{
	UpdateData(TRUE);
	m_edSlctFile.GetWindowText(m_szFileName, QRECMAX);
	m_cmbGyousya.GetWindowText(m_szGyousya, QRECMAX);
	m_cmbKouji.GetWindowText(m_szKouji, QRECMAX);
	m_edComent1.GetWindowText(m_szComent1, QRECMAX);
	m_edComent2.GetWindowText(m_szComent2, QRECMAX);
	m_edComent3.GetWindowText(m_szComent3, QRECMAX);
	SaveCmbGyousya();
	SaveCmbKouji();
	CDialog::OnOK();
}

void CDialogFileSave::FileHeadLoad()
{
	char	fname[FNAMEMAX];
	CString	csMsg;
	int		nVerNo;

	sprintf(fname, "%s\\%s", m_szPathName, m_szFileName);

	CFile file(fname, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	ar >> nVerNo;
	BitMapImageLoad(ar);
	ar >> csMsg;
	strcpy(m_szGyousya, csMsg.GetBuffer(QRECMAX));
	ar >> csMsg;
	strcpy(m_szKouji, csMsg.GetBuffer(QRECMAX));
	ar >> csMsg;
	strcpy(m_szComent1, csMsg.GetBuffer(QRECMAX));
	ar >> csMsg;
	strcpy(m_szComent2, csMsg.GetBuffer(QRECMAX));
	ar >> csMsg;
	strcpy(m_szComent3, csMsg.GetBuffer(QRECMAX));

	ar.Close();
	file.Close();

	m_cmbGyousya.SetWindowText(m_szGyousya);
	m_cmbKouji.SetWindowText(m_szKouji);
	m_edComent1.SetWindowText(m_szComent1);
	m_edComent2.SetWindowText(m_szComent2);
	m_edComent3.SetWindowText(m_szComent3);
	UpdateData(FALSE);
}

void CDialogFileSave::BitMapImageLoad(CArchive &ar)
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

void CDialogFileSave::BitMapImageDisp(CDC *dc)
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

void CDialogFileSave::LoadCmbGyousya()
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
	
void CDialogFileSave::LoadCmbKouji()
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

void CDialogFileSave::SaveCmbGyousya()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	int		addflag;
	int		idx, idxmax;

	if(m_szGyousya == '\0'){
		return;
	}
	senvgetexepath(exepath);
	sprintf(fname, "%s\\fgyousya.lst", exepath);
	if((fp = fopen(fname, "w")) == NULL){
		return;
	}
	addflag = ON;
	idxmax = m_cmbGyousya.GetCount();
	for(idx = 0; idx < idxmax; idx++){
		m_cmbGyousya.GetLBText(idx, buf);
		if(strcmp(buf, m_szGyousya) == 0){
			addflag = OFF;
		}
		fprintf(fp, "%s\n", buf);
	}
	if(addflag == ON){
		fprintf(fp, "%s\n", m_szGyousya);
	}
	fclose(fp);
}
	
void CDialogFileSave::SaveCmbKouji()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	int		addflag;
	int		idx, idxmax;

	if(m_szGyousya == '\0'){
		return;
	}
	senvgetexepath(exepath);
	sprintf(fname, "%s\\fkouji.lst", exepath);
	if((fp = fopen(fname, "w")) == NULL){
		return;
	}
	addflag = ON;
	idxmax = m_cmbKouji.GetCount();
	for(idx = 0; idx < idxmax; idx++){
		m_cmbKouji.GetLBText(idx, buf);
		if(strcmp(buf, m_szKouji) == 0){
			addflag = OFF;
		}
		fprintf(fp, "%s\n", buf);
	}
	if(addflag == ON){
		fprintf(fp, "%s\n", m_szKouji);
	}
	fclose(fp);
}
