// DialogToolObjectBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlTolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogToolObjectBar ダイアログ

CDialogToolObjectBar::CDialogToolObjectBar(CWnd* pParent /*=NULL*/)
	: CMyDialogBar(pParent)
{
	m_pcView = NULL;
	m_nFirst = OFF;
	//{{AFX_DATA_INIT(CDialogToolObjectBar)
	//}}AFX_DATA_INIT
}

void CDialogToolObjectBar::DoDataExchange(CDataExchange* pDX)
{
	CMyDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogToolObjectBar)
	DDX_Control(pDX, ID_OBJ_SELECT, m_btnObjSelect);
	DDX_Control(pDX, ID_OBJ_FIGU, m_btnObjFigu);
	DDX_Control(pDX, ID_OBJ_TEXT, m_btnObjText);
	DDX_Control(pDX, ID_PAINT_EXEC, m_btnPaintExec);
	DDX_Control(pDX, ID_OBJ_DELETE, m_btnObjDelete);
	DDX_Control(pDX, ID_CRDNT_ONEUP, m_btnCrdntOneUp);
	DDX_Control(pDX, ID_FILE_INPORT, m_btnFileInport);
	DDX_Control(pDX, ID_CRDNT_ONEDOWN, m_btnCrdntOneDown);
	DDX_Control(pDX, ID_ZOOM_AREA, m_btnZoomArea);
	DDX_Control(pDX, ID_FILE_PRINT, m_btnFilePrint);
	DDX_Control(pDX, ID_CLR_PALLET, m_btnClrPallet);
	DDX_Control(pDX, ID_FILE_ENVSET, m_btnFileEnvset);
	DDX_Control(pDX, ID_FILE_DELETE, m_btnFileDelete);
	DDX_Control(pDX, ID_REDRAW, m_btnRedraw);
	DDX_Control(pDX, ID_SYSTEM_HELP, m_btnSystemHelp);
	DDX_Control(pDX, ID_CRCLDATA1, m_btnCrcldata1);
	DDX_Control(pDX, ID_CRCLDATA2, m_btnCrcldata2);
	DDX_Control(pDX, ID_CRCLDATA3, m_btnCrcldata3);
	DDX_Control(pDX, ID_RECTDATA4, m_btnRectdata4);
	DDX_Control(pDX, ID_RECTDATA5, m_btnRectdata5);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogToolObjectBar, CMyDialogBar)
	//{{AFX_MSG_MAP(CDialogToolObjectBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_OBJ_SELECT, OnObjSelect)
	ON_BN_CLICKED(ID_OBJ_FIGU, OnObjFigu)
	ON_BN_CLICKED(ID_OBJ_TEXT, OnObjText)
	ON_BN_CLICKED(ID_PAINT_EXEC, OnPaintExec)
	ON_BN_CLICKED(ID_OBJ_DELETE, OnObjDelete)
	ON_BN_CLICKED(ID_CRDNT_ONEUP, OnCrdntOneup)
	ON_BN_CLICKED(ID_FILE_INPORT, OnFileInport)
	ON_BN_CLICKED(ID_CRDNT_ONEDOWN, OnCrdntOnedown)
	ON_BN_CLICKED(ID_ZOOM_AREA, OnZoomArea)
	ON_BN_CLICKED(ID_FILE_PRINT, OnFilePrint)
	ON_BN_CLICKED(ID_CLR_PALLET, OnClrPallet)
	ON_BN_CLICKED(ID_FILE_ENVSET, OnFileEnvset)
	ON_BN_CLICKED(ID_FILE_DELETE, OnFileDelete)
	ON_BN_CLICKED(ID_REDRAW, OnRedraw)
	ON_BN_CLICKED(ID_SYSTEM_HELP, OnSystemHelp)
	ON_BN_CLICKED(ID_CRCLDATA1, OnCrcldata1)
	ON_BN_CLICKED(ID_CRCLDATA2, OnCrcldata2)
	ON_BN_CLICKED(ID_CRCLDATA3, OnCrcldata3)
	ON_BN_CLICKED(ID_RECTDATA4, OnRectdata4)
	ON_BN_CLICKED(ID_RECTDATA5, OnRectdata5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogToolObjectBar メッセージ ハンドラ

int CDialogToolObjectBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	int		ret;

	if (CMyDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	m_nFirst = ON;
	
	return 0;
}

void CDialogToolObjectBar::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	if(m_nFirst == ON){
		m_nFirst = OFF;

		VERIFY(m_btnObjSelect.SubclassDlgItem(ID_OBJ_SELECT, this));
		VERIFY(m_btnObjFigu.SubclassDlgItem(ID_OBJ_FIGU, this));
		VERIFY(m_btnObjText.SubclassDlgItem(ID_OBJ_TEXT, this));
		VERIFY(m_btnPaintExec.SubclassDlgItem(ID_PAINT_EXEC, this));
		VERIFY(m_btnObjDelete.SubclassDlgItem(ID_OBJ_DELETE, this));
		VERIFY(m_btnCrdntOneUp.SubclassDlgItem(ID_CRDNT_ONEUP, this));
		VERIFY(m_btnFileInport.SubclassDlgItem(ID_FILE_INPORT, this));
		VERIFY(m_btnCrdntOneDown.SubclassDlgItem(ID_CRDNT_ONEDOWN, this));
		VERIFY(m_btnZoomArea.SubclassDlgItem(ID_ZOOM_AREA, this));
		VERIFY(m_btnFilePrint.SubclassDlgItem(ID_FILE_PRINT, this));
		VERIFY(m_btnClrPallet.SubclassDlgItem(ID_CLR_PALLET, this));
		VERIFY(m_btnFileEnvset.SubclassDlgItem(ID_FILE_ENVSET, this));
		VERIFY(m_btnFileDelete.SubclassDlgItem(ID_FILE_DELETE, this));
		VERIFY(m_btnRedraw.SubclassDlgItem(ID_REDRAW, this));
		VERIFY(m_btnSystemHelp.SubclassDlgItem(ID_SYSTEM_HELP, this));
		VERIFY(m_btnCrcldata1.SubclassDlgItem(ID_CRCLDATA1, this));
		VERIFY(m_btnCrcldata2.SubclassDlgItem(ID_CRCLDATA2, this));
		VERIFY(m_btnCrcldata3.SubclassDlgItem(ID_CRCLDATA3, this));
		VERIFY(m_btnRectdata4.SubclassDlgItem(ID_RECTDATA4, this));
		VERIFY(m_btnRectdata5.SubclassDlgItem(ID_RECTDATA5, this));

		SetButtonElement();
		UpdateData(FALSE);
	}
}

void CDialogToolObjectBar::SetButtonElement()
{
	CTonApp*	theApp;
	CMainFrame*	theWnd;

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	if(theWnd->m_nCrtCommand == ID_OBJ_SELECT){
		m_btnObjSelect.LoadBitmaps(IDB_OBJ_SELECT_DOWN, NULL, NULL);
	}
	else{
		m_btnObjSelect.LoadBitmaps(IDB_OBJ_SELECT_UP, IDB_OBJ_SELECT_DOWN, NULL);
	}
	if(theWnd->m_nCrtCommand == ID_OBJ_FIGU){
		m_btnObjFigu.LoadBitmaps(IDB_OBJ_FIGU_DOWN, NULL, NULL);
	}
	else{
		m_btnObjFigu.LoadBitmaps(IDB_OBJ_FIGU_UP, IDB_OBJ_FIGU_DOWN, NULL);
	}
	if(theWnd->m_nCrtCommand == ID_OBJ_TEXT){
		m_btnObjText.LoadBitmaps(IDB_OBJ_TEXT_DOWN, NULL, NULL);
	}
	else{
		m_btnObjText.LoadBitmaps(IDB_OBJ_TEXT_UP, IDB_OBJ_TEXT_DOWN, NULL);
	}
	if(theWnd->m_nCrtCommand == ID_PAINT_EXEC){
		m_btnPaintExec.LoadBitmaps(IDB_PAINT_EXEC_DOWN, NULL, NULL);
	}
	else{
		m_btnPaintExec.LoadBitmaps(IDB_PAINT_EXEC_UP, IDB_PAINT_EXEC_DOWN, NULL);
	}
	//if(theWnd->m_nCrtCommand == ID_OBJ_DELETE){
	//	m_btnObjDelete.LoadBitmaps(IDB_OBJ_DELETE_DOWN, NULL, NULL);
	//}
	//else{
		m_btnObjDelete.LoadBitmaps(IDB_OBJ_DELETE_UP, IDB_OBJ_DELETE_DOWN, NULL);
	//}
	//if(theWnd->m_nCrtCommand == ID_CRDNT_ONEUP){
	//	m_btnCrdntOneUp.LoadBitmaps(IDB_CRDNT_ONEUP_DOWN, NULL, NULL);
	//}
	//else{
		m_btnCrdntOneUp.LoadBitmaps(IDB_CRDNT_ONEUP_UP, IRDB_CDNT_ONEUP_DOWN, NULL);
	//}
	if(theWnd->m_nCrtCommand == ID_FILE_INPORT){
		m_btnFileInport.LoadBitmaps(IDB_FILE_INPORT_DOWN, NULL, NULL);
	}
	else{
		m_btnFileInport.LoadBitmaps(IDB_FILE_INPORT_UP, IDB_FILE_INPORT_DOWN, NULL);
	}
	//if(theWnd->m_nCrtCommand == ID_CRDNT_ONEDOWN){
	//	m_btnCrdntOneDown.LoadBitmaps(IDB_CRDNT_ONEDOWN_DOWN, NULL, NULL);
	//}
	//else{
		m_btnCrdntOneDown.LoadBitmaps(IDB_CRDNT_ONEDOWN_UP, IDB_CRDNT_ONEDOWN_DOWN, NULL);
	//}
	if(theWnd->m_nCrtCommand == ID_ZOOM_AREA){
		m_btnZoomArea.LoadBitmaps(IDB_ZOOM_AREA_DOWN, NULL, NULL);
	}
	else{
		m_btnZoomArea.LoadBitmaps(IDB_ZOOM_AREA_UP, IDB_ZOOM_AREA_DOWN, NULL);
	}
	if(theWnd->m_nCrtCommand == ID_FILE_PRINT){
		m_btnFilePrint.LoadBitmaps(IDB_FILE_PRINT_DOWN, NULL, NULL);
	}
	else{
		m_btnFilePrint.LoadBitmaps(IDB_FILE_PRINT_UP, IDB_FILE_PRINT_DOWN, NULL);
	}
	if(theWnd->m_nCrtCommand == ID_CLR_PALLET){
		m_btnClrPallet.LoadBitmaps(IDB_CLR_PALLET_DOWN, NULL, NULL);
	}
	else{
		m_btnClrPallet.LoadBitmaps(IDB_CLR_PALLET_UP, IDB_CLR_PALLET_DOWN, NULL);
	}
	//if(theWnd->m_nCrtCommand == ID_FILE_ENNVSET){
	//	m_btnFileDelete.LoadBitmaps(IDB_FILE_ENVSET_DOWN, NULL, NULL);
	//}
	//else{
		m_btnFileEnvset.LoadBitmaps(IDB_FILE_ENVSET_UP, IDB_FILE_ENVSET_DOWN, NULL);
	//}
	//if(theWnd->m_nCrtCommand == ID_FILE_DELETE){
	//	m_btnFileDelete.LoadBitmaps(IDB_FILE_DELETE_DOWN, NULL, NULL);
	//}
	//else{
		m_btnFileDelete.LoadBitmaps(IDB_FILE_DELETE_UP, IDB_FILE_DELETE_DOWN, NULL);
	//}
	//if(theWnd->m_nCrtCommand == ID_REDRAW){
	//	m_btnRedraw.LoadBitmaps(IDB_REDRAW_DOWN, NULL, NULL);
	//}
	//else{
		m_btnRedraw.LoadBitmaps(IDB_REDRAW_UP, IDB_REDRAW_DOWN, NULL);
	//}
	if(theWnd->m_nCrtCommand == ID_SYSTEM_HELP){
		m_btnSystemHelp.LoadBitmaps(IDB_SYSTEM_HELP_DOWN, NULL, NULL);
	}
	else{
		m_btnSystemHelp.LoadBitmaps(IDB_SYSTEM_HELP_UP, IDB_SYSTEM_HELP_DOWN, NULL);
	}
	if(theWnd->m_nCrtDataNo == 1){
		m_btnCrcldata1.LoadBitmaps(IDB_CRCLDATA1_DOWN, NULL, NULL);
	}
	else{
		m_btnCrcldata1.LoadBitmaps(IDB_CRCLDATA1_UP, IDB_CRCLDATA1_DOWN, NULL);
	}
	if(theWnd->m_nCrtDataNo == 2){
		m_btnCrcldata2.LoadBitmaps(IDB_CRCLDATA2_DOWN, NULL, NULL);
	}
	else{
		m_btnCrcldata2.LoadBitmaps(IDB_CRCLDATA2_UP, IDB_CRCLDATA2_DOWN, NULL);
	}
	if(theWnd->m_nCrtDataNo == 3){
		m_btnCrcldata3.LoadBitmaps(IDB_CRCLDATA3_DOWN, NULL, NULL);
	}
	else{
		m_btnCrcldata3.LoadBitmaps(IDB_CRCLDATA3_UP, IDB_CRCLDATA3_DOWN, NULL);
	}
	if(theWnd->m_nCrtDataNo == 4){
		m_btnRectdata4.LoadBitmaps(IDB_RECTDATA4_DOWN, NULL, NULL);
	}
	else{
		m_btnRectdata4.LoadBitmaps(IDB_RECTDATA4_UP, IDB_RECTDATA4_DOWN, NULL);
	}
	if(theWnd->m_nCrtDataNo == 5){
		m_btnRectdata5.LoadBitmaps(IDB_RECTDATA5_DOWN, NULL, NULL);
	}
	else{
		m_btnRectdata5.LoadBitmaps(IDB_RECTDATA5_UP, IDB_RECTDATA5_DOWN, NULL);
	}
}

void CDialogToolObjectBar::SetCrtView(CTonView * pcView)
{
	m_pcView = pcView;
}

void CDialogToolObjectBar::OnSystemHelp() 
{
	m_pcView->OnSystemHelp();	
}

void CDialogToolObjectBar::OnRedraw() 
{
	m_pcView->OnRedraw();	
}

void CDialogToolObjectBar::OnFileDelete() 
{
	m_pcView->OnFileDelete();
}

void CDialogToolObjectBar::OnObjSelect() 
{
	m_pcView->OnObjSelect();
}

void CDialogToolObjectBar::OnObjFigu() 
{
	m_pcView->OnObjFigu();
}

void CDialogToolObjectBar::OnObjText() 
{
	m_pcView->OnObjText();
}

void CDialogToolObjectBar::OnPaintExec() 
{
	m_pcView->OnPaintExec();
}

void CDialogToolObjectBar::OnObjDelete() 
{
	m_pcView->OnObjDelete();
}

void CDialogToolObjectBar::OnCrdntOneup() 
{
	m_pcView->OnCrdntOneup();
}

void CDialogToolObjectBar::OnFileInport() 
{
	m_pcView->OnFileInport();
}

void CDialogToolObjectBar::OnCrdntOnedown() 
{
	m_pcView->OnCrdntOnedown();
}

void CDialogToolObjectBar::OnZoomArea() 
{
	m_pcView->OnZoomArea();
}

void CDialogToolObjectBar::OnFilePrint() 
{
	m_pcView->OnFilePrint();
}

void CDialogToolObjectBar::OnClrPallet() 
{
	m_pcView->OnClrPallet();
}

void CDialogToolObjectBar::OnFileEnvset() 
{
	m_pcView->OnFileEnvset();
}

void CDialogToolObjectBar::SetItemElement()
{
	SetButtonElement();	
	Invalidate();
}

void CDialogToolObjectBar::OnRectdata4() 
{
	m_pcView->OnRectdata4();
}

void CDialogToolObjectBar::OnRectdata5() 
{
	m_pcView->OnRectdata5();
}

void CDialogToolObjectBar::OnCrcldata1() 
{
	m_pcView->OnCrcldata1();
}

void CDialogToolObjectBar::OnCrcldata2() 
{
	m_pcView->OnCrcldata2();
}

void CDialogToolObjectBar::OnCrcldata3() 
{
	m_pcView->OnCrcldata3();
}

