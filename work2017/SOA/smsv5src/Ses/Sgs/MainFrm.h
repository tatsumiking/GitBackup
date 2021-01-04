
// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once

#include	"../../Object/FSF1Font.h"
#include	"../../Object/ESF0Font.h"
#include	"../../Object/ESF1Font.h"
#include	"../../Object/FBU0Font.h"
#include	"../../Object/WinTTFont.h"
#include	"../../Object/ARCFont.h"

#include	"../Sgs/DialogWakuBarG1.h"
#include	"../Sgs/DialogWakuBarG2.h"

class CTonView;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 属性
public:
	int		m_nFirstFlag;
	int		m_nCrtCommand;
	TCHAR	m_szKaisyaName[HRECMAX];
	DWORD	m_dwClr;
	CTonView* m_pcCrtView;
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CFBU0Font* m_pcFBU0Font;
	CWinTTFont* m_pcWNTTFont;
	CARCFont* m_pcARCFont;
	int		m_nTTEisuFlag;
	int		m_nOptSlctObj;
	int		m_nOptChangeCurser;
	int		m_nEpsClrTrns;
	DBL		m_dBackMMX, m_dBackMMY;
public:
	GdiplusStartupInput m_gdiSI;
	ULONG_PTR           m_gdiToken;

protected:  // コントロール バー用メンバー
	CSize		m_szBaseUnits;
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndLeftSideBar;
	CDialogWakuBarG1 m_wndWakuBarG1;
	CDialogWakuBarG2 m_wndWakuBarG2;
	// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// 実装
public:
	BOOL CreateWakuBarG1();
	BOOL CreateWakuBarG2();
	void StatXYOut(DBL xabs, DBL yabs);
	void HelpTextOut(int idx, LPTSTR msgbuf);
	void ShowItemElement();
	void HideItemElement();
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void ScrnAddEnvSave();
	void ScrnAddEnvLoad();
	void ThisViewMax(CTonView *pcView);
	void SetCrtView(CTonView * pcView);
	void SetSWakLocate();
	int CheckProtectFile();
	int CheckProtectUsb();
	CTonView* GetCrtView();
protected:
	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
private:
	void InitExePath(LPTSTR szExePath);
	void EnvLoad();
	void EnvSave();
	void ScrnEnvSave();
	void MojiEnvSave();
	void FiguEnvSave();
	void DivDrawEnvLoad();
	void ScrnEnvLoad();
	void MojiEnvLoad();
	void FiguEnvLoad();
	void FontEnvLoad();
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
	// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

};


