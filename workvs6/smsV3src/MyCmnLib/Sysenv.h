// sysenv.h : Bk32 アプリケーションのメイン ヘッダー ファイル
//

#if !defined(SYSENV_H)
#define SYSENV_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CSysEnv:
// このクラスの動作の定義に関しては sysenv.cpp ファイルを参照してください。
//

class AFX_CLASS_EXPORT CSysEnv
{
public:
	CSysEnv();
	~CSysEnv();

public:
	void GetCmntFname(int idx, LPSTR fname1, LPSTR fname2);
	void GetTableListFname(int idx, LPSTR listfname);
	BOOL GetTablePartFname(LPSTR listfname, LPSTR tablename, LPSTR partfname);
	void GetTksgoFname(int idx, LPSTR fname);
	void GetPaperSize(int idx, LPDBL xsize, LPDBL ysize);
	void ScrnGetCntrXY(LPDBL cntrx, LPDBL cntry);
	void ScrnSetCntrXY(DBL cntrx, DBL cntry);
	int ScrnGetSubXY(LPDBL subx, LPDBL suby);
	void ScrnSetSubXY(DBL subx, DBL suby);
	void ScrnGetSubRag(LPDBL subrag);
	void ScrnSetSubRag(DBL subrag);
	int	ScrnOffSubFlag();
	void ScrnGetGrid(LPDBL grid);
	void ScrnSetGrid(DBL grid);
	void ScrnAdjustGrid(LPDBL dx, LPDBL dy);
	int ScrnCheckCntrlKey();
	void ScrnSetCntrlKey(int flag);
	int ScrnCheckShiftKey();
	void ScrnSetShiftKey(int flag);
	void SetExePath(LPSTR pcExePath);
	void GetExePath(LPSTR pcExePath);
	void SaveEnv();
	void LoadEnv();
	void SetCrtKihonNo(int nKihonNo);
	void GetCrtKihonNo(int *nKihonNo);
	void GetCrtHotel(LPSTR lpszHotel);
	void SetCrtHotel(LPSTR lpszHotel);

public:
	void GetTableMax(int idx, int *max);
	int m_nCrtKihonNo;
	char	m_szExePath[FNAMEMAX];
	int		m_nCntrlKey;
	int		m_nShiftKey;
	char	m_szUserName[RECMAX];		// ユーザー名
	char	m_szCrtHotel[RECMAX];		// 現在のホテル名
	DBL		m_dCntrX, m_dCntrY;
	DBL		m_dSubX, m_dSubY, m_dSubRag;
	int		m_nSubFlag;
	DBL		m_dGrid;
};

#endif // !defined(SYSENV_H)

