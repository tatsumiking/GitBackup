#if !defined(DIMGSAVE_H)
#define DIMGSAVE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DImgSave.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogImageSave �_�C�A���O

class CDialogImageSave : public CFileDialog
{
	DECLARE_DYNAMIC(CDialogImageSave)

public:
	CDialogImageSave(LPSTR lpInitPath);
	~CDialogImageSave();

// �_�C�A���O �f�[�^
public:
	int	m_nXSize;
	int	m_nYSize;
	int	m_nQuality;
	int	m_nRotate;

// �I�[�o�[���C�h
	virtual BOOL OnFileNameOK();

// �C���v�������e�[�V����
public:
protected:
	//{{AFX_MSG(CDialogImageSave)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DIMGSAVE_H)
