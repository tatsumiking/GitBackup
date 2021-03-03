// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MAINFRM_H)
#define MAINFRM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"..\object\fsf1font.h"
#include	"..\object\esf0font.h"
#include	"..\object\esf1font.h"
#include	"..\object\wnttfont.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();

// アトリビュート
public:
	int	m_nFirst;
// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// データ
public:
	int		m_nMainExec;
	char	m_szKaisyaName[RECMAX];
	char	m_szEnvPath[FNAMEMAX];
	int		m_nTTEisuFlag;
	CTonView* m_pcCrtView;
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CWNTTFont* m_pcWNTTFont;

// インプリメンテーション
public:
	void GetItemElement();
	void SetItemElement();
	void HideItemElement();
	void ShowItemElement();
	void InitItemElement();
	void HelpTextOut(int idx, LPSTR msg);
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
	int CheckProtectFile();
	int	CheckProtectUsb();
	char* CrDelFgets(char *buf, int max, FILE *fp);
	void TrueTypeFontListSet(int idx);
	static BOOL CALLBACK AFX_EXPORT EnumFamSiftjisCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID lpAdrs);
	static BOOL CALLBACK AFX_EXPORT EnumFamAsciiCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID lpAdrs);
	void StrgSpaceDelCpy(LPSTR dststrg, LPSTR srcstrg);

// 生成されたメッセージ マップ関数
protected:
	void InitExePath(LPSTR szExePath);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(MAINFRM_H)
