#ifndef __FILEVECT_H
#define __FILEVECT_H

class CFileVect
{
public:
	CFileVect(CScrollView* pcview);
	void LoadVectFile(LPSTR fname);
	void LoadLogoFile(LPSTR fname);
public:
	CTonView*	m_pcView;
	CTonDoc*	m_pcDoc;
	int		m_nFontType;
	int		m_nFirst;
public:
	int m_nFontNo;
	void Trnsnxybittodxy(int nx, int ny, int xybit, DBL *dx, DBL *dy);
	void LoadLogoVect(LPSTR filename);
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CFSF1Font* m_pcFSF1Font;
private:
	CData* LoadLogoData(FILE *fp, char buf[]);
	CData* LoadStrgData(FILE *fp, char buf[]);
	CData* LoadMojiData(FILE *fp, char buf[]);
	CData* LoadVectData(FILE *fp, char buf[]);
	CData* LoadWakuData(FILE *fp, char buf[]);
	CData* LoadRuleData(FILE *fp, char buf[]);
	CData* LoadOldVectData(FILE *fp, char buf[]);
	void FontFileOpen(char basename[]);
	void FontFileClose();
	int LoadFontData(int datano);
	LPSTR FileFgets(char *buf, int bufmax, FILE *fp);
	int FileCommentChk(char *buf);
	void DibTrnsLogoFile(char *dotfname, char *bmpfname);

};

#endif
