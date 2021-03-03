// DSetJpgEle.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "..\imgkit\imagekit.h"

#include "smsout.h"
#include "DSetJpgEle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSetJpgElement ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	int	senvsetdrawmode(int mode);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	void diblibcreatebmpfile(LPSTR bmpfname);
}

#define	EXTIME	3

CDialogSetJpgElement::CDialogSetJpgElement(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetJpgElement::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSetJpgElement)
	m_csBaseName = _T("");
	m_nJpgSizeX = 0;
	m_nJpgSizeY = 0;
	m_nJpgSX = 0;
	m_nJpgSY = 0;
	m_nImageSizeX = 0;
	m_nImageSizeY = 0;
	m_nImageSPX = 0;
	m_nImageSPY = 0;
	m_nChkNoWakuOut = FALSE;
	m_nJpgQuality = 0;
	m_nRoundRag = 0;
	//}}AFX_DATA_INIT
}

CDialogSetJpgElement::~CDialogSetJpgElement()
{
}

void CDialogSetJpgElement::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSetJpgElement)
	DDX_Text(pDX, IDC_EDBASENAME, m_csBaseName);
	DDX_Text(pDX, IDC_EDJPGSIZEX, m_nJpgSizeX);
	DDX_Text(pDX, IDC_EDJPGSIZEY, m_nJpgSizeY);
	DDX_Text(pDX, IDC_EDJPGSX, m_nJpgSX);
	DDX_Text(pDX, IDC_EDJPGSY, m_nJpgSY);
	DDX_Text(pDX, IDC_EDIMAGESIZEX, m_nImageSizeX);
	DDX_Text(pDX, IDC_EDIMAGESIZEY, m_nImageSizeY);
	DDX_Text(pDX, IDC_EDIMAGESPX, m_nImageSPX);
	DDX_Text(pDX, IDC_EDIMAGESPY, m_nImageSPY);
	DDX_Check(pDX, IDC_CHKNOWAKUOUT, m_nChkNoWakuOut);
	DDX_Text(pDX, IDC_EDJPGQUALITY, m_nJpgQuality);
	DDX_Text(pDX, IDC_EDJPGROUNDRAG, m_nRoundRag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSetJpgElement, CDialog)
	//{{AFX_MSG_MAP(CDialogSetJpgElement)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSetJpgElement メッセージ ハンドラ

BOOL CDialogSetJpgElement::OnInitDialog() 
{
	char	exepath[FNAMEMAX];
	char	filename[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];

	CDialog::OnInitDialog();
	m_csBaseName = _T("c:\\outimage");
	m_nJpgQuality = 50;
	m_nRoundRag = 0;
	m_nChkNoWakuOut = 1;
	m_nJpgSizeX = 768;
	m_nJpgSizeY = 1024;
	m_nJpgSX = 0;
	m_nJpgSY = 0;
	m_nImageSizeX = 768;
	m_nImageSizeY = -1;
	m_nImageSPX = 0;
	m_nImageSPY = -1;
	
	senvgetexepath(exepath);
	sprintf(filename, "%s\\jpgout.env", exepath);
	fp = fopen(filename, "r");
	if(fp != NULL){
		fgets(buf, RECMAX, fp);
		buf[strlen(buf)-1] = '\0';
		m_csBaseName = _T(buf);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nJpgQuality);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nRoundRag);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nChkNoWakuOut);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nJpgSizeX);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nJpgSizeY);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nJpgSX);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nJpgSY);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nImageSizeX);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nImageSizeY);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nImageSPX);
		fgets(buf, RECMAX, fp);
		sscanf(buf, "%d", &m_nImageSPY);
		fclose(fp);
	}
	UpdateData(FALSE);
	return TRUE;
}

void CDialogSetJpgElement::OnOK() 
{
	CDC		*pcDC;

	if(m_nImageSizeX == -1 && m_nImageSizeY == -1){
		::AfxMessageBox("イメージサイズはどちらか片方は設定してください", MB_OK);
		return;
	}
	UpdateData(TRUE);
	SaveJpgoutElement();

	m_pcBmpFile = new CBmpFile(m_pcView);
	m_pcBmpFile->InitClass();
	m_pcBmpFile->CreateBmpInit(m_nJpgSizeX*EXTIME, m_nJpgSizeY*EXTIME);
	m_nDotSizeX = m_pcBmpFile->m_nXDotMax;
	m_nDotSizeY = m_pcBmpFile->m_nYDotMax;
	pcDC = &(m_pcBmpFile->m_cdcMemCDC);

	JpgOutMain(pcDC);
	
	m_pcBmpFile->CreateBmpEnd();
	CDialog::OnOK();
}

void CDialogSetJpgElement::JpgOutMain(CDC *pcDC)
{
	int		nPageFirst;
	int		nPageNo;
	int		sx, sy;
	CRect	rect;

	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		EndCrctDB();
		return;
	}
	nPageFirst = TRUE;
	nPageNo = 1;
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			if(nPageFirst == TRUE){
				nPageFirst = FALSE;
				sx = m_nJpgSX * EXTIME;
				sy = m_nJpgSY * EXTIME;
				pcDC->BitBlt(0, 0, m_nDotSizeX, m_nDotSizeY, NULL, 0, 0, WHITENESS);
			}
			m_pcDoc->DataAllCls();
			m_pcDoc->WakuLoad(m_pcRdsOutCrct->m_column4.GetBuffer(FNAMEMAX));
			SetFitingElement();
			m_pcView->AllWakuSetString(m_pcRdsOutCrct->m_column5.GetBuffer(FNAMEMAX));
			rect.left = sx;
			rect.top = sy;
			rect.right = rect.left + m_nImageFitSizeX;
			rect.bottom = rect.top + m_nImageFitSizeY;
			if(m_nChkNoWakuOut == 0){
				senvsetdrawmode(PREVIEW);
			}
			else{
				senvsetdrawmode(PRINTVIEW);
			}
			m_pcView->DrawWakuInRect(pcDC, rect);
			sx = sx + m_nImageFitSizeX + m_nImageSPX * EXTIME;
			if((sx + m_nImageFitSizeX) > m_nJpgSizeX * EXTIME){
				sx = m_nJpgSX * EXTIME;
				sy = sy + m_nImageFitSizeY + m_nImageSPY * EXTIME;
				if((sy + m_nImageFitSizeY) > m_nJpgSizeY * EXTIME){
					CreateJpgFile(nPageNo);
					nPageNo++;
					nPageFirst = TRUE;
				}
			}
		}
		m_pcRdsOutCrct->MoveNext();
	}
	if(nPageFirst == FALSE){
		CreateJpgFile(nPageNo);
	}
	EndCrctDB();
}

void CDialogSetJpgElement::SetFitingElement()
{
	DBL		xsize, ysize;

	scrngetmmsize(&xsize, &ysize);
	if(m_nImageSizeX == -1){
		m_nImageFitSizeX = (int)(xsize * EXTIME * m_nImageSizeY / ysize);
		m_nImageFitSizeY = m_nImageSizeY * EXTIME;
	}else if(m_nImageSizeY == -1){
		m_nImageFitSizeX = m_nImageSizeX * EXTIME;
		m_nImageFitSizeY = (int)(ysize * EXTIME * m_nImageSizeX / xsize);
	}else{
		m_nImageFitSizeX = m_nImageSizeX * EXTIME;
		m_nImageFitSizeY = m_nImageSizeY * EXTIME;
	}
}

void CDialogSetJpgElement::CreateJpgFile(int nPageNo)
{
	char	bmpfilename[FNAMEMAX];
	char	jpgfilename[FNAMEMAX];
	HANDLE	hndle;
	HANDLE	nhndle;
	int		mode;
	int		ret;

	diblibcreatebmpfile(bmpfilename);
	m_pcBmpFile->BmpFileOutHalf(bmpfilename, m_nDotSizeX, m_nDotSizeY);
	sprintf(jpgfilename, "%s%d.jpg", m_csBaseName.GetBuffer(RECMAX), nPageNo);

	hndle = ImgKitFileLoad(bmpfilename, "", "");
	if(m_nRoundRag == 90){
		mode = 1;
	}else if(m_nRoundRag == 180){
		mode = 0;
	}else if(m_nRoundRag == 270){
		mode = 2;
	}else{
		mode = -1;
	}
	if(mode == -1){
		ret = ImgKitJpegFileSave(jpgfilename, hndle, "", "", m_nJpgQuality, 0);
		ret = ImgKitMemFree(hndle);
	}else{
		nhndle = ImgKitRotation(hndle, "", "", mode);
		ret = ImgKitJpegFileSave(jpgfilename, nhndle, "", "", m_nJpgQuality, 0);
		ret = ImgKitMemFree(hndle);
		ret = ImgKitMemFree(nhndle);
	}
}

void CDialogSetJpgElement::InitCrctDB()
{
	char	szExePath[FNAMEMAX];
	CString	csDBName;
	CString	csSQL;

	senvgetexepath(szExePath);
	csDBName.Format("%s\\outcrect.mdb", szExePath);
	if(m_nInSort == 0){
		csSQL.Format("select * from [ソートクエリー]");
	}
	else{
		csSQL.Format("select * from [出力テーブル]");
	}
	m_dbOutcrct.Open(csDBName, FALSE, FALSE);
	m_pcRdsOutCrct = new CRdsOutCrct(&m_dbOutcrct);
    m_pcRdsOutCrct->Open(dbOpenDynaset, csSQL, dbDenyWrite);
}

void CDialogSetJpgElement::EndCrctDB()
{
	m_pcRdsOutCrct->Close();
	delete(m_pcRdsOutCrct);
	m_dbOutcrct.Close();
}

void CDialogSetJpgElement::SaveJpgoutElement()
{
	char	exepath[FNAMEMAX];
	char	filename[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(exepath);
	sprintf(filename, "%s\\jpgout.env", exepath);
	fp = fopen(filename, "w");
	if(fp == NULL){
		return;
	}
	fprintf(fp, "%s\n", m_csBaseName.GetBuffer(RECMAX));
	fprintf(fp, "%d\n", m_nJpgQuality);
	fprintf(fp, "%d\n", m_nRoundRag);
	fprintf(fp, "%d\n", m_nChkNoWakuOut);
	fprintf(fp, "%d\n", m_nJpgSizeX);
	fprintf(fp, "%d\n", m_nJpgSizeY);
	fprintf(fp, "%d\n", m_nJpgSX);
	fprintf(fp, "%d\n", m_nJpgSY);
	fprintf(fp, "%d\n", m_nImageSizeX);
	fprintf(fp, "%d\n", m_nImageSizeY);
	fprintf(fp, "%d\n", m_nImageSPX);
	fprintf(fp, "%d\n", m_nImageSPY);
	fclose(fp);
}
