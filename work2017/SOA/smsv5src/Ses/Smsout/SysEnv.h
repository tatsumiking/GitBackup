// sysenv.h : ES98 �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//


#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSysEnv:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� sysenv.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

#define	CHKPOINTMAX	5

class CSysEnv
{
public:
	CSysEnv();
public:
	void ScrnGetCoodMoveXY(LPDBL movex, LPDBL movey);
	void ScrnCnvMMToCood(LPDBL coodx, LPDBL coody);
	void ScrnCnvCoodToMM(LPDBL mmx, LPDBL mmy);
	void ScrnGetCntrXY(LPDBL cntrx, LPDBL cntry);
	void ScrnSetCntrXY(DBL cntrx, DBL cntry);
	int ScrnGetSubXY(LPDBL subx, LPDBL suby);
	void ScrnSetSubXY(DBL subx, DBL suby);
	void ScrnGetSubRag(LPDBL subrag);
	void ScrnSetSubRag(DBL subrag);
	int	ScrnOffSubFlag();
	void ScrnGetGrid(LPDBL grid);
	void ScrnSetGrid(DBL grid);
	void ScrnAdjustPoint(LPDBL dx, LPDBL dy);
	void ScrnAdjustGrid(LPDBL dx, LPDBL dy);
	int ScrnCheckCntrlKey();
	void ScrnSetCntrlKey(int flag);
	int ScrnCheckShiftKey();
	void ScrnSetShiftKey(int flag);
	void ScrnAllDrawCheckPoint(CDC *pcDC);
	void ScrnDrawCheckPoint(CDC *pcDC, int idx);
	void ScrnSetCheckPoint(int idx, DBL dx, DBL dy);
	void ScrnGetCheckPoint(int idx, LPDBL dx, LPDBL dy);
	int ScrnIncCheckPoint(int idx);

public:
	void ScrnOnOffGrid(int flag);
	int ScrnCheckGrid();
	DBL		m_dChkX[CHKPOINTMAX];
	DBL		m_dChkY[CHKPOINTMAX];
	DBL		m_dCntrX, m_dCntrY;
	DBL		m_dSubX, m_dSubY, m_dSubRag;
	int		m_nSubFlag;
	DBL		m_dGrid;
	int		m_nGridFlag;
	int		m_nCntrlKey;
	int		m_nShiftKey;

	// �����g�֌W��`
	DBL		m_dWidth;
	DBL		m_dHight;
	DBL		m_dSX;
	DBL		m_dSY;
	TCHAR	m_szStrg[RECMAX];
	TCHAR	m_szFont[RECMAX];
	TCHAR	m_szFildName[RECMAX];
	int		m_nTateYoko;
	DBL		m_dFlatPar;
	DBL		m_dSpaceMM;
	DBL		m_dKanaPar;
	int		m_nOderKind;	// �ϓ�(ODEREQUAL)�A����(ODERCNTR)�A����(ODERLEFT)�A�E��(ODERRIGHT)
	int		m_nSpaceKind;
	DBL		m_dCharCount;
	int		m_nYokoGumi;
	int		m_nFixFlag;
	int		m_nMirror;
	int		m_nShadowKind;
	COLORREF	m_clrShadow;
	DBL		m_dShadowDistance;
	DBL		m_dShadowRoundRag;


};


