// DlgGSlct.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogGaijiSlct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSlct ダイアログ

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Senvlib.h"
//}

CDialogGaijiSlct::CDialogGaijiSlct(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGaijiSlct::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogGaijiSlct)
	//}}AFX_DATA_INIT
}


void CDialogGaijiSlct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGaijiSlct)
	DDX_Control(pDX, IDC_LSTSTR, m_lstStr);
	DDX_Control(pDX, IDC_CMBNAME, m_cmbName);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDTEXT11, m_csText[0]);
	DDX_Text(pDX, IDC_EDTEXT12, m_csText[1]);
	DDX_Text(pDX, IDC_EDTEXT13, m_csText[2]);
	DDX_Text(pDX, IDC_EDTEXT14, m_csText[3]);
	DDX_Text(pDX, IDC_EDTEXT15, m_csText[4]);
	DDX_Text(pDX, IDC_EDTEXT16, m_csText[5]);
	DDX_Text(pDX, IDC_EDTEXT17, m_csText[6]);
	DDX_Text(pDX, IDC_EDTEXT18, m_csText[7]);
	DDX_Text(pDX, IDC_EDTEXT21, m_csText[8]);
	DDX_Text(pDX, IDC_EDTEXT22, m_csText[9]);
	DDX_Text(pDX, IDC_EDTEXT23, m_csText[10]);
	DDX_Text(pDX, IDC_EDTEXT24, m_csText[11]);
	DDX_Text(pDX, IDC_EDTEXT25, m_csText[12]);
	DDX_Text(pDX, IDC_EDTEXT26, m_csText[13]);
	DDX_Text(pDX, IDC_EDTEXT27, m_csText[14]);
	DDX_Text(pDX, IDC_EDTEXT28, m_csText[15]);
	DDX_Text(pDX, IDC_EDTEXT31, m_csText[16]);
	DDX_Text(pDX, IDC_EDTEXT32, m_csText[17]);
	DDX_Text(pDX, IDC_EDTEXT33, m_csText[18]);
	DDX_Text(pDX, IDC_EDTEXT34, m_csText[19]);
	DDX_Text(pDX, IDC_EDTEXT35, m_csText[20]);
	DDX_Text(pDX, IDC_EDTEXT36, m_csText[21]);
	DDX_Text(pDX, IDC_EDTEXT37, m_csText[22]);
	DDX_Text(pDX, IDC_EDTEXT38, m_csText[23]);
	DDX_Text(pDX, IDC_EDTEXT41, m_csText[24]);
	DDX_Text(pDX, IDC_EDTEXT42, m_csText[25]);
	DDX_Text(pDX, IDC_EDTEXT43, m_csText[26]);
	DDX_Text(pDX, IDC_EDTEXT44, m_csText[27]);
	DDX_Text(pDX, IDC_EDTEXT45, m_csText[28]);
	DDX_Text(pDX, IDC_EDTEXT46, m_csText[29]);
	DDX_Text(pDX, IDC_EDTEXT47, m_csText[30]);
	DDX_Text(pDX, IDC_EDTEXT48, m_csText[31]);
}


BEGIN_MESSAGE_MAP(CDialogGaijiSlct, CDialog)
	//{{AFX_MSG_MAP(CDialogGaijiSlct)
	ON_EN_SETFOCUS(IDC_EDTEXT11, OnSetfocusEdtext11)
	ON_EN_SETFOCUS(IDC_EDTEXT12, OnSetfocusEdtext12)
	ON_EN_SETFOCUS(IDC_EDTEXT13, OnSetfocusEdtext13)
	ON_EN_SETFOCUS(IDC_EDTEXT14, OnSetfocusEdtext14)
	ON_EN_SETFOCUS(IDC_EDTEXT15, OnSetfocusEdtext15)
	ON_EN_SETFOCUS(IDC_EDTEXT16, OnSetfocusEdtext16)
	ON_EN_SETFOCUS(IDC_EDTEXT17, OnSetfocusEdtext17)
	ON_EN_SETFOCUS(IDC_EDTEXT18, OnSetfocusEdtext18)
	ON_EN_SETFOCUS(IDC_EDTEXT21, OnSetfocusEdtext21)
	ON_EN_SETFOCUS(IDC_EDTEXT22, OnSetfocusEdtext22)
	ON_EN_SETFOCUS(IDC_EDTEXT23, OnSetfocusEdtext23)
	ON_EN_SETFOCUS(IDC_EDTEXT24, OnSetfocusEdtext24)
	ON_EN_SETFOCUS(IDC_EDTEXT25, OnSetfocusEdtext25)
	ON_EN_SETFOCUS(IDC_EDTEXT26, OnSetfocusEdtext26)
	ON_EN_SETFOCUS(IDC_EDTEXT27, OnSetfocusEdtext27)
	ON_EN_SETFOCUS(IDC_EDTEXT28, OnSetfocusEdtext28)
	ON_EN_SETFOCUS(IDC_EDTEXT31, OnSetfocusEdtext31)
	ON_EN_SETFOCUS(IDC_EDTEXT32, OnSetfocusEdtext32)
	ON_EN_SETFOCUS(IDC_EDTEXT33, OnSetfocusEdtext33)
	ON_EN_SETFOCUS(IDC_EDTEXT34, OnSetfocusEdtext34)
	ON_EN_SETFOCUS(IDC_EDTEXT35, OnSetfocusEdtext35)
	ON_EN_SETFOCUS(IDC_EDTEXT36, OnSetfocusEdtext36)
	ON_EN_SETFOCUS(IDC_EDTEXT37, OnSetfocusEdtext37)
	ON_EN_SETFOCUS(IDC_EDTEXT38, OnSetfocusEdtext38)
	ON_EN_SETFOCUS(IDC_EDTEXT41, OnSetfocusEdtext41)
	ON_EN_SETFOCUS(IDC_EDTEXT42, OnSetfocusEdtext42)
	ON_EN_SETFOCUS(IDC_EDTEXT43, OnSetfocusEdtext43)
	ON_EN_SETFOCUS(IDC_EDTEXT44, OnSetfocusEdtext44)
	ON_EN_SETFOCUS(IDC_EDTEXT45, OnSetfocusEdtext45)
	ON_EN_SETFOCUS(IDC_EDTEXT46, OnSetfocusEdtext46)
	ON_EN_SETFOCUS(IDC_EDTEXT47, OnSetfocusEdtext47)
	ON_EN_SETFOCUS(IDC_EDTEXT48, OnSetfocusEdtext48)
	ON_CBN_SELCHANGE(IDC_CMBNAME, OnSelchangeCmbname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGaijiSlct メッセージ ハンドラ

BOOL CDialogGaijiSlct::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitNameCmb();
	UpdateEditTable(m_nSlct);
	UpdateData(FALSE);
	return TRUE;
}

void CDialogGaijiSlct::InitNameCmb()
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szGjFName[FNAMEMAX];
	FILE	*fp;
	TCHAR	szBuf[QRECMAX];
	TCHAR	szName[QRECMAX];
	TCHAR	szStr[QRECMAX];
	LPTSTR	sp;

	senvgetexepath(szExePath);
	_stprintf_s(szGjFName, _T("%s\\GAIJITBL.LST"), szExePath);
	_tfopen_s(&fp, szGjFName, _T("r"));
	if (fp == NULL) {
		return;
	}
	while(1){
		if(_fgetts(szBuf, QRECMAX, fp) == NULL){
			break;
		}
		sp = szBuf;
		sp = basecsvstrdivcpy(sp, szName);
		m_cmbName.AddString(szName);
		sp = basecsvstrdivcpy(sp, szStr);
		m_lstStr.AddString(szStr);
	}
	m_nSlct = 0;
	m_cmbName.SetCurSel(m_nSlct);
	fclose(fp);
}

void CDialogGaijiSlct::OnSelchangeCmbname() 
{
	UpdateData(TRUE);
	m_nSlct = m_cmbName.GetCurSel();
	UpdateEditTable(m_nSlct);
	UpdateData(FALSE);
}

void CDialogGaijiSlct::UpdateEditTable(int nslct)
{
	LPTSTR	sp;
	TCHAR	szChar[4];
	int		i;
	int		len;

	len = _tcslen(_T("あ"));
	m_lstStr.GetText(nslct, m_szStr);
	sp = m_szStr;
	for(i = 0; i < GJTBLMAX; i++){
		if(*sp == _T('\0')){
			break;
		}
		_tcsncpy(szChar, sp, len);
		szChar[len] = _T('\0');
		m_csText[i] = szChar;
		sp += len;
	}
	for(; i < GJTBLMAX; i++){
		m_csText[i] = _T("");
	}
	m_szStr[0] = _T('\0');
}

void CDialogGaijiSlct::OnSetfocusEdtext11() { SetEndSlct(0); }
void CDialogGaijiSlct::OnSetfocusEdtext12() { SetEndSlct(1); }
void CDialogGaijiSlct::OnSetfocusEdtext13() { SetEndSlct(2); }
void CDialogGaijiSlct::OnSetfocusEdtext14() { SetEndSlct(3); }
void CDialogGaijiSlct::OnSetfocusEdtext15() { SetEndSlct(4); }
void CDialogGaijiSlct::OnSetfocusEdtext16() { SetEndSlct(5); }
void CDialogGaijiSlct::OnSetfocusEdtext17() { SetEndSlct(6); }
void CDialogGaijiSlct::OnSetfocusEdtext18() { SetEndSlct(7); }
void CDialogGaijiSlct::OnSetfocusEdtext21() { SetEndSlct(8); }
void CDialogGaijiSlct::OnSetfocusEdtext22() { SetEndSlct(9); }
void CDialogGaijiSlct::OnSetfocusEdtext23() { SetEndSlct(10); }
void CDialogGaijiSlct::OnSetfocusEdtext24() { SetEndSlct(11); }
void CDialogGaijiSlct::OnSetfocusEdtext25() { SetEndSlct(12); }
void CDialogGaijiSlct::OnSetfocusEdtext26() { SetEndSlct(13); }
void CDialogGaijiSlct::OnSetfocusEdtext27() { SetEndSlct(14); }
void CDialogGaijiSlct::OnSetfocusEdtext28() { SetEndSlct(15); }
void CDialogGaijiSlct::OnSetfocusEdtext31() { SetEndSlct(16); }
void CDialogGaijiSlct::OnSetfocusEdtext32() { SetEndSlct(17); }
void CDialogGaijiSlct::OnSetfocusEdtext33() { SetEndSlct(18); }
void CDialogGaijiSlct::OnSetfocusEdtext34() { SetEndSlct(19); }
void CDialogGaijiSlct::OnSetfocusEdtext35() { SetEndSlct(20); }
void CDialogGaijiSlct::OnSetfocusEdtext36() { SetEndSlct(21); }
void CDialogGaijiSlct::OnSetfocusEdtext37() { SetEndSlct(22); }
void CDialogGaijiSlct::OnSetfocusEdtext38() { SetEndSlct(23); }
void CDialogGaijiSlct::OnSetfocusEdtext41() { SetEndSlct(24); }
void CDialogGaijiSlct::OnSetfocusEdtext42() { SetEndSlct(25); }
void CDialogGaijiSlct::OnSetfocusEdtext43() { SetEndSlct(26); }
void CDialogGaijiSlct::OnSetfocusEdtext44() { SetEndSlct(27); }
void CDialogGaijiSlct::OnSetfocusEdtext45() { SetEndSlct(28); }
void CDialogGaijiSlct::OnSetfocusEdtext46() { SetEndSlct(29); }
void CDialogGaijiSlct::OnSetfocusEdtext47() { SetEndSlct(30); }
void CDialogGaijiSlct::OnSetfocusEdtext48() { SetEndSlct(31); }

void CDialogGaijiSlct::SetEndSlct(int idx)
{
	UpdateData(TRUE);
	_tcscpy_s(m_szStr, m_csText[idx].GetBuffer(QRECMAX));
	CDialog::OnOK();
}
