
// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once

#include	"../../Object/FSF1Font.h"
#include	"../../Object/ESF0Font.h"
#include	"../../Object/ESF1Font.h"
#include	"../../Object/FBU0Font.h"
#include	"../../Object/WinTTFont.h"

#include	"../CmnSrc/DialogClrBar.h"
#include	"../CmnSrc/DialogWakuBar1.h"
#include	"../CmnSrc/DialogWakuBar2.h"
#include	"../CmnSrc/DialogWakuBar3.h"
#include	"../CmnSrc/DialogWakuBarSide.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
// アトリビュート
public:
	int		m_nFirstFlag;
	HIMC	m_hImc;
	HIMC	m_hOldImc;
	int		m_nCrtCommand;
	TCHAR	m_szKaisyaName[QRECMAX];
	TCHAR	m_szEnvPath[FNAMEMAX];
	DWORD	m_dwClr;
	CTonView* m_pcCrtView;
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CFBU0Font* m_pcFBU0Font;
	CWinTTFont* m_pcWNTTFont;
	int		m_nTTEisuFlag;
	int		m_nOptSlctObj;
	int		m_nOptChangeCurser;
	int		m_nEpsClrTrns;
	int		m_nMainExec;
	UINT	m_nTimerID;
	int		m_nFontSetIdx;
public:
	GdiplusStartupInput m_gdiSI;
	ULONG_PTR           m_gdiToken;

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
	CMainFrame() noexcept;
	virtual ~CMainFrame();

// 属性
public:

// 操作
public:
	void StartTimer();
	void InitIme();
	void EndIme();
	void OnIme();
	void OffIme();
	void StatXYOut(DBL xabs, DBL yabs);
	void HelpTextOut(int idx, LPTSTR msgbuf);
	void ShowItemElement();
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void ScrnAddEnvSave();
	void ScrnAddEnvLoad();
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
	void InitExePath(LPTSTR szExePath);
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
	void CreateFolderLoop(LPTSTR lpPath);
	void TrueTypeFontListSet();
	static BOOL CALLBACK AFX_EXPORT EnumFamTategakiCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID lpAdrs);
	static BOOL CALLBACK AFX_EXPORT EnumFamOpenTypeCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType,
		LPVOID lpAdrs);
	static BOOL CALLBACK AFX_EXPORT EnumFamAsciiCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID lpAdrs);

public:
	CTonView* GetCrtView();
	void SetSWakLocate();
	int CheckProtectFile();
	int CheckProtectUsb();
	void HideItemElement();

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバー

// 生成された、メッセージ割り当て関数
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnKeyUp(UINT nTCHAR, UINT nRepCnt, UINT nFlags);
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


