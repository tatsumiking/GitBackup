// sgs.h : SGS �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CTonApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� es98.cpp �t�@�C�����Q�Ƃ��Ă��������B
//
class CCharLib;
class CSysEnv;

class CTonApp : public CWinApp
{
public:
	CTonApp();
	~CTonApp();

public:
	CCharLib *m_pcCharLib;
	CSysEnv	*m_pcSysEnv;

	HANDLE	m_hEvent;
	CDC m_dcScreen;
	int m_nDefFont;
	TCHAR	m_szArgv1[FNAMEMAX];
	TCHAR	m_szSavePath[FNAMEMAX];
	TCHAR	m_szSaveName[FNAMEMAX];
	TCHAR	m_szBackImagePath[FNAMEMAX];
	HANDLE	m_hTempStr;	// ��ƕ�����o�b�t�@�n���h��
	LPTSTR	m_lpTempStr;
	COLORREF	m_clrBack;
	BOOL m_bWin4;
#ifndef _UNICODE
	BOOL m_bWin31;
#endif
	BOOL m_bLargeIcons;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTonApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	HANDLE GetDevMode();
	HANDLE GetDevNames();
	int CheckEnumPrinter(HANDLE hDevNames);
	void SetDevNamesElement(LPTSTR lpDevice, LPTSTR lpDriver, LPTSTR lpOutput);

	COleTemplateServer m_server;
		// �h�L�������g���쐬���邽�߂̃T�[�o�[ �I�u�W�F�N�g

	//{{AFX_MSG(CTonApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFilePrintSetup();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B


