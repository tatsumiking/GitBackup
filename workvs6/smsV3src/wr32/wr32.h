// wr32.h : WR32 �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_WR32_H__5CDF5605_823E_11D3_BF1B_00A0B0086478__INCLUDED_)
#define AFX_WR32_H__5CDF5605_823E_11D3_BF1B_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CWr32App:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� wr32.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CWr32App : public CWinApp
{
public:
	CWr32App();
	~CWr32App();

public:
	CSysEnv	*m_pcSysEnv;
	CFuncLib	*m_pcFuncLib;
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CWr32App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CWr32App)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_WR32_H__5CDF5605_823E_11D3_BF1B_00A0B0086478__INCLUDED_)
