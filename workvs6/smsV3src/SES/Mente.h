#if !defined(_MENTE_H_)
#define _MENTE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mente.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CMente �E�B���h�E

class CMente
{
// �R���X�g���N�V����
public:
	CMente();
	virtual ~CMente();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �C���v�������e�[�V����
public:
	void SrchBackUpFolder(LPSTR lpBackupPath);
	void CopyFolder(LPSTR lpSrcPath, LPSTR lpDstPath);

};

#endif // !defined(_MENTE_H_)
