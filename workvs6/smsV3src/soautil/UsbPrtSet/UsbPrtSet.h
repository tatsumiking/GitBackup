// UsbPrtSet.h : USBPRTSET �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(USBPRTSET_H)
#define USBPRTSET_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CUsbPrtSetApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� UsbPrtSet.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CUsbPrtSetApp : public CWinApp
{
public:
	CUsbPrtSetApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CUsbPrtSetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CUsbPrtSetApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(USBPRTSET_H)
