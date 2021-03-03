// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MAINFRM_H)
#define MAINFRM_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include	"..\object\fsf1font.h"
#include	"..\object\esf0font.h"
#include	"..\object\esf1font.h"
#include	"..\object\wnttfont.h"

#include	"DClrBar.h"
#include	"..\cmnsrc\DWkBar1.h"
#include	"..\cmnsrc\DWkBar2.h"
#include	"..\cmnsrc\DWkBar3.h"
#include	"..\cmnsrc\DWkBarSD.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
// アトリビュート
public:
	int		m_nFirstFlag;
	HIMC	m_hImc;
	HIMC	m_hOldImc;
	int		m_nCrtCommand;
	char	m_szKaisyaName[QRECMAX];
	char	m_szEnvPath[FNAMEMAX];
	DWORD	m_dwClr;
	CTonView* m_pcCrtView;
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CWNTTFont* m_pcWNTTFont;
	int		m_nTTEisuFlag;
	int		m_nOptSlctObj;
	int		m_nOptChangeCurser;
	int		m_nEpsClrTrns;
	int		m_nMainExec;
	UINT	m_nTimerID;
public:

protected:  // コントロール バー用メンバ
	CSize		m_szBaseUnits;
	int		m_nStatusBar;
	CStatusBar  m_wndStatusBar;
	CDialogClrBar m_wndClrBar;
	CDialogWakuBar1 m_wndWakuBar1;
	CDialogWakuBar2 m_wndWakuBar2;
	CDialogWakuBar3 m_wndWakuBar3;
	CDialogWakuBarSide m_wndWakuBarSide;
	CToolBar    m_wndLeftSideBar;
public:
	CMainFrame();
	virtual ~CMainFrame();

// オペレーション
public:
	void StartTimer();
	void InitIme();
	void EndIme();
	void OnIme();
	void OffIme();
	void StatXYOut(DBL xabs, DBL yabs);
	void HelpTextOut(int idx, LPSTR msgbuf);
	void ShowItemElement();
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void ScrnAddEnvSave();
	void ScrnAddEnvLoad();
	int GetFiguKind();
	void ThisViewMax(CTonView *pcView);
	void SetCrtView(CTonView * pcView);
	void MemoryEnvSave();

protected:
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);
	BOOL CreateClrBar();
	BOOL CreateWakuBar1();
	BOOL CreateWakuBar2();
	BOOL CreateWakuBar3();
	BOOL CreateWakuBarSide();

private:
	void InitExePath(LPSTR szExePath);
	void EnvLoad();
	void EnvSave();
	void ScrnEnvSave();
	void MojiEnvSave();
	void FiguEnvSave();
	void WakuEnvSave();
	void DivDrawEnvLoad();
	void ScrnEnvLoad();
	void MojiEnvLoad();
	void FiguEnvLoad();
	void FontEnvLoad();
	void WakuEnvLoad();
	void MemoryEnvLoad();
	char* CrDelFgets(char *buf, int max, FILE *fp);
	void TrueTypeFontListSet(int idx);
	static BOOL CALLBACK AFX_EXPORT EnumFamSiftjisCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID lpAdrs);
	static BOOL CALLBACK AFX_EXPORT EnumFamAsciiCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID lpAdrs);
	void StrgSpaceDelCpy(LPSTR dststrg, LPSTR srcstrg);

public:
	CTonView* GetCrtView();
	void SetSWakLocate();
	int CheckProtectFile();
	int CheckProtectUsb();
	void HideItemElement();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnWakuTateyoko();
	afx_msg void OnWakuConerkind();
	afx_msg void OnWakuStartkind();
	afx_msg void OnFileSlctprint();
	afx_msg void OnFileCenterkind();
	afx_msg void OnFileSavepaper();
	afx_msg void OnFileSetspace();
	afx_msg void OnWakuPaperadd();
	afx_msg void OnViewStatusBar();
	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(MAINFRM_H)
