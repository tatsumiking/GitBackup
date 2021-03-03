
#ifndef __CMD_FILE_H
#define __CMD_FILE_H

//C ファイル操作関係コマンド処理クラス
class CCmdFile : public CCmdBase
{
public:
	CDataGrup	*m_pcDataGrup;
	CDataList	*m_pcDataList;
	CVect	*m_pcVect;
	char	m_szInpSafixs[NRECMAX];
	char	m_szExpSafixs[NRECMAX];
	char	m_szInpPath[FNAMEMAX];
	char	m_szExpPath[FNAMEMAX];
	char	m_szDatPath[FNAMEMAX];
public:
	CCmdFile(CScrollView* pcview);
	~CCmdFile();
public:
	void SaveEmfFile(LPSTR fname);
	void SaveBmpFile(LPSTR fname);
	void ToupperString(LPSTR safics);
	void ErrorMessageOut(CFileException *e);
	long CheckFreeSpace(LPSTR path);
	void GetDirName(LPSTR filename, LPSTR dirname);
	void LoadDxfFile(LPSTR fname);
	void SaveDxfFile(LPSTR fname);
	void LoadCrtEnv(LPSTR fname);
	void SaveCrtEnv(LPSTR fname);
	void LoadArcFile(LPSTR fname);
	void SetFilter(LPSTR addfilter, LPSTR addsafixs);
	UINT ExecMyFileLoad(UINT event, DBL dx, DBL dy);
	UINT ExecMyFileSave(UINT event, DBL dx, DBL dy);
	UINT ExecLoadVect(UINT event, DBL dx, DBL dy);
	UINT ExecAddEnvLoad(UINT event, DBL dx, DBL dy);
	UINT ExecAddLoad(UINT event, DBL dx, DBL dy);
	UINT ExecAddSave(UINT event, DBL dx, DBL dy);
	UINT ExecAddDel(UINT event, DBL dx, DBL dy);
	UINT ExecCrctLoad(UINT event, DBL dx, DBL dy);
	UINT ExecCrctSave(UINT event, DBL dx, DBL dy);
	UINT ExecCrctDel(UINT event, DBL dx, DBL dy);
	UINT ExecInport(UINT event, DBL dx, DBL dy);
	UINT ExecDelete(UINT event, DBL dx, DBL dy);
	void SaveEpsFile(LPSTR fname);
	UINT ExecExport(UINT event, DBL dx, DBL dy);
	void LoadEpsFile(LPSTR filename);
	void LoadLogoFile(LPSTR filename);
	BOOL LoadInportFile();
	void LoadVectFile(LPSTR filename);
	void LoadBmpFile(LPSTR fname);
	void LoadJPGFile(LPSTR fname);
	void LoadTIFFFile(LPSTR fname);
	void MoveTrnsMain();
	void TrnsOverScreenData();
private:
	void SaveGyousya(LPSTR lpGyousya);
	void SaveKouji(LPSTR lpGyousya);
};

#endif
