
#pragma once


#include "../CmnSrc/DialogMojiSize.h"
#include "../CmnSrc/DialogMojiMove.h"
#include "../CmnSrc/DialogMojiFiting.h"
#include "../CmnSrc/DialogFontChange.h"

//C 文字操作関係コマンド処理クラス
class CCmdMoji : public CCmdBase
{
public:
	CCmdMoji(CScrollView* pcview);
	 ~CCmdMoji();

	UINT ExecMojiSize(UINT event, DBL dx, DBL dy);
	UINT ExecMojiRound(UINT event, DBL dx, DBL dy);
	UINT ExecMojiMove(UINT event, DBL dx, DBL dy);
	UINT ExecMojiFiting(UINT event, DBL dx, DBL dy);
	UINT ExecMojiLoad(UINT event, DBL dx, DBL dy);
	UINT ExecMojiSave(UINT event, DBL dx, DBL dy);
	UINT ExecFontChange(UINT event, DBL dx, DBL dy);
public:
	void CreateSetTomboData();
	void DeleteGrup(CDataGrup *pcDataGrup);
	void MojiSaveDelete(LPTSTR fontname, UINT sjis);
	UINT ExecMojiSaveDel(UINT event, DBL dx, DBL dy);
	void RoundMojiMain();
	void MoveMojiDown();
	void MoveMojiUp();
	void MoveMojiRight();
	void MoveMojiLeft();
	void SizeMojiBig();
	void SizeMojiSmall();
	void SizeMojiRLBig();
	void SizeMojiRLSmall();
	void SizeMojiUDBig();
	void SizeMojiUDSmall();
	void GetMojiSizeFromDialog();
	void SetMojiSizeToDialog();
	void LocateMojiSizeDlg();
	void LocateMojiMoveDlg();
	void LocateMojiFitingDlg();
	void LocateFontChangeDlg();
	void MojiSaveAtnVect(LPTSTR fontname, int code, DBL sx, DBL sy, DBL size);
	void AtntionDraw(CDC *pcDC);
	void InAreaDataAtntionOn();
	void MojiLoadSetVect(LPTSTR fontname, int code);
	BOOL MojiSaveMain();
	BOOL MojiLoadMain();
	BOOL MojiDelLoadMain();
	void ListResetLoop(CDataGrup *pcSetDataGrup, CDataList *pcGetDataList);
	void InitPointSet();
	void MovePointSet(DBL dx, DBL dy);
	void DrawBoxs(CDC *pcDC);
	void FlagCheckMojiChange(int flag);

private:
	CDialogMojiSize *m_pcDialogMojiSize;
	CDialogMojiMove *m_pcDialogMojiMove;
	CDialogMojiFiting *m_pcDialogMojiFiting;
	CDialogFontChange *m_pcDialogFontChange;
	CData*		m_pcData;
	CDataList*	m_pcDataList;
	CDataGrup*	m_pcDataGrup;
	DBL m_dWD;
	DBL m_dHI;
	DBL m_dStepmm;
	int m_nMjidx;
	TCHAR	m_szFontName[RECMAX];
	UINT	m_nCode;
};

