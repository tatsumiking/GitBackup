// scrnenv.h : Bk32 アプリケーションのメイン ヘッダー ファイル
//

#if !defined(SCRNENV_H)
#define SCRNENV_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CScrnEnv:
// このクラスの動作の定義に関しては ScrnEnv.cpp ファイルを参照してください。
//

class AFX_CLASS_EXPORT CScrnEnv
{
public:
	CScrnEnv();
	~CScrnEnv();

public:

public:
	void TrnsLenMMToRltv(LPDBL len);
	void TrnsLenRltvToMM(LPDBL len);
	void TrnsXYMMToRltv(LPDBL xx, LPDBL yy);
	void TrnsXYRltvToMM(LPDBL xx, LPDBL yy);
	void SetWinArea(int sx, int sy, int ex, int ey);
	void InitSizeElement(LPINT w, LPINT h);
	void GetRltvTop(LPDBL rltvxtop, LPDBL rltvytop);
	void GetRltvSize(LPDBL rltvxsize, LPDBL rltvysize);
	void SetRltvArea(DBL sx, DBL sy, DBL ex, DBL ey);
	void SetRltvMove(DBL movex, DBL movey);
	void SetDispRltvMove(DBL movex, DBL movey);
	void SetRltvTop(DBL topx, DBL topy);
	void SetRltvElement();
	DBL SetXYTime(DBL xtime, DBL ytime);
	void GetMMSize(LPDBL xsize, LPDBL ysize);
	int SetDrawMode(int mode);
	int	CheckDrawMode();
	int	CheckUndo();
	void OnUndo();
	void OffUndo();
};

#endif // !defined(SCRNENV_H)

