
#pragma once

// FontCBox.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CFontCBox �E�B���h�E

class CFontCBox : public CComboBox
{
// �R���X�g���N�V����
public:
	CFontCBox();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �C���v�������e�[�V����
public:
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void SetFontList();
	void LimitText(int nMaxTCHAR);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CFontCBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

