
// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once

#include	"../../Object/FSF1Font.h"
#include	"../../Object/ESF0Font.h"
#include	"../../Object/ESF1Font.h"
#include	"../../Object/FBU0Font.h"
#include	"../../Object/WinTTFont.h"


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
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CFBU0Font* m_pcFBU0Font;
	CWinTTFont* m_pcWNTTFont;

// 操作
public:
	void InitExePath(LPTSTR szExePath);
	void EnvLoad();
	void EnvSave();
	void FontEnvLoad();

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:

protected:  // コントロール バー用メンバー
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};


