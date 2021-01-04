
#pragma once


#include "FontCBox.h"	// ClassView �ɂ���Ēǉ�����܂����B

// WakuTool.h : �w�b�_�[ �t�@�C��
//
/////////////////////////////////////////////////////////////////////////////
// CWakuTool �E�B���h�E

class CWakuTool : public CToolBar
{
// �R���X�g���N�V����
public:
	CWakuTool();
	virtual ~CWakuTool();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CWakuTool)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	CFontCBox m_cFontCBox;
	CEdit m_cEditStartX;
	CSpinButtonCtrl m_cSpinStartX;
	CEdit m_cEditStartY;
	CSpinButtonCtrl m_cSpinStartY;
	CEdit m_cEditWidth;
	CSpinButtonCtrl	m_cSpinWidth;
	CEdit m_cEditHight;
	CSpinButtonCtrl m_cSpinHight;
	CEdit m_cEditMojiCount;
	CSpinButtonCtrl m_cSpinMojiCount;

	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	BOOL SetToolItem(CSize textsize);
	void TateyokoChange();
	void TateYokoFlagBottonSet(int ftategaki);
	void CntrKindChange();
	void CntrKindBottnSet(int ncntrkind);
	void StartKindChange();
	void StartKindBottonSet(int nstartkind);
	void ConerKindChange();
	void ConerKindBottonSet(int nconerkind);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CWakuTool)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

