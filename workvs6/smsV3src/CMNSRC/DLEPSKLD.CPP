// dlepskld.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dlepskld.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	DATASIZE	280.0

/////////////////////////////////////////////////////////////////////////////
// CDialogEpsKamonLoad ダイアログ

extern "C" {
	void senvgetexepath(LPSTR path);
	void senvgetkmnpath(LPSTR path);
}

CDialogEpsKamonLoad::CDialogEpsKamonLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEpsKamonLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEpsKamonLoad)
	m_dKmnSize = 0.0;
	m_csSrchChar = _T("");
	m_csSrchStrg = _T("");
	//}}AFX_DATA_INIT
	m_dDispSx = 0;
	m_dDispSy = 0;
}


void CDialogEpsKamonLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEpsKamonLoad)
	DDX_Control(pDX, IDC_KMNVIEW, m_edKmnView);
	DDX_Control(pDX, IDC_NAMELIST, m_lstName);
	DDX_Text(pDX, IDC_KMNSIZE, m_dKmnSize);
	DDX_Text(pDX, IDC_SRCHCHAR, m_csSrchChar);
	DDX_Text(pDX, IDC_SRCHSTRG, m_csSrchStrg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogEpsKamonLoad, CDialog)
	//{{AFX_MSG_MAP(CDialogEpsKamonLoad)
	ON_EN_CHANGE(IDC_SRCHCHAR, OnChangeSrchchar)
	ON_EN_CHANGE(IDC_SRCHSTRG, OnChangeSrchstrg)
	ON_LBN_SELCHANGE(IDC_NAMELIST, OnSelchangeNamelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEpsKamonLoad メッセージ ハンドラ
BOOL CDialogEpsKamonLoad::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	NameListInit();

	return TRUE;
}

void CDialogEpsKamonLoad::NameListInit()
{
	char	exepath[FNAMEMAX+2];
	char	fname[FNAMEMAX+2];
	FILE	*fp;
	char	buf[QRECMAX];
	char	epsname[QRECMAX];
	char	kname[QRECMAX];
	int		len;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\kamon.csv", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(fgets(buf, RECMAX, fp) == NULL){
				break;
			}
			len = strlen(buf);
			buf[len-1] = '\0';
			strncpy(epsname, buf, 5);
			epsname[5] = '\0';
			strcpy(kname, &(buf[6]));
			wsprintf(buf, "%-40s %s", kname, epsname);
			m_lstName.AddString(buf);
		}
		fclose(fp);
	}
}

void CDialogEpsKamonLoad::OnChangeSrchchar() 
{
	char	exepath[FNAMEMAX+2];
	char	fname[FNAMEMAX+2];
	FILE	*fp;
	char	buf[QRECMAX];
	char	srchbuf[QRECMAX];
	char	name[QRECMAX];
	int		idx, idxmax, idxlast;

	UpdateData(TRUE);
	strcpy(srchbuf, m_csSrchChar.GetBuffer(QRECMAX));
	senvgetexepath(exepath);
	sprintf(fname, "%s\\kmnepsch.lst", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(fgets(buf, QRECMAX, fp) == NULL){
				break;
			}
			if(strncmp(buf, srchbuf, 2) == 0){
				sscanf(buf, "%s %d", name, &idx);
				idxlast = idx+LISTMAX;
				idxmax = m_lstName.GetCount();
				if(idxmax <= idxlast){
					idxlast = idxmax-1;
				}
				m_lstName.SetCurSel(idxlast);
				m_lstName.SetCurSel(idx);
				KamnSelectDisp(idx);
				break;
			}
		}
		fclose(fp);
	}
}

void CDialogEpsKamonLoad::OnChangeSrchstrg() 
{
	char	srchbuf[QRECMAX];
	int		idx, idxmax, idxlast;

	UpdateData(TRUE);
	strcpy(srchbuf, m_csSrchStrg.GetBuffer(QRECMAX));
	idx = m_lstName.FindString(-1, srchbuf);
	idxlast = idx+LISTMAX;
	idxmax = m_lstName.GetCount();
	if(idxmax <= idxlast){
		idxlast = idxmax-1;
	}
	m_lstName.SetCurSel(idxlast);
	m_lstName.SetCurSel(idx);
	KamnSelectDisp(idx);
}

void CDialogEpsKamonLoad::KamnSelectDisp(int idx)
{
	char	buf[RECMAX];
	char	kname[QRECMAX];
	char	epsname[QRECMAX];
	CRect	rect;

	m_lstName.GetText(idx, buf);
	sscanf(buf, "%s %s", kname, epsname);

	CDC *pcCDC = m_edKmnView.GetDC();
	m_edKmnView.GetClientRect(&rect);
	m_dDispSize = rect.right - rect.left;
	m_dTime = m_dDispSize / DATASIZE;
	CBrush *pcBrush = new CBrush(RGB(255, 255, 255));
	pcCDC->FillRect(&rect, pcBrush);
	delete pcBrush;
	EpsDispLoad(pcCDC, epsname);

	m_edKmnView.ReleaseDC(pcCDC);
}

void CDialogEpsKamonLoad::EpsDispLoad(CDC *pcCDC, LPSTR epsname)
{
	char	kmnpath[FNAMEMAX];
	char	pathstr[FNAMEMAX];
	FILE	*fp;

	strcpy(pathstr, epsname);
	pathstr[2] = '\0';
	senvgetkmnpath(kmnpath);
	wsprintf(m_szFileName, "%s\\%s\\%s.eps", kmnpath, pathstr, epsname);
	if((fp = fopen(m_szFileName, "rb")) != NULL){
		SkipEpsHead(fp);
		DispEpsLoadLoop(fp, pcCDC);
		fclose(fp);
	}
}

void CDialogEpsKamonLoad::SkipEpsHead(FILE *fp)
{
	char	buffer[RECMAX];

	fseek(fp, 0x0FA00, 0);
	while(1){
		if(ReadString(fp, buffer, RECMAX) == FALSE){
			break;
		}
		if(strncmp(buffer, "%%EndSetup", 10) == 0){
			break;
		}
	}
}

void CDialogEpsKamonLoad::DispEpsLoadLoop(FILE *fp, CDC *pcCDC)
{
	char	buffer[RECMAX];
	int		len;
	DBL		crtx, crty;
	DBL		x2, y2, x3, y3, x4, y4;

	while(1){
		if(ReadString(fp, buffer, RECMAX) == FALSE){
			break;
		}
		if(strncmp(buffer, "%%Trailer", 9) == 0){
			break;
		}
		len = strlen(buffer);
		if(buffer[len-2] != ' '){
			continue;
		}
		if(NumChk(buffer, len-1) == OFF){
			continue;
		}
		switch(buffer[len-1]){
		case 'm':
		case 'M':
			sscanf(buffer, "%lf %lf", &crtx, &crty);
			DispTimeTrns(&crtx, &crty);
			pcCDC->MoveTo((int)crtx, (int)crty);
			break;
		case 'l':
		case 'L':
			sscanf(buffer, "%lf %lf", &crtx, &crty);
			DispTimeTrns(&crtx, &crty);
			pcCDC->LineTo((int)crtx, (int)crty);
			break;
		case 'c':
		case 'C':
			sscanf(buffer, "%lf %lf %lf %lf %lf %lf",
					&x2, &y2, &x3, &y3, &x4, &y4);
			DispTimeTrns(&x2, &y2);
			DispTimeTrns(&x3, &y3);
			DispTimeTrns(&x4, &y4);
			BezeDraw(pcCDC, crtx, crty, x2, y2, x3, y3, x4, y4);
			crtx = x4; crty = y4;
			break;
		case 'v':
		case 'V':
			sscanf(buffer, "%lf %lf %lf %lf",
					&x3, &y3, &x4, &y4);
			DispTimeTrns(&x3, &y3);
			DispTimeTrns(&x4, &y4);
			x2 = x3; y2 = y3;
			BezeDraw(pcCDC, crtx, crty, x2, y2, x3, y3, x4, y4);
			crtx = x4; crty = y4;
			break;
		case 'y':
		case 'Y':
			sscanf(buffer, "%lf %lf %lf %lf",
					&x2, &y2, &x3, &y3);
			DispTimeTrns(&x2, &y2);
			DispTimeTrns(&x3, &y3);
			x4 = x3; y4 = y3;
			BezeDraw(pcCDC, crtx, crty, x2, y2, x3, y3, x4, y4);
			crtx = x4; crty = y4;
			break;
		}
	}
}

void CDialogEpsKamonLoad::BezeDraw(CDC *pcCDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4)
{
	DBL	t, t1, b1, b2, b3, b4;
	DBL	xx, yy;

	for(t = 0.25; t < 1.0; t += 0.25){
		t1 = 1 - t;
		b1 = t1 * t1 * t1;
		b2 = 3 * t * t1 * t1;
		b3 = 3 * t * t * t1;
		b4 = t * t * t;
		xx = (b1 * x1 + b2 * x2 + b3 * x3 + b4 * x4);
		yy = (b1 * y1 + b2 * y2 + b3 * y3 + b4 * y4);
		pcCDC->LineTo((int)xx, (int)yy);
	}
	pcCDC->LineTo((int)x4, (int)y4);
}

void CDialogEpsKamonLoad::DispTimeTrns(LPDBL xx, LPDBL yy)
{
	*xx = *xx * m_dTime + m_dDispSx;
	*yy = (m_dDispSize - *yy * m_dTime) + m_dDispSy;
}

int CDialogEpsKamonLoad::NumChk(char buf[], int len)
{
	int		i;

	for(i = 0; i < len; i++){
		if(('0' <= buf[i] && buf[i] <= '9')
		|| ('.' == buf[i])
		|| ('-' == buf[i])
		|| (' ' == buf[i])){
			;
		}
		else{
			return(OFF);
		}
	}
	return(ON);
}


void CDialogEpsKamonLoad::OnSelchangeNamelist() 
{
	int		idx;

	UpdateData(TRUE);
	idx = m_lstName.GetCurSel();
	KamnSelectDisp(idx);
}

void CDialogEpsKamonLoad::OnOK() 
{
		
	CDialog::OnOK();
}

BOOL CDialogEpsKamonLoad::ReadString(FILE *fp, char *buffer, int max)
{
	int		idx, ret;

	idx = 0;
	while(1){
		if(max-2 < idx){
			break;
		}
		ret = fread(&buffer[idx], 1, 1, fp);
		if(ret == 0){
			buffer[idx] = '\0';
			return(FALSE);
		}
		if(buffer[idx] != 0x0a){
			if(buffer[idx] == 0x0d){
				buffer[idx] = '\0';
				idx++;
				break;
			}
			idx++;
		}
	}
	buffer[idx] = '\0';
	return(TRUE);
}

