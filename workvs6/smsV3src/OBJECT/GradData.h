
#ifndef __GRADDATA_H
#define __GRADDATA_H

#include "dibcdata.h"
// �����g�p�\��N���X�錾

#define	CLRMAX	128

class AFX_CLASS_EXPORT CGradData : public CDibCData
{
protected:
	CGradData();
	DECLARE_SERIAL(CGradData)
public:
	CGradData(int nDummy);
	~CGradData();
public:
	int	m_nKind;	// ����0,�~�`1
	DBL	m_dSX, m_dSY;	// �J�n�ʒu
	DBL	m_dLen;		// ����
	DBL	m_dRag;		// �����p�x
	int	m_nClrTblCount;	// �F�e�[�u���̌�
	DBL	m_dClrPos[CLRMAX];		// �F�̈ʒu(0�`100)
	DBL	m_dClrCtrlPos[CLRMAX];	// �O�̐F�Ƃ̏d�݈ʒu(0�`100)
	COLORREF	m_clrRGB[CLRMAX];	// �F���
public:

private:
	
public:
	virtual int CheckID();
	virtual void Serialize(CArchive& ar);
};

#endif

