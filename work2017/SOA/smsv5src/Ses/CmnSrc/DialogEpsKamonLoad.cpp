// dlepskld.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogEpsKamonLoad.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	DATASIZE	280.0

/////////////////////////////////////////////////////////////////////////////
// CDialogEpsKamonLoad ダイアログ

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
	ON_EN_CHANGE(IDC_SRCHCHAR, OnChangeSrchChar)
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
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[QRECMAX];
	TCHAR	epsname[QRECMAX];
	TCHAR	kname[QRECMAX];
	int		len;

	senvgetexepath(exepath);
	_stprintf_s(fname, FNAMEMAX, _T("%s\\kamon.csv"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		while(1){
			if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			_tcscpy_s(epsname, QRECMAX, buf);
			epsname[5] = _T('\0');
			_tcscpy_s(kname, &(buf[6]));
			_stprintf_s(buf, FNAMEMAX, _T("%-40s %s"), kname, epsname);
			m_lstName.AddString(buf);
		}
		fclose(fp);
	}
}

void CDialogEpsKamonLoad::OnChangeSrchChar() 
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	srchbuf[QRECMAX];
	TCHAR	name[QRECMAX];
	int		len;
	int		idx, idxmax, idxlast;

	UpdateData(TRUE);
	_tcscpy_s(srchbuf, m_csSrchChar.GetBuffer(QRECMAX));
	len = _tcslen(srchbuf);
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\kmnepsch.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		while(1){
			if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			if(_tcsnccmp(buf, srchbuf, len) == 0){
				_stscanf_s(buf, _T("%s %d"), name, QRECMAX, &idx);
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
	TCHAR	srchbuf[QRECMAX];
	int		idx, idxmax, idxlast;

	UpdateData(TRUE);
	_tcscpy_s(srchbuf, m_csSrchStrg.GetBuffer(QRECMAX));
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
	TCHAR	buf[RECMAX];
	TCHAR	kname[QRECMAX];
	TCHAR	epsname[QRECMAX];
	CRect	rect;

	m_lstName.GetText(idx, buf);
	_stscanf_s(buf, _T("%s %s"), kname, QRECMAX, epsname, QRECMAX);

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

void CDialogEpsKamonLoad::EpsDispLoad(CDC *pcCDC, LPTSTR epsname)
{
	TCHAR	kmnpath[FNAMEMAX];
	TCHAR	pathstr[FNAMEMAX];
	FILE	*fp;

	_tcscpy_s(pathstr, epsname);
	pathstr[2] = _T('\0');
	senvgetkmnpath(kmnpath);
	_stprintf_s(m_szFileName, FNAMEMAX, _T("%s\\%s\\%s.eps"), kmnpath, pathstr, epsname);
	_tfopen_s(&fp, m_szFileName, _T("rb"));
	if(fp != NULL){
		SkipEpsHead(fp);
		DispEpsLoadLoop(fp, pcCDC);
		fclose(fp);
	}
}

void CDialogEpsKamonLoad::SkipEpsHead(FILE *fp)
{
	int len;
	TCHAR	buffer[RECMAX];


	len = _tcslen(_T("%%EndSetup"));
	fseek(fp, 0x0FA00, 0);
	while(1){
		if(ReadString(fp, buffer, RECMAX) == FALSE){
			break;
		}
		if(_tcsnccmp(buffer, _T("%%EndSetup"), len) == 0){
			break;
		}
	}
}

void CDialogEpsKamonLoad::DispEpsLoadLoop(FILE *fp, CDC *pcCDC)
{
	TCHAR	buffer[RECMAX];
	int		len;
	DBL		crtx, crty;
	DBL		x2, y2, x3, y3, x4, y4;

	len = _tcslen(_T("%%Trailer"));
	while(1){
		if(ReadString(fp, buffer, RECMAX) == FALSE){
			break;
		}
		if(_tcsnccmp(buffer, _T("%%Trailer"), len) == 0){
			break;
		}
		len = _tcslen(buffer);
		if(buffer[len-2] != _T(' ')){
			continue;
		}
		if(NumChk(buffer, len-1) == OFF){
			continue;
		}
		switch(buffer[len-1]){
		case _T('m'):
		case _T('M'):
			_stscanf_s(buffer, _T("%lf %lf"), &crtx, &crty);
			DispTimeTrns(&crtx, &crty);
			pcCDC->MoveTo((int)crtx, (int)crty);
			break;
		case _T('l'):
		case _T('L'):
			_stscanf_s(buffer, _T("%lf %lf"), &crtx, &crty);
			DispTimeTrns(&crtx, &crty);
			pcCDC->LineTo((int)crtx, (int)crty);
			break;
		case _T('c'):
		case _T('C'):
			_stscanf_s(buffer, _T("%lf %lf %lf %lf %lf %lf"),
					&x2, &y2, &x3, &y3, &x4, &y4);
			DispTimeTrns(&x2, &y2);
			DispTimeTrns(&x3, &y3);
			DispTimeTrns(&x4, &y4);
			BezeDraw(pcCDC, crtx, crty, x2, y2, x3, y3, x4, y4);
			crtx = x4; crty = y4;
			break;
		case _T('v'):
		case _T('V'):
			_stscanf_s(buffer, _T("%lf %lf %lf %lf"),
					&x3, &y3, &x4, &y4);
			DispTimeTrns(&x3, &y3);
			DispTimeTrns(&x4, &y4);
			x2 = x3; y2 = y3;
			BezeDraw(pcCDC, crtx, crty, x2, y2, x3, y3, x4, y4);
			crtx = x4; crty = y4;
			break;
		case _T('y'):
		case _T('Y'):
			_stscanf_s(buffer, _T("%lf %lf %lf %lf"),
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

int CDialogEpsKamonLoad::NumChk(TCHAR buf[], int len)
{
	int		i;

	for(i = 0; i < len; i++){
		if((_T('0') <= buf[i] && buf[i] <= _T('9'))
		|| (_T('.') == buf[i])
		|| (_T('-') == buf[i])
		|| (_T(' ') == buf[i])){
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

BOOL CDialogEpsKamonLoad::ReadString(FILE *fp, TCHAR *buffer, int max)
{
	int		idx, ret;

	idx = 0;
	while(1){
		if(max-2 < idx){
			break;
		}
		ret = fread(&buffer[idx], 1, 1, fp);
		if(ret == 0){
			buffer[idx] = _T('\0');
			return(FALSE);
		}
		if(buffer[idx] != 0x0a){
			if(buffer[idx] == 0x0d){
				buffer[idx] = _T('\0');
				idx++;
				break;
			}
			idx++;
		}
	}
	buffer[idx] = _T('\0');
	return(TRUE);
}

