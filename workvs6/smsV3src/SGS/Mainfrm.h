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
#include	"..\object\arcfont.h"

#include	"..\cmnsrc\DWkBarG1.h"
#include	"..\cmnsrc\DWkBarG2.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
// アトリビュート
public:
	int		m_nFirstFlag;
	int		m_nCrtCommand;
	char	m_szKaisyaName[QRECMAX];
	DWORD	m_dwClr;
	CTonView* m_pcCrtView;
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CWNTTFont* m_pcWNTTFont;
	CARCFont* m_pcARCFont;
	int		m_nTTEisuFlag;
	int		m_nOptSlctObj;
	int		m_nOptChangeCurser;
	int		m_nEpsClrTrns;
	DBL		m_dBackMMX, m_dBackMMY;

public:

protected:  // コントロール バー用メンバ
	CSize		m_szBaseUnits;
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndLeftSideBar;
	CDialogWakuBarG1 m_wndWakuBarG1;
	CDialogWakuBarG2 m_wndWakuBarG2;
public:
	CMainFrame();
	virtual ~CMainFrame();

// オペレーション
public:
	BOOL CreateWakuBarG1();
	BOOL CreateWakuBarG2();
	void StatXYOut(DBL xabs, DBL yabs);
	void HelpTextOut(int idx, LPSTR msgbuf);
	void ShowItemElement();
	void HideItemElement();
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void ScrnAddEnvSave();
	void ScrnAddEnvLoad();
	void ThisViewMax(CTonView *pcView);
	void SetCrtView(CTonView * pcView);

protected:
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);

private:
	void InitExePath(LPSTR szExePath);
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
	void SetSWakLocate();
	int CheckProtectFile();
	int CheckProtectUsb();
	CTonView* GetCrtView();

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
	afx_msg void OnTateyoko();
	afx_msg void OnConerkind();
	afx_msg void OnStartkind();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBtnsetwaku();
	afx_msg void OnCntrkind();
	afx_msg void OnFiguCntr();
	afx_msg void OnObjBmp();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(MAINFRM_H)
