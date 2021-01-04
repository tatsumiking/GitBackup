
#pragma once


#include "../CmnSrc/DialogWakuInput.h"
#include "../CmnSrc/DialogWakuInSekiji.h"
#include "../CmnSrc/DialogSlctStrg.h"
#include "../CmnSrc/DialogCrctDisp.h"

class CCmdSWak : public CCmdBase
{
public:
	CCmdSWak(CScrollView* pcview);
	~CCmdSWak();
	UINT ExecCreate(UINT event, DBL dx, DBL dy);
	UINT ExecAnyInput(UINT event, DBL dx, DBL dy);
	UINT ExecAnyInSekiji(UINT event, DBL dx, DBL dy);
	UINT ExecWakuChange(UINT event, DBL dx, DBL dy);
public:
	CDialogWakuInput	*m_pcDialogWakuInput;
	CDialogWakuInSekiji	*m_pcDialogWakuInSekiji;
	CDialogSlctStrg	*m_pcDialogSlctStrg;
	CDialogCrctDisp *m_pcDialogCrctDisp;
	CSWakVect		*m_pcSWakVect;
	DBL		m_dWidth;
	DBL		m_dHight;
	DBL		m_dSTX;
	DBL		m_dSTY;
	int		m_nPno;
	CData	*m_pcData;
	TCHAR	m_szStrg[STRGMAX];
	TCHAR	m_szFont[RECMAX];
	TCHAR	m_szFildName[RECMAX];
	COLORREF	m_clrFill;
	int		m_nSpaceKind;	// �����g(SPACEWAKU), �f�[�^(SPACEDATA)
	int		m_nRoundKind;	// ����ŉ�]���邩���Ȃ���
	DBL		m_dFlatPar;		// ���́A���́i�����T�C�Y�ɑ΂��銄���j
	DBL		m_dSpacePar;	// �����Ԋu
	int		m_nTateYoko;
	DBL		m_dTCHARCount;
	DBL		m_dKanaPar;		// �Ђ炪�ȏk���p�[�Z���g
	int		m_nOderKind;	// �ϓ�(ODEREQUAL)�A����(ODERCNTR)�A����(ODERLEFT)�A�E��(ODERRIGHT)
	int		m_nYokoGumi;
	int		m_nFixFlag;
	int		m_nMirror;
	int		m_nShadowKind;
	COLORREF	m_clrShadow;
	DBL		m_dShadowDistance;
	DBL		m_dShadowRoundRag;
private:

public:
	void UpdateStringList(LPTSTR LPTSTR);
	void SetSWakLocate();
	void ResetAnyWakuAtr();
	void SetAnyWakuAtr();
	void SaveWakuString();
	void LoadWakuString();
	void SaveGroupString();
	void LoadGroupString();
	void ResetGrupNo();
	void SetSWakString(CSWakVect *pcSWakVect, LPTSTR lpSetStr);
	void SetFitingVectElement(CVect *pcVect);
	void GetFitingVectElement(int ntateyoko, DBL wd, DBL hi, CVect *pcVect);
	void CheckSetDataElement();
	void GetDataElement();
	void SetDataElement();
	void GetItemElement();
	void SetItemElement();
	void CreateSWakVect();
};

