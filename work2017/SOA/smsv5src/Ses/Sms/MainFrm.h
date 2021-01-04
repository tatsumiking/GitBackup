// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include	"../../Object/FSF1Font.h"
#include	"../../Object/ESF0Font.h"
#include	"../../Object/ESF1Font.h"
#include	"../../Object/FBU0Font.h"
#include	"../../Object/WinTTFont.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTonView;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:

// データ
public:
	int		m_nFirst;
	TCHAR	m_szKaisyaName[HRECMAX];
	int		m_nTTEisuFlag;
	CTonView* m_pcCrtView;
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CFBU0Font* m_pcFBU0Font;
	CWinTTFont* m_pcWNTTFont;
	GdiplusStartupInput m_gdiSI;
	ULONG_PTR           m_gdiToken;

// インプリメンテーション
public:
	void InitExePath(LPTSTR szExePath);
	void GetItemElement();
	void SetItemElement();
	void HideItemElement();
	void ShowItemElement();
	void InitItemElement();
	void HelpTextOut(int idx, LPTSTR msg);
	CTonView* GetCrtView();
	void EnvSave();
	void EnvLoad();
	void DivDrawEnvLoad();
	void ScrnEnvLoad();
	void MojiEnvLoad();
	void FiguEnvLoad();
	void FontEnvLoad();
	void WakuEnvLoad();
	void ScrnAddEnvLoad();
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

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

