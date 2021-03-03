// DDBEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DaoDBLib.h"
#include "DDBEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDBEdit ダイアログ
#define DBEDITFILEMAX	250
#define	STARTCOL	1	// 選択チェックボックスの次から

CDialogDBEdit::CDialogDBEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDBEdit::IDD, pParent)
{
	m_pcApp = (CTonApp*)::AfxGetApp();
	m_pcView = (CTonView *)pParent;
	m_pcDoc = m_pcView->GetDocument();
	m_nMode = MODAL;
	m_nFirst = TRUE;
	m_nFildMax = 0;
	m_nLineMax = 0;
	m_nCrtCol = 0;
	m_nCrtLine = 0;
	//{{AFX_DATA_INIT(CDialogDBEdit)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

void CDialogDBEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDBEdit)
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_LSTITEM, m_lstFileList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDBEdit, CDialog)
	//{{AFX_MSG_MAP(CDialogDBEdit)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LSTITEM, OnClickLstitem)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LSTITEM, OnCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDBEdit メッセージ ハンドラ

BOOL CDialogDBEdit::OnInitDialog() 
{
	int nFild;
	int	nCol;
	int width;

	CDialog::OnInitDialog();

	InitDaoDBLib();
	if(m_pcDaoDBLib->m_nDBOpen == FALSE){
		m_nLineMax = 0;
		return(FALSE);
	}
	m_nFildMax = InitFildName();
	if(m_nFildMax == 0){
		EndDaoDBLib();
		m_nLineMax = 0;
		return(FALSE);
	}
	m_nLineMax = InitRecord();

	// 発行チェックボックス追加
	m_lstFileList.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES);

	//	カラムサイズ修正
	for(nFild = 0; nFild < m_nFildMax; nFild++){
		nCol = nFild + STARTCOL;
		m_lstFileList.SetColumnWidth(nCol, LVSCW_AUTOSIZE);
		width = m_lstFileList.GetColumnWidth(nCol);
		if(width >= 200){
			m_lstFileList.SetColumnWidth(nCol, 200);
		}
	}
	
	EndDaoDBLib();
	
	return TRUE;
}

void CDialogDBEdit::OnSize(UINT nType, int cx, int cy) 
{
	CRect	rectTop;
	CRect	rectList;
	CRect	rect;
	int		xsize, ysize, oneysize;

	if(m_nFildMax == 0 || m_nLineMax == 0){
		return;
	}
	int space = 12;

	CDialog::OnSize(nType, cx, cy);
	m_stTop.GetWindowRect(&rectTop);
	m_lstFileList.GetWindowRect(&rectList);
	rectList.top -= rectTop.top;
	rectList.left -= rectTop.left;
	rectList.bottom -= rectTop.top;
	rectList.right -= rectTop.left;
	cx -= (rectList.left + space);
	cy -= (rectList.top + space);
	GetListCtrlSize(&xsize, &ysize, &oneysize);
	cy = cy / oneysize * oneysize + 6;
	
	xsize += 4;
	ysize += 4;
	int	addx = 1;
	int	addy = 1;
	if(cx < xsize){
		addy += GetSystemMetrics(SM_CYHSCROLL);
	}
	if(cy < ysize){
		addx += GetSystemMetrics(SM_CXHSCROLL);
	}
	xsize += addx;
	ysize += addy;

	if(cx > xsize){
		cx = xsize;
	}
	if(cy > ysize){
		cy = ysize;
	}
	m_lstFileList.SetWindowPos(0, 0, 0, cx, cy, SWP_NOMOVE|SWP_NOOWNERZORDER);
}

void CDialogDBEdit::OnClickLstitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int		nColMax;
	int		nCheck;

	*pResult = 0;
	LPNMLISTVIEW lpnmlv=(LPNMLISTVIEW)pNMHDR;
	LVHITTESTINFO lvh;
	lvh.pt=lpnmlv->ptAction;
	m_lstFileList.SubItemHitTest(&lvh);
	nColMax = m_nFildMax + STARTCOL;
	if(nColMax < lvh.iSubItem || (int)m_nLineMax <= lvh.iItem){
		return;
	}	
	m_nCrtCol = lvh.iSubItem;
	m_nCrtLine = lvh.iItem;
	if(m_nCrtCol == 0){
		nCheck = m_lstFileList.GetCheck(m_nCrtLine);
		if(nCheck == TRUE){
			m_lstFileList.SetItemText(m_nCrtLine, 1, "0");
		}else{
			m_lstFileList.SetItemText(m_nCrtLine, 1, "1");
		}
	}
	m_lstFileList.Update(0);
	m_pcDoc->UpdateDBObject();
	m_pcView->Invalidate(TRUE);
	OnSetFocus(m_pcView);
}

void CDialogDBEdit::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{//NM_CUSTOMDRAW
	LPNMLVCUSTOMDRAW lcd=(LPNMLVCUSTOMDRAW)pNMHDR;

	switch(lcd->nmcd.dwDrawStage){
	case CDDS_PREPAINT:
		*pResult=CDRF_NOTIFYSUBITEMDRAW;//サブアイテムを処理します。
		break;
	case CDDS_ITEMPREPAINT://アイテム単位で処理します。
		*pResult=CDRF_NOTIFYSUBITEMDRAW;//サブアイテムを処理します。
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		*pResult=CDRF_DODEFAULT;
		// 特定アイテムのサブアイテムの色を指定します。
		if(lcd->nmcd.dwItemSpec == m_nCrtLine){
			lcd->clrText=RGB(0xff,0x00,0x00);
			lcd->clrTextBk=RGB(0x00,0xff,0xff);
			*pResult=CDRF_NEWFONT;
		}else{
			lcd->clrTextBk=GetSysColor(COLOR_WINDOW);
			lcd->clrText=GetSysColor(COLOR_WINDOWTEXT);
			*pResult=CDRF_NEWFONT;
		}
		break;
	default:
		*pResult=CDRF_DODEFAULT;
		break;
	}
}

void CDialogDBEdit::SetModaless()
{
	m_nMode = MODALESS;
}

void CDialogDBEdit::InitItemElement()
{
}

void CDialogDBEdit::InitDaoDBLib()
{
	m_pcDaoDBLib = new CDaoDBLib();
	m_pcDaoDBLib->m_nType = m_pcDoc->m_nDBType;
	m_pcDaoDBLib->m_csFileName = m_pcDoc->m_csDBFileName;
	m_pcDaoDBLib->m_csTableName = m_pcDoc->m_csDBTableName;
	m_pcDaoDBLib->Open();
}

void CDialogDBEdit::EndDaoDBLib()
{
	m_pcDaoDBLib->Close();
	delete(m_pcDaoDBLib);
}

int CDialogDBEdit::InitFildName()
{
	int		fildmax;
	int		getfild;
	int		nCol;

// カラム名設定
// フィールド名(選択非選択)
	//strcpy(m_pcApp->m_lpTempStr, "選択");
	//m_lstFileList.InsertColumn(0, m_pcApp->m_lpTempStr, LVCFMT_LEFT, 20);
	strcpy(m_pcApp->m_lpTempStr, "");
	m_lstFileList.InsertColumn(0, m_pcApp->m_lpTempStr, LVCFMT_LEFT, 1);
// フィールド名(データベース)
	if(m_pcDaoDBLib->InitGetFild() == FALSE){
		return(0);
	}
	fildmax = m_pcDaoDBLib->m_nFildMax;
	if(fildmax > DBEDITFILEMAX){
		fildmax = DBEDITFILEMAX;
		::AfxMessageBox("フィールド数が多すぎるため２５０以降のフィールドは処理できません", MB_OK);
	}
	for(getfild = 0; getfild < fildmax; getfild++){
		m_pcDaoDBLib->GetFildName(getfild, m_pcApp->m_lpTempStr);
		nCol = getfild + STARTCOL;
		m_lstFileList.InsertColumn(nCol, m_pcApp->m_lpTempStr, LVCFMT_LEFT, -1);
	}
	m_pcDaoDBLib->EndGetFild();
	return(fildmax);
}

int CDialogDBEdit::InitRecord()
{
	int		nRecMax;
	int		line;
	int		getfild;
	UINT	nRow;
	int		nCol;
	int		ret;

	if(m_pcDaoDBLib->InitGetRecord() == FALSE){
		return(0);
	}
	nRecMax = 10000;
	line = 0;
	while(1){
		if(line >= nRecMax){
			line = nRecMax;
			sprintf(m_pcApp->m_lpTempStr, "レコード数が多すぎるため%d以降のレコードは処理できません", nRecMax); 
			::AfxMessageBox(m_pcApp->m_lpTempStr, MB_OK);
			break;
		}
		nRow = line;
		m_lstFileList.InsertItem(nRow+1, " ");
		//m_lstFileList.SetCheck(nRow, TRUE);
		//m_lstFileList.SetItemText(nRow, 1, "0");
		m_lstFileList.SetItemText(nRow, 1, "");
		ret = FALSE;
		for(getfild = 0; getfild < m_nFildMax; getfild++){
			m_pcDaoDBLib->GetItemStr(getfild, m_pcApp->m_lpTempStr);
			nCol = getfild + STARTCOL;
			m_lstFileList.SetItemText(nRow, nCol, m_pcApp->m_lpTempStr);
			if(m_pcApp->m_lpTempStr[0] != '\0'){
				ret = TRUE;
			}
		}
		if(m_pcDoc->m_nDBType == DBXLS && ret == FALSE){
			m_lstFileList.DeleteItem(nRow);
			break;
		}
		line++;
		ret = m_pcDaoDBLib->SetNextRecord();
		if(ret == FALSE){
			break;
		}
	}
	m_pcDaoDBLib->EndGetRecord();
	return(line);
}

void CDialogDBEdit::GetFildName(int nFildNo, LPSTR lpFildName)
{
	LVCOLUMN	cLvcolumn;
	int		nCol;
	
	cLvcolumn.mask       = LVCF_TEXT;
	cLvcolumn.pszText    = lpFildName;
	cLvcolumn.cchTextMax = QRECMAX;
	nCol = nFildNo+STARTCOL;
	m_lstFileList.GetColumn(nCol, &cLvcolumn);
}

void CDialogDBEdit::GetListCtrlSize(int *xsize, int *ysize, int *oneysize)
{
	CRect	rect;
	UINT	nRow;
	int		nFild, nCol;

	nRow = 0;
	m_lstFileList.GetItemRect(0, &rect, LVIR_LABEL);
	*xsize = rect.Width() + 20;
	m_lstFileList.GetSubItemRect(nRow, 1, LVIR_BOUNDS, rect);
	(*xsize) += rect.Width();
	for(nFild = 0; nFild < m_nFildMax; nFild++){
		nCol = nFild + STARTCOL;
		m_lstFileList.GetSubItemRect(nRow, nCol, LVIR_BOUNDS, rect);
		(*xsize) += rect.Width();
	}

	nCol = 1;
	m_lstFileList.GetSubItemRect(0, nCol, LVIR_BOUNDS, rect);
	*ysize = rect.Height();
	for(nRow = 0; nRow < (UINT)m_nLineMax; nRow++){
		m_lstFileList.GetSubItemRect(nRow, nCol, LVIR_BOUNDS, rect);
		(*ysize) += rect.Height();
	}
	*oneysize = rect.Height();
}

int CDialogDBEdit::GetFildElement(LPSTR lpFildName, LPSTR lpStr)
{
	int nFildNo = GetFildNo(lpFildName);
	if(nFildNo == -1){
		return(FALSE);
	}
	GetFildElement(nFildNo, lpStr);
	return(TRUE);
}

int CDialogDBEdit::GetFildNo(LPSTR lpFildName)
{
	LVCOLUMN	cLvcolumn;
	int		nFildNo;
	int		nCol;
	char	szFildName[RECMAX];

	cLvcolumn.mask       = LVCF_TEXT;
	cLvcolumn.pszText    = szFildName;
	cLvcolumn.cchTextMax = QRECMAX;
	for(nFildNo = 0; nFildNo < m_nFildMax; nFildNo++){
		nCol = nFildNo+STARTCOL;
		m_lstFileList.GetColumn(nCol, &cLvcolumn);
		if(strcmp(lpFildName, szFildName) == 0){
			return(nFildNo);
		}
	}
	return(-1);
}

void CDialogDBEdit::GetFildElement(int nFildNo, LPSTR lpStr)
{
	int		nCol;

	if(m_nCrtLine == -1){
		lpStr[0] = '\0';
		return;
	}
	nCol = nFildNo+STARTCOL;
	m_lstFileList.GetItemText(m_nCrtLine, nCol, lpStr, RECMAX8);
	int len = strlen(lpStr);
}
