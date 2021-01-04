
#pragma once

class CFileVect
{
public:
	CFileVect(CScrollView* pcview);
	void LoadVectFile(LPTSTR fname);
	void LoadLogoFile(LPTSTR fname);
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
	int		m_nFontType;
	int		m_nFirst;
public:
	int m_nFontNo;
	void Trnsnxybittodxy(int nx, int ny, int xybit, DBL *dx, DBL *dy);
	void LoadLogoVect(LPTSTR filename);
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CFSF1Font* m_pcFSF1Font;
private:
	CData* LoadLogoData(FILE *fp, TCHAR buf[]);
	CData* LoadStrgData(FILE *fp, TCHAR buf[]);
	CData* LoadMojiData(FILE *fp, TCHAR buf[]);
	CData* LoadVectData(FILE *fp, TCHAR buf[]);
	CData* LoadWakuData(FILE *fp, TCHAR buf[]);
	CData* LoadOldVectData(FILE *fp, TCHAR buf[]);
	void FontFileOpen(TCHAR basename[]);
	void FontFileClose();
	int LoadFontData(int datano);
	LPTSTR FileFgets(TCHAR *buf, int bufmax, FILE *fp);
	int FileCommentChk(TCHAR *buf);
	void DibTrnsLogoFile(TCHAR *dotfname, TCHAR *bmpfname);

};

