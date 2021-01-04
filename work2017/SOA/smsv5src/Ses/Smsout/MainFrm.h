// MainFrm.h : CMainFrame �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
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

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:

// �f�[�^
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

// �C���v�������e�[�V����
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

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

