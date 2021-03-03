// ses.h : SES �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(SES_H)
#define SES_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CTonApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� es98.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CTonApp : public CWinApp
{
public:
	CTonApp();
	~CTonApp();

public:
	CSysEnv	*m_pcSysEnv;
	CDC m_dcScreen;
	int m_nDefFont;
	char	m_szArgv1[FNAMEMAX];
	char	m_szSavePath[FNAMEMAX];
	char	m_szSaveName[FNAMEMAX];
	int		m_nNoDispFlag;
	char	m_szBackImagePath[FNAMEMAX];
	char	m_szPhotoImagePath[FNAMEMAX];
	char	m_szTempImagePath[FNAMEMAX];
	char	m_szBackBmpName[FNAMEMAX];
	HANDLE	m_hTempStr;	// ��ƕ�����o�b�t�@�n���h��
	LPSTR	m_lpTempStr;
	COLORREF	m_clrBack;
	int		m_nMajor;
	int		m_nMinor;
	BOOL m_bWin4;
#ifndef _UNICODE
	BOOL m_bWin31;
#endif
	BOOL m_bLargeIcons;

public:
	void CreateWakuNameList(LPSTR lpWakuPath);
	void AppExit();
	HANDLE GetDevMode();
	HANDLE GetDevNames();
	int CheckEnumPrinter(HANDLE hDevNames);
	void SetDevNamesElement(LPSTR lpDevice, LPSTR lpDriver, LPSTR lpOutput);

	// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTonApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	COleTemplateServer m_server;
		// �h�L�������g���쐬���邽�߂̃T�[�o�[ �I�u�W�F�N�g

	//{{AFX_MSG(CTonApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFilePrintSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(SES_H)
