// funclib.h : Bk32 アプリケーションのメイン ヘッダー ファイル
//

#if !defined(FUNCLIB_H)
#define FUNCLIB_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CFuncLib:
// このクラスの動作の定義に関しては funclib.cpp ファイルを参照してください。
//

class AFX_CLASS_EXPORT CFuncLib
{
public:
	CFuncLib();
	~CFuncLib();

public:

public:
	int GetLastCharPos(char ch, LPSTR str);
	void ChangeSafix(LPSTR fname, LPSTR safixs);
	void AddSafix(LPSTR fname, LPSTR safixs);
	BOOL DelCRfgets(LPSTR lpBuf, int max, FILE *fp);
	void GetCrtDateStr(LPSTR lpDateStr);
	void GetCrtDate(int *yy, int *mm, int *dd, int *ww);
	void GetCrtTime(int *hh, int *min, int *ss);
	int IsKanji(int hc);
	int DivCsvData(LPSTR csvsp[], LPSTR csvbuf);
	void CreateDateStr(LPSTR lpDateStr, int yyyy, int mm, int dd, int hh, int min);
	void ConverteDateStr(LPSTR lpDateStr, int *yyyy, int *mm, int *dd, int *hh, int *min);
	void NameSetSapce(LPSTR name1, LPSTR name2, LPSTR name);
};

#endif // !defined(FUNCLIB_H)

