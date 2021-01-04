// DlgKmnLd.h.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogKamonLoad.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
//}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogKamonLoad ダイアログ

CDialogKamonLoad::CDialogKamonLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogKamonLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogKamonLoad)
	m_dKmnSize = 0.0;
	m_csSrchChar = _T("");
	m_csSrchStrg = _T("");
	m_csKmnView = _T("");
	//}}AFX_DATA_INIT
}


void CDialogKamonLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogKamonLoad)
	DDX_Text(pDX, IDC_KMNSIZE, m_dKmnSize);
	DDX_Text(pDX, IDC_SRCHCHAR, m_csSrchChar);
	DDX_Text(pDX, IDC_SRCHSTRG, m_csSrchStrg);
	DDX_Text(pDX, IDC_KMNVIEW, m_csKmnView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogKamonLoad, CDialog)
	//{{AFX_MSG_MAP(CDialogKamonLoad)
	ON_EN_CHANGE(IDC_SRCHCHAR, OnChangeSrchChar)
	ON_EN_CHANGE(IDC_SRCHSTRG, OnChangeSrchstrg)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_NAMELIST, OnSelchangeNamelist)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPKMNSIZE, OnDeltaposSpkmnsize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogKamonLoad メッセージ ハンドラ
BOOL CDialogKamonLoad::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	NameListInit();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogKamonLoad::NameListInit()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[QRECMAX];
	int		len;

	CListBox* pLB = (CListBox*)GetDlgItem(IDC_NAMELIST);
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\kamncrct.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		while(1){
			if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			pLB->AddString(buf);
		}
		fclose(fp);
	}
}

void CDialogKamonLoad::OnChangeSrchChar() 
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	srchbuf[QRECMAX];
	int		len;
	TCHAR	name[QRECMAX];
	int		idx, idxmax, idxlast;

	UpdateData(TRUE);
	_tcscpy_s(srchbuf, m_csSrchChar.GetBuffer(QRECMAX));
	len = _tcslen(srchbuf);
	CListBox* pLB = (CListBox*)GetDlgItem(IDC_NAMELIST);
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\kamnchar.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp != NULL){
		while(1){
			if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
				break;
			}
			if(_tcsnccmp(buf, srchbuf, len) == 0){
				_stscanf_s(buf, _T("%s %d"), name, QRECMAX, &idx);
				idxlast = idx+18;
				idxmax = pLB->GetCount();
				if(idxmax <= idxlast){
					idxlast = idxmax-1;
				}
				pLB->SetCurSel(idxlast);
				pLB->SetCurSel(idx);
				KamnSelectDisp(idx);
				break;
			}
		}
		fclose(fp);
	}
}

void CDialogKamonLoad::OnChangeSrchstrg() 
{
	TCHAR	srchbuf[QRECMAX];
	int		idx, idxmax, idxlast;

	UpdateData(TRUE);
	_tcscpy_s(srchbuf, m_csSrchStrg.GetBuffer(QRECMAX));
	CListBox* pLB = (CListBox*)GetDlgItem(IDC_NAMELIST);
	idx = pLB-> FindString(-1, srchbuf);
	idxlast = idx+LISTMAX;
	idxmax = pLB->GetCount();
	if(idxmax <= idxlast){
		idxlast = idxmax-1;
	}
	pLB->SetCurSel(idxlast);
	pLB->SetCurSel(idx);
	KamnSelectDisp(idx);
}

void CDialogKamonLoad::KamnSelectDisp(int idx)
{
	TCHAR	buf[RECMAX];
	TCHAR	bname[QRECMAX];
	TCHAR	kname[QRECMAX];

	CListBox* pLB = (CListBox*)GetDlgItem(IDC_NAMELIST);
	pLB->GetText(idx, buf);
	_stscanf_s(buf, _T("%s %s %s %s"), bname, QRECMAX, kname, QRECMAX, m_szKamonChar, QRECMAX, m_szKamonFont, QRECMAX);

	CEdit* pED = (CEdit*)GetDlgItem(IDC_KMNVIEW);

	CDC *pcCDC = pED->GetDC();
	CFont* pcfontView = new CFont();
	pcfontView->CreateFont(64, 64, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET,OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,m_szKamonFont);

	CFont *pcoldFont = pcCDC->SelectObject(pcfontView);
	pcCDC->TextOut(1, 1, m_szKamonChar, 1);
	pcCDC->SelectObject(pcoldFont);
	delete(pcfontView);
	pED->ReleaseDC(pcCDC);
}

void CDialogKamonLoad::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
}

void CDialogKamonLoad::OnSelchangeNamelist() 
{
	int		idx;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CListBox* pLB = (CListBox*)GetDlgItem(IDC_NAMELIST);
	idx = pLB->GetCurSel();
	KamnSelectDisp(idx);
}


void CDialogKamonLoad::OnDeltaposSpkmnsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(TRUE);
	m_dKmnSize = m_dKmnSize - pNMUpDown->iDelta;
	*pResult = 1;
	UpdateData(FALSE);
}
