// DialogESKamonLoad.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlEKmnLd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	WAKUSIZE		150
#define WAKUSPACE		20
#define WAKUCOUNT		6

extern "C" {
int kamonlibopen(char *mode);
int kamonlibwakuopen(char *mode);
void kamonlibclose();
int kamonlibreadhead(int no, kamonheadtype *head);
int kamonlibwritehead(int no, kamonheadtype *head, int pcnt);
int kamonlibreadatrxy(int *atr, DBL *dx, DBL *dy);
int kamonlibwriteatrxy(int atr, DBL dx, DBL dy);
void pdrwbezelow(HDC PaintDC, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3, DBL x4, DBL y4);
};
/////////////////////////////////////////////////////////////////////////////
// CDialogESKamonLoad ダイアログ

CDialogESKamonLoad::CDialogESKamonLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogESKamonLoad::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	m_nCrtTop = 1;
	//{{AFX_DATA_INIT(CDialogESKamonLoad)
	m_dKmnSize = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogESKamonLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogESKamonLoad)
	DDX_Control(pDX, IDC_KMNLIST, m_lstKamonName);
	DDX_Text(pDX, IDC_EDKMNSIZE, m_dKmnSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogESKamonLoad, CDialog)
	//{{AFX_MSG_MAP(CDialogESKamonLoad)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_NEXTPAGE, OnNextpage)
	ON_BN_CLICKED(IDC_BEFORPAGE, OnBeforpage)
	ON_LBN_SELCHANGE(IDC_KMNLIST, OnSelchangeKmnlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogESKamonLoad メッセージ ハンドラ

void CDialogESKamonLoad::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int		sx, sy;
	int		idx;

	m_nSelect = -1;
	sx = WAKUSPACE;
	sy = WAKUSPACE;
	idx = 0;
	while(1){
		if(sx <= point.x && point.x <= (sx+WAKUSIZE+WAKUSPACE)
		&& sy <= point.y && point.y <= (sy+WAKUSIZE)){
			m_nSelect = idx + m_nCrtTop;
			break;
		}
		idx++;
		sx = sx + WAKUSIZE + WAKUSPACE;
		if((sx + WAKUSIZE) > m_nDlgWidth){
			sx = WAKUSPACE;
			sy = sy + WAKUSIZE + WAKUSPACE;
			if((sy + WAKUSIZE) > m_nDlgHight){
				break;
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);

	if(0 <= m_nSelect && m_nSelect < KAMONDATAMAX){
		CDialog::OnOK();
	}
	else{
		CDialog::OnCancel();
	}
}

void CDialogESKamonLoad::OnPaint() 
{
	CRect	cRect, cTRect;
	long	nsx, nsy, nex, ney;
	HPEN	hPen, hOldPen;
	HBRUSH	hBrush, hOldBrush;

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	m_lstKamonName.GetWindowRect(&cTRect);	
	GetWindowRect(&cRect);
	m_nDlgWidth = cRect.Width() -  cTRect.Width() - WAKUSPACE;
	m_nDlgHight = cRect.Height();

    hPen = ::CreatePen(0, 1, RGB(255, 255, 255));
	hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
    hOldPen = (HPEN)::SelectObject(dc.m_hDC, hPen);
    hOldBrush = (HBRUSH)::SelectObject(dc.m_hDC, hBrush);

	nsx = 0;
	nsy = 0;
	nex = m_nDlgWidth;
	ney = m_nDlgHight;
	::Rectangle(dc.m_hDC, nsx, nsy, nex, ney);

    hPen = (HPEN)::SelectObject(dc.m_hDC, hOldPen);
    hBrush = (HBRUSH)::SelectObject(dc.m_hDC, hOldBrush);
    ::DeleteObject(hPen);
    ::DeleteObject(hBrush);

	KamonAllDraw(&dc);
}

void CDialogESKamonLoad::KamonAllDraw(CDC *pcDC)
{
	DBL		sx, sy;
	DBL		ex, ey;
	int		idx, no;
	kamonheadtype head;
	int		flag, pcnt;
	int		atr;
	DBL		dx, dy;
	DBL		x1, y1, x2, y2, x3, y3, x4, y4;
	DBL		rsx, rsy;
	int		bezeflag;
	char	nobuf[QRECMAX];

	m_dTrnsTime = WAKUSIZE / 256.0;

	kamonlibopen("r+b");

	sx = WAKUSPACE;
	sy = WAKUSPACE;
	idx = 0;
	while(1){
		ex = sx + WAKUSIZE;
		ey = sy + WAKUSIZE;
		no = idx + m_nCrtTop;
		pcDC->MoveTo((int)sx, (int)sy);
		pcDC->LineTo((int)ex, (int)sy);
		pcDC->LineTo((int)ex, (int)ey);
		pcDC->LineTo((int)sx, (int)ey);
		pcDC->LineTo((int)sx, (int)sy);
		pcnt = kamonlibreadhead(no, &head);
		if(pcnt > 0){
			sprintf(nobuf, "No.%d", no);
			TextOut(pcDC->m_hDC, (int)sx, (int)sy, nobuf, strlen(nobuf));
			if(strlen(head.fullname) == 0){
				TextOut(pcDC->m_hDC, (int)sx, (int)ey, head.name, strlen(head.name));
			}
			else{
				TextOut(pcDC->m_hDC, (int)sx, (int)ey, head.fullname, strlen(head.fullname));
			}
		}
		flag = ON;
		bezeflag = 0;
		for(;pcnt >= 0; pcnt--){
			kamonlibreadatrxy(&atr, &dx, &dy);
			x1 = (dx * m_dTrnsTime + sx);
			y1 = (dy * m_dTrnsTime + sy);
			if(flag == ON){
				pcDC->MoveTo((int)x1, (int)y1);
				if((atr & BEZEATR)){
					x4 = x1, y4 = y1;
					bezeflag = 1;
				}
				else{
					bezeflag = 0;
				}
				rsx = x1;
				rsy = y1;
				flag = OFF;
			}
			else{
				if(bezeflag == 1){
					x3 = x1, y3 = y1;
					bezeflag = 2;
				}
				else if(bezeflag == 2){
					x2 = x1, y2 = y1;
					bezeflag = 3;
				}
				else if(bezeflag == 3){
					pdrwbezelow(pcDC->m_hDC, x4, y4, x3, y3, x2, y2, x1, y1);
					if((atr & BEZEATR)){
						x4 = x1, y4 = y1;
						bezeflag = 1;
					}
					else{
						bezeflag = 0;
					}
				}
				else{
					pcDC->LineTo((int)x1, (int)y1);
					if((atr & BEZEATR)){
						x4 = x1, y4 = y1;
						bezeflag = 1;
					}
					else{
						bezeflag = 0;
					}
				}
			}
			if(atr & REJIONMASK){
				flag = ON;
				if(atr & KAMONREJION){
					if(bezeflag == 1){
						pcDC->LineTo((int)x4, (int)y4);
						pcDC->LineTo((int)rsx, (int)rsy);
					}
					else if(bezeflag == 2){
						x2 = rsx; y2 = rsy;
						x1 = rsx; y1 = rsy;
						pdrwbezelow(pcDC->m_hDC, x4, y4, x3, y3, x2, y2, x1, y1);
					}
					else if(bezeflag == 3){
						x1 = rsx; y1 = rsy;
						pdrwbezelow(pcDC->m_hDC, x4, y4, x3, y3, x2, y2, x1, y1);
					}
					else{
						pcDC->LineTo((int)rsx, (int)rsy);
					}
					bezeflag = 0;
				}
			}
		}
		idx++;
		sx = sx + WAKUSIZE + WAKUSPACE;
		if((sx + WAKUSIZE) > m_nDlgWidth){
			sx = WAKUSPACE;
			sy = sy + WAKUSIZE + WAKUSPACE;
			if((sy + WAKUSIZE) > m_nDlgHight){
				break;
			}
		}
	}
	kamonlibclose();
}

void CDialogESKamonLoad::OnNextpage() 
{
	m_nCrtTop = m_nCrtTop + WAKUCOUNT/2;
	if(m_nCrtTop > (KAMONDATAMAX - WAKUCOUNT)){
		m_nCrtTop = KAMONDATAMAX - WAKUCOUNT;
	}
	Invalidate(FALSE);
}

void CDialogESKamonLoad::OnBeforpage() 
{
	m_nCrtTop = m_nCrtTop - WAKUCOUNT/2;
	if(m_nCrtTop < 1){
		m_nCrtTop = 1;
	}
	Invalidate(FALSE);
}

BOOL CDialogESKamonLoad::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitKamonNameList();
	// TODO: この位置に初期化の補足処理を追加してください

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogESKamonLoad::InitKamonNameList()
{
	int		idx;
	kamonheadtype head;
	char	buf[RECMAX];

	kamonlibopen("r+b");
	idx = 0;
	while(1){
		if(kamonlibreadhead(idx+1, &head) == FAIL){
			break;
		}
		if(strlen(head.fullname) == 0){
			sprintf(buf, "%03d %s", idx+1, head.name);
		}
		else{
			sprintf(buf, "%03d %s", idx+1, head.fullname);
		}
		m_lstKamonName.AddString(buf);
		idx++;
	}
	kamonlibclose();
}

void CDialogESKamonLoad::OnSelchangeKmnlist() 
{
	int		idx;
	char	buf[RECMAX];

	idx = m_lstKamonName.GetCurSel();
	m_lstKamonName.GetText(idx, buf);
	m_nCrtTop = atoi(buf);	
	Invalidate(FALSE);
}
