// smsout.h : SMSOUT �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

class CSysEnv;
/////////////////////////////////////////////////////////////////////////////
// CTonApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� smsout.cpp �t�@�C�����Q�Ƃ��Ă��������B
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
	CDC		m_dcScreen;
	COLORREF	m_clrBack;

public:
	void SetupPrinter();
	void AppExit();
	HANDLE GetDevMode();
	HANDLE GetDevNames();
	int CheckEnumPrinter(HANDLE hDevNames);
	void SetDevNamesElement(LPTSTR lpDevice, LPTSTR lpDriver, LPTSTR lpOutput);
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTonApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CTonApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

