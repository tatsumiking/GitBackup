// DlgWkSlct.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgWkSlct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuSlct ダイアログ
extern "C" {
	LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR retstr);
	LPSTR basedivstrg(LPSTR sp, LPSTR retsp, char far sparator);
	int senvsetdrawmode(int mode);
	void wakuenvgetfullpaperpath(LPSTR paperpath);
}

#define	XDIVCOUNT	3
#define YDIVCOUNT	2
#define SPACE	10

CDialogWakuSlct::CDialogWakuSlct(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWakuSlct::IDD, pParent)
{
	m_nSlct = 0;
	m_csWakuName = _T("");
	m_pcView = (CTonView*)pParent;
	m_pcDoc = (CTonDoc*)m_pcView->GetDocument();
	//{{AFX_DATA_INIT(CDialogWakuSlct)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogWakuSlct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuSlct)
	DDX_Control(pDX, IDC_LSTWKNAME, m_lstWkName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogWakuSlct, CDialog)
	//{{AFX_MSG_MAP(CDialogWakuSlct)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuSlct メッセージ ハンドラ

BOOL CDialogWakuSlct::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitWakuNameList();	
	return TRUE;
}

void CDialogWakuSlct::OnPaint() 
{
	CRect	dlgrect;
	int		sx, sy;
	int		x, y;
	CString	csWkName;
	int		idx;
	CRect	rect;

	CPaintDC dc(this);
	senvsetdrawmode(PREVIEW);
	GetWindowRect(&dlgrect);
	m_nXSize = (dlgrect.Width() - (SPACE * (XDIVCOUNT + 1))) / XDIVCOUNT;
	m_nYSize = (dlgrect.Height() - (SPACE * (YDIVCOUNT + 1))) / YDIVCOUNT;
	sy = SPACE;
	idx = 0;
	for(y = 0; y < YDIVCOUNT; y++){
		sx = SPACE;
		for(x = 0; x < XDIVCOUNT; x++){
			rect.left = sx;
			rect.top = sy;
			rect.right = sx + m_nXSize;
			rect.bottom = sy + m_nYSize;
			if(m_nListMax <= idx){
				break;
			}
			m_lstWkName.GetText(idx, csWkName);
			m_pcDoc->DataAllCls();
			m_pcDoc->WakuLoad(csWkName.GetBuffer(FNAMEMAX));
			m_pcDoc->OnFlag(ACTIVEFLAG);
			m_pcView->AllWakuSetString(m_szSetStr);
			m_pcView->DrawWakuInRect(&dc, rect);
			if(idx == m_nSlct){
				DrawSlctRect(&dc, rect);
			}
			sx = rect.right + SPACE;
			idx++;
		}
		sy = rect.bottom + SPACE;
	}
}

BOOL CDialogWakuSlct::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN
	|| pMsg->message == WM_SYSKEYDOWN){
		switch(pMsg->wParam){
		case VK_LEFT:
			m_nSlct = LeglSlctNo(m_nSlct-1);
			Invalidate(TRUE);
			break;
		case VK_RIGHT:
			m_nSlct = LeglSlctNo(m_nSlct+1);
			Invalidate(TRUE);
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogWakuSlct::InitWakuNameList()
{
	char	paperpath[FNAMEMAX];
	char	wakulistfname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX8];
	LPSTR	sp;
	char	wakuname[FNAMEMAX];
	char	wakufile[FNAMEMAX];
	FILE	*wfp;
	int		cnt;

	m_lstWkName.ResetContent();
	wakuenvgetfullpaperpath(paperpath);
	sprintf(wakulistfname, "%s\\wakuname.lst", paperpath);
	if((fp = fopen(wakulistfname, "r")) == NULL){
		return;
	}
	m_nListMax = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, wakuname);
		sprintf(wakufile, "%s\\%s", paperpath, wakuname);
		if((wfp = fopen(wakufile, "r")) == NULL){
			return;
		}
		cnt = 0;
		while(1){
			if(fgets(buf, RECMAX8, wfp) == NULL){
				break;
			}
			cnt++;
		}
		fclose(wfp);
		if(cnt == m_nWakuCnt){
			m_lstWkName.AddString(wakufile);
			m_nListMax++;
		}
	}
	fclose(fp);
}

void CDialogWakuSlct::DrawSlctRect(CDC *pcDC, CRect rect)
{
	CPen	chPen;
	CPen	*pcOldPen;
	int		sx, sy, ex, ey;

	sx = rect.left;
	sy = rect.top;
	ex = rect.right;
	ey = rect.bottom;
	pcDC->SetROP2(R2_XORPEN);
	chPen.CreatePen(PS_SOLID, 5, RGB(0, 255, 255));
	pcOldPen = pcDC->SelectObject(&chPen);
	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(sx, ey);
	pcDC->LineTo(ex, ey);
	pcDC->LineTo(ex, sy);
	pcDC->LineTo(sx, sy);
	pcDC->SelectObject(pcOldPen);
	chPen.DeleteObject();
	pcDC->SetROP2(R2_COPYPEN);
}

int CDialogWakuSlct::LeglSlctNo(int idx)
{
	int		slct;

	slct = idx;
	if(slct < 0){
		slct = 0;
	}
	if(slct >= m_nListMax){
		slct = m_nListMax - 1;
	}
	return(slct);
}

void CDialogWakuSlct::OnOK() 
{
	m_lstWkName.GetText(m_nSlct, m_csWakuName);
	m_pcDoc->DataAllCls();
	CDialog::OnOK();
}
