
#pragma once


//G�@�T�v
//�@�I�u�W�F�N�g�ɑ΂������ȐF������ݒ肷��
//	���F�ݒ�A�J�b�g�ݒ�A�O���f�[�V�����ݒ�
//

#include "../../Object/DataFDib.h"
#include "../CmnSrc/DialogGradation.h"

//C �F�t���֌W�R�}���h�����N���X
class CCmdClr : public CCmdBase
{
public:
	CCmdClr(CScrollView* pcview);
	UINT ExecClrSet(UINT event, DBL dx, DBL dy);
	UINT ExecClrBackSet(UINT event, DBL dx, DBL dy);
	UINT ExecClrBackReset(UINT event, DBL dx, DBL dy);
	UINT ExecClrCutOnOff(UINT event, DBL dx, DBL dy);
	UINT ExecClrSlctCutOn(UINT event, DBL dx, DBL dy);
	UINT ExecClrSlctCutOff(UINT event, DBL dx, DBL dy);
	UINT ExecGradation(UINT event, DBL dx, DBL dy);
public:
	UINT ExecClrPicup(UINT event, DBL dx, DBL dy);
	void DataFDibCreate();
	void CreateGradationDib();
	void LocateGradationDlg();

private:
	CDialogGradation*	m_pcDialogGradation;
	TCHAR	m_szBmpFname[FNAMEMAX];
	CDataFDib*	m_pcDataFDib;

private:
};

