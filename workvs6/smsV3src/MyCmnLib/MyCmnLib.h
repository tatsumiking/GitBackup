// MyCmnLib.h : MYCMNLIB �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_MYCMNLIB_H__D2A3DDEB_2404_11D3_A712_00A0B0086478__INCLUDED_)
#define AFX_MYCMNLIB_H__D2A3DDEB_2404_11D3_A712_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CMyCmnLibApp
// ���̃N���X�̓���̒�`�Ɋւ��Ă� MyCmnLib.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CMyCmnLibApp : public CWinApp
{
public:
	CMyCmnLibApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMyCmnLibApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMyCmnLibApp)
		// ���� -  ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//         ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MYCMNLIB_H__D2A3DDEB_2404_11D3_A712_00A0B0086478__INCLUDED_)
