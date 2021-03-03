// DlgKmnLd.h.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgKmnLd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogKamonLoad ダイアログ

extern "C" {
	void senvgetexepath(LPSTR path);
}

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
	ON_EN_CHANGE(IDC_SRCHCHAR, OnChangeSrchchar)
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
	char	exepath[FNAMEMAX+2];
	char	fname[FNAMEMAX+2];
	FILE	*fp;
	char	buf[QRECMAX];
	int		len;

	CListBox* pLB = (CListBox*)GetDlgItem(IDC_NAMELIST);
	senvgetexepath(exepath);
	sprintf(fname, "%s\\kamncrct.lst", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(fgets(buf, RECMAX, fp) == NULL){
				break;
			}
			len = strlen(buf);
			buf[len-1] = '\0';
			pLB->AddString(buf);
		}
		fclose(fp);
	}
}

void CDialogKamonLoad::OnChangeSrchchar() 
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
	CListBox* pLB = (CListBox*)GetDlgItem(IDC_NAMELIST);
	senvgetexepath(exepath);
	sprintf(fname, "%s\\kamnchar.lst", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		while(1){
			if(fgets(buf, QRECMAX, fp) == NULL){
				break;
			}
			if(strncmp(buf, srchbuf, 2) == 0){
				sscanf(buf, "%s %d", name, &idx);
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
	char	srchbuf[QRECMAX];
	int		idx, idxmax, idxlast;

	UpdateData(TRUE);
	strcpy(srchbuf, m_csSrchStrg.GetBuffer(QRECMAX));
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
	char	buf[RECMAX];
	char	bname[QRECMAX];
	char	kname[QRECMAX];

	CListBox* pLB = (CListBox*)GetDlgItem(IDC_NAMELIST);
	pLB->GetText(idx, buf);
	sscanf(buf, "%s %s %s %s", bname, kname, m_szKamonChar, m_szKamonFont);

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
