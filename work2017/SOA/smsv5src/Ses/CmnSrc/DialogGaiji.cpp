// DlgGaiji.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogGaiji.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogGaiji ダイアログ

static POINT dbUnit;

static int dbX(int n)
{
   return ((n * dbUnit.x + 2) / 4);
}

static int dbY(int n)
{
   return ((n * dbUnit.y + 4) / 8);
}

CDialogGaiji::CDialogGaiji(CWnd* pParent /*=NULL*/)
: CMyDialog(CDialogGaiji::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogGaiji)
	m_csTitle = _T("");
	//}}AFX_DATA_INIT
}


void CDialogGaiji::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGaiji)
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_COMBO1, m_cmbTitle);
	DDX_CBString(pDX, IDC_COMBO1, m_csTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogGaiji, CDialog)
	//{{AFX_MSG_MAP(CDialogGaiji)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGaiji メッセージ ハンドラ

BOOL CDialogGaiji::OnInitDialog() 
{
	CRect	rect;
	int		idx;
	long	dwUnit;

	CMyDialog::OnInitDialog();

	m_szGaijiText[0] = _T('\0');
	dwUnit = GetDialogBaseUnits();
	dbUnit.x = (dwUnit >> 16) & 0x0000ffff;
	dbUnit.y = (dwUnit) & 0x0000ffff;
	rect.left = 6;
	rect.right = rect.left + 24;
	rect.top = 0;
	rect.bottom = 32;
	for(idx = 0; idx < GAIJIEDITMAX; idx++){
		if((idx % 8) == 0){
			rect.left = 8;
			rect.right = rect.left + 24;
			rect.top = rect.bottom + 4;
			rect.bottom = rect.top + 24;
		}
		m_pcEdit[idx] = new CMyEdit();
		m_pcEdit[idx]->Create(EDITNONSTYLE, rect, this, IDC_EDIT1+idx);
		rect.left = rect.right + 4;
		rect.right = rect.left + 24;
	}

	InitTitleCombo();
	UpdateData(TRUE);
	IniTCHAREdit();

	return TRUE;
}

void CDialogGaiji::InitTitleCombo()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	FILE	*fp;
	LPTSTR	sp;
	TCHAR	title[QRECMAX];
	TCHAR	codes[HRECMAX];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\Gaijitbl.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r,ccs=UNICODE"));
	if(fp != NULL){
		while(1){
			if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			sp = buf;
			sp = basecsvstrdivcpy(sp, title);
			sp = basecsvstrdivcpy(sp, codes);
			m_cmbTitle.AddString(title);
		}
		fclose(fp);
	}
	m_cmbTitle.SetCurSel(0);
}

void CDialogGaiji::IniTCHAREdit()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	TCHAR	buf[RECMAX];
	FILE	*fp;
	int		idx;
	LPTSTR	sp;
	TCHAR	ttitle[QRECMAX];
	TCHAR	title[QRECMAX];
	TCHAR	codes[HRECMAX];
	TCHAR	setCode[4];

	_tcscpy_s(ttitle, m_csTitle.GetBuffer(QRECMAX));

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\Gaijitbl.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r,ccs=UNICODE"));
	if(fp != NULL){
		while(1){
			if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			sp = buf;
			sp = basecsvstrdivcpy(sp, title);
			sp = basecsvstrdivcpy(sp, codes);
			if(_tcscmp(ttitle, title) == 0){
				for(idx = 0; idx < GAIJIEDITMAX; idx++){
#ifdef UNICODE
					if (codes[idx] == _T('\0')) {
						break;
					}
					setCode[0] = codes[idx];
					setCode[1] = _T('\0');
#else
					if(codes[idx*2] == _T('\0')){
						break;
					}
					setCode[0] = codes[idx*2];
					setCode[1] = codes[idx*2+1];
					setCode[2] = _T('\0');
#endif
					m_pcEdit[idx]->SetWindowText(setCode);
					
				}
				setCode[0] = _T('\0');
				for(; idx < GAIJIEDITMAX; idx++){
					m_pcEdit[idx]->SetWindowText(setCode);
				}
				break;
			}
		}
		fclose(fp);
	}
	m_cmbTitle.SetCurSel(0);
}

void CDialogGaiji::OnSelchangeCombo1() 
{
	int		idx;
	TCHAR	szStr[QRECMAX];

	idx = m_cmbTitle.GetCurSel();
	m_cmbTitle.GetLBText(idx, szStr);
	m_csTitle = szStr;
	IniTCHAREdit();
	UpdateData(FALSE);
}

void CDialogGaiji::SetGaijiText(int x, int y) 
{
	CRect	rect;
	int		idx;

	m_szGaijiText[0] = _T('\0');
	rect.left = 6;
	rect.right = rect.left + 24;
	rect.top = 0;
	rect.bottom = 32;
	for(idx = 0; idx < GAIJIEDITMAX; idx++){
		if((idx % 8) == 0){
			rect.left = 8;
			rect.right = rect.left + 24;
			rect.top = rect.bottom + 4;
			rect.bottom = rect.top + 24;
		}
		if(rect.left <= x && x <= rect.right
		&& rect.top <= y && y <= rect.bottom){
			m_pcEdit[idx]->GetWindowText(m_szGaijiText, QRECMAX);
			break;
		}
		rect.left = rect.right + 4;
		rect.right = rect.left + 24;
	}
	CMyDialog::OnOK();
}

BOOL CDialogGaiji::PreTranslateMessage(MSG* pMsg) 
{
	CRect	rect;
	int		x, y;

    switch (pMsg->message){
	case WM_LBUTTONUP:
		m_stTop.GetWindowRect(&rect);
		x = pMsg->pt.x - rect.left;
		y = pMsg->pt.y - rect.top;
		if(32 < y){
			SetGaijiText(x, y);
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
