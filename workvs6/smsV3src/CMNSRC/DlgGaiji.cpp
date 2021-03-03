// DlgGaiji.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgGaiji.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogGaiji ダイアログ

extern "C" {
	LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR retstr);
	void senvgetexepath(LPSTR path);
}

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

	m_szGaijiText[0] = '\0';
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
	InitCharEdit();

	return TRUE;
}

void CDialogGaiji::InitTitleCombo()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	buf[RECMAX];
	FILE	*fp;
	LPSTR	sp;
	char	title[QRECMAX];
	char	chars[HRECMAX];

	senvgetexepath(exepath);
	sprintf(fname, "%s\\Gaijitbl.lst", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(fgets(buf, RECMAX, fp) == NULL){
				break;
			}
			sp = buf;
			sp = basecsvstrdivcpy(sp, title);
			sp = basecsvstrdivcpy(sp, chars);
			m_cmbTitle.AddString(title);
		}
		fclose(fp);
	}
	m_cmbTitle.SetCurSel(0);
}

void CDialogGaiji::InitCharEdit()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	char	buf[RECMAX];
	FILE	*fp;
	int		idx;
	LPSTR	sp;
	char	ttitle[QRECMAX];
	char	title[QRECMAX];
	char	chars[HRECMAX];
	char	setchar[4];

	strcpy(ttitle, m_csTitle.GetBuffer(QRECMAX));

	senvgetexepath(exepath);
	sprintf(fname, "%s\\Gaijitbl.lst", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(fgets(buf, RECMAX, fp) == NULL){
				break;
			}
			sp = buf;
			sp = basecsvstrdivcpy(sp, title);
			sp = basecsvstrdivcpy(sp, chars);
			if(strcmp(ttitle, title) == 0){
				for(idx = 0; idx < GAIJIEDITMAX; idx++){
					if(chars[idx*2] == '\0'){
						break;
					}
					setchar[0] = chars[idx*2];
					setchar[1] = chars[idx*2+1];
					setchar[2] = '\0';
					m_pcEdit[idx]->SetWindowText(setchar);
					
				}
				setchar[0] = '\0';
				for(; idx < GAIJIEDITMAX; idx++){
					m_pcEdit[idx]->SetWindowText(setchar);
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
	char	szStr[QRECMAX];

	idx = m_cmbTitle.GetCurSel();
	m_cmbTitle.GetLBText(idx, szStr);
	m_csTitle = _T(szStr);
	InitCharEdit();
	UpdateData(FALSE);
}

void CDialogGaiji::SetGaijiText(int x, int y) 
{
	CRect	rect;
	int		idx;

	m_szGaijiText[0] = '\0';
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
