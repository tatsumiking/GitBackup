
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
	int		m_nSpaceKind;	// 文字枠(SPACEWAKU), データ(SPACEDATA)
	int		m_nRoundKind;	// 線上で回転するかしないか
	DBL		m_dFlatPar;		// 長体、平体（文字サイズに対する割合）
	DBL		m_dSpacePar;	// 文字間隔
	int		m_nTateYoko;
	DBL		m_dTCHARCount;
	DBL		m_dKanaPar;		// ひらがな縮小パーセント
	int		m_nOderKind;	// 均等(ODEREQUAL)、中央(ODERCNTR)、左上(ODERLEFT)、右下(ODERRIGHT)
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

