// UsbPrtTest.h : USBPRTTEST �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_USBPRTTEST_H__3681B068_9F3E_4861_B3A7_EF6DE88A12AD__INCLUDED_)
#define AFX_USBPRTTEST_H__3681B068_9F3E_4861_B3A7_EF6DE88A12AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CTonApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� UsbPrtTest.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CTonApp : public CWinApp
{
public:
	CTonApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTonApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CTonApp)
	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_USBPRTTEST_H__3681B068_9F3E_4861_B3A7_EF6DE88A12AD__INCLUDED_)
