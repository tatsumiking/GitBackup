

#pragma once


//C ファイル操作関係コマンド処理クラス
class CCmdFile : public CCmdBase
{
public:
	CDataGrup	*m_pcDataGrup;
	CDataList	*m_pcDataList;
	CVect	*m_pcVect;
	TCHAR	m_szInpSafixs[NRECMAX];
	TCHAR	m_szExpSafixs[NRECMAX];
	TCHAR	n_szInpPath[FNAMEMAX];
	TCHAR	n_szExpPath[FNAMEMAX];
	TCHAR	n_szDatPath[FNAMEMAX];
public:
	CCmdFile(CScrollView* pcview);
	~CCmdFile();
public:
	void DeleteCrctFile(LPTSTR lpFileName);
	void CopyCrctFile(LPTSTR lpSrcFname, LPTSTR lpDstFname);
	void ErrorMessageOut(CFileException *e);
	long CheckFreeSpace(LPTSTR path);
	void GetDirName(LPTSTR filename, LPTSTR dirname);
	void LoadDxfFile(LPTSTR fname);
	void SaveDxfFile(LPTSTR fname);
	void LoadCrtEnv(LPTSTR fname);
	void SaveCrtEnv(LPTSTR fname);
	void LoadArcFile(LPTSTR fname);
	int BitMapImageSave(CArchive& ar);
	void BitMapImageLoad(CArchive& ar);
	void SetFilter(LPTSTR addfilter, LPTSTR addsafixs);
	UINT ExecMyFileLoad(UINT event, DBL dx, DBL dy);
	UINT ExecMyFileSave(UINT event, DBL dx, DBL dy);
	UINT ExecAddEnvLoad(UINT event, DBL dx, DBL dy);
	UINT ExecAddLoad(UINT event, DBL dx, DBL dy);
	UINT ExecAddSave(UINT event, DBL dx, DBL dy);
	UINT ExecAddDel(UINT event, DBL dx, DBL dy);
	UINT ExecCrctLoad(UINT event, DBL dx, DBL dy);
	UINT ExecCrctSave(UINT event, DBL dx, DBL dy);
	UINT ExecCrctDel(UINT event, DBL dx, DBL dy);
	UINT ExecInport(UINT event, DBL dx, DBL dy);
	UINT ExecDelete(UINT event, DBL dx, DBL dy);
	UINT ExecExport(UINT event, DBL dx, DBL dy);
	void SaveEpsFile(LPTSTR fname);
	void LoadEpsFile(LPTSTR filename);
	void LoadLogoFile(LPTSTR filename);
	BOOL LoadInportFile();
	void LoadVectFile(LPTSTR filename);
	void LoadImageFile(LPTSTR fname);
	void LoadWMFFile(LPTSTR fname);
	void LoadEMFFile(LPTSTR fname);
	void MoveTrnsMain();
	void TrnsOverScreenData();
private:
	void SaveGyousya(LPTSTR lpGyousya);
	void SaveKouji(LPTSTR lpGyousya);
};

